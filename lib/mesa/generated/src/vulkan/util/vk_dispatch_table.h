/*
 * Copyright 2020 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
 * IN NO EVENT SHALL VMWARE AND/OR ITS SUPPLIERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
/* This file generated from vk_dispatch_table_gen.py, don't edit directly. */

#ifndef VK_DISPATCH_TABLE_H
#define VK_DISPATCH_TABLE_H

#include "vulkan/vulkan.h"
#include "vulkan/vk_android_native_buffer.h"

#include "vk_extensions.h"

/* Windows api conflict */
#ifdef _WIN32
#include <windows.h>
#ifdef CreateSemaphore
#undef CreateSemaphore
#endif
#ifdef CreateEvent
#undef CreateEvent
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _MSC_VER
VKAPI_ATTR void VKAPI_CALL vk_entrypoint_stub(void);
#endif





struct vk_instance_dispatch_table {
  
    PFN_vkCreateInstance CreateInstance;
    PFN_vkDestroyInstance DestroyInstance;
    PFN_vkEnumeratePhysicalDevices EnumeratePhysicalDevices;
    PFN_vkGetInstanceProcAddr GetInstanceProcAddr;
    PFN_vkEnumerateInstanceVersion EnumerateInstanceVersion;
    PFN_vkEnumerateInstanceLayerProperties EnumerateInstanceLayerProperties;
    PFN_vkEnumerateInstanceExtensionProperties EnumerateInstanceExtensionProperties;
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    PFN_vkCreateAndroidSurfaceKHR CreateAndroidSurfaceKHR;
#else
    PFN_vkVoidFunction CreateAndroidSurfaceKHR;
#endif
    PFN_vkCreateDisplayPlaneSurfaceKHR CreateDisplayPlaneSurfaceKHR;
    PFN_vkDestroySurfaceKHR DestroySurfaceKHR;
#ifdef VK_USE_PLATFORM_VI_NN
    PFN_vkCreateViSurfaceNN CreateViSurfaceNN;
#else
    PFN_vkVoidFunction CreateViSurfaceNN;
#endif
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
    PFN_vkCreateWaylandSurfaceKHR CreateWaylandSurfaceKHR;
#else
    PFN_vkVoidFunction CreateWaylandSurfaceKHR;
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
    PFN_vkCreateWin32SurfaceKHR CreateWin32SurfaceKHR;
#else
    PFN_vkVoidFunction CreateWin32SurfaceKHR;
#endif
#ifdef VK_USE_PLATFORM_XLIB_KHR
    PFN_vkCreateXlibSurfaceKHR CreateXlibSurfaceKHR;
#else
    PFN_vkVoidFunction CreateXlibSurfaceKHR;
#endif
#ifdef VK_USE_PLATFORM_XCB_KHR
    PFN_vkCreateXcbSurfaceKHR CreateXcbSurfaceKHR;
#else
    PFN_vkVoidFunction CreateXcbSurfaceKHR;
#endif
#ifdef VK_USE_PLATFORM_DIRECTFB_EXT
    PFN_vkCreateDirectFBSurfaceEXT CreateDirectFBSurfaceEXT;
#else
    PFN_vkVoidFunction CreateDirectFBSurfaceEXT;
#endif
#ifdef VK_USE_PLATFORM_FUCHSIA
    PFN_vkCreateImagePipeSurfaceFUCHSIA CreateImagePipeSurfaceFUCHSIA;
#else
    PFN_vkVoidFunction CreateImagePipeSurfaceFUCHSIA;
#endif
#ifdef VK_USE_PLATFORM_GGP
    PFN_vkCreateStreamDescriptorSurfaceGGP CreateStreamDescriptorSurfaceGGP;
#else
    PFN_vkVoidFunction CreateStreamDescriptorSurfaceGGP;
#endif
#ifdef VK_USE_PLATFORM_SCREEN_QNX
    PFN_vkCreateScreenSurfaceQNX CreateScreenSurfaceQNX;
#else
    PFN_vkVoidFunction CreateScreenSurfaceQNX;
#endif
    PFN_vkCreateDebugReportCallbackEXT CreateDebugReportCallbackEXT;
    PFN_vkDestroyDebugReportCallbackEXT DestroyDebugReportCallbackEXT;
    PFN_vkDebugReportMessageEXT DebugReportMessageEXT;
    union {
        PFN_vkEnumeratePhysicalDeviceGroups EnumeratePhysicalDeviceGroups;
        PFN_vkEnumeratePhysicalDeviceGroupsKHR EnumeratePhysicalDeviceGroupsKHR;
    };
    #ifdef VK_USE_PLATFORM_IOS_MVK
    PFN_vkCreateIOSSurfaceMVK CreateIOSSurfaceMVK;
#else
    PFN_vkVoidFunction CreateIOSSurfaceMVK;
#endif
#ifdef VK_USE_PLATFORM_MACOS_MVK
    PFN_vkCreateMacOSSurfaceMVK CreateMacOSSurfaceMVK;
#else
    PFN_vkVoidFunction CreateMacOSSurfaceMVK;
#endif
#ifdef VK_USE_PLATFORM_METAL_EXT
    PFN_vkCreateMetalSurfaceEXT CreateMetalSurfaceEXT;
#else
    PFN_vkVoidFunction CreateMetalSurfaceEXT;
#endif
    PFN_vkCreateDebugUtilsMessengerEXT CreateDebugUtilsMessengerEXT;
    PFN_vkDestroyDebugUtilsMessengerEXT DestroyDebugUtilsMessengerEXT;
    PFN_vkSubmitDebugUtilsMessageEXT SubmitDebugUtilsMessageEXT;
    PFN_vkCreateHeadlessSurfaceEXT CreateHeadlessSurfaceEXT;

};

struct vk_physical_device_dispatch_table {
  
    PFN_vkGetPhysicalDeviceProperties GetPhysicalDeviceProperties;
    PFN_vkGetPhysicalDeviceQueueFamilyProperties GetPhysicalDeviceQueueFamilyProperties;
    PFN_vkGetPhysicalDeviceMemoryProperties GetPhysicalDeviceMemoryProperties;
    PFN_vkGetPhysicalDeviceFeatures GetPhysicalDeviceFeatures;
    PFN_vkGetPhysicalDeviceFormatProperties GetPhysicalDeviceFormatProperties;
    PFN_vkGetPhysicalDeviceImageFormatProperties GetPhysicalDeviceImageFormatProperties;
    PFN_vkCreateDevice CreateDevice;
    PFN_vkEnumerateDeviceLayerProperties EnumerateDeviceLayerProperties;
    PFN_vkEnumerateDeviceExtensionProperties EnumerateDeviceExtensionProperties;
    PFN_vkGetPhysicalDeviceSparseImageFormatProperties GetPhysicalDeviceSparseImageFormatProperties;
    PFN_vkGetPhysicalDeviceDisplayPropertiesKHR GetPhysicalDeviceDisplayPropertiesKHR;
    PFN_vkGetPhysicalDeviceDisplayPlanePropertiesKHR GetPhysicalDeviceDisplayPlanePropertiesKHR;
    PFN_vkGetDisplayPlaneSupportedDisplaysKHR GetDisplayPlaneSupportedDisplaysKHR;
    PFN_vkGetDisplayModePropertiesKHR GetDisplayModePropertiesKHR;
    PFN_vkCreateDisplayModeKHR CreateDisplayModeKHR;
    PFN_vkGetDisplayPlaneCapabilitiesKHR GetDisplayPlaneCapabilitiesKHR;
    PFN_vkGetPhysicalDeviceSurfaceSupportKHR GetPhysicalDeviceSurfaceSupportKHR;
    PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR GetPhysicalDeviceSurfaceCapabilitiesKHR;
    PFN_vkGetPhysicalDeviceSurfaceFormatsKHR GetPhysicalDeviceSurfaceFormatsKHR;
    PFN_vkGetPhysicalDeviceSurfacePresentModesKHR GetPhysicalDeviceSurfacePresentModesKHR;
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
    PFN_vkGetPhysicalDeviceWaylandPresentationSupportKHR GetPhysicalDeviceWaylandPresentationSupportKHR;
#else
    PFN_vkVoidFunction GetPhysicalDeviceWaylandPresentationSupportKHR;
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
    PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR GetPhysicalDeviceWin32PresentationSupportKHR;
#else
    PFN_vkVoidFunction GetPhysicalDeviceWin32PresentationSupportKHR;
#endif
#ifdef VK_USE_PLATFORM_XLIB_KHR
    PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR GetPhysicalDeviceXlibPresentationSupportKHR;
#else
    PFN_vkVoidFunction GetPhysicalDeviceXlibPresentationSupportKHR;
#endif
#ifdef VK_USE_PLATFORM_XCB_KHR
    PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR GetPhysicalDeviceXcbPresentationSupportKHR;
#else
    PFN_vkVoidFunction GetPhysicalDeviceXcbPresentationSupportKHR;
#endif
#ifdef VK_USE_PLATFORM_DIRECTFB_EXT
    PFN_vkGetPhysicalDeviceDirectFBPresentationSupportEXT GetPhysicalDeviceDirectFBPresentationSupportEXT;
#else
    PFN_vkVoidFunction GetPhysicalDeviceDirectFBPresentationSupportEXT;
#endif
#ifdef VK_USE_PLATFORM_SCREEN_QNX
    PFN_vkGetPhysicalDeviceScreenPresentationSupportQNX GetPhysicalDeviceScreenPresentationSupportQNX;
#else
    PFN_vkVoidFunction GetPhysicalDeviceScreenPresentationSupportQNX;
#endif
    PFN_vkGetPhysicalDeviceExternalImageFormatPropertiesNV GetPhysicalDeviceExternalImageFormatPropertiesNV;
    union {
        PFN_vkGetPhysicalDeviceFeatures2 GetPhysicalDeviceFeatures2;
        PFN_vkGetPhysicalDeviceFeatures2KHR GetPhysicalDeviceFeatures2KHR;
    };
        union {
        PFN_vkGetPhysicalDeviceProperties2 GetPhysicalDeviceProperties2;
        PFN_vkGetPhysicalDeviceProperties2KHR GetPhysicalDeviceProperties2KHR;
    };
        union {
        PFN_vkGetPhysicalDeviceFormatProperties2 GetPhysicalDeviceFormatProperties2;
        PFN_vkGetPhysicalDeviceFormatProperties2KHR GetPhysicalDeviceFormatProperties2KHR;
    };
        union {
        PFN_vkGetPhysicalDeviceImageFormatProperties2 GetPhysicalDeviceImageFormatProperties2;
        PFN_vkGetPhysicalDeviceImageFormatProperties2KHR GetPhysicalDeviceImageFormatProperties2KHR;
    };
        union {
        PFN_vkGetPhysicalDeviceQueueFamilyProperties2 GetPhysicalDeviceQueueFamilyProperties2;
        PFN_vkGetPhysicalDeviceQueueFamilyProperties2KHR GetPhysicalDeviceQueueFamilyProperties2KHR;
    };
        union {
        PFN_vkGetPhysicalDeviceMemoryProperties2 GetPhysicalDeviceMemoryProperties2;
        PFN_vkGetPhysicalDeviceMemoryProperties2KHR GetPhysicalDeviceMemoryProperties2KHR;
    };
        union {
        PFN_vkGetPhysicalDeviceSparseImageFormatProperties2 GetPhysicalDeviceSparseImageFormatProperties2;
        PFN_vkGetPhysicalDeviceSparseImageFormatProperties2KHR GetPhysicalDeviceSparseImageFormatProperties2KHR;
    };
        union {
        PFN_vkGetPhysicalDeviceExternalBufferProperties GetPhysicalDeviceExternalBufferProperties;
        PFN_vkGetPhysicalDeviceExternalBufferPropertiesKHR GetPhysicalDeviceExternalBufferPropertiesKHR;
    };
        union {
        PFN_vkGetPhysicalDeviceExternalSemaphoreProperties GetPhysicalDeviceExternalSemaphoreProperties;
        PFN_vkGetPhysicalDeviceExternalSemaphorePropertiesKHR GetPhysicalDeviceExternalSemaphorePropertiesKHR;
    };
        union {
        PFN_vkGetPhysicalDeviceExternalFenceProperties GetPhysicalDeviceExternalFenceProperties;
        PFN_vkGetPhysicalDeviceExternalFencePropertiesKHR GetPhysicalDeviceExternalFencePropertiesKHR;
    };
        PFN_vkReleaseDisplayEXT ReleaseDisplayEXT;
#ifdef VK_USE_PLATFORM_XLIB_XRANDR_EXT
    PFN_vkAcquireXlibDisplayEXT AcquireXlibDisplayEXT;
#else
    PFN_vkVoidFunction AcquireXlibDisplayEXT;
#endif
#ifdef VK_USE_PLATFORM_XLIB_XRANDR_EXT
    PFN_vkGetRandROutputDisplayEXT GetRandROutputDisplayEXT;
#else
    PFN_vkVoidFunction GetRandROutputDisplayEXT;
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
    PFN_vkAcquireWinrtDisplayNV AcquireWinrtDisplayNV;
#else
    PFN_vkVoidFunction AcquireWinrtDisplayNV;
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
    PFN_vkGetWinrtDisplayNV GetWinrtDisplayNV;
#else
    PFN_vkVoidFunction GetWinrtDisplayNV;
#endif
    PFN_vkGetPhysicalDeviceSurfaceCapabilities2EXT GetPhysicalDeviceSurfaceCapabilities2EXT;
    PFN_vkGetPhysicalDevicePresentRectanglesKHR GetPhysicalDevicePresentRectanglesKHR;
    PFN_vkGetPhysicalDeviceMultisamplePropertiesEXT GetPhysicalDeviceMultisamplePropertiesEXT;
    PFN_vkGetPhysicalDeviceSurfaceCapabilities2KHR GetPhysicalDeviceSurfaceCapabilities2KHR;
    PFN_vkGetPhysicalDeviceSurfaceFormats2KHR GetPhysicalDeviceSurfaceFormats2KHR;
    PFN_vkGetPhysicalDeviceDisplayProperties2KHR GetPhysicalDeviceDisplayProperties2KHR;
    PFN_vkGetPhysicalDeviceDisplayPlaneProperties2KHR GetPhysicalDeviceDisplayPlaneProperties2KHR;
    PFN_vkGetDisplayModeProperties2KHR GetDisplayModeProperties2KHR;
    PFN_vkGetDisplayPlaneCapabilities2KHR GetDisplayPlaneCapabilities2KHR;
    PFN_vkGetPhysicalDeviceCalibrateableTimeDomainsEXT GetPhysicalDeviceCalibrateableTimeDomainsEXT;
    PFN_vkGetPhysicalDeviceCooperativeMatrixPropertiesNV GetPhysicalDeviceCooperativeMatrixPropertiesNV;
#ifdef VK_USE_PLATFORM_WIN32_KHR
    PFN_vkGetPhysicalDeviceSurfacePresentModes2EXT GetPhysicalDeviceSurfacePresentModes2EXT;
#else
    PFN_vkVoidFunction GetPhysicalDeviceSurfacePresentModes2EXT;
#endif
    PFN_vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR EnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR;
    PFN_vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR GetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR;
    PFN_vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV GetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV;
    union {
        PFN_vkGetPhysicalDeviceToolProperties GetPhysicalDeviceToolProperties;
        PFN_vkGetPhysicalDeviceToolPropertiesEXT GetPhysicalDeviceToolPropertiesEXT;
    };
        PFN_vkGetPhysicalDeviceFragmentShadingRatesKHR GetPhysicalDeviceFragmentShadingRatesKHR;
    PFN_vkGetPhysicalDeviceVideoCapabilitiesKHR GetPhysicalDeviceVideoCapabilitiesKHR;
    PFN_vkGetPhysicalDeviceVideoFormatPropertiesKHR GetPhysicalDeviceVideoFormatPropertiesKHR;
    PFN_vkAcquireDrmDisplayEXT AcquireDrmDisplayEXT;
    PFN_vkGetDrmDisplayEXT GetDrmDisplayEXT;
    PFN_vkGetPhysicalDeviceOpticalFlowImageFormatsNV GetPhysicalDeviceOpticalFlowImageFormatsNV;

};

struct vk_device_dispatch_table {
  
    PFN_vkGetDeviceProcAddr GetDeviceProcAddr;
    PFN_vkDestroyDevice DestroyDevice;
    PFN_vkGetDeviceQueue GetDeviceQueue;
    PFN_vkQueueSubmit QueueSubmit;
    PFN_vkQueueWaitIdle QueueWaitIdle;
    PFN_vkDeviceWaitIdle DeviceWaitIdle;
    PFN_vkAllocateMemory AllocateMemory;
    PFN_vkFreeMemory FreeMemory;
    PFN_vkMapMemory MapMemory;
    PFN_vkUnmapMemory UnmapMemory;
    PFN_vkFlushMappedMemoryRanges FlushMappedMemoryRanges;
    PFN_vkInvalidateMappedMemoryRanges InvalidateMappedMemoryRanges;
    PFN_vkGetDeviceMemoryCommitment GetDeviceMemoryCommitment;
    PFN_vkGetBufferMemoryRequirements GetBufferMemoryRequirements;
    PFN_vkBindBufferMemory BindBufferMemory;
    PFN_vkGetImageMemoryRequirements GetImageMemoryRequirements;
    PFN_vkBindImageMemory BindImageMemory;
    PFN_vkGetImageSparseMemoryRequirements GetImageSparseMemoryRequirements;
    PFN_vkQueueBindSparse QueueBindSparse;
    PFN_vkCreateFence CreateFence;
    PFN_vkDestroyFence DestroyFence;
    PFN_vkResetFences ResetFences;
    PFN_vkGetFenceStatus GetFenceStatus;
    PFN_vkWaitForFences WaitForFences;
    PFN_vkCreateSemaphore CreateSemaphore;
    PFN_vkDestroySemaphore DestroySemaphore;
    PFN_vkCreateEvent CreateEvent;
    PFN_vkDestroyEvent DestroyEvent;
    PFN_vkGetEventStatus GetEventStatus;
    PFN_vkSetEvent SetEvent;
    PFN_vkResetEvent ResetEvent;
    PFN_vkCreateQueryPool CreateQueryPool;
    PFN_vkDestroyQueryPool DestroyQueryPool;
    PFN_vkGetQueryPoolResults GetQueryPoolResults;
    union {
        PFN_vkResetQueryPool ResetQueryPool;
        PFN_vkResetQueryPoolEXT ResetQueryPoolEXT;
    };
        PFN_vkCreateBuffer CreateBuffer;
    PFN_vkDestroyBuffer DestroyBuffer;
    PFN_vkCreateBufferView CreateBufferView;
    PFN_vkDestroyBufferView DestroyBufferView;
    PFN_vkCreateImage CreateImage;
    PFN_vkDestroyImage DestroyImage;
    PFN_vkGetImageSubresourceLayout GetImageSubresourceLayout;
    PFN_vkCreateImageView CreateImageView;
    PFN_vkDestroyImageView DestroyImageView;
    PFN_vkCreateShaderModule CreateShaderModule;
    PFN_vkDestroyShaderModule DestroyShaderModule;
    PFN_vkCreatePipelineCache CreatePipelineCache;
    PFN_vkDestroyPipelineCache DestroyPipelineCache;
    PFN_vkGetPipelineCacheData GetPipelineCacheData;
    PFN_vkMergePipelineCaches MergePipelineCaches;
    PFN_vkCreateGraphicsPipelines CreateGraphicsPipelines;
    PFN_vkCreateComputePipelines CreateComputePipelines;
    PFN_vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI GetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI;
    PFN_vkDestroyPipeline DestroyPipeline;
    PFN_vkCreatePipelineLayout CreatePipelineLayout;
    PFN_vkDestroyPipelineLayout DestroyPipelineLayout;
    PFN_vkCreateSampler CreateSampler;
    PFN_vkDestroySampler DestroySampler;
    PFN_vkCreateDescriptorSetLayout CreateDescriptorSetLayout;
    PFN_vkDestroyDescriptorSetLayout DestroyDescriptorSetLayout;
    PFN_vkCreateDescriptorPool CreateDescriptorPool;
    PFN_vkDestroyDescriptorPool DestroyDescriptorPool;
    PFN_vkResetDescriptorPool ResetDescriptorPool;
    PFN_vkAllocateDescriptorSets AllocateDescriptorSets;
    PFN_vkFreeDescriptorSets FreeDescriptorSets;
    PFN_vkUpdateDescriptorSets UpdateDescriptorSets;
    PFN_vkCreateFramebuffer CreateFramebuffer;
    PFN_vkDestroyFramebuffer DestroyFramebuffer;
    PFN_vkCreateRenderPass CreateRenderPass;
    PFN_vkDestroyRenderPass DestroyRenderPass;
    PFN_vkGetRenderAreaGranularity GetRenderAreaGranularity;
    PFN_vkCreateCommandPool CreateCommandPool;
    PFN_vkDestroyCommandPool DestroyCommandPool;
    PFN_vkResetCommandPool ResetCommandPool;
    PFN_vkAllocateCommandBuffers AllocateCommandBuffers;
    PFN_vkFreeCommandBuffers FreeCommandBuffers;
    PFN_vkBeginCommandBuffer BeginCommandBuffer;
    PFN_vkEndCommandBuffer EndCommandBuffer;
    PFN_vkResetCommandBuffer ResetCommandBuffer;
    PFN_vkCmdBindPipeline CmdBindPipeline;
    PFN_vkCmdSetViewport CmdSetViewport;
    PFN_vkCmdSetScissor CmdSetScissor;
    PFN_vkCmdSetLineWidth CmdSetLineWidth;
    PFN_vkCmdSetDepthBias CmdSetDepthBias;
    PFN_vkCmdSetBlendConstants CmdSetBlendConstants;
    PFN_vkCmdSetDepthBounds CmdSetDepthBounds;
    PFN_vkCmdSetStencilCompareMask CmdSetStencilCompareMask;
    PFN_vkCmdSetStencilWriteMask CmdSetStencilWriteMask;
    PFN_vkCmdSetStencilReference CmdSetStencilReference;
    PFN_vkCmdBindDescriptorSets CmdBindDescriptorSets;
    PFN_vkCmdBindIndexBuffer CmdBindIndexBuffer;
    PFN_vkCmdBindVertexBuffers CmdBindVertexBuffers;
    PFN_vkCmdDraw CmdDraw;
    PFN_vkCmdDrawIndexed CmdDrawIndexed;
    PFN_vkCmdDrawMultiEXT CmdDrawMultiEXT;
    PFN_vkCmdDrawMultiIndexedEXT CmdDrawMultiIndexedEXT;
    PFN_vkCmdDrawIndirect CmdDrawIndirect;
    PFN_vkCmdDrawIndexedIndirect CmdDrawIndexedIndirect;
    PFN_vkCmdDispatch CmdDispatch;
    PFN_vkCmdDispatchIndirect CmdDispatchIndirect;
    PFN_vkCmdSubpassShadingHUAWEI CmdSubpassShadingHUAWEI;
    PFN_vkCmdDrawClusterHUAWEI CmdDrawClusterHUAWEI;
    PFN_vkCmdDrawClusterIndirectHUAWEI CmdDrawClusterIndirectHUAWEI;
    PFN_vkCmdCopyBuffer CmdCopyBuffer;
    PFN_vkCmdCopyImage CmdCopyImage;
    PFN_vkCmdBlitImage CmdBlitImage;
    PFN_vkCmdCopyBufferToImage CmdCopyBufferToImage;
    PFN_vkCmdCopyImageToBuffer CmdCopyImageToBuffer;
    PFN_vkCmdCopyMemoryIndirectNV CmdCopyMemoryIndirectNV;
    PFN_vkCmdCopyMemoryToImageIndirectNV CmdCopyMemoryToImageIndirectNV;
    PFN_vkCmdUpdateBuffer CmdUpdateBuffer;
    PFN_vkCmdFillBuffer CmdFillBuffer;
    PFN_vkCmdClearColorImage CmdClearColorImage;
    PFN_vkCmdClearDepthStencilImage CmdClearDepthStencilImage;
    PFN_vkCmdClearAttachments CmdClearAttachments;
    PFN_vkCmdResolveImage CmdResolveImage;
    PFN_vkCmdSetEvent CmdSetEvent;
    PFN_vkCmdResetEvent CmdResetEvent;
    PFN_vkCmdWaitEvents CmdWaitEvents;
    PFN_vkCmdPipelineBarrier CmdPipelineBarrier;
    PFN_vkCmdBeginQuery CmdBeginQuery;
    PFN_vkCmdEndQuery CmdEndQuery;
    PFN_vkCmdBeginConditionalRenderingEXT CmdBeginConditionalRenderingEXT;
    PFN_vkCmdEndConditionalRenderingEXT CmdEndConditionalRenderingEXT;
    PFN_vkCmdResetQueryPool CmdResetQueryPool;
    PFN_vkCmdWriteTimestamp CmdWriteTimestamp;
    PFN_vkCmdCopyQueryPoolResults CmdCopyQueryPoolResults;
    PFN_vkCmdPushConstants CmdPushConstants;
    PFN_vkCmdBeginRenderPass CmdBeginRenderPass;
    PFN_vkCmdNextSubpass CmdNextSubpass;
    PFN_vkCmdEndRenderPass CmdEndRenderPass;
    PFN_vkCmdExecuteCommands CmdExecuteCommands;
    PFN_vkCreateSharedSwapchainsKHR CreateSharedSwapchainsKHR;
    PFN_vkCreateSwapchainKHR CreateSwapchainKHR;
    PFN_vkDestroySwapchainKHR DestroySwapchainKHR;
    PFN_vkGetSwapchainImagesKHR GetSwapchainImagesKHR;
    PFN_vkAcquireNextImageKHR AcquireNextImageKHR;
    PFN_vkQueuePresentKHR QueuePresentKHR;
    PFN_vkDebugMarkerSetObjectNameEXT DebugMarkerSetObjectNameEXT;
    PFN_vkDebugMarkerSetObjectTagEXT DebugMarkerSetObjectTagEXT;
    PFN_vkCmdDebugMarkerBeginEXT CmdDebugMarkerBeginEXT;
    PFN_vkCmdDebugMarkerEndEXT CmdDebugMarkerEndEXT;
    PFN_vkCmdDebugMarkerInsertEXT CmdDebugMarkerInsertEXT;
#ifdef VK_USE_PLATFORM_WIN32_KHR
    PFN_vkGetMemoryWin32HandleNV GetMemoryWin32HandleNV;
#else
    PFN_vkVoidFunction GetMemoryWin32HandleNV;
#endif
    PFN_vkCmdExecuteGeneratedCommandsNV CmdExecuteGeneratedCommandsNV;
    PFN_vkCmdPreprocessGeneratedCommandsNV CmdPreprocessGeneratedCommandsNV;
    PFN_vkCmdBindPipelineShaderGroupNV CmdBindPipelineShaderGroupNV;
    PFN_vkGetGeneratedCommandsMemoryRequirementsNV GetGeneratedCommandsMemoryRequirementsNV;
    PFN_vkCreateIndirectCommandsLayoutNV CreateIndirectCommandsLayoutNV;
    PFN_vkDestroyIndirectCommandsLayoutNV DestroyIndirectCommandsLayoutNV;
    PFN_vkCmdPushDescriptorSetKHR CmdPushDescriptorSetKHR;
    union {
        PFN_vkTrimCommandPool TrimCommandPool;
        PFN_vkTrimCommandPoolKHR TrimCommandPoolKHR;
    };
    #ifdef VK_USE_PLATFORM_WIN32_KHR
    PFN_vkGetMemoryWin32HandleKHR GetMemoryWin32HandleKHR;
#else
    PFN_vkVoidFunction GetMemoryWin32HandleKHR;
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
    PFN_vkGetMemoryWin32HandlePropertiesKHR GetMemoryWin32HandlePropertiesKHR;
#else
    PFN_vkVoidFunction GetMemoryWin32HandlePropertiesKHR;
#endif
    PFN_vkGetMemoryFdKHR GetMemoryFdKHR;
    PFN_vkGetMemoryFdPropertiesKHR GetMemoryFdPropertiesKHR;
#ifdef VK_USE_PLATFORM_FUCHSIA
    PFN_vkGetMemoryZirconHandleFUCHSIA GetMemoryZirconHandleFUCHSIA;
#else
    PFN_vkVoidFunction GetMemoryZirconHandleFUCHSIA;
#endif
#ifdef VK_USE_PLATFORM_FUCHSIA
    PFN_vkGetMemoryZirconHandlePropertiesFUCHSIA GetMemoryZirconHandlePropertiesFUCHSIA;
#else
    PFN_vkVoidFunction GetMemoryZirconHandlePropertiesFUCHSIA;
#endif
    PFN_vkGetMemoryRemoteAddressNV GetMemoryRemoteAddressNV;
#ifdef VK_USE_PLATFORM_WIN32_KHR
    PFN_vkGetSemaphoreWin32HandleKHR GetSemaphoreWin32HandleKHR;
#else
    PFN_vkVoidFunction GetSemaphoreWin32HandleKHR;
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
    PFN_vkImportSemaphoreWin32HandleKHR ImportSemaphoreWin32HandleKHR;
#else
    PFN_vkVoidFunction ImportSemaphoreWin32HandleKHR;
#endif
    PFN_vkGetSemaphoreFdKHR GetSemaphoreFdKHR;
    PFN_vkImportSemaphoreFdKHR ImportSemaphoreFdKHR;
#ifdef VK_USE_PLATFORM_FUCHSIA
    PFN_vkGetSemaphoreZirconHandleFUCHSIA GetSemaphoreZirconHandleFUCHSIA;
#else
    PFN_vkVoidFunction GetSemaphoreZirconHandleFUCHSIA;
#endif
#ifdef VK_USE_PLATFORM_FUCHSIA
    PFN_vkImportSemaphoreZirconHandleFUCHSIA ImportSemaphoreZirconHandleFUCHSIA;
#else
    PFN_vkVoidFunction ImportSemaphoreZirconHandleFUCHSIA;
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
    PFN_vkGetFenceWin32HandleKHR GetFenceWin32HandleKHR;
#else
    PFN_vkVoidFunction GetFenceWin32HandleKHR;
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
    PFN_vkImportFenceWin32HandleKHR ImportFenceWin32HandleKHR;
#else
    PFN_vkVoidFunction ImportFenceWin32HandleKHR;
#endif
    PFN_vkGetFenceFdKHR GetFenceFdKHR;
    PFN_vkImportFenceFdKHR ImportFenceFdKHR;
    PFN_vkDisplayPowerControlEXT DisplayPowerControlEXT;
    PFN_vkRegisterDeviceEventEXT RegisterDeviceEventEXT;
    PFN_vkRegisterDisplayEventEXT RegisterDisplayEventEXT;
    PFN_vkGetSwapchainCounterEXT GetSwapchainCounterEXT;
    union {
        PFN_vkGetDeviceGroupPeerMemoryFeatures GetDeviceGroupPeerMemoryFeatures;
        PFN_vkGetDeviceGroupPeerMemoryFeaturesKHR GetDeviceGroupPeerMemoryFeaturesKHR;
    };
        union {
        PFN_vkBindBufferMemory2 BindBufferMemory2;
        PFN_vkBindBufferMemory2KHR BindBufferMemory2KHR;
    };
        union {
        PFN_vkBindImageMemory2 BindImageMemory2;
        PFN_vkBindImageMemory2KHR BindImageMemory2KHR;
    };
        union {
        PFN_vkCmdSetDeviceMask CmdSetDeviceMask;
        PFN_vkCmdSetDeviceMaskKHR CmdSetDeviceMaskKHR;
    };
        PFN_vkGetDeviceGroupPresentCapabilitiesKHR GetDeviceGroupPresentCapabilitiesKHR;
    PFN_vkGetDeviceGroupSurfacePresentModesKHR GetDeviceGroupSurfacePresentModesKHR;
    PFN_vkAcquireNextImage2KHR AcquireNextImage2KHR;
    union {
        PFN_vkCmdDispatchBase CmdDispatchBase;
        PFN_vkCmdDispatchBaseKHR CmdDispatchBaseKHR;
    };
        union {
        PFN_vkCreateDescriptorUpdateTemplate CreateDescriptorUpdateTemplate;
        PFN_vkCreateDescriptorUpdateTemplateKHR CreateDescriptorUpdateTemplateKHR;
    };
        union {
        PFN_vkDestroyDescriptorUpdateTemplate DestroyDescriptorUpdateTemplate;
        PFN_vkDestroyDescriptorUpdateTemplateKHR DestroyDescriptorUpdateTemplateKHR;
    };
        union {
        PFN_vkUpdateDescriptorSetWithTemplate UpdateDescriptorSetWithTemplate;
        PFN_vkUpdateDescriptorSetWithTemplateKHR UpdateDescriptorSetWithTemplateKHR;
    };
        PFN_vkCmdPushDescriptorSetWithTemplateKHR CmdPushDescriptorSetWithTemplateKHR;
    PFN_vkSetHdrMetadataEXT SetHdrMetadataEXT;
    PFN_vkGetSwapchainStatusKHR GetSwapchainStatusKHR;
    PFN_vkGetRefreshCycleDurationGOOGLE GetRefreshCycleDurationGOOGLE;
    PFN_vkGetPastPresentationTimingGOOGLE GetPastPresentationTimingGOOGLE;
    PFN_vkCmdSetViewportWScalingNV CmdSetViewportWScalingNV;
    PFN_vkCmdSetDiscardRectangleEXT CmdSetDiscardRectangleEXT;
    PFN_vkCmdSetDiscardRectangleEnableEXT CmdSetDiscardRectangleEnableEXT;
    PFN_vkCmdSetDiscardRectangleModeEXT CmdSetDiscardRectangleModeEXT;
    PFN_vkCmdSetSampleLocationsEXT CmdSetSampleLocationsEXT;
    union {
        PFN_vkGetBufferMemoryRequirements2 GetBufferMemoryRequirements2;
        PFN_vkGetBufferMemoryRequirements2KHR GetBufferMemoryRequirements2KHR;
    };
        union {
        PFN_vkGetImageMemoryRequirements2 GetImageMemoryRequirements2;
        PFN_vkGetImageMemoryRequirements2KHR GetImageMemoryRequirements2KHR;
    };
        union {
        PFN_vkGetImageSparseMemoryRequirements2 GetImageSparseMemoryRequirements2;
        PFN_vkGetImageSparseMemoryRequirements2KHR GetImageSparseMemoryRequirements2KHR;
    };
        union {
        PFN_vkGetDeviceBufferMemoryRequirements GetDeviceBufferMemoryRequirements;
        PFN_vkGetDeviceBufferMemoryRequirementsKHR GetDeviceBufferMemoryRequirementsKHR;
    };
        union {
        PFN_vkGetDeviceImageMemoryRequirements GetDeviceImageMemoryRequirements;
        PFN_vkGetDeviceImageMemoryRequirementsKHR GetDeviceImageMemoryRequirementsKHR;
    };
        union {
        PFN_vkGetDeviceImageSparseMemoryRequirements GetDeviceImageSparseMemoryRequirements;
        PFN_vkGetDeviceImageSparseMemoryRequirementsKHR GetDeviceImageSparseMemoryRequirementsKHR;
    };
        union {
        PFN_vkCreateSamplerYcbcrConversion CreateSamplerYcbcrConversion;
        PFN_vkCreateSamplerYcbcrConversionKHR CreateSamplerYcbcrConversionKHR;
    };
        union {
        PFN_vkDestroySamplerYcbcrConversion DestroySamplerYcbcrConversion;
        PFN_vkDestroySamplerYcbcrConversionKHR DestroySamplerYcbcrConversionKHR;
    };
        PFN_vkGetDeviceQueue2 GetDeviceQueue2;
    PFN_vkCreateValidationCacheEXT CreateValidationCacheEXT;
    PFN_vkDestroyValidationCacheEXT DestroyValidationCacheEXT;
    PFN_vkGetValidationCacheDataEXT GetValidationCacheDataEXT;
    PFN_vkMergeValidationCachesEXT MergeValidationCachesEXT;
    union {
        PFN_vkGetDescriptorSetLayoutSupport GetDescriptorSetLayoutSupport;
        PFN_vkGetDescriptorSetLayoutSupportKHR GetDescriptorSetLayoutSupportKHR;
    };
    #ifdef VK_USE_PLATFORM_ANDROID_KHR
    PFN_vkGetSwapchainGrallocUsageANDROID GetSwapchainGrallocUsageANDROID;
#else
    PFN_vkVoidFunction GetSwapchainGrallocUsageANDROID;
#endif
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    PFN_vkGetSwapchainGrallocUsage2ANDROID GetSwapchainGrallocUsage2ANDROID;
#else
    PFN_vkVoidFunction GetSwapchainGrallocUsage2ANDROID;
#endif
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    PFN_vkAcquireImageANDROID AcquireImageANDROID;
#else
    PFN_vkVoidFunction AcquireImageANDROID;
#endif
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    PFN_vkQueueSignalReleaseImageANDROID QueueSignalReleaseImageANDROID;
#else
    PFN_vkVoidFunction QueueSignalReleaseImageANDROID;
#endif
    PFN_vkGetShaderInfoAMD GetShaderInfoAMD;
    PFN_vkSetLocalDimmingAMD SetLocalDimmingAMD;
    PFN_vkGetCalibratedTimestampsEXT GetCalibratedTimestampsEXT;
    PFN_vkSetDebugUtilsObjectNameEXT SetDebugUtilsObjectNameEXT;
    PFN_vkSetDebugUtilsObjectTagEXT SetDebugUtilsObjectTagEXT;
    PFN_vkQueueBeginDebugUtilsLabelEXT QueueBeginDebugUtilsLabelEXT;
    PFN_vkQueueEndDebugUtilsLabelEXT QueueEndDebugUtilsLabelEXT;
    PFN_vkQueueInsertDebugUtilsLabelEXT QueueInsertDebugUtilsLabelEXT;
    PFN_vkCmdBeginDebugUtilsLabelEXT CmdBeginDebugUtilsLabelEXT;
    PFN_vkCmdEndDebugUtilsLabelEXT CmdEndDebugUtilsLabelEXT;
    PFN_vkCmdInsertDebugUtilsLabelEXT CmdInsertDebugUtilsLabelEXT;
    PFN_vkGetMemoryHostPointerPropertiesEXT GetMemoryHostPointerPropertiesEXT;
    PFN_vkCmdWriteBufferMarkerAMD CmdWriteBufferMarkerAMD;
    union {
        PFN_vkCreateRenderPass2 CreateRenderPass2;
        PFN_vkCreateRenderPass2KHR CreateRenderPass2KHR;
    };
        union {
        PFN_vkCmdBeginRenderPass2 CmdBeginRenderPass2;
        PFN_vkCmdBeginRenderPass2KHR CmdBeginRenderPass2KHR;
    };
        union {
        PFN_vkCmdNextSubpass2 CmdNextSubpass2;
        PFN_vkCmdNextSubpass2KHR CmdNextSubpass2KHR;
    };
        union {
        PFN_vkCmdEndRenderPass2 CmdEndRenderPass2;
        PFN_vkCmdEndRenderPass2KHR CmdEndRenderPass2KHR;
    };
        union {
        PFN_vkGetSemaphoreCounterValue GetSemaphoreCounterValue;
        PFN_vkGetSemaphoreCounterValueKHR GetSemaphoreCounterValueKHR;
    };
        union {
        PFN_vkWaitSemaphores WaitSemaphores;
        PFN_vkWaitSemaphoresKHR WaitSemaphoresKHR;
    };
        union {
        PFN_vkSignalSemaphore SignalSemaphore;
        PFN_vkSignalSemaphoreKHR SignalSemaphoreKHR;
    };
    #ifdef VK_USE_PLATFORM_ANDROID_KHR
    PFN_vkGetAndroidHardwareBufferPropertiesANDROID GetAndroidHardwareBufferPropertiesANDROID;
#else
    PFN_vkVoidFunction GetAndroidHardwareBufferPropertiesANDROID;
#endif
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    PFN_vkGetMemoryAndroidHardwareBufferANDROID GetMemoryAndroidHardwareBufferANDROID;
#else
    PFN_vkVoidFunction GetMemoryAndroidHardwareBufferANDROID;
#endif
    union {
        PFN_vkCmdDrawIndirectCount CmdDrawIndirectCount;
        PFN_vkCmdDrawIndirectCountKHR CmdDrawIndirectCountKHR;
        PFN_vkCmdDrawIndirectCountAMD CmdDrawIndirectCountAMD;
    };
            union {
        PFN_vkCmdDrawIndexedIndirectCount CmdDrawIndexedIndirectCount;
        PFN_vkCmdDrawIndexedIndirectCountKHR CmdDrawIndexedIndirectCountKHR;
        PFN_vkCmdDrawIndexedIndirectCountAMD CmdDrawIndexedIndirectCountAMD;
    };
            PFN_vkCmdSetCheckpointNV CmdSetCheckpointNV;
    PFN_vkGetQueueCheckpointDataNV GetQueueCheckpointDataNV;
    PFN_vkCmdBindTransformFeedbackBuffersEXT CmdBindTransformFeedbackBuffersEXT;
    PFN_vkCmdBeginTransformFeedbackEXT CmdBeginTransformFeedbackEXT;
    PFN_vkCmdEndTransformFeedbackEXT CmdEndTransformFeedbackEXT;
    PFN_vkCmdBeginQueryIndexedEXT CmdBeginQueryIndexedEXT;
    PFN_vkCmdEndQueryIndexedEXT CmdEndQueryIndexedEXT;
    PFN_vkCmdDrawIndirectByteCountEXT CmdDrawIndirectByteCountEXT;
    PFN_vkCmdSetExclusiveScissorNV CmdSetExclusiveScissorNV;
    PFN_vkCmdSetExclusiveScissorEnableNV CmdSetExclusiveScissorEnableNV;
    PFN_vkCmdBindShadingRateImageNV CmdBindShadingRateImageNV;
    PFN_vkCmdSetViewportShadingRatePaletteNV CmdSetViewportShadingRatePaletteNV;
    PFN_vkCmdSetCoarseSampleOrderNV CmdSetCoarseSampleOrderNV;
    PFN_vkCmdDrawMeshTasksNV CmdDrawMeshTasksNV;
    PFN_vkCmdDrawMeshTasksIndirectNV CmdDrawMeshTasksIndirectNV;
    PFN_vkCmdDrawMeshTasksIndirectCountNV CmdDrawMeshTasksIndirectCountNV;
    PFN_vkCmdDrawMeshTasksEXT CmdDrawMeshTasksEXT;
    PFN_vkCmdDrawMeshTasksIndirectEXT CmdDrawMeshTasksIndirectEXT;
    PFN_vkCmdDrawMeshTasksIndirectCountEXT CmdDrawMeshTasksIndirectCountEXT;
    PFN_vkCompileDeferredNV CompileDeferredNV;
    PFN_vkCreateAccelerationStructureNV CreateAccelerationStructureNV;
    PFN_vkCmdBindInvocationMaskHUAWEI CmdBindInvocationMaskHUAWEI;
    PFN_vkDestroyAccelerationStructureKHR DestroyAccelerationStructureKHR;
    PFN_vkDestroyAccelerationStructureNV DestroyAccelerationStructureNV;
    PFN_vkGetAccelerationStructureMemoryRequirementsNV GetAccelerationStructureMemoryRequirementsNV;
    PFN_vkBindAccelerationStructureMemoryNV BindAccelerationStructureMemoryNV;
    PFN_vkCmdCopyAccelerationStructureNV CmdCopyAccelerationStructureNV;
    PFN_vkCmdCopyAccelerationStructureKHR CmdCopyAccelerationStructureKHR;
    PFN_vkCopyAccelerationStructureKHR CopyAccelerationStructureKHR;
    PFN_vkCmdCopyAccelerationStructureToMemoryKHR CmdCopyAccelerationStructureToMemoryKHR;
    PFN_vkCopyAccelerationStructureToMemoryKHR CopyAccelerationStructureToMemoryKHR;
    PFN_vkCmdCopyMemoryToAccelerationStructureKHR CmdCopyMemoryToAccelerationStructureKHR;
    PFN_vkCopyMemoryToAccelerationStructureKHR CopyMemoryToAccelerationStructureKHR;
    PFN_vkCmdWriteAccelerationStructuresPropertiesKHR CmdWriteAccelerationStructuresPropertiesKHR;
    PFN_vkCmdWriteAccelerationStructuresPropertiesNV CmdWriteAccelerationStructuresPropertiesNV;
    PFN_vkCmdBuildAccelerationStructureNV CmdBuildAccelerationStructureNV;
    PFN_vkWriteAccelerationStructuresPropertiesKHR WriteAccelerationStructuresPropertiesKHR;
    PFN_vkCmdTraceRaysKHR CmdTraceRaysKHR;
    PFN_vkCmdTraceRaysNV CmdTraceRaysNV;
    union {
        PFN_vkGetRayTracingShaderGroupHandlesKHR GetRayTracingShaderGroupHandlesKHR;
        PFN_vkGetRayTracingShaderGroupHandlesNV GetRayTracingShaderGroupHandlesNV;
    };
        PFN_vkGetRayTracingCaptureReplayShaderGroupHandlesKHR GetRayTracingCaptureReplayShaderGroupHandlesKHR;
    PFN_vkGetAccelerationStructureHandleNV GetAccelerationStructureHandleNV;
    PFN_vkCreateRayTracingPipelinesNV CreateRayTracingPipelinesNV;
    PFN_vkCreateRayTracingPipelinesKHR CreateRayTracingPipelinesKHR;
    PFN_vkCmdTraceRaysIndirectKHR CmdTraceRaysIndirectKHR;
    PFN_vkCmdTraceRaysIndirect2KHR CmdTraceRaysIndirect2KHR;
    PFN_vkGetDeviceAccelerationStructureCompatibilityKHR GetDeviceAccelerationStructureCompatibilityKHR;
    PFN_vkGetRayTracingShaderGroupStackSizeKHR GetRayTracingShaderGroupStackSizeKHR;
    PFN_vkCmdSetRayTracingPipelineStackSizeKHR CmdSetRayTracingPipelineStackSizeKHR;
    PFN_vkGetImageViewHandleNVX GetImageViewHandleNVX;
    PFN_vkGetImageViewAddressNVX GetImageViewAddressNVX;
#ifdef VK_USE_PLATFORM_WIN32_KHR
    PFN_vkGetDeviceGroupSurfacePresentModes2EXT GetDeviceGroupSurfacePresentModes2EXT;
#else
    PFN_vkVoidFunction GetDeviceGroupSurfacePresentModes2EXT;
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
    PFN_vkAcquireFullScreenExclusiveModeEXT AcquireFullScreenExclusiveModeEXT;
#else
    PFN_vkVoidFunction AcquireFullScreenExclusiveModeEXT;
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
    PFN_vkReleaseFullScreenExclusiveModeEXT ReleaseFullScreenExclusiveModeEXT;
#else
    PFN_vkVoidFunction ReleaseFullScreenExclusiveModeEXT;
#endif
    PFN_vkAcquireProfilingLockKHR AcquireProfilingLockKHR;
    PFN_vkReleaseProfilingLockKHR ReleaseProfilingLockKHR;
    PFN_vkGetImageDrmFormatModifierPropertiesEXT GetImageDrmFormatModifierPropertiesEXT;
    union {
        PFN_vkGetBufferOpaqueCaptureAddress GetBufferOpaqueCaptureAddress;
        PFN_vkGetBufferOpaqueCaptureAddressKHR GetBufferOpaqueCaptureAddressKHR;
    };
        union {
        PFN_vkGetBufferDeviceAddress GetBufferDeviceAddress;
        PFN_vkGetBufferDeviceAddressKHR GetBufferDeviceAddressKHR;
        PFN_vkGetBufferDeviceAddressEXT GetBufferDeviceAddressEXT;
    };
            PFN_vkInitializePerformanceApiINTEL InitializePerformanceApiINTEL;
    PFN_vkUninitializePerformanceApiINTEL UninitializePerformanceApiINTEL;
    PFN_vkCmdSetPerformanceMarkerINTEL CmdSetPerformanceMarkerINTEL;
    PFN_vkCmdSetPerformanceStreamMarkerINTEL CmdSetPerformanceStreamMarkerINTEL;
    PFN_vkCmdSetPerformanceOverrideINTEL CmdSetPerformanceOverrideINTEL;
    PFN_vkAcquirePerformanceConfigurationINTEL AcquirePerformanceConfigurationINTEL;
    PFN_vkReleasePerformanceConfigurationINTEL ReleasePerformanceConfigurationINTEL;
    PFN_vkQueueSetPerformanceConfigurationINTEL QueueSetPerformanceConfigurationINTEL;
    PFN_vkGetPerformanceParameterINTEL GetPerformanceParameterINTEL;
    union {
        PFN_vkGetDeviceMemoryOpaqueCaptureAddress GetDeviceMemoryOpaqueCaptureAddress;
        PFN_vkGetDeviceMemoryOpaqueCaptureAddressKHR GetDeviceMemoryOpaqueCaptureAddressKHR;
    };
        PFN_vkGetPipelineExecutablePropertiesKHR GetPipelineExecutablePropertiesKHR;
    PFN_vkGetPipelineExecutableStatisticsKHR GetPipelineExecutableStatisticsKHR;
    PFN_vkGetPipelineExecutableInternalRepresentationsKHR GetPipelineExecutableInternalRepresentationsKHR;
    PFN_vkCmdSetLineStippleEXT CmdSetLineStippleEXT;
    PFN_vkCreateAccelerationStructureKHR CreateAccelerationStructureKHR;
    PFN_vkCmdBuildAccelerationStructuresKHR CmdBuildAccelerationStructuresKHR;
    PFN_vkCmdBuildAccelerationStructuresIndirectKHR CmdBuildAccelerationStructuresIndirectKHR;
    PFN_vkBuildAccelerationStructuresKHR BuildAccelerationStructuresKHR;
    PFN_vkGetAccelerationStructureDeviceAddressKHR GetAccelerationStructureDeviceAddressKHR;
    PFN_vkCreateDeferredOperationKHR CreateDeferredOperationKHR;
    PFN_vkDestroyDeferredOperationKHR DestroyDeferredOperationKHR;
    PFN_vkGetDeferredOperationMaxConcurrencyKHR GetDeferredOperationMaxConcurrencyKHR;
    PFN_vkGetDeferredOperationResultKHR GetDeferredOperationResultKHR;
    PFN_vkDeferredOperationJoinKHR DeferredOperationJoinKHR;
    union {
        PFN_vkCmdSetCullMode CmdSetCullMode;
        PFN_vkCmdSetCullModeEXT CmdSetCullModeEXT;
    };
        union {
        PFN_vkCmdSetFrontFace CmdSetFrontFace;
        PFN_vkCmdSetFrontFaceEXT CmdSetFrontFaceEXT;
    };
        union {
        PFN_vkCmdSetPrimitiveTopology CmdSetPrimitiveTopology;
        PFN_vkCmdSetPrimitiveTopologyEXT CmdSetPrimitiveTopologyEXT;
    };
        union {
        PFN_vkCmdSetViewportWithCount CmdSetViewportWithCount;
        PFN_vkCmdSetViewportWithCountEXT CmdSetViewportWithCountEXT;
    };
        union {
        PFN_vkCmdSetScissorWithCount CmdSetScissorWithCount;
        PFN_vkCmdSetScissorWithCountEXT CmdSetScissorWithCountEXT;
    };
        union {
        PFN_vkCmdBindVertexBuffers2 CmdBindVertexBuffers2;
        PFN_vkCmdBindVertexBuffers2EXT CmdBindVertexBuffers2EXT;
    };
        union {
        PFN_vkCmdSetDepthTestEnable CmdSetDepthTestEnable;
        PFN_vkCmdSetDepthTestEnableEXT CmdSetDepthTestEnableEXT;
    };
        union {
        PFN_vkCmdSetDepthWriteEnable CmdSetDepthWriteEnable;
        PFN_vkCmdSetDepthWriteEnableEXT CmdSetDepthWriteEnableEXT;
    };
        union {
        PFN_vkCmdSetDepthCompareOp CmdSetDepthCompareOp;
        PFN_vkCmdSetDepthCompareOpEXT CmdSetDepthCompareOpEXT;
    };
        union {
        PFN_vkCmdSetDepthBoundsTestEnable CmdSetDepthBoundsTestEnable;
        PFN_vkCmdSetDepthBoundsTestEnableEXT CmdSetDepthBoundsTestEnableEXT;
    };
        union {
        PFN_vkCmdSetStencilTestEnable CmdSetStencilTestEnable;
        PFN_vkCmdSetStencilTestEnableEXT CmdSetStencilTestEnableEXT;
    };
        union {
        PFN_vkCmdSetStencilOp CmdSetStencilOp;
        PFN_vkCmdSetStencilOpEXT CmdSetStencilOpEXT;
    };
        PFN_vkCmdSetPatchControlPointsEXT CmdSetPatchControlPointsEXT;
    union {
        PFN_vkCmdSetRasterizerDiscardEnable CmdSetRasterizerDiscardEnable;
        PFN_vkCmdSetRasterizerDiscardEnableEXT CmdSetRasterizerDiscardEnableEXT;
    };
        union {
        PFN_vkCmdSetDepthBiasEnable CmdSetDepthBiasEnable;
        PFN_vkCmdSetDepthBiasEnableEXT CmdSetDepthBiasEnableEXT;
    };
        PFN_vkCmdSetLogicOpEXT CmdSetLogicOpEXT;
    union {
        PFN_vkCmdSetPrimitiveRestartEnable CmdSetPrimitiveRestartEnable;
        PFN_vkCmdSetPrimitiveRestartEnableEXT CmdSetPrimitiveRestartEnableEXT;
    };
        PFN_vkCmdSetTessellationDomainOriginEXT CmdSetTessellationDomainOriginEXT;
    PFN_vkCmdSetDepthClampEnableEXT CmdSetDepthClampEnableEXT;
    PFN_vkCmdSetPolygonModeEXT CmdSetPolygonModeEXT;
    PFN_vkCmdSetRasterizationSamplesEXT CmdSetRasterizationSamplesEXT;
    PFN_vkCmdSetSampleMaskEXT CmdSetSampleMaskEXT;
    PFN_vkCmdSetAlphaToCoverageEnableEXT CmdSetAlphaToCoverageEnableEXT;
    PFN_vkCmdSetAlphaToOneEnableEXT CmdSetAlphaToOneEnableEXT;
    PFN_vkCmdSetLogicOpEnableEXT CmdSetLogicOpEnableEXT;
    PFN_vkCmdSetColorBlendEnableEXT CmdSetColorBlendEnableEXT;
    PFN_vkCmdSetColorBlendEquationEXT CmdSetColorBlendEquationEXT;
    PFN_vkCmdSetColorWriteMaskEXT CmdSetColorWriteMaskEXT;
    PFN_vkCmdSetRasterizationStreamEXT CmdSetRasterizationStreamEXT;
    PFN_vkCmdSetConservativeRasterizationModeEXT CmdSetConservativeRasterizationModeEXT;
    PFN_vkCmdSetExtraPrimitiveOverestimationSizeEXT CmdSetExtraPrimitiveOverestimationSizeEXT;
    PFN_vkCmdSetDepthClipEnableEXT CmdSetDepthClipEnableEXT;
    PFN_vkCmdSetSampleLocationsEnableEXT CmdSetSampleLocationsEnableEXT;
    PFN_vkCmdSetColorBlendAdvancedEXT CmdSetColorBlendAdvancedEXT;
    PFN_vkCmdSetProvokingVertexModeEXT CmdSetProvokingVertexModeEXT;
    PFN_vkCmdSetLineRasterizationModeEXT CmdSetLineRasterizationModeEXT;
    PFN_vkCmdSetLineStippleEnableEXT CmdSetLineStippleEnableEXT;
    PFN_vkCmdSetDepthClipNegativeOneToOneEXT CmdSetDepthClipNegativeOneToOneEXT;
    PFN_vkCmdSetViewportWScalingEnableNV CmdSetViewportWScalingEnableNV;
    PFN_vkCmdSetViewportSwizzleNV CmdSetViewportSwizzleNV;
    PFN_vkCmdSetCoverageToColorEnableNV CmdSetCoverageToColorEnableNV;
    PFN_vkCmdSetCoverageToColorLocationNV CmdSetCoverageToColorLocationNV;
    PFN_vkCmdSetCoverageModulationModeNV CmdSetCoverageModulationModeNV;
    PFN_vkCmdSetCoverageModulationTableEnableNV CmdSetCoverageModulationTableEnableNV;
    PFN_vkCmdSetCoverageModulationTableNV CmdSetCoverageModulationTableNV;
    PFN_vkCmdSetShadingRateImageEnableNV CmdSetShadingRateImageEnableNV;
    PFN_vkCmdSetCoverageReductionModeNV CmdSetCoverageReductionModeNV;
    PFN_vkCmdSetRepresentativeFragmentTestEnableNV CmdSetRepresentativeFragmentTestEnableNV;
    union {
        PFN_vkCreatePrivateDataSlot CreatePrivateDataSlot;
        PFN_vkCreatePrivateDataSlotEXT CreatePrivateDataSlotEXT;
    };
        union {
        PFN_vkDestroyPrivateDataSlot DestroyPrivateDataSlot;
        PFN_vkDestroyPrivateDataSlotEXT DestroyPrivateDataSlotEXT;
    };
        union {
        PFN_vkSetPrivateData SetPrivateData;
        PFN_vkSetPrivateDataEXT SetPrivateDataEXT;
    };
        union {
        PFN_vkGetPrivateData GetPrivateData;
        PFN_vkGetPrivateDataEXT GetPrivateDataEXT;
    };
        union {
        PFN_vkCmdCopyBuffer2 CmdCopyBuffer2;
        PFN_vkCmdCopyBuffer2KHR CmdCopyBuffer2KHR;
    };
        union {
        PFN_vkCmdCopyImage2 CmdCopyImage2;
        PFN_vkCmdCopyImage2KHR CmdCopyImage2KHR;
    };
        union {
        PFN_vkCmdBlitImage2 CmdBlitImage2;
        PFN_vkCmdBlitImage2KHR CmdBlitImage2KHR;
    };
        union {
        PFN_vkCmdCopyBufferToImage2 CmdCopyBufferToImage2;
        PFN_vkCmdCopyBufferToImage2KHR CmdCopyBufferToImage2KHR;
    };
        union {
        PFN_vkCmdCopyImageToBuffer2 CmdCopyImageToBuffer2;
        PFN_vkCmdCopyImageToBuffer2KHR CmdCopyImageToBuffer2KHR;
    };
        union {
        PFN_vkCmdResolveImage2 CmdResolveImage2;
        PFN_vkCmdResolveImage2KHR CmdResolveImage2KHR;
    };
        PFN_vkCmdSetFragmentShadingRateKHR CmdSetFragmentShadingRateKHR;
    PFN_vkCmdSetFragmentShadingRateEnumNV CmdSetFragmentShadingRateEnumNV;
    PFN_vkGetAccelerationStructureBuildSizesKHR GetAccelerationStructureBuildSizesKHR;
    PFN_vkCmdSetVertexInputEXT CmdSetVertexInputEXT;
    PFN_vkCmdSetColorWriteEnableEXT CmdSetColorWriteEnableEXT;
    union {
        PFN_vkCmdSetEvent2 CmdSetEvent2;
        PFN_vkCmdSetEvent2KHR CmdSetEvent2KHR;
    };
        union {
        PFN_vkCmdResetEvent2 CmdResetEvent2;
        PFN_vkCmdResetEvent2KHR CmdResetEvent2KHR;
    };
        union {
        PFN_vkCmdWaitEvents2 CmdWaitEvents2;
        PFN_vkCmdWaitEvents2KHR CmdWaitEvents2KHR;
    };
        union {
        PFN_vkCmdPipelineBarrier2 CmdPipelineBarrier2;
        PFN_vkCmdPipelineBarrier2KHR CmdPipelineBarrier2KHR;
    };
        union {
        PFN_vkQueueSubmit2 QueueSubmit2;
        PFN_vkQueueSubmit2KHR QueueSubmit2KHR;
    };
        union {
        PFN_vkCmdWriteTimestamp2 CmdWriteTimestamp2;
        PFN_vkCmdWriteTimestamp2KHR CmdWriteTimestamp2KHR;
    };
        PFN_vkCmdWriteBufferMarker2AMD CmdWriteBufferMarker2AMD;
    PFN_vkGetQueueCheckpointData2NV GetQueueCheckpointData2NV;
    PFN_vkCreateVideoSessionKHR CreateVideoSessionKHR;
    PFN_vkDestroyVideoSessionKHR DestroyVideoSessionKHR;
    PFN_vkCreateVideoSessionParametersKHR CreateVideoSessionParametersKHR;
    PFN_vkUpdateVideoSessionParametersKHR UpdateVideoSessionParametersKHR;
    PFN_vkDestroyVideoSessionParametersKHR DestroyVideoSessionParametersKHR;
    PFN_vkGetVideoSessionMemoryRequirementsKHR GetVideoSessionMemoryRequirementsKHR;
    PFN_vkBindVideoSessionMemoryKHR BindVideoSessionMemoryKHR;
    PFN_vkCmdDecodeVideoKHR CmdDecodeVideoKHR;
    PFN_vkCmdBeginVideoCodingKHR CmdBeginVideoCodingKHR;
    PFN_vkCmdControlVideoCodingKHR CmdControlVideoCodingKHR;
    PFN_vkCmdEndVideoCodingKHR CmdEndVideoCodingKHR;
    PFN_vkCmdDecompressMemoryNV CmdDecompressMemoryNV;
    PFN_vkCmdDecompressMemoryIndirectCountNV CmdDecompressMemoryIndirectCountNV;
    PFN_vkCreateCuModuleNVX CreateCuModuleNVX;
    PFN_vkCreateCuFunctionNVX CreateCuFunctionNVX;
    PFN_vkDestroyCuModuleNVX DestroyCuModuleNVX;
    PFN_vkDestroyCuFunctionNVX DestroyCuFunctionNVX;
    PFN_vkCmdCuLaunchKernelNVX CmdCuLaunchKernelNVX;
    PFN_vkGetDescriptorSetLayoutSizeEXT GetDescriptorSetLayoutSizeEXT;
    PFN_vkGetDescriptorSetLayoutBindingOffsetEXT GetDescriptorSetLayoutBindingOffsetEXT;
    PFN_vkGetDescriptorEXT GetDescriptorEXT;
    PFN_vkCmdBindDescriptorBuffersEXT CmdBindDescriptorBuffersEXT;
    PFN_vkCmdSetDescriptorBufferOffsetsEXT CmdSetDescriptorBufferOffsetsEXT;
    PFN_vkCmdBindDescriptorBufferEmbeddedSamplersEXT CmdBindDescriptorBufferEmbeddedSamplersEXT;
    PFN_vkGetBufferOpaqueCaptureDescriptorDataEXT GetBufferOpaqueCaptureDescriptorDataEXT;
    PFN_vkGetImageOpaqueCaptureDescriptorDataEXT GetImageOpaqueCaptureDescriptorDataEXT;
    PFN_vkGetImageViewOpaqueCaptureDescriptorDataEXT GetImageViewOpaqueCaptureDescriptorDataEXT;
    PFN_vkGetSamplerOpaqueCaptureDescriptorDataEXT GetSamplerOpaqueCaptureDescriptorDataEXT;
    PFN_vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT GetAccelerationStructureOpaqueCaptureDescriptorDataEXT;
    PFN_vkSetDeviceMemoryPriorityEXT SetDeviceMemoryPriorityEXT;
    PFN_vkWaitForPresentKHR WaitForPresentKHR;
#ifdef VK_USE_PLATFORM_FUCHSIA
    PFN_vkCreateBufferCollectionFUCHSIA CreateBufferCollectionFUCHSIA;
#else
    PFN_vkVoidFunction CreateBufferCollectionFUCHSIA;
#endif
#ifdef VK_USE_PLATFORM_FUCHSIA
    PFN_vkSetBufferCollectionBufferConstraintsFUCHSIA SetBufferCollectionBufferConstraintsFUCHSIA;
#else
    PFN_vkVoidFunction SetBufferCollectionBufferConstraintsFUCHSIA;
#endif
#ifdef VK_USE_PLATFORM_FUCHSIA
    PFN_vkSetBufferCollectionImageConstraintsFUCHSIA SetBufferCollectionImageConstraintsFUCHSIA;
#else
    PFN_vkVoidFunction SetBufferCollectionImageConstraintsFUCHSIA;
#endif
#ifdef VK_USE_PLATFORM_FUCHSIA
    PFN_vkDestroyBufferCollectionFUCHSIA DestroyBufferCollectionFUCHSIA;
#else
    PFN_vkVoidFunction DestroyBufferCollectionFUCHSIA;
#endif
#ifdef VK_USE_PLATFORM_FUCHSIA
    PFN_vkGetBufferCollectionPropertiesFUCHSIA GetBufferCollectionPropertiesFUCHSIA;
#else
    PFN_vkVoidFunction GetBufferCollectionPropertiesFUCHSIA;
#endif
    union {
        PFN_vkCmdBeginRendering CmdBeginRendering;
        PFN_vkCmdBeginRenderingKHR CmdBeginRenderingKHR;
    };
        union {
        PFN_vkCmdEndRendering CmdEndRendering;
        PFN_vkCmdEndRenderingKHR CmdEndRenderingKHR;
    };
        PFN_vkGetDescriptorSetLayoutHostMappingInfoVALVE GetDescriptorSetLayoutHostMappingInfoVALVE;
    PFN_vkGetDescriptorSetHostMappingVALVE GetDescriptorSetHostMappingVALVE;
    PFN_vkCreateMicromapEXT CreateMicromapEXT;
    PFN_vkCmdBuildMicromapsEXT CmdBuildMicromapsEXT;
    PFN_vkBuildMicromapsEXT BuildMicromapsEXT;
    PFN_vkDestroyMicromapEXT DestroyMicromapEXT;
    PFN_vkCmdCopyMicromapEXT CmdCopyMicromapEXT;
    PFN_vkCopyMicromapEXT CopyMicromapEXT;
    PFN_vkCmdCopyMicromapToMemoryEXT CmdCopyMicromapToMemoryEXT;
    PFN_vkCopyMicromapToMemoryEXT CopyMicromapToMemoryEXT;
    PFN_vkCmdCopyMemoryToMicromapEXT CmdCopyMemoryToMicromapEXT;
    PFN_vkCopyMemoryToMicromapEXT CopyMemoryToMicromapEXT;
    PFN_vkCmdWriteMicromapsPropertiesEXT CmdWriteMicromapsPropertiesEXT;
    PFN_vkWriteMicromapsPropertiesEXT WriteMicromapsPropertiesEXT;
    PFN_vkGetDeviceMicromapCompatibilityEXT GetDeviceMicromapCompatibilityEXT;
    PFN_vkGetMicromapBuildSizesEXT GetMicromapBuildSizesEXT;
    PFN_vkGetShaderModuleIdentifierEXT GetShaderModuleIdentifierEXT;
    PFN_vkGetShaderModuleCreateInfoIdentifierEXT GetShaderModuleCreateInfoIdentifierEXT;
    PFN_vkGetImageSubresourceLayout2EXT GetImageSubresourceLayout2EXT;
    PFN_vkGetPipelinePropertiesEXT GetPipelinePropertiesEXT;
#ifdef VK_USE_PLATFORM_METAL_EXT
    PFN_vkExportMetalObjectsEXT ExportMetalObjectsEXT;
#else
    PFN_vkVoidFunction ExportMetalObjectsEXT;
#endif
    PFN_vkGetFramebufferTilePropertiesQCOM GetFramebufferTilePropertiesQCOM;
    PFN_vkGetDynamicRenderingTilePropertiesQCOM GetDynamicRenderingTilePropertiesQCOM;
    PFN_vkCreateOpticalFlowSessionNV CreateOpticalFlowSessionNV;
    PFN_vkDestroyOpticalFlowSessionNV DestroyOpticalFlowSessionNV;
    PFN_vkBindOpticalFlowSessionImageNV BindOpticalFlowSessionImageNV;
    PFN_vkCmdOpticalFlowExecuteNV CmdOpticalFlowExecuteNV;
    PFN_vkGetDeviceFaultInfoEXT GetDeviceFaultInfoEXT;
    PFN_vkReleaseSwapchainImagesEXT ReleaseSwapchainImagesEXT;
    PFN_vkMapMemory2KHR MapMemory2KHR;
    PFN_vkUnmapMemory2KHR UnmapMemory2KHR;
    PFN_vkCreateShadersEXT CreateShadersEXT;
    PFN_vkDestroyShaderEXT DestroyShaderEXT;
    PFN_vkGetShaderBinaryDataEXT GetShaderBinaryDataEXT;
    PFN_vkCmdBindShadersEXT CmdBindShadersEXT;

};

struct vk_dispatch_table {
    union {
        struct {
            struct vk_instance_dispatch_table instance;
            struct vk_physical_device_dispatch_table physical_device;
            struct vk_device_dispatch_table device;
        };

        struct {
            
    PFN_vkCreateInstance CreateInstance;
    PFN_vkDestroyInstance DestroyInstance;
    PFN_vkEnumeratePhysicalDevices EnumeratePhysicalDevices;
    PFN_vkGetInstanceProcAddr GetInstanceProcAddr;
    PFN_vkEnumerateInstanceVersion EnumerateInstanceVersion;
    PFN_vkEnumerateInstanceLayerProperties EnumerateInstanceLayerProperties;
    PFN_vkEnumerateInstanceExtensionProperties EnumerateInstanceExtensionProperties;
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    PFN_vkCreateAndroidSurfaceKHR CreateAndroidSurfaceKHR;
#else
    PFN_vkVoidFunction CreateAndroidSurfaceKHR;
#endif
    PFN_vkCreateDisplayPlaneSurfaceKHR CreateDisplayPlaneSurfaceKHR;
    PFN_vkDestroySurfaceKHR DestroySurfaceKHR;
#ifdef VK_USE_PLATFORM_VI_NN
    PFN_vkCreateViSurfaceNN CreateViSurfaceNN;
#else
    PFN_vkVoidFunction CreateViSurfaceNN;
#endif
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
    PFN_vkCreateWaylandSurfaceKHR CreateWaylandSurfaceKHR;
#else
    PFN_vkVoidFunction CreateWaylandSurfaceKHR;
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
    PFN_vkCreateWin32SurfaceKHR CreateWin32SurfaceKHR;
#else
    PFN_vkVoidFunction CreateWin32SurfaceKHR;
#endif
#ifdef VK_USE_PLATFORM_XLIB_KHR
    PFN_vkCreateXlibSurfaceKHR CreateXlibSurfaceKHR;
#else
    PFN_vkVoidFunction CreateXlibSurfaceKHR;
#endif
#ifdef VK_USE_PLATFORM_XCB_KHR
    PFN_vkCreateXcbSurfaceKHR CreateXcbSurfaceKHR;
#else
    PFN_vkVoidFunction CreateXcbSurfaceKHR;
#endif
#ifdef VK_USE_PLATFORM_DIRECTFB_EXT
    PFN_vkCreateDirectFBSurfaceEXT CreateDirectFBSurfaceEXT;
#else
    PFN_vkVoidFunction CreateDirectFBSurfaceEXT;
#endif
#ifdef VK_USE_PLATFORM_FUCHSIA
    PFN_vkCreateImagePipeSurfaceFUCHSIA CreateImagePipeSurfaceFUCHSIA;
#else
    PFN_vkVoidFunction CreateImagePipeSurfaceFUCHSIA;
#endif
#ifdef VK_USE_PLATFORM_GGP
    PFN_vkCreateStreamDescriptorSurfaceGGP CreateStreamDescriptorSurfaceGGP;
#else
    PFN_vkVoidFunction CreateStreamDescriptorSurfaceGGP;
#endif
#ifdef VK_USE_PLATFORM_SCREEN_QNX
    PFN_vkCreateScreenSurfaceQNX CreateScreenSurfaceQNX;
#else
    PFN_vkVoidFunction CreateScreenSurfaceQNX;
#endif
    PFN_vkCreateDebugReportCallbackEXT CreateDebugReportCallbackEXT;
    PFN_vkDestroyDebugReportCallbackEXT DestroyDebugReportCallbackEXT;
    PFN_vkDebugReportMessageEXT DebugReportMessageEXT;
    union {
        PFN_vkEnumeratePhysicalDeviceGroups EnumeratePhysicalDeviceGroups;
        PFN_vkEnumeratePhysicalDeviceGroupsKHR EnumeratePhysicalDeviceGroupsKHR;
    };
    #ifdef VK_USE_PLATFORM_IOS_MVK
    PFN_vkCreateIOSSurfaceMVK CreateIOSSurfaceMVK;
#else
    PFN_vkVoidFunction CreateIOSSurfaceMVK;
#endif
#ifdef VK_USE_PLATFORM_MACOS_MVK
    PFN_vkCreateMacOSSurfaceMVK CreateMacOSSurfaceMVK;
#else
    PFN_vkVoidFunction CreateMacOSSurfaceMVK;
#endif
#ifdef VK_USE_PLATFORM_METAL_EXT
    PFN_vkCreateMetalSurfaceEXT CreateMetalSurfaceEXT;
#else
    PFN_vkVoidFunction CreateMetalSurfaceEXT;
#endif
    PFN_vkCreateDebugUtilsMessengerEXT CreateDebugUtilsMessengerEXT;
    PFN_vkDestroyDebugUtilsMessengerEXT DestroyDebugUtilsMessengerEXT;
    PFN_vkSubmitDebugUtilsMessageEXT SubmitDebugUtilsMessageEXT;
    PFN_vkCreateHeadlessSurfaceEXT CreateHeadlessSurfaceEXT;

            
    PFN_vkGetPhysicalDeviceProperties GetPhysicalDeviceProperties;
    PFN_vkGetPhysicalDeviceQueueFamilyProperties GetPhysicalDeviceQueueFamilyProperties;
    PFN_vkGetPhysicalDeviceMemoryProperties GetPhysicalDeviceMemoryProperties;
    PFN_vkGetPhysicalDeviceFeatures GetPhysicalDeviceFeatures;
    PFN_vkGetPhysicalDeviceFormatProperties GetPhysicalDeviceFormatProperties;
    PFN_vkGetPhysicalDeviceImageFormatProperties GetPhysicalDeviceImageFormatProperties;
    PFN_vkCreateDevice CreateDevice;
    PFN_vkEnumerateDeviceLayerProperties EnumerateDeviceLayerProperties;
    PFN_vkEnumerateDeviceExtensionProperties EnumerateDeviceExtensionProperties;
    PFN_vkGetPhysicalDeviceSparseImageFormatProperties GetPhysicalDeviceSparseImageFormatProperties;
    PFN_vkGetPhysicalDeviceDisplayPropertiesKHR GetPhysicalDeviceDisplayPropertiesKHR;
    PFN_vkGetPhysicalDeviceDisplayPlanePropertiesKHR GetPhysicalDeviceDisplayPlanePropertiesKHR;
    PFN_vkGetDisplayPlaneSupportedDisplaysKHR GetDisplayPlaneSupportedDisplaysKHR;
    PFN_vkGetDisplayModePropertiesKHR GetDisplayModePropertiesKHR;
    PFN_vkCreateDisplayModeKHR CreateDisplayModeKHR;
    PFN_vkGetDisplayPlaneCapabilitiesKHR GetDisplayPlaneCapabilitiesKHR;
    PFN_vkGetPhysicalDeviceSurfaceSupportKHR GetPhysicalDeviceSurfaceSupportKHR;
    PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR GetPhysicalDeviceSurfaceCapabilitiesKHR;
    PFN_vkGetPhysicalDeviceSurfaceFormatsKHR GetPhysicalDeviceSurfaceFormatsKHR;
    PFN_vkGetPhysicalDeviceSurfacePresentModesKHR GetPhysicalDeviceSurfacePresentModesKHR;
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
    PFN_vkGetPhysicalDeviceWaylandPresentationSupportKHR GetPhysicalDeviceWaylandPresentationSupportKHR;
#else
    PFN_vkVoidFunction GetPhysicalDeviceWaylandPresentationSupportKHR;
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
    PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR GetPhysicalDeviceWin32PresentationSupportKHR;
#else
    PFN_vkVoidFunction GetPhysicalDeviceWin32PresentationSupportKHR;
#endif
#ifdef VK_USE_PLATFORM_XLIB_KHR
    PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR GetPhysicalDeviceXlibPresentationSupportKHR;
#else
    PFN_vkVoidFunction GetPhysicalDeviceXlibPresentationSupportKHR;
#endif
#ifdef VK_USE_PLATFORM_XCB_KHR
    PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR GetPhysicalDeviceXcbPresentationSupportKHR;
#else
    PFN_vkVoidFunction GetPhysicalDeviceXcbPresentationSupportKHR;
#endif
#ifdef VK_USE_PLATFORM_DIRECTFB_EXT
    PFN_vkGetPhysicalDeviceDirectFBPresentationSupportEXT GetPhysicalDeviceDirectFBPresentationSupportEXT;
#else
    PFN_vkVoidFunction GetPhysicalDeviceDirectFBPresentationSupportEXT;
#endif
#ifdef VK_USE_PLATFORM_SCREEN_QNX
    PFN_vkGetPhysicalDeviceScreenPresentationSupportQNX GetPhysicalDeviceScreenPresentationSupportQNX;
#else
    PFN_vkVoidFunction GetPhysicalDeviceScreenPresentationSupportQNX;
#endif
    PFN_vkGetPhysicalDeviceExternalImageFormatPropertiesNV GetPhysicalDeviceExternalImageFormatPropertiesNV;
    union {
        PFN_vkGetPhysicalDeviceFeatures2 GetPhysicalDeviceFeatures2;
        PFN_vkGetPhysicalDeviceFeatures2KHR GetPhysicalDeviceFeatures2KHR;
    };
        union {
        PFN_vkGetPhysicalDeviceProperties2 GetPhysicalDeviceProperties2;
        PFN_vkGetPhysicalDeviceProperties2KHR GetPhysicalDeviceProperties2KHR;
    };
        union {
        PFN_vkGetPhysicalDeviceFormatProperties2 GetPhysicalDeviceFormatProperties2;
        PFN_vkGetPhysicalDeviceFormatProperties2KHR GetPhysicalDeviceFormatProperties2KHR;
    };
        union {
        PFN_vkGetPhysicalDeviceImageFormatProperties2 GetPhysicalDeviceImageFormatProperties2;
        PFN_vkGetPhysicalDeviceImageFormatProperties2KHR GetPhysicalDeviceImageFormatProperties2KHR;
    };
        union {
        PFN_vkGetPhysicalDeviceQueueFamilyProperties2 GetPhysicalDeviceQueueFamilyProperties2;
        PFN_vkGetPhysicalDeviceQueueFamilyProperties2KHR GetPhysicalDeviceQueueFamilyProperties2KHR;
    };
        union {
        PFN_vkGetPhysicalDeviceMemoryProperties2 GetPhysicalDeviceMemoryProperties2;
        PFN_vkGetPhysicalDeviceMemoryProperties2KHR GetPhysicalDeviceMemoryProperties2KHR;
    };
        union {
        PFN_vkGetPhysicalDeviceSparseImageFormatProperties2 GetPhysicalDeviceSparseImageFormatProperties2;
        PFN_vkGetPhysicalDeviceSparseImageFormatProperties2KHR GetPhysicalDeviceSparseImageFormatProperties2KHR;
    };
        union {
        PFN_vkGetPhysicalDeviceExternalBufferProperties GetPhysicalDeviceExternalBufferProperties;
        PFN_vkGetPhysicalDeviceExternalBufferPropertiesKHR GetPhysicalDeviceExternalBufferPropertiesKHR;
    };
        union {
        PFN_vkGetPhysicalDeviceExternalSemaphoreProperties GetPhysicalDeviceExternalSemaphoreProperties;
        PFN_vkGetPhysicalDeviceExternalSemaphorePropertiesKHR GetPhysicalDeviceExternalSemaphorePropertiesKHR;
    };
        union {
        PFN_vkGetPhysicalDeviceExternalFenceProperties GetPhysicalDeviceExternalFenceProperties;
        PFN_vkGetPhysicalDeviceExternalFencePropertiesKHR GetPhysicalDeviceExternalFencePropertiesKHR;
    };
        PFN_vkReleaseDisplayEXT ReleaseDisplayEXT;
#ifdef VK_USE_PLATFORM_XLIB_XRANDR_EXT
    PFN_vkAcquireXlibDisplayEXT AcquireXlibDisplayEXT;
#else
    PFN_vkVoidFunction AcquireXlibDisplayEXT;
#endif
#ifdef VK_USE_PLATFORM_XLIB_XRANDR_EXT
    PFN_vkGetRandROutputDisplayEXT GetRandROutputDisplayEXT;
#else
    PFN_vkVoidFunction GetRandROutputDisplayEXT;
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
    PFN_vkAcquireWinrtDisplayNV AcquireWinrtDisplayNV;
#else
    PFN_vkVoidFunction AcquireWinrtDisplayNV;
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
    PFN_vkGetWinrtDisplayNV GetWinrtDisplayNV;
#else
    PFN_vkVoidFunction GetWinrtDisplayNV;
#endif
    PFN_vkGetPhysicalDeviceSurfaceCapabilities2EXT GetPhysicalDeviceSurfaceCapabilities2EXT;
    PFN_vkGetPhysicalDevicePresentRectanglesKHR GetPhysicalDevicePresentRectanglesKHR;
    PFN_vkGetPhysicalDeviceMultisamplePropertiesEXT GetPhysicalDeviceMultisamplePropertiesEXT;
    PFN_vkGetPhysicalDeviceSurfaceCapabilities2KHR GetPhysicalDeviceSurfaceCapabilities2KHR;
    PFN_vkGetPhysicalDeviceSurfaceFormats2KHR GetPhysicalDeviceSurfaceFormats2KHR;
    PFN_vkGetPhysicalDeviceDisplayProperties2KHR GetPhysicalDeviceDisplayProperties2KHR;
    PFN_vkGetPhysicalDeviceDisplayPlaneProperties2KHR GetPhysicalDeviceDisplayPlaneProperties2KHR;
    PFN_vkGetDisplayModeProperties2KHR GetDisplayModeProperties2KHR;
    PFN_vkGetDisplayPlaneCapabilities2KHR GetDisplayPlaneCapabilities2KHR;
    PFN_vkGetPhysicalDeviceCalibrateableTimeDomainsEXT GetPhysicalDeviceCalibrateableTimeDomainsEXT;
    PFN_vkGetPhysicalDeviceCooperativeMatrixPropertiesNV GetPhysicalDeviceCooperativeMatrixPropertiesNV;
#ifdef VK_USE_PLATFORM_WIN32_KHR
    PFN_vkGetPhysicalDeviceSurfacePresentModes2EXT GetPhysicalDeviceSurfacePresentModes2EXT;
#else
    PFN_vkVoidFunction GetPhysicalDeviceSurfacePresentModes2EXT;
#endif
    PFN_vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR EnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR;
    PFN_vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR GetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR;
    PFN_vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV GetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV;
    union {
        PFN_vkGetPhysicalDeviceToolProperties GetPhysicalDeviceToolProperties;
        PFN_vkGetPhysicalDeviceToolPropertiesEXT GetPhysicalDeviceToolPropertiesEXT;
    };
        PFN_vkGetPhysicalDeviceFragmentShadingRatesKHR GetPhysicalDeviceFragmentShadingRatesKHR;
    PFN_vkGetPhysicalDeviceVideoCapabilitiesKHR GetPhysicalDeviceVideoCapabilitiesKHR;
    PFN_vkGetPhysicalDeviceVideoFormatPropertiesKHR GetPhysicalDeviceVideoFormatPropertiesKHR;
    PFN_vkAcquireDrmDisplayEXT AcquireDrmDisplayEXT;
    PFN_vkGetDrmDisplayEXT GetDrmDisplayEXT;
    PFN_vkGetPhysicalDeviceOpticalFlowImageFormatsNV GetPhysicalDeviceOpticalFlowImageFormatsNV;

            
    PFN_vkGetDeviceProcAddr GetDeviceProcAddr;
    PFN_vkDestroyDevice DestroyDevice;
    PFN_vkGetDeviceQueue GetDeviceQueue;
    PFN_vkQueueSubmit QueueSubmit;
    PFN_vkQueueWaitIdle QueueWaitIdle;
    PFN_vkDeviceWaitIdle DeviceWaitIdle;
    PFN_vkAllocateMemory AllocateMemory;
    PFN_vkFreeMemory FreeMemory;
    PFN_vkMapMemory MapMemory;
    PFN_vkUnmapMemory UnmapMemory;
    PFN_vkFlushMappedMemoryRanges FlushMappedMemoryRanges;
    PFN_vkInvalidateMappedMemoryRanges InvalidateMappedMemoryRanges;
    PFN_vkGetDeviceMemoryCommitment GetDeviceMemoryCommitment;
    PFN_vkGetBufferMemoryRequirements GetBufferMemoryRequirements;
    PFN_vkBindBufferMemory BindBufferMemory;
    PFN_vkGetImageMemoryRequirements GetImageMemoryRequirements;
    PFN_vkBindImageMemory BindImageMemory;
    PFN_vkGetImageSparseMemoryRequirements GetImageSparseMemoryRequirements;
    PFN_vkQueueBindSparse QueueBindSparse;
    PFN_vkCreateFence CreateFence;
    PFN_vkDestroyFence DestroyFence;
    PFN_vkResetFences ResetFences;
    PFN_vkGetFenceStatus GetFenceStatus;
    PFN_vkWaitForFences WaitForFences;
    PFN_vkCreateSemaphore CreateSemaphore;
    PFN_vkDestroySemaphore DestroySemaphore;
    PFN_vkCreateEvent CreateEvent;
    PFN_vkDestroyEvent DestroyEvent;
    PFN_vkGetEventStatus GetEventStatus;
    PFN_vkSetEvent SetEvent;
    PFN_vkResetEvent ResetEvent;
    PFN_vkCreateQueryPool CreateQueryPool;
    PFN_vkDestroyQueryPool DestroyQueryPool;
    PFN_vkGetQueryPoolResults GetQueryPoolResults;
    union {
        PFN_vkResetQueryPool ResetQueryPool;
        PFN_vkResetQueryPoolEXT ResetQueryPoolEXT;
    };
        PFN_vkCreateBuffer CreateBuffer;
    PFN_vkDestroyBuffer DestroyBuffer;
    PFN_vkCreateBufferView CreateBufferView;
    PFN_vkDestroyBufferView DestroyBufferView;
    PFN_vkCreateImage CreateImage;
    PFN_vkDestroyImage DestroyImage;
    PFN_vkGetImageSubresourceLayout GetImageSubresourceLayout;
    PFN_vkCreateImageView CreateImageView;
    PFN_vkDestroyImageView DestroyImageView;
    PFN_vkCreateShaderModule CreateShaderModule;
    PFN_vkDestroyShaderModule DestroyShaderModule;
    PFN_vkCreatePipelineCache CreatePipelineCache;
    PFN_vkDestroyPipelineCache DestroyPipelineCache;
    PFN_vkGetPipelineCacheData GetPipelineCacheData;
    PFN_vkMergePipelineCaches MergePipelineCaches;
    PFN_vkCreateGraphicsPipelines CreateGraphicsPipelines;
    PFN_vkCreateComputePipelines CreateComputePipelines;
    PFN_vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI GetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI;
    PFN_vkDestroyPipeline DestroyPipeline;
    PFN_vkCreatePipelineLayout CreatePipelineLayout;
    PFN_vkDestroyPipelineLayout DestroyPipelineLayout;
    PFN_vkCreateSampler CreateSampler;
    PFN_vkDestroySampler DestroySampler;
    PFN_vkCreateDescriptorSetLayout CreateDescriptorSetLayout;
    PFN_vkDestroyDescriptorSetLayout DestroyDescriptorSetLayout;
    PFN_vkCreateDescriptorPool CreateDescriptorPool;
    PFN_vkDestroyDescriptorPool DestroyDescriptorPool;
    PFN_vkResetDescriptorPool ResetDescriptorPool;
    PFN_vkAllocateDescriptorSets AllocateDescriptorSets;
    PFN_vkFreeDescriptorSets FreeDescriptorSets;
    PFN_vkUpdateDescriptorSets UpdateDescriptorSets;
    PFN_vkCreateFramebuffer CreateFramebuffer;
    PFN_vkDestroyFramebuffer DestroyFramebuffer;
    PFN_vkCreateRenderPass CreateRenderPass;
    PFN_vkDestroyRenderPass DestroyRenderPass;
    PFN_vkGetRenderAreaGranularity GetRenderAreaGranularity;
    PFN_vkCreateCommandPool CreateCommandPool;
    PFN_vkDestroyCommandPool DestroyCommandPool;
    PFN_vkResetCommandPool ResetCommandPool;
    PFN_vkAllocateCommandBuffers AllocateCommandBuffers;
    PFN_vkFreeCommandBuffers FreeCommandBuffers;
    PFN_vkBeginCommandBuffer BeginCommandBuffer;
    PFN_vkEndCommandBuffer EndCommandBuffer;
    PFN_vkResetCommandBuffer ResetCommandBuffer;
    PFN_vkCmdBindPipeline CmdBindPipeline;
    PFN_vkCmdSetViewport CmdSetViewport;
    PFN_vkCmdSetScissor CmdSetScissor;
    PFN_vkCmdSetLineWidth CmdSetLineWidth;
    PFN_vkCmdSetDepthBias CmdSetDepthBias;
    PFN_vkCmdSetBlendConstants CmdSetBlendConstants;
    PFN_vkCmdSetDepthBounds CmdSetDepthBounds;
    PFN_vkCmdSetStencilCompareMask CmdSetStencilCompareMask;
    PFN_vkCmdSetStencilWriteMask CmdSetStencilWriteMask;
    PFN_vkCmdSetStencilReference CmdSetStencilReference;
    PFN_vkCmdBindDescriptorSets CmdBindDescriptorSets;
    PFN_vkCmdBindIndexBuffer CmdBindIndexBuffer;
    PFN_vkCmdBindVertexBuffers CmdBindVertexBuffers;
    PFN_vkCmdDraw CmdDraw;
    PFN_vkCmdDrawIndexed CmdDrawIndexed;
    PFN_vkCmdDrawMultiEXT CmdDrawMultiEXT;
    PFN_vkCmdDrawMultiIndexedEXT CmdDrawMultiIndexedEXT;
    PFN_vkCmdDrawIndirect CmdDrawIndirect;
    PFN_vkCmdDrawIndexedIndirect CmdDrawIndexedIndirect;
    PFN_vkCmdDispatch CmdDispatch;
    PFN_vkCmdDispatchIndirect CmdDispatchIndirect;
    PFN_vkCmdSubpassShadingHUAWEI CmdSubpassShadingHUAWEI;
    PFN_vkCmdDrawClusterHUAWEI CmdDrawClusterHUAWEI;
    PFN_vkCmdDrawClusterIndirectHUAWEI CmdDrawClusterIndirectHUAWEI;
    PFN_vkCmdCopyBuffer CmdCopyBuffer;
    PFN_vkCmdCopyImage CmdCopyImage;
    PFN_vkCmdBlitImage CmdBlitImage;
    PFN_vkCmdCopyBufferToImage CmdCopyBufferToImage;
    PFN_vkCmdCopyImageToBuffer CmdCopyImageToBuffer;
    PFN_vkCmdCopyMemoryIndirectNV CmdCopyMemoryIndirectNV;
    PFN_vkCmdCopyMemoryToImageIndirectNV CmdCopyMemoryToImageIndirectNV;
    PFN_vkCmdUpdateBuffer CmdUpdateBuffer;
    PFN_vkCmdFillBuffer CmdFillBuffer;
    PFN_vkCmdClearColorImage CmdClearColorImage;
    PFN_vkCmdClearDepthStencilImage CmdClearDepthStencilImage;
    PFN_vkCmdClearAttachments CmdClearAttachments;
    PFN_vkCmdResolveImage CmdResolveImage;
    PFN_vkCmdSetEvent CmdSetEvent;
    PFN_vkCmdResetEvent CmdResetEvent;
    PFN_vkCmdWaitEvents CmdWaitEvents;
    PFN_vkCmdPipelineBarrier CmdPipelineBarrier;
    PFN_vkCmdBeginQuery CmdBeginQuery;
    PFN_vkCmdEndQuery CmdEndQuery;
    PFN_vkCmdBeginConditionalRenderingEXT CmdBeginConditionalRenderingEXT;
    PFN_vkCmdEndConditionalRenderingEXT CmdEndConditionalRenderingEXT;
    PFN_vkCmdResetQueryPool CmdResetQueryPool;
    PFN_vkCmdWriteTimestamp CmdWriteTimestamp;
    PFN_vkCmdCopyQueryPoolResults CmdCopyQueryPoolResults;
    PFN_vkCmdPushConstants CmdPushConstants;
    PFN_vkCmdBeginRenderPass CmdBeginRenderPass;
    PFN_vkCmdNextSubpass CmdNextSubpass;
    PFN_vkCmdEndRenderPass CmdEndRenderPass;
    PFN_vkCmdExecuteCommands CmdExecuteCommands;
    PFN_vkCreateSharedSwapchainsKHR CreateSharedSwapchainsKHR;
    PFN_vkCreateSwapchainKHR CreateSwapchainKHR;
    PFN_vkDestroySwapchainKHR DestroySwapchainKHR;
    PFN_vkGetSwapchainImagesKHR GetSwapchainImagesKHR;
    PFN_vkAcquireNextImageKHR AcquireNextImageKHR;
    PFN_vkQueuePresentKHR QueuePresentKHR;
    PFN_vkDebugMarkerSetObjectNameEXT DebugMarkerSetObjectNameEXT;
    PFN_vkDebugMarkerSetObjectTagEXT DebugMarkerSetObjectTagEXT;
    PFN_vkCmdDebugMarkerBeginEXT CmdDebugMarkerBeginEXT;
    PFN_vkCmdDebugMarkerEndEXT CmdDebugMarkerEndEXT;
    PFN_vkCmdDebugMarkerInsertEXT CmdDebugMarkerInsertEXT;
#ifdef VK_USE_PLATFORM_WIN32_KHR
    PFN_vkGetMemoryWin32HandleNV GetMemoryWin32HandleNV;
#else
    PFN_vkVoidFunction GetMemoryWin32HandleNV;
#endif
    PFN_vkCmdExecuteGeneratedCommandsNV CmdExecuteGeneratedCommandsNV;
    PFN_vkCmdPreprocessGeneratedCommandsNV CmdPreprocessGeneratedCommandsNV;
    PFN_vkCmdBindPipelineShaderGroupNV CmdBindPipelineShaderGroupNV;
    PFN_vkGetGeneratedCommandsMemoryRequirementsNV GetGeneratedCommandsMemoryRequirementsNV;
    PFN_vkCreateIndirectCommandsLayoutNV CreateIndirectCommandsLayoutNV;
    PFN_vkDestroyIndirectCommandsLayoutNV DestroyIndirectCommandsLayoutNV;
    PFN_vkCmdPushDescriptorSetKHR CmdPushDescriptorSetKHR;
    union {
        PFN_vkTrimCommandPool TrimCommandPool;
        PFN_vkTrimCommandPoolKHR TrimCommandPoolKHR;
    };
    #ifdef VK_USE_PLATFORM_WIN32_KHR
    PFN_vkGetMemoryWin32HandleKHR GetMemoryWin32HandleKHR;
#else
    PFN_vkVoidFunction GetMemoryWin32HandleKHR;
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
    PFN_vkGetMemoryWin32HandlePropertiesKHR GetMemoryWin32HandlePropertiesKHR;
#else
    PFN_vkVoidFunction GetMemoryWin32HandlePropertiesKHR;
#endif
    PFN_vkGetMemoryFdKHR GetMemoryFdKHR;
    PFN_vkGetMemoryFdPropertiesKHR GetMemoryFdPropertiesKHR;
#ifdef VK_USE_PLATFORM_FUCHSIA
    PFN_vkGetMemoryZirconHandleFUCHSIA GetMemoryZirconHandleFUCHSIA;
#else
    PFN_vkVoidFunction GetMemoryZirconHandleFUCHSIA;
#endif
#ifdef VK_USE_PLATFORM_FUCHSIA
    PFN_vkGetMemoryZirconHandlePropertiesFUCHSIA GetMemoryZirconHandlePropertiesFUCHSIA;
#else
    PFN_vkVoidFunction GetMemoryZirconHandlePropertiesFUCHSIA;
#endif
    PFN_vkGetMemoryRemoteAddressNV GetMemoryRemoteAddressNV;
#ifdef VK_USE_PLATFORM_WIN32_KHR
    PFN_vkGetSemaphoreWin32HandleKHR GetSemaphoreWin32HandleKHR;
#else
    PFN_vkVoidFunction GetSemaphoreWin32HandleKHR;
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
    PFN_vkImportSemaphoreWin32HandleKHR ImportSemaphoreWin32HandleKHR;
#else
    PFN_vkVoidFunction ImportSemaphoreWin32HandleKHR;
#endif
    PFN_vkGetSemaphoreFdKHR GetSemaphoreFdKHR;
    PFN_vkImportSemaphoreFdKHR ImportSemaphoreFdKHR;
#ifdef VK_USE_PLATFORM_FUCHSIA
    PFN_vkGetSemaphoreZirconHandleFUCHSIA GetSemaphoreZirconHandleFUCHSIA;
#else
    PFN_vkVoidFunction GetSemaphoreZirconHandleFUCHSIA;
#endif
#ifdef VK_USE_PLATFORM_FUCHSIA
    PFN_vkImportSemaphoreZirconHandleFUCHSIA ImportSemaphoreZirconHandleFUCHSIA;
#else
    PFN_vkVoidFunction ImportSemaphoreZirconHandleFUCHSIA;
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
    PFN_vkGetFenceWin32HandleKHR GetFenceWin32HandleKHR;
#else
    PFN_vkVoidFunction GetFenceWin32HandleKHR;
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
    PFN_vkImportFenceWin32HandleKHR ImportFenceWin32HandleKHR;
#else
    PFN_vkVoidFunction ImportFenceWin32HandleKHR;
#endif
    PFN_vkGetFenceFdKHR GetFenceFdKHR;
    PFN_vkImportFenceFdKHR ImportFenceFdKHR;
    PFN_vkDisplayPowerControlEXT DisplayPowerControlEXT;
    PFN_vkRegisterDeviceEventEXT RegisterDeviceEventEXT;
    PFN_vkRegisterDisplayEventEXT RegisterDisplayEventEXT;
    PFN_vkGetSwapchainCounterEXT GetSwapchainCounterEXT;
    union {
        PFN_vkGetDeviceGroupPeerMemoryFeatures GetDeviceGroupPeerMemoryFeatures;
        PFN_vkGetDeviceGroupPeerMemoryFeaturesKHR GetDeviceGroupPeerMemoryFeaturesKHR;
    };
        union {
        PFN_vkBindBufferMemory2 BindBufferMemory2;
        PFN_vkBindBufferMemory2KHR BindBufferMemory2KHR;
    };
        union {
        PFN_vkBindImageMemory2 BindImageMemory2;
        PFN_vkBindImageMemory2KHR BindImageMemory2KHR;
    };
        union {
        PFN_vkCmdSetDeviceMask CmdSetDeviceMask;
        PFN_vkCmdSetDeviceMaskKHR CmdSetDeviceMaskKHR;
    };
        PFN_vkGetDeviceGroupPresentCapabilitiesKHR GetDeviceGroupPresentCapabilitiesKHR;
    PFN_vkGetDeviceGroupSurfacePresentModesKHR GetDeviceGroupSurfacePresentModesKHR;
    PFN_vkAcquireNextImage2KHR AcquireNextImage2KHR;
    union {
        PFN_vkCmdDispatchBase CmdDispatchBase;
        PFN_vkCmdDispatchBaseKHR CmdDispatchBaseKHR;
    };
        union {
        PFN_vkCreateDescriptorUpdateTemplate CreateDescriptorUpdateTemplate;
        PFN_vkCreateDescriptorUpdateTemplateKHR CreateDescriptorUpdateTemplateKHR;
    };
        union {
        PFN_vkDestroyDescriptorUpdateTemplate DestroyDescriptorUpdateTemplate;
        PFN_vkDestroyDescriptorUpdateTemplateKHR DestroyDescriptorUpdateTemplateKHR;
    };
        union {
        PFN_vkUpdateDescriptorSetWithTemplate UpdateDescriptorSetWithTemplate;
        PFN_vkUpdateDescriptorSetWithTemplateKHR UpdateDescriptorSetWithTemplateKHR;
    };
        PFN_vkCmdPushDescriptorSetWithTemplateKHR CmdPushDescriptorSetWithTemplateKHR;
    PFN_vkSetHdrMetadataEXT SetHdrMetadataEXT;
    PFN_vkGetSwapchainStatusKHR GetSwapchainStatusKHR;
    PFN_vkGetRefreshCycleDurationGOOGLE GetRefreshCycleDurationGOOGLE;
    PFN_vkGetPastPresentationTimingGOOGLE GetPastPresentationTimingGOOGLE;
    PFN_vkCmdSetViewportWScalingNV CmdSetViewportWScalingNV;
    PFN_vkCmdSetDiscardRectangleEXT CmdSetDiscardRectangleEXT;
    PFN_vkCmdSetDiscardRectangleEnableEXT CmdSetDiscardRectangleEnableEXT;
    PFN_vkCmdSetDiscardRectangleModeEXT CmdSetDiscardRectangleModeEXT;
    PFN_vkCmdSetSampleLocationsEXT CmdSetSampleLocationsEXT;
    union {
        PFN_vkGetBufferMemoryRequirements2 GetBufferMemoryRequirements2;
        PFN_vkGetBufferMemoryRequirements2KHR GetBufferMemoryRequirements2KHR;
    };
        union {
        PFN_vkGetImageMemoryRequirements2 GetImageMemoryRequirements2;
        PFN_vkGetImageMemoryRequirements2KHR GetImageMemoryRequirements2KHR;
    };
        union {
        PFN_vkGetImageSparseMemoryRequirements2 GetImageSparseMemoryRequirements2;
        PFN_vkGetImageSparseMemoryRequirements2KHR GetImageSparseMemoryRequirements2KHR;
    };
        union {
        PFN_vkGetDeviceBufferMemoryRequirements GetDeviceBufferMemoryRequirements;
        PFN_vkGetDeviceBufferMemoryRequirementsKHR GetDeviceBufferMemoryRequirementsKHR;
    };
        union {
        PFN_vkGetDeviceImageMemoryRequirements GetDeviceImageMemoryRequirements;
        PFN_vkGetDeviceImageMemoryRequirementsKHR GetDeviceImageMemoryRequirementsKHR;
    };
        union {
        PFN_vkGetDeviceImageSparseMemoryRequirements GetDeviceImageSparseMemoryRequirements;
        PFN_vkGetDeviceImageSparseMemoryRequirementsKHR GetDeviceImageSparseMemoryRequirementsKHR;
    };
        union {
        PFN_vkCreateSamplerYcbcrConversion CreateSamplerYcbcrConversion;
        PFN_vkCreateSamplerYcbcrConversionKHR CreateSamplerYcbcrConversionKHR;
    };
        union {
        PFN_vkDestroySamplerYcbcrConversion DestroySamplerYcbcrConversion;
        PFN_vkDestroySamplerYcbcrConversionKHR DestroySamplerYcbcrConversionKHR;
    };
        PFN_vkGetDeviceQueue2 GetDeviceQueue2;
    PFN_vkCreateValidationCacheEXT CreateValidationCacheEXT;
    PFN_vkDestroyValidationCacheEXT DestroyValidationCacheEXT;
    PFN_vkGetValidationCacheDataEXT GetValidationCacheDataEXT;
    PFN_vkMergeValidationCachesEXT MergeValidationCachesEXT;
    union {
        PFN_vkGetDescriptorSetLayoutSupport GetDescriptorSetLayoutSupport;
        PFN_vkGetDescriptorSetLayoutSupportKHR GetDescriptorSetLayoutSupportKHR;
    };
    #ifdef VK_USE_PLATFORM_ANDROID_KHR
    PFN_vkGetSwapchainGrallocUsageANDROID GetSwapchainGrallocUsageANDROID;
#else
    PFN_vkVoidFunction GetSwapchainGrallocUsageANDROID;
#endif
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    PFN_vkGetSwapchainGrallocUsage2ANDROID GetSwapchainGrallocUsage2ANDROID;
#else
    PFN_vkVoidFunction GetSwapchainGrallocUsage2ANDROID;
#endif
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    PFN_vkAcquireImageANDROID AcquireImageANDROID;
#else
    PFN_vkVoidFunction AcquireImageANDROID;
#endif
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    PFN_vkQueueSignalReleaseImageANDROID QueueSignalReleaseImageANDROID;
#else
    PFN_vkVoidFunction QueueSignalReleaseImageANDROID;
#endif
    PFN_vkGetShaderInfoAMD GetShaderInfoAMD;
    PFN_vkSetLocalDimmingAMD SetLocalDimmingAMD;
    PFN_vkGetCalibratedTimestampsEXT GetCalibratedTimestampsEXT;
    PFN_vkSetDebugUtilsObjectNameEXT SetDebugUtilsObjectNameEXT;
    PFN_vkSetDebugUtilsObjectTagEXT SetDebugUtilsObjectTagEXT;
    PFN_vkQueueBeginDebugUtilsLabelEXT QueueBeginDebugUtilsLabelEXT;
    PFN_vkQueueEndDebugUtilsLabelEXT QueueEndDebugUtilsLabelEXT;
    PFN_vkQueueInsertDebugUtilsLabelEXT QueueInsertDebugUtilsLabelEXT;
    PFN_vkCmdBeginDebugUtilsLabelEXT CmdBeginDebugUtilsLabelEXT;
    PFN_vkCmdEndDebugUtilsLabelEXT CmdEndDebugUtilsLabelEXT;
    PFN_vkCmdInsertDebugUtilsLabelEXT CmdInsertDebugUtilsLabelEXT;
    PFN_vkGetMemoryHostPointerPropertiesEXT GetMemoryHostPointerPropertiesEXT;
    PFN_vkCmdWriteBufferMarkerAMD CmdWriteBufferMarkerAMD;
    union {
        PFN_vkCreateRenderPass2 CreateRenderPass2;
        PFN_vkCreateRenderPass2KHR CreateRenderPass2KHR;
    };
        union {
        PFN_vkCmdBeginRenderPass2 CmdBeginRenderPass2;
        PFN_vkCmdBeginRenderPass2KHR CmdBeginRenderPass2KHR;
    };
        union {
        PFN_vkCmdNextSubpass2 CmdNextSubpass2;
        PFN_vkCmdNextSubpass2KHR CmdNextSubpass2KHR;
    };
        union {
        PFN_vkCmdEndRenderPass2 CmdEndRenderPass2;
        PFN_vkCmdEndRenderPass2KHR CmdEndRenderPass2KHR;
    };
        union {
        PFN_vkGetSemaphoreCounterValue GetSemaphoreCounterValue;
        PFN_vkGetSemaphoreCounterValueKHR GetSemaphoreCounterValueKHR;
    };
        union {
        PFN_vkWaitSemaphores WaitSemaphores;
        PFN_vkWaitSemaphoresKHR WaitSemaphoresKHR;
    };
        union {
        PFN_vkSignalSemaphore SignalSemaphore;
        PFN_vkSignalSemaphoreKHR SignalSemaphoreKHR;
    };
    #ifdef VK_USE_PLATFORM_ANDROID_KHR
    PFN_vkGetAndroidHardwareBufferPropertiesANDROID GetAndroidHardwareBufferPropertiesANDROID;
#else
    PFN_vkVoidFunction GetAndroidHardwareBufferPropertiesANDROID;
#endif
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    PFN_vkGetMemoryAndroidHardwareBufferANDROID GetMemoryAndroidHardwareBufferANDROID;
#else
    PFN_vkVoidFunction GetMemoryAndroidHardwareBufferANDROID;
#endif
    union {
        PFN_vkCmdDrawIndirectCount CmdDrawIndirectCount;
        PFN_vkCmdDrawIndirectCountKHR CmdDrawIndirectCountKHR;
        PFN_vkCmdDrawIndirectCountAMD CmdDrawIndirectCountAMD;
    };
            union {
        PFN_vkCmdDrawIndexedIndirectCount CmdDrawIndexedIndirectCount;
        PFN_vkCmdDrawIndexedIndirectCountKHR CmdDrawIndexedIndirectCountKHR;
        PFN_vkCmdDrawIndexedIndirectCountAMD CmdDrawIndexedIndirectCountAMD;
    };
            PFN_vkCmdSetCheckpointNV CmdSetCheckpointNV;
    PFN_vkGetQueueCheckpointDataNV GetQueueCheckpointDataNV;
    PFN_vkCmdBindTransformFeedbackBuffersEXT CmdBindTransformFeedbackBuffersEXT;
    PFN_vkCmdBeginTransformFeedbackEXT CmdBeginTransformFeedbackEXT;
    PFN_vkCmdEndTransformFeedbackEXT CmdEndTransformFeedbackEXT;
    PFN_vkCmdBeginQueryIndexedEXT CmdBeginQueryIndexedEXT;
    PFN_vkCmdEndQueryIndexedEXT CmdEndQueryIndexedEXT;
    PFN_vkCmdDrawIndirectByteCountEXT CmdDrawIndirectByteCountEXT;
    PFN_vkCmdSetExclusiveScissorNV CmdSetExclusiveScissorNV;
    PFN_vkCmdSetExclusiveScissorEnableNV CmdSetExclusiveScissorEnableNV;
    PFN_vkCmdBindShadingRateImageNV CmdBindShadingRateImageNV;
    PFN_vkCmdSetViewportShadingRatePaletteNV CmdSetViewportShadingRatePaletteNV;
    PFN_vkCmdSetCoarseSampleOrderNV CmdSetCoarseSampleOrderNV;
    PFN_vkCmdDrawMeshTasksNV CmdDrawMeshTasksNV;
    PFN_vkCmdDrawMeshTasksIndirectNV CmdDrawMeshTasksIndirectNV;
    PFN_vkCmdDrawMeshTasksIndirectCountNV CmdDrawMeshTasksIndirectCountNV;
    PFN_vkCmdDrawMeshTasksEXT CmdDrawMeshTasksEXT;
    PFN_vkCmdDrawMeshTasksIndirectEXT CmdDrawMeshTasksIndirectEXT;
    PFN_vkCmdDrawMeshTasksIndirectCountEXT CmdDrawMeshTasksIndirectCountEXT;
    PFN_vkCompileDeferredNV CompileDeferredNV;
    PFN_vkCreateAccelerationStructureNV CreateAccelerationStructureNV;
    PFN_vkCmdBindInvocationMaskHUAWEI CmdBindInvocationMaskHUAWEI;
    PFN_vkDestroyAccelerationStructureKHR DestroyAccelerationStructureKHR;
    PFN_vkDestroyAccelerationStructureNV DestroyAccelerationStructureNV;
    PFN_vkGetAccelerationStructureMemoryRequirementsNV GetAccelerationStructureMemoryRequirementsNV;
    PFN_vkBindAccelerationStructureMemoryNV BindAccelerationStructureMemoryNV;
    PFN_vkCmdCopyAccelerationStructureNV CmdCopyAccelerationStructureNV;
    PFN_vkCmdCopyAccelerationStructureKHR CmdCopyAccelerationStructureKHR;
    PFN_vkCopyAccelerationStructureKHR CopyAccelerationStructureKHR;
    PFN_vkCmdCopyAccelerationStructureToMemoryKHR CmdCopyAccelerationStructureToMemoryKHR;
    PFN_vkCopyAccelerationStructureToMemoryKHR CopyAccelerationStructureToMemoryKHR;
    PFN_vkCmdCopyMemoryToAccelerationStructureKHR CmdCopyMemoryToAccelerationStructureKHR;
    PFN_vkCopyMemoryToAccelerationStructureKHR CopyMemoryToAccelerationStructureKHR;
    PFN_vkCmdWriteAccelerationStructuresPropertiesKHR CmdWriteAccelerationStructuresPropertiesKHR;
    PFN_vkCmdWriteAccelerationStructuresPropertiesNV CmdWriteAccelerationStructuresPropertiesNV;
    PFN_vkCmdBuildAccelerationStructureNV CmdBuildAccelerationStructureNV;
    PFN_vkWriteAccelerationStructuresPropertiesKHR WriteAccelerationStructuresPropertiesKHR;
    PFN_vkCmdTraceRaysKHR CmdTraceRaysKHR;
    PFN_vkCmdTraceRaysNV CmdTraceRaysNV;
    union {
        PFN_vkGetRayTracingShaderGroupHandlesKHR GetRayTracingShaderGroupHandlesKHR;
        PFN_vkGetRayTracingShaderGroupHandlesNV GetRayTracingShaderGroupHandlesNV;
    };
        PFN_vkGetRayTracingCaptureReplayShaderGroupHandlesKHR GetRayTracingCaptureReplayShaderGroupHandlesKHR;
    PFN_vkGetAccelerationStructureHandleNV GetAccelerationStructureHandleNV;
    PFN_vkCreateRayTracingPipelinesNV CreateRayTracingPipelinesNV;
    PFN_vkCreateRayTracingPipelinesKHR CreateRayTracingPipelinesKHR;
    PFN_vkCmdTraceRaysIndirectKHR CmdTraceRaysIndirectKHR;
    PFN_vkCmdTraceRaysIndirect2KHR CmdTraceRaysIndirect2KHR;
    PFN_vkGetDeviceAccelerationStructureCompatibilityKHR GetDeviceAccelerationStructureCompatibilityKHR;
    PFN_vkGetRayTracingShaderGroupStackSizeKHR GetRayTracingShaderGroupStackSizeKHR;
    PFN_vkCmdSetRayTracingPipelineStackSizeKHR CmdSetRayTracingPipelineStackSizeKHR;
    PFN_vkGetImageViewHandleNVX GetImageViewHandleNVX;
    PFN_vkGetImageViewAddressNVX GetImageViewAddressNVX;
#ifdef VK_USE_PLATFORM_WIN32_KHR
    PFN_vkGetDeviceGroupSurfacePresentModes2EXT GetDeviceGroupSurfacePresentModes2EXT;
#else
    PFN_vkVoidFunction GetDeviceGroupSurfacePresentModes2EXT;
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
    PFN_vkAcquireFullScreenExclusiveModeEXT AcquireFullScreenExclusiveModeEXT;
#else
    PFN_vkVoidFunction AcquireFullScreenExclusiveModeEXT;
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
    PFN_vkReleaseFullScreenExclusiveModeEXT ReleaseFullScreenExclusiveModeEXT;
#else
    PFN_vkVoidFunction ReleaseFullScreenExclusiveModeEXT;
#endif
    PFN_vkAcquireProfilingLockKHR AcquireProfilingLockKHR;
    PFN_vkReleaseProfilingLockKHR ReleaseProfilingLockKHR;
    PFN_vkGetImageDrmFormatModifierPropertiesEXT GetImageDrmFormatModifierPropertiesEXT;
    union {
        PFN_vkGetBufferOpaqueCaptureAddress GetBufferOpaqueCaptureAddress;
        PFN_vkGetBufferOpaqueCaptureAddressKHR GetBufferOpaqueCaptureAddressKHR;
    };
        union {
        PFN_vkGetBufferDeviceAddress GetBufferDeviceAddress;
        PFN_vkGetBufferDeviceAddressKHR GetBufferDeviceAddressKHR;
        PFN_vkGetBufferDeviceAddressEXT GetBufferDeviceAddressEXT;
    };
            PFN_vkInitializePerformanceApiINTEL InitializePerformanceApiINTEL;
    PFN_vkUninitializePerformanceApiINTEL UninitializePerformanceApiINTEL;
    PFN_vkCmdSetPerformanceMarkerINTEL CmdSetPerformanceMarkerINTEL;
    PFN_vkCmdSetPerformanceStreamMarkerINTEL CmdSetPerformanceStreamMarkerINTEL;
    PFN_vkCmdSetPerformanceOverrideINTEL CmdSetPerformanceOverrideINTEL;
    PFN_vkAcquirePerformanceConfigurationINTEL AcquirePerformanceConfigurationINTEL;
    PFN_vkReleasePerformanceConfigurationINTEL ReleasePerformanceConfigurationINTEL;
    PFN_vkQueueSetPerformanceConfigurationINTEL QueueSetPerformanceConfigurationINTEL;
    PFN_vkGetPerformanceParameterINTEL GetPerformanceParameterINTEL;
    union {
        PFN_vkGetDeviceMemoryOpaqueCaptureAddress GetDeviceMemoryOpaqueCaptureAddress;
        PFN_vkGetDeviceMemoryOpaqueCaptureAddressKHR GetDeviceMemoryOpaqueCaptureAddressKHR;
    };
        PFN_vkGetPipelineExecutablePropertiesKHR GetPipelineExecutablePropertiesKHR;
    PFN_vkGetPipelineExecutableStatisticsKHR GetPipelineExecutableStatisticsKHR;
    PFN_vkGetPipelineExecutableInternalRepresentationsKHR GetPipelineExecutableInternalRepresentationsKHR;
    PFN_vkCmdSetLineStippleEXT CmdSetLineStippleEXT;
    PFN_vkCreateAccelerationStructureKHR CreateAccelerationStructureKHR;
    PFN_vkCmdBuildAccelerationStructuresKHR CmdBuildAccelerationStructuresKHR;
    PFN_vkCmdBuildAccelerationStructuresIndirectKHR CmdBuildAccelerationStructuresIndirectKHR;
    PFN_vkBuildAccelerationStructuresKHR BuildAccelerationStructuresKHR;
    PFN_vkGetAccelerationStructureDeviceAddressKHR GetAccelerationStructureDeviceAddressKHR;
    PFN_vkCreateDeferredOperationKHR CreateDeferredOperationKHR;
    PFN_vkDestroyDeferredOperationKHR DestroyDeferredOperationKHR;
    PFN_vkGetDeferredOperationMaxConcurrencyKHR GetDeferredOperationMaxConcurrencyKHR;
    PFN_vkGetDeferredOperationResultKHR GetDeferredOperationResultKHR;
    PFN_vkDeferredOperationJoinKHR DeferredOperationJoinKHR;
    union {
        PFN_vkCmdSetCullMode CmdSetCullMode;
        PFN_vkCmdSetCullModeEXT CmdSetCullModeEXT;
    };
        union {
        PFN_vkCmdSetFrontFace CmdSetFrontFace;
        PFN_vkCmdSetFrontFaceEXT CmdSetFrontFaceEXT;
    };
        union {
        PFN_vkCmdSetPrimitiveTopology CmdSetPrimitiveTopology;
        PFN_vkCmdSetPrimitiveTopologyEXT CmdSetPrimitiveTopologyEXT;
    };
        union {
        PFN_vkCmdSetViewportWithCount CmdSetViewportWithCount;
        PFN_vkCmdSetViewportWithCountEXT CmdSetViewportWithCountEXT;
    };
        union {
        PFN_vkCmdSetScissorWithCount CmdSetScissorWithCount;
        PFN_vkCmdSetScissorWithCountEXT CmdSetScissorWithCountEXT;
    };
        union {
        PFN_vkCmdBindVertexBuffers2 CmdBindVertexBuffers2;
        PFN_vkCmdBindVertexBuffers2EXT CmdBindVertexBuffers2EXT;
    };
        union {
        PFN_vkCmdSetDepthTestEnable CmdSetDepthTestEnable;
        PFN_vkCmdSetDepthTestEnableEXT CmdSetDepthTestEnableEXT;
    };
        union {
        PFN_vkCmdSetDepthWriteEnable CmdSetDepthWriteEnable;
        PFN_vkCmdSetDepthWriteEnableEXT CmdSetDepthWriteEnableEXT;
    };
        union {
        PFN_vkCmdSetDepthCompareOp CmdSetDepthCompareOp;
        PFN_vkCmdSetDepthCompareOpEXT CmdSetDepthCompareOpEXT;
    };
        union {
        PFN_vkCmdSetDepthBoundsTestEnable CmdSetDepthBoundsTestEnable;
        PFN_vkCmdSetDepthBoundsTestEnableEXT CmdSetDepthBoundsTestEnableEXT;
    };
        union {
        PFN_vkCmdSetStencilTestEnable CmdSetStencilTestEnable;
        PFN_vkCmdSetStencilTestEnableEXT CmdSetStencilTestEnableEXT;
    };
        union {
        PFN_vkCmdSetStencilOp CmdSetStencilOp;
        PFN_vkCmdSetStencilOpEXT CmdSetStencilOpEXT;
    };
        PFN_vkCmdSetPatchControlPointsEXT CmdSetPatchControlPointsEXT;
    union {
        PFN_vkCmdSetRasterizerDiscardEnable CmdSetRasterizerDiscardEnable;
        PFN_vkCmdSetRasterizerDiscardEnableEXT CmdSetRasterizerDiscardEnableEXT;
    };
        union {
        PFN_vkCmdSetDepthBiasEnable CmdSetDepthBiasEnable;
        PFN_vkCmdSetDepthBiasEnableEXT CmdSetDepthBiasEnableEXT;
    };
        PFN_vkCmdSetLogicOpEXT CmdSetLogicOpEXT;
    union {
        PFN_vkCmdSetPrimitiveRestartEnable CmdSetPrimitiveRestartEnable;
        PFN_vkCmdSetPrimitiveRestartEnableEXT CmdSetPrimitiveRestartEnableEXT;
    };
        PFN_vkCmdSetTessellationDomainOriginEXT CmdSetTessellationDomainOriginEXT;
    PFN_vkCmdSetDepthClampEnableEXT CmdSetDepthClampEnableEXT;
    PFN_vkCmdSetPolygonModeEXT CmdSetPolygonModeEXT;
    PFN_vkCmdSetRasterizationSamplesEXT CmdSetRasterizationSamplesEXT;
    PFN_vkCmdSetSampleMaskEXT CmdSetSampleMaskEXT;
    PFN_vkCmdSetAlphaToCoverageEnableEXT CmdSetAlphaToCoverageEnableEXT;
    PFN_vkCmdSetAlphaToOneEnableEXT CmdSetAlphaToOneEnableEXT;
    PFN_vkCmdSetLogicOpEnableEXT CmdSetLogicOpEnableEXT;
    PFN_vkCmdSetColorBlendEnableEXT CmdSetColorBlendEnableEXT;
    PFN_vkCmdSetColorBlendEquationEXT CmdSetColorBlendEquationEXT;
    PFN_vkCmdSetColorWriteMaskEXT CmdSetColorWriteMaskEXT;
    PFN_vkCmdSetRasterizationStreamEXT CmdSetRasterizationStreamEXT;
    PFN_vkCmdSetConservativeRasterizationModeEXT CmdSetConservativeRasterizationModeEXT;
    PFN_vkCmdSetExtraPrimitiveOverestimationSizeEXT CmdSetExtraPrimitiveOverestimationSizeEXT;
    PFN_vkCmdSetDepthClipEnableEXT CmdSetDepthClipEnableEXT;
    PFN_vkCmdSetSampleLocationsEnableEXT CmdSetSampleLocationsEnableEXT;
    PFN_vkCmdSetColorBlendAdvancedEXT CmdSetColorBlendAdvancedEXT;
    PFN_vkCmdSetProvokingVertexModeEXT CmdSetProvokingVertexModeEXT;
    PFN_vkCmdSetLineRasterizationModeEXT CmdSetLineRasterizationModeEXT;
    PFN_vkCmdSetLineStippleEnableEXT CmdSetLineStippleEnableEXT;
    PFN_vkCmdSetDepthClipNegativeOneToOneEXT CmdSetDepthClipNegativeOneToOneEXT;
    PFN_vkCmdSetViewportWScalingEnableNV CmdSetViewportWScalingEnableNV;
    PFN_vkCmdSetViewportSwizzleNV CmdSetViewportSwizzleNV;
    PFN_vkCmdSetCoverageToColorEnableNV CmdSetCoverageToColorEnableNV;
    PFN_vkCmdSetCoverageToColorLocationNV CmdSetCoverageToColorLocationNV;
    PFN_vkCmdSetCoverageModulationModeNV CmdSetCoverageModulationModeNV;
    PFN_vkCmdSetCoverageModulationTableEnableNV CmdSetCoverageModulationTableEnableNV;
    PFN_vkCmdSetCoverageModulationTableNV CmdSetCoverageModulationTableNV;
    PFN_vkCmdSetShadingRateImageEnableNV CmdSetShadingRateImageEnableNV;
    PFN_vkCmdSetCoverageReductionModeNV CmdSetCoverageReductionModeNV;
    PFN_vkCmdSetRepresentativeFragmentTestEnableNV CmdSetRepresentativeFragmentTestEnableNV;
    union {
        PFN_vkCreatePrivateDataSlot CreatePrivateDataSlot;
        PFN_vkCreatePrivateDataSlotEXT CreatePrivateDataSlotEXT;
    };
        union {
        PFN_vkDestroyPrivateDataSlot DestroyPrivateDataSlot;
        PFN_vkDestroyPrivateDataSlotEXT DestroyPrivateDataSlotEXT;
    };
        union {
        PFN_vkSetPrivateData SetPrivateData;
        PFN_vkSetPrivateDataEXT SetPrivateDataEXT;
    };
        union {
        PFN_vkGetPrivateData GetPrivateData;
        PFN_vkGetPrivateDataEXT GetPrivateDataEXT;
    };
        union {
        PFN_vkCmdCopyBuffer2 CmdCopyBuffer2;
        PFN_vkCmdCopyBuffer2KHR CmdCopyBuffer2KHR;
    };
        union {
        PFN_vkCmdCopyImage2 CmdCopyImage2;
        PFN_vkCmdCopyImage2KHR CmdCopyImage2KHR;
    };
        union {
        PFN_vkCmdBlitImage2 CmdBlitImage2;
        PFN_vkCmdBlitImage2KHR CmdBlitImage2KHR;
    };
        union {
        PFN_vkCmdCopyBufferToImage2 CmdCopyBufferToImage2;
        PFN_vkCmdCopyBufferToImage2KHR CmdCopyBufferToImage2KHR;
    };
        union {
        PFN_vkCmdCopyImageToBuffer2 CmdCopyImageToBuffer2;
        PFN_vkCmdCopyImageToBuffer2KHR CmdCopyImageToBuffer2KHR;
    };
        union {
        PFN_vkCmdResolveImage2 CmdResolveImage2;
        PFN_vkCmdResolveImage2KHR CmdResolveImage2KHR;
    };
        PFN_vkCmdSetFragmentShadingRateKHR CmdSetFragmentShadingRateKHR;
    PFN_vkCmdSetFragmentShadingRateEnumNV CmdSetFragmentShadingRateEnumNV;
    PFN_vkGetAccelerationStructureBuildSizesKHR GetAccelerationStructureBuildSizesKHR;
    PFN_vkCmdSetVertexInputEXT CmdSetVertexInputEXT;
    PFN_vkCmdSetColorWriteEnableEXT CmdSetColorWriteEnableEXT;
    union {
        PFN_vkCmdSetEvent2 CmdSetEvent2;
        PFN_vkCmdSetEvent2KHR CmdSetEvent2KHR;
    };
        union {
        PFN_vkCmdResetEvent2 CmdResetEvent2;
        PFN_vkCmdResetEvent2KHR CmdResetEvent2KHR;
    };
        union {
        PFN_vkCmdWaitEvents2 CmdWaitEvents2;
        PFN_vkCmdWaitEvents2KHR CmdWaitEvents2KHR;
    };
        union {
        PFN_vkCmdPipelineBarrier2 CmdPipelineBarrier2;
        PFN_vkCmdPipelineBarrier2KHR CmdPipelineBarrier2KHR;
    };
        union {
        PFN_vkQueueSubmit2 QueueSubmit2;
        PFN_vkQueueSubmit2KHR QueueSubmit2KHR;
    };
        union {
        PFN_vkCmdWriteTimestamp2 CmdWriteTimestamp2;
        PFN_vkCmdWriteTimestamp2KHR CmdWriteTimestamp2KHR;
    };
        PFN_vkCmdWriteBufferMarker2AMD CmdWriteBufferMarker2AMD;
    PFN_vkGetQueueCheckpointData2NV GetQueueCheckpointData2NV;
    PFN_vkCreateVideoSessionKHR CreateVideoSessionKHR;
    PFN_vkDestroyVideoSessionKHR DestroyVideoSessionKHR;
    PFN_vkCreateVideoSessionParametersKHR CreateVideoSessionParametersKHR;
    PFN_vkUpdateVideoSessionParametersKHR UpdateVideoSessionParametersKHR;
    PFN_vkDestroyVideoSessionParametersKHR DestroyVideoSessionParametersKHR;
    PFN_vkGetVideoSessionMemoryRequirementsKHR GetVideoSessionMemoryRequirementsKHR;
    PFN_vkBindVideoSessionMemoryKHR BindVideoSessionMemoryKHR;
    PFN_vkCmdDecodeVideoKHR CmdDecodeVideoKHR;
    PFN_vkCmdBeginVideoCodingKHR CmdBeginVideoCodingKHR;
    PFN_vkCmdControlVideoCodingKHR CmdControlVideoCodingKHR;
    PFN_vkCmdEndVideoCodingKHR CmdEndVideoCodingKHR;
    PFN_vkCmdDecompressMemoryNV CmdDecompressMemoryNV;
    PFN_vkCmdDecompressMemoryIndirectCountNV CmdDecompressMemoryIndirectCountNV;
    PFN_vkCreateCuModuleNVX CreateCuModuleNVX;
    PFN_vkCreateCuFunctionNVX CreateCuFunctionNVX;
    PFN_vkDestroyCuModuleNVX DestroyCuModuleNVX;
    PFN_vkDestroyCuFunctionNVX DestroyCuFunctionNVX;
    PFN_vkCmdCuLaunchKernelNVX CmdCuLaunchKernelNVX;
    PFN_vkGetDescriptorSetLayoutSizeEXT GetDescriptorSetLayoutSizeEXT;
    PFN_vkGetDescriptorSetLayoutBindingOffsetEXT GetDescriptorSetLayoutBindingOffsetEXT;
    PFN_vkGetDescriptorEXT GetDescriptorEXT;
    PFN_vkCmdBindDescriptorBuffersEXT CmdBindDescriptorBuffersEXT;
    PFN_vkCmdSetDescriptorBufferOffsetsEXT CmdSetDescriptorBufferOffsetsEXT;
    PFN_vkCmdBindDescriptorBufferEmbeddedSamplersEXT CmdBindDescriptorBufferEmbeddedSamplersEXT;
    PFN_vkGetBufferOpaqueCaptureDescriptorDataEXT GetBufferOpaqueCaptureDescriptorDataEXT;
    PFN_vkGetImageOpaqueCaptureDescriptorDataEXT GetImageOpaqueCaptureDescriptorDataEXT;
    PFN_vkGetImageViewOpaqueCaptureDescriptorDataEXT GetImageViewOpaqueCaptureDescriptorDataEXT;
    PFN_vkGetSamplerOpaqueCaptureDescriptorDataEXT GetSamplerOpaqueCaptureDescriptorDataEXT;
    PFN_vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT GetAccelerationStructureOpaqueCaptureDescriptorDataEXT;
    PFN_vkSetDeviceMemoryPriorityEXT SetDeviceMemoryPriorityEXT;
    PFN_vkWaitForPresentKHR WaitForPresentKHR;
#ifdef VK_USE_PLATFORM_FUCHSIA
    PFN_vkCreateBufferCollectionFUCHSIA CreateBufferCollectionFUCHSIA;
#else
    PFN_vkVoidFunction CreateBufferCollectionFUCHSIA;
#endif
#ifdef VK_USE_PLATFORM_FUCHSIA
    PFN_vkSetBufferCollectionBufferConstraintsFUCHSIA SetBufferCollectionBufferConstraintsFUCHSIA;
#else
    PFN_vkVoidFunction SetBufferCollectionBufferConstraintsFUCHSIA;
#endif
#ifdef VK_USE_PLATFORM_FUCHSIA
    PFN_vkSetBufferCollectionImageConstraintsFUCHSIA SetBufferCollectionImageConstraintsFUCHSIA;
#else
    PFN_vkVoidFunction SetBufferCollectionImageConstraintsFUCHSIA;
#endif
#ifdef VK_USE_PLATFORM_FUCHSIA
    PFN_vkDestroyBufferCollectionFUCHSIA DestroyBufferCollectionFUCHSIA;
#else
    PFN_vkVoidFunction DestroyBufferCollectionFUCHSIA;
#endif
#ifdef VK_USE_PLATFORM_FUCHSIA
    PFN_vkGetBufferCollectionPropertiesFUCHSIA GetBufferCollectionPropertiesFUCHSIA;
#else
    PFN_vkVoidFunction GetBufferCollectionPropertiesFUCHSIA;
#endif
    union {
        PFN_vkCmdBeginRendering CmdBeginRendering;
        PFN_vkCmdBeginRenderingKHR CmdBeginRenderingKHR;
    };
        union {
        PFN_vkCmdEndRendering CmdEndRendering;
        PFN_vkCmdEndRenderingKHR CmdEndRenderingKHR;
    };
        PFN_vkGetDescriptorSetLayoutHostMappingInfoVALVE GetDescriptorSetLayoutHostMappingInfoVALVE;
    PFN_vkGetDescriptorSetHostMappingVALVE GetDescriptorSetHostMappingVALVE;
    PFN_vkCreateMicromapEXT CreateMicromapEXT;
    PFN_vkCmdBuildMicromapsEXT CmdBuildMicromapsEXT;
    PFN_vkBuildMicromapsEXT BuildMicromapsEXT;
    PFN_vkDestroyMicromapEXT DestroyMicromapEXT;
    PFN_vkCmdCopyMicromapEXT CmdCopyMicromapEXT;
    PFN_vkCopyMicromapEXT CopyMicromapEXT;
    PFN_vkCmdCopyMicromapToMemoryEXT CmdCopyMicromapToMemoryEXT;
    PFN_vkCopyMicromapToMemoryEXT CopyMicromapToMemoryEXT;
    PFN_vkCmdCopyMemoryToMicromapEXT CmdCopyMemoryToMicromapEXT;
    PFN_vkCopyMemoryToMicromapEXT CopyMemoryToMicromapEXT;
    PFN_vkCmdWriteMicromapsPropertiesEXT CmdWriteMicromapsPropertiesEXT;
    PFN_vkWriteMicromapsPropertiesEXT WriteMicromapsPropertiesEXT;
    PFN_vkGetDeviceMicromapCompatibilityEXT GetDeviceMicromapCompatibilityEXT;
    PFN_vkGetMicromapBuildSizesEXT GetMicromapBuildSizesEXT;
    PFN_vkGetShaderModuleIdentifierEXT GetShaderModuleIdentifierEXT;
    PFN_vkGetShaderModuleCreateInfoIdentifierEXT GetShaderModuleCreateInfoIdentifierEXT;
    PFN_vkGetImageSubresourceLayout2EXT GetImageSubresourceLayout2EXT;
    PFN_vkGetPipelinePropertiesEXT GetPipelinePropertiesEXT;
#ifdef VK_USE_PLATFORM_METAL_EXT
    PFN_vkExportMetalObjectsEXT ExportMetalObjectsEXT;
#else
    PFN_vkVoidFunction ExportMetalObjectsEXT;
#endif
    PFN_vkGetFramebufferTilePropertiesQCOM GetFramebufferTilePropertiesQCOM;
    PFN_vkGetDynamicRenderingTilePropertiesQCOM GetDynamicRenderingTilePropertiesQCOM;
    PFN_vkCreateOpticalFlowSessionNV CreateOpticalFlowSessionNV;
    PFN_vkDestroyOpticalFlowSessionNV DestroyOpticalFlowSessionNV;
    PFN_vkBindOpticalFlowSessionImageNV BindOpticalFlowSessionImageNV;
    PFN_vkCmdOpticalFlowExecuteNV CmdOpticalFlowExecuteNV;
    PFN_vkGetDeviceFaultInfoEXT GetDeviceFaultInfoEXT;
    PFN_vkReleaseSwapchainImagesEXT ReleaseSwapchainImagesEXT;
    PFN_vkMapMemory2KHR MapMemory2KHR;
    PFN_vkUnmapMemory2KHR UnmapMemory2KHR;
    PFN_vkCreateShadersEXT CreateShadersEXT;
    PFN_vkDestroyShaderEXT DestroyShaderEXT;
    PFN_vkGetShaderBinaryDataEXT GetShaderBinaryDataEXT;
    PFN_vkCmdBindShadersEXT CmdBindShadersEXT;

        };
    };
};


struct vk_instance_entrypoint_table {
    PFN_vkCreateInstance CreateInstance;
    PFN_vkDestroyInstance DestroyInstance;
    PFN_vkEnumeratePhysicalDevices EnumeratePhysicalDevices;
    PFN_vkGetInstanceProcAddr GetInstanceProcAddr;
    PFN_vkEnumerateInstanceVersion EnumerateInstanceVersion;
    PFN_vkEnumerateInstanceLayerProperties EnumerateInstanceLayerProperties;
    PFN_vkEnumerateInstanceExtensionProperties EnumerateInstanceExtensionProperties;
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    PFN_vkCreateAndroidSurfaceKHR CreateAndroidSurfaceKHR;
#else
    PFN_vkVoidFunction CreateAndroidSurfaceKHR;
# endif
    PFN_vkCreateDisplayPlaneSurfaceKHR CreateDisplayPlaneSurfaceKHR;
    PFN_vkDestroySurfaceKHR DestroySurfaceKHR;
#ifdef VK_USE_PLATFORM_VI_NN
    PFN_vkCreateViSurfaceNN CreateViSurfaceNN;
#else
    PFN_vkVoidFunction CreateViSurfaceNN;
# endif
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
    PFN_vkCreateWaylandSurfaceKHR CreateWaylandSurfaceKHR;
#else
    PFN_vkVoidFunction CreateWaylandSurfaceKHR;
# endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
    PFN_vkCreateWin32SurfaceKHR CreateWin32SurfaceKHR;
#else
    PFN_vkVoidFunction CreateWin32SurfaceKHR;
# endif
#ifdef VK_USE_PLATFORM_XLIB_KHR
    PFN_vkCreateXlibSurfaceKHR CreateXlibSurfaceKHR;
#else
    PFN_vkVoidFunction CreateXlibSurfaceKHR;
# endif
#ifdef VK_USE_PLATFORM_XCB_KHR
    PFN_vkCreateXcbSurfaceKHR CreateXcbSurfaceKHR;
#else
    PFN_vkVoidFunction CreateXcbSurfaceKHR;
# endif
#ifdef VK_USE_PLATFORM_DIRECTFB_EXT
    PFN_vkCreateDirectFBSurfaceEXT CreateDirectFBSurfaceEXT;
#else
    PFN_vkVoidFunction CreateDirectFBSurfaceEXT;
# endif
#ifdef VK_USE_PLATFORM_FUCHSIA
    PFN_vkCreateImagePipeSurfaceFUCHSIA CreateImagePipeSurfaceFUCHSIA;
#else
    PFN_vkVoidFunction CreateImagePipeSurfaceFUCHSIA;
# endif
#ifdef VK_USE_PLATFORM_GGP
    PFN_vkCreateStreamDescriptorSurfaceGGP CreateStreamDescriptorSurfaceGGP;
#else
    PFN_vkVoidFunction CreateStreamDescriptorSurfaceGGP;
# endif
#ifdef VK_USE_PLATFORM_SCREEN_QNX
    PFN_vkCreateScreenSurfaceQNX CreateScreenSurfaceQNX;
#else
    PFN_vkVoidFunction CreateScreenSurfaceQNX;
# endif
    PFN_vkCreateDebugReportCallbackEXT CreateDebugReportCallbackEXT;
    PFN_vkDestroyDebugReportCallbackEXT DestroyDebugReportCallbackEXT;
    PFN_vkDebugReportMessageEXT DebugReportMessageEXT;
    PFN_vkEnumeratePhysicalDeviceGroups EnumeratePhysicalDeviceGroups;
    PFN_vkEnumeratePhysicalDeviceGroupsKHR EnumeratePhysicalDeviceGroupsKHR;
#ifdef VK_USE_PLATFORM_IOS_MVK
    PFN_vkCreateIOSSurfaceMVK CreateIOSSurfaceMVK;
#else
    PFN_vkVoidFunction CreateIOSSurfaceMVK;
# endif
#ifdef VK_USE_PLATFORM_MACOS_MVK
    PFN_vkCreateMacOSSurfaceMVK CreateMacOSSurfaceMVK;
#else
    PFN_vkVoidFunction CreateMacOSSurfaceMVK;
# endif
#ifdef VK_USE_PLATFORM_METAL_EXT
    PFN_vkCreateMetalSurfaceEXT CreateMetalSurfaceEXT;
#else
    PFN_vkVoidFunction CreateMetalSurfaceEXT;
# endif
    PFN_vkCreateDebugUtilsMessengerEXT CreateDebugUtilsMessengerEXT;
    PFN_vkDestroyDebugUtilsMessengerEXT DestroyDebugUtilsMessengerEXT;
    PFN_vkSubmitDebugUtilsMessageEXT SubmitDebugUtilsMessageEXT;
    PFN_vkCreateHeadlessSurfaceEXT CreateHeadlessSurfaceEXT;
};


struct vk_physical_device_entrypoint_table {
    PFN_vkGetPhysicalDeviceProperties GetPhysicalDeviceProperties;
    PFN_vkGetPhysicalDeviceQueueFamilyProperties GetPhysicalDeviceQueueFamilyProperties;
    PFN_vkGetPhysicalDeviceMemoryProperties GetPhysicalDeviceMemoryProperties;
    PFN_vkGetPhysicalDeviceFeatures GetPhysicalDeviceFeatures;
    PFN_vkGetPhysicalDeviceFormatProperties GetPhysicalDeviceFormatProperties;
    PFN_vkGetPhysicalDeviceImageFormatProperties GetPhysicalDeviceImageFormatProperties;
    PFN_vkCreateDevice CreateDevice;
    PFN_vkEnumerateDeviceLayerProperties EnumerateDeviceLayerProperties;
    PFN_vkEnumerateDeviceExtensionProperties EnumerateDeviceExtensionProperties;
    PFN_vkGetPhysicalDeviceSparseImageFormatProperties GetPhysicalDeviceSparseImageFormatProperties;
    PFN_vkGetPhysicalDeviceDisplayPropertiesKHR GetPhysicalDeviceDisplayPropertiesKHR;
    PFN_vkGetPhysicalDeviceDisplayPlanePropertiesKHR GetPhysicalDeviceDisplayPlanePropertiesKHR;
    PFN_vkGetDisplayPlaneSupportedDisplaysKHR GetDisplayPlaneSupportedDisplaysKHR;
    PFN_vkGetDisplayModePropertiesKHR GetDisplayModePropertiesKHR;
    PFN_vkCreateDisplayModeKHR CreateDisplayModeKHR;
    PFN_vkGetDisplayPlaneCapabilitiesKHR GetDisplayPlaneCapabilitiesKHR;
    PFN_vkGetPhysicalDeviceSurfaceSupportKHR GetPhysicalDeviceSurfaceSupportKHR;
    PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR GetPhysicalDeviceSurfaceCapabilitiesKHR;
    PFN_vkGetPhysicalDeviceSurfaceFormatsKHR GetPhysicalDeviceSurfaceFormatsKHR;
    PFN_vkGetPhysicalDeviceSurfacePresentModesKHR GetPhysicalDeviceSurfacePresentModesKHR;
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
    PFN_vkGetPhysicalDeviceWaylandPresentationSupportKHR GetPhysicalDeviceWaylandPresentationSupportKHR;
#else
    PFN_vkVoidFunction GetPhysicalDeviceWaylandPresentationSupportKHR;
# endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
    PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR GetPhysicalDeviceWin32PresentationSupportKHR;
#else
    PFN_vkVoidFunction GetPhysicalDeviceWin32PresentationSupportKHR;
# endif
#ifdef VK_USE_PLATFORM_XLIB_KHR
    PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR GetPhysicalDeviceXlibPresentationSupportKHR;
#else
    PFN_vkVoidFunction GetPhysicalDeviceXlibPresentationSupportKHR;
# endif
#ifdef VK_USE_PLATFORM_XCB_KHR
    PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR GetPhysicalDeviceXcbPresentationSupportKHR;
#else
    PFN_vkVoidFunction GetPhysicalDeviceXcbPresentationSupportKHR;
# endif
#ifdef VK_USE_PLATFORM_DIRECTFB_EXT
    PFN_vkGetPhysicalDeviceDirectFBPresentationSupportEXT GetPhysicalDeviceDirectFBPresentationSupportEXT;
#else
    PFN_vkVoidFunction GetPhysicalDeviceDirectFBPresentationSupportEXT;
# endif
#ifdef VK_USE_PLATFORM_SCREEN_QNX
    PFN_vkGetPhysicalDeviceScreenPresentationSupportQNX GetPhysicalDeviceScreenPresentationSupportQNX;
#else
    PFN_vkVoidFunction GetPhysicalDeviceScreenPresentationSupportQNX;
# endif
    PFN_vkGetPhysicalDeviceExternalImageFormatPropertiesNV GetPhysicalDeviceExternalImageFormatPropertiesNV;
    PFN_vkGetPhysicalDeviceFeatures2 GetPhysicalDeviceFeatures2;
    PFN_vkGetPhysicalDeviceFeatures2KHR GetPhysicalDeviceFeatures2KHR;
    PFN_vkGetPhysicalDeviceProperties2 GetPhysicalDeviceProperties2;
    PFN_vkGetPhysicalDeviceProperties2KHR GetPhysicalDeviceProperties2KHR;
    PFN_vkGetPhysicalDeviceFormatProperties2 GetPhysicalDeviceFormatProperties2;
    PFN_vkGetPhysicalDeviceFormatProperties2KHR GetPhysicalDeviceFormatProperties2KHR;
    PFN_vkGetPhysicalDeviceImageFormatProperties2 GetPhysicalDeviceImageFormatProperties2;
    PFN_vkGetPhysicalDeviceImageFormatProperties2KHR GetPhysicalDeviceImageFormatProperties2KHR;
    PFN_vkGetPhysicalDeviceQueueFamilyProperties2 GetPhysicalDeviceQueueFamilyProperties2;
    PFN_vkGetPhysicalDeviceQueueFamilyProperties2KHR GetPhysicalDeviceQueueFamilyProperties2KHR;
    PFN_vkGetPhysicalDeviceMemoryProperties2 GetPhysicalDeviceMemoryProperties2;
    PFN_vkGetPhysicalDeviceMemoryProperties2KHR GetPhysicalDeviceMemoryProperties2KHR;
    PFN_vkGetPhysicalDeviceSparseImageFormatProperties2 GetPhysicalDeviceSparseImageFormatProperties2;
    PFN_vkGetPhysicalDeviceSparseImageFormatProperties2KHR GetPhysicalDeviceSparseImageFormatProperties2KHR;
    PFN_vkGetPhysicalDeviceExternalBufferProperties GetPhysicalDeviceExternalBufferProperties;
    PFN_vkGetPhysicalDeviceExternalBufferPropertiesKHR GetPhysicalDeviceExternalBufferPropertiesKHR;
    PFN_vkGetPhysicalDeviceExternalSemaphoreProperties GetPhysicalDeviceExternalSemaphoreProperties;
    PFN_vkGetPhysicalDeviceExternalSemaphorePropertiesKHR GetPhysicalDeviceExternalSemaphorePropertiesKHR;
    PFN_vkGetPhysicalDeviceExternalFenceProperties GetPhysicalDeviceExternalFenceProperties;
    PFN_vkGetPhysicalDeviceExternalFencePropertiesKHR GetPhysicalDeviceExternalFencePropertiesKHR;
    PFN_vkReleaseDisplayEXT ReleaseDisplayEXT;
#ifdef VK_USE_PLATFORM_XLIB_XRANDR_EXT
    PFN_vkAcquireXlibDisplayEXT AcquireXlibDisplayEXT;
#else
    PFN_vkVoidFunction AcquireXlibDisplayEXT;
# endif
#ifdef VK_USE_PLATFORM_XLIB_XRANDR_EXT
    PFN_vkGetRandROutputDisplayEXT GetRandROutputDisplayEXT;
#else
    PFN_vkVoidFunction GetRandROutputDisplayEXT;
# endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
    PFN_vkAcquireWinrtDisplayNV AcquireWinrtDisplayNV;
#else
    PFN_vkVoidFunction AcquireWinrtDisplayNV;
# endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
    PFN_vkGetWinrtDisplayNV GetWinrtDisplayNV;
#else
    PFN_vkVoidFunction GetWinrtDisplayNV;
# endif
    PFN_vkGetPhysicalDeviceSurfaceCapabilities2EXT GetPhysicalDeviceSurfaceCapabilities2EXT;
    PFN_vkGetPhysicalDevicePresentRectanglesKHR GetPhysicalDevicePresentRectanglesKHR;
    PFN_vkGetPhysicalDeviceMultisamplePropertiesEXT GetPhysicalDeviceMultisamplePropertiesEXT;
    PFN_vkGetPhysicalDeviceSurfaceCapabilities2KHR GetPhysicalDeviceSurfaceCapabilities2KHR;
    PFN_vkGetPhysicalDeviceSurfaceFormats2KHR GetPhysicalDeviceSurfaceFormats2KHR;
    PFN_vkGetPhysicalDeviceDisplayProperties2KHR GetPhysicalDeviceDisplayProperties2KHR;
    PFN_vkGetPhysicalDeviceDisplayPlaneProperties2KHR GetPhysicalDeviceDisplayPlaneProperties2KHR;
    PFN_vkGetDisplayModeProperties2KHR GetDisplayModeProperties2KHR;
    PFN_vkGetDisplayPlaneCapabilities2KHR GetDisplayPlaneCapabilities2KHR;
    PFN_vkGetPhysicalDeviceCalibrateableTimeDomainsEXT GetPhysicalDeviceCalibrateableTimeDomainsEXT;
    PFN_vkGetPhysicalDeviceCooperativeMatrixPropertiesNV GetPhysicalDeviceCooperativeMatrixPropertiesNV;
#ifdef VK_USE_PLATFORM_WIN32_KHR
    PFN_vkGetPhysicalDeviceSurfacePresentModes2EXT GetPhysicalDeviceSurfacePresentModes2EXT;
#else
    PFN_vkVoidFunction GetPhysicalDeviceSurfacePresentModes2EXT;
# endif
    PFN_vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR EnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR;
    PFN_vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR GetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR;
    PFN_vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV GetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV;
    PFN_vkGetPhysicalDeviceToolProperties GetPhysicalDeviceToolProperties;
    PFN_vkGetPhysicalDeviceToolPropertiesEXT GetPhysicalDeviceToolPropertiesEXT;
    PFN_vkGetPhysicalDeviceFragmentShadingRatesKHR GetPhysicalDeviceFragmentShadingRatesKHR;
    PFN_vkGetPhysicalDeviceVideoCapabilitiesKHR GetPhysicalDeviceVideoCapabilitiesKHR;
    PFN_vkGetPhysicalDeviceVideoFormatPropertiesKHR GetPhysicalDeviceVideoFormatPropertiesKHR;
    PFN_vkAcquireDrmDisplayEXT AcquireDrmDisplayEXT;
    PFN_vkGetDrmDisplayEXT GetDrmDisplayEXT;
    PFN_vkGetPhysicalDeviceOpticalFlowImageFormatsNV GetPhysicalDeviceOpticalFlowImageFormatsNV;
};


struct vk_device_entrypoint_table {
    PFN_vkGetDeviceProcAddr GetDeviceProcAddr;
    PFN_vkDestroyDevice DestroyDevice;
    PFN_vkGetDeviceQueue GetDeviceQueue;
    PFN_vkQueueSubmit QueueSubmit;
    PFN_vkQueueWaitIdle QueueWaitIdle;
    PFN_vkDeviceWaitIdle DeviceWaitIdle;
    PFN_vkAllocateMemory AllocateMemory;
    PFN_vkFreeMemory FreeMemory;
    PFN_vkMapMemory MapMemory;
    PFN_vkUnmapMemory UnmapMemory;
    PFN_vkFlushMappedMemoryRanges FlushMappedMemoryRanges;
    PFN_vkInvalidateMappedMemoryRanges InvalidateMappedMemoryRanges;
    PFN_vkGetDeviceMemoryCommitment GetDeviceMemoryCommitment;
    PFN_vkGetBufferMemoryRequirements GetBufferMemoryRequirements;
    PFN_vkBindBufferMemory BindBufferMemory;
    PFN_vkGetImageMemoryRequirements GetImageMemoryRequirements;
    PFN_vkBindImageMemory BindImageMemory;
    PFN_vkGetImageSparseMemoryRequirements GetImageSparseMemoryRequirements;
    PFN_vkQueueBindSparse QueueBindSparse;
    PFN_vkCreateFence CreateFence;
    PFN_vkDestroyFence DestroyFence;
    PFN_vkResetFences ResetFences;
    PFN_vkGetFenceStatus GetFenceStatus;
    PFN_vkWaitForFences WaitForFences;
    PFN_vkCreateSemaphore CreateSemaphore;
    PFN_vkDestroySemaphore DestroySemaphore;
    PFN_vkCreateEvent CreateEvent;
    PFN_vkDestroyEvent DestroyEvent;
    PFN_vkGetEventStatus GetEventStatus;
    PFN_vkSetEvent SetEvent;
    PFN_vkResetEvent ResetEvent;
    PFN_vkCreateQueryPool CreateQueryPool;
    PFN_vkDestroyQueryPool DestroyQueryPool;
    PFN_vkGetQueryPoolResults GetQueryPoolResults;
    PFN_vkResetQueryPool ResetQueryPool;
    PFN_vkResetQueryPoolEXT ResetQueryPoolEXT;
    PFN_vkCreateBuffer CreateBuffer;
    PFN_vkDestroyBuffer DestroyBuffer;
    PFN_vkCreateBufferView CreateBufferView;
    PFN_vkDestroyBufferView DestroyBufferView;
    PFN_vkCreateImage CreateImage;
    PFN_vkDestroyImage DestroyImage;
    PFN_vkGetImageSubresourceLayout GetImageSubresourceLayout;
    PFN_vkCreateImageView CreateImageView;
    PFN_vkDestroyImageView DestroyImageView;
    PFN_vkCreateShaderModule CreateShaderModule;
    PFN_vkDestroyShaderModule DestroyShaderModule;
    PFN_vkCreatePipelineCache CreatePipelineCache;
    PFN_vkDestroyPipelineCache DestroyPipelineCache;
    PFN_vkGetPipelineCacheData GetPipelineCacheData;
    PFN_vkMergePipelineCaches MergePipelineCaches;
    PFN_vkCreateGraphicsPipelines CreateGraphicsPipelines;
    PFN_vkCreateComputePipelines CreateComputePipelines;
    PFN_vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI GetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI;
    PFN_vkDestroyPipeline DestroyPipeline;
    PFN_vkCreatePipelineLayout CreatePipelineLayout;
    PFN_vkDestroyPipelineLayout DestroyPipelineLayout;
    PFN_vkCreateSampler CreateSampler;
    PFN_vkDestroySampler DestroySampler;
    PFN_vkCreateDescriptorSetLayout CreateDescriptorSetLayout;
    PFN_vkDestroyDescriptorSetLayout DestroyDescriptorSetLayout;
    PFN_vkCreateDescriptorPool CreateDescriptorPool;
    PFN_vkDestroyDescriptorPool DestroyDescriptorPool;
    PFN_vkResetDescriptorPool ResetDescriptorPool;
    PFN_vkAllocateDescriptorSets AllocateDescriptorSets;
    PFN_vkFreeDescriptorSets FreeDescriptorSets;
    PFN_vkUpdateDescriptorSets UpdateDescriptorSets;
    PFN_vkCreateFramebuffer CreateFramebuffer;
    PFN_vkDestroyFramebuffer DestroyFramebuffer;
    PFN_vkCreateRenderPass CreateRenderPass;
    PFN_vkDestroyRenderPass DestroyRenderPass;
    PFN_vkGetRenderAreaGranularity GetRenderAreaGranularity;
    PFN_vkCreateCommandPool CreateCommandPool;
    PFN_vkDestroyCommandPool DestroyCommandPool;
    PFN_vkResetCommandPool ResetCommandPool;
    PFN_vkAllocateCommandBuffers AllocateCommandBuffers;
    PFN_vkFreeCommandBuffers FreeCommandBuffers;
    PFN_vkBeginCommandBuffer BeginCommandBuffer;
    PFN_vkEndCommandBuffer EndCommandBuffer;
    PFN_vkResetCommandBuffer ResetCommandBuffer;
    PFN_vkCmdBindPipeline CmdBindPipeline;
    PFN_vkCmdSetViewport CmdSetViewport;
    PFN_vkCmdSetScissor CmdSetScissor;
    PFN_vkCmdSetLineWidth CmdSetLineWidth;
    PFN_vkCmdSetDepthBias CmdSetDepthBias;
    PFN_vkCmdSetBlendConstants CmdSetBlendConstants;
    PFN_vkCmdSetDepthBounds CmdSetDepthBounds;
    PFN_vkCmdSetStencilCompareMask CmdSetStencilCompareMask;
    PFN_vkCmdSetStencilWriteMask CmdSetStencilWriteMask;
    PFN_vkCmdSetStencilReference CmdSetStencilReference;
    PFN_vkCmdBindDescriptorSets CmdBindDescriptorSets;
    PFN_vkCmdBindIndexBuffer CmdBindIndexBuffer;
    PFN_vkCmdBindVertexBuffers CmdBindVertexBuffers;
    PFN_vkCmdDraw CmdDraw;
    PFN_vkCmdDrawIndexed CmdDrawIndexed;
    PFN_vkCmdDrawMultiEXT CmdDrawMultiEXT;
    PFN_vkCmdDrawMultiIndexedEXT CmdDrawMultiIndexedEXT;
    PFN_vkCmdDrawIndirect CmdDrawIndirect;
    PFN_vkCmdDrawIndexedIndirect CmdDrawIndexedIndirect;
    PFN_vkCmdDispatch CmdDispatch;
    PFN_vkCmdDispatchIndirect CmdDispatchIndirect;
    PFN_vkCmdSubpassShadingHUAWEI CmdSubpassShadingHUAWEI;
    PFN_vkCmdDrawClusterHUAWEI CmdDrawClusterHUAWEI;
    PFN_vkCmdDrawClusterIndirectHUAWEI CmdDrawClusterIndirectHUAWEI;
    PFN_vkCmdCopyBuffer CmdCopyBuffer;
    PFN_vkCmdCopyImage CmdCopyImage;
    PFN_vkCmdBlitImage CmdBlitImage;
    PFN_vkCmdCopyBufferToImage CmdCopyBufferToImage;
    PFN_vkCmdCopyImageToBuffer CmdCopyImageToBuffer;
    PFN_vkCmdCopyMemoryIndirectNV CmdCopyMemoryIndirectNV;
    PFN_vkCmdCopyMemoryToImageIndirectNV CmdCopyMemoryToImageIndirectNV;
    PFN_vkCmdUpdateBuffer CmdUpdateBuffer;
    PFN_vkCmdFillBuffer CmdFillBuffer;
    PFN_vkCmdClearColorImage CmdClearColorImage;
    PFN_vkCmdClearDepthStencilImage CmdClearDepthStencilImage;
    PFN_vkCmdClearAttachments CmdClearAttachments;
    PFN_vkCmdResolveImage CmdResolveImage;
    PFN_vkCmdSetEvent CmdSetEvent;
    PFN_vkCmdResetEvent CmdResetEvent;
    PFN_vkCmdWaitEvents CmdWaitEvents;
    PFN_vkCmdPipelineBarrier CmdPipelineBarrier;
    PFN_vkCmdBeginQuery CmdBeginQuery;
    PFN_vkCmdEndQuery CmdEndQuery;
    PFN_vkCmdBeginConditionalRenderingEXT CmdBeginConditionalRenderingEXT;
    PFN_vkCmdEndConditionalRenderingEXT CmdEndConditionalRenderingEXT;
    PFN_vkCmdResetQueryPool CmdResetQueryPool;
    PFN_vkCmdWriteTimestamp CmdWriteTimestamp;
    PFN_vkCmdCopyQueryPoolResults CmdCopyQueryPoolResults;
    PFN_vkCmdPushConstants CmdPushConstants;
    PFN_vkCmdBeginRenderPass CmdBeginRenderPass;
    PFN_vkCmdNextSubpass CmdNextSubpass;
    PFN_vkCmdEndRenderPass CmdEndRenderPass;
    PFN_vkCmdExecuteCommands CmdExecuteCommands;
    PFN_vkCreateSharedSwapchainsKHR CreateSharedSwapchainsKHR;
    PFN_vkCreateSwapchainKHR CreateSwapchainKHR;
    PFN_vkDestroySwapchainKHR DestroySwapchainKHR;
    PFN_vkGetSwapchainImagesKHR GetSwapchainImagesKHR;
    PFN_vkAcquireNextImageKHR AcquireNextImageKHR;
    PFN_vkQueuePresentKHR QueuePresentKHR;
    PFN_vkDebugMarkerSetObjectNameEXT DebugMarkerSetObjectNameEXT;
    PFN_vkDebugMarkerSetObjectTagEXT DebugMarkerSetObjectTagEXT;
    PFN_vkCmdDebugMarkerBeginEXT CmdDebugMarkerBeginEXT;
    PFN_vkCmdDebugMarkerEndEXT CmdDebugMarkerEndEXT;
    PFN_vkCmdDebugMarkerInsertEXT CmdDebugMarkerInsertEXT;
#ifdef VK_USE_PLATFORM_WIN32_KHR
    PFN_vkGetMemoryWin32HandleNV GetMemoryWin32HandleNV;
#else
    PFN_vkVoidFunction GetMemoryWin32HandleNV;
# endif
    PFN_vkCmdExecuteGeneratedCommandsNV CmdExecuteGeneratedCommandsNV;
    PFN_vkCmdPreprocessGeneratedCommandsNV CmdPreprocessGeneratedCommandsNV;
    PFN_vkCmdBindPipelineShaderGroupNV CmdBindPipelineShaderGroupNV;
    PFN_vkGetGeneratedCommandsMemoryRequirementsNV GetGeneratedCommandsMemoryRequirementsNV;
    PFN_vkCreateIndirectCommandsLayoutNV CreateIndirectCommandsLayoutNV;
    PFN_vkDestroyIndirectCommandsLayoutNV DestroyIndirectCommandsLayoutNV;
    PFN_vkCmdPushDescriptorSetKHR CmdPushDescriptorSetKHR;
    PFN_vkTrimCommandPool TrimCommandPool;
    PFN_vkTrimCommandPoolKHR TrimCommandPoolKHR;
#ifdef VK_USE_PLATFORM_WIN32_KHR
    PFN_vkGetMemoryWin32HandleKHR GetMemoryWin32HandleKHR;
#else
    PFN_vkVoidFunction GetMemoryWin32HandleKHR;
# endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
    PFN_vkGetMemoryWin32HandlePropertiesKHR GetMemoryWin32HandlePropertiesKHR;
#else
    PFN_vkVoidFunction GetMemoryWin32HandlePropertiesKHR;
# endif
    PFN_vkGetMemoryFdKHR GetMemoryFdKHR;
    PFN_vkGetMemoryFdPropertiesKHR GetMemoryFdPropertiesKHR;
#ifdef VK_USE_PLATFORM_FUCHSIA
    PFN_vkGetMemoryZirconHandleFUCHSIA GetMemoryZirconHandleFUCHSIA;
#else
    PFN_vkVoidFunction GetMemoryZirconHandleFUCHSIA;
# endif
#ifdef VK_USE_PLATFORM_FUCHSIA
    PFN_vkGetMemoryZirconHandlePropertiesFUCHSIA GetMemoryZirconHandlePropertiesFUCHSIA;
#else
    PFN_vkVoidFunction GetMemoryZirconHandlePropertiesFUCHSIA;
# endif
    PFN_vkGetMemoryRemoteAddressNV GetMemoryRemoteAddressNV;
#ifdef VK_USE_PLATFORM_WIN32_KHR
    PFN_vkGetSemaphoreWin32HandleKHR GetSemaphoreWin32HandleKHR;
#else
    PFN_vkVoidFunction GetSemaphoreWin32HandleKHR;
# endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
    PFN_vkImportSemaphoreWin32HandleKHR ImportSemaphoreWin32HandleKHR;
#else
    PFN_vkVoidFunction ImportSemaphoreWin32HandleKHR;
# endif
    PFN_vkGetSemaphoreFdKHR GetSemaphoreFdKHR;
    PFN_vkImportSemaphoreFdKHR ImportSemaphoreFdKHR;
#ifdef VK_USE_PLATFORM_FUCHSIA
    PFN_vkGetSemaphoreZirconHandleFUCHSIA GetSemaphoreZirconHandleFUCHSIA;
#else
    PFN_vkVoidFunction GetSemaphoreZirconHandleFUCHSIA;
# endif
#ifdef VK_USE_PLATFORM_FUCHSIA
    PFN_vkImportSemaphoreZirconHandleFUCHSIA ImportSemaphoreZirconHandleFUCHSIA;
#else
    PFN_vkVoidFunction ImportSemaphoreZirconHandleFUCHSIA;
# endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
    PFN_vkGetFenceWin32HandleKHR GetFenceWin32HandleKHR;
#else
    PFN_vkVoidFunction GetFenceWin32HandleKHR;
# endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
    PFN_vkImportFenceWin32HandleKHR ImportFenceWin32HandleKHR;
#else
    PFN_vkVoidFunction ImportFenceWin32HandleKHR;
# endif
    PFN_vkGetFenceFdKHR GetFenceFdKHR;
    PFN_vkImportFenceFdKHR ImportFenceFdKHR;
    PFN_vkDisplayPowerControlEXT DisplayPowerControlEXT;
    PFN_vkRegisterDeviceEventEXT RegisterDeviceEventEXT;
    PFN_vkRegisterDisplayEventEXT RegisterDisplayEventEXT;
    PFN_vkGetSwapchainCounterEXT GetSwapchainCounterEXT;
    PFN_vkGetDeviceGroupPeerMemoryFeatures GetDeviceGroupPeerMemoryFeatures;
    PFN_vkGetDeviceGroupPeerMemoryFeaturesKHR GetDeviceGroupPeerMemoryFeaturesKHR;
    PFN_vkBindBufferMemory2 BindBufferMemory2;
    PFN_vkBindBufferMemory2KHR BindBufferMemory2KHR;
    PFN_vkBindImageMemory2 BindImageMemory2;
    PFN_vkBindImageMemory2KHR BindImageMemory2KHR;
    PFN_vkCmdSetDeviceMask CmdSetDeviceMask;
    PFN_vkCmdSetDeviceMaskKHR CmdSetDeviceMaskKHR;
    PFN_vkGetDeviceGroupPresentCapabilitiesKHR GetDeviceGroupPresentCapabilitiesKHR;
    PFN_vkGetDeviceGroupSurfacePresentModesKHR GetDeviceGroupSurfacePresentModesKHR;
    PFN_vkAcquireNextImage2KHR AcquireNextImage2KHR;
    PFN_vkCmdDispatchBase CmdDispatchBase;
    PFN_vkCmdDispatchBaseKHR CmdDispatchBaseKHR;
    PFN_vkCreateDescriptorUpdateTemplate CreateDescriptorUpdateTemplate;
    PFN_vkCreateDescriptorUpdateTemplateKHR CreateDescriptorUpdateTemplateKHR;
    PFN_vkDestroyDescriptorUpdateTemplate DestroyDescriptorUpdateTemplate;
    PFN_vkDestroyDescriptorUpdateTemplateKHR DestroyDescriptorUpdateTemplateKHR;
    PFN_vkUpdateDescriptorSetWithTemplate UpdateDescriptorSetWithTemplate;
    PFN_vkUpdateDescriptorSetWithTemplateKHR UpdateDescriptorSetWithTemplateKHR;
    PFN_vkCmdPushDescriptorSetWithTemplateKHR CmdPushDescriptorSetWithTemplateKHR;
    PFN_vkSetHdrMetadataEXT SetHdrMetadataEXT;
    PFN_vkGetSwapchainStatusKHR GetSwapchainStatusKHR;
    PFN_vkGetRefreshCycleDurationGOOGLE GetRefreshCycleDurationGOOGLE;
    PFN_vkGetPastPresentationTimingGOOGLE GetPastPresentationTimingGOOGLE;
    PFN_vkCmdSetViewportWScalingNV CmdSetViewportWScalingNV;
    PFN_vkCmdSetDiscardRectangleEXT CmdSetDiscardRectangleEXT;
    PFN_vkCmdSetDiscardRectangleEnableEXT CmdSetDiscardRectangleEnableEXT;
    PFN_vkCmdSetDiscardRectangleModeEXT CmdSetDiscardRectangleModeEXT;
    PFN_vkCmdSetSampleLocationsEXT CmdSetSampleLocationsEXT;
    PFN_vkGetBufferMemoryRequirements2 GetBufferMemoryRequirements2;
    PFN_vkGetBufferMemoryRequirements2KHR GetBufferMemoryRequirements2KHR;
    PFN_vkGetImageMemoryRequirements2 GetImageMemoryRequirements2;
    PFN_vkGetImageMemoryRequirements2KHR GetImageMemoryRequirements2KHR;
    PFN_vkGetImageSparseMemoryRequirements2 GetImageSparseMemoryRequirements2;
    PFN_vkGetImageSparseMemoryRequirements2KHR GetImageSparseMemoryRequirements2KHR;
    PFN_vkGetDeviceBufferMemoryRequirements GetDeviceBufferMemoryRequirements;
    PFN_vkGetDeviceBufferMemoryRequirementsKHR GetDeviceBufferMemoryRequirementsKHR;
    PFN_vkGetDeviceImageMemoryRequirements GetDeviceImageMemoryRequirements;
    PFN_vkGetDeviceImageMemoryRequirementsKHR GetDeviceImageMemoryRequirementsKHR;
    PFN_vkGetDeviceImageSparseMemoryRequirements GetDeviceImageSparseMemoryRequirements;
    PFN_vkGetDeviceImageSparseMemoryRequirementsKHR GetDeviceImageSparseMemoryRequirementsKHR;
    PFN_vkCreateSamplerYcbcrConversion CreateSamplerYcbcrConversion;
    PFN_vkCreateSamplerYcbcrConversionKHR CreateSamplerYcbcrConversionKHR;
    PFN_vkDestroySamplerYcbcrConversion DestroySamplerYcbcrConversion;
    PFN_vkDestroySamplerYcbcrConversionKHR DestroySamplerYcbcrConversionKHR;
    PFN_vkGetDeviceQueue2 GetDeviceQueue2;
    PFN_vkCreateValidationCacheEXT CreateValidationCacheEXT;
    PFN_vkDestroyValidationCacheEXT DestroyValidationCacheEXT;
    PFN_vkGetValidationCacheDataEXT GetValidationCacheDataEXT;
    PFN_vkMergeValidationCachesEXT MergeValidationCachesEXT;
    PFN_vkGetDescriptorSetLayoutSupport GetDescriptorSetLayoutSupport;
    PFN_vkGetDescriptorSetLayoutSupportKHR GetDescriptorSetLayoutSupportKHR;
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    PFN_vkGetSwapchainGrallocUsageANDROID GetSwapchainGrallocUsageANDROID;
#else
    PFN_vkVoidFunction GetSwapchainGrallocUsageANDROID;
# endif
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    PFN_vkGetSwapchainGrallocUsage2ANDROID GetSwapchainGrallocUsage2ANDROID;
#else
    PFN_vkVoidFunction GetSwapchainGrallocUsage2ANDROID;
# endif
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    PFN_vkAcquireImageANDROID AcquireImageANDROID;
#else
    PFN_vkVoidFunction AcquireImageANDROID;
# endif
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    PFN_vkQueueSignalReleaseImageANDROID QueueSignalReleaseImageANDROID;
#else
    PFN_vkVoidFunction QueueSignalReleaseImageANDROID;
# endif
    PFN_vkGetShaderInfoAMD GetShaderInfoAMD;
    PFN_vkSetLocalDimmingAMD SetLocalDimmingAMD;
    PFN_vkGetCalibratedTimestampsEXT GetCalibratedTimestampsEXT;
    PFN_vkSetDebugUtilsObjectNameEXT SetDebugUtilsObjectNameEXT;
    PFN_vkSetDebugUtilsObjectTagEXT SetDebugUtilsObjectTagEXT;
    PFN_vkQueueBeginDebugUtilsLabelEXT QueueBeginDebugUtilsLabelEXT;
    PFN_vkQueueEndDebugUtilsLabelEXT QueueEndDebugUtilsLabelEXT;
    PFN_vkQueueInsertDebugUtilsLabelEXT QueueInsertDebugUtilsLabelEXT;
    PFN_vkCmdBeginDebugUtilsLabelEXT CmdBeginDebugUtilsLabelEXT;
    PFN_vkCmdEndDebugUtilsLabelEXT CmdEndDebugUtilsLabelEXT;
    PFN_vkCmdInsertDebugUtilsLabelEXT CmdInsertDebugUtilsLabelEXT;
    PFN_vkGetMemoryHostPointerPropertiesEXT GetMemoryHostPointerPropertiesEXT;
    PFN_vkCmdWriteBufferMarkerAMD CmdWriteBufferMarkerAMD;
    PFN_vkCreateRenderPass2 CreateRenderPass2;
    PFN_vkCreateRenderPass2KHR CreateRenderPass2KHR;
    PFN_vkCmdBeginRenderPass2 CmdBeginRenderPass2;
    PFN_vkCmdBeginRenderPass2KHR CmdBeginRenderPass2KHR;
    PFN_vkCmdNextSubpass2 CmdNextSubpass2;
    PFN_vkCmdNextSubpass2KHR CmdNextSubpass2KHR;
    PFN_vkCmdEndRenderPass2 CmdEndRenderPass2;
    PFN_vkCmdEndRenderPass2KHR CmdEndRenderPass2KHR;
    PFN_vkGetSemaphoreCounterValue GetSemaphoreCounterValue;
    PFN_vkGetSemaphoreCounterValueKHR GetSemaphoreCounterValueKHR;
    PFN_vkWaitSemaphores WaitSemaphores;
    PFN_vkWaitSemaphoresKHR WaitSemaphoresKHR;
    PFN_vkSignalSemaphore SignalSemaphore;
    PFN_vkSignalSemaphoreKHR SignalSemaphoreKHR;
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    PFN_vkGetAndroidHardwareBufferPropertiesANDROID GetAndroidHardwareBufferPropertiesANDROID;
#else
    PFN_vkVoidFunction GetAndroidHardwareBufferPropertiesANDROID;
# endif
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    PFN_vkGetMemoryAndroidHardwareBufferANDROID GetMemoryAndroidHardwareBufferANDROID;
#else
    PFN_vkVoidFunction GetMemoryAndroidHardwareBufferANDROID;
# endif
    PFN_vkCmdDrawIndirectCount CmdDrawIndirectCount;
    PFN_vkCmdDrawIndirectCountKHR CmdDrawIndirectCountKHR;
    PFN_vkCmdDrawIndirectCountAMD CmdDrawIndirectCountAMD;
    PFN_vkCmdDrawIndexedIndirectCount CmdDrawIndexedIndirectCount;
    PFN_vkCmdDrawIndexedIndirectCountKHR CmdDrawIndexedIndirectCountKHR;
    PFN_vkCmdDrawIndexedIndirectCountAMD CmdDrawIndexedIndirectCountAMD;
    PFN_vkCmdSetCheckpointNV CmdSetCheckpointNV;
    PFN_vkGetQueueCheckpointDataNV GetQueueCheckpointDataNV;
    PFN_vkCmdBindTransformFeedbackBuffersEXT CmdBindTransformFeedbackBuffersEXT;
    PFN_vkCmdBeginTransformFeedbackEXT CmdBeginTransformFeedbackEXT;
    PFN_vkCmdEndTransformFeedbackEXT CmdEndTransformFeedbackEXT;
    PFN_vkCmdBeginQueryIndexedEXT CmdBeginQueryIndexedEXT;
    PFN_vkCmdEndQueryIndexedEXT CmdEndQueryIndexedEXT;
    PFN_vkCmdDrawIndirectByteCountEXT CmdDrawIndirectByteCountEXT;
    PFN_vkCmdSetExclusiveScissorNV CmdSetExclusiveScissorNV;
    PFN_vkCmdSetExclusiveScissorEnableNV CmdSetExclusiveScissorEnableNV;
    PFN_vkCmdBindShadingRateImageNV CmdBindShadingRateImageNV;
    PFN_vkCmdSetViewportShadingRatePaletteNV CmdSetViewportShadingRatePaletteNV;
    PFN_vkCmdSetCoarseSampleOrderNV CmdSetCoarseSampleOrderNV;
    PFN_vkCmdDrawMeshTasksNV CmdDrawMeshTasksNV;
    PFN_vkCmdDrawMeshTasksIndirectNV CmdDrawMeshTasksIndirectNV;
    PFN_vkCmdDrawMeshTasksIndirectCountNV CmdDrawMeshTasksIndirectCountNV;
    PFN_vkCmdDrawMeshTasksEXT CmdDrawMeshTasksEXT;
    PFN_vkCmdDrawMeshTasksIndirectEXT CmdDrawMeshTasksIndirectEXT;
    PFN_vkCmdDrawMeshTasksIndirectCountEXT CmdDrawMeshTasksIndirectCountEXT;
    PFN_vkCompileDeferredNV CompileDeferredNV;
    PFN_vkCreateAccelerationStructureNV CreateAccelerationStructureNV;
    PFN_vkCmdBindInvocationMaskHUAWEI CmdBindInvocationMaskHUAWEI;
    PFN_vkDestroyAccelerationStructureKHR DestroyAccelerationStructureKHR;
    PFN_vkDestroyAccelerationStructureNV DestroyAccelerationStructureNV;
    PFN_vkGetAccelerationStructureMemoryRequirementsNV GetAccelerationStructureMemoryRequirementsNV;
    PFN_vkBindAccelerationStructureMemoryNV BindAccelerationStructureMemoryNV;
    PFN_vkCmdCopyAccelerationStructureNV CmdCopyAccelerationStructureNV;
    PFN_vkCmdCopyAccelerationStructureKHR CmdCopyAccelerationStructureKHR;
    PFN_vkCopyAccelerationStructureKHR CopyAccelerationStructureKHR;
    PFN_vkCmdCopyAccelerationStructureToMemoryKHR CmdCopyAccelerationStructureToMemoryKHR;
    PFN_vkCopyAccelerationStructureToMemoryKHR CopyAccelerationStructureToMemoryKHR;
    PFN_vkCmdCopyMemoryToAccelerationStructureKHR CmdCopyMemoryToAccelerationStructureKHR;
    PFN_vkCopyMemoryToAccelerationStructureKHR CopyMemoryToAccelerationStructureKHR;
    PFN_vkCmdWriteAccelerationStructuresPropertiesKHR CmdWriteAccelerationStructuresPropertiesKHR;
    PFN_vkCmdWriteAccelerationStructuresPropertiesNV CmdWriteAccelerationStructuresPropertiesNV;
    PFN_vkCmdBuildAccelerationStructureNV CmdBuildAccelerationStructureNV;
    PFN_vkWriteAccelerationStructuresPropertiesKHR WriteAccelerationStructuresPropertiesKHR;
    PFN_vkCmdTraceRaysKHR CmdTraceRaysKHR;
    PFN_vkCmdTraceRaysNV CmdTraceRaysNV;
    PFN_vkGetRayTracingShaderGroupHandlesKHR GetRayTracingShaderGroupHandlesKHR;
    PFN_vkGetRayTracingShaderGroupHandlesNV GetRayTracingShaderGroupHandlesNV;
    PFN_vkGetRayTracingCaptureReplayShaderGroupHandlesKHR GetRayTracingCaptureReplayShaderGroupHandlesKHR;
    PFN_vkGetAccelerationStructureHandleNV GetAccelerationStructureHandleNV;
    PFN_vkCreateRayTracingPipelinesNV CreateRayTracingPipelinesNV;
    PFN_vkCreateRayTracingPipelinesKHR CreateRayTracingPipelinesKHR;
    PFN_vkCmdTraceRaysIndirectKHR CmdTraceRaysIndirectKHR;
    PFN_vkCmdTraceRaysIndirect2KHR CmdTraceRaysIndirect2KHR;
    PFN_vkGetDeviceAccelerationStructureCompatibilityKHR GetDeviceAccelerationStructureCompatibilityKHR;
    PFN_vkGetRayTracingShaderGroupStackSizeKHR GetRayTracingShaderGroupStackSizeKHR;
    PFN_vkCmdSetRayTracingPipelineStackSizeKHR CmdSetRayTracingPipelineStackSizeKHR;
    PFN_vkGetImageViewHandleNVX GetImageViewHandleNVX;
    PFN_vkGetImageViewAddressNVX GetImageViewAddressNVX;
#ifdef VK_USE_PLATFORM_WIN32_KHR
    PFN_vkGetDeviceGroupSurfacePresentModes2EXT GetDeviceGroupSurfacePresentModes2EXT;
#else
    PFN_vkVoidFunction GetDeviceGroupSurfacePresentModes2EXT;
# endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
    PFN_vkAcquireFullScreenExclusiveModeEXT AcquireFullScreenExclusiveModeEXT;
#else
    PFN_vkVoidFunction AcquireFullScreenExclusiveModeEXT;
# endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
    PFN_vkReleaseFullScreenExclusiveModeEXT ReleaseFullScreenExclusiveModeEXT;
#else
    PFN_vkVoidFunction ReleaseFullScreenExclusiveModeEXT;
# endif
    PFN_vkAcquireProfilingLockKHR AcquireProfilingLockKHR;
    PFN_vkReleaseProfilingLockKHR ReleaseProfilingLockKHR;
    PFN_vkGetImageDrmFormatModifierPropertiesEXT GetImageDrmFormatModifierPropertiesEXT;
    PFN_vkGetBufferOpaqueCaptureAddress GetBufferOpaqueCaptureAddress;
    PFN_vkGetBufferOpaqueCaptureAddressKHR GetBufferOpaqueCaptureAddressKHR;
    PFN_vkGetBufferDeviceAddress GetBufferDeviceAddress;
    PFN_vkGetBufferDeviceAddressKHR GetBufferDeviceAddressKHR;
    PFN_vkGetBufferDeviceAddressEXT GetBufferDeviceAddressEXT;
    PFN_vkInitializePerformanceApiINTEL InitializePerformanceApiINTEL;
    PFN_vkUninitializePerformanceApiINTEL UninitializePerformanceApiINTEL;
    PFN_vkCmdSetPerformanceMarkerINTEL CmdSetPerformanceMarkerINTEL;
    PFN_vkCmdSetPerformanceStreamMarkerINTEL CmdSetPerformanceStreamMarkerINTEL;
    PFN_vkCmdSetPerformanceOverrideINTEL CmdSetPerformanceOverrideINTEL;
    PFN_vkAcquirePerformanceConfigurationINTEL AcquirePerformanceConfigurationINTEL;
    PFN_vkReleasePerformanceConfigurationINTEL ReleasePerformanceConfigurationINTEL;
    PFN_vkQueueSetPerformanceConfigurationINTEL QueueSetPerformanceConfigurationINTEL;
    PFN_vkGetPerformanceParameterINTEL GetPerformanceParameterINTEL;
    PFN_vkGetDeviceMemoryOpaqueCaptureAddress GetDeviceMemoryOpaqueCaptureAddress;
    PFN_vkGetDeviceMemoryOpaqueCaptureAddressKHR GetDeviceMemoryOpaqueCaptureAddressKHR;
    PFN_vkGetPipelineExecutablePropertiesKHR GetPipelineExecutablePropertiesKHR;
    PFN_vkGetPipelineExecutableStatisticsKHR GetPipelineExecutableStatisticsKHR;
    PFN_vkGetPipelineExecutableInternalRepresentationsKHR GetPipelineExecutableInternalRepresentationsKHR;
    PFN_vkCmdSetLineStippleEXT CmdSetLineStippleEXT;
    PFN_vkCreateAccelerationStructureKHR CreateAccelerationStructureKHR;
    PFN_vkCmdBuildAccelerationStructuresKHR CmdBuildAccelerationStructuresKHR;
    PFN_vkCmdBuildAccelerationStructuresIndirectKHR CmdBuildAccelerationStructuresIndirectKHR;
    PFN_vkBuildAccelerationStructuresKHR BuildAccelerationStructuresKHR;
    PFN_vkGetAccelerationStructureDeviceAddressKHR GetAccelerationStructureDeviceAddressKHR;
    PFN_vkCreateDeferredOperationKHR CreateDeferredOperationKHR;
    PFN_vkDestroyDeferredOperationKHR DestroyDeferredOperationKHR;
    PFN_vkGetDeferredOperationMaxConcurrencyKHR GetDeferredOperationMaxConcurrencyKHR;
    PFN_vkGetDeferredOperationResultKHR GetDeferredOperationResultKHR;
    PFN_vkDeferredOperationJoinKHR DeferredOperationJoinKHR;
    PFN_vkCmdSetCullMode CmdSetCullMode;
    PFN_vkCmdSetCullModeEXT CmdSetCullModeEXT;
    PFN_vkCmdSetFrontFace CmdSetFrontFace;
    PFN_vkCmdSetFrontFaceEXT CmdSetFrontFaceEXT;
    PFN_vkCmdSetPrimitiveTopology CmdSetPrimitiveTopology;
    PFN_vkCmdSetPrimitiveTopologyEXT CmdSetPrimitiveTopologyEXT;
    PFN_vkCmdSetViewportWithCount CmdSetViewportWithCount;
    PFN_vkCmdSetViewportWithCountEXT CmdSetViewportWithCountEXT;
    PFN_vkCmdSetScissorWithCount CmdSetScissorWithCount;
    PFN_vkCmdSetScissorWithCountEXT CmdSetScissorWithCountEXT;
    PFN_vkCmdBindVertexBuffers2 CmdBindVertexBuffers2;
    PFN_vkCmdBindVertexBuffers2EXT CmdBindVertexBuffers2EXT;
    PFN_vkCmdSetDepthTestEnable CmdSetDepthTestEnable;
    PFN_vkCmdSetDepthTestEnableEXT CmdSetDepthTestEnableEXT;
    PFN_vkCmdSetDepthWriteEnable CmdSetDepthWriteEnable;
    PFN_vkCmdSetDepthWriteEnableEXT CmdSetDepthWriteEnableEXT;
    PFN_vkCmdSetDepthCompareOp CmdSetDepthCompareOp;
    PFN_vkCmdSetDepthCompareOpEXT CmdSetDepthCompareOpEXT;
    PFN_vkCmdSetDepthBoundsTestEnable CmdSetDepthBoundsTestEnable;
    PFN_vkCmdSetDepthBoundsTestEnableEXT CmdSetDepthBoundsTestEnableEXT;
    PFN_vkCmdSetStencilTestEnable CmdSetStencilTestEnable;
    PFN_vkCmdSetStencilTestEnableEXT CmdSetStencilTestEnableEXT;
    PFN_vkCmdSetStencilOp CmdSetStencilOp;
    PFN_vkCmdSetStencilOpEXT CmdSetStencilOpEXT;
    PFN_vkCmdSetPatchControlPointsEXT CmdSetPatchControlPointsEXT;
    PFN_vkCmdSetRasterizerDiscardEnable CmdSetRasterizerDiscardEnable;
    PFN_vkCmdSetRasterizerDiscardEnableEXT CmdSetRasterizerDiscardEnableEXT;
    PFN_vkCmdSetDepthBiasEnable CmdSetDepthBiasEnable;
    PFN_vkCmdSetDepthBiasEnableEXT CmdSetDepthBiasEnableEXT;
    PFN_vkCmdSetLogicOpEXT CmdSetLogicOpEXT;
    PFN_vkCmdSetPrimitiveRestartEnable CmdSetPrimitiveRestartEnable;
    PFN_vkCmdSetPrimitiveRestartEnableEXT CmdSetPrimitiveRestartEnableEXT;
    PFN_vkCmdSetTessellationDomainOriginEXT CmdSetTessellationDomainOriginEXT;
    PFN_vkCmdSetDepthClampEnableEXT CmdSetDepthClampEnableEXT;
    PFN_vkCmdSetPolygonModeEXT CmdSetPolygonModeEXT;
    PFN_vkCmdSetRasterizationSamplesEXT CmdSetRasterizationSamplesEXT;
    PFN_vkCmdSetSampleMaskEXT CmdSetSampleMaskEXT;
    PFN_vkCmdSetAlphaToCoverageEnableEXT CmdSetAlphaToCoverageEnableEXT;
    PFN_vkCmdSetAlphaToOneEnableEXT CmdSetAlphaToOneEnableEXT;
    PFN_vkCmdSetLogicOpEnableEXT CmdSetLogicOpEnableEXT;
    PFN_vkCmdSetColorBlendEnableEXT CmdSetColorBlendEnableEXT;
    PFN_vkCmdSetColorBlendEquationEXT CmdSetColorBlendEquationEXT;
    PFN_vkCmdSetColorWriteMaskEXT CmdSetColorWriteMaskEXT;
    PFN_vkCmdSetRasterizationStreamEXT CmdSetRasterizationStreamEXT;
    PFN_vkCmdSetConservativeRasterizationModeEXT CmdSetConservativeRasterizationModeEXT;
    PFN_vkCmdSetExtraPrimitiveOverestimationSizeEXT CmdSetExtraPrimitiveOverestimationSizeEXT;
    PFN_vkCmdSetDepthClipEnableEXT CmdSetDepthClipEnableEXT;
    PFN_vkCmdSetSampleLocationsEnableEXT CmdSetSampleLocationsEnableEXT;
    PFN_vkCmdSetColorBlendAdvancedEXT CmdSetColorBlendAdvancedEXT;
    PFN_vkCmdSetProvokingVertexModeEXT CmdSetProvokingVertexModeEXT;
    PFN_vkCmdSetLineRasterizationModeEXT CmdSetLineRasterizationModeEXT;
    PFN_vkCmdSetLineStippleEnableEXT CmdSetLineStippleEnableEXT;
    PFN_vkCmdSetDepthClipNegativeOneToOneEXT CmdSetDepthClipNegativeOneToOneEXT;
    PFN_vkCmdSetViewportWScalingEnableNV CmdSetViewportWScalingEnableNV;
    PFN_vkCmdSetViewportSwizzleNV CmdSetViewportSwizzleNV;
    PFN_vkCmdSetCoverageToColorEnableNV CmdSetCoverageToColorEnableNV;
    PFN_vkCmdSetCoverageToColorLocationNV CmdSetCoverageToColorLocationNV;
    PFN_vkCmdSetCoverageModulationModeNV CmdSetCoverageModulationModeNV;
    PFN_vkCmdSetCoverageModulationTableEnableNV CmdSetCoverageModulationTableEnableNV;
    PFN_vkCmdSetCoverageModulationTableNV CmdSetCoverageModulationTableNV;
    PFN_vkCmdSetShadingRateImageEnableNV CmdSetShadingRateImageEnableNV;
    PFN_vkCmdSetCoverageReductionModeNV CmdSetCoverageReductionModeNV;
    PFN_vkCmdSetRepresentativeFragmentTestEnableNV CmdSetRepresentativeFragmentTestEnableNV;
    PFN_vkCreatePrivateDataSlot CreatePrivateDataSlot;
    PFN_vkCreatePrivateDataSlotEXT CreatePrivateDataSlotEXT;
    PFN_vkDestroyPrivateDataSlot DestroyPrivateDataSlot;
    PFN_vkDestroyPrivateDataSlotEXT DestroyPrivateDataSlotEXT;
    PFN_vkSetPrivateData SetPrivateData;
    PFN_vkSetPrivateDataEXT SetPrivateDataEXT;
    PFN_vkGetPrivateData GetPrivateData;
    PFN_vkGetPrivateDataEXT GetPrivateDataEXT;
    PFN_vkCmdCopyBuffer2 CmdCopyBuffer2;
    PFN_vkCmdCopyBuffer2KHR CmdCopyBuffer2KHR;
    PFN_vkCmdCopyImage2 CmdCopyImage2;
    PFN_vkCmdCopyImage2KHR CmdCopyImage2KHR;
    PFN_vkCmdBlitImage2 CmdBlitImage2;
    PFN_vkCmdBlitImage2KHR CmdBlitImage2KHR;
    PFN_vkCmdCopyBufferToImage2 CmdCopyBufferToImage2;
    PFN_vkCmdCopyBufferToImage2KHR CmdCopyBufferToImage2KHR;
    PFN_vkCmdCopyImageToBuffer2 CmdCopyImageToBuffer2;
    PFN_vkCmdCopyImageToBuffer2KHR CmdCopyImageToBuffer2KHR;
    PFN_vkCmdResolveImage2 CmdResolveImage2;
    PFN_vkCmdResolveImage2KHR CmdResolveImage2KHR;
    PFN_vkCmdSetFragmentShadingRateKHR CmdSetFragmentShadingRateKHR;
    PFN_vkCmdSetFragmentShadingRateEnumNV CmdSetFragmentShadingRateEnumNV;
    PFN_vkGetAccelerationStructureBuildSizesKHR GetAccelerationStructureBuildSizesKHR;
    PFN_vkCmdSetVertexInputEXT CmdSetVertexInputEXT;
    PFN_vkCmdSetColorWriteEnableEXT CmdSetColorWriteEnableEXT;
    PFN_vkCmdSetEvent2 CmdSetEvent2;
    PFN_vkCmdSetEvent2KHR CmdSetEvent2KHR;
    PFN_vkCmdResetEvent2 CmdResetEvent2;
    PFN_vkCmdResetEvent2KHR CmdResetEvent2KHR;
    PFN_vkCmdWaitEvents2 CmdWaitEvents2;
    PFN_vkCmdWaitEvents2KHR CmdWaitEvents2KHR;
    PFN_vkCmdPipelineBarrier2 CmdPipelineBarrier2;
    PFN_vkCmdPipelineBarrier2KHR CmdPipelineBarrier2KHR;
    PFN_vkQueueSubmit2 QueueSubmit2;
    PFN_vkQueueSubmit2KHR QueueSubmit2KHR;
    PFN_vkCmdWriteTimestamp2 CmdWriteTimestamp2;
    PFN_vkCmdWriteTimestamp2KHR CmdWriteTimestamp2KHR;
    PFN_vkCmdWriteBufferMarker2AMD CmdWriteBufferMarker2AMD;
    PFN_vkGetQueueCheckpointData2NV GetQueueCheckpointData2NV;
    PFN_vkCreateVideoSessionKHR CreateVideoSessionKHR;
    PFN_vkDestroyVideoSessionKHR DestroyVideoSessionKHR;
    PFN_vkCreateVideoSessionParametersKHR CreateVideoSessionParametersKHR;
    PFN_vkUpdateVideoSessionParametersKHR UpdateVideoSessionParametersKHR;
    PFN_vkDestroyVideoSessionParametersKHR DestroyVideoSessionParametersKHR;
    PFN_vkGetVideoSessionMemoryRequirementsKHR GetVideoSessionMemoryRequirementsKHR;
    PFN_vkBindVideoSessionMemoryKHR BindVideoSessionMemoryKHR;
    PFN_vkCmdDecodeVideoKHR CmdDecodeVideoKHR;
    PFN_vkCmdBeginVideoCodingKHR CmdBeginVideoCodingKHR;
    PFN_vkCmdControlVideoCodingKHR CmdControlVideoCodingKHR;
    PFN_vkCmdEndVideoCodingKHR CmdEndVideoCodingKHR;
    PFN_vkCmdDecompressMemoryNV CmdDecompressMemoryNV;
    PFN_vkCmdDecompressMemoryIndirectCountNV CmdDecompressMemoryIndirectCountNV;
    PFN_vkCreateCuModuleNVX CreateCuModuleNVX;
    PFN_vkCreateCuFunctionNVX CreateCuFunctionNVX;
    PFN_vkDestroyCuModuleNVX DestroyCuModuleNVX;
    PFN_vkDestroyCuFunctionNVX DestroyCuFunctionNVX;
    PFN_vkCmdCuLaunchKernelNVX CmdCuLaunchKernelNVX;
    PFN_vkGetDescriptorSetLayoutSizeEXT GetDescriptorSetLayoutSizeEXT;
    PFN_vkGetDescriptorSetLayoutBindingOffsetEXT GetDescriptorSetLayoutBindingOffsetEXT;
    PFN_vkGetDescriptorEXT GetDescriptorEXT;
    PFN_vkCmdBindDescriptorBuffersEXT CmdBindDescriptorBuffersEXT;
    PFN_vkCmdSetDescriptorBufferOffsetsEXT CmdSetDescriptorBufferOffsetsEXT;
    PFN_vkCmdBindDescriptorBufferEmbeddedSamplersEXT CmdBindDescriptorBufferEmbeddedSamplersEXT;
    PFN_vkGetBufferOpaqueCaptureDescriptorDataEXT GetBufferOpaqueCaptureDescriptorDataEXT;
    PFN_vkGetImageOpaqueCaptureDescriptorDataEXT GetImageOpaqueCaptureDescriptorDataEXT;
    PFN_vkGetImageViewOpaqueCaptureDescriptorDataEXT GetImageViewOpaqueCaptureDescriptorDataEXT;
    PFN_vkGetSamplerOpaqueCaptureDescriptorDataEXT GetSamplerOpaqueCaptureDescriptorDataEXT;
    PFN_vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT GetAccelerationStructureOpaqueCaptureDescriptorDataEXT;
    PFN_vkSetDeviceMemoryPriorityEXT SetDeviceMemoryPriorityEXT;
    PFN_vkWaitForPresentKHR WaitForPresentKHR;
#ifdef VK_USE_PLATFORM_FUCHSIA
    PFN_vkCreateBufferCollectionFUCHSIA CreateBufferCollectionFUCHSIA;
#else
    PFN_vkVoidFunction CreateBufferCollectionFUCHSIA;
# endif
#ifdef VK_USE_PLATFORM_FUCHSIA
    PFN_vkSetBufferCollectionBufferConstraintsFUCHSIA SetBufferCollectionBufferConstraintsFUCHSIA;
#else
    PFN_vkVoidFunction SetBufferCollectionBufferConstraintsFUCHSIA;
# endif
#ifdef VK_USE_PLATFORM_FUCHSIA
    PFN_vkSetBufferCollectionImageConstraintsFUCHSIA SetBufferCollectionImageConstraintsFUCHSIA;
#else
    PFN_vkVoidFunction SetBufferCollectionImageConstraintsFUCHSIA;
# endif
#ifdef VK_USE_PLATFORM_FUCHSIA
    PFN_vkDestroyBufferCollectionFUCHSIA DestroyBufferCollectionFUCHSIA;
#else
    PFN_vkVoidFunction DestroyBufferCollectionFUCHSIA;
# endif
#ifdef VK_USE_PLATFORM_FUCHSIA
    PFN_vkGetBufferCollectionPropertiesFUCHSIA GetBufferCollectionPropertiesFUCHSIA;
#else
    PFN_vkVoidFunction GetBufferCollectionPropertiesFUCHSIA;
# endif
    PFN_vkCmdBeginRendering CmdBeginRendering;
    PFN_vkCmdBeginRenderingKHR CmdBeginRenderingKHR;
    PFN_vkCmdEndRendering CmdEndRendering;
    PFN_vkCmdEndRenderingKHR CmdEndRenderingKHR;
    PFN_vkGetDescriptorSetLayoutHostMappingInfoVALVE GetDescriptorSetLayoutHostMappingInfoVALVE;
    PFN_vkGetDescriptorSetHostMappingVALVE GetDescriptorSetHostMappingVALVE;
    PFN_vkCreateMicromapEXT CreateMicromapEXT;
    PFN_vkCmdBuildMicromapsEXT CmdBuildMicromapsEXT;
    PFN_vkBuildMicromapsEXT BuildMicromapsEXT;
    PFN_vkDestroyMicromapEXT DestroyMicromapEXT;
    PFN_vkCmdCopyMicromapEXT CmdCopyMicromapEXT;
    PFN_vkCopyMicromapEXT CopyMicromapEXT;
    PFN_vkCmdCopyMicromapToMemoryEXT CmdCopyMicromapToMemoryEXT;
    PFN_vkCopyMicromapToMemoryEXT CopyMicromapToMemoryEXT;
    PFN_vkCmdCopyMemoryToMicromapEXT CmdCopyMemoryToMicromapEXT;
    PFN_vkCopyMemoryToMicromapEXT CopyMemoryToMicromapEXT;
    PFN_vkCmdWriteMicromapsPropertiesEXT CmdWriteMicromapsPropertiesEXT;
    PFN_vkWriteMicromapsPropertiesEXT WriteMicromapsPropertiesEXT;
    PFN_vkGetDeviceMicromapCompatibilityEXT GetDeviceMicromapCompatibilityEXT;
    PFN_vkGetMicromapBuildSizesEXT GetMicromapBuildSizesEXT;
    PFN_vkGetShaderModuleIdentifierEXT GetShaderModuleIdentifierEXT;
    PFN_vkGetShaderModuleCreateInfoIdentifierEXT GetShaderModuleCreateInfoIdentifierEXT;
    PFN_vkGetImageSubresourceLayout2EXT GetImageSubresourceLayout2EXT;
    PFN_vkGetPipelinePropertiesEXT GetPipelinePropertiesEXT;
#ifdef VK_USE_PLATFORM_METAL_EXT
    PFN_vkExportMetalObjectsEXT ExportMetalObjectsEXT;
#else
    PFN_vkVoidFunction ExportMetalObjectsEXT;
# endif
    PFN_vkGetFramebufferTilePropertiesQCOM GetFramebufferTilePropertiesQCOM;
    PFN_vkGetDynamicRenderingTilePropertiesQCOM GetDynamicRenderingTilePropertiesQCOM;
    PFN_vkCreateOpticalFlowSessionNV CreateOpticalFlowSessionNV;
    PFN_vkDestroyOpticalFlowSessionNV DestroyOpticalFlowSessionNV;
    PFN_vkBindOpticalFlowSessionImageNV BindOpticalFlowSessionImageNV;
    PFN_vkCmdOpticalFlowExecuteNV CmdOpticalFlowExecuteNV;
    PFN_vkGetDeviceFaultInfoEXT GetDeviceFaultInfoEXT;
    PFN_vkReleaseSwapchainImagesEXT ReleaseSwapchainImagesEXT;
    PFN_vkMapMemory2KHR MapMemory2KHR;
    PFN_vkUnmapMemory2KHR UnmapMemory2KHR;
    PFN_vkCreateShadersEXT CreateShadersEXT;
    PFN_vkDestroyShaderEXT DestroyShaderEXT;
    PFN_vkGetShaderBinaryDataEXT GetShaderBinaryDataEXT;
    PFN_vkCmdBindShadersEXT CmdBindShadersEXT;
};


void
vk_instance_dispatch_table_load(struct vk_instance_dispatch_table *table,
                                PFN_vkGetInstanceProcAddr gpa,
                                VkInstance instance);
void
vk_physical_device_dispatch_table_load(struct vk_physical_device_dispatch_table *table,
                                       PFN_vkGetInstanceProcAddr gpa,
                                       VkInstance instance);
void
vk_device_dispatch_table_load(struct vk_device_dispatch_table *table,
                              PFN_vkGetDeviceProcAddr gpa,
                              VkDevice device);

void vk_instance_dispatch_table_from_entrypoints(
    struct vk_instance_dispatch_table *dispatch_table,
    const struct vk_instance_entrypoint_table *entrypoint_table,
    bool overwrite);

void vk_physical_device_dispatch_table_from_entrypoints(
    struct vk_physical_device_dispatch_table *dispatch_table,
    const struct vk_physical_device_entrypoint_table *entrypoint_table,
    bool overwrite);

void vk_device_dispatch_table_from_entrypoints(
    struct vk_device_dispatch_table *dispatch_table,
    const struct vk_device_entrypoint_table *entrypoint_table,
    bool overwrite);

PFN_vkVoidFunction
vk_instance_dispatch_table_get(const struct vk_instance_dispatch_table *table,
                               const char *name);

PFN_vkVoidFunction
vk_physical_device_dispatch_table_get(const struct vk_physical_device_dispatch_table *table,
                                      const char *name);

PFN_vkVoidFunction
vk_device_dispatch_table_get(const struct vk_device_dispatch_table *table,
                             const char *name);

PFN_vkVoidFunction
vk_instance_dispatch_table_get_if_supported(
    const struct vk_instance_dispatch_table *table,
    const char *name,
    uint32_t core_version,
    const struct vk_instance_extension_table *instance_exts);

PFN_vkVoidFunction
vk_physical_device_dispatch_table_get_if_supported(
    const struct vk_physical_device_dispatch_table *table,
    const char *name,
    uint32_t core_version,
    const struct vk_instance_extension_table *instance_exts);

PFN_vkVoidFunction
vk_device_dispatch_table_get_if_supported(
    const struct vk_device_dispatch_table *table,
    const char *name,
    uint32_t core_version,
    const struct vk_instance_extension_table *instance_exts,
    const struct vk_device_extension_table *device_exts);

#ifdef __cplusplus
}
#endif

#endif /* VK_DISPATCH_TABLE_H */
