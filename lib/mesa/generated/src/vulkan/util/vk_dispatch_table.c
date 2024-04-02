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

#include "vk_dispatch_table.h"

#include "util/macros.h"
#include "string.h"




void
vk_instance_dispatch_table_load(struct vk_instance_dispatch_table *table,
                               PFN_vkGetInstanceProcAddr gpa,
                               VkInstance obj)
{
    table->GetInstanceProcAddr = gpa;
    table->CreateInstance = (PFN_vkCreateInstance) gpa(obj, "vkCreateInstance");
    table->DestroyInstance = (PFN_vkDestroyInstance) gpa(obj, "vkDestroyInstance");
    table->EnumeratePhysicalDevices = (PFN_vkEnumeratePhysicalDevices) gpa(obj, "vkEnumeratePhysicalDevices");
    table->GetInstanceProcAddr = (PFN_vkGetInstanceProcAddr) gpa(obj, "vkGetInstanceProcAddr");
    table->EnumerateInstanceVersion = (PFN_vkEnumerateInstanceVersion) gpa(obj, "vkEnumerateInstanceVersion");
    table->EnumerateInstanceLayerProperties = (PFN_vkEnumerateInstanceLayerProperties) gpa(obj, "vkEnumerateInstanceLayerProperties");
    table->EnumerateInstanceExtensionProperties = (PFN_vkEnumerateInstanceExtensionProperties) gpa(obj, "vkEnumerateInstanceExtensionProperties");
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    table->CreateAndroidSurfaceKHR = (PFN_vkCreateAndroidSurfaceKHR) gpa(obj, "vkCreateAndroidSurfaceKHR");
#endif
    table->CreateDisplayPlaneSurfaceKHR = (PFN_vkCreateDisplayPlaneSurfaceKHR) gpa(obj, "vkCreateDisplayPlaneSurfaceKHR");
    table->DestroySurfaceKHR = (PFN_vkDestroySurfaceKHR) gpa(obj, "vkDestroySurfaceKHR");
#ifdef VK_USE_PLATFORM_VI_NN
    table->CreateViSurfaceNN = (PFN_vkCreateViSurfaceNN) gpa(obj, "vkCreateViSurfaceNN");
#endif
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
    table->CreateWaylandSurfaceKHR = (PFN_vkCreateWaylandSurfaceKHR) gpa(obj, "vkCreateWaylandSurfaceKHR");
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
    table->CreateWin32SurfaceKHR = (PFN_vkCreateWin32SurfaceKHR) gpa(obj, "vkCreateWin32SurfaceKHR");
#endif
#ifdef VK_USE_PLATFORM_XLIB_KHR
    table->CreateXlibSurfaceKHR = (PFN_vkCreateXlibSurfaceKHR) gpa(obj, "vkCreateXlibSurfaceKHR");
#endif
#ifdef VK_USE_PLATFORM_XCB_KHR
    table->CreateXcbSurfaceKHR = (PFN_vkCreateXcbSurfaceKHR) gpa(obj, "vkCreateXcbSurfaceKHR");
#endif
#ifdef VK_USE_PLATFORM_DIRECTFB_EXT
    table->CreateDirectFBSurfaceEXT = (PFN_vkCreateDirectFBSurfaceEXT) gpa(obj, "vkCreateDirectFBSurfaceEXT");
#endif
#ifdef VK_USE_PLATFORM_FUCHSIA
    table->CreateImagePipeSurfaceFUCHSIA = (PFN_vkCreateImagePipeSurfaceFUCHSIA) gpa(obj, "vkCreateImagePipeSurfaceFUCHSIA");
#endif
#ifdef VK_USE_PLATFORM_GGP
    table->CreateStreamDescriptorSurfaceGGP = (PFN_vkCreateStreamDescriptorSurfaceGGP) gpa(obj, "vkCreateStreamDescriptorSurfaceGGP");
#endif
#ifdef VK_USE_PLATFORM_SCREEN_QNX
    table->CreateScreenSurfaceQNX = (PFN_vkCreateScreenSurfaceQNX) gpa(obj, "vkCreateScreenSurfaceQNX");
#endif
    table->CreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT) gpa(obj, "vkCreateDebugReportCallbackEXT");
    table->DestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT) gpa(obj, "vkDestroyDebugReportCallbackEXT");
    table->DebugReportMessageEXT = (PFN_vkDebugReportMessageEXT) gpa(obj, "vkDebugReportMessageEXT");
    table->EnumeratePhysicalDeviceGroups = (PFN_vkEnumeratePhysicalDeviceGroups) gpa(obj, "vkEnumeratePhysicalDeviceGroups");
    if (table->EnumeratePhysicalDeviceGroups == NULL) {
        table->EnumeratePhysicalDeviceGroups = (PFN_vkEnumeratePhysicalDeviceGroups) gpa(obj, "vkEnumeratePhysicalDeviceGroupsKHR");
    }
    #ifdef VK_USE_PLATFORM_IOS_MVK
    table->CreateIOSSurfaceMVK = (PFN_vkCreateIOSSurfaceMVK) gpa(obj, "vkCreateIOSSurfaceMVK");
#endif
#ifdef VK_USE_PLATFORM_MACOS_MVK
    table->CreateMacOSSurfaceMVK = (PFN_vkCreateMacOSSurfaceMVK) gpa(obj, "vkCreateMacOSSurfaceMVK");
#endif
#ifdef VK_USE_PLATFORM_METAL_EXT
    table->CreateMetalSurfaceEXT = (PFN_vkCreateMetalSurfaceEXT) gpa(obj, "vkCreateMetalSurfaceEXT");
#endif
    table->CreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT) gpa(obj, "vkCreateDebugUtilsMessengerEXT");
    table->DestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT) gpa(obj, "vkDestroyDebugUtilsMessengerEXT");
    table->SubmitDebugUtilsMessageEXT = (PFN_vkSubmitDebugUtilsMessageEXT) gpa(obj, "vkSubmitDebugUtilsMessageEXT");
    table->CreateHeadlessSurfaceEXT = (PFN_vkCreateHeadlessSurfaceEXT) gpa(obj, "vkCreateHeadlessSurfaceEXT");
}



void
vk_physical_device_dispatch_table_load(struct vk_physical_device_dispatch_table *table,
                               PFN_vkGetInstanceProcAddr gpa,
                               VkInstance obj)
{
    table->GetPhysicalDeviceProperties = (PFN_vkGetPhysicalDeviceProperties) gpa(obj, "vkGetPhysicalDeviceProperties");
    table->GetPhysicalDeviceQueueFamilyProperties = (PFN_vkGetPhysicalDeviceQueueFamilyProperties) gpa(obj, "vkGetPhysicalDeviceQueueFamilyProperties");
    table->GetPhysicalDeviceMemoryProperties = (PFN_vkGetPhysicalDeviceMemoryProperties) gpa(obj, "vkGetPhysicalDeviceMemoryProperties");
    table->GetPhysicalDeviceFeatures = (PFN_vkGetPhysicalDeviceFeatures) gpa(obj, "vkGetPhysicalDeviceFeatures");
    table->GetPhysicalDeviceFormatProperties = (PFN_vkGetPhysicalDeviceFormatProperties) gpa(obj, "vkGetPhysicalDeviceFormatProperties");
    table->GetPhysicalDeviceImageFormatProperties = (PFN_vkGetPhysicalDeviceImageFormatProperties) gpa(obj, "vkGetPhysicalDeviceImageFormatProperties");
    table->CreateDevice = (PFN_vkCreateDevice) gpa(obj, "vkCreateDevice");
    table->EnumerateDeviceLayerProperties = (PFN_vkEnumerateDeviceLayerProperties) gpa(obj, "vkEnumerateDeviceLayerProperties");
    table->EnumerateDeviceExtensionProperties = (PFN_vkEnumerateDeviceExtensionProperties) gpa(obj, "vkEnumerateDeviceExtensionProperties");
    table->GetPhysicalDeviceSparseImageFormatProperties = (PFN_vkGetPhysicalDeviceSparseImageFormatProperties) gpa(obj, "vkGetPhysicalDeviceSparseImageFormatProperties");
    table->GetPhysicalDeviceDisplayPropertiesKHR = (PFN_vkGetPhysicalDeviceDisplayPropertiesKHR) gpa(obj, "vkGetPhysicalDeviceDisplayPropertiesKHR");
    table->GetPhysicalDeviceDisplayPlanePropertiesKHR = (PFN_vkGetPhysicalDeviceDisplayPlanePropertiesKHR) gpa(obj, "vkGetPhysicalDeviceDisplayPlanePropertiesKHR");
    table->GetDisplayPlaneSupportedDisplaysKHR = (PFN_vkGetDisplayPlaneSupportedDisplaysKHR) gpa(obj, "vkGetDisplayPlaneSupportedDisplaysKHR");
    table->GetDisplayModePropertiesKHR = (PFN_vkGetDisplayModePropertiesKHR) gpa(obj, "vkGetDisplayModePropertiesKHR");
    table->CreateDisplayModeKHR = (PFN_vkCreateDisplayModeKHR) gpa(obj, "vkCreateDisplayModeKHR");
    table->GetDisplayPlaneCapabilitiesKHR = (PFN_vkGetDisplayPlaneCapabilitiesKHR) gpa(obj, "vkGetDisplayPlaneCapabilitiesKHR");
    table->GetPhysicalDeviceSurfaceSupportKHR = (PFN_vkGetPhysicalDeviceSurfaceSupportKHR) gpa(obj, "vkGetPhysicalDeviceSurfaceSupportKHR");
    table->GetPhysicalDeviceSurfaceCapabilitiesKHR = (PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR) gpa(obj, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR");
    table->GetPhysicalDeviceSurfaceFormatsKHR = (PFN_vkGetPhysicalDeviceSurfaceFormatsKHR) gpa(obj, "vkGetPhysicalDeviceSurfaceFormatsKHR");
    table->GetPhysicalDeviceSurfacePresentModesKHR = (PFN_vkGetPhysicalDeviceSurfacePresentModesKHR) gpa(obj, "vkGetPhysicalDeviceSurfacePresentModesKHR");
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
    table->GetPhysicalDeviceWaylandPresentationSupportKHR = (PFN_vkGetPhysicalDeviceWaylandPresentationSupportKHR) gpa(obj, "vkGetPhysicalDeviceWaylandPresentationSupportKHR");
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
    table->GetPhysicalDeviceWin32PresentationSupportKHR = (PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR) gpa(obj, "vkGetPhysicalDeviceWin32PresentationSupportKHR");
#endif
#ifdef VK_USE_PLATFORM_XLIB_KHR
    table->GetPhysicalDeviceXlibPresentationSupportKHR = (PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR) gpa(obj, "vkGetPhysicalDeviceXlibPresentationSupportKHR");
#endif
#ifdef VK_USE_PLATFORM_XCB_KHR
    table->GetPhysicalDeviceXcbPresentationSupportKHR = (PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR) gpa(obj, "vkGetPhysicalDeviceXcbPresentationSupportKHR");
#endif
#ifdef VK_USE_PLATFORM_DIRECTFB_EXT
    table->GetPhysicalDeviceDirectFBPresentationSupportEXT = (PFN_vkGetPhysicalDeviceDirectFBPresentationSupportEXT) gpa(obj, "vkGetPhysicalDeviceDirectFBPresentationSupportEXT");
#endif
#ifdef VK_USE_PLATFORM_SCREEN_QNX
    table->GetPhysicalDeviceScreenPresentationSupportQNX = (PFN_vkGetPhysicalDeviceScreenPresentationSupportQNX) gpa(obj, "vkGetPhysicalDeviceScreenPresentationSupportQNX");
#endif
    table->GetPhysicalDeviceExternalImageFormatPropertiesNV = (PFN_vkGetPhysicalDeviceExternalImageFormatPropertiesNV) gpa(obj, "vkGetPhysicalDeviceExternalImageFormatPropertiesNV");
    table->GetPhysicalDeviceFeatures2 = (PFN_vkGetPhysicalDeviceFeatures2) gpa(obj, "vkGetPhysicalDeviceFeatures2");
    if (table->GetPhysicalDeviceFeatures2 == NULL) {
        table->GetPhysicalDeviceFeatures2 = (PFN_vkGetPhysicalDeviceFeatures2) gpa(obj, "vkGetPhysicalDeviceFeatures2KHR");
    }
        table->GetPhysicalDeviceProperties2 = (PFN_vkGetPhysicalDeviceProperties2) gpa(obj, "vkGetPhysicalDeviceProperties2");
    if (table->GetPhysicalDeviceProperties2 == NULL) {
        table->GetPhysicalDeviceProperties2 = (PFN_vkGetPhysicalDeviceProperties2) gpa(obj, "vkGetPhysicalDeviceProperties2KHR");
    }
        table->GetPhysicalDeviceFormatProperties2 = (PFN_vkGetPhysicalDeviceFormatProperties2) gpa(obj, "vkGetPhysicalDeviceFormatProperties2");
    if (table->GetPhysicalDeviceFormatProperties2 == NULL) {
        table->GetPhysicalDeviceFormatProperties2 = (PFN_vkGetPhysicalDeviceFormatProperties2) gpa(obj, "vkGetPhysicalDeviceFormatProperties2KHR");
    }
        table->GetPhysicalDeviceImageFormatProperties2 = (PFN_vkGetPhysicalDeviceImageFormatProperties2) gpa(obj, "vkGetPhysicalDeviceImageFormatProperties2");
    if (table->GetPhysicalDeviceImageFormatProperties2 == NULL) {
        table->GetPhysicalDeviceImageFormatProperties2 = (PFN_vkGetPhysicalDeviceImageFormatProperties2) gpa(obj, "vkGetPhysicalDeviceImageFormatProperties2KHR");
    }
        table->GetPhysicalDeviceQueueFamilyProperties2 = (PFN_vkGetPhysicalDeviceQueueFamilyProperties2) gpa(obj, "vkGetPhysicalDeviceQueueFamilyProperties2");
    if (table->GetPhysicalDeviceQueueFamilyProperties2 == NULL) {
        table->GetPhysicalDeviceQueueFamilyProperties2 = (PFN_vkGetPhysicalDeviceQueueFamilyProperties2) gpa(obj, "vkGetPhysicalDeviceQueueFamilyProperties2KHR");
    }
        table->GetPhysicalDeviceMemoryProperties2 = (PFN_vkGetPhysicalDeviceMemoryProperties2) gpa(obj, "vkGetPhysicalDeviceMemoryProperties2");
    if (table->GetPhysicalDeviceMemoryProperties2 == NULL) {
        table->GetPhysicalDeviceMemoryProperties2 = (PFN_vkGetPhysicalDeviceMemoryProperties2) gpa(obj, "vkGetPhysicalDeviceMemoryProperties2KHR");
    }
        table->GetPhysicalDeviceSparseImageFormatProperties2 = (PFN_vkGetPhysicalDeviceSparseImageFormatProperties2) gpa(obj, "vkGetPhysicalDeviceSparseImageFormatProperties2");
    if (table->GetPhysicalDeviceSparseImageFormatProperties2 == NULL) {
        table->GetPhysicalDeviceSparseImageFormatProperties2 = (PFN_vkGetPhysicalDeviceSparseImageFormatProperties2) gpa(obj, "vkGetPhysicalDeviceSparseImageFormatProperties2KHR");
    }
        table->GetPhysicalDeviceExternalBufferProperties = (PFN_vkGetPhysicalDeviceExternalBufferProperties) gpa(obj, "vkGetPhysicalDeviceExternalBufferProperties");
    if (table->GetPhysicalDeviceExternalBufferProperties == NULL) {
        table->GetPhysicalDeviceExternalBufferProperties = (PFN_vkGetPhysicalDeviceExternalBufferProperties) gpa(obj, "vkGetPhysicalDeviceExternalBufferPropertiesKHR");
    }
        table->GetPhysicalDeviceExternalSemaphoreProperties = (PFN_vkGetPhysicalDeviceExternalSemaphoreProperties) gpa(obj, "vkGetPhysicalDeviceExternalSemaphoreProperties");
    if (table->GetPhysicalDeviceExternalSemaphoreProperties == NULL) {
        table->GetPhysicalDeviceExternalSemaphoreProperties = (PFN_vkGetPhysicalDeviceExternalSemaphoreProperties) gpa(obj, "vkGetPhysicalDeviceExternalSemaphorePropertiesKHR");
    }
        table->GetPhysicalDeviceExternalFenceProperties = (PFN_vkGetPhysicalDeviceExternalFenceProperties) gpa(obj, "vkGetPhysicalDeviceExternalFenceProperties");
    if (table->GetPhysicalDeviceExternalFenceProperties == NULL) {
        table->GetPhysicalDeviceExternalFenceProperties = (PFN_vkGetPhysicalDeviceExternalFenceProperties) gpa(obj, "vkGetPhysicalDeviceExternalFencePropertiesKHR");
    }
        table->ReleaseDisplayEXT = (PFN_vkReleaseDisplayEXT) gpa(obj, "vkReleaseDisplayEXT");
#ifdef VK_USE_PLATFORM_XLIB_XRANDR_EXT
    table->AcquireXlibDisplayEXT = (PFN_vkAcquireXlibDisplayEXT) gpa(obj, "vkAcquireXlibDisplayEXT");
#endif
#ifdef VK_USE_PLATFORM_XLIB_XRANDR_EXT
    table->GetRandROutputDisplayEXT = (PFN_vkGetRandROutputDisplayEXT) gpa(obj, "vkGetRandROutputDisplayEXT");
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
    table->AcquireWinrtDisplayNV = (PFN_vkAcquireWinrtDisplayNV) gpa(obj, "vkAcquireWinrtDisplayNV");
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
    table->GetWinrtDisplayNV = (PFN_vkGetWinrtDisplayNV) gpa(obj, "vkGetWinrtDisplayNV");
#endif
    table->GetPhysicalDeviceSurfaceCapabilities2EXT = (PFN_vkGetPhysicalDeviceSurfaceCapabilities2EXT) gpa(obj, "vkGetPhysicalDeviceSurfaceCapabilities2EXT");
    table->GetPhysicalDevicePresentRectanglesKHR = (PFN_vkGetPhysicalDevicePresentRectanglesKHR) gpa(obj, "vkGetPhysicalDevicePresentRectanglesKHR");
    table->GetPhysicalDeviceMultisamplePropertiesEXT = (PFN_vkGetPhysicalDeviceMultisamplePropertiesEXT) gpa(obj, "vkGetPhysicalDeviceMultisamplePropertiesEXT");
    table->GetPhysicalDeviceSurfaceCapabilities2KHR = (PFN_vkGetPhysicalDeviceSurfaceCapabilities2KHR) gpa(obj, "vkGetPhysicalDeviceSurfaceCapabilities2KHR");
    table->GetPhysicalDeviceSurfaceFormats2KHR = (PFN_vkGetPhysicalDeviceSurfaceFormats2KHR) gpa(obj, "vkGetPhysicalDeviceSurfaceFormats2KHR");
    table->GetPhysicalDeviceDisplayProperties2KHR = (PFN_vkGetPhysicalDeviceDisplayProperties2KHR) gpa(obj, "vkGetPhysicalDeviceDisplayProperties2KHR");
    table->GetPhysicalDeviceDisplayPlaneProperties2KHR = (PFN_vkGetPhysicalDeviceDisplayPlaneProperties2KHR) gpa(obj, "vkGetPhysicalDeviceDisplayPlaneProperties2KHR");
    table->GetDisplayModeProperties2KHR = (PFN_vkGetDisplayModeProperties2KHR) gpa(obj, "vkGetDisplayModeProperties2KHR");
    table->GetDisplayPlaneCapabilities2KHR = (PFN_vkGetDisplayPlaneCapabilities2KHR) gpa(obj, "vkGetDisplayPlaneCapabilities2KHR");
    table->GetPhysicalDeviceCalibrateableTimeDomainsEXT = (PFN_vkGetPhysicalDeviceCalibrateableTimeDomainsEXT) gpa(obj, "vkGetPhysicalDeviceCalibrateableTimeDomainsEXT");
    table->GetPhysicalDeviceCooperativeMatrixPropertiesNV = (PFN_vkGetPhysicalDeviceCooperativeMatrixPropertiesNV) gpa(obj, "vkGetPhysicalDeviceCooperativeMatrixPropertiesNV");
#ifdef VK_USE_PLATFORM_WIN32_KHR
    table->GetPhysicalDeviceSurfacePresentModes2EXT = (PFN_vkGetPhysicalDeviceSurfacePresentModes2EXT) gpa(obj, "vkGetPhysicalDeviceSurfacePresentModes2EXT");
#endif
    table->EnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR = (PFN_vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR) gpa(obj, "vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR");
    table->GetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR = (PFN_vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR) gpa(obj, "vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR");
    table->GetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV = (PFN_vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV) gpa(obj, "vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV");
    table->GetPhysicalDeviceToolProperties = (PFN_vkGetPhysicalDeviceToolProperties) gpa(obj, "vkGetPhysicalDeviceToolProperties");
    if (table->GetPhysicalDeviceToolProperties == NULL) {
        table->GetPhysicalDeviceToolProperties = (PFN_vkGetPhysicalDeviceToolProperties) gpa(obj, "vkGetPhysicalDeviceToolPropertiesEXT");
    }
        table->GetPhysicalDeviceFragmentShadingRatesKHR = (PFN_vkGetPhysicalDeviceFragmentShadingRatesKHR) gpa(obj, "vkGetPhysicalDeviceFragmentShadingRatesKHR");
    table->GetPhysicalDeviceVideoCapabilitiesKHR = (PFN_vkGetPhysicalDeviceVideoCapabilitiesKHR) gpa(obj, "vkGetPhysicalDeviceVideoCapabilitiesKHR");
    table->GetPhysicalDeviceVideoFormatPropertiesKHR = (PFN_vkGetPhysicalDeviceVideoFormatPropertiesKHR) gpa(obj, "vkGetPhysicalDeviceVideoFormatPropertiesKHR");
    table->AcquireDrmDisplayEXT = (PFN_vkAcquireDrmDisplayEXT) gpa(obj, "vkAcquireDrmDisplayEXT");
    table->GetDrmDisplayEXT = (PFN_vkGetDrmDisplayEXT) gpa(obj, "vkGetDrmDisplayEXT");
    table->GetPhysicalDeviceOpticalFlowImageFormatsNV = (PFN_vkGetPhysicalDeviceOpticalFlowImageFormatsNV) gpa(obj, "vkGetPhysicalDeviceOpticalFlowImageFormatsNV");
    table->GetPhysicalDeviceCooperativeMatrixPropertiesKHR = (PFN_vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR) gpa(obj, "vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR");
}



void
vk_device_dispatch_table_load(struct vk_device_dispatch_table *table,
                               PFN_vkGetDeviceProcAddr gpa,
                               VkDevice obj)
{
    table->GetDeviceProcAddr = gpa;
    table->GetDeviceProcAddr = (PFN_vkGetDeviceProcAddr) gpa(obj, "vkGetDeviceProcAddr");
    table->DestroyDevice = (PFN_vkDestroyDevice) gpa(obj, "vkDestroyDevice");
    table->GetDeviceQueue = (PFN_vkGetDeviceQueue) gpa(obj, "vkGetDeviceQueue");
    table->QueueSubmit = (PFN_vkQueueSubmit) gpa(obj, "vkQueueSubmit");
    table->QueueWaitIdle = (PFN_vkQueueWaitIdle) gpa(obj, "vkQueueWaitIdle");
    table->DeviceWaitIdle = (PFN_vkDeviceWaitIdle) gpa(obj, "vkDeviceWaitIdle");
    table->AllocateMemory = (PFN_vkAllocateMemory) gpa(obj, "vkAllocateMemory");
    table->FreeMemory = (PFN_vkFreeMemory) gpa(obj, "vkFreeMemory");
    table->MapMemory = (PFN_vkMapMemory) gpa(obj, "vkMapMemory");
    table->UnmapMemory = (PFN_vkUnmapMemory) gpa(obj, "vkUnmapMemory");
    table->FlushMappedMemoryRanges = (PFN_vkFlushMappedMemoryRanges) gpa(obj, "vkFlushMappedMemoryRanges");
    table->InvalidateMappedMemoryRanges = (PFN_vkInvalidateMappedMemoryRanges) gpa(obj, "vkInvalidateMappedMemoryRanges");
    table->GetDeviceMemoryCommitment = (PFN_vkGetDeviceMemoryCommitment) gpa(obj, "vkGetDeviceMemoryCommitment");
    table->GetBufferMemoryRequirements = (PFN_vkGetBufferMemoryRequirements) gpa(obj, "vkGetBufferMemoryRequirements");
    table->BindBufferMemory = (PFN_vkBindBufferMemory) gpa(obj, "vkBindBufferMemory");
    table->GetImageMemoryRequirements = (PFN_vkGetImageMemoryRequirements) gpa(obj, "vkGetImageMemoryRequirements");
    table->BindImageMemory = (PFN_vkBindImageMemory) gpa(obj, "vkBindImageMemory");
    table->GetImageSparseMemoryRequirements = (PFN_vkGetImageSparseMemoryRequirements) gpa(obj, "vkGetImageSparseMemoryRequirements");
    table->QueueBindSparse = (PFN_vkQueueBindSparse) gpa(obj, "vkQueueBindSparse");
    table->CreateFence = (PFN_vkCreateFence) gpa(obj, "vkCreateFence");
    table->DestroyFence = (PFN_vkDestroyFence) gpa(obj, "vkDestroyFence");
    table->ResetFences = (PFN_vkResetFences) gpa(obj, "vkResetFences");
    table->GetFenceStatus = (PFN_vkGetFenceStatus) gpa(obj, "vkGetFenceStatus");
    table->WaitForFences = (PFN_vkWaitForFences) gpa(obj, "vkWaitForFences");
    table->CreateSemaphore = (PFN_vkCreateSemaphore) gpa(obj, "vkCreateSemaphore");
    table->DestroySemaphore = (PFN_vkDestroySemaphore) gpa(obj, "vkDestroySemaphore");
    table->CreateEvent = (PFN_vkCreateEvent) gpa(obj, "vkCreateEvent");
    table->DestroyEvent = (PFN_vkDestroyEvent) gpa(obj, "vkDestroyEvent");
    table->GetEventStatus = (PFN_vkGetEventStatus) gpa(obj, "vkGetEventStatus");
    table->SetEvent = (PFN_vkSetEvent) gpa(obj, "vkSetEvent");
    table->ResetEvent = (PFN_vkResetEvent) gpa(obj, "vkResetEvent");
    table->CreateQueryPool = (PFN_vkCreateQueryPool) gpa(obj, "vkCreateQueryPool");
    table->DestroyQueryPool = (PFN_vkDestroyQueryPool) gpa(obj, "vkDestroyQueryPool");
    table->GetQueryPoolResults = (PFN_vkGetQueryPoolResults) gpa(obj, "vkGetQueryPoolResults");
    table->ResetQueryPool = (PFN_vkResetQueryPool) gpa(obj, "vkResetQueryPool");
    if (table->ResetQueryPool == NULL) {
        table->ResetQueryPool = (PFN_vkResetQueryPool) gpa(obj, "vkResetQueryPoolEXT");
    }
        table->CreateBuffer = (PFN_vkCreateBuffer) gpa(obj, "vkCreateBuffer");
    table->DestroyBuffer = (PFN_vkDestroyBuffer) gpa(obj, "vkDestroyBuffer");
    table->CreateBufferView = (PFN_vkCreateBufferView) gpa(obj, "vkCreateBufferView");
    table->DestroyBufferView = (PFN_vkDestroyBufferView) gpa(obj, "vkDestroyBufferView");
    table->CreateImage = (PFN_vkCreateImage) gpa(obj, "vkCreateImage");
    table->DestroyImage = (PFN_vkDestroyImage) gpa(obj, "vkDestroyImage");
    table->GetImageSubresourceLayout = (PFN_vkGetImageSubresourceLayout) gpa(obj, "vkGetImageSubresourceLayout");
    table->CreateImageView = (PFN_vkCreateImageView) gpa(obj, "vkCreateImageView");
    table->DestroyImageView = (PFN_vkDestroyImageView) gpa(obj, "vkDestroyImageView");
    table->CreateShaderModule = (PFN_vkCreateShaderModule) gpa(obj, "vkCreateShaderModule");
    table->DestroyShaderModule = (PFN_vkDestroyShaderModule) gpa(obj, "vkDestroyShaderModule");
    table->CreatePipelineCache = (PFN_vkCreatePipelineCache) gpa(obj, "vkCreatePipelineCache");
    table->DestroyPipelineCache = (PFN_vkDestroyPipelineCache) gpa(obj, "vkDestroyPipelineCache");
    table->GetPipelineCacheData = (PFN_vkGetPipelineCacheData) gpa(obj, "vkGetPipelineCacheData");
    table->MergePipelineCaches = (PFN_vkMergePipelineCaches) gpa(obj, "vkMergePipelineCaches");
    table->CreateGraphicsPipelines = (PFN_vkCreateGraphicsPipelines) gpa(obj, "vkCreateGraphicsPipelines");
    table->CreateComputePipelines = (PFN_vkCreateComputePipelines) gpa(obj, "vkCreateComputePipelines");
    table->GetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI = (PFN_vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI) gpa(obj, "vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI");
    table->DestroyPipeline = (PFN_vkDestroyPipeline) gpa(obj, "vkDestroyPipeline");
    table->CreatePipelineLayout = (PFN_vkCreatePipelineLayout) gpa(obj, "vkCreatePipelineLayout");
    table->DestroyPipelineLayout = (PFN_vkDestroyPipelineLayout) gpa(obj, "vkDestroyPipelineLayout");
    table->CreateSampler = (PFN_vkCreateSampler) gpa(obj, "vkCreateSampler");
    table->DestroySampler = (PFN_vkDestroySampler) gpa(obj, "vkDestroySampler");
    table->CreateDescriptorSetLayout = (PFN_vkCreateDescriptorSetLayout) gpa(obj, "vkCreateDescriptorSetLayout");
    table->DestroyDescriptorSetLayout = (PFN_vkDestroyDescriptorSetLayout) gpa(obj, "vkDestroyDescriptorSetLayout");
    table->CreateDescriptorPool = (PFN_vkCreateDescriptorPool) gpa(obj, "vkCreateDescriptorPool");
    table->DestroyDescriptorPool = (PFN_vkDestroyDescriptorPool) gpa(obj, "vkDestroyDescriptorPool");
    table->ResetDescriptorPool = (PFN_vkResetDescriptorPool) gpa(obj, "vkResetDescriptorPool");
    table->AllocateDescriptorSets = (PFN_vkAllocateDescriptorSets) gpa(obj, "vkAllocateDescriptorSets");
    table->FreeDescriptorSets = (PFN_vkFreeDescriptorSets) gpa(obj, "vkFreeDescriptorSets");
    table->UpdateDescriptorSets = (PFN_vkUpdateDescriptorSets) gpa(obj, "vkUpdateDescriptorSets");
    table->CreateFramebuffer = (PFN_vkCreateFramebuffer) gpa(obj, "vkCreateFramebuffer");
    table->DestroyFramebuffer = (PFN_vkDestroyFramebuffer) gpa(obj, "vkDestroyFramebuffer");
    table->CreateRenderPass = (PFN_vkCreateRenderPass) gpa(obj, "vkCreateRenderPass");
    table->DestroyRenderPass = (PFN_vkDestroyRenderPass) gpa(obj, "vkDestroyRenderPass");
    table->GetRenderAreaGranularity = (PFN_vkGetRenderAreaGranularity) gpa(obj, "vkGetRenderAreaGranularity");
    table->GetRenderingAreaGranularityKHR = (PFN_vkGetRenderingAreaGranularityKHR) gpa(obj, "vkGetRenderingAreaGranularityKHR");
    table->CreateCommandPool = (PFN_vkCreateCommandPool) gpa(obj, "vkCreateCommandPool");
    table->DestroyCommandPool = (PFN_vkDestroyCommandPool) gpa(obj, "vkDestroyCommandPool");
    table->ResetCommandPool = (PFN_vkResetCommandPool) gpa(obj, "vkResetCommandPool");
    table->AllocateCommandBuffers = (PFN_vkAllocateCommandBuffers) gpa(obj, "vkAllocateCommandBuffers");
    table->FreeCommandBuffers = (PFN_vkFreeCommandBuffers) gpa(obj, "vkFreeCommandBuffers");
    table->BeginCommandBuffer = (PFN_vkBeginCommandBuffer) gpa(obj, "vkBeginCommandBuffer");
    table->EndCommandBuffer = (PFN_vkEndCommandBuffer) gpa(obj, "vkEndCommandBuffer");
    table->ResetCommandBuffer = (PFN_vkResetCommandBuffer) gpa(obj, "vkResetCommandBuffer");
    table->CmdBindPipeline = (PFN_vkCmdBindPipeline) gpa(obj, "vkCmdBindPipeline");
    table->CmdSetAttachmentFeedbackLoopEnableEXT = (PFN_vkCmdSetAttachmentFeedbackLoopEnableEXT) gpa(obj, "vkCmdSetAttachmentFeedbackLoopEnableEXT");
    table->CmdSetViewport = (PFN_vkCmdSetViewport) gpa(obj, "vkCmdSetViewport");
    table->CmdSetScissor = (PFN_vkCmdSetScissor) gpa(obj, "vkCmdSetScissor");
    table->CmdSetLineWidth = (PFN_vkCmdSetLineWidth) gpa(obj, "vkCmdSetLineWidth");
    table->CmdSetDepthBias = (PFN_vkCmdSetDepthBias) gpa(obj, "vkCmdSetDepthBias");
    table->CmdSetBlendConstants = (PFN_vkCmdSetBlendConstants) gpa(obj, "vkCmdSetBlendConstants");
    table->CmdSetDepthBounds = (PFN_vkCmdSetDepthBounds) gpa(obj, "vkCmdSetDepthBounds");
    table->CmdSetStencilCompareMask = (PFN_vkCmdSetStencilCompareMask) gpa(obj, "vkCmdSetStencilCompareMask");
    table->CmdSetStencilWriteMask = (PFN_vkCmdSetStencilWriteMask) gpa(obj, "vkCmdSetStencilWriteMask");
    table->CmdSetStencilReference = (PFN_vkCmdSetStencilReference) gpa(obj, "vkCmdSetStencilReference");
    table->CmdBindDescriptorSets = (PFN_vkCmdBindDescriptorSets) gpa(obj, "vkCmdBindDescriptorSets");
    table->CmdBindIndexBuffer = (PFN_vkCmdBindIndexBuffer) gpa(obj, "vkCmdBindIndexBuffer");
    table->CmdBindVertexBuffers = (PFN_vkCmdBindVertexBuffers) gpa(obj, "vkCmdBindVertexBuffers");
    table->CmdDraw = (PFN_vkCmdDraw) gpa(obj, "vkCmdDraw");
    table->CmdDrawIndexed = (PFN_vkCmdDrawIndexed) gpa(obj, "vkCmdDrawIndexed");
    table->CmdDrawMultiEXT = (PFN_vkCmdDrawMultiEXT) gpa(obj, "vkCmdDrawMultiEXT");
    table->CmdDrawMultiIndexedEXT = (PFN_vkCmdDrawMultiIndexedEXT) gpa(obj, "vkCmdDrawMultiIndexedEXT");
    table->CmdDrawIndirect = (PFN_vkCmdDrawIndirect) gpa(obj, "vkCmdDrawIndirect");
    table->CmdDrawIndexedIndirect = (PFN_vkCmdDrawIndexedIndirect) gpa(obj, "vkCmdDrawIndexedIndirect");
    table->CmdDispatch = (PFN_vkCmdDispatch) gpa(obj, "vkCmdDispatch");
    table->CmdDispatchIndirect = (PFN_vkCmdDispatchIndirect) gpa(obj, "vkCmdDispatchIndirect");
    table->CmdSubpassShadingHUAWEI = (PFN_vkCmdSubpassShadingHUAWEI) gpa(obj, "vkCmdSubpassShadingHUAWEI");
    table->CmdDrawClusterHUAWEI = (PFN_vkCmdDrawClusterHUAWEI) gpa(obj, "vkCmdDrawClusterHUAWEI");
    table->CmdDrawClusterIndirectHUAWEI = (PFN_vkCmdDrawClusterIndirectHUAWEI) gpa(obj, "vkCmdDrawClusterIndirectHUAWEI");
    table->CmdUpdatePipelineIndirectBufferNV = (PFN_vkCmdUpdatePipelineIndirectBufferNV) gpa(obj, "vkCmdUpdatePipelineIndirectBufferNV");
    table->CmdCopyBuffer = (PFN_vkCmdCopyBuffer) gpa(obj, "vkCmdCopyBuffer");
    table->CmdCopyImage = (PFN_vkCmdCopyImage) gpa(obj, "vkCmdCopyImage");
    table->CmdBlitImage = (PFN_vkCmdBlitImage) gpa(obj, "vkCmdBlitImage");
    table->CmdCopyBufferToImage = (PFN_vkCmdCopyBufferToImage) gpa(obj, "vkCmdCopyBufferToImage");
    table->CmdCopyImageToBuffer = (PFN_vkCmdCopyImageToBuffer) gpa(obj, "vkCmdCopyImageToBuffer");
    table->CmdCopyMemoryIndirectNV = (PFN_vkCmdCopyMemoryIndirectNV) gpa(obj, "vkCmdCopyMemoryIndirectNV");
    table->CmdCopyMemoryToImageIndirectNV = (PFN_vkCmdCopyMemoryToImageIndirectNV) gpa(obj, "vkCmdCopyMemoryToImageIndirectNV");
    table->CmdUpdateBuffer = (PFN_vkCmdUpdateBuffer) gpa(obj, "vkCmdUpdateBuffer");
    table->CmdFillBuffer = (PFN_vkCmdFillBuffer) gpa(obj, "vkCmdFillBuffer");
    table->CmdClearColorImage = (PFN_vkCmdClearColorImage) gpa(obj, "vkCmdClearColorImage");
    table->CmdClearDepthStencilImage = (PFN_vkCmdClearDepthStencilImage) gpa(obj, "vkCmdClearDepthStencilImage");
    table->CmdClearAttachments = (PFN_vkCmdClearAttachments) gpa(obj, "vkCmdClearAttachments");
    table->CmdResolveImage = (PFN_vkCmdResolveImage) gpa(obj, "vkCmdResolveImage");
    table->CmdSetEvent = (PFN_vkCmdSetEvent) gpa(obj, "vkCmdSetEvent");
    table->CmdResetEvent = (PFN_vkCmdResetEvent) gpa(obj, "vkCmdResetEvent");
    table->CmdWaitEvents = (PFN_vkCmdWaitEvents) gpa(obj, "vkCmdWaitEvents");
    table->CmdPipelineBarrier = (PFN_vkCmdPipelineBarrier) gpa(obj, "vkCmdPipelineBarrier");
    table->CmdBeginQuery = (PFN_vkCmdBeginQuery) gpa(obj, "vkCmdBeginQuery");
    table->CmdEndQuery = (PFN_vkCmdEndQuery) gpa(obj, "vkCmdEndQuery");
    table->CmdBeginConditionalRenderingEXT = (PFN_vkCmdBeginConditionalRenderingEXT) gpa(obj, "vkCmdBeginConditionalRenderingEXT");
    table->CmdEndConditionalRenderingEXT = (PFN_vkCmdEndConditionalRenderingEXT) gpa(obj, "vkCmdEndConditionalRenderingEXT");
    table->CmdResetQueryPool = (PFN_vkCmdResetQueryPool) gpa(obj, "vkCmdResetQueryPool");
    table->CmdWriteTimestamp = (PFN_vkCmdWriteTimestamp) gpa(obj, "vkCmdWriteTimestamp");
    table->CmdCopyQueryPoolResults = (PFN_vkCmdCopyQueryPoolResults) gpa(obj, "vkCmdCopyQueryPoolResults");
    table->CmdPushConstants = (PFN_vkCmdPushConstants) gpa(obj, "vkCmdPushConstants");
    table->CmdBeginRenderPass = (PFN_vkCmdBeginRenderPass) gpa(obj, "vkCmdBeginRenderPass");
    table->CmdNextSubpass = (PFN_vkCmdNextSubpass) gpa(obj, "vkCmdNextSubpass");
    table->CmdEndRenderPass = (PFN_vkCmdEndRenderPass) gpa(obj, "vkCmdEndRenderPass");
    table->CmdExecuteCommands = (PFN_vkCmdExecuteCommands) gpa(obj, "vkCmdExecuteCommands");
    table->CreateSharedSwapchainsKHR = (PFN_vkCreateSharedSwapchainsKHR) gpa(obj, "vkCreateSharedSwapchainsKHR");
    table->CreateSwapchainKHR = (PFN_vkCreateSwapchainKHR) gpa(obj, "vkCreateSwapchainKHR");
    table->DestroySwapchainKHR = (PFN_vkDestroySwapchainKHR) gpa(obj, "vkDestroySwapchainKHR");
    table->GetSwapchainImagesKHR = (PFN_vkGetSwapchainImagesKHR) gpa(obj, "vkGetSwapchainImagesKHR");
    table->AcquireNextImageKHR = (PFN_vkAcquireNextImageKHR) gpa(obj, "vkAcquireNextImageKHR");
    table->QueuePresentKHR = (PFN_vkQueuePresentKHR) gpa(obj, "vkQueuePresentKHR");
    table->DebugMarkerSetObjectNameEXT = (PFN_vkDebugMarkerSetObjectNameEXT) gpa(obj, "vkDebugMarkerSetObjectNameEXT");
    table->DebugMarkerSetObjectTagEXT = (PFN_vkDebugMarkerSetObjectTagEXT) gpa(obj, "vkDebugMarkerSetObjectTagEXT");
    table->CmdDebugMarkerBeginEXT = (PFN_vkCmdDebugMarkerBeginEXT) gpa(obj, "vkCmdDebugMarkerBeginEXT");
    table->CmdDebugMarkerEndEXT = (PFN_vkCmdDebugMarkerEndEXT) gpa(obj, "vkCmdDebugMarkerEndEXT");
    table->CmdDebugMarkerInsertEXT = (PFN_vkCmdDebugMarkerInsertEXT) gpa(obj, "vkCmdDebugMarkerInsertEXT");
#ifdef VK_USE_PLATFORM_WIN32_KHR
    table->GetMemoryWin32HandleNV = (PFN_vkGetMemoryWin32HandleNV) gpa(obj, "vkGetMemoryWin32HandleNV");
#endif
    table->CmdExecuteGeneratedCommandsNV = (PFN_vkCmdExecuteGeneratedCommandsNV) gpa(obj, "vkCmdExecuteGeneratedCommandsNV");
    table->CmdPreprocessGeneratedCommandsNV = (PFN_vkCmdPreprocessGeneratedCommandsNV) gpa(obj, "vkCmdPreprocessGeneratedCommandsNV");
    table->CmdBindPipelineShaderGroupNV = (PFN_vkCmdBindPipelineShaderGroupNV) gpa(obj, "vkCmdBindPipelineShaderGroupNV");
    table->GetGeneratedCommandsMemoryRequirementsNV = (PFN_vkGetGeneratedCommandsMemoryRequirementsNV) gpa(obj, "vkGetGeneratedCommandsMemoryRequirementsNV");
    table->CreateIndirectCommandsLayoutNV = (PFN_vkCreateIndirectCommandsLayoutNV) gpa(obj, "vkCreateIndirectCommandsLayoutNV");
    table->DestroyIndirectCommandsLayoutNV = (PFN_vkDestroyIndirectCommandsLayoutNV) gpa(obj, "vkDestroyIndirectCommandsLayoutNV");
    table->CmdPushDescriptorSetKHR = (PFN_vkCmdPushDescriptorSetKHR) gpa(obj, "vkCmdPushDescriptorSetKHR");
    table->TrimCommandPool = (PFN_vkTrimCommandPool) gpa(obj, "vkTrimCommandPool");
    if (table->TrimCommandPool == NULL) {
        table->TrimCommandPool = (PFN_vkTrimCommandPool) gpa(obj, "vkTrimCommandPoolKHR");
    }
    #ifdef VK_USE_PLATFORM_WIN32_KHR
    table->GetMemoryWin32HandleKHR = (PFN_vkGetMemoryWin32HandleKHR) gpa(obj, "vkGetMemoryWin32HandleKHR");
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
    table->GetMemoryWin32HandlePropertiesKHR = (PFN_vkGetMemoryWin32HandlePropertiesKHR) gpa(obj, "vkGetMemoryWin32HandlePropertiesKHR");
#endif
    table->GetMemoryFdKHR = (PFN_vkGetMemoryFdKHR) gpa(obj, "vkGetMemoryFdKHR");
    table->GetMemoryFdPropertiesKHR = (PFN_vkGetMemoryFdPropertiesKHR) gpa(obj, "vkGetMemoryFdPropertiesKHR");
#ifdef VK_USE_PLATFORM_FUCHSIA
    table->GetMemoryZirconHandleFUCHSIA = (PFN_vkGetMemoryZirconHandleFUCHSIA) gpa(obj, "vkGetMemoryZirconHandleFUCHSIA");
#endif
#ifdef VK_USE_PLATFORM_FUCHSIA
    table->GetMemoryZirconHandlePropertiesFUCHSIA = (PFN_vkGetMemoryZirconHandlePropertiesFUCHSIA) gpa(obj, "vkGetMemoryZirconHandlePropertiesFUCHSIA");
#endif
    table->GetMemoryRemoteAddressNV = (PFN_vkGetMemoryRemoteAddressNV) gpa(obj, "vkGetMemoryRemoteAddressNV");
#ifdef VK_USE_PLATFORM_WIN32_KHR
    table->GetSemaphoreWin32HandleKHR = (PFN_vkGetSemaphoreWin32HandleKHR) gpa(obj, "vkGetSemaphoreWin32HandleKHR");
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
    table->ImportSemaphoreWin32HandleKHR = (PFN_vkImportSemaphoreWin32HandleKHR) gpa(obj, "vkImportSemaphoreWin32HandleKHR");
#endif
    table->GetSemaphoreFdKHR = (PFN_vkGetSemaphoreFdKHR) gpa(obj, "vkGetSemaphoreFdKHR");
    table->ImportSemaphoreFdKHR = (PFN_vkImportSemaphoreFdKHR) gpa(obj, "vkImportSemaphoreFdKHR");
#ifdef VK_USE_PLATFORM_FUCHSIA
    table->GetSemaphoreZirconHandleFUCHSIA = (PFN_vkGetSemaphoreZirconHandleFUCHSIA) gpa(obj, "vkGetSemaphoreZirconHandleFUCHSIA");
#endif
#ifdef VK_USE_PLATFORM_FUCHSIA
    table->ImportSemaphoreZirconHandleFUCHSIA = (PFN_vkImportSemaphoreZirconHandleFUCHSIA) gpa(obj, "vkImportSemaphoreZirconHandleFUCHSIA");
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
    table->GetFenceWin32HandleKHR = (PFN_vkGetFenceWin32HandleKHR) gpa(obj, "vkGetFenceWin32HandleKHR");
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
    table->ImportFenceWin32HandleKHR = (PFN_vkImportFenceWin32HandleKHR) gpa(obj, "vkImportFenceWin32HandleKHR");
#endif
    table->GetFenceFdKHR = (PFN_vkGetFenceFdKHR) gpa(obj, "vkGetFenceFdKHR");
    table->ImportFenceFdKHR = (PFN_vkImportFenceFdKHR) gpa(obj, "vkImportFenceFdKHR");
    table->DisplayPowerControlEXT = (PFN_vkDisplayPowerControlEXT) gpa(obj, "vkDisplayPowerControlEXT");
    table->RegisterDeviceEventEXT = (PFN_vkRegisterDeviceEventEXT) gpa(obj, "vkRegisterDeviceEventEXT");
    table->RegisterDisplayEventEXT = (PFN_vkRegisterDisplayEventEXT) gpa(obj, "vkRegisterDisplayEventEXT");
    table->GetSwapchainCounterEXT = (PFN_vkGetSwapchainCounterEXT) gpa(obj, "vkGetSwapchainCounterEXT");
    table->GetDeviceGroupPeerMemoryFeatures = (PFN_vkGetDeviceGroupPeerMemoryFeatures) gpa(obj, "vkGetDeviceGroupPeerMemoryFeatures");
    if (table->GetDeviceGroupPeerMemoryFeatures == NULL) {
        table->GetDeviceGroupPeerMemoryFeatures = (PFN_vkGetDeviceGroupPeerMemoryFeatures) gpa(obj, "vkGetDeviceGroupPeerMemoryFeaturesKHR");
    }
        table->BindBufferMemory2 = (PFN_vkBindBufferMemory2) gpa(obj, "vkBindBufferMemory2");
    if (table->BindBufferMemory2 == NULL) {
        table->BindBufferMemory2 = (PFN_vkBindBufferMemory2) gpa(obj, "vkBindBufferMemory2KHR");
    }
        table->BindImageMemory2 = (PFN_vkBindImageMemory2) gpa(obj, "vkBindImageMemory2");
    if (table->BindImageMemory2 == NULL) {
        table->BindImageMemory2 = (PFN_vkBindImageMemory2) gpa(obj, "vkBindImageMemory2KHR");
    }
        table->CmdSetDeviceMask = (PFN_vkCmdSetDeviceMask) gpa(obj, "vkCmdSetDeviceMask");
    if (table->CmdSetDeviceMask == NULL) {
        table->CmdSetDeviceMask = (PFN_vkCmdSetDeviceMask) gpa(obj, "vkCmdSetDeviceMaskKHR");
    }
        table->GetDeviceGroupPresentCapabilitiesKHR = (PFN_vkGetDeviceGroupPresentCapabilitiesKHR) gpa(obj, "vkGetDeviceGroupPresentCapabilitiesKHR");
    table->GetDeviceGroupSurfacePresentModesKHR = (PFN_vkGetDeviceGroupSurfacePresentModesKHR) gpa(obj, "vkGetDeviceGroupSurfacePresentModesKHR");
    table->AcquireNextImage2KHR = (PFN_vkAcquireNextImage2KHR) gpa(obj, "vkAcquireNextImage2KHR");
    table->CmdDispatchBase = (PFN_vkCmdDispatchBase) gpa(obj, "vkCmdDispatchBase");
    if (table->CmdDispatchBase == NULL) {
        table->CmdDispatchBase = (PFN_vkCmdDispatchBase) gpa(obj, "vkCmdDispatchBaseKHR");
    }
        table->CreateDescriptorUpdateTemplate = (PFN_vkCreateDescriptorUpdateTemplate) gpa(obj, "vkCreateDescriptorUpdateTemplate");
    if (table->CreateDescriptorUpdateTemplate == NULL) {
        table->CreateDescriptorUpdateTemplate = (PFN_vkCreateDescriptorUpdateTemplate) gpa(obj, "vkCreateDescriptorUpdateTemplateKHR");
    }
        table->DestroyDescriptorUpdateTemplate = (PFN_vkDestroyDescriptorUpdateTemplate) gpa(obj, "vkDestroyDescriptorUpdateTemplate");
    if (table->DestroyDescriptorUpdateTemplate == NULL) {
        table->DestroyDescriptorUpdateTemplate = (PFN_vkDestroyDescriptorUpdateTemplate) gpa(obj, "vkDestroyDescriptorUpdateTemplateKHR");
    }
        table->UpdateDescriptorSetWithTemplate = (PFN_vkUpdateDescriptorSetWithTemplate) gpa(obj, "vkUpdateDescriptorSetWithTemplate");
    if (table->UpdateDescriptorSetWithTemplate == NULL) {
        table->UpdateDescriptorSetWithTemplate = (PFN_vkUpdateDescriptorSetWithTemplate) gpa(obj, "vkUpdateDescriptorSetWithTemplateKHR");
    }
        table->CmdPushDescriptorSetWithTemplateKHR = (PFN_vkCmdPushDescriptorSetWithTemplateKHR) gpa(obj, "vkCmdPushDescriptorSetWithTemplateKHR");
    table->SetHdrMetadataEXT = (PFN_vkSetHdrMetadataEXT) gpa(obj, "vkSetHdrMetadataEXT");
    table->GetSwapchainStatusKHR = (PFN_vkGetSwapchainStatusKHR) gpa(obj, "vkGetSwapchainStatusKHR");
    table->GetRefreshCycleDurationGOOGLE = (PFN_vkGetRefreshCycleDurationGOOGLE) gpa(obj, "vkGetRefreshCycleDurationGOOGLE");
    table->GetPastPresentationTimingGOOGLE = (PFN_vkGetPastPresentationTimingGOOGLE) gpa(obj, "vkGetPastPresentationTimingGOOGLE");
    table->CmdSetViewportWScalingNV = (PFN_vkCmdSetViewportWScalingNV) gpa(obj, "vkCmdSetViewportWScalingNV");
    table->CmdSetDiscardRectangleEXT = (PFN_vkCmdSetDiscardRectangleEXT) gpa(obj, "vkCmdSetDiscardRectangleEXT");
    table->CmdSetDiscardRectangleEnableEXT = (PFN_vkCmdSetDiscardRectangleEnableEXT) gpa(obj, "vkCmdSetDiscardRectangleEnableEXT");
    table->CmdSetDiscardRectangleModeEXT = (PFN_vkCmdSetDiscardRectangleModeEXT) gpa(obj, "vkCmdSetDiscardRectangleModeEXT");
    table->CmdSetSampleLocationsEXT = (PFN_vkCmdSetSampleLocationsEXT) gpa(obj, "vkCmdSetSampleLocationsEXT");
    table->GetBufferMemoryRequirements2 = (PFN_vkGetBufferMemoryRequirements2) gpa(obj, "vkGetBufferMemoryRequirements2");
    if (table->GetBufferMemoryRequirements2 == NULL) {
        table->GetBufferMemoryRequirements2 = (PFN_vkGetBufferMemoryRequirements2) gpa(obj, "vkGetBufferMemoryRequirements2KHR");
    }
        table->GetImageMemoryRequirements2 = (PFN_vkGetImageMemoryRequirements2) gpa(obj, "vkGetImageMemoryRequirements2");
    if (table->GetImageMemoryRequirements2 == NULL) {
        table->GetImageMemoryRequirements2 = (PFN_vkGetImageMemoryRequirements2) gpa(obj, "vkGetImageMemoryRequirements2KHR");
    }
        table->GetImageSparseMemoryRequirements2 = (PFN_vkGetImageSparseMemoryRequirements2) gpa(obj, "vkGetImageSparseMemoryRequirements2");
    if (table->GetImageSparseMemoryRequirements2 == NULL) {
        table->GetImageSparseMemoryRequirements2 = (PFN_vkGetImageSparseMemoryRequirements2) gpa(obj, "vkGetImageSparseMemoryRequirements2KHR");
    }
        table->GetDeviceBufferMemoryRequirements = (PFN_vkGetDeviceBufferMemoryRequirements) gpa(obj, "vkGetDeviceBufferMemoryRequirements");
    if (table->GetDeviceBufferMemoryRequirements == NULL) {
        table->GetDeviceBufferMemoryRequirements = (PFN_vkGetDeviceBufferMemoryRequirements) gpa(obj, "vkGetDeviceBufferMemoryRequirementsKHR");
    }
        table->GetDeviceImageMemoryRequirements = (PFN_vkGetDeviceImageMemoryRequirements) gpa(obj, "vkGetDeviceImageMemoryRequirements");
    if (table->GetDeviceImageMemoryRequirements == NULL) {
        table->GetDeviceImageMemoryRequirements = (PFN_vkGetDeviceImageMemoryRequirements) gpa(obj, "vkGetDeviceImageMemoryRequirementsKHR");
    }
        table->GetDeviceImageSparseMemoryRequirements = (PFN_vkGetDeviceImageSparseMemoryRequirements) gpa(obj, "vkGetDeviceImageSparseMemoryRequirements");
    if (table->GetDeviceImageSparseMemoryRequirements == NULL) {
        table->GetDeviceImageSparseMemoryRequirements = (PFN_vkGetDeviceImageSparseMemoryRequirements) gpa(obj, "vkGetDeviceImageSparseMemoryRequirementsKHR");
    }
        table->CreateSamplerYcbcrConversion = (PFN_vkCreateSamplerYcbcrConversion) gpa(obj, "vkCreateSamplerYcbcrConversion");
    if (table->CreateSamplerYcbcrConversion == NULL) {
        table->CreateSamplerYcbcrConversion = (PFN_vkCreateSamplerYcbcrConversion) gpa(obj, "vkCreateSamplerYcbcrConversionKHR");
    }
        table->DestroySamplerYcbcrConversion = (PFN_vkDestroySamplerYcbcrConversion) gpa(obj, "vkDestroySamplerYcbcrConversion");
    if (table->DestroySamplerYcbcrConversion == NULL) {
        table->DestroySamplerYcbcrConversion = (PFN_vkDestroySamplerYcbcrConversion) gpa(obj, "vkDestroySamplerYcbcrConversionKHR");
    }
        table->GetDeviceQueue2 = (PFN_vkGetDeviceQueue2) gpa(obj, "vkGetDeviceQueue2");
    table->CreateValidationCacheEXT = (PFN_vkCreateValidationCacheEXT) gpa(obj, "vkCreateValidationCacheEXT");
    table->DestroyValidationCacheEXT = (PFN_vkDestroyValidationCacheEXT) gpa(obj, "vkDestroyValidationCacheEXT");
    table->GetValidationCacheDataEXT = (PFN_vkGetValidationCacheDataEXT) gpa(obj, "vkGetValidationCacheDataEXT");
    table->MergeValidationCachesEXT = (PFN_vkMergeValidationCachesEXT) gpa(obj, "vkMergeValidationCachesEXT");
    table->GetDescriptorSetLayoutSupport = (PFN_vkGetDescriptorSetLayoutSupport) gpa(obj, "vkGetDescriptorSetLayoutSupport");
    if (table->GetDescriptorSetLayoutSupport == NULL) {
        table->GetDescriptorSetLayoutSupport = (PFN_vkGetDescriptorSetLayoutSupport) gpa(obj, "vkGetDescriptorSetLayoutSupportKHR");
    }
    #ifdef VK_USE_PLATFORM_ANDROID_KHR
    table->GetSwapchainGrallocUsageANDROID = (PFN_vkGetSwapchainGrallocUsageANDROID) gpa(obj, "vkGetSwapchainGrallocUsageANDROID");
#endif
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    table->GetSwapchainGrallocUsage2ANDROID = (PFN_vkGetSwapchainGrallocUsage2ANDROID) gpa(obj, "vkGetSwapchainGrallocUsage2ANDROID");
#endif
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    table->AcquireImageANDROID = (PFN_vkAcquireImageANDROID) gpa(obj, "vkAcquireImageANDROID");
#endif
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    table->QueueSignalReleaseImageANDROID = (PFN_vkQueueSignalReleaseImageANDROID) gpa(obj, "vkQueueSignalReleaseImageANDROID");
#endif
    table->GetShaderInfoAMD = (PFN_vkGetShaderInfoAMD) gpa(obj, "vkGetShaderInfoAMD");
    table->SetLocalDimmingAMD = (PFN_vkSetLocalDimmingAMD) gpa(obj, "vkSetLocalDimmingAMD");
    table->GetCalibratedTimestampsEXT = (PFN_vkGetCalibratedTimestampsEXT) gpa(obj, "vkGetCalibratedTimestampsEXT");
    table->SetDebugUtilsObjectNameEXT = (PFN_vkSetDebugUtilsObjectNameEXT) gpa(obj, "vkSetDebugUtilsObjectNameEXT");
    table->SetDebugUtilsObjectTagEXT = (PFN_vkSetDebugUtilsObjectTagEXT) gpa(obj, "vkSetDebugUtilsObjectTagEXT");
    table->QueueBeginDebugUtilsLabelEXT = (PFN_vkQueueBeginDebugUtilsLabelEXT) gpa(obj, "vkQueueBeginDebugUtilsLabelEXT");
    table->QueueEndDebugUtilsLabelEXT = (PFN_vkQueueEndDebugUtilsLabelEXT) gpa(obj, "vkQueueEndDebugUtilsLabelEXT");
    table->QueueInsertDebugUtilsLabelEXT = (PFN_vkQueueInsertDebugUtilsLabelEXT) gpa(obj, "vkQueueInsertDebugUtilsLabelEXT");
    table->CmdBeginDebugUtilsLabelEXT = (PFN_vkCmdBeginDebugUtilsLabelEXT) gpa(obj, "vkCmdBeginDebugUtilsLabelEXT");
    table->CmdEndDebugUtilsLabelEXT = (PFN_vkCmdEndDebugUtilsLabelEXT) gpa(obj, "vkCmdEndDebugUtilsLabelEXT");
    table->CmdInsertDebugUtilsLabelEXT = (PFN_vkCmdInsertDebugUtilsLabelEXT) gpa(obj, "vkCmdInsertDebugUtilsLabelEXT");
    table->GetMemoryHostPointerPropertiesEXT = (PFN_vkGetMemoryHostPointerPropertiesEXT) gpa(obj, "vkGetMemoryHostPointerPropertiesEXT");
    table->CmdWriteBufferMarkerAMD = (PFN_vkCmdWriteBufferMarkerAMD) gpa(obj, "vkCmdWriteBufferMarkerAMD");
    table->CreateRenderPass2 = (PFN_vkCreateRenderPass2) gpa(obj, "vkCreateRenderPass2");
    if (table->CreateRenderPass2 == NULL) {
        table->CreateRenderPass2 = (PFN_vkCreateRenderPass2) gpa(obj, "vkCreateRenderPass2KHR");
    }
        table->CmdBeginRenderPass2 = (PFN_vkCmdBeginRenderPass2) gpa(obj, "vkCmdBeginRenderPass2");
    if (table->CmdBeginRenderPass2 == NULL) {
        table->CmdBeginRenderPass2 = (PFN_vkCmdBeginRenderPass2) gpa(obj, "vkCmdBeginRenderPass2KHR");
    }
        table->CmdNextSubpass2 = (PFN_vkCmdNextSubpass2) gpa(obj, "vkCmdNextSubpass2");
    if (table->CmdNextSubpass2 == NULL) {
        table->CmdNextSubpass2 = (PFN_vkCmdNextSubpass2) gpa(obj, "vkCmdNextSubpass2KHR");
    }
        table->CmdEndRenderPass2 = (PFN_vkCmdEndRenderPass2) gpa(obj, "vkCmdEndRenderPass2");
    if (table->CmdEndRenderPass2 == NULL) {
        table->CmdEndRenderPass2 = (PFN_vkCmdEndRenderPass2) gpa(obj, "vkCmdEndRenderPass2KHR");
    }
        table->GetSemaphoreCounterValue = (PFN_vkGetSemaphoreCounterValue) gpa(obj, "vkGetSemaphoreCounterValue");
    if (table->GetSemaphoreCounterValue == NULL) {
        table->GetSemaphoreCounterValue = (PFN_vkGetSemaphoreCounterValue) gpa(obj, "vkGetSemaphoreCounterValueKHR");
    }
        table->WaitSemaphores = (PFN_vkWaitSemaphores) gpa(obj, "vkWaitSemaphores");
    if (table->WaitSemaphores == NULL) {
        table->WaitSemaphores = (PFN_vkWaitSemaphores) gpa(obj, "vkWaitSemaphoresKHR");
    }
        table->SignalSemaphore = (PFN_vkSignalSemaphore) gpa(obj, "vkSignalSemaphore");
    if (table->SignalSemaphore == NULL) {
        table->SignalSemaphore = (PFN_vkSignalSemaphore) gpa(obj, "vkSignalSemaphoreKHR");
    }
    #ifdef VK_USE_PLATFORM_ANDROID_KHR
    table->GetAndroidHardwareBufferPropertiesANDROID = (PFN_vkGetAndroidHardwareBufferPropertiesANDROID) gpa(obj, "vkGetAndroidHardwareBufferPropertiesANDROID");
#endif
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    table->GetMemoryAndroidHardwareBufferANDROID = (PFN_vkGetMemoryAndroidHardwareBufferANDROID) gpa(obj, "vkGetMemoryAndroidHardwareBufferANDROID");
#endif
    table->CmdDrawIndirectCount = (PFN_vkCmdDrawIndirectCount) gpa(obj, "vkCmdDrawIndirectCount");
    if (table->CmdDrawIndirectCount == NULL) {
        table->CmdDrawIndirectCount = (PFN_vkCmdDrawIndirectCount) gpa(obj, "vkCmdDrawIndirectCountKHR");
    }
    if (table->CmdDrawIndirectCount == NULL) {
        table->CmdDrawIndirectCount = (PFN_vkCmdDrawIndirectCount) gpa(obj, "vkCmdDrawIndirectCountAMD");
    }
            table->CmdDrawIndexedIndirectCount = (PFN_vkCmdDrawIndexedIndirectCount) gpa(obj, "vkCmdDrawIndexedIndirectCount");
    if (table->CmdDrawIndexedIndirectCount == NULL) {
        table->CmdDrawIndexedIndirectCount = (PFN_vkCmdDrawIndexedIndirectCount) gpa(obj, "vkCmdDrawIndexedIndirectCountKHR");
    }
    if (table->CmdDrawIndexedIndirectCount == NULL) {
        table->CmdDrawIndexedIndirectCount = (PFN_vkCmdDrawIndexedIndirectCount) gpa(obj, "vkCmdDrawIndexedIndirectCountAMD");
    }
            table->CmdSetCheckpointNV = (PFN_vkCmdSetCheckpointNV) gpa(obj, "vkCmdSetCheckpointNV");
    table->GetQueueCheckpointDataNV = (PFN_vkGetQueueCheckpointDataNV) gpa(obj, "vkGetQueueCheckpointDataNV");
    table->CmdBindTransformFeedbackBuffersEXT = (PFN_vkCmdBindTransformFeedbackBuffersEXT) gpa(obj, "vkCmdBindTransformFeedbackBuffersEXT");
    table->CmdBeginTransformFeedbackEXT = (PFN_vkCmdBeginTransformFeedbackEXT) gpa(obj, "vkCmdBeginTransformFeedbackEXT");
    table->CmdEndTransformFeedbackEXT = (PFN_vkCmdEndTransformFeedbackEXT) gpa(obj, "vkCmdEndTransformFeedbackEXT");
    table->CmdBeginQueryIndexedEXT = (PFN_vkCmdBeginQueryIndexedEXT) gpa(obj, "vkCmdBeginQueryIndexedEXT");
    table->CmdEndQueryIndexedEXT = (PFN_vkCmdEndQueryIndexedEXT) gpa(obj, "vkCmdEndQueryIndexedEXT");
    table->CmdDrawIndirectByteCountEXT = (PFN_vkCmdDrawIndirectByteCountEXT) gpa(obj, "vkCmdDrawIndirectByteCountEXT");
    table->CmdSetExclusiveScissorNV = (PFN_vkCmdSetExclusiveScissorNV) gpa(obj, "vkCmdSetExclusiveScissorNV");
    table->CmdSetExclusiveScissorEnableNV = (PFN_vkCmdSetExclusiveScissorEnableNV) gpa(obj, "vkCmdSetExclusiveScissorEnableNV");
    table->CmdBindShadingRateImageNV = (PFN_vkCmdBindShadingRateImageNV) gpa(obj, "vkCmdBindShadingRateImageNV");
    table->CmdSetViewportShadingRatePaletteNV = (PFN_vkCmdSetViewportShadingRatePaletteNV) gpa(obj, "vkCmdSetViewportShadingRatePaletteNV");
    table->CmdSetCoarseSampleOrderNV = (PFN_vkCmdSetCoarseSampleOrderNV) gpa(obj, "vkCmdSetCoarseSampleOrderNV");
    table->CmdDrawMeshTasksNV = (PFN_vkCmdDrawMeshTasksNV) gpa(obj, "vkCmdDrawMeshTasksNV");
    table->CmdDrawMeshTasksIndirectNV = (PFN_vkCmdDrawMeshTasksIndirectNV) gpa(obj, "vkCmdDrawMeshTasksIndirectNV");
    table->CmdDrawMeshTasksIndirectCountNV = (PFN_vkCmdDrawMeshTasksIndirectCountNV) gpa(obj, "vkCmdDrawMeshTasksIndirectCountNV");
    table->CmdDrawMeshTasksEXT = (PFN_vkCmdDrawMeshTasksEXT) gpa(obj, "vkCmdDrawMeshTasksEXT");
    table->CmdDrawMeshTasksIndirectEXT = (PFN_vkCmdDrawMeshTasksIndirectEXT) gpa(obj, "vkCmdDrawMeshTasksIndirectEXT");
    table->CmdDrawMeshTasksIndirectCountEXT = (PFN_vkCmdDrawMeshTasksIndirectCountEXT) gpa(obj, "vkCmdDrawMeshTasksIndirectCountEXT");
    table->CompileDeferredNV = (PFN_vkCompileDeferredNV) gpa(obj, "vkCompileDeferredNV");
    table->CreateAccelerationStructureNV = (PFN_vkCreateAccelerationStructureNV) gpa(obj, "vkCreateAccelerationStructureNV");
    table->CmdBindInvocationMaskHUAWEI = (PFN_vkCmdBindInvocationMaskHUAWEI) gpa(obj, "vkCmdBindInvocationMaskHUAWEI");
    table->DestroyAccelerationStructureKHR = (PFN_vkDestroyAccelerationStructureKHR) gpa(obj, "vkDestroyAccelerationStructureKHR");
    table->DestroyAccelerationStructureNV = (PFN_vkDestroyAccelerationStructureNV) gpa(obj, "vkDestroyAccelerationStructureNV");
    table->GetAccelerationStructureMemoryRequirementsNV = (PFN_vkGetAccelerationStructureMemoryRequirementsNV) gpa(obj, "vkGetAccelerationStructureMemoryRequirementsNV");
    table->BindAccelerationStructureMemoryNV = (PFN_vkBindAccelerationStructureMemoryNV) gpa(obj, "vkBindAccelerationStructureMemoryNV");
    table->CmdCopyAccelerationStructureNV = (PFN_vkCmdCopyAccelerationStructureNV) gpa(obj, "vkCmdCopyAccelerationStructureNV");
    table->CmdCopyAccelerationStructureKHR = (PFN_vkCmdCopyAccelerationStructureKHR) gpa(obj, "vkCmdCopyAccelerationStructureKHR");
    table->CopyAccelerationStructureKHR = (PFN_vkCopyAccelerationStructureKHR) gpa(obj, "vkCopyAccelerationStructureKHR");
    table->CmdCopyAccelerationStructureToMemoryKHR = (PFN_vkCmdCopyAccelerationStructureToMemoryKHR) gpa(obj, "vkCmdCopyAccelerationStructureToMemoryKHR");
    table->CopyAccelerationStructureToMemoryKHR = (PFN_vkCopyAccelerationStructureToMemoryKHR) gpa(obj, "vkCopyAccelerationStructureToMemoryKHR");
    table->CmdCopyMemoryToAccelerationStructureKHR = (PFN_vkCmdCopyMemoryToAccelerationStructureKHR) gpa(obj, "vkCmdCopyMemoryToAccelerationStructureKHR");
    table->CopyMemoryToAccelerationStructureKHR = (PFN_vkCopyMemoryToAccelerationStructureKHR) gpa(obj, "vkCopyMemoryToAccelerationStructureKHR");
    table->CmdWriteAccelerationStructuresPropertiesKHR = (PFN_vkCmdWriteAccelerationStructuresPropertiesKHR) gpa(obj, "vkCmdWriteAccelerationStructuresPropertiesKHR");
    table->CmdWriteAccelerationStructuresPropertiesNV = (PFN_vkCmdWriteAccelerationStructuresPropertiesNV) gpa(obj, "vkCmdWriteAccelerationStructuresPropertiesNV");
    table->CmdBuildAccelerationStructureNV = (PFN_vkCmdBuildAccelerationStructureNV) gpa(obj, "vkCmdBuildAccelerationStructureNV");
    table->WriteAccelerationStructuresPropertiesKHR = (PFN_vkWriteAccelerationStructuresPropertiesKHR) gpa(obj, "vkWriteAccelerationStructuresPropertiesKHR");
    table->CmdTraceRaysKHR = (PFN_vkCmdTraceRaysKHR) gpa(obj, "vkCmdTraceRaysKHR");
    table->CmdTraceRaysNV = (PFN_vkCmdTraceRaysNV) gpa(obj, "vkCmdTraceRaysNV");
    table->GetRayTracingShaderGroupHandlesKHR = (PFN_vkGetRayTracingShaderGroupHandlesKHR) gpa(obj, "vkGetRayTracingShaderGroupHandlesKHR");
    if (table->GetRayTracingShaderGroupHandlesKHR == NULL) {
        table->GetRayTracingShaderGroupHandlesKHR = (PFN_vkGetRayTracingShaderGroupHandlesKHR) gpa(obj, "vkGetRayTracingShaderGroupHandlesNV");
    }
        table->GetRayTracingCaptureReplayShaderGroupHandlesKHR = (PFN_vkGetRayTracingCaptureReplayShaderGroupHandlesKHR) gpa(obj, "vkGetRayTracingCaptureReplayShaderGroupHandlesKHR");
    table->GetAccelerationStructureHandleNV = (PFN_vkGetAccelerationStructureHandleNV) gpa(obj, "vkGetAccelerationStructureHandleNV");
    table->CreateRayTracingPipelinesNV = (PFN_vkCreateRayTracingPipelinesNV) gpa(obj, "vkCreateRayTracingPipelinesNV");
    table->CreateRayTracingPipelinesKHR = (PFN_vkCreateRayTracingPipelinesKHR) gpa(obj, "vkCreateRayTracingPipelinesKHR");
    table->CmdTraceRaysIndirectKHR = (PFN_vkCmdTraceRaysIndirectKHR) gpa(obj, "vkCmdTraceRaysIndirectKHR");
    table->CmdTraceRaysIndirect2KHR = (PFN_vkCmdTraceRaysIndirect2KHR) gpa(obj, "vkCmdTraceRaysIndirect2KHR");
    table->GetDeviceAccelerationStructureCompatibilityKHR = (PFN_vkGetDeviceAccelerationStructureCompatibilityKHR) gpa(obj, "vkGetDeviceAccelerationStructureCompatibilityKHR");
    table->GetRayTracingShaderGroupStackSizeKHR = (PFN_vkGetRayTracingShaderGroupStackSizeKHR) gpa(obj, "vkGetRayTracingShaderGroupStackSizeKHR");
    table->CmdSetRayTracingPipelineStackSizeKHR = (PFN_vkCmdSetRayTracingPipelineStackSizeKHR) gpa(obj, "vkCmdSetRayTracingPipelineStackSizeKHR");
    table->GetImageViewHandleNVX = (PFN_vkGetImageViewHandleNVX) gpa(obj, "vkGetImageViewHandleNVX");
    table->GetImageViewAddressNVX = (PFN_vkGetImageViewAddressNVX) gpa(obj, "vkGetImageViewAddressNVX");
#ifdef VK_USE_PLATFORM_WIN32_KHR
    table->GetDeviceGroupSurfacePresentModes2EXT = (PFN_vkGetDeviceGroupSurfacePresentModes2EXT) gpa(obj, "vkGetDeviceGroupSurfacePresentModes2EXT");
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
    table->AcquireFullScreenExclusiveModeEXT = (PFN_vkAcquireFullScreenExclusiveModeEXT) gpa(obj, "vkAcquireFullScreenExclusiveModeEXT");
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
    table->ReleaseFullScreenExclusiveModeEXT = (PFN_vkReleaseFullScreenExclusiveModeEXT) gpa(obj, "vkReleaseFullScreenExclusiveModeEXT");
#endif
    table->AcquireProfilingLockKHR = (PFN_vkAcquireProfilingLockKHR) gpa(obj, "vkAcquireProfilingLockKHR");
    table->ReleaseProfilingLockKHR = (PFN_vkReleaseProfilingLockKHR) gpa(obj, "vkReleaseProfilingLockKHR");
    table->GetImageDrmFormatModifierPropertiesEXT = (PFN_vkGetImageDrmFormatModifierPropertiesEXT) gpa(obj, "vkGetImageDrmFormatModifierPropertiesEXT");
    table->GetBufferOpaqueCaptureAddress = (PFN_vkGetBufferOpaqueCaptureAddress) gpa(obj, "vkGetBufferOpaqueCaptureAddress");
    if (table->GetBufferOpaqueCaptureAddress == NULL) {
        table->GetBufferOpaqueCaptureAddress = (PFN_vkGetBufferOpaqueCaptureAddress) gpa(obj, "vkGetBufferOpaqueCaptureAddressKHR");
    }
        table->GetBufferDeviceAddress = (PFN_vkGetBufferDeviceAddress) gpa(obj, "vkGetBufferDeviceAddress");
    if (table->GetBufferDeviceAddress == NULL) {
        table->GetBufferDeviceAddress = (PFN_vkGetBufferDeviceAddress) gpa(obj, "vkGetBufferDeviceAddressKHR");
    }
    if (table->GetBufferDeviceAddress == NULL) {
        table->GetBufferDeviceAddress = (PFN_vkGetBufferDeviceAddress) gpa(obj, "vkGetBufferDeviceAddressEXT");
    }
            table->InitializePerformanceApiINTEL = (PFN_vkInitializePerformanceApiINTEL) gpa(obj, "vkInitializePerformanceApiINTEL");
    table->UninitializePerformanceApiINTEL = (PFN_vkUninitializePerformanceApiINTEL) gpa(obj, "vkUninitializePerformanceApiINTEL");
    table->CmdSetPerformanceMarkerINTEL = (PFN_vkCmdSetPerformanceMarkerINTEL) gpa(obj, "vkCmdSetPerformanceMarkerINTEL");
    table->CmdSetPerformanceStreamMarkerINTEL = (PFN_vkCmdSetPerformanceStreamMarkerINTEL) gpa(obj, "vkCmdSetPerformanceStreamMarkerINTEL");
    table->CmdSetPerformanceOverrideINTEL = (PFN_vkCmdSetPerformanceOverrideINTEL) gpa(obj, "vkCmdSetPerformanceOverrideINTEL");
    table->AcquirePerformanceConfigurationINTEL = (PFN_vkAcquirePerformanceConfigurationINTEL) gpa(obj, "vkAcquirePerformanceConfigurationINTEL");
    table->ReleasePerformanceConfigurationINTEL = (PFN_vkReleasePerformanceConfigurationINTEL) gpa(obj, "vkReleasePerformanceConfigurationINTEL");
    table->QueueSetPerformanceConfigurationINTEL = (PFN_vkQueueSetPerformanceConfigurationINTEL) gpa(obj, "vkQueueSetPerformanceConfigurationINTEL");
    table->GetPerformanceParameterINTEL = (PFN_vkGetPerformanceParameterINTEL) gpa(obj, "vkGetPerformanceParameterINTEL");
    table->GetDeviceMemoryOpaqueCaptureAddress = (PFN_vkGetDeviceMemoryOpaqueCaptureAddress) gpa(obj, "vkGetDeviceMemoryOpaqueCaptureAddress");
    if (table->GetDeviceMemoryOpaqueCaptureAddress == NULL) {
        table->GetDeviceMemoryOpaqueCaptureAddress = (PFN_vkGetDeviceMemoryOpaqueCaptureAddress) gpa(obj, "vkGetDeviceMemoryOpaqueCaptureAddressKHR");
    }
        table->GetPipelineExecutablePropertiesKHR = (PFN_vkGetPipelineExecutablePropertiesKHR) gpa(obj, "vkGetPipelineExecutablePropertiesKHR");
    table->GetPipelineExecutableStatisticsKHR = (PFN_vkGetPipelineExecutableStatisticsKHR) gpa(obj, "vkGetPipelineExecutableStatisticsKHR");
    table->GetPipelineExecutableInternalRepresentationsKHR = (PFN_vkGetPipelineExecutableInternalRepresentationsKHR) gpa(obj, "vkGetPipelineExecutableInternalRepresentationsKHR");
    table->CmdSetLineStippleEXT = (PFN_vkCmdSetLineStippleEXT) gpa(obj, "vkCmdSetLineStippleEXT");
    table->CreateAccelerationStructureKHR = (PFN_vkCreateAccelerationStructureKHR) gpa(obj, "vkCreateAccelerationStructureKHR");
    table->CmdBuildAccelerationStructuresKHR = (PFN_vkCmdBuildAccelerationStructuresKHR) gpa(obj, "vkCmdBuildAccelerationStructuresKHR");
    table->CmdBuildAccelerationStructuresIndirectKHR = (PFN_vkCmdBuildAccelerationStructuresIndirectKHR) gpa(obj, "vkCmdBuildAccelerationStructuresIndirectKHR");
    table->BuildAccelerationStructuresKHR = (PFN_vkBuildAccelerationStructuresKHR) gpa(obj, "vkBuildAccelerationStructuresKHR");
    table->GetAccelerationStructureDeviceAddressKHR = (PFN_vkGetAccelerationStructureDeviceAddressKHR) gpa(obj, "vkGetAccelerationStructureDeviceAddressKHR");
    table->CreateDeferredOperationKHR = (PFN_vkCreateDeferredOperationKHR) gpa(obj, "vkCreateDeferredOperationKHR");
    table->DestroyDeferredOperationKHR = (PFN_vkDestroyDeferredOperationKHR) gpa(obj, "vkDestroyDeferredOperationKHR");
    table->GetDeferredOperationMaxConcurrencyKHR = (PFN_vkGetDeferredOperationMaxConcurrencyKHR) gpa(obj, "vkGetDeferredOperationMaxConcurrencyKHR");
    table->GetDeferredOperationResultKHR = (PFN_vkGetDeferredOperationResultKHR) gpa(obj, "vkGetDeferredOperationResultKHR");
    table->DeferredOperationJoinKHR = (PFN_vkDeferredOperationJoinKHR) gpa(obj, "vkDeferredOperationJoinKHR");
    table->GetPipelineIndirectMemoryRequirementsNV = (PFN_vkGetPipelineIndirectMemoryRequirementsNV) gpa(obj, "vkGetPipelineIndirectMemoryRequirementsNV");
    table->GetPipelineIndirectDeviceAddressNV = (PFN_vkGetPipelineIndirectDeviceAddressNV) gpa(obj, "vkGetPipelineIndirectDeviceAddressNV");
    table->CmdSetCullMode = (PFN_vkCmdSetCullMode) gpa(obj, "vkCmdSetCullMode");
    if (table->CmdSetCullMode == NULL) {
        table->CmdSetCullMode = (PFN_vkCmdSetCullMode) gpa(obj, "vkCmdSetCullModeEXT");
    }
        table->CmdSetFrontFace = (PFN_vkCmdSetFrontFace) gpa(obj, "vkCmdSetFrontFace");
    if (table->CmdSetFrontFace == NULL) {
        table->CmdSetFrontFace = (PFN_vkCmdSetFrontFace) gpa(obj, "vkCmdSetFrontFaceEXT");
    }
        table->CmdSetPrimitiveTopology = (PFN_vkCmdSetPrimitiveTopology) gpa(obj, "vkCmdSetPrimitiveTopology");
    if (table->CmdSetPrimitiveTopology == NULL) {
        table->CmdSetPrimitiveTopology = (PFN_vkCmdSetPrimitiveTopology) gpa(obj, "vkCmdSetPrimitiveTopologyEXT");
    }
        table->CmdSetViewportWithCount = (PFN_vkCmdSetViewportWithCount) gpa(obj, "vkCmdSetViewportWithCount");
    if (table->CmdSetViewportWithCount == NULL) {
        table->CmdSetViewportWithCount = (PFN_vkCmdSetViewportWithCount) gpa(obj, "vkCmdSetViewportWithCountEXT");
    }
        table->CmdSetScissorWithCount = (PFN_vkCmdSetScissorWithCount) gpa(obj, "vkCmdSetScissorWithCount");
    if (table->CmdSetScissorWithCount == NULL) {
        table->CmdSetScissorWithCount = (PFN_vkCmdSetScissorWithCount) gpa(obj, "vkCmdSetScissorWithCountEXT");
    }
        table->CmdBindIndexBuffer2KHR = (PFN_vkCmdBindIndexBuffer2KHR) gpa(obj, "vkCmdBindIndexBuffer2KHR");
    table->CmdBindVertexBuffers2 = (PFN_vkCmdBindVertexBuffers2) gpa(obj, "vkCmdBindVertexBuffers2");
    if (table->CmdBindVertexBuffers2 == NULL) {
        table->CmdBindVertexBuffers2 = (PFN_vkCmdBindVertexBuffers2) gpa(obj, "vkCmdBindVertexBuffers2EXT");
    }
        table->CmdSetDepthTestEnable = (PFN_vkCmdSetDepthTestEnable) gpa(obj, "vkCmdSetDepthTestEnable");
    if (table->CmdSetDepthTestEnable == NULL) {
        table->CmdSetDepthTestEnable = (PFN_vkCmdSetDepthTestEnable) gpa(obj, "vkCmdSetDepthTestEnableEXT");
    }
        table->CmdSetDepthWriteEnable = (PFN_vkCmdSetDepthWriteEnable) gpa(obj, "vkCmdSetDepthWriteEnable");
    if (table->CmdSetDepthWriteEnable == NULL) {
        table->CmdSetDepthWriteEnable = (PFN_vkCmdSetDepthWriteEnable) gpa(obj, "vkCmdSetDepthWriteEnableEXT");
    }
        table->CmdSetDepthCompareOp = (PFN_vkCmdSetDepthCompareOp) gpa(obj, "vkCmdSetDepthCompareOp");
    if (table->CmdSetDepthCompareOp == NULL) {
        table->CmdSetDepthCompareOp = (PFN_vkCmdSetDepthCompareOp) gpa(obj, "vkCmdSetDepthCompareOpEXT");
    }
        table->CmdSetDepthBoundsTestEnable = (PFN_vkCmdSetDepthBoundsTestEnable) gpa(obj, "vkCmdSetDepthBoundsTestEnable");
    if (table->CmdSetDepthBoundsTestEnable == NULL) {
        table->CmdSetDepthBoundsTestEnable = (PFN_vkCmdSetDepthBoundsTestEnable) gpa(obj, "vkCmdSetDepthBoundsTestEnableEXT");
    }
        table->CmdSetStencilTestEnable = (PFN_vkCmdSetStencilTestEnable) gpa(obj, "vkCmdSetStencilTestEnable");
    if (table->CmdSetStencilTestEnable == NULL) {
        table->CmdSetStencilTestEnable = (PFN_vkCmdSetStencilTestEnable) gpa(obj, "vkCmdSetStencilTestEnableEXT");
    }
        table->CmdSetStencilOp = (PFN_vkCmdSetStencilOp) gpa(obj, "vkCmdSetStencilOp");
    if (table->CmdSetStencilOp == NULL) {
        table->CmdSetStencilOp = (PFN_vkCmdSetStencilOp) gpa(obj, "vkCmdSetStencilOpEXT");
    }
        table->CmdSetPatchControlPointsEXT = (PFN_vkCmdSetPatchControlPointsEXT) gpa(obj, "vkCmdSetPatchControlPointsEXT");
    table->CmdSetRasterizerDiscardEnable = (PFN_vkCmdSetRasterizerDiscardEnable) gpa(obj, "vkCmdSetRasterizerDiscardEnable");
    if (table->CmdSetRasterizerDiscardEnable == NULL) {
        table->CmdSetRasterizerDiscardEnable = (PFN_vkCmdSetRasterizerDiscardEnable) gpa(obj, "vkCmdSetRasterizerDiscardEnableEXT");
    }
        table->CmdSetDepthBiasEnable = (PFN_vkCmdSetDepthBiasEnable) gpa(obj, "vkCmdSetDepthBiasEnable");
    if (table->CmdSetDepthBiasEnable == NULL) {
        table->CmdSetDepthBiasEnable = (PFN_vkCmdSetDepthBiasEnable) gpa(obj, "vkCmdSetDepthBiasEnableEXT");
    }
        table->CmdSetLogicOpEXT = (PFN_vkCmdSetLogicOpEXT) gpa(obj, "vkCmdSetLogicOpEXT");
    table->CmdSetPrimitiveRestartEnable = (PFN_vkCmdSetPrimitiveRestartEnable) gpa(obj, "vkCmdSetPrimitiveRestartEnable");
    if (table->CmdSetPrimitiveRestartEnable == NULL) {
        table->CmdSetPrimitiveRestartEnable = (PFN_vkCmdSetPrimitiveRestartEnable) gpa(obj, "vkCmdSetPrimitiveRestartEnableEXT");
    }
        table->CmdSetTessellationDomainOriginEXT = (PFN_vkCmdSetTessellationDomainOriginEXT) gpa(obj, "vkCmdSetTessellationDomainOriginEXT");
    table->CmdSetDepthClampEnableEXT = (PFN_vkCmdSetDepthClampEnableEXT) gpa(obj, "vkCmdSetDepthClampEnableEXT");
    table->CmdSetPolygonModeEXT = (PFN_vkCmdSetPolygonModeEXT) gpa(obj, "vkCmdSetPolygonModeEXT");
    table->CmdSetRasterizationSamplesEXT = (PFN_vkCmdSetRasterizationSamplesEXT) gpa(obj, "vkCmdSetRasterizationSamplesEXT");
    table->CmdSetSampleMaskEXT = (PFN_vkCmdSetSampleMaskEXT) gpa(obj, "vkCmdSetSampleMaskEXT");
    table->CmdSetAlphaToCoverageEnableEXT = (PFN_vkCmdSetAlphaToCoverageEnableEXT) gpa(obj, "vkCmdSetAlphaToCoverageEnableEXT");
    table->CmdSetAlphaToOneEnableEXT = (PFN_vkCmdSetAlphaToOneEnableEXT) gpa(obj, "vkCmdSetAlphaToOneEnableEXT");
    table->CmdSetLogicOpEnableEXT = (PFN_vkCmdSetLogicOpEnableEXT) gpa(obj, "vkCmdSetLogicOpEnableEXT");
    table->CmdSetColorBlendEnableEXT = (PFN_vkCmdSetColorBlendEnableEXT) gpa(obj, "vkCmdSetColorBlendEnableEXT");
    table->CmdSetColorBlendEquationEXT = (PFN_vkCmdSetColorBlendEquationEXT) gpa(obj, "vkCmdSetColorBlendEquationEXT");
    table->CmdSetColorWriteMaskEXT = (PFN_vkCmdSetColorWriteMaskEXT) gpa(obj, "vkCmdSetColorWriteMaskEXT");
    table->CmdSetRasterizationStreamEXT = (PFN_vkCmdSetRasterizationStreamEXT) gpa(obj, "vkCmdSetRasterizationStreamEXT");
    table->CmdSetConservativeRasterizationModeEXT = (PFN_vkCmdSetConservativeRasterizationModeEXT) gpa(obj, "vkCmdSetConservativeRasterizationModeEXT");
    table->CmdSetExtraPrimitiveOverestimationSizeEXT = (PFN_vkCmdSetExtraPrimitiveOverestimationSizeEXT) gpa(obj, "vkCmdSetExtraPrimitiveOverestimationSizeEXT");
    table->CmdSetDepthClipEnableEXT = (PFN_vkCmdSetDepthClipEnableEXT) gpa(obj, "vkCmdSetDepthClipEnableEXT");
    table->CmdSetSampleLocationsEnableEXT = (PFN_vkCmdSetSampleLocationsEnableEXT) gpa(obj, "vkCmdSetSampleLocationsEnableEXT");
    table->CmdSetColorBlendAdvancedEXT = (PFN_vkCmdSetColorBlendAdvancedEXT) gpa(obj, "vkCmdSetColorBlendAdvancedEXT");
    table->CmdSetProvokingVertexModeEXT = (PFN_vkCmdSetProvokingVertexModeEXT) gpa(obj, "vkCmdSetProvokingVertexModeEXT");
    table->CmdSetLineRasterizationModeEXT = (PFN_vkCmdSetLineRasterizationModeEXT) gpa(obj, "vkCmdSetLineRasterizationModeEXT");
    table->CmdSetLineStippleEnableEXT = (PFN_vkCmdSetLineStippleEnableEXT) gpa(obj, "vkCmdSetLineStippleEnableEXT");
    table->CmdSetDepthClipNegativeOneToOneEXT = (PFN_vkCmdSetDepthClipNegativeOneToOneEXT) gpa(obj, "vkCmdSetDepthClipNegativeOneToOneEXT");
    table->CmdSetViewportWScalingEnableNV = (PFN_vkCmdSetViewportWScalingEnableNV) gpa(obj, "vkCmdSetViewportWScalingEnableNV");
    table->CmdSetViewportSwizzleNV = (PFN_vkCmdSetViewportSwizzleNV) gpa(obj, "vkCmdSetViewportSwizzleNV");
    table->CmdSetCoverageToColorEnableNV = (PFN_vkCmdSetCoverageToColorEnableNV) gpa(obj, "vkCmdSetCoverageToColorEnableNV");
    table->CmdSetCoverageToColorLocationNV = (PFN_vkCmdSetCoverageToColorLocationNV) gpa(obj, "vkCmdSetCoverageToColorLocationNV");
    table->CmdSetCoverageModulationModeNV = (PFN_vkCmdSetCoverageModulationModeNV) gpa(obj, "vkCmdSetCoverageModulationModeNV");
    table->CmdSetCoverageModulationTableEnableNV = (PFN_vkCmdSetCoverageModulationTableEnableNV) gpa(obj, "vkCmdSetCoverageModulationTableEnableNV");
    table->CmdSetCoverageModulationTableNV = (PFN_vkCmdSetCoverageModulationTableNV) gpa(obj, "vkCmdSetCoverageModulationTableNV");
    table->CmdSetShadingRateImageEnableNV = (PFN_vkCmdSetShadingRateImageEnableNV) gpa(obj, "vkCmdSetShadingRateImageEnableNV");
    table->CmdSetCoverageReductionModeNV = (PFN_vkCmdSetCoverageReductionModeNV) gpa(obj, "vkCmdSetCoverageReductionModeNV");
    table->CmdSetRepresentativeFragmentTestEnableNV = (PFN_vkCmdSetRepresentativeFragmentTestEnableNV) gpa(obj, "vkCmdSetRepresentativeFragmentTestEnableNV");
    table->CreatePrivateDataSlot = (PFN_vkCreatePrivateDataSlot) gpa(obj, "vkCreatePrivateDataSlot");
    if (table->CreatePrivateDataSlot == NULL) {
        table->CreatePrivateDataSlot = (PFN_vkCreatePrivateDataSlot) gpa(obj, "vkCreatePrivateDataSlotEXT");
    }
        table->DestroyPrivateDataSlot = (PFN_vkDestroyPrivateDataSlot) gpa(obj, "vkDestroyPrivateDataSlot");
    if (table->DestroyPrivateDataSlot == NULL) {
        table->DestroyPrivateDataSlot = (PFN_vkDestroyPrivateDataSlot) gpa(obj, "vkDestroyPrivateDataSlotEXT");
    }
        table->SetPrivateData = (PFN_vkSetPrivateData) gpa(obj, "vkSetPrivateData");
    if (table->SetPrivateData == NULL) {
        table->SetPrivateData = (PFN_vkSetPrivateData) gpa(obj, "vkSetPrivateDataEXT");
    }
        table->GetPrivateData = (PFN_vkGetPrivateData) gpa(obj, "vkGetPrivateData");
    if (table->GetPrivateData == NULL) {
        table->GetPrivateData = (PFN_vkGetPrivateData) gpa(obj, "vkGetPrivateDataEXT");
    }
        table->CmdCopyBuffer2 = (PFN_vkCmdCopyBuffer2) gpa(obj, "vkCmdCopyBuffer2");
    if (table->CmdCopyBuffer2 == NULL) {
        table->CmdCopyBuffer2 = (PFN_vkCmdCopyBuffer2) gpa(obj, "vkCmdCopyBuffer2KHR");
    }
        table->CmdCopyImage2 = (PFN_vkCmdCopyImage2) gpa(obj, "vkCmdCopyImage2");
    if (table->CmdCopyImage2 == NULL) {
        table->CmdCopyImage2 = (PFN_vkCmdCopyImage2) gpa(obj, "vkCmdCopyImage2KHR");
    }
        table->CmdBlitImage2 = (PFN_vkCmdBlitImage2) gpa(obj, "vkCmdBlitImage2");
    if (table->CmdBlitImage2 == NULL) {
        table->CmdBlitImage2 = (PFN_vkCmdBlitImage2) gpa(obj, "vkCmdBlitImage2KHR");
    }
        table->CmdCopyBufferToImage2 = (PFN_vkCmdCopyBufferToImage2) gpa(obj, "vkCmdCopyBufferToImage2");
    if (table->CmdCopyBufferToImage2 == NULL) {
        table->CmdCopyBufferToImage2 = (PFN_vkCmdCopyBufferToImage2) gpa(obj, "vkCmdCopyBufferToImage2KHR");
    }
        table->CmdCopyImageToBuffer2 = (PFN_vkCmdCopyImageToBuffer2) gpa(obj, "vkCmdCopyImageToBuffer2");
    if (table->CmdCopyImageToBuffer2 == NULL) {
        table->CmdCopyImageToBuffer2 = (PFN_vkCmdCopyImageToBuffer2) gpa(obj, "vkCmdCopyImageToBuffer2KHR");
    }
        table->CmdResolveImage2 = (PFN_vkCmdResolveImage2) gpa(obj, "vkCmdResolveImage2");
    if (table->CmdResolveImage2 == NULL) {
        table->CmdResolveImage2 = (PFN_vkCmdResolveImage2) gpa(obj, "vkCmdResolveImage2KHR");
    }
        table->CmdSetFragmentShadingRateKHR = (PFN_vkCmdSetFragmentShadingRateKHR) gpa(obj, "vkCmdSetFragmentShadingRateKHR");
    table->CmdSetFragmentShadingRateEnumNV = (PFN_vkCmdSetFragmentShadingRateEnumNV) gpa(obj, "vkCmdSetFragmentShadingRateEnumNV");
    table->GetAccelerationStructureBuildSizesKHR = (PFN_vkGetAccelerationStructureBuildSizesKHR) gpa(obj, "vkGetAccelerationStructureBuildSizesKHR");
    table->CmdSetVertexInputEXT = (PFN_vkCmdSetVertexInputEXT) gpa(obj, "vkCmdSetVertexInputEXT");
    table->CmdSetColorWriteEnableEXT = (PFN_vkCmdSetColorWriteEnableEXT) gpa(obj, "vkCmdSetColorWriteEnableEXT");
    table->CmdSetEvent2 = (PFN_vkCmdSetEvent2) gpa(obj, "vkCmdSetEvent2");
    if (table->CmdSetEvent2 == NULL) {
        table->CmdSetEvent2 = (PFN_vkCmdSetEvent2) gpa(obj, "vkCmdSetEvent2KHR");
    }
        table->CmdResetEvent2 = (PFN_vkCmdResetEvent2) gpa(obj, "vkCmdResetEvent2");
    if (table->CmdResetEvent2 == NULL) {
        table->CmdResetEvent2 = (PFN_vkCmdResetEvent2) gpa(obj, "vkCmdResetEvent2KHR");
    }
        table->CmdWaitEvents2 = (PFN_vkCmdWaitEvents2) gpa(obj, "vkCmdWaitEvents2");
    if (table->CmdWaitEvents2 == NULL) {
        table->CmdWaitEvents2 = (PFN_vkCmdWaitEvents2) gpa(obj, "vkCmdWaitEvents2KHR");
    }
        table->CmdPipelineBarrier2 = (PFN_vkCmdPipelineBarrier2) gpa(obj, "vkCmdPipelineBarrier2");
    if (table->CmdPipelineBarrier2 == NULL) {
        table->CmdPipelineBarrier2 = (PFN_vkCmdPipelineBarrier2) gpa(obj, "vkCmdPipelineBarrier2KHR");
    }
        table->QueueSubmit2 = (PFN_vkQueueSubmit2) gpa(obj, "vkQueueSubmit2");
    if (table->QueueSubmit2 == NULL) {
        table->QueueSubmit2 = (PFN_vkQueueSubmit2) gpa(obj, "vkQueueSubmit2KHR");
    }
        table->CmdWriteTimestamp2 = (PFN_vkCmdWriteTimestamp2) gpa(obj, "vkCmdWriteTimestamp2");
    if (table->CmdWriteTimestamp2 == NULL) {
        table->CmdWriteTimestamp2 = (PFN_vkCmdWriteTimestamp2) gpa(obj, "vkCmdWriteTimestamp2KHR");
    }
        table->CmdWriteBufferMarker2AMD = (PFN_vkCmdWriteBufferMarker2AMD) gpa(obj, "vkCmdWriteBufferMarker2AMD");
    table->GetQueueCheckpointData2NV = (PFN_vkGetQueueCheckpointData2NV) gpa(obj, "vkGetQueueCheckpointData2NV");
    table->CopyMemoryToImageEXT = (PFN_vkCopyMemoryToImageEXT) gpa(obj, "vkCopyMemoryToImageEXT");
    table->CopyImageToMemoryEXT = (PFN_vkCopyImageToMemoryEXT) gpa(obj, "vkCopyImageToMemoryEXT");
    table->CopyImageToImageEXT = (PFN_vkCopyImageToImageEXT) gpa(obj, "vkCopyImageToImageEXT");
    table->TransitionImageLayoutEXT = (PFN_vkTransitionImageLayoutEXT) gpa(obj, "vkTransitionImageLayoutEXT");
    table->CreateVideoSessionKHR = (PFN_vkCreateVideoSessionKHR) gpa(obj, "vkCreateVideoSessionKHR");
    table->DestroyVideoSessionKHR = (PFN_vkDestroyVideoSessionKHR) gpa(obj, "vkDestroyVideoSessionKHR");
    table->CreateVideoSessionParametersKHR = (PFN_vkCreateVideoSessionParametersKHR) gpa(obj, "vkCreateVideoSessionParametersKHR");
    table->UpdateVideoSessionParametersKHR = (PFN_vkUpdateVideoSessionParametersKHR) gpa(obj, "vkUpdateVideoSessionParametersKHR");
    table->DestroyVideoSessionParametersKHR = (PFN_vkDestroyVideoSessionParametersKHR) gpa(obj, "vkDestroyVideoSessionParametersKHR");
    table->GetVideoSessionMemoryRequirementsKHR = (PFN_vkGetVideoSessionMemoryRequirementsKHR) gpa(obj, "vkGetVideoSessionMemoryRequirementsKHR");
    table->BindVideoSessionMemoryKHR = (PFN_vkBindVideoSessionMemoryKHR) gpa(obj, "vkBindVideoSessionMemoryKHR");
    table->CmdDecodeVideoKHR = (PFN_vkCmdDecodeVideoKHR) gpa(obj, "vkCmdDecodeVideoKHR");
    table->CmdBeginVideoCodingKHR = (PFN_vkCmdBeginVideoCodingKHR) gpa(obj, "vkCmdBeginVideoCodingKHR");
    table->CmdControlVideoCodingKHR = (PFN_vkCmdControlVideoCodingKHR) gpa(obj, "vkCmdControlVideoCodingKHR");
    table->CmdEndVideoCodingKHR = (PFN_vkCmdEndVideoCodingKHR) gpa(obj, "vkCmdEndVideoCodingKHR");
    table->CmdDecompressMemoryNV = (PFN_vkCmdDecompressMemoryNV) gpa(obj, "vkCmdDecompressMemoryNV");
    table->CmdDecompressMemoryIndirectCountNV = (PFN_vkCmdDecompressMemoryIndirectCountNV) gpa(obj, "vkCmdDecompressMemoryIndirectCountNV");
    table->CreateCuModuleNVX = (PFN_vkCreateCuModuleNVX) gpa(obj, "vkCreateCuModuleNVX");
    table->CreateCuFunctionNVX = (PFN_vkCreateCuFunctionNVX) gpa(obj, "vkCreateCuFunctionNVX");
    table->DestroyCuModuleNVX = (PFN_vkDestroyCuModuleNVX) gpa(obj, "vkDestroyCuModuleNVX");
    table->DestroyCuFunctionNVX = (PFN_vkDestroyCuFunctionNVX) gpa(obj, "vkDestroyCuFunctionNVX");
    table->CmdCuLaunchKernelNVX = (PFN_vkCmdCuLaunchKernelNVX) gpa(obj, "vkCmdCuLaunchKernelNVX");
    table->GetDescriptorSetLayoutSizeEXT = (PFN_vkGetDescriptorSetLayoutSizeEXT) gpa(obj, "vkGetDescriptorSetLayoutSizeEXT");
    table->GetDescriptorSetLayoutBindingOffsetEXT = (PFN_vkGetDescriptorSetLayoutBindingOffsetEXT) gpa(obj, "vkGetDescriptorSetLayoutBindingOffsetEXT");
    table->GetDescriptorEXT = (PFN_vkGetDescriptorEXT) gpa(obj, "vkGetDescriptorEXT");
    table->CmdBindDescriptorBuffersEXT = (PFN_vkCmdBindDescriptorBuffersEXT) gpa(obj, "vkCmdBindDescriptorBuffersEXT");
    table->CmdSetDescriptorBufferOffsetsEXT = (PFN_vkCmdSetDescriptorBufferOffsetsEXT) gpa(obj, "vkCmdSetDescriptorBufferOffsetsEXT");
    table->CmdBindDescriptorBufferEmbeddedSamplersEXT = (PFN_vkCmdBindDescriptorBufferEmbeddedSamplersEXT) gpa(obj, "vkCmdBindDescriptorBufferEmbeddedSamplersEXT");
    table->GetBufferOpaqueCaptureDescriptorDataEXT = (PFN_vkGetBufferOpaqueCaptureDescriptorDataEXT) gpa(obj, "vkGetBufferOpaqueCaptureDescriptorDataEXT");
    table->GetImageOpaqueCaptureDescriptorDataEXT = (PFN_vkGetImageOpaqueCaptureDescriptorDataEXT) gpa(obj, "vkGetImageOpaqueCaptureDescriptorDataEXT");
    table->GetImageViewOpaqueCaptureDescriptorDataEXT = (PFN_vkGetImageViewOpaqueCaptureDescriptorDataEXT) gpa(obj, "vkGetImageViewOpaqueCaptureDescriptorDataEXT");
    table->GetSamplerOpaqueCaptureDescriptorDataEXT = (PFN_vkGetSamplerOpaqueCaptureDescriptorDataEXT) gpa(obj, "vkGetSamplerOpaqueCaptureDescriptorDataEXT");
    table->GetAccelerationStructureOpaqueCaptureDescriptorDataEXT = (PFN_vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT) gpa(obj, "vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT");
    table->SetDeviceMemoryPriorityEXT = (PFN_vkSetDeviceMemoryPriorityEXT) gpa(obj, "vkSetDeviceMemoryPriorityEXT");
    table->WaitForPresentKHR = (PFN_vkWaitForPresentKHR) gpa(obj, "vkWaitForPresentKHR");
#ifdef VK_USE_PLATFORM_FUCHSIA
    table->CreateBufferCollectionFUCHSIA = (PFN_vkCreateBufferCollectionFUCHSIA) gpa(obj, "vkCreateBufferCollectionFUCHSIA");
#endif
#ifdef VK_USE_PLATFORM_FUCHSIA
    table->SetBufferCollectionBufferConstraintsFUCHSIA = (PFN_vkSetBufferCollectionBufferConstraintsFUCHSIA) gpa(obj, "vkSetBufferCollectionBufferConstraintsFUCHSIA");
#endif
#ifdef VK_USE_PLATFORM_FUCHSIA
    table->SetBufferCollectionImageConstraintsFUCHSIA = (PFN_vkSetBufferCollectionImageConstraintsFUCHSIA) gpa(obj, "vkSetBufferCollectionImageConstraintsFUCHSIA");
#endif
#ifdef VK_USE_PLATFORM_FUCHSIA
    table->DestroyBufferCollectionFUCHSIA = (PFN_vkDestroyBufferCollectionFUCHSIA) gpa(obj, "vkDestroyBufferCollectionFUCHSIA");
#endif
#ifdef VK_USE_PLATFORM_FUCHSIA
    table->GetBufferCollectionPropertiesFUCHSIA = (PFN_vkGetBufferCollectionPropertiesFUCHSIA) gpa(obj, "vkGetBufferCollectionPropertiesFUCHSIA");
#endif
    table->CmdBeginRendering = (PFN_vkCmdBeginRendering) gpa(obj, "vkCmdBeginRendering");
    if (table->CmdBeginRendering == NULL) {
        table->CmdBeginRendering = (PFN_vkCmdBeginRendering) gpa(obj, "vkCmdBeginRenderingKHR");
    }
        table->CmdEndRendering = (PFN_vkCmdEndRendering) gpa(obj, "vkCmdEndRendering");
    if (table->CmdEndRendering == NULL) {
        table->CmdEndRendering = (PFN_vkCmdEndRendering) gpa(obj, "vkCmdEndRenderingKHR");
    }
        table->GetDescriptorSetLayoutHostMappingInfoVALVE = (PFN_vkGetDescriptorSetLayoutHostMappingInfoVALVE) gpa(obj, "vkGetDescriptorSetLayoutHostMappingInfoVALVE");
    table->GetDescriptorSetHostMappingVALVE = (PFN_vkGetDescriptorSetHostMappingVALVE) gpa(obj, "vkGetDescriptorSetHostMappingVALVE");
    table->CreateMicromapEXT = (PFN_vkCreateMicromapEXT) gpa(obj, "vkCreateMicromapEXT");
    table->CmdBuildMicromapsEXT = (PFN_vkCmdBuildMicromapsEXT) gpa(obj, "vkCmdBuildMicromapsEXT");
    table->BuildMicromapsEXT = (PFN_vkBuildMicromapsEXT) gpa(obj, "vkBuildMicromapsEXT");
    table->DestroyMicromapEXT = (PFN_vkDestroyMicromapEXT) gpa(obj, "vkDestroyMicromapEXT");
    table->CmdCopyMicromapEXT = (PFN_vkCmdCopyMicromapEXT) gpa(obj, "vkCmdCopyMicromapEXT");
    table->CopyMicromapEXT = (PFN_vkCopyMicromapEXT) gpa(obj, "vkCopyMicromapEXT");
    table->CmdCopyMicromapToMemoryEXT = (PFN_vkCmdCopyMicromapToMemoryEXT) gpa(obj, "vkCmdCopyMicromapToMemoryEXT");
    table->CopyMicromapToMemoryEXT = (PFN_vkCopyMicromapToMemoryEXT) gpa(obj, "vkCopyMicromapToMemoryEXT");
    table->CmdCopyMemoryToMicromapEXT = (PFN_vkCmdCopyMemoryToMicromapEXT) gpa(obj, "vkCmdCopyMemoryToMicromapEXT");
    table->CopyMemoryToMicromapEXT = (PFN_vkCopyMemoryToMicromapEXT) gpa(obj, "vkCopyMemoryToMicromapEXT");
    table->CmdWriteMicromapsPropertiesEXT = (PFN_vkCmdWriteMicromapsPropertiesEXT) gpa(obj, "vkCmdWriteMicromapsPropertiesEXT");
    table->WriteMicromapsPropertiesEXT = (PFN_vkWriteMicromapsPropertiesEXT) gpa(obj, "vkWriteMicromapsPropertiesEXT");
    table->GetDeviceMicromapCompatibilityEXT = (PFN_vkGetDeviceMicromapCompatibilityEXT) gpa(obj, "vkGetDeviceMicromapCompatibilityEXT");
    table->GetMicromapBuildSizesEXT = (PFN_vkGetMicromapBuildSizesEXT) gpa(obj, "vkGetMicromapBuildSizesEXT");
    table->GetShaderModuleIdentifierEXT = (PFN_vkGetShaderModuleIdentifierEXT) gpa(obj, "vkGetShaderModuleIdentifierEXT");
    table->GetShaderModuleCreateInfoIdentifierEXT = (PFN_vkGetShaderModuleCreateInfoIdentifierEXT) gpa(obj, "vkGetShaderModuleCreateInfoIdentifierEXT");
    table->GetImageSubresourceLayout2KHR = (PFN_vkGetImageSubresourceLayout2KHR) gpa(obj, "vkGetImageSubresourceLayout2KHR");
    if (table->GetImageSubresourceLayout2KHR == NULL) {
        table->GetImageSubresourceLayout2KHR = (PFN_vkGetImageSubresourceLayout2KHR) gpa(obj, "vkGetImageSubresourceLayout2EXT");
    }
        table->GetPipelinePropertiesEXT = (PFN_vkGetPipelinePropertiesEXT) gpa(obj, "vkGetPipelinePropertiesEXT");
#ifdef VK_USE_PLATFORM_METAL_EXT
    table->ExportMetalObjectsEXT = (PFN_vkExportMetalObjectsEXT) gpa(obj, "vkExportMetalObjectsEXT");
#endif
    table->GetFramebufferTilePropertiesQCOM = (PFN_vkGetFramebufferTilePropertiesQCOM) gpa(obj, "vkGetFramebufferTilePropertiesQCOM");
    table->GetDynamicRenderingTilePropertiesQCOM = (PFN_vkGetDynamicRenderingTilePropertiesQCOM) gpa(obj, "vkGetDynamicRenderingTilePropertiesQCOM");
    table->CreateOpticalFlowSessionNV = (PFN_vkCreateOpticalFlowSessionNV) gpa(obj, "vkCreateOpticalFlowSessionNV");
    table->DestroyOpticalFlowSessionNV = (PFN_vkDestroyOpticalFlowSessionNV) gpa(obj, "vkDestroyOpticalFlowSessionNV");
    table->BindOpticalFlowSessionImageNV = (PFN_vkBindOpticalFlowSessionImageNV) gpa(obj, "vkBindOpticalFlowSessionImageNV");
    table->CmdOpticalFlowExecuteNV = (PFN_vkCmdOpticalFlowExecuteNV) gpa(obj, "vkCmdOpticalFlowExecuteNV");
    table->GetDeviceFaultInfoEXT = (PFN_vkGetDeviceFaultInfoEXT) gpa(obj, "vkGetDeviceFaultInfoEXT");
    table->CmdSetDepthBias2EXT = (PFN_vkCmdSetDepthBias2EXT) gpa(obj, "vkCmdSetDepthBias2EXT");
    table->ReleaseSwapchainImagesEXT = (PFN_vkReleaseSwapchainImagesEXT) gpa(obj, "vkReleaseSwapchainImagesEXT");
    table->GetDeviceImageSubresourceLayoutKHR = (PFN_vkGetDeviceImageSubresourceLayoutKHR) gpa(obj, "vkGetDeviceImageSubresourceLayoutKHR");
    table->MapMemory2KHR = (PFN_vkMapMemory2KHR) gpa(obj, "vkMapMemory2KHR");
    table->UnmapMemory2KHR = (PFN_vkUnmapMemory2KHR) gpa(obj, "vkUnmapMemory2KHR");
    table->CreateShadersEXT = (PFN_vkCreateShadersEXT) gpa(obj, "vkCreateShadersEXT");
    table->DestroyShaderEXT = (PFN_vkDestroyShaderEXT) gpa(obj, "vkDestroyShaderEXT");
    table->GetShaderBinaryDataEXT = (PFN_vkGetShaderBinaryDataEXT) gpa(obj, "vkGetShaderBinaryDataEXT");
    table->CmdBindShadersEXT = (PFN_vkCmdBindShadersEXT) gpa(obj, "vkCmdBindShadersEXT");
#ifdef VK_USE_PLATFORM_SCREEN_QNX
    table->GetScreenBufferPropertiesQNX = (PFN_vkGetScreenBufferPropertiesQNX) gpa(obj, "vkGetScreenBufferPropertiesQNX");
#endif
    table->SetLatencySleepModeNV = (PFN_vkSetLatencySleepModeNV) gpa(obj, "vkSetLatencySleepModeNV");
    table->LatencySleepNV = (PFN_vkLatencySleepNV) gpa(obj, "vkLatencySleepNV");
    table->SetLatencyMarkerNV = (PFN_vkSetLatencyMarkerNV) gpa(obj, "vkSetLatencyMarkerNV");
    table->GetLatencyTimingsNV = (PFN_vkGetLatencyTimingsNV) gpa(obj, "vkGetLatencyTimingsNV");
    table->QueueNotifyOutOfBandNV = (PFN_vkQueueNotifyOutOfBandNV) gpa(obj, "vkQueueNotifyOutOfBandNV");
}



struct string_map_entry {
   uint32_t name;
   uint32_t hash;
   uint32_t num;
};

/* We use a big string constant to avoid lots of reloctions from the entry
 * point table to lots of little strings. The entries in the entry point table
 * store the index into this big string.
 */




static const char instance_strings[] =
    "vkCreateAndroidSurfaceKHR\0"
    "vkCreateDebugReportCallbackEXT\0"
    "vkCreateDebugUtilsMessengerEXT\0"
    "vkCreateDirectFBSurfaceEXT\0"
    "vkCreateDisplayPlaneSurfaceKHR\0"
    "vkCreateHeadlessSurfaceEXT\0"
    "vkCreateIOSSurfaceMVK\0"
    "vkCreateImagePipeSurfaceFUCHSIA\0"
    "vkCreateInstance\0"
    "vkCreateMacOSSurfaceMVK\0"
    "vkCreateMetalSurfaceEXT\0"
    "vkCreateScreenSurfaceQNX\0"
    "vkCreateStreamDescriptorSurfaceGGP\0"
    "vkCreateViSurfaceNN\0"
    "vkCreateWaylandSurfaceKHR\0"
    "vkCreateWin32SurfaceKHR\0"
    "vkCreateXcbSurfaceKHR\0"
    "vkCreateXlibSurfaceKHR\0"
    "vkDebugReportMessageEXT\0"
    "vkDestroyDebugReportCallbackEXT\0"
    "vkDestroyDebugUtilsMessengerEXT\0"
    "vkDestroyInstance\0"
    "vkDestroySurfaceKHR\0"
    "vkEnumerateInstanceExtensionProperties\0"
    "vkEnumerateInstanceLayerProperties\0"
    "vkEnumerateInstanceVersion\0"
    "vkEnumeratePhysicalDeviceGroups\0"
    "vkEnumeratePhysicalDeviceGroupsKHR\0"
    "vkEnumeratePhysicalDevices\0"
    "vkGetInstanceProcAddr\0"
    "vkSubmitDebugUtilsMessageEXT\0"
;

static const struct string_map_entry instance_string_map_entries[] = {
    { 0, 0x3667f4e, 7 }, /* vkCreateAndroidSurfaceKHR */
    { 26, 0x987ef56, 19 }, /* vkCreateDebugReportCallbackEXT */
    { 57, 0xb674c135, 27 }, /* vkCreateDebugUtilsMessengerEXT */
    { 88, 0x2aff82da, 15 }, /* vkCreateDirectFBSurfaceEXT */
    { 115, 0x7ac4dacb, 8 }, /* vkCreateDisplayPlaneSurfaceKHR */
    { 146, 0x475d2386, 30 }, /* vkCreateHeadlessSurfaceEXT */
    { 173, 0x12f99669, 24 }, /* vkCreateIOSSurfaceMVK */
    { 195, 0xdee614a2, 16 }, /* vkCreateImagePipeSurfaceFUCHSIA */
    { 227, 0x38a581a6, 0 }, /* vkCreateInstance */
    { 244, 0x30feffa7, 25 }, /* vkCreateMacOSSurfaceMVK */
    { 268, 0xa436224a, 26 }, /* vkCreateMetalSurfaceEXT */
    { 292, 0x6338a483, 18 }, /* vkCreateScreenSurfaceQNX */
    { 317, 0x14361e93, 17 }, /* vkCreateStreamDescriptorSurfaceGGP */
    { 352, 0xba0e6b39, 10 }, /* vkCreateViSurfaceNN */
    { 372, 0x2b2a4b79, 11 }, /* vkCreateWaylandSurfaceKHR */
    { 398, 0xfa2ba1e2, 12 }, /* vkCreateWin32SurfaceKHR */
    { 422, 0xc5e5b106, 14 }, /* vkCreateXcbSurfaceKHR */
    { 444, 0xa693bc66, 13 }, /* vkCreateXlibSurfaceKHR */
    { 467, 0xa4e75334, 21 }, /* vkDebugReportMessageEXT */
    { 491, 0x43d4c4e2, 20 }, /* vkDestroyDebugReportCallbackEXT */
    { 523, 0xf0c196c1, 28 }, /* vkDestroyDebugUtilsMessengerEXT */
    { 555, 0x9bd21af2, 1 }, /* vkDestroyInstance */
    { 573, 0xf204ce7d, 9 }, /* vkDestroySurfaceKHR */
    { 593, 0xeb27627e, 6 }, /* vkEnumerateInstanceExtensionProperties */
    { 632, 0x81f69d8, 5 }, /* vkEnumerateInstanceLayerProperties */
    { 667, 0xd0481e5c, 4 }, /* vkEnumerateInstanceVersion */
    { 694, 0x270514f0, 22 }, /* vkEnumeratePhysicalDeviceGroups */
    { 726, 0x549ce595, 23 }, /* vkEnumeratePhysicalDeviceGroupsKHR */
    { 761, 0x5787c327, 2 }, /* vkEnumeratePhysicalDevices */
    { 788, 0x3d2ae9ad, 3 }, /* vkGetInstanceProcAddr */
    { 810, 0x92943f0d, 29 }, /* vkSubmitDebugUtilsMessageEXT */
};

/* Hash table stats:
 * size 31 entries
 * collisions entries:
 *     0      25
 *     1      3
 *     2      1
 *     3      2
 *     4      0
 *     5      0
 *     6      0
 *     7      0
 *     8      0
 *     9+     0
 */

#define none 0xffff
static const uint16_t instance_string_map[64] = {
    none,
    0x0014,
    none,
    0x000b,
    none,
    none,
    0x0005,
    none,
    0x000f,
    none,
    0x000a,
    0x0004,
    0x000e,
    0x001e,
    0x0000,
    none,
    none,
    none,
    none,
    0x000c,
    none,
    0x001b,
    0x0001,
    none,
    0x0018,
    0x0010,
    0x0003,
    0x0013,
    0x0019,
    none,
    none,
    0x0011,
    none,
    none,
    0x0007,
    none,
    none,
    none,
    0x0008,
    0x0009,
    none,
    0x0006,
    none,
    none,
    none,
    0x001d,
    none,
    none,
    0x001a,
    none,
    0x0015,
    none,
    0x0012,
    0x0002,
    none,
    none,
    none,
    0x000d,
    0x001c,
    none,
    none,
    0x0016,
    0x0017,
    none,
};

static int
instance_string_map_lookup(const char *str)
{
    static const uint32_t prime_factor = 5024183;
    static const uint32_t prime_step = 19;
    const struct string_map_entry *e;
    uint32_t hash, h;
    uint16_t i;
    const char *p;

    hash = 0;
    for (p = str; *p; p++)
        hash = hash * prime_factor + *p;

    h = hash;
    while (1) {
        i = instance_string_map[h & 63];
        if (i == none)
           return -1;
        e = &instance_string_map_entries[i];
        if (e->hash == hash && strcmp(str, instance_strings + e->name) == 0)
            return e->num;
        h += prime_step;
    }

    return -1;
}


static const char physical_device_strings[] =
    "vkAcquireDrmDisplayEXT\0"
    "vkAcquireWinrtDisplayNV\0"
    "vkAcquireXlibDisplayEXT\0"
    "vkCreateDevice\0"
    "vkCreateDisplayModeKHR\0"
    "vkEnumerateDeviceExtensionProperties\0"
    "vkEnumerateDeviceLayerProperties\0"
    "vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR\0"
    "vkGetDisplayModeProperties2KHR\0"
    "vkGetDisplayModePropertiesKHR\0"
    "vkGetDisplayPlaneCapabilities2KHR\0"
    "vkGetDisplayPlaneCapabilitiesKHR\0"
    "vkGetDisplayPlaneSupportedDisplaysKHR\0"
    "vkGetDrmDisplayEXT\0"
    "vkGetPhysicalDeviceCalibrateableTimeDomainsEXT\0"
    "vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR\0"
    "vkGetPhysicalDeviceCooperativeMatrixPropertiesNV\0"
    "vkGetPhysicalDeviceDirectFBPresentationSupportEXT\0"
    "vkGetPhysicalDeviceDisplayPlaneProperties2KHR\0"
    "vkGetPhysicalDeviceDisplayPlanePropertiesKHR\0"
    "vkGetPhysicalDeviceDisplayProperties2KHR\0"
    "vkGetPhysicalDeviceDisplayPropertiesKHR\0"
    "vkGetPhysicalDeviceExternalBufferProperties\0"
    "vkGetPhysicalDeviceExternalBufferPropertiesKHR\0"
    "vkGetPhysicalDeviceExternalFenceProperties\0"
    "vkGetPhysicalDeviceExternalFencePropertiesKHR\0"
    "vkGetPhysicalDeviceExternalImageFormatPropertiesNV\0"
    "vkGetPhysicalDeviceExternalSemaphoreProperties\0"
    "vkGetPhysicalDeviceExternalSemaphorePropertiesKHR\0"
    "vkGetPhysicalDeviceFeatures\0"
    "vkGetPhysicalDeviceFeatures2\0"
    "vkGetPhysicalDeviceFeatures2KHR\0"
    "vkGetPhysicalDeviceFormatProperties\0"
    "vkGetPhysicalDeviceFormatProperties2\0"
    "vkGetPhysicalDeviceFormatProperties2KHR\0"
    "vkGetPhysicalDeviceFragmentShadingRatesKHR\0"
    "vkGetPhysicalDeviceImageFormatProperties\0"
    "vkGetPhysicalDeviceImageFormatProperties2\0"
    "vkGetPhysicalDeviceImageFormatProperties2KHR\0"
    "vkGetPhysicalDeviceMemoryProperties\0"
    "vkGetPhysicalDeviceMemoryProperties2\0"
    "vkGetPhysicalDeviceMemoryProperties2KHR\0"
    "vkGetPhysicalDeviceMultisamplePropertiesEXT\0"
    "vkGetPhysicalDeviceOpticalFlowImageFormatsNV\0"
    "vkGetPhysicalDevicePresentRectanglesKHR\0"
    "vkGetPhysicalDeviceProperties\0"
    "vkGetPhysicalDeviceProperties2\0"
    "vkGetPhysicalDeviceProperties2KHR\0"
    "vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR\0"
    "vkGetPhysicalDeviceQueueFamilyProperties\0"
    "vkGetPhysicalDeviceQueueFamilyProperties2\0"
    "vkGetPhysicalDeviceQueueFamilyProperties2KHR\0"
    "vkGetPhysicalDeviceScreenPresentationSupportQNX\0"
    "vkGetPhysicalDeviceSparseImageFormatProperties\0"
    "vkGetPhysicalDeviceSparseImageFormatProperties2\0"
    "vkGetPhysicalDeviceSparseImageFormatProperties2KHR\0"
    "vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV\0"
    "vkGetPhysicalDeviceSurfaceCapabilities2EXT\0"
    "vkGetPhysicalDeviceSurfaceCapabilities2KHR\0"
    "vkGetPhysicalDeviceSurfaceCapabilitiesKHR\0"
    "vkGetPhysicalDeviceSurfaceFormats2KHR\0"
    "vkGetPhysicalDeviceSurfaceFormatsKHR\0"
    "vkGetPhysicalDeviceSurfacePresentModes2EXT\0"
    "vkGetPhysicalDeviceSurfacePresentModesKHR\0"
    "vkGetPhysicalDeviceSurfaceSupportKHR\0"
    "vkGetPhysicalDeviceToolProperties\0"
    "vkGetPhysicalDeviceToolPropertiesEXT\0"
    "vkGetPhysicalDeviceVideoCapabilitiesKHR\0"
    "vkGetPhysicalDeviceVideoFormatPropertiesKHR\0"
    "vkGetPhysicalDeviceWaylandPresentationSupportKHR\0"
    "vkGetPhysicalDeviceWin32PresentationSupportKHR\0"
    "vkGetPhysicalDeviceXcbPresentationSupportKHR\0"
    "vkGetPhysicalDeviceXlibPresentationSupportKHR\0"
    "vkGetRandROutputDisplayEXT\0"
    "vkGetWinrtDisplayNV\0"
    "vkReleaseDisplayEXT\0"
;

static const struct string_map_entry physical_device_string_map_entries[] = {
    { 0, 0x837a3e0d, 72 }, /* vkAcquireDrmDisplayEXT */
    { 23, 0xdfb5cfcd, 50 }, /* vkAcquireWinrtDisplayNV */
    { 47, 0x60df100d, 48 }, /* vkAcquireXlibDisplayEXT */
    { 71, 0x85ed23f, 6 }, /* vkCreateDevice */
    { 86, 0xcc0bde41, 14 }, /* vkCreateDisplayModeKHR */
    { 109, 0x5fd13eed, 8 }, /* vkEnumerateDeviceExtensionProperties */
    { 146, 0x2f8566e7, 7 }, /* vkEnumerateDeviceLayerProperties */
    { 179, 0x8d3d4995, 64 }, /* vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR */
    { 243, 0x3e613e42, 59 }, /* vkGetDisplayModeProperties2KHR */
    { 274, 0x36b8a8de, 13 }, /* vkGetDisplayModePropertiesKHR */
    { 304, 0xff1655a4, 60 }, /* vkGetDisplayPlaneCapabilities2KHR */
    { 338, 0x4b60d48c, 15 }, /* vkGetDisplayPlaneCapabilitiesKHR */
    { 371, 0xabef4889, 12 }, /* vkGetDisplayPlaneSupportedDisplaysKHR */
    { 409, 0x35c4e65, 73 }, /* vkGetDrmDisplayEXT */
    { 428, 0xea07da1a, 61 }, /* vkGetPhysicalDeviceCalibrateableTimeDomainsEXT */
    { 475, 0xb948444c, 75 }, /* vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR */
    { 525, 0x3c5ac109, 62 }, /* vkGetPhysicalDeviceCooperativeMatrixPropertiesNV */
    { 574, 0x8d00453f, 24 }, /* vkGetPhysicalDeviceDirectFBPresentationSupportEXT */
    { 624, 0xb7bc4386, 58 }, /* vkGetPhysicalDeviceDisplayPlaneProperties2KHR */
    { 670, 0xb9b8ddba, 11 }, /* vkGetPhysicalDeviceDisplayPlanePropertiesKHR */
    { 715, 0x540c0372, 57 }, /* vkGetPhysicalDeviceDisplayProperties2KHR */
    { 756, 0xfa0cd2e, 10 }, /* vkGetPhysicalDeviceDisplayPropertiesKHR */
    { 796, 0x944476dc, 41 }, /* vkGetPhysicalDeviceExternalBufferProperties */
    { 840, 0xee68b389, 42 }, /* vkGetPhysicalDeviceExternalBufferPropertiesKHR */
    { 887, 0x3bc965eb, 45 }, /* vkGetPhysicalDeviceExternalFenceProperties */
    { 930, 0x99b35492, 46 }, /* vkGetPhysicalDeviceExternalFencePropertiesKHR */
    { 976, 0xc8420c4c, 26 }, /* vkGetPhysicalDeviceExternalImageFormatPropertiesNV */
    { 1027, 0xcf251b0e, 43 }, /* vkGetPhysicalDeviceExternalSemaphoreProperties */
    { 1074, 0x984c3fa7, 44 }, /* vkGetPhysicalDeviceExternalSemaphorePropertiesKHR */
    { 1124, 0x113e2f33, 3 }, /* vkGetPhysicalDeviceFeatures */
    { 1152, 0x63c068a7, 27 }, /* vkGetPhysicalDeviceFeatures2 */
    { 1181, 0x6a9a3636, 28 }, /* vkGetPhysicalDeviceFeatures2KHR */
    { 1213, 0x3e54b398, 4 }, /* vkGetPhysicalDeviceFormatProperties */
    { 1249, 0xca3bb9da, 31 }, /* vkGetPhysicalDeviceFormatProperties2 */
    { 1286, 0x9099cbbb, 32 }, /* vkGetPhysicalDeviceFormatProperties2KHR */
    { 1326, 0x6f0a9ed6, 69 }, /* vkGetPhysicalDeviceFragmentShadingRatesKHR */
    { 1369, 0xdd36a867, 5 }, /* vkGetPhysicalDeviceImageFormatProperties */
    { 1410, 0x35d260d3, 33 }, /* vkGetPhysicalDeviceImageFormatProperties2 */
    { 1452, 0x102ff7ea, 34 }, /* vkGetPhysicalDeviceImageFormatProperties2KHR */
    { 1497, 0xa90da4da, 2 }, /* vkGetPhysicalDeviceMemoryProperties */
    { 1533, 0xcb4cc208, 37 }, /* vkGetPhysicalDeviceMemoryProperties2 */
    { 1570, 0xc8c3da3d, 38 }, /* vkGetPhysicalDeviceMemoryProperties2KHR */
    { 1610, 0x219aa0b9, 54 }, /* vkGetPhysicalDeviceMultisamplePropertiesEXT */
    { 1654, 0x521044d1, 74 }, /* vkGetPhysicalDeviceOpticalFlowImageFormatsNV */
    { 1699, 0x100341b4, 53 }, /* vkGetPhysicalDevicePresentRectanglesKHR */
    { 1739, 0x52fe22c9, 0 }, /* vkGetPhysicalDeviceProperties */
    { 1769, 0x6c4d8ee1, 29 }, /* vkGetPhysicalDeviceProperties2 */
    { 1800, 0xcd15838c, 30 }, /* vkGetPhysicalDeviceProperties2KHR */
    { 1834, 0x7c7c9a0f, 65 }, /* vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR */
    { 1890, 0x4e5fc88a, 1 }, /* vkGetPhysicalDeviceQueueFamilyProperties */
    { 1931, 0xcad374d8, 35 }, /* vkGetPhysicalDeviceQueueFamilyProperties2 */
    { 1973, 0x5ceb2bed, 36 }, /* vkGetPhysicalDeviceQueueFamilyProperties2KHR */
    { 2018, 0xb5c7dc78, 25 }, /* vkGetPhysicalDeviceScreenPresentationSupportQNX */
    { 2066, 0x272ef8ef, 9 }, /* vkGetPhysicalDeviceSparseImageFormatProperties */
    { 2113, 0xebddba0b, 39 }, /* vkGetPhysicalDeviceSparseImageFormatProperties2 */
    { 2161, 0x8746ed72, 40 }, /* vkGetPhysicalDeviceSparseImageFormatProperties2KHR */
    { 2212, 0x432ca8f7, 66 }, /* vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV */
    { 2278, 0x5a5fba04, 52 }, /* vkGetPhysicalDeviceSurfaceCapabilities2EXT */
    { 2321, 0x9497e378, 55 }, /* vkGetPhysicalDeviceSurfaceCapabilities2KHR */
    { 2364, 0x77890558, 17 }, /* vkGetPhysicalDeviceSurfaceCapabilitiesKHR */
    { 2406, 0xd00b7188, 56 }, /* vkGetPhysicalDeviceSurfaceFormats2KHR */
    { 2444, 0xe32227c8, 18 }, /* vkGetPhysicalDeviceSurfaceFormatsKHR */
    { 2481, 0x33557b83, 63 }, /* vkGetPhysicalDeviceSurfacePresentModes2EXT */
    { 2524, 0x31c3cbd1, 19 }, /* vkGetPhysicalDeviceSurfacePresentModesKHR */
    { 2566, 0x1a687885, 16 }, /* vkGetPhysicalDeviceSurfaceSupportKHR */
    { 2603, 0x7b5f3fb9, 67 }, /* vkGetPhysicalDeviceToolProperties */
    { 2637, 0xd1685100, 68 }, /* vkGetPhysicalDeviceToolPropertiesEXT */
    { 2674, 0x8ee6bf8a, 70 }, /* vkGetPhysicalDeviceVideoCapabilitiesKHR */
    { 2714, 0xbb7625d6, 71 }, /* vkGetPhysicalDeviceVideoFormatPropertiesKHR */
    { 2758, 0x84e085ac, 20 }, /* vkGetPhysicalDeviceWaylandPresentationSupportKHR */
    { 2807, 0x80e72505, 21 }, /* vkGetPhysicalDeviceWin32PresentationSupportKHR */
    { 2854, 0x41782cb9, 23 }, /* vkGetPhysicalDeviceXcbPresentationSupportKHR */
    { 2899, 0x34a063ab, 22 }, /* vkGetPhysicalDeviceXlibPresentationSupportKHR */
    { 2945, 0xb87cdd6c, 49 }, /* vkGetRandROutputDisplayEXT */
    { 2972, 0x613996b5, 51 }, /* vkGetWinrtDisplayNV */
    { 2992, 0x4207f4f1, 47 }, /* vkReleaseDisplayEXT */
};

/* Hash table stats:
 * size 76 entries
 * collisions entries:
 *     0      53
 *     1      11
 *     2      7
 *     3      1
 *     4      0
 *     5      0
 *     6      0
 *     7      1
 *     8      0
 *     9+     3
 */

#define none 0xffff
static const uint16_t physical_device_string_map[128] = {
    0x0027,
    none,
    none,
    0x003e,
    0x0039,
    0x0037,
    0x0012,
    none,
    0x0028,
    0x000c,
    0x0031,
    0x0036,
    0x000b,
    0x0000,
    0x001b,
    0x0030,
    none,
    none,
    0x0019,
    0x0033,
    none,
    0x0007,
    none,
    none,
    0x0020,
    none,
    0x000e,
    0x003c,
    0x0010,
    0x0043,
    0x003a,
    0x002f,
    0x0002,
    none,
    none,
    none,
    0x000a,
    none,
    0x0042,
    0x001c,
    none,
    none,
    none,
    0x0040,
    0x0045,
    none,
    0x0015,
    0x0017,
    0x0046,
    none,
    none,
    0x001d,
    0x002c,
    0x004a,
    0x001f,
    none,
    none,
    0x002a,
    0x0013,
    0x0022,
    none,
    0x0029,
    0x0041,
    0x0003,
    none,
    0x0004,
    0x0008,
    0x0047,
    none,
    none,
    none,
    none,
    0x003d,
    0x002d,
    none,
    none,
    0x000f,
    0x0001,
    none,
    none,
    none,
    0x002b,
    0x0011,
    0x0025,
    none,
    none,
    0x0023,
    none,
    0x0032,
    none,
    0x0021,
    none,
    0x0016,
    none,
    0x0009,
    0x001a,
    0x001e,
    0x002e,
    none,
    none,
    0x003f,
    0x000d,
    none,
    0x0006,
    none,
    0x0044,
    0x0026,
    0x0018,
    0x0049,
    0x0005,
    none,
    0x0035,
    none,
    0x004b,
    0x0014,
    none,
    none,
    none,
    none,
    0x0038,
    0x0034,
    none,
    0x0024,
    none,
    0x0048,
    none,
    0x003b,
    none,
};

static int
physical_device_string_map_lookup(const char *str)
{
    static const uint32_t prime_factor = 5024183;
    static const uint32_t prime_step = 19;
    const struct string_map_entry *e;
    uint32_t hash, h;
    uint16_t i;
    const char *p;

    hash = 0;
    for (p = str; *p; p++)
        hash = hash * prime_factor + *p;

    h = hash;
    while (1) {
        i = physical_device_string_map[h & 127];
        if (i == none)
           return -1;
        e = &physical_device_string_map_entries[i];
        if (e->hash == hash && strcmp(str, physical_device_strings + e->name) == 0)
            return e->num;
        h += prime_step;
    }

    return -1;
}


static const char device_strings[] =
    "vkAcquireFullScreenExclusiveModeEXT\0"
    "vkAcquireImageANDROID\0"
    "vkAcquireNextImage2KHR\0"
    "vkAcquireNextImageKHR\0"
    "vkAcquirePerformanceConfigurationINTEL\0"
    "vkAcquireProfilingLockKHR\0"
    "vkAllocateCommandBuffers\0"
    "vkAllocateDescriptorSets\0"
    "vkAllocateMemory\0"
    "vkBeginCommandBuffer\0"
    "vkBindAccelerationStructureMemoryNV\0"
    "vkBindBufferMemory\0"
    "vkBindBufferMemory2\0"
    "vkBindBufferMemory2KHR\0"
    "vkBindImageMemory\0"
    "vkBindImageMemory2\0"
    "vkBindImageMemory2KHR\0"
    "vkBindOpticalFlowSessionImageNV\0"
    "vkBindVideoSessionMemoryKHR\0"
    "vkBuildAccelerationStructuresKHR\0"
    "vkBuildMicromapsEXT\0"
    "vkCmdBeginConditionalRenderingEXT\0"
    "vkCmdBeginDebugUtilsLabelEXT\0"
    "vkCmdBeginQuery\0"
    "vkCmdBeginQueryIndexedEXT\0"
    "vkCmdBeginRenderPass\0"
    "vkCmdBeginRenderPass2\0"
    "vkCmdBeginRenderPass2KHR\0"
    "vkCmdBeginRendering\0"
    "vkCmdBeginRenderingKHR\0"
    "vkCmdBeginTransformFeedbackEXT\0"
    "vkCmdBeginVideoCodingKHR\0"
    "vkCmdBindDescriptorBufferEmbeddedSamplersEXT\0"
    "vkCmdBindDescriptorBuffersEXT\0"
    "vkCmdBindDescriptorSets\0"
    "vkCmdBindIndexBuffer\0"
    "vkCmdBindIndexBuffer2KHR\0"
    "vkCmdBindInvocationMaskHUAWEI\0"
    "vkCmdBindPipeline\0"
    "vkCmdBindPipelineShaderGroupNV\0"
    "vkCmdBindShadersEXT\0"
    "vkCmdBindShadingRateImageNV\0"
    "vkCmdBindTransformFeedbackBuffersEXT\0"
    "vkCmdBindVertexBuffers\0"
    "vkCmdBindVertexBuffers2\0"
    "vkCmdBindVertexBuffers2EXT\0"
    "vkCmdBlitImage\0"
    "vkCmdBlitImage2\0"
    "vkCmdBlitImage2KHR\0"
    "vkCmdBuildAccelerationStructureNV\0"
    "vkCmdBuildAccelerationStructuresIndirectKHR\0"
    "vkCmdBuildAccelerationStructuresKHR\0"
    "vkCmdBuildMicromapsEXT\0"
    "vkCmdClearAttachments\0"
    "vkCmdClearColorImage\0"
    "vkCmdClearDepthStencilImage\0"
    "vkCmdControlVideoCodingKHR\0"
    "vkCmdCopyAccelerationStructureKHR\0"
    "vkCmdCopyAccelerationStructureNV\0"
    "vkCmdCopyAccelerationStructureToMemoryKHR\0"
    "vkCmdCopyBuffer\0"
    "vkCmdCopyBuffer2\0"
    "vkCmdCopyBuffer2KHR\0"
    "vkCmdCopyBufferToImage\0"
    "vkCmdCopyBufferToImage2\0"
    "vkCmdCopyBufferToImage2KHR\0"
    "vkCmdCopyImage\0"
    "vkCmdCopyImage2\0"
    "vkCmdCopyImage2KHR\0"
    "vkCmdCopyImageToBuffer\0"
    "vkCmdCopyImageToBuffer2\0"
    "vkCmdCopyImageToBuffer2KHR\0"
    "vkCmdCopyMemoryIndirectNV\0"
    "vkCmdCopyMemoryToAccelerationStructureKHR\0"
    "vkCmdCopyMemoryToImageIndirectNV\0"
    "vkCmdCopyMemoryToMicromapEXT\0"
    "vkCmdCopyMicromapEXT\0"
    "vkCmdCopyMicromapToMemoryEXT\0"
    "vkCmdCopyQueryPoolResults\0"
    "vkCmdCuLaunchKernelNVX\0"
    "vkCmdDebugMarkerBeginEXT\0"
    "vkCmdDebugMarkerEndEXT\0"
    "vkCmdDebugMarkerInsertEXT\0"
    "vkCmdDecodeVideoKHR\0"
    "vkCmdDecompressMemoryIndirectCountNV\0"
    "vkCmdDecompressMemoryNV\0"
    "vkCmdDispatch\0"
    "vkCmdDispatchBase\0"
    "vkCmdDispatchBaseKHR\0"
    "vkCmdDispatchIndirect\0"
    "vkCmdDraw\0"
    "vkCmdDrawClusterHUAWEI\0"
    "vkCmdDrawClusterIndirectHUAWEI\0"
    "vkCmdDrawIndexed\0"
    "vkCmdDrawIndexedIndirect\0"
    "vkCmdDrawIndexedIndirectCount\0"
    "vkCmdDrawIndexedIndirectCountAMD\0"
    "vkCmdDrawIndexedIndirectCountKHR\0"
    "vkCmdDrawIndirect\0"
    "vkCmdDrawIndirectByteCountEXT\0"
    "vkCmdDrawIndirectCount\0"
    "vkCmdDrawIndirectCountAMD\0"
    "vkCmdDrawIndirectCountKHR\0"
    "vkCmdDrawMeshTasksEXT\0"
    "vkCmdDrawMeshTasksIndirectCountEXT\0"
    "vkCmdDrawMeshTasksIndirectCountNV\0"
    "vkCmdDrawMeshTasksIndirectEXT\0"
    "vkCmdDrawMeshTasksIndirectNV\0"
    "vkCmdDrawMeshTasksNV\0"
    "vkCmdDrawMultiEXT\0"
    "vkCmdDrawMultiIndexedEXT\0"
    "vkCmdEndConditionalRenderingEXT\0"
    "vkCmdEndDebugUtilsLabelEXT\0"
    "vkCmdEndQuery\0"
    "vkCmdEndQueryIndexedEXT\0"
    "vkCmdEndRenderPass\0"
    "vkCmdEndRenderPass2\0"
    "vkCmdEndRenderPass2KHR\0"
    "vkCmdEndRendering\0"
    "vkCmdEndRenderingKHR\0"
    "vkCmdEndTransformFeedbackEXT\0"
    "vkCmdEndVideoCodingKHR\0"
    "vkCmdExecuteCommands\0"
    "vkCmdExecuteGeneratedCommandsNV\0"
    "vkCmdFillBuffer\0"
    "vkCmdInsertDebugUtilsLabelEXT\0"
    "vkCmdNextSubpass\0"
    "vkCmdNextSubpass2\0"
    "vkCmdNextSubpass2KHR\0"
    "vkCmdOpticalFlowExecuteNV\0"
    "vkCmdPipelineBarrier\0"
    "vkCmdPipelineBarrier2\0"
    "vkCmdPipelineBarrier2KHR\0"
    "vkCmdPreprocessGeneratedCommandsNV\0"
    "vkCmdPushConstants\0"
    "vkCmdPushDescriptorSetKHR\0"
    "vkCmdPushDescriptorSetWithTemplateKHR\0"
    "vkCmdResetEvent\0"
    "vkCmdResetEvent2\0"
    "vkCmdResetEvent2KHR\0"
    "vkCmdResetQueryPool\0"
    "vkCmdResolveImage\0"
    "vkCmdResolveImage2\0"
    "vkCmdResolveImage2KHR\0"
    "vkCmdSetAlphaToCoverageEnableEXT\0"
    "vkCmdSetAlphaToOneEnableEXT\0"
    "vkCmdSetAttachmentFeedbackLoopEnableEXT\0"
    "vkCmdSetBlendConstants\0"
    "vkCmdSetCheckpointNV\0"
    "vkCmdSetCoarseSampleOrderNV\0"
    "vkCmdSetColorBlendAdvancedEXT\0"
    "vkCmdSetColorBlendEnableEXT\0"
    "vkCmdSetColorBlendEquationEXT\0"
    "vkCmdSetColorWriteEnableEXT\0"
    "vkCmdSetColorWriteMaskEXT\0"
    "vkCmdSetConservativeRasterizationModeEXT\0"
    "vkCmdSetCoverageModulationModeNV\0"
    "vkCmdSetCoverageModulationTableEnableNV\0"
    "vkCmdSetCoverageModulationTableNV\0"
    "vkCmdSetCoverageReductionModeNV\0"
    "vkCmdSetCoverageToColorEnableNV\0"
    "vkCmdSetCoverageToColorLocationNV\0"
    "vkCmdSetCullMode\0"
    "vkCmdSetCullModeEXT\0"
    "vkCmdSetDepthBias\0"
    "vkCmdSetDepthBias2EXT\0"
    "vkCmdSetDepthBiasEnable\0"
    "vkCmdSetDepthBiasEnableEXT\0"
    "vkCmdSetDepthBounds\0"
    "vkCmdSetDepthBoundsTestEnable\0"
    "vkCmdSetDepthBoundsTestEnableEXT\0"
    "vkCmdSetDepthClampEnableEXT\0"
    "vkCmdSetDepthClipEnableEXT\0"
    "vkCmdSetDepthClipNegativeOneToOneEXT\0"
    "vkCmdSetDepthCompareOp\0"
    "vkCmdSetDepthCompareOpEXT\0"
    "vkCmdSetDepthTestEnable\0"
    "vkCmdSetDepthTestEnableEXT\0"
    "vkCmdSetDepthWriteEnable\0"
    "vkCmdSetDepthWriteEnableEXT\0"
    "vkCmdSetDescriptorBufferOffsetsEXT\0"
    "vkCmdSetDeviceMask\0"
    "vkCmdSetDeviceMaskKHR\0"
    "vkCmdSetDiscardRectangleEXT\0"
    "vkCmdSetDiscardRectangleEnableEXT\0"
    "vkCmdSetDiscardRectangleModeEXT\0"
    "vkCmdSetEvent\0"
    "vkCmdSetEvent2\0"
    "vkCmdSetEvent2KHR\0"
    "vkCmdSetExclusiveScissorEnableNV\0"
    "vkCmdSetExclusiveScissorNV\0"
    "vkCmdSetExtraPrimitiveOverestimationSizeEXT\0"
    "vkCmdSetFragmentShadingRateEnumNV\0"
    "vkCmdSetFragmentShadingRateKHR\0"
    "vkCmdSetFrontFace\0"
    "vkCmdSetFrontFaceEXT\0"
    "vkCmdSetLineRasterizationModeEXT\0"
    "vkCmdSetLineStippleEXT\0"
    "vkCmdSetLineStippleEnableEXT\0"
    "vkCmdSetLineWidth\0"
    "vkCmdSetLogicOpEXT\0"
    "vkCmdSetLogicOpEnableEXT\0"
    "vkCmdSetPatchControlPointsEXT\0"
    "vkCmdSetPerformanceMarkerINTEL\0"
    "vkCmdSetPerformanceOverrideINTEL\0"
    "vkCmdSetPerformanceStreamMarkerINTEL\0"
    "vkCmdSetPolygonModeEXT\0"
    "vkCmdSetPrimitiveRestartEnable\0"
    "vkCmdSetPrimitiveRestartEnableEXT\0"
    "vkCmdSetPrimitiveTopology\0"
    "vkCmdSetPrimitiveTopologyEXT\0"
    "vkCmdSetProvokingVertexModeEXT\0"
    "vkCmdSetRasterizationSamplesEXT\0"
    "vkCmdSetRasterizationStreamEXT\0"
    "vkCmdSetRasterizerDiscardEnable\0"
    "vkCmdSetRasterizerDiscardEnableEXT\0"
    "vkCmdSetRayTracingPipelineStackSizeKHR\0"
    "vkCmdSetRepresentativeFragmentTestEnableNV\0"
    "vkCmdSetSampleLocationsEXT\0"
    "vkCmdSetSampleLocationsEnableEXT\0"
    "vkCmdSetSampleMaskEXT\0"
    "vkCmdSetScissor\0"
    "vkCmdSetScissorWithCount\0"
    "vkCmdSetScissorWithCountEXT\0"
    "vkCmdSetShadingRateImageEnableNV\0"
    "vkCmdSetStencilCompareMask\0"
    "vkCmdSetStencilOp\0"
    "vkCmdSetStencilOpEXT\0"
    "vkCmdSetStencilReference\0"
    "vkCmdSetStencilTestEnable\0"
    "vkCmdSetStencilTestEnableEXT\0"
    "vkCmdSetStencilWriteMask\0"
    "vkCmdSetTessellationDomainOriginEXT\0"
    "vkCmdSetVertexInputEXT\0"
    "vkCmdSetViewport\0"
    "vkCmdSetViewportShadingRatePaletteNV\0"
    "vkCmdSetViewportSwizzleNV\0"
    "vkCmdSetViewportWScalingEnableNV\0"
    "vkCmdSetViewportWScalingNV\0"
    "vkCmdSetViewportWithCount\0"
    "vkCmdSetViewportWithCountEXT\0"
    "vkCmdSubpassShadingHUAWEI\0"
    "vkCmdTraceRaysIndirect2KHR\0"
    "vkCmdTraceRaysIndirectKHR\0"
    "vkCmdTraceRaysKHR\0"
    "vkCmdTraceRaysNV\0"
    "vkCmdUpdateBuffer\0"
    "vkCmdUpdatePipelineIndirectBufferNV\0"
    "vkCmdWaitEvents\0"
    "vkCmdWaitEvents2\0"
    "vkCmdWaitEvents2KHR\0"
    "vkCmdWriteAccelerationStructuresPropertiesKHR\0"
    "vkCmdWriteAccelerationStructuresPropertiesNV\0"
    "vkCmdWriteBufferMarker2AMD\0"
    "vkCmdWriteBufferMarkerAMD\0"
    "vkCmdWriteMicromapsPropertiesEXT\0"
    "vkCmdWriteTimestamp\0"
    "vkCmdWriteTimestamp2\0"
    "vkCmdWriteTimestamp2KHR\0"
    "vkCompileDeferredNV\0"
    "vkCopyAccelerationStructureKHR\0"
    "vkCopyAccelerationStructureToMemoryKHR\0"
    "vkCopyImageToImageEXT\0"
    "vkCopyImageToMemoryEXT\0"
    "vkCopyMemoryToAccelerationStructureKHR\0"
    "vkCopyMemoryToImageEXT\0"
    "vkCopyMemoryToMicromapEXT\0"
    "vkCopyMicromapEXT\0"
    "vkCopyMicromapToMemoryEXT\0"
    "vkCreateAccelerationStructureKHR\0"
    "vkCreateAccelerationStructureNV\0"
    "vkCreateBuffer\0"
    "vkCreateBufferCollectionFUCHSIA\0"
    "vkCreateBufferView\0"
    "vkCreateCommandPool\0"
    "vkCreateComputePipelines\0"
    "vkCreateCuFunctionNVX\0"
    "vkCreateCuModuleNVX\0"
    "vkCreateDeferredOperationKHR\0"
    "vkCreateDescriptorPool\0"
    "vkCreateDescriptorSetLayout\0"
    "vkCreateDescriptorUpdateTemplate\0"
    "vkCreateDescriptorUpdateTemplateKHR\0"
    "vkCreateEvent\0"
    "vkCreateFence\0"
    "vkCreateFramebuffer\0"
    "vkCreateGraphicsPipelines\0"
    "vkCreateImage\0"
    "vkCreateImageView\0"
    "vkCreateIndirectCommandsLayoutNV\0"
    "vkCreateMicromapEXT\0"
    "vkCreateOpticalFlowSessionNV\0"
    "vkCreatePipelineCache\0"
    "vkCreatePipelineLayout\0"
    "vkCreatePrivateDataSlot\0"
    "vkCreatePrivateDataSlotEXT\0"
    "vkCreateQueryPool\0"
    "vkCreateRayTracingPipelinesKHR\0"
    "vkCreateRayTracingPipelinesNV\0"
    "vkCreateRenderPass\0"
    "vkCreateRenderPass2\0"
    "vkCreateRenderPass2KHR\0"
    "vkCreateSampler\0"
    "vkCreateSamplerYcbcrConversion\0"
    "vkCreateSamplerYcbcrConversionKHR\0"
    "vkCreateSemaphore\0"
    "vkCreateShaderModule\0"
    "vkCreateShadersEXT\0"
    "vkCreateSharedSwapchainsKHR\0"
    "vkCreateSwapchainKHR\0"
    "vkCreateValidationCacheEXT\0"
    "vkCreateVideoSessionKHR\0"
    "vkCreateVideoSessionParametersKHR\0"
    "vkDebugMarkerSetObjectNameEXT\0"
    "vkDebugMarkerSetObjectTagEXT\0"
    "vkDeferredOperationJoinKHR\0"
    "vkDestroyAccelerationStructureKHR\0"
    "vkDestroyAccelerationStructureNV\0"
    "vkDestroyBuffer\0"
    "vkDestroyBufferCollectionFUCHSIA\0"
    "vkDestroyBufferView\0"
    "vkDestroyCommandPool\0"
    "vkDestroyCuFunctionNVX\0"
    "vkDestroyCuModuleNVX\0"
    "vkDestroyDeferredOperationKHR\0"
    "vkDestroyDescriptorPool\0"
    "vkDestroyDescriptorSetLayout\0"
    "vkDestroyDescriptorUpdateTemplate\0"
    "vkDestroyDescriptorUpdateTemplateKHR\0"
    "vkDestroyDevice\0"
    "vkDestroyEvent\0"
    "vkDestroyFence\0"
    "vkDestroyFramebuffer\0"
    "vkDestroyImage\0"
    "vkDestroyImageView\0"
    "vkDestroyIndirectCommandsLayoutNV\0"
    "vkDestroyMicromapEXT\0"
    "vkDestroyOpticalFlowSessionNV\0"
    "vkDestroyPipeline\0"
    "vkDestroyPipelineCache\0"
    "vkDestroyPipelineLayout\0"
    "vkDestroyPrivateDataSlot\0"
    "vkDestroyPrivateDataSlotEXT\0"
    "vkDestroyQueryPool\0"
    "vkDestroyRenderPass\0"
    "vkDestroySampler\0"
    "vkDestroySamplerYcbcrConversion\0"
    "vkDestroySamplerYcbcrConversionKHR\0"
    "vkDestroySemaphore\0"
    "vkDestroyShaderEXT\0"
    "vkDestroyShaderModule\0"
    "vkDestroySwapchainKHR\0"
    "vkDestroyValidationCacheEXT\0"
    "vkDestroyVideoSessionKHR\0"
    "vkDestroyVideoSessionParametersKHR\0"
    "vkDeviceWaitIdle\0"
    "vkDisplayPowerControlEXT\0"
    "vkEndCommandBuffer\0"
    "vkExportMetalObjectsEXT\0"
    "vkFlushMappedMemoryRanges\0"
    "vkFreeCommandBuffers\0"
    "vkFreeDescriptorSets\0"
    "vkFreeMemory\0"
    "vkGetAccelerationStructureBuildSizesKHR\0"
    "vkGetAccelerationStructureDeviceAddressKHR\0"
    "vkGetAccelerationStructureHandleNV\0"
    "vkGetAccelerationStructureMemoryRequirementsNV\0"
    "vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT\0"
    "vkGetAndroidHardwareBufferPropertiesANDROID\0"
    "vkGetBufferCollectionPropertiesFUCHSIA\0"
    "vkGetBufferDeviceAddress\0"
    "vkGetBufferDeviceAddressEXT\0"
    "vkGetBufferDeviceAddressKHR\0"
    "vkGetBufferMemoryRequirements\0"
    "vkGetBufferMemoryRequirements2\0"
    "vkGetBufferMemoryRequirements2KHR\0"
    "vkGetBufferOpaqueCaptureAddress\0"
    "vkGetBufferOpaqueCaptureAddressKHR\0"
    "vkGetBufferOpaqueCaptureDescriptorDataEXT\0"
    "vkGetCalibratedTimestampsEXT\0"
    "vkGetDeferredOperationMaxConcurrencyKHR\0"
    "vkGetDeferredOperationResultKHR\0"
    "vkGetDescriptorEXT\0"
    "vkGetDescriptorSetHostMappingVALVE\0"
    "vkGetDescriptorSetLayoutBindingOffsetEXT\0"
    "vkGetDescriptorSetLayoutHostMappingInfoVALVE\0"
    "vkGetDescriptorSetLayoutSizeEXT\0"
    "vkGetDescriptorSetLayoutSupport\0"
    "vkGetDescriptorSetLayoutSupportKHR\0"
    "vkGetDeviceAccelerationStructureCompatibilityKHR\0"
    "vkGetDeviceBufferMemoryRequirements\0"
    "vkGetDeviceBufferMemoryRequirementsKHR\0"
    "vkGetDeviceFaultInfoEXT\0"
    "vkGetDeviceGroupPeerMemoryFeatures\0"
    "vkGetDeviceGroupPeerMemoryFeaturesKHR\0"
    "vkGetDeviceGroupPresentCapabilitiesKHR\0"
    "vkGetDeviceGroupSurfacePresentModes2EXT\0"
    "vkGetDeviceGroupSurfacePresentModesKHR\0"
    "vkGetDeviceImageMemoryRequirements\0"
    "vkGetDeviceImageMemoryRequirementsKHR\0"
    "vkGetDeviceImageSparseMemoryRequirements\0"
    "vkGetDeviceImageSparseMemoryRequirementsKHR\0"
    "vkGetDeviceImageSubresourceLayoutKHR\0"
    "vkGetDeviceMemoryCommitment\0"
    "vkGetDeviceMemoryOpaqueCaptureAddress\0"
    "vkGetDeviceMemoryOpaqueCaptureAddressKHR\0"
    "vkGetDeviceMicromapCompatibilityEXT\0"
    "vkGetDeviceProcAddr\0"
    "vkGetDeviceQueue\0"
    "vkGetDeviceQueue2\0"
    "vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI\0"
    "vkGetDynamicRenderingTilePropertiesQCOM\0"
    "vkGetEventStatus\0"
    "vkGetFenceFdKHR\0"
    "vkGetFenceStatus\0"
    "vkGetFenceWin32HandleKHR\0"
    "vkGetFramebufferTilePropertiesQCOM\0"
    "vkGetGeneratedCommandsMemoryRequirementsNV\0"
    "vkGetImageDrmFormatModifierPropertiesEXT\0"
    "vkGetImageMemoryRequirements\0"
    "vkGetImageMemoryRequirements2\0"
    "vkGetImageMemoryRequirements2KHR\0"
    "vkGetImageOpaqueCaptureDescriptorDataEXT\0"
    "vkGetImageSparseMemoryRequirements\0"
    "vkGetImageSparseMemoryRequirements2\0"
    "vkGetImageSparseMemoryRequirements2KHR\0"
    "vkGetImageSubresourceLayout\0"
    "vkGetImageSubresourceLayout2EXT\0"
    "vkGetImageSubresourceLayout2KHR\0"
    "vkGetImageViewAddressNVX\0"
    "vkGetImageViewHandleNVX\0"
    "vkGetImageViewOpaqueCaptureDescriptorDataEXT\0"
    "vkGetLatencyTimingsNV\0"
    "vkGetMemoryAndroidHardwareBufferANDROID\0"
    "vkGetMemoryFdKHR\0"
    "vkGetMemoryFdPropertiesKHR\0"
    "vkGetMemoryHostPointerPropertiesEXT\0"
    "vkGetMemoryRemoteAddressNV\0"
    "vkGetMemoryWin32HandleKHR\0"
    "vkGetMemoryWin32HandleNV\0"
    "vkGetMemoryWin32HandlePropertiesKHR\0"
    "vkGetMemoryZirconHandleFUCHSIA\0"
    "vkGetMemoryZirconHandlePropertiesFUCHSIA\0"
    "vkGetMicromapBuildSizesEXT\0"
    "vkGetPastPresentationTimingGOOGLE\0"
    "vkGetPerformanceParameterINTEL\0"
    "vkGetPipelineCacheData\0"
    "vkGetPipelineExecutableInternalRepresentationsKHR\0"
    "vkGetPipelineExecutablePropertiesKHR\0"
    "vkGetPipelineExecutableStatisticsKHR\0"
    "vkGetPipelineIndirectDeviceAddressNV\0"
    "vkGetPipelineIndirectMemoryRequirementsNV\0"
    "vkGetPipelinePropertiesEXT\0"
    "vkGetPrivateData\0"
    "vkGetPrivateDataEXT\0"
    "vkGetQueryPoolResults\0"
    "vkGetQueueCheckpointData2NV\0"
    "vkGetQueueCheckpointDataNV\0"
    "vkGetRayTracingCaptureReplayShaderGroupHandlesKHR\0"
    "vkGetRayTracingShaderGroupHandlesKHR\0"
    "vkGetRayTracingShaderGroupHandlesNV\0"
    "vkGetRayTracingShaderGroupStackSizeKHR\0"
    "vkGetRefreshCycleDurationGOOGLE\0"
    "vkGetRenderAreaGranularity\0"
    "vkGetRenderingAreaGranularityKHR\0"
    "vkGetSamplerOpaqueCaptureDescriptorDataEXT\0"
    "vkGetScreenBufferPropertiesQNX\0"
    "vkGetSemaphoreCounterValue\0"
    "vkGetSemaphoreCounterValueKHR\0"
    "vkGetSemaphoreFdKHR\0"
    "vkGetSemaphoreWin32HandleKHR\0"
    "vkGetSemaphoreZirconHandleFUCHSIA\0"
    "vkGetShaderBinaryDataEXT\0"
    "vkGetShaderInfoAMD\0"
    "vkGetShaderModuleCreateInfoIdentifierEXT\0"
    "vkGetShaderModuleIdentifierEXT\0"
    "vkGetSwapchainCounterEXT\0"
    "vkGetSwapchainGrallocUsage2ANDROID\0"
    "vkGetSwapchainGrallocUsageANDROID\0"
    "vkGetSwapchainImagesKHR\0"
    "vkGetSwapchainStatusKHR\0"
    "vkGetValidationCacheDataEXT\0"
    "vkGetVideoSessionMemoryRequirementsKHR\0"
    "vkImportFenceFdKHR\0"
    "vkImportFenceWin32HandleKHR\0"
    "vkImportSemaphoreFdKHR\0"
    "vkImportSemaphoreWin32HandleKHR\0"
    "vkImportSemaphoreZirconHandleFUCHSIA\0"
    "vkInitializePerformanceApiINTEL\0"
    "vkInvalidateMappedMemoryRanges\0"
    "vkLatencySleepNV\0"
    "vkMapMemory\0"
    "vkMapMemory2KHR\0"
    "vkMergePipelineCaches\0"
    "vkMergeValidationCachesEXT\0"
    "vkQueueBeginDebugUtilsLabelEXT\0"
    "vkQueueBindSparse\0"
    "vkQueueEndDebugUtilsLabelEXT\0"
    "vkQueueInsertDebugUtilsLabelEXT\0"
    "vkQueueNotifyOutOfBandNV\0"
    "vkQueuePresentKHR\0"
    "vkQueueSetPerformanceConfigurationINTEL\0"
    "vkQueueSignalReleaseImageANDROID\0"
    "vkQueueSubmit\0"
    "vkQueueSubmit2\0"
    "vkQueueSubmit2KHR\0"
    "vkQueueWaitIdle\0"
    "vkRegisterDeviceEventEXT\0"
    "vkRegisterDisplayEventEXT\0"
    "vkReleaseFullScreenExclusiveModeEXT\0"
    "vkReleasePerformanceConfigurationINTEL\0"
    "vkReleaseProfilingLockKHR\0"
    "vkReleaseSwapchainImagesEXT\0"
    "vkResetCommandBuffer\0"
    "vkResetCommandPool\0"
    "vkResetDescriptorPool\0"
    "vkResetEvent\0"
    "vkResetFences\0"
    "vkResetQueryPool\0"
    "vkResetQueryPoolEXT\0"
    "vkSetBufferCollectionBufferConstraintsFUCHSIA\0"
    "vkSetBufferCollectionImageConstraintsFUCHSIA\0"
    "vkSetDebugUtilsObjectNameEXT\0"
    "vkSetDebugUtilsObjectTagEXT\0"
    "vkSetDeviceMemoryPriorityEXT\0"
    "vkSetEvent\0"
    "vkSetHdrMetadataEXT\0"
    "vkSetLatencyMarkerNV\0"
    "vkSetLatencySleepModeNV\0"
    "vkSetLocalDimmingAMD\0"
    "vkSetPrivateData\0"
    "vkSetPrivateDataEXT\0"
    "vkSignalSemaphore\0"
    "vkSignalSemaphoreKHR\0"
    "vkTransitionImageLayoutEXT\0"
    "vkTrimCommandPool\0"
    "vkTrimCommandPoolKHR\0"
    "vkUninitializePerformanceApiINTEL\0"
    "vkUnmapMemory\0"
    "vkUnmapMemory2KHR\0"
    "vkUpdateDescriptorSetWithTemplate\0"
    "vkUpdateDescriptorSetWithTemplateKHR\0"
    "vkUpdateDescriptorSets\0"
    "vkUpdateVideoSessionParametersKHR\0"
    "vkWaitForFences\0"
    "vkWaitForPresentKHR\0"
    "vkWaitSemaphores\0"
    "vkWaitSemaphoresKHR\0"
    "vkWriteAccelerationStructuresPropertiesKHR\0"
    "vkWriteMicromapsPropertiesEXT\0"
;

static const struct string_map_entry device_string_map_entries[] = {
    { 0, 0x8a43a1cc, 322 }, /* vkAcquireFullScreenExclusiveModeEXT */
    { 36, 0x6bf780dd, 232 }, /* vkAcquireImageANDROID */
    { 58, 0x82860572, 188 }, /* vkAcquireNextImage2KHR */
    { 81, 0xc3fedb2e, 140 }, /* vkAcquireNextImageKHR */
    { 103, 0x33d2767, 337 }, /* vkAcquirePerformanceConfigurationINTEL */
    { 142, 0xaf1d64ad, 324 }, /* vkAcquireProfilingLockKHR */
    { 168, 0x8c0c811a, 76 }, /* vkAllocateCommandBuffers */
    { 193, 0x4c449d3a, 64 }, /* vkAllocateDescriptorSets */
    { 218, 0x522b85d3, 6 }, /* vkAllocateMemory */
    { 235, 0xc54f7327, 78 }, /* vkBeginCommandBuffer */
    { 256, 0x3ec4e21a, 294 }, /* vkBindAccelerationStructureMemoryNV */
    { 292, 0x6bcbdcb, 14 }, /* vkBindBufferMemory */
    { 311, 0xc27aaf4f, 180 }, /* vkBindBufferMemory2 */
    { 331, 0x6878d3ce, 181 }, /* vkBindBufferMemory2KHR */
    { 354, 0x5caaae4a, 16 }, /* vkBindImageMemory */
    { 372, 0xa9097118, 182 }, /* vkBindImageMemory2 */
    { 391, 0xf18729ad, 183 }, /* vkBindImageMemory2KHR */
    { 413, 0xc3b6afe1, 532 }, /* vkBindOpticalFlowSessionImageNV */
    { 445, 0x61c0a1e7, 472 }, /* vkBindVideoSessionMemoryKHR */
    { 473, 0xf7d6c55c, 350 }, /* vkBuildAccelerationStructuresKHR */
    { 506, 0x9aa65b87, 510 }, /* vkBuildMicromapsEXT */
    { 526, 0xe561c19f, 126 }, /* vkCmdBeginConditionalRenderingEXT */
    { 560, 0x6184193f, 242 }, /* vkCmdBeginDebugUtilsLabelEXT */
    { 589, 0xf5064ea4, 124 }, /* vkCmdBeginQuery */
    { 605, 0x73251a2c, 274 }, /* vkCmdBeginQueryIndexedEXT */
    { 631, 0xcb7a58e3, 132 }, /* vkCmdBeginRenderPass */
    { 652, 0x9c876577, 249 }, /* vkCmdBeginRenderPass2 */
    { 674, 0x8b6b4de6, 250 }, /* vkCmdBeginRenderPass2KHR */
    { 699, 0x385cfdb8, 502 }, /* vkCmdBeginRendering */
    { 719, 0x50d17e0d, 503 }, /* vkCmdBeginRenderingKHR */
    { 742, 0xb217c94, 272 }, /* vkCmdBeginTransformFeedbackEXT */
    { 773, 0xd941eabc, 474 }, /* vkCmdBeginVideoCodingKHR */
    { 798, 0xaf295ae3, 489 }, /* vkCmdBindDescriptorBufferEmbeddedSamplersEXT */
    { 843, 0x64e825f, 487 }, /* vkCmdBindDescriptorBuffersEXT */
    { 873, 0x28c7a5da, 92 }, /* vkCmdBindDescriptorSets */
    { 897, 0x4c22d870, 93 }, /* vkCmdBindIndexBuffer */
    { 918, 0xce5c6793, 369 }, /* vkCmdBindIndexBuffer2KHR */
    { 943, 0xa8f55bdd, 290 }, /* vkCmdBindInvocationMaskHUAWEI */
    { 973, 0x3af9fd84, 81 }, /* vkCmdBindPipeline */
    { 991, 0x353570d6, 150 }, /* vkCmdBindPipelineShaderGroupNV */
    { 1022, 0x3e18f3f5, 543 }, /* vkCmdBindShadersEXT */
    { 1042, 0xbae753eb, 279 }, /* vkCmdBindShadingRateImageNV */
    { 1070, 0x98fdb5cd, 271 }, /* vkCmdBindTransformFeedbackBuffersEXT */
    { 1107, 0xa9c83f1d, 94 }, /* vkCmdBindVertexBuffers */
    { 1130, 0x4c6b42ed, 370 }, /* vkCmdBindVertexBuffers2 */
    { 1154, 0x30a5f2ec, 371 }, /* vkCmdBindVertexBuffers2EXT */
    { 1181, 0x331ebf89, 109 }, /* vkCmdBlitImage */
    { 1196, 0xb4185c21, 435 }, /* vkCmdBlitImage2 */
    { 1212, 0x785f984c, 436 }, /* vkCmdBlitImage2KHR */
    { 1231, 0x8e9d180a, 304 }, /* vkCmdBuildAccelerationStructureNV */
    { 1265, 0xb02d6bee, 349 }, /* vkCmdBuildAccelerationStructuresIndirectKHR */
    { 1309, 0x25a727dc, 348 }, /* vkCmdBuildAccelerationStructuresKHR */
    { 1345, 0x331d3d07, 509 }, /* vkCmdBuildMicromapsEXT */
    { 1368, 0x93cb5cb8, 118 }, /* vkCmdClearAttachments */
    { 1390, 0xb4bc8d08, 116 }, /* vkCmdClearColorImage */
    { 1411, 0x4f88e4ba, 117 }, /* vkCmdClearDepthStencilImage */
    { 1439, 0x2a5f6f70, 475 }, /* vkCmdControlVideoCodingKHR */
    { 1466, 0xe2d4fe2c, 296 }, /* vkCmdCopyAccelerationStructureKHR */
    { 1500, 0x84ab5629, 295 }, /* vkCmdCopyAccelerationStructureNV */
    { 1533, 0x46b2a8a0, 298 }, /* vkCmdCopyAccelerationStructureToMemoryKHR */
    { 1575, 0xc939a0da, 107 }, /* vkCmdCopyBuffer */
    { 1591, 0xa419e608, 431 }, /* vkCmdCopyBuffer2 */
    { 1608, 0x90c5563d, 432 }, /* vkCmdCopyBuffer2KHR */
    { 1628, 0x929847e, 110 }, /* vkCmdCopyBufferToImage */
    { 1651, 0x4a8ce444, 437 }, /* vkCmdCopyBufferToImage2 */
    { 1675, 0x1e9f6861, 438 }, /* vkCmdCopyBufferToImage2KHR */
    { 1702, 0x278effa9, 108 }, /* vkCmdCopyImage */
    { 1717, 0x942b5301, 433 }, /* vkCmdCopyImage2 */
    { 1733, 0xdad52c6c, 434 }, /* vkCmdCopyImage2KHR */
    { 1752, 0x68cddbac, 111 }, /* vkCmdCopyImageToBuffer */
    { 1775, 0x83c9426, 439 }, /* vkCmdCopyImageToBuffer2 */
    { 1799, 0x2db6484f, 440 }, /* vkCmdCopyImageToBuffer2KHR */
    { 1826, 0x329f3fc9, 112 }, /* vkCmdCopyMemoryIndirectNV */
    { 1852, 0xa76c5fd8, 300 }, /* vkCmdCopyMemoryToAccelerationStructureKHR */
    { 1894, 0x42caec43, 113 }, /* vkCmdCopyMemoryToImageIndirectNV */
    { 1927, 0x47d6c41b, 516 }, /* vkCmdCopyMemoryToMicromapEXT */
    { 1956, 0x8c3811e7, 512 }, /* vkCmdCopyMicromapEXT */
    { 1977, 0x715dff1b, 514 }, /* vkCmdCopyMicromapToMemoryEXT */
    { 2006, 0xdee8c6d4, 130 }, /* vkCmdCopyQueryPoolResults */
    { 2032, 0x29000809, 483 }, /* vkCmdCuLaunchKernelNVX */
    { 2055, 0xaec8bb10, 144 }, /* vkCmdDebugMarkerBeginEXT */
    { 2080, 0xd6a1433e, 145 }, /* vkCmdDebugMarkerEndEXT */
    { 2103, 0x173d440, 146 }, /* vkCmdDebugMarkerInsertEXT */
    { 2129, 0x5c38928d, 473 }, /* vkCmdDecodeVideoKHR */
    { 2149, 0xca5aa668, 478 }, /* vkCmdDecompressMemoryIndirectCountNV */
    { 2186, 0xbb136ec9, 477 }, /* vkCmdDecompressMemoryNV */
    { 2210, 0xbd58e867, 101 }, /* vkCmdDispatch */
    { 2224, 0xfb767220, 189 }, /* vkCmdDispatchBase */
    { 2242, 0x402403e5, 190 }, /* vkCmdDispatchBaseKHR */
    { 2263, 0xd6353005, 102 }, /* vkCmdDispatchIndirect */
    { 2285, 0x9912c1a1, 95 }, /* vkCmdDraw */
    { 2295, 0x255fa5c8, 104 }, /* vkCmdDrawClusterHUAWEI */
    { 2318, 0x73efb906, 105 }, /* vkCmdDrawClusterIndirectHUAWEI */
    { 2349, 0xbe5a8058, 96 }, /* vkCmdDrawIndexed */
    { 2366, 0x94e7ed36, 100 }, /* vkCmdDrawIndexedIndirect */
    { 2391, 0xb4acef41, 266 }, /* vkCmdDrawIndexedIndirectCount */
    { 2421, 0xc86e9287, 268 }, /* vkCmdDrawIndexedIndirectCountAMD */
    { 2454, 0xda9e8a2c, 267 }, /* vkCmdDrawIndexedIndirectCountKHR */
    { 2487, 0xe9ac41bf, 99 }, /* vkCmdDrawIndirect */
    { 2505, 0x80c3b089, 276 }, /* vkCmdDrawIndirectByteCountEXT */
    { 2535, 0x40079990, 263 }, /* vkCmdDrawIndirectCount */
    { 2558, 0xe5ad0a50, 265 }, /* vkCmdDrawIndirectCountAMD */
    { 2584, 0xf7dd01f5, 264 }, /* vkCmdDrawIndirectCountKHR */
    { 2610, 0xfa045961, 285 }, /* vkCmdDrawMeshTasksEXT */
    { 2632, 0xac310210, 287 }, /* vkCmdDrawMeshTasksIndirectCountEXT */
    { 2667, 0xf21f391, 284 }, /* vkCmdDrawMeshTasksIndirectCountNV */
    { 2701, 0x6c0d4cb3, 286 }, /* vkCmdDrawMeshTasksIndirectEXT */
    { 2731, 0xaecd0a06, 283 }, /* vkCmdDrawMeshTasksIndirectNV */
    { 2760, 0xfba21ac8, 282 }, /* vkCmdDrawMeshTasksNV */
    { 2781, 0xa6c231d9, 97 }, /* vkCmdDrawMultiEXT */
    { 2799, 0xb6bd0f40, 98 }, /* vkCmdDrawMultiIndexedEXT */
    { 2824, 0x18c8217d, 127 }, /* vkCmdEndConditionalRenderingEXT */
    { 2856, 0x29875911, 243 }, /* vkCmdEndDebugUtilsLabelEXT */
    { 2883, 0xd556fd22, 125 }, /* vkCmdEndQuery */
    { 2897, 0xd5c2f48a, 275 }, /* vkCmdEndQueryIndexedEXT */
    { 2921, 0xdcdb0235, 134 }, /* vkCmdEndRenderPass */
    { 2940, 0x1cbf9115, 253 }, /* vkCmdEndRenderPass2 */
    { 2960, 0x57eebe78, 254 }, /* vkCmdEndRenderPass2KHR */
    { 2983, 0x22c5e6f6, 504 }, /* vkCmdEndRendering */
    { 3001, 0xabf9ff, 505 }, /* vkCmdEndRenderingKHR */
    { 3022, 0xf008d706, 273 }, /* vkCmdEndTransformFeedbackEXT */
    { 3051, 0xa5c55b4e, 476 }, /* vkCmdEndVideoCodingKHR */
    { 3074, 0x9eaabe40, 135 }, /* vkCmdExecuteCommands */
    { 3095, 0xe02372d7, 148 }, /* vkCmdExecuteGeneratedCommandsNV */
    { 3127, 0x5bdd2ae0, 115 }, /* vkCmdFillBuffer */
    { 3143, 0xce6aa7d1, 244 }, /* vkCmdInsertDebugUtilsLabelEXT */
    { 3173, 0x2eeec2f9, 133 }, /* vkCmdNextSubpass */
    { 3190, 0xd4fc131, 251 }, /* vkCmdNextSubpass2 */
    { 3208, 0x25b621bc, 252 }, /* vkCmdNextSubpass2KHR */
    { 3229, 0x9fa9b32c, 533 }, /* vkCmdOpticalFlowExecuteNV */
    { 3255, 0x97fccfe8, 123 }, /* vkCmdPipelineBarrier */
    { 3276, 0x43d8c70a, 454 }, /* vkCmdPipelineBarrier2 */
    { 3298, 0x9654ba0b, 455 }, /* vkCmdPipelineBarrier2KHR */
    { 3323, 0x26eff1e6, 149 }, /* vkCmdPreprocessGeneratedCommandsNV */
    { 3358, 0xb1c6b468, 131 }, /* vkCmdPushConstants */
    { 3377, 0xf17232a1, 154 }, /* vkCmdPushDescriptorSetKHR */
    { 3403, 0x3d528981, 197 }, /* vkCmdPushDescriptorSetWithTemplateKHR */
    { 3441, 0x4fccce28, 121 }, /* vkCmdResetEvent */
    { 3457, 0x735fc6ca, 450 }, /* vkCmdResetEvent2 */
    { 3474, 0x950a204b, 451 }, /* vkCmdResetEvent2KHR */
    { 3494, 0x2f614082, 128 }, /* vkCmdResetQueryPool */
    { 3514, 0x671bb594, 119 }, /* vkCmdResolveImage */
    { 3532, 0xe79d80fe, 441 }, /* vkCmdResolveImage2 */
    { 3551, 0x9fea6337, 442 }, /* vkCmdResolveImage2KHR */
    { 3573, 0x8344384a, 397 }, /* vkCmdSetAlphaToCoverageEnableEXT */
    { 3606, 0x246d31e4, 398 }, /* vkCmdSetAlphaToOneEnableEXT */
    { 3634, 0xd6cf1e45, 82 }, /* vkCmdSetAttachmentFeedbackLoopEnableEXT */
    { 3674, 0x1c989dfb, 87 }, /* vkCmdSetBlendConstants */
    { 3697, 0x4331556d, 269 }, /* vkCmdSetCheckpointNV */
    { 3718, 0xcd76e1c0, 281 }, /* vkCmdSetCoarseSampleOrderNV */
    { 3746, 0x67adf3f4, 408 }, /* vkCmdSetColorBlendAdvancedEXT */
    { 3776, 0x7242b4bb, 400 }, /* vkCmdSetColorBlendEnableEXT */
    { 3804, 0x6828ae82, 401 }, /* vkCmdSetColorBlendEquationEXT */
    { 3834, 0x5402d31d, 447 }, /* vkCmdSetColorWriteEnableEXT */
    { 3862, 0x80a5014c, 402 }, /* vkCmdSetColorWriteMaskEXT */
    { 3888, 0xbd4e36a9, 404 }, /* vkCmdSetConservativeRasterizationModeEXT */
    { 3929, 0xd1185034, 417 }, /* vkCmdSetCoverageModulationModeNV */
    { 3962, 0x58921b18, 418 }, /* vkCmdSetCoverageModulationTableEnableNV */
    { 4002, 0xc32a1ef5, 419 }, /* vkCmdSetCoverageModulationTableNV */
    { 4036, 0x64509d31, 421 }, /* vkCmdSetCoverageReductionModeNV */
    { 4068, 0xb956bfce, 415 }, /* vkCmdSetCoverageToColorEnableNV */
    { 4100, 0xf35aaa00, 416 }, /* vkCmdSetCoverageToColorLocationNV */
    { 4134, 0xcf691c42, 359 }, /* vkCmdSetCullMode */
    { 4151, 0xb7fcea1f, 360 }, /* vkCmdSetCullModeEXT */
    { 4171, 0x30f14d07, 86 }, /* vkCmdSetDepthBias */
    { 4189, 0xc3139716, 535 }, /* vkCmdSetDepthBias2EXT */
    { 4211, 0xc07e1b1a, 387 }, /* vkCmdSetDepthBiasEnable */
    { 4235, 0x5d604307, 388 }, /* vkCmdSetDepthBiasEnableEXT */
    { 4262, 0x7b3a8a63, 88 }, /* vkCmdSetDepthBounds */
    { 4282, 0xe72cce0, 378 }, /* vkCmdSetDepthBoundsTestEnable */
    { 4312, 0x3f2ddb1, 379 }, /* vkCmdSetDepthBoundsTestEnableEXT */
    { 4345, 0x6a365461, 393 }, /* vkCmdSetDepthClampEnableEXT */
    { 4373, 0x88cb8180, 406 }, /* vkCmdSetDepthClipEnableEXT */
    { 4400, 0x69601111, 412 }, /* vkCmdSetDepthClipNegativeOneToOneEXT */
    { 4437, 0xda98add0, 376 }, /* vkCmdSetDepthCompareOp */
    { 4460, 0x2f377e41, 377 }, /* vkCmdSetDepthCompareOpEXT */
    { 4486, 0x68666de3, 372 }, /* vkCmdSetDepthTestEnable */
    { 4510, 0x57c5efe6, 373 }, /* vkCmdSetDepthTestEnableEXT */
    { 4537, 0x373bda6c, 374 }, /* vkCmdSetDepthWriteEnable */
    { 4562, 0xbe217905, 375 }, /* vkCmdSetDepthWriteEnableEXT */
    { 4590, 0x6cef26ad, 488 }, /* vkCmdSetDescriptorBufferOffsetsEXT */
    { 4625, 0xaecdae87, 184 }, /* vkCmdSetDeviceMask */
    { 4644, 0xfbb79356, 185 }, /* vkCmdSetDeviceMaskKHR */
    { 4666, 0x64df188b, 203 }, /* vkCmdSetDiscardRectangleEXT */
    { 4694, 0xb7bee320, 204 }, /* vkCmdSetDiscardRectangleEnableEXT */
    { 4728, 0xc1898148, 205 }, /* vkCmdSetDiscardRectangleModeEXT */
    { 4760, 0xe257f075, 120 }, /* vkCmdSetEvent */
    { 4774, 0x3d5620d5, 448 }, /* vkCmdSetEvent2 */
    { 4789, 0xa3c714b8, 449 }, /* vkCmdSetEvent2KHR */
    { 4807, 0x938290a6, 278 }, /* vkCmdSetExclusiveScissorEnableNV */
    { 4840, 0xb2537e63, 277 }, /* vkCmdSetExclusiveScissorNV */
    { 4867, 0x22d38855, 405 }, /* vkCmdSetExtraPrimitiveOverestimationSizeEXT */
    { 4911, 0x7670296e, 444 }, /* vkCmdSetFragmentShadingRateEnumNV */
    { 4945, 0x4c696cd8, 443 }, /* vkCmdSetFragmentShadingRateKHR */
    { 4976, 0x4cd999a9, 361 }, /* vkCmdSetFrontFace */
    { 4994, 0xa7a7a090, 362 }, /* vkCmdSetFrontFaceEXT */
    { 5015, 0x82fb3a20, 410 }, /* vkCmdSetLineRasterizationModeEXT */
    { 5048, 0xbdaa62f9, 346 }, /* vkCmdSetLineStippleEXT */
    { 5071, 0x36f713ae, 411 }, /* vkCmdSetLineStippleEnableEXT */
    { 5100, 0x32282165, 85 }, /* vkCmdSetLineWidth */
    { 5118, 0x7689581f, 389 }, /* vkCmdSetLogicOpEXT */
    { 5137, 0xc1fbf774, 399 }, /* vkCmdSetLogicOpEnableEXT */
    { 5162, 0x58604abc, 384 }, /* vkCmdSetPatchControlPointsEXT */
    { 5192, 0x4eb21af9, 334 }, /* vkCmdSetPerformanceMarkerINTEL */
    { 5223, 0x30d793c7, 336 }, /* vkCmdSetPerformanceOverrideINTEL */
    { 5256, 0xc50b03a9, 335 }, /* vkCmdSetPerformanceStreamMarkerINTEL */
    { 5293, 0x966edf9, 394 }, /* vkCmdSetPolygonModeEXT */
    { 5316, 0x2b3504c0, 390 }, /* vkCmdSetPrimitiveRestartEnable */
    { 5347, 0x28d998d1, 391 }, /* vkCmdSetPrimitiveRestartEnableEXT */
    { 5381, 0xb9524b01, 363 }, /* vkCmdSetPrimitiveTopology */
    { 5407, 0x1dacaf8, 364 }, /* vkCmdSetPrimitiveTopologyEXT */
    { 5436, 0x710ab2e2, 409 }, /* vkCmdSetProvokingVertexModeEXT */
    { 5467, 0xbe15d782, 395 }, /* vkCmdSetRasterizationSamplesEXT */
    { 5499, 0xc1c028f, 403 }, /* vkCmdSetRasterizationStreamEXT */
    { 5530, 0x81319b79, 385 }, /* vkCmdSetRasterizerDiscardEnable */
    { 5562, 0x1f7bb40, 386 }, /* vkCmdSetRasterizerDiscardEnableEXT */
    { 5597, 0xd056ef9b, 318 }, /* vkCmdSetRayTracingPipelineStackSizeKHR */
    { 5636, 0xb8eb12ff, 422 }, /* vkCmdSetRepresentativeFragmentTestEnableNV */
    { 5679, 0xa9e2c72, 206 }, /* vkCmdSetSampleLocationsEXT */
    { 5706, 0x3b21c717, 407 }, /* vkCmdSetSampleLocationsEnableEXT */
    { 5739, 0xd997f166, 396 }, /* vkCmdSetSampleMaskEXT */
    { 5761, 0x48f28c7f, 84 }, /* vkCmdSetScissor */
    { 5777, 0x159097b2, 367 }, /* vkCmdSetScissorWithCount */
    { 5802, 0xf349b42f, 368 }, /* vkCmdSetScissorWithCountEXT */
    { 5830, 0x9300b169, 420 }, /* vkCmdSetShadingRateImageEnableNV */
    { 5863, 0xa8f534e2, 89 }, /* vkCmdSetStencilCompareMask */
    { 5890, 0x43020f38, 382 }, /* vkCmdSetStencilOp */
    { 5908, 0xbb885f19, 383 }, /* vkCmdSetStencilOpEXT */
    { 5929, 0x83e2b024, 91 }, /* vkCmdSetStencilReference */
    { 5954, 0x63fedc5c, 380 }, /* vkCmdSetStencilTestEnable */
    { 5980, 0x16cc6095, 381 }, /* vkCmdSetStencilTestEnableEXT */
    { 6009, 0xe7c4b134, 90 }, /* vkCmdSetStencilWriteMask */
    { 6034, 0xf2d7ac79, 392 }, /* vkCmdSetTessellationDomainOriginEXT */
    { 6070, 0x9dd954c8, 446 }, /* vkCmdSetVertexInputEXT */
    { 6093, 0x53d6c2b, 83 }, /* vkCmdSetViewport */
    { 6110, 0x54d063a4, 280 }, /* vkCmdSetViewportShadingRatePaletteNV */
    { 6147, 0x5118219b, 414 }, /* vkCmdSetViewportSwizzleNV */
    { 6173, 0xad68ff96, 413 }, /* vkCmdSetViewportWScalingEnableNV */
    { 6206, 0x60ee2453, 202 }, /* vkCmdSetViewportWScalingNV */
    { 6233, 0xbdea58a6, 365 }, /* vkCmdSetViewportWithCount */
    { 6259, 0xa3d72e5b, 366 }, /* vkCmdSetViewportWithCountEXT */
    { 6288, 0xf2c7909d, 103 }, /* vkCmdSubpassShadingHUAWEI */
    { 6314, 0x519b0602, 315 }, /* vkCmdTraceRaysIndirect2KHR */
    { 6341, 0xaf8c1f1e, 314 }, /* vkCmdTraceRaysIndirectKHR */
    { 6367, 0x5eb65f0c, 306 }, /* vkCmdTraceRaysKHR */
    { 6385, 0xe8687c49, 307 }, /* vkCmdTraceRaysNV */
    { 6402, 0xd2986b5e, 114 }, /* vkCmdUpdateBuffer */
    { 6420, 0xa195186, 106 }, /* vkCmdUpdatePipelineIndirectBufferNV */
    { 6456, 0x3b9346b3, 122 }, /* vkCmdWaitEvents */
    { 6472, 0xcd17b527, 452 }, /* vkCmdWaitEvents2 */
    { 6489, 0x8c98fdb6, 453 }, /* vkCmdWaitEvents2KHR */
    { 6509, 0x9ecb3888, 302 }, /* vkCmdWriteAccelerationStructuresPropertiesKHR */
    { 6555, 0xd2925ead, 303 }, /* vkCmdWriteAccelerationStructuresPropertiesNV */
    { 6600, 0xe277f952, 460 }, /* vkCmdWriteBufferMarker2AMD */
    { 6627, 0x447be82c, 246 }, /* vkCmdWriteBufferMarkerAMD */
    { 6653, 0xa6efed5d, 518 }, /* vkCmdWriteMicromapsPropertiesEXT */
    { 6686, 0xec4d324c, 129 }, /* vkCmdWriteTimestamp */
    { 6706, 0xcff32086, 458 }, /* vkCmdWriteTimestamp2 */
    { 6727, 0xa43a02ef, 459 }, /* vkCmdWriteTimestamp2KHR */
    { 6751, 0xbad693ed, 288 }, /* vkCompileDeferredNV */
    { 6771, 0x45e623ac, 297 }, /* vkCopyAccelerationStructureKHR */
    { 6802, 0x9726ae20, 299 }, /* vkCopyAccelerationStructureToMemoryKHR */
    { 6841, 0x95dd682, 464 }, /* vkCopyImageToImageEXT */
    { 6863, 0xbd2a2884, 463 }, /* vkCopyImageToMemoryEXT */
    { 6886, 0xf7e06558, 301 }, /* vkCopyMemoryToAccelerationStructureKHR */
    { 6925, 0x9a7023bc, 462 }, /* vkCopyMemoryToImageEXT */
    { 6948, 0xa8f9ba9b, 517 }, /* vkCopyMemoryToMicromapEXT */
    { 6974, 0x5aa2e867, 513 }, /* vkCopyMicromapEXT */
    { 6992, 0xd280f59b, 515 }, /* vkCopyMicromapToMemoryEXT */
    { 7018, 0x30dceabb, 347 }, /* vkCreateAccelerationStructureKHR */
    { 7051, 0x9dc98a12, 289 }, /* vkCreateAccelerationStructureNV */
    { 7083, 0x7d4282b9, 36 }, /* vkCreateBuffer */
    { 7098, 0xaba0b50, 497 }, /* vkCreateBufferCollectionFUCHSIA */
    { 7130, 0x925bd256, 38 }, /* vkCreateBufferView */
    { 7149, 0x820fe476, 73 }, /* vkCreateCommandPool */
    { 7169, 0xf70c85eb, 52 }, /* vkCreateComputePipelines */
    { 7194, 0x9bff3a5d, 480 }, /* vkCreateCuFunctionNVX */
    { 7216, 0xf3c09939, 479 }, /* vkCreateCuModuleNVX */
    { 7236, 0x3f5d1a36, 352 }, /* vkCreateDeferredOperationKHR */
    { 7265, 0xfb95a8a4, 61 }, /* vkCreateDescriptorPool */
    { 7288, 0x3c14cc74, 59 }, /* vkCreateDescriptorSetLayout */
    { 7316, 0xad3ce733, 191 }, /* vkCreateDescriptorUpdateTemplate */
    { 7349, 0x5189488a, 192 }, /* vkCreateDescriptorUpdateTemplateKHR */
    { 7385, 0xe7188731, 26 }, /* vkCreateEvent */
    { 7399, 0x958af968, 19 }, /* vkCreateFence */
    { 7413, 0x887a38c4, 67 }, /* vkCreateFramebuffer */
    { 7433, 0x4b59f96d, 51 }, /* vkCreateGraphicsPipelines */
    { 7459, 0x652128c2, 40 }, /* vkCreateImage */
    { 7473, 0xdce077ff, 43 }, /* vkCreateImageView */
    { 7491, 0x3bc09b11, 152 }, /* vkCreateIndirectCommandsLayoutNV */
    { 7524, 0x4dea1af0, 508 }, /* vkCreateMicromapEXT */
    { 7544, 0x7a80ce69, 530 }, /* vkCreateOpticalFlowSessionNV */
    { 7573, 0xcbf6489f, 47 }, /* vkCreatePipelineCache */
    { 7595, 0x451ef1ed, 55 }, /* vkCreatePipelineLayout */
    { 7618, 0xd1450d02, 423 }, /* vkCreatePrivateDataSlot */
    { 7642, 0xc06d475f, 424 }, /* vkCreatePrivateDataSlotEXT */
    { 7669, 0x5edcd92b, 31 }, /* vkCreateQueryPool */
    { 7687, 0x28847755, 313 }, /* vkCreateRayTracingPipelinesKHR */
    { 7718, 0x12bca48, 312 }, /* vkCreateRayTracingPipelinesNV */
    { 7748, 0x109a9c18, 69 }, /* vkCreateRenderPass */
    { 7767, 0x46b16d5a, 247 }, /* vkCreateRenderPass2 */
    { 7787, 0xfa16043b, 248 }, /* vkCreateRenderPass2KHR */
    { 7810, 0x13cf03f, 57 }, /* vkCreateSampler */
    { 7826, 0xe6a58c26, 219 }, /* vkCreateSamplerYcbcrConversion */
    { 7857, 0x7482104f, 220 }, /* vkCreateSamplerYcbcrConversionKHR */
    { 7891, 0xf2065e5b, 24 }, /* vkCreateSemaphore */
    { 7909, 0xa0d3cea2, 45 }, /* vkCreateShaderModule */
    { 7930, 0x5d0a2b04, 540 }, /* vkCreateShadersEXT */
    { 7949, 0x47655c4a, 136 }, /* vkCreateSharedSwapchainsKHR */
    { 7977, 0xcdefcaa8, 137 }, /* vkCreateSwapchainKHR */
    { 7998, 0x591d7ed9, 224 }, /* vkCreateValidationCacheEXT */
    { 8025, 0xcddb2969, 466 }, /* vkCreateVideoSessionKHR */
    { 8049, 0x83987bd7, 468 }, /* vkCreateVideoSessionParametersKHR */
    { 8083, 0xe206fb25, 142 }, /* vkDebugMarkerSetObjectNameEXT */
    { 8113, 0x30799448, 143 }, /* vkDebugMarkerSetObjectTagEXT */
    { 8142, 0x8c8648b8, 356 }, /* vkDeferredOperationJoinKHR */
    { 8169, 0x3eccc207, 291 }, /* vkDestroyAccelerationStructureKHR */
    { 8203, 0x693f9d26, 292 }, /* vkDestroyAccelerationStructureNV */
    { 8236, 0x94a07a45, 37 }, /* vkDestroyBuffer */
    { 8252, 0xd6301e64, 500 }, /* vkDestroyBufferCollectionFUCHSIA */
    { 8285, 0x98b27962, 39 }, /* vkDestroyBufferView */
    { 8305, 0xd5d83a0a, 74 }, /* vkDestroyCommandPool */
    { 8326, 0x111b9831, 482 }, /* vkDestroyCuFunctionNVX */
    { 8349, 0x4788eecd, 481 }, /* vkDestroyCuModuleNVX */
    { 8370, 0x7d549a02, 353 }, /* vkDestroyDeferredOperationKHR */
    { 8400, 0x47bdaf30, 62 }, /* vkDestroyDescriptorPool */
    { 8424, 0xa4227b08, 60 }, /* vkDestroyDescriptorSetLayout */
    { 8453, 0xbb2cbe7f, 193 }, /* vkDestroyDescriptorUpdateTemplate */
    { 8487, 0xaa83901e, 194 }, /* vkDestroyDescriptorUpdateTemplateKHR */
    { 8524, 0x1fbcc9cb, 1 }, /* vkDestroyDevice */
    { 8540, 0x4df27c05, 27 }, /* vkDestroyEvent */
    { 8555, 0xfc64ee3c, 20 }, /* vkDestroyFence */
    { 8570, 0xdc428e58, 68 }, /* vkDestroyFramebuffer */
    { 8591, 0xcbfb1d96, 41 }, /* vkDestroyImage */
    { 8606, 0xb5853953, 44 }, /* vkDestroyImageView */
    { 8625, 0x49b0725d, 153 }, /* vkDestroyIndirectCommandsLayoutNV */
    { 8659, 0xa1b27084, 511 }, /* vkDestroyMicromapEXT */
    { 8680, 0xb8784e35, 531 }, /* vkDestroyOpticalFlowSessionNV */
    { 8710, 0x6aac68af, 54 }, /* vkDestroyPipeline */
    { 8728, 0x4112a673, 48 }, /* vkDestroyPipelineCache */
    { 8751, 0x9146f879, 56 }, /* vkDestroyPipelineLayout */
    { 8775, 0x9fc42716, 425 }, /* vkDestroyPrivateDataSlot */
    { 8800, 0xe18d5d6b, 426 }, /* vkDestroyPrivateDataSlotEXT */
    { 8828, 0x37819a7f, 32 }, /* vkDestroyQueryPool */
    { 8847, 0x16f14324, 70 }, /* vkDestroyRenderPass */
    { 8867, 0x3b645153, 58 }, /* vkDestroySampler */
    { 8884, 0x20f261b2, 221 }, /* vkDestroySamplerYcbcrConversion */
    { 8916, 0xaaa623a3, 222 }, /* vkDestroySamplerYcbcrConversionKHR */
    { 8951, 0xcaab1faf, 25 }, /* vkDestroySemaphore */
    { 8970, 0x9def5f27, 541 }, /* vkDestroyShaderEXT */
    { 8989, 0x2d77af6e, 46 }, /* vkDestroyShaderModule */
    { 9011, 0x5a93ab74, 138 }, /* vkDestroySwapchainKHR */
    { 9033, 0x7a3d94e5, 225 }, /* vkDestroyValidationCacheEXT */
    { 9061, 0x9c5a437d, 467 }, /* vkDestroyVideoSessionKHR */
    { 9086, 0xb9bc8f2b, 470 }, /* vkDestroyVideoSessionParametersKHR */
    { 9121, 0xd46c5f24, 5 }, /* vkDeviceWaitIdle */
    { 9138, 0xdbb064, 174 }, /* vkDisplayPowerControlEXT */
    { 9163, 0xaffb5725, 79 }, /* vkEndCommandBuffer */
    { 9182, 0xa59ad883, 527 }, /* vkExportMetalObjectsEXT */
    { 9206, 0xff52f051, 10 }, /* vkFlushMappedMemoryRanges */
    { 9232, 0xb9db2b91, 77 }, /* vkFreeCommandBuffers */
    { 9253, 0x7a1347b1, 65 }, /* vkFreeDescriptorSets */
    { 9274, 0x8f6f838a, 7 }, /* vkFreeMemory */
    { 9287, 0x4dbe8d2f, 445 }, /* vkGetAccelerationStructureBuildSizesKHR */
    { 9327, 0x1a50de81, 351 }, /* vkGetAccelerationStructureDeviceAddressKHR */
    { 9370, 0xd26f255a, 311 }, /* vkGetAccelerationStructureHandleNV */
    { 9405, 0x5d79203, 293 }, /* vkGetAccelerationStructureMemoryRequirementsNV */
    { 9452, 0x2c8ad5, 494 }, /* vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT */
    { 9509, 0xb891b5e, 261 }, /* vkGetAndroidHardwareBufferPropertiesANDROID */
    { 9553, 0x14b1e93d, 501 }, /* vkGetBufferCollectionPropertiesFUCHSIA */
    { 9592, 0x7022f0cd, 329 }, /* vkGetBufferDeviceAddress */
    { 9617, 0x3703280c, 331 }, /* vkGetBufferDeviceAddressEXT */
    { 9645, 0x713b5180, 330 }, /* vkGetBufferDeviceAddressKHR */
    { 9673, 0xab98422a, 13 }, /* vkGetBufferMemoryRequirements */
    { 9703, 0xd1fd0638, 207 }, /* vkGetBufferMemoryRequirements2 */
    { 9734, 0x78dbe98d, 208 }, /* vkGetBufferMemoryRequirements2KHR */
    { 9768, 0x2a5545a0, 327 }, /* vkGetBufferOpaqueCaptureAddress */
    { 9800, 0xddac1c65, 328 }, /* vkGetBufferOpaqueCaptureAddressKHR */
    { 9835, 0x82935b24, 490 }, /* vkGetBufferOpaqueCaptureDescriptorDataEXT */
    { 9877, 0xcf3070fe, 236 }, /* vkGetCalibratedTimestampsEXT */
    { 9906, 0x7d902967, 354 }, /* vkGetDeferredOperationMaxConcurrencyKHR */
    { 9946, 0xf2144be9, 355 }, /* vkGetDeferredOperationResultKHR */
    { 9978, 0x1e9d8271, 486 }, /* vkGetDescriptorEXT */
    { 9997, 0x19faddac, 507 }, /* vkGetDescriptorSetHostMappingVALVE */
    { 10032, 0xbb130e35, 485 }, /* vkGetDescriptorSetLayoutBindingOffsetEXT */
    { 10073, 0x5485f810, 506 }, /* vkGetDescriptorSetLayoutHostMappingInfoVALVE */
    { 10118, 0x9ea2493c, 484 }, /* vkGetDescriptorSetLayoutSizeEXT */
    { 10150, 0xfeac9573, 228 }, /* vkGetDescriptorSetLayoutSupport */
    { 10182, 0xd7e44a, 229 }, /* vkGetDescriptorSetLayoutSupportKHR */
    { 10217, 0xe86334c9, 316 }, /* vkGetDeviceAccelerationStructureCompatibilityKHR */
    { 10266, 0xec1a1918, 213 }, /* vkGetDeviceBufferMemoryRequirements */
    { 10302, 0xa56ac1ad, 214 }, /* vkGetDeviceBufferMemoryRequirementsKHR */
    { 10341, 0x8d21a400, 534 }, /* vkGetDeviceFaultInfoEXT */
    { 10365, 0x2e218c10, 178 }, /* vkGetDeviceGroupPeerMemoryFeatures */
    { 10400, 0xa3809375, 179 }, /* vkGetDeviceGroupPeerMemoryFeaturesKHR */
    { 10438, 0xf72c87d4, 186 }, /* vkGetDeviceGroupPresentCapabilitiesKHR */
    { 10477, 0x41b28e81, 321 }, /* vkGetDeviceGroupSurfacePresentModes2EXT */
    { 10517, 0x6b9448c3, 187 }, /* vkGetDeviceGroupSurfacePresentModesKHR */
    { 10556, 0x5f18b6e5, 215 }, /* vkGetDeviceImageMemoryRequirements */
    { 10591, 0x3a2c5528, 216 }, /* vkGetDeviceImageMemoryRequirementsKHR */
    { 10629, 0x1ac18abd, 217 }, /* vkGetDeviceImageSparseMemoryRequirements */
    { 10670, 0xb8906110, 218 }, /* vkGetDeviceImageSparseMemoryRequirementsKHR */
    { 10714, 0x57561f11, 537 }, /* vkGetDeviceImageSubresourceLayoutKHR */
    { 10751, 0x46e38db5, 12 }, /* vkGetDeviceMemoryCommitment */
    { 10779, 0x9a0fe777, 341 }, /* vkGetDeviceMemoryOpaqueCaptureAddress */
    { 10817, 0x49339be6, 342 }, /* vkGetDeviceMemoryOpaqueCaptureAddressKHR */
    { 10858, 0x31e1f1f0, 520 }, /* vkGetDeviceMicromapCompatibilityEXT */
    { 10894, 0xba013486, 0 }, /* vkGetDeviceProcAddr */
    { 10914, 0xcc920d9a, 2 }, /* vkGetDeviceQueue */
    { 10931, 0xb11a6348, 223 }, /* vkGetDeviceQueue2 */
    { 10949, 0x9d280cca, 53 }, /* vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI */
    { 10997, 0x4711995f, 529 }, /* vkGetDynamicRenderingTilePropertiesQCOM */
    { 11037, 0x96d834b, 28 }, /* vkGetEventStatus */
    { 11054, 0x69a5d6af, 172 }, /* vkGetFenceFdKHR */
    { 11070, 0x5f391892, 22 }, /* vkGetFenceStatus */
    { 11087, 0x8963de2, 170 }, /* vkGetFenceWin32HandleKHR */
    { 11112, 0xb332dbcd, 528 }, /* vkGetFramebufferTilePropertiesQCOM */
    { 11147, 0xac420aaf, 151 }, /* vkGetGeneratedCommandsMemoryRequirementsNV */
    { 11190, 0x12fa78a3, 326 }, /* vkGetImageDrmFormatModifierPropertiesEXT */
    { 11231, 0x916f1e63, 15 }, /* vkGetImageMemoryRequirements */
    { 11260, 0x56e213f7, 209 }, /* vkGetImageMemoryRequirements2 */
    { 11290, 0x8de28366, 210 }, /* vkGetImageMemoryRequirements2KHR */
    { 11323, 0x881c3c3d, 491 }, /* vkGetImageOpaqueCaptureDescriptorDataEXT */
    { 11364, 0x15855f5b, 17 }, /* vkGetImageSparseMemoryRequirements */
    { 11399, 0xbd4e3d3f, 211 }, /* vkGetImageSparseMemoryRequirements2 */
    { 11435, 0x3df40f5e, 212 }, /* vkGetImageSparseMemoryRequirements2KHR */
    { 11474, 0x9163b686, 42 }, /* vkGetImageSubresourceLayout */
    { 11502, 0x1a0c05f5, 525 }, /* vkGetImageSubresourceLayout2EXT */
    { 11534, 0x54442f69, 524 }, /* vkGetImageSubresourceLayout2KHR */
    { 11566, 0xed8f1d33, 320 }, /* vkGetImageViewAddressNVX */
    { 11591, 0x20caa1e1, 319 }, /* vkGetImageViewHandleNVX */
    { 11615, 0xe2c45ea, 492 }, /* vkGetImageViewOpaqueCaptureDescriptorDataEXT */
    { 11660, 0x1595f564, 548 }, /* vkGetLatencyTimingsNV */
    { 11682, 0x71220e82, 262 }, /* vkGetMemoryAndroidHardwareBufferANDROID */
    { 11722, 0x503c14c5, 159 }, /* vkGetMemoryFdKHR */
    { 11739, 0xb028a792, 160 }, /* vkGetMemoryFdPropertiesKHR */
    { 11766, 0x7030ee5b, 245 }, /* vkGetMemoryHostPointerPropertiesEXT */
    { 11802, 0x93d6c7a4, 163 }, /* vkGetMemoryRemoteAddressNV */
    { 11829, 0x45fc7e1c, 157 }, /* vkGetMemoryWin32HandleKHR */
    { 11855, 0xc8795b9, 147 }, /* vkGetMemoryWin32HandleNV */
    { 11880, 0xb8f59859, 158 }, /* vkGetMemoryWin32HandlePropertiesKHR */
    { 11916, 0x4540b38e, 161 }, /* vkGetMemoryZirconHandleFUCHSIA */
    { 11947, 0x5a4149eb, 162 }, /* vkGetMemoryZirconHandlePropertiesFUCHSIA */
    { 11988, 0x715aea54, 521 }, /* vkGetMicromapBuildSizesEXT */
    { 12015, 0x19616a98, 201 }, /* vkGetPastPresentationTimingGOOGLE */
    { 12049, 0x1ec6c4ec, 340 }, /* vkGetPerformanceParameterINTEL */
    { 12080, 0x2092a349, 49 }, /* vkGetPipelineCacheData */
    { 12103, 0x8b20fc09, 345 }, /* vkGetPipelineExecutableInternalRepresentationsKHR */
    { 12153, 0x748dd8cd, 343 }, /* vkGetPipelineExecutablePropertiesKHR */
    { 12190, 0x5c4d6435, 344 }, /* vkGetPipelineExecutableStatisticsKHR */
    { 12227, 0x4b0a7b6d, 358 }, /* vkGetPipelineIndirectDeviceAddressNV */
    { 12264, 0x617eabaa, 357 }, /* vkGetPipelineIndirectMemoryRequirementsNV */
    { 12306, 0xd93861f3, 526 }, /* vkGetPipelinePropertiesEXT */
    { 12333, 0xa60eca94, 429 }, /* vkGetPrivateData */
    { 12350, 0x2dc1491d, 430 }, /* vkGetPrivateDataEXT */
    { 12370, 0xbf3f2cb3, 33 }, /* vkGetQueryPoolResults */
    { 12392, 0xf6cef020, 461 }, /* vkGetQueueCheckpointData2NV */
    { 12420, 0x428d4692, 270 }, /* vkGetQueueCheckpointDataNV */
    { 12447, 0x4b32ff8, 310 }, /* vkGetRayTracingCaptureReplayShaderGroupHandlesKHR */
    { 12497, 0x4693e853, 308 }, /* vkGetRayTracingShaderGroupHandlesKHR */
    { 12534, 0x3b54d93a, 309 }, /* vkGetRayTracingShaderGroupHandlesNV */
    { 12570, 0x2f709815, 317 }, /* vkGetRayTracingShaderGroupStackSizeKHR */
    { 12609, 0x85a9d101, 200 }, /* vkGetRefreshCycleDurationGOOGLE */
    { 12641, 0xa9820d22, 71 }, /* vkGetRenderAreaGranularity */
    { 12668, 0xf98a6b4f, 72 }, /* vkGetRenderingAreaGranularityKHR */
    { 12701, 0xf13b1f2a, 493 }, /* vkGetSamplerOpaqueCaptureDescriptorDataEXT */
    { 12744, 0xc3009b1b, 544 }, /* vkGetScreenBufferPropertiesQNX */
    { 12775, 0xd05a61a0, 255 }, /* vkGetSemaphoreCounterValue */
    { 12802, 0xf3c26065, 256 }, /* vkGetSemaphoreCounterValueKHR */
    { 12832, 0x3e0e9884, 166 }, /* vkGetSemaphoreFdKHR */
    { 12852, 0xd04be5e5, 164 }, /* vkGetSemaphoreWin32HandleKHR */
    { 12881, 0x37c0989d, 168 }, /* vkGetSemaphoreZirconHandleFUCHSIA */
    { 12915, 0x42987180, 542 }, /* vkGetShaderBinaryDataEXT */
    { 12940, 0x5330743c, 234 }, /* vkGetShaderInfoAMD */
    { 12959, 0x81d5d7e4, 523 }, /* vkGetShaderModuleCreateInfoIdentifierEXT */
    { 13000, 0xb7334436, 522 }, /* vkGetShaderModuleIdentifierEXT */
    { 13031, 0xa4aeb5a, 177 }, /* vkGetSwapchainCounterEXT */
    { 13056, 0x219d929, 231 }, /* vkGetSwapchainGrallocUsage2ANDROID */
    { 13091, 0x4979c9a3, 230 }, /* vkGetSwapchainGrallocUsageANDROID */
    { 13125, 0x57695f28, 139 }, /* vkGetSwapchainImagesKHR */
    { 13149, 0x66ae725e, 199 }, /* vkGetSwapchainStatusKHR */
    { 13173, 0xbbc9f99f, 226 }, /* vkGetValidationCacheDataEXT */
    { 13201, 0xd8960270, 471 }, /* vkGetVideoSessionMemoryRequirementsKHR */
    { 13240, 0x51df0390, 173 }, /* vkImportFenceFdKHR */
    { 13259, 0x1bcbb079, 171 }, /* vkImportFenceWin32HandleKHR */
    { 13287, 0x36337c05, 167 }, /* vkImportSemaphoreFdKHR */
    { 13310, 0x7e2cfcdc, 165 }, /* vkImportSemaphoreWin32HandleKHR */
    { 13342, 0x4d1996ce, 169 }, /* vkImportSemaphoreZirconHandleFUCHSIA */
    { 13379, 0x65a01d77, 332 }, /* vkInitializePerformanceApiINTEL */
    { 13411, 0x1e115cca, 11 }, /* vkInvalidateMappedMemoryRanges */
    { 13442, 0x2b780b06, 546 }, /* vkLatencySleepNV */
    { 13459, 0xcb977bd8, 8 }, /* vkMapMemory */
    { 13471, 0x5d243bfb, 538 }, /* vkMapMemory2KHR */
    { 13487, 0xc3499606, 50 }, /* vkMergePipelineCaches */
    { 13509, 0xe8fe1154, 227 }, /* vkMergeValidationCachesEXT */
    { 13536, 0xcb7dc88, 239 }, /* vkQueueBeginDebugUtilsLabelEXT */
    { 13567, 0xc3628a09, 18 }, /* vkQueueBindSparse */
    { 13585, 0xf130b20a, 240 }, /* vkQueueEndDebugUtilsLabelEXT */
    { 13614, 0x56027200, 241 }, /* vkQueueInsertDebugUtilsLabelEXT */
    { 13646, 0x19ae9eed, 549 }, /* vkQueueNotifyOutOfBandNV */
    { 13671, 0xfc5fb6ce, 141 }, /* vkQueuePresentKHR */
    { 13689, 0xf8499f82, 339 }, /* vkQueueSetPerformanceConfigurationINTEL */
    { 13729, 0xa0313eef, 233 }, /* vkQueueSignalReleaseImageANDROID */
    { 13762, 0xfa4713ec, 3 }, /* vkQueueSubmit */
    { 13776, 0xbf0609e6, 456 }, /* vkQueueSubmit2 */
    { 13791, 0xc2dd288f, 457 }, /* vkQueueSubmit2KHR */
    { 13809, 0x6f8fc2a5, 4 }, /* vkQueueWaitIdle */
    { 13825, 0x26cc78f5, 175 }, /* vkRegisterDeviceEventEXT */
    { 13850, 0x4a0bd849, 176 }, /* vkRegisterDisplayEventEXT */
    { 13876, 0x13814325, 323 }, /* vkReleaseFullScreenExclusiveModeEXT */
    { 13912, 0x28575036, 338 }, /* vkReleasePerformanceConfigurationINTEL */
    { 13951, 0x8bdecb76, 325 }, /* vkReleaseProfilingLockKHR */
    { 13977, 0xf217e5, 536 }, /* vkReleaseSwapchainImagesEXT */
    { 14005, 0x847dc731, 80 }, /* vkResetCommandBuffer */
    { 14026, 0x6da9f7fd, 75 }, /* vkResetCommandPool */
    { 14045, 0x9bd85f5, 63 }, /* vkResetDescriptorPool */
    { 14067, 0x6d373ba8, 30 }, /* vkResetEvent */
    { 14080, 0x684781dc, 21 }, /* vkResetFences */
    { 14094, 0x4e671e02, 34 }, /* vkResetQueryPool */
    { 14111, 0xe6701e5f, 35 }, /* vkResetQueryPoolEXT */
    { 14131, 0x3c97f0dc, 498 }, /* vkSetBufferCollectionBufferConstraintsFUCHSIA */
    { 14177, 0xcae02471, 499 }, /* vkSetBufferCollectionImageConstraintsFUCHSIA */
    { 14222, 0x180cec44, 237 }, /* vkSetDebugUtilsObjectNameEXT */
    { 14251, 0x15942821, 238 }, /* vkSetDebugUtilsObjectTagEXT */
    { 14279, 0xa42f1309, 495 }, /* vkSetDeviceMemoryPriorityEXT */
    { 14308, 0x592ae5f5, 29 }, /* vkSetEvent */
    { 14319, 0xa20f1ea9, 198 }, /* vkSetHdrMetadataEXT */
    { 14339, 0xfba7b4a3, 547 }, /* vkSetLatencyMarkerNV */
    { 14360, 0x4adbd599, 545 }, /* vkSetLatencySleepModeNV */
    { 14384, 0xbd1cd781, 235 }, /* vkSetLocalDimmingAMD */
    { 14405, 0x5cf49ca8, 427 }, /* vkSetPrivateData */
    { 14422, 0x23456729, 428 }, /* vkSetPrivateDataEXT */
    { 14442, 0xcd347297, 259 }, /* vkSignalSemaphore */
    { 14460, 0x8fef55c6, 260 }, /* vkSignalSemaphoreKHR */
    { 14481, 0x50997986, 465 }, /* vkTransitionImageLayoutEXT */
    { 14508, 0xfef2fb38, 155 }, /* vkTrimCommandPool */
    { 14526, 0x51177c8d, 156 }, /* vkTrimCommandPoolKHR */
    { 14547, 0x408975ae, 333 }, /* vkUninitializePerformanceApiINTEL */
    { 14581, 0x1a1a0e2f, 9 }, /* vkUnmapMemory */
    { 14595, 0xfb0bcab2, 539 }, /* vkUnmapMemory2KHR */
    { 14613, 0x5349c9d, 195 }, /* vkUpdateDescriptorSetWithTemplate */
    { 14647, 0x214ad230, 196 }, /* vkUpdateDescriptorSetWithTemplateKHR */
    { 14684, 0xbfd090ae, 66 }, /* vkUpdateDescriptorSets */
    { 14707, 0x8e570a3a, 469 }, /* vkUpdateVideoSessionParametersKHR */
    { 14741, 0x19d64c81, 23 }, /* vkWaitForFences */
    { 14757, 0x19c136b1, 496 }, /* vkWaitForPresentKHR */
    { 14777, 0x74368ad9, 257 }, /* vkWaitSemaphores */
    { 14794, 0x2bc77454, 258 }, /* vkWaitSemaphoresKHR */
    { 14814, 0x8bc9ae08, 305 }, /* vkWriteAccelerationStructuresPropertiesKHR */
    { 14857, 0xc43df3dd, 519 }, /* vkWriteMicromapsPropertiesEXT */
};

/* Hash table stats:
 * size 550 entries
 * collisions entries:
 *     0      410
 *     1      76
 *     2      28
 *     3      14
 *     4      7
 *     5      1
 *     6      7
 *     7      1
 *     8      4
 *     9+     2
 */

#define none 0xffff
static const uint16_t device_string_map[1024] = {
    0x0188,
    none,
    none,
    none,
    none,
    0x0059,
    none,
    0x00a0,
    none,
    0x004f,
    0x0031,
    0x0200,
    0x0173,
    none,
    none,
    none,
    0x0181,
    none,
    none,
    none,
    none,
    0x01cd,
    none,
    none,
    0x012b,
    none,
    none,
    0x004b,
    0x01bf,
    none,
    0x0148,
    0x00c8,
    0x01c8,
    0x002f,
    none,
    0x0189,
    0x00e4,
    none,
    0x0046,
    none,
    none,
    none,
    none,
    0x00ea,
    0x00fe,
    none,
    none,
    0x00df,
    none,
    0x0142,
    none,
    none,
    0x009c,
    0x01c1,
    0x01db,
    none,
    none,
    0x012f,
    none,
    0x012d,
    0x01d9,
    0x01a6,
    0x014a,
    0x012e,
    0x0052,
    none,
    0x00a2,
    0x004a,
    0x0040,
    none,
    none,
    0x020b,
    0x013a,
    0x00f5,
    0x0090,
    0x008b,
    0x0030,
    none,
    none,
    0x0047,
    none,
    0x0167,
    none,
    0x00ee,
    0x0223,
    0x00bf,
    none,
    0x020a,
    0x005d,
    0x01b8,
    none,
    none,
    0x00e5,
    0x0134,
    none,
    none,
    none,
    0x0041,
    0x0130,
    none,
    0x0164,
    0x0179,
    0x01cb,
    0x0056,
    0x0086,
    none,
    none,
    none,
    0x0044,
    none,
    none,
    0x01fc,
    0x0023,
    0x0209,
    0x00da,
    none,
    0x00ab,
    0x00ba,
    0x0112,
    0x01e5,
    0x01d4,
    0x00e8,
    0x010b,
    none,
    none,
    none,
    0x003f,
    0x00dd,
    none,
    0x0220,
    0x008c,
    0x0166,
    0x0107,
    none,
    0x0101,
    0x0118,
    0x00fb,
    0x0063,
    0x0072,
    0x00b7,
    0x0154,
    0x0218,
    none,
    0x01f9,
    0x00c3,
    none,
    0x019e,
    none,
    0x001e,
    0x00e6,
    0x0184,
    0x0150,
    none,
    0x0197,
    none,
    0x01ef,
    none,
    0x00f1,
    none,
    0x0124,
    0x003b,
    none,
    none,
    0x01a2,
    0x0117,
    none,
    0x00bd,
    none,
    0x01fe,
    none,
    0x01d5,
    none,
    none,
    0x0005,
    0x021e,
    0x0152,
    0x011a,
    none,
    0x01e4,
    0x006a,
    none,
    none,
    0x020f,
    none,
    0x0035,
    0x00ef,
    0x0037,
    0x0097,
    none,
    none,
    none,
    none,
    0x00cf,
    none,
    0x011f,
    0x018d,
    0x011d,
    0x01b2,
    0x01c7,
    none,
    0x00e9,
    0x0185,
    0x019a,
    0x00bc,
    none,
    0x0172,
    0x0212,
    none,
    none,
    0x00d0,
    none,
    none,
    none,
    0x00bb,
    0x0027,
    none,
    0x00c1,
    none,
    0x003c,
    none,
    0x01e6,
    0x0001,
    0x013b,
    none,
    0x00a9,
    none,
    0x00e1,
    0x0019,
    none,
    0x0160,
    none,
    none,
    none,
    0x01d7,
    none,
    none,
    0x01bd,
    none,
    none,
    0x0208,
    0x01e9,
    none,
    none,
    0x01c0,
    none,
    0x01fb,
    none,
    none,
    none,
    none,
    none,
    none,
    0x021c,
    none,
    0x008e,
    none,
    none,
    0x01ce,
    0x0126,
    none,
    none,
    0x00b3,
    0x005c,
    0x0034,
    0x0036,
    none,
    none,
    none,
    none,
    none,
    none,
    none,
    0x0191,
    0x0070,
    none,
    none,
    none,
    0x0074,
    none,
    none,
    0x000f,
    none,
    0x0006,
    none,
    none,
    0x01c6,
    none,
    none,
    none,
    none,
    0x0071,
    none,
    0x00ad,
    none,
    0x013d,
    0x00f9,
    0x018f,
    0x01dd,
    none,
    0x0128,
    none,
    0x00a4,
    none,
    0x016b,
    none,
    0x007f,
    none,
    0x01ad,
    0x00e7,
    0x01cf,
    0x005e,
    0x017b,
    none,
    0x0115,
    0x0007,
    none,
    0x0182,
    0x0171,
    0x0186,
    0x0016,
    none,
    none,
    none,
    none,
    0x009f,
    none,
    none,
    none,
    0x00b9,
    none,
    none,
    none,
    0x009a,
    0x01cc,
    none,
    none,
    none,
    none,
    0x00fd,
    0x014e,
    0x01ee,
    none,
    none,
    none,
    0x0108,
    none,
    0x012c,
    none,
    0x0013,
    0x00ff,
    none,
    0x019b,
    none,
    0x0067,
    0x0140,
    none,
    0x01b0,
    0x00c7,
    0x00dc,
    0x017c,
    0x011c,
    0x00e0,
    none,
    0x0156,
    none,
    0x0094,
    0x00c0,
    none,
    none,
    none,
    0x0002,
    0x0183,
    none,
    none,
    none,
    0x001a,
    0x01a8,
    0x0159,
    none,
    none,
    0x0137,
    0x006f,
    none,
    none,
    0x00ac,
    0x0088,
    none,
    none,
    0x0026,
    none,
    0x00f7,
    none,
    none,
    none,
    0x01e8,
    none,
    none,
    0x0177,
    none,
    none,
    0x0064,
    none,
    none,
    0x011e,
    0x008d,
    none,
    0x014d,
    none,
    none,
    0x0210,
    0x0198,
    0x00ec,
    none,
    none,
    none,
    0x0015,
    0x0178,
    0x005a,
    none,
    0x01de,
    none,
    none,
    0x016d,
    none,
    none,
    0x00c2,
    none,
    none,
    0x017f,
    0x0010,
    0x010c,
    none,
    none,
    0x00aa,
    0x015a,
    0x01d3,
    none,
    0x0193,
    0x00fa,
    none,
    0x001c,
    0x0174,
    none,
    none,
    0x0080,
    none,
    none,
    0x0062,
    0x0095,
    0x0219,
    none,
    none,
    none,
    0x01e1,
    0x0215,
    none,
    0x005b,
    none,
    none,
    0x000b,
    0x0000,
    0x002a,
    none,
    none,
    0x00ae,
    none,
    0x0216,
    0x0008,
    none,
    none,
    none,
    none,
    none,
    0x006d,
    0x0022,
    none,
    0x0205,
    none,
    0x0149,
    0x01b7,
    none,
    0x01ae,
    0x019f,
    0x00b0,
    0x0091,
    0x01d6,
    0x001b,
    0x0012,
    none,
    none,
    0x01af,
    0x0113,
    none,
    0x0125,
    none,
    none,
    0x0196,
    none,
    0x01d8,
    0x01c4,
    none,
    0x0066,
    none,
    none,
    none,
    0x0085,
    0x004c,
    0x0093,
    none,
    none,
    0x01ba,
    0x0077,
    0x00a1,
    none,
    0x00f2,
    0x016e,
    none,
    none,
    0x006b,
    0x013c,
    0x003d,
    0x01f0,
    0x0141,
    0x0084,
    0x00ce,
    0x001d,
    none,
    none,
    0x0068,
    none,
    0x010e,
    0x01f2,
    none,
    0x0144,
    none,
    none,
    none,
    0x01ed,
    0x000a,
    0x01ab,
    0x01b6,
    0x01f1,
    none,
    0x00a3,
    0x0057,
    none,
    none,
    none,
    none,
    none,
    none,
    none,
    0x0089,
    0x003a,
    0x0175,
    none,
    0x0018,
    none,
    0x0203,
    0x021a,
    0x021d,
    none,
    0x0187,
    0x00c4,
    none,
    0x0073,
    0x0116,
    none,
    0x0176,
    none,
    0x021f,
    0x0206,
    0x014b,
    0x003e,
    none,
    0x0039,
    0x007a,
    0x00af,
    none,
    none,
    none,
    0x0092,
    0x0105,
    none,
    0x012a,
    none,
    0x000e,
    none,
    0x0100,
    none,
    none,
    none,
    0x0065,
    none,
    0x0061,
    none,
    0x01bb,
    none,
    0x0111,
    none,
    0x013e,
    none,
    none,
    0x00f0,
    none,
    0x0114,
    0x01e0,
    0x0021,
    none,
    none,
    none,
    0x00a8,
    0x013f,
    none,
    none,
    0x020d,
    0x0054,
    0x0123,
    none,
    0x014c,
    0x00b2,
    none,
    0x0131,
    none,
    0x014f,
    0x017e,
    0x0207,
    0x0153,
    none,
    none,
    0x00be,
    none,
    0x0075,
    none,
    0x0224,
    none,
    none,
    none,
    0x01f8,
    0x0147,
    none,
    0x0151,
    0x0098,
    0x01e2,
    none,
    none,
    0x01aa,
    0x0060,
    none,
    0x01a3,
    none,
    none,
    none,
    0x0053,
    none,
    0x00d5,
    none,
    none,
    0x0157,
    none,
    0x016c,
    0x0106,
    none,
    0x0214,
    0x01bc,
    none,
    0x00b5,
    0x010a,
    none,
    none,
    none,
    none,
    none,
    0x0087,
    0x0132,
    none,
    0x0017,
    0x01c9,
    none,
    0x0180,
    0x0135,
    0x009b,
    none,
    none,
    none,
    0x00b4,
    none,
    0x019d,
    none,
    0x0221,
    0x021b,
    0x00f8,
    none,
    none,
    none,
    none,
    0x01fa,
    0x010f,
    0x018c,
    0x010d,
    0x001f,
    0x0190,
    none,
    none,
    0x00fc,
    none,
    0x01a1,
    none,
    none,
    none,
    none,
    none,
    0x006c,
    0x0055,
    0x008a,
    none,
    none,
    0x0143,
    0x01b1,
    0x00ca,
    none,
    none,
    none,
    none,
    0x004e,
    0x016f,
    none,
    0x007b,
    none,
    0x0136,
    none,
    none,
    none,
    none,
    none,
    none,
    0x007c,
    0x01e7,
    0x00d3,
    0x0020,
    none,
    0x018e,
    none,
    none,
    none,
    none,
    none,
    none,
    0x002d,
    0x002c,
    none,
    0x0102,
    0x0122,
    none,
    none,
    0x01b4,
    0x01f4,
    0x009e,
    0x0076,
    none,
    0x00d2,
    0x007e,
    none,
    none,
    none,
    none,
    none,
    0x00d9,
    0x01f3,
    0x0043,
    0x01f6,
    none,
    0x0133,
    none,
    0x0078,
    0x00a7,
    0x0146,
    0x020c,
    0x0083,
    none,
    0x00c5,
    none,
    none,
    none,
    0x0050,
    0x0121,
    0x0222,
    none,
    0x00d1,
    none,
    0x00a5,
    0x00db,
    0x009d,
    0x00e3,
    0x00a6,
    0x004d,
    none,
    0x002b,
    0x00f3,
    0x00cb,
    0x00b8,
    none,
    none,
    none,
    0x0158,
    0x0139,
    none,
    0x0009,
    0x01df,
    0x0155,
    0x01d1,
    0x0162,
    0x0081,
    none,
    0x0003,
    none,
    0x0099,
    0x011b,
    0x00f4,
    0x0119,
    none,
    none,
    none,
    0x008f,
    0x00e2,
    none,
    0x015d,
    none,
    0x0213,
    none,
    0x0051,
    0x01c5,
    0x006e,
    0x005f,
    none,
    0x0145,
    0x0201,
    none,
    none,
    none,
    0x0199,
    0x01be,
    0x0163,
    0x0165,
    none,
    none,
    0x0079,
    0x000c,
    0x0110,
    none,
    0x01ea,
    0x00d7,
    0x01d2,
    0x0129,
    0x00b6,
    none,
    none,
    none,
    0x01dc,
    0x01a7,
    none,
    0x017a,
    0x00f6,
    0x0127,
    none,
    none,
    0x01d0,
    none,
    none,
    none,
    0x01a5,
    0x0004,
    none,
    0x01ac,
    none,
    none,
    none,
    0x01c2,
    0x015e,
    none,
    0x0038,
    0x0170,
    none,
    none,
    0x00c9,
    0x018a,
    0x01ff,
    0x0194,
    none,
    0x00d6,
    0x020e,
    none,
    none,
    0x0161,
    none,
    none,
    none,
    0x0211,
    0x00d4,
    0x0192,
    0x019c,
    none,
    none,
    0x0014,
    none,
    0x002e,
    0x016a,
    none,
    none,
    none,
    0x01b9,
    none,
    0x01e3,
    0x0069,
    0x01b3,
    0x0024,
    none,
    0x01f5,
    0x00ed,
    0x01a9,
    none,
    none,
    0x015f,
    0x00d8,
    none,
    none,
    none,
    none,
    none,
    none,
    none,
    0x015b,
    0x00eb,
    none,
    none,
    none,
    0x0204,
    0x0042,
    0x01c3,
    none,
    0x0045,
    none,
    0x00c6,
    0x015c,
    none,
    0x0169,
    0x00de,
    none,
    none,
    none,
    none,
    0x0168,
    none,
    none,
    none,
    none,
    0x00cd,
    0x01fd,
    none,
    0x0104,
    none,
    none,
    none,
    none,
    none,
    none,
    none,
    0x00cc,
    none,
    0x0048,
    0x01b5,
    none,
    none,
    0x01a0,
    0x000d,
    0x0109,
    0x0217,
    0x007d,
    none,
    none,
    0x018b,
    none,
    none,
    0x0138,
    0x0049,
    none,
    none,
    none,
    0x0033,
    0x0025,
    none,
    none,
    none,
    0x0011,
    none,
    none,
    0x01da,
    0x0058,
    0x00b1,
    none,
    0x0082,
    0x017d,
    none,
    0x0029,
    0x01f7,
    0x0103,
    0x0032,
    none,
    0x0225,
    none,
    none,
    none,
    0x0096,
    0x0028,
    none,
    0x01a4,
    0x01ca,
    0x0195,
    none,
    0x01ec,
    none,
    0x0202,
    0x01eb,
    0x0120,
};

static int
device_string_map_lookup(const char *str)
{
    static const uint32_t prime_factor = 5024183;
    static const uint32_t prime_step = 19;
    const struct string_map_entry *e;
    uint32_t hash, h;
    uint16_t i;
    const char *p;

    hash = 0;
    for (p = str; *p; p++)
        hash = hash * prime_factor + *p;

    h = hash;
    while (1) {
        i = device_string_map[h & 1023];
        if (i == none)
           return -1;
        e = &device_string_map_entries[i];
        if (e->hash == hash && strcmp(str, device_strings + e->name) == 0)
            return e->num;
        h += prime_step;
    }

    return -1;
}



static const uint8_t instance_compaction_table[] = {
    0,
    1,
    2,
    3,
    4,
    5,
    6,
    7,
    8,
    9,
    10,
    11,
    12,
    13,
    14,
    15,
    16,
    17,
    18,
    19,
    20,
    21,
    22,
    22,
    23,
    24,
    25,
    26,
    27,
    28,
    29,
};


static const uint8_t physical_device_compaction_table[] = {
    0,
    1,
    2,
    3,
    4,
    5,
    6,
    7,
    8,
    9,
    10,
    11,
    12,
    13,
    14,
    15,
    16,
    17,
    18,
    19,
    20,
    21,
    22,
    23,
    24,
    25,
    26,
    27,
    27,
    28,
    28,
    29,
    29,
    30,
    30,
    31,
    31,
    32,
    32,
    33,
    33,
    34,
    34,
    35,
    35,
    36,
    36,
    37,
    38,
    39,
    40,
    41,
    42,
    43,
    44,
    45,
    46,
    47,
    48,
    49,
    50,
    51,
    52,
    53,
    54,
    55,
    56,
    57,
    57,
    58,
    59,
    60,
    61,
    62,
    63,
    64,
};


static const uint16_t device_compaction_table[] = {
    0,
    1,
    2,
    3,
    4,
    5,
    6,
    7,
    8,
    9,
    10,
    11,
    12,
    13,
    14,
    15,
    16,
    17,
    18,
    19,
    20,
    21,
    22,
    23,
    24,
    25,
    26,
    27,
    28,
    29,
    30,
    31,
    32,
    33,
    34,
    34,
    35,
    36,
    37,
    38,
    39,
    40,
    41,
    42,
    43,
    44,
    45,
    46,
    47,
    48,
    49,
    50,
    51,
    52,
    53,
    54,
    55,
    56,
    57,
    58,
    59,
    60,
    61,
    62,
    63,
    64,
    65,
    66,
    67,
    68,
    69,
    70,
    71,
    72,
    73,
    74,
    75,
    76,
    77,
    78,
    79,
    80,
    81,
    82,
    83,
    84,
    85,
    86,
    87,
    88,
    89,
    90,
    91,
    92,
    93,
    94,
    95,
    96,
    97,
    98,
    99,
    100,
    101,
    102,
    103,
    104,
    105,
    106,
    107,
    108,
    109,
    110,
    111,
    112,
    113,
    114,
    115,
    116,
    117,
    118,
    119,
    120,
    121,
    122,
    123,
    124,
    125,
    126,
    127,
    128,
    129,
    130,
    131,
    132,
    133,
    134,
    135,
    136,
    137,
    138,
    139,
    140,
    141,
    142,
    143,
    144,
    145,
    146,
    147,
    148,
    149,
    150,
    151,
    152,
    153,
    154,
    154,
    155,
    156,
    157,
    158,
    159,
    160,
    161,
    162,
    163,
    164,
    165,
    166,
    167,
    168,
    169,
    170,
    171,
    172,
    173,
    174,
    175,
    176,
    176,
    177,
    177,
    178,
    178,
    179,
    179,
    180,
    181,
    182,
    183,
    183,
    184,
    184,
    185,
    185,
    186,
    186,
    187,
    188,
    189,
    190,
    191,
    192,
    193,
    194,
    195,
    196,
    197,
    197,
    198,
    198,
    199,
    199,
    200,
    200,
    201,
    201,
    202,
    202,
    203,
    203,
    204,
    204,
    205,
    206,
    207,
    208,
    209,
    210,
    210,
    211,
    212,
    213,
    214,
    215,
    216,
    217,
    218,
    219,
    220,
    221,
    222,
    223,
    224,
    225,
    226,
    227,
    228,
    228,
    229,
    229,
    230,
    230,
    231,
    231,
    232,
    232,
    233,
    233,
    234,
    234,
    235,
    236,
    237,
    237,
    237,
    238,
    238,
    238,
    239,
    240,
    241,
    242,
    243,
    244,
    245,
    246,
    247,
    248,
    249,
    250,
    251,
    252,
    253,
    254,
    255,
    256,
    257,
    258,
    259,
    260,
    261,
    262,
    263,
    264,
    265,
    266,
    267,
    268,
    269,
    270,
    271,
    272,
    273,
    274,
    275,
    276,
    277,
    278,
    278,
    279,
    280,
    281,
    282,
    283,
    284,
    285,
    286,
    287,
    288,
    289,
    290,
    291,
    292,
    293,
    294,
    295,
    296,
    296,
    297,
    297,
    297,
    298,
    299,
    300,
    301,
    302,
    303,
    304,
    305,
    306,
    307,
    307,
    308,
    309,
    310,
    311,
    312,
    313,
    314,
    315,
    316,
    317,
    318,
    319,
    320,
    321,
    322,
    323,
    324,
    324,
    325,
    325,
    326,
    326,
    327,
    327,
    328,
    328,
    329,
    330,
    330,
    331,
    331,
    332,
    332,
    333,
    333,
    334,
    334,
    335,
    335,
    336,
    336,
    337,
    338,
    338,
    339,
    339,
    340,
    341,
    341,
    342,
    343,
    344,
    345,
    346,
    347,
    348,
    349,
    350,
    351,
    352,
    353,
    354,
    355,
    356,
    357,
    358,
    359,
    360,
    361,
    362,
    363,
    364,
    365,
    366,
    367,
    368,
    369,
    370,
    371,
    372,
    373,
    373,
    374,
    374,
    375,
    375,
    376,
    376,
    377,
    377,
    378,
    378,
    379,
    379,
    380,
    380,
    381,
    381,
    382,
    382,
    383,
    384,
    385,
    386,
    387,
    388,
    388,
    389,
    389,
    390,
    390,
    391,
    391,
    392,
    392,
    393,
    393,
    394,
    395,
    396,
    397,
    398,
    399,
    400,
    401,
    402,
    403,
    404,
    405,
    406,
    407,
    408,
    409,
    410,
    411,
    412,
    413,
    414,
    415,
    416,
    417,
    418,
    419,
    420,
    421,
    422,
    423,
    424,
    425,
    426,
    427,
    428,
    429,
    430,
    431,
    432,
    433,
    434,
    435,
    436,
    436,
    437,
    437,
    438,
    439,
    440,
    441,
    442,
    443,
    444,
    445,
    446,
    447,
    448,
    449,
    450,
    451,
    452,
    453,
    454,
    455,
    456,
    456,
    457,
    458,
    459,
    460,
    461,
    462,
    463,
    464,
    465,
    466,
    467,
    468,
    469,
    470,
    471,
    472,
    473,
    474,
    475,
    476,
    477,
    478,
    479,
    480,
};

static bool
vk_instance_entrypoint_is_enabled(int index, uint32_t core_version,
                                  const struct vk_instance_extension_table *instance)
{
   switch (index) {
   case 0:
      /* CreateInstance */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 1:
      /* DestroyInstance */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 2:
      /* EnumeratePhysicalDevices */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 3:
      /* GetInstanceProcAddr */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 4:
      /* EnumerateInstanceVersion */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 5:
      /* EnumerateInstanceLayerProperties */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 6:
      /* EnumerateInstanceExtensionProperties */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 7:
      /* CreateAndroidSurfaceKHR */
      if (instance->KHR_android_surface) return true;
      return false;
   case 8:
      /* CreateDisplayPlaneSurfaceKHR */
      if (instance->KHR_display) return true;
      return false;
   case 9:
      /* DestroySurfaceKHR */
      if (instance->KHR_surface) return true;
      return false;
   case 10:
      /* CreateViSurfaceNN */
      if (instance->NN_vi_surface) return true;
      return false;
   case 11:
      /* CreateWaylandSurfaceKHR */
      if (instance->KHR_wayland_surface) return true;
      return false;
   case 12:
      /* CreateWin32SurfaceKHR */
      if (instance->KHR_win32_surface) return true;
      return false;
   case 13:
      /* CreateXlibSurfaceKHR */
      if (instance->KHR_xlib_surface) return true;
      return false;
   case 14:
      /* CreateXcbSurfaceKHR */
      if (instance->KHR_xcb_surface) return true;
      return false;
   case 15:
      /* CreateDirectFBSurfaceEXT */
      if (instance->EXT_directfb_surface) return true;
      return false;
   case 16:
      /* CreateImagePipeSurfaceFUCHSIA */
      if (instance->FUCHSIA_imagepipe_surface) return true;
      return false;
   case 17:
      /* CreateStreamDescriptorSurfaceGGP */
      if (instance->GGP_stream_descriptor_surface) return true;
      return false;
   case 18:
      /* CreateScreenSurfaceQNX */
      if (instance->QNX_screen_surface) return true;
      return false;
   case 19:
      /* CreateDebugReportCallbackEXT */
      if (instance->EXT_debug_report) return true;
      return false;
   case 20:
      /* DestroyDebugReportCallbackEXT */
      if (instance->EXT_debug_report) return true;
      return false;
   case 21:
      /* DebugReportMessageEXT */
      if (instance->EXT_debug_report) return true;
      return false;
   case 22:
      /* EnumeratePhysicalDeviceGroups */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 23:
      /* EnumeratePhysicalDeviceGroupsKHR */
      if (instance->KHR_device_group_creation) return true;
      return false;
   case 24:
      /* CreateIOSSurfaceMVK */
      if (instance->MVK_ios_surface) return true;
      return false;
   case 25:
      /* CreateMacOSSurfaceMVK */
      if (instance->MVK_macos_surface) return true;
      return false;
   case 26:
      /* CreateMetalSurfaceEXT */
      if (instance->EXT_metal_surface) return true;
      return false;
   case 27:
      /* CreateDebugUtilsMessengerEXT */
      if (instance->EXT_debug_utils) return true;
      return false;
   case 28:
      /* DestroyDebugUtilsMessengerEXT */
      if (instance->EXT_debug_utils) return true;
      return false;
   case 29:
      /* SubmitDebugUtilsMessageEXT */
      if (instance->EXT_debug_utils) return true;
      return false;
   case 30:
      /* CreateHeadlessSurfaceEXT */
      if (instance->EXT_headless_surface) return true;
      return false;
   default:
      return false;
   }
}

/** Return true if the core version or extension in which the given entrypoint
 * is defined is enabled.
 *
 * If device is NULL, all device extensions are considered enabled.
 */
static bool
vk_physical_device_entrypoint_is_enabled(int index, uint32_t core_version,
                                         const struct vk_instance_extension_table *instance)
{
   switch (index) {
   case 0:
      /* GetPhysicalDeviceProperties */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 1:
      /* GetPhysicalDeviceQueueFamilyProperties */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 2:
      /* GetPhysicalDeviceMemoryProperties */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 3:
      /* GetPhysicalDeviceFeatures */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 4:
      /* GetPhysicalDeviceFormatProperties */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 5:
      /* GetPhysicalDeviceImageFormatProperties */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 6:
      /* CreateDevice */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 7:
      /* EnumerateDeviceLayerProperties */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 8:
      /* EnumerateDeviceExtensionProperties */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 9:
      /* GetPhysicalDeviceSparseImageFormatProperties */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 10:
      /* GetPhysicalDeviceDisplayPropertiesKHR */
      if (instance->KHR_display) return true;
      return false;
   case 11:
      /* GetPhysicalDeviceDisplayPlanePropertiesKHR */
      if (instance->KHR_display) return true;
      return false;
   case 12:
      /* GetDisplayPlaneSupportedDisplaysKHR */
      if (instance->KHR_display) return true;
      return false;
   case 13:
      /* GetDisplayModePropertiesKHR */
      if (instance->KHR_display) return true;
      return false;
   case 14:
      /* CreateDisplayModeKHR */
      if (instance->KHR_display) return true;
      return false;
   case 15:
      /* GetDisplayPlaneCapabilitiesKHR */
      if (instance->KHR_display) return true;
      return false;
   case 16:
      /* GetPhysicalDeviceSurfaceSupportKHR */
      if (instance->KHR_surface) return true;
      return false;
   case 17:
      /* GetPhysicalDeviceSurfaceCapabilitiesKHR */
      if (instance->KHR_surface) return true;
      return false;
   case 18:
      /* GetPhysicalDeviceSurfaceFormatsKHR */
      if (instance->KHR_surface) return true;
      return false;
   case 19:
      /* GetPhysicalDeviceSurfacePresentModesKHR */
      if (instance->KHR_surface) return true;
      return false;
   case 20:
      /* GetPhysicalDeviceWaylandPresentationSupportKHR */
      if (instance->KHR_wayland_surface) return true;
      return false;
   case 21:
      /* GetPhysicalDeviceWin32PresentationSupportKHR */
      if (instance->KHR_win32_surface) return true;
      return false;
   case 22:
      /* GetPhysicalDeviceXlibPresentationSupportKHR */
      if (instance->KHR_xlib_surface) return true;
      return false;
   case 23:
      /* GetPhysicalDeviceXcbPresentationSupportKHR */
      if (instance->KHR_xcb_surface) return true;
      return false;
   case 24:
      /* GetPhysicalDeviceDirectFBPresentationSupportEXT */
      if (instance->EXT_directfb_surface) return true;
      return false;
   case 25:
      /* GetPhysicalDeviceScreenPresentationSupportQNX */
      if (instance->QNX_screen_surface) return true;
      return false;
   case 26:
      /* GetPhysicalDeviceExternalImageFormatPropertiesNV */
      if (instance->NV_external_memory_capabilities) return true;
      return false;
   case 27:
      /* GetPhysicalDeviceFeatures2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 28:
      /* GetPhysicalDeviceFeatures2KHR */
      if (instance->KHR_get_physical_device_properties2) return true;
      return false;
   case 29:
      /* GetPhysicalDeviceProperties2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 30:
      /* GetPhysicalDeviceProperties2KHR */
      if (instance->KHR_get_physical_device_properties2) return true;
      return false;
   case 31:
      /* GetPhysicalDeviceFormatProperties2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 32:
      /* GetPhysicalDeviceFormatProperties2KHR */
      if (instance->KHR_get_physical_device_properties2) return true;
      return false;
   case 33:
      /* GetPhysicalDeviceImageFormatProperties2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 34:
      /* GetPhysicalDeviceImageFormatProperties2KHR */
      if (instance->KHR_get_physical_device_properties2) return true;
      return false;
   case 35:
      /* GetPhysicalDeviceQueueFamilyProperties2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 36:
      /* GetPhysicalDeviceQueueFamilyProperties2KHR */
      if (instance->KHR_get_physical_device_properties2) return true;
      return false;
   case 37:
      /* GetPhysicalDeviceMemoryProperties2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 38:
      /* GetPhysicalDeviceMemoryProperties2KHR */
      if (instance->KHR_get_physical_device_properties2) return true;
      return false;
   case 39:
      /* GetPhysicalDeviceSparseImageFormatProperties2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 40:
      /* GetPhysicalDeviceSparseImageFormatProperties2KHR */
      if (instance->KHR_get_physical_device_properties2) return true;
      return false;
   case 41:
      /* GetPhysicalDeviceExternalBufferProperties */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 42:
      /* GetPhysicalDeviceExternalBufferPropertiesKHR */
      if (instance->KHR_external_memory_capabilities) return true;
      return false;
   case 43:
      /* GetPhysicalDeviceExternalSemaphoreProperties */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 44:
      /* GetPhysicalDeviceExternalSemaphorePropertiesKHR */
      if (instance->KHR_external_semaphore_capabilities) return true;
      return false;
   case 45:
      /* GetPhysicalDeviceExternalFenceProperties */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 46:
      /* GetPhysicalDeviceExternalFencePropertiesKHR */
      if (instance->KHR_external_fence_capabilities) return true;
      return false;
   case 47:
      /* ReleaseDisplayEXT */
      if (instance->EXT_direct_mode_display) return true;
      return false;
   case 48:
      /* AcquireXlibDisplayEXT */
      if (instance->EXT_acquire_xlib_display) return true;
      return false;
   case 49:
      /* GetRandROutputDisplayEXT */
      if (instance->EXT_acquire_xlib_display) return true;
      return false;
   case 50:
      /* AcquireWinrtDisplayNV */
      /* All device extensions are considered enabled at the instance level */
      return true;
      return false;
   case 51:
      /* GetWinrtDisplayNV */
      /* All device extensions are considered enabled at the instance level */
      return true;
      return false;
   case 52:
      /* GetPhysicalDeviceSurfaceCapabilities2EXT */
      if (instance->EXT_display_surface_counter) return true;
      return false;
   case 53:
      /* GetPhysicalDevicePresentRectanglesKHR */
      /* All device extensions are considered enabled at the instance level */
      return true;
      /* All device extensions are considered enabled at the instance level */
      return true;
      return false;
   case 54:
      /* GetPhysicalDeviceMultisamplePropertiesEXT */
      /* All device extensions are considered enabled at the instance level */
      return true;
      return false;
   case 55:
      /* GetPhysicalDeviceSurfaceCapabilities2KHR */
      if (instance->KHR_get_surface_capabilities2) return true;
      return false;
   case 56:
      /* GetPhysicalDeviceSurfaceFormats2KHR */
      if (instance->KHR_get_surface_capabilities2) return true;
      return false;
   case 57:
      /* GetPhysicalDeviceDisplayProperties2KHR */
      if (instance->KHR_get_display_properties2) return true;
      return false;
   case 58:
      /* GetPhysicalDeviceDisplayPlaneProperties2KHR */
      if (instance->KHR_get_display_properties2) return true;
      return false;
   case 59:
      /* GetDisplayModeProperties2KHR */
      if (instance->KHR_get_display_properties2) return true;
      return false;
   case 60:
      /* GetDisplayPlaneCapabilities2KHR */
      if (instance->KHR_get_display_properties2) return true;
      return false;
   case 61:
      /* GetPhysicalDeviceCalibrateableTimeDomainsEXT */
      /* All device extensions are considered enabled at the instance level */
      return true;
      return false;
   case 62:
      /* GetPhysicalDeviceCooperativeMatrixPropertiesNV */
      /* All device extensions are considered enabled at the instance level */
      return true;
      return false;
   case 63:
      /* GetPhysicalDeviceSurfacePresentModes2EXT */
      /* All device extensions are considered enabled at the instance level */
      return true;
      return false;
   case 64:
      /* EnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR */
      /* All device extensions are considered enabled at the instance level */
      return true;
      return false;
   case 65:
      /* GetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR */
      /* All device extensions are considered enabled at the instance level */
      return true;
      return false;
   case 66:
      /* GetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV */
      /* All device extensions are considered enabled at the instance level */
      return true;
      return false;
   case 67:
      /* GetPhysicalDeviceToolProperties */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 68:
      /* GetPhysicalDeviceToolPropertiesEXT */
      /* All device extensions are considered enabled at the instance level */
      return true;
      return false;
   case 69:
      /* GetPhysicalDeviceFragmentShadingRatesKHR */
      /* All device extensions are considered enabled at the instance level */
      return true;
      return false;
   case 70:
      /* GetPhysicalDeviceVideoCapabilitiesKHR */
      /* All device extensions are considered enabled at the instance level */
      return true;
      return false;
   case 71:
      /* GetPhysicalDeviceVideoFormatPropertiesKHR */
      /* All device extensions are considered enabled at the instance level */
      return true;
      return false;
   case 72:
      /* AcquireDrmDisplayEXT */
      if (instance->EXT_acquire_drm_display) return true;
      return false;
   case 73:
      /* GetDrmDisplayEXT */
      if (instance->EXT_acquire_drm_display) return true;
      return false;
   case 74:
      /* GetPhysicalDeviceOpticalFlowImageFormatsNV */
      /* All device extensions are considered enabled at the instance level */
      return true;
      return false;
   case 75:
      /* GetPhysicalDeviceCooperativeMatrixPropertiesKHR */
      /* All device extensions are considered enabled at the instance level */
      return true;
      return false;
   default:
      return false;
   }
}

/** Return true if the core version or extension in which the given entrypoint
 * is defined is enabled.
 *
 * If device is NULL, all device extensions are considered enabled.
 */
static bool
vk_device_entrypoint_is_enabled(int index, uint32_t core_version,
                                const struct vk_instance_extension_table *instance,
                                const struct vk_device_extension_table *device)
{
   switch (index) {
   case 0:
      /* GetDeviceProcAddr */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 1:
      /* DestroyDevice */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 2:
      /* GetDeviceQueue */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 3:
      /* QueueSubmit */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 4:
      /* QueueWaitIdle */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 5:
      /* DeviceWaitIdle */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 6:
      /* AllocateMemory */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 7:
      /* FreeMemory */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 8:
      /* MapMemory */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 9:
      /* UnmapMemory */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 10:
      /* FlushMappedMemoryRanges */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 11:
      /* InvalidateMappedMemoryRanges */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 12:
      /* GetDeviceMemoryCommitment */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 13:
      /* GetBufferMemoryRequirements */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 14:
      /* BindBufferMemory */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 15:
      /* GetImageMemoryRequirements */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 16:
      /* BindImageMemory */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 17:
      /* GetImageSparseMemoryRequirements */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 18:
      /* QueueBindSparse */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 19:
      /* CreateFence */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 20:
      /* DestroyFence */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 21:
      /* ResetFences */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 22:
      /* GetFenceStatus */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 23:
      /* WaitForFences */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 24:
      /* CreateSemaphore */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 25:
      /* DestroySemaphore */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 26:
      /* CreateEvent */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 27:
      /* DestroyEvent */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 28:
      /* GetEventStatus */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 29:
      /* SetEvent */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 30:
      /* ResetEvent */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 31:
      /* CreateQueryPool */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 32:
      /* DestroyQueryPool */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 33:
      /* GetQueryPoolResults */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 34:
      /* ResetQueryPool */
      return VK_MAKE_VERSION(1, 2, 0) <= core_version;
   case 35:
      /* ResetQueryPoolEXT */
      if (!device || device->EXT_host_query_reset) return true;
      return false;
   case 36:
      /* CreateBuffer */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 37:
      /* DestroyBuffer */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 38:
      /* CreateBufferView */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 39:
      /* DestroyBufferView */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 40:
      /* CreateImage */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 41:
      /* DestroyImage */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 42:
      /* GetImageSubresourceLayout */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 43:
      /* CreateImageView */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 44:
      /* DestroyImageView */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 45:
      /* CreateShaderModule */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 46:
      /* DestroyShaderModule */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 47:
      /* CreatePipelineCache */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 48:
      /* DestroyPipelineCache */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 49:
      /* GetPipelineCacheData */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 50:
      /* MergePipelineCaches */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 51:
      /* CreateGraphicsPipelines */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 52:
      /* CreateComputePipelines */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 53:
      /* GetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI */
      if (!device || device->HUAWEI_subpass_shading) return true;
      return false;
   case 54:
      /* DestroyPipeline */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 55:
      /* CreatePipelineLayout */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 56:
      /* DestroyPipelineLayout */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 57:
      /* CreateSampler */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 58:
      /* DestroySampler */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 59:
      /* CreateDescriptorSetLayout */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 60:
      /* DestroyDescriptorSetLayout */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 61:
      /* CreateDescriptorPool */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 62:
      /* DestroyDescriptorPool */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 63:
      /* ResetDescriptorPool */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 64:
      /* AllocateDescriptorSets */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 65:
      /* FreeDescriptorSets */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 66:
      /* UpdateDescriptorSets */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 67:
      /* CreateFramebuffer */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 68:
      /* DestroyFramebuffer */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 69:
      /* CreateRenderPass */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 70:
      /* DestroyRenderPass */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 71:
      /* GetRenderAreaGranularity */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 72:
      /* GetRenderingAreaGranularityKHR */
      if (!device || device->KHR_maintenance5) return true;
      return false;
   case 73:
      /* CreateCommandPool */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 74:
      /* DestroyCommandPool */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 75:
      /* ResetCommandPool */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 76:
      /* AllocateCommandBuffers */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 77:
      /* FreeCommandBuffers */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 78:
      /* BeginCommandBuffer */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 79:
      /* EndCommandBuffer */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 80:
      /* ResetCommandBuffer */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 81:
      /* CmdBindPipeline */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 82:
      /* CmdSetAttachmentFeedbackLoopEnableEXT */
      if (!device || device->EXT_attachment_feedback_loop_dynamic_state) return true;
      return false;
   case 83:
      /* CmdSetViewport */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 84:
      /* CmdSetScissor */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 85:
      /* CmdSetLineWidth */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 86:
      /* CmdSetDepthBias */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 87:
      /* CmdSetBlendConstants */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 88:
      /* CmdSetDepthBounds */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 89:
      /* CmdSetStencilCompareMask */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 90:
      /* CmdSetStencilWriteMask */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 91:
      /* CmdSetStencilReference */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 92:
      /* CmdBindDescriptorSets */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 93:
      /* CmdBindIndexBuffer */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 94:
      /* CmdBindVertexBuffers */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 95:
      /* CmdDraw */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 96:
      /* CmdDrawIndexed */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 97:
      /* CmdDrawMultiEXT */
      if (!device || device->EXT_multi_draw) return true;
      return false;
   case 98:
      /* CmdDrawMultiIndexedEXT */
      if (!device || device->EXT_multi_draw) return true;
      return false;
   case 99:
      /* CmdDrawIndirect */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 100:
      /* CmdDrawIndexedIndirect */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 101:
      /* CmdDispatch */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 102:
      /* CmdDispatchIndirect */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 103:
      /* CmdSubpassShadingHUAWEI */
      if (!device || device->HUAWEI_subpass_shading) return true;
      return false;
   case 104:
      /* CmdDrawClusterHUAWEI */
      if (!device || device->HUAWEI_cluster_culling_shader) return true;
      return false;
   case 105:
      /* CmdDrawClusterIndirectHUAWEI */
      if (!device || device->HUAWEI_cluster_culling_shader) return true;
      return false;
   case 106:
      /* CmdUpdatePipelineIndirectBufferNV */
      if (!device || device->NV_device_generated_commands_compute) return true;
      return false;
   case 107:
      /* CmdCopyBuffer */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 108:
      /* CmdCopyImage */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 109:
      /* CmdBlitImage */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 110:
      /* CmdCopyBufferToImage */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 111:
      /* CmdCopyImageToBuffer */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 112:
      /* CmdCopyMemoryIndirectNV */
      if (!device || device->NV_copy_memory_indirect) return true;
      return false;
   case 113:
      /* CmdCopyMemoryToImageIndirectNV */
      if (!device || device->NV_copy_memory_indirect) return true;
      return false;
   case 114:
      /* CmdUpdateBuffer */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 115:
      /* CmdFillBuffer */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 116:
      /* CmdClearColorImage */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 117:
      /* CmdClearDepthStencilImage */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 118:
      /* CmdClearAttachments */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 119:
      /* CmdResolveImage */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 120:
      /* CmdSetEvent */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 121:
      /* CmdResetEvent */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 122:
      /* CmdWaitEvents */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 123:
      /* CmdPipelineBarrier */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 124:
      /* CmdBeginQuery */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 125:
      /* CmdEndQuery */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 126:
      /* CmdBeginConditionalRenderingEXT */
      if (!device || device->EXT_conditional_rendering) return true;
      return false;
   case 127:
      /* CmdEndConditionalRenderingEXT */
      if (!device || device->EXT_conditional_rendering) return true;
      return false;
   case 128:
      /* CmdResetQueryPool */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 129:
      /* CmdWriteTimestamp */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 130:
      /* CmdCopyQueryPoolResults */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 131:
      /* CmdPushConstants */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 132:
      /* CmdBeginRenderPass */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 133:
      /* CmdNextSubpass */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 134:
      /* CmdEndRenderPass */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 135:
      /* CmdExecuteCommands */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 136:
      /* CreateSharedSwapchainsKHR */
      if (!device || device->KHR_display_swapchain) return true;
      return false;
   case 137:
      /* CreateSwapchainKHR */
      if (!device || device->KHR_swapchain) return true;
      return false;
   case 138:
      /* DestroySwapchainKHR */
      if (!device || device->KHR_swapchain) return true;
      return false;
   case 139:
      /* GetSwapchainImagesKHR */
      if (!device || device->KHR_swapchain) return true;
      return false;
   case 140:
      /* AcquireNextImageKHR */
      if (!device || device->KHR_swapchain) return true;
      return false;
   case 141:
      /* QueuePresentKHR */
      if (!device || device->KHR_swapchain) return true;
      return false;
   case 142:
      /* DebugMarkerSetObjectNameEXT */
      if (!device || device->EXT_debug_marker) return true;
      return false;
   case 143:
      /* DebugMarkerSetObjectTagEXT */
      if (!device || device->EXT_debug_marker) return true;
      return false;
   case 144:
      /* CmdDebugMarkerBeginEXT */
      if (!device || device->EXT_debug_marker) return true;
      return false;
   case 145:
      /* CmdDebugMarkerEndEXT */
      if (!device || device->EXT_debug_marker) return true;
      return false;
   case 146:
      /* CmdDebugMarkerInsertEXT */
      if (!device || device->EXT_debug_marker) return true;
      return false;
   case 147:
      /* GetMemoryWin32HandleNV */
      if (!device || device->NV_external_memory_win32) return true;
      return false;
   case 148:
      /* CmdExecuteGeneratedCommandsNV */
      if (!device || device->NV_device_generated_commands) return true;
      return false;
   case 149:
      /* CmdPreprocessGeneratedCommandsNV */
      if (!device || device->NV_device_generated_commands) return true;
      return false;
   case 150:
      /* CmdBindPipelineShaderGroupNV */
      if (!device || device->NV_device_generated_commands) return true;
      return false;
   case 151:
      /* GetGeneratedCommandsMemoryRequirementsNV */
      if (!device || device->NV_device_generated_commands) return true;
      return false;
   case 152:
      /* CreateIndirectCommandsLayoutNV */
      if (!device || device->NV_device_generated_commands) return true;
      return false;
   case 153:
      /* DestroyIndirectCommandsLayoutNV */
      if (!device || device->NV_device_generated_commands) return true;
      return false;
   case 154:
      /* CmdPushDescriptorSetKHR */
      if (!device || device->KHR_push_descriptor) return true;
      return false;
   case 155:
      /* TrimCommandPool */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 156:
      /* TrimCommandPoolKHR */
      if (!device || device->KHR_maintenance1) return true;
      return false;
   case 157:
      /* GetMemoryWin32HandleKHR */
      if (!device || device->KHR_external_memory_win32) return true;
      return false;
   case 158:
      /* GetMemoryWin32HandlePropertiesKHR */
      if (!device || device->KHR_external_memory_win32) return true;
      return false;
   case 159:
      /* GetMemoryFdKHR */
      if (!device || device->KHR_external_memory_fd) return true;
      return false;
   case 160:
      /* GetMemoryFdPropertiesKHR */
      if (!device || device->KHR_external_memory_fd) return true;
      return false;
   case 161:
      /* GetMemoryZirconHandleFUCHSIA */
      if (!device || device->FUCHSIA_external_memory) return true;
      return false;
   case 162:
      /* GetMemoryZirconHandlePropertiesFUCHSIA */
      if (!device || device->FUCHSIA_external_memory) return true;
      return false;
   case 163:
      /* GetMemoryRemoteAddressNV */
      if (!device || device->NV_external_memory_rdma) return true;
      return false;
   case 164:
      /* GetSemaphoreWin32HandleKHR */
      if (!device || device->KHR_external_semaphore_win32) return true;
      return false;
   case 165:
      /* ImportSemaphoreWin32HandleKHR */
      if (!device || device->KHR_external_semaphore_win32) return true;
      return false;
   case 166:
      /* GetSemaphoreFdKHR */
      if (!device || device->KHR_external_semaphore_fd) return true;
      return false;
   case 167:
      /* ImportSemaphoreFdKHR */
      if (!device || device->KHR_external_semaphore_fd) return true;
      return false;
   case 168:
      /* GetSemaphoreZirconHandleFUCHSIA */
      if (!device || device->FUCHSIA_external_semaphore) return true;
      return false;
   case 169:
      /* ImportSemaphoreZirconHandleFUCHSIA */
      if (!device || device->FUCHSIA_external_semaphore) return true;
      return false;
   case 170:
      /* GetFenceWin32HandleKHR */
      if (!device || device->KHR_external_fence_win32) return true;
      return false;
   case 171:
      /* ImportFenceWin32HandleKHR */
      if (!device || device->KHR_external_fence_win32) return true;
      return false;
   case 172:
      /* GetFenceFdKHR */
      if (!device || device->KHR_external_fence_fd) return true;
      return false;
   case 173:
      /* ImportFenceFdKHR */
      if (!device || device->KHR_external_fence_fd) return true;
      return false;
   case 174:
      /* DisplayPowerControlEXT */
      if (!device || device->EXT_display_control) return true;
      return false;
   case 175:
      /* RegisterDeviceEventEXT */
      if (!device || device->EXT_display_control) return true;
      return false;
   case 176:
      /* RegisterDisplayEventEXT */
      if (!device || device->EXT_display_control) return true;
      return false;
   case 177:
      /* GetSwapchainCounterEXT */
      if (!device || device->EXT_display_control) return true;
      return false;
   case 178:
      /* GetDeviceGroupPeerMemoryFeatures */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 179:
      /* GetDeviceGroupPeerMemoryFeaturesKHR */
      if (!device || device->KHR_device_group) return true;
      return false;
   case 180:
      /* BindBufferMemory2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 181:
      /* BindBufferMemory2KHR */
      if (!device || device->KHR_bind_memory2) return true;
      return false;
   case 182:
      /* BindImageMemory2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 183:
      /* BindImageMemory2KHR */
      if (!device || device->KHR_bind_memory2) return true;
      return false;
   case 184:
      /* CmdSetDeviceMask */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 185:
      /* CmdSetDeviceMaskKHR */
      if (!device || device->KHR_device_group) return true;
      return false;
   case 186:
      /* GetDeviceGroupPresentCapabilitiesKHR */
      if (!device || device->KHR_swapchain) return true;
      if (!device || device->KHR_device_group) return true;
      return false;
   case 187:
      /* GetDeviceGroupSurfacePresentModesKHR */
      if (!device || device->KHR_swapchain) return true;
      if (!device || device->KHR_device_group) return true;
      return false;
   case 188:
      /* AcquireNextImage2KHR */
      if (!device || device->KHR_swapchain) return true;
      if (!device || device->KHR_device_group) return true;
      return false;
   case 189:
      /* CmdDispatchBase */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 190:
      /* CmdDispatchBaseKHR */
      if (!device || device->KHR_device_group) return true;
      return false;
   case 191:
      /* CreateDescriptorUpdateTemplate */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 192:
      /* CreateDescriptorUpdateTemplateKHR */
      if (!device || device->KHR_descriptor_update_template) return true;
      return false;
   case 193:
      /* DestroyDescriptorUpdateTemplate */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 194:
      /* DestroyDescriptorUpdateTemplateKHR */
      if (!device || device->KHR_descriptor_update_template) return true;
      return false;
   case 195:
      /* UpdateDescriptorSetWithTemplate */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 196:
      /* UpdateDescriptorSetWithTemplateKHR */
      if (!device || device->KHR_descriptor_update_template) return true;
      return false;
   case 197:
      /* CmdPushDescriptorSetWithTemplateKHR */
      if (!device || device->KHR_push_descriptor) return true;
      if (!device || device->KHR_descriptor_update_template) return true;
      return false;
   case 198:
      /* SetHdrMetadataEXT */
      if (!device || device->EXT_hdr_metadata) return true;
      return false;
   case 199:
      /* GetSwapchainStatusKHR */
      if (!device || device->KHR_shared_presentable_image) return true;
      return false;
   case 200:
      /* GetRefreshCycleDurationGOOGLE */
      if (!device || device->GOOGLE_display_timing) return true;
      return false;
   case 201:
      /* GetPastPresentationTimingGOOGLE */
      if (!device || device->GOOGLE_display_timing) return true;
      return false;
   case 202:
      /* CmdSetViewportWScalingNV */
      if (!device || device->NV_clip_space_w_scaling) return true;
      return false;
   case 203:
      /* CmdSetDiscardRectangleEXT */
      if (!device || device->EXT_discard_rectangles) return true;
      return false;
   case 204:
      /* CmdSetDiscardRectangleEnableEXT */
      if (!device || device->EXT_discard_rectangles) return true;
      return false;
   case 205:
      /* CmdSetDiscardRectangleModeEXT */
      if (!device || device->EXT_discard_rectangles) return true;
      return false;
   case 206:
      /* CmdSetSampleLocationsEXT */
      if (!device || device->EXT_sample_locations) return true;
      return false;
   case 207:
      /* GetBufferMemoryRequirements2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 208:
      /* GetBufferMemoryRequirements2KHR */
      if (!device || device->KHR_get_memory_requirements2) return true;
      return false;
   case 209:
      /* GetImageMemoryRequirements2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 210:
      /* GetImageMemoryRequirements2KHR */
      if (!device || device->KHR_get_memory_requirements2) return true;
      return false;
   case 211:
      /* GetImageSparseMemoryRequirements2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 212:
      /* GetImageSparseMemoryRequirements2KHR */
      if (!device || device->KHR_get_memory_requirements2) return true;
      return false;
   case 213:
      /* GetDeviceBufferMemoryRequirements */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 214:
      /* GetDeviceBufferMemoryRequirementsKHR */
      if (!device || device->KHR_maintenance4) return true;
      return false;
   case 215:
      /* GetDeviceImageMemoryRequirements */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 216:
      /* GetDeviceImageMemoryRequirementsKHR */
      if (!device || device->KHR_maintenance4) return true;
      return false;
   case 217:
      /* GetDeviceImageSparseMemoryRequirements */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 218:
      /* GetDeviceImageSparseMemoryRequirementsKHR */
      if (!device || device->KHR_maintenance4) return true;
      return false;
   case 219:
      /* CreateSamplerYcbcrConversion */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 220:
      /* CreateSamplerYcbcrConversionKHR */
      if (!device || device->KHR_sampler_ycbcr_conversion) return true;
      return false;
   case 221:
      /* DestroySamplerYcbcrConversion */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 222:
      /* DestroySamplerYcbcrConversionKHR */
      if (!device || device->KHR_sampler_ycbcr_conversion) return true;
      return false;
   case 223:
      /* GetDeviceQueue2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 224:
      /* CreateValidationCacheEXT */
      if (!device || device->EXT_validation_cache) return true;
      return false;
   case 225:
      /* DestroyValidationCacheEXT */
      if (!device || device->EXT_validation_cache) return true;
      return false;
   case 226:
      /* GetValidationCacheDataEXT */
      if (!device || device->EXT_validation_cache) return true;
      return false;
   case 227:
      /* MergeValidationCachesEXT */
      if (!device || device->EXT_validation_cache) return true;
      return false;
   case 228:
      /* GetDescriptorSetLayoutSupport */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 229:
      /* GetDescriptorSetLayoutSupportKHR */
      if (!device || device->KHR_maintenance3) return true;
      return false;
   case 230:
      /* GetSwapchainGrallocUsageANDROID */
      if (!device || device->ANDROID_native_buffer) return true;
      return false;
   case 231:
      /* GetSwapchainGrallocUsage2ANDROID */
      if (!device || device->ANDROID_native_buffer) return true;
      return false;
   case 232:
      /* AcquireImageANDROID */
      if (!device || device->ANDROID_native_buffer) return true;
      return false;
   case 233:
      /* QueueSignalReleaseImageANDROID */
      if (!device || device->ANDROID_native_buffer) return true;
      return false;
   case 234:
      /* GetShaderInfoAMD */
      if (!device || device->AMD_shader_info) return true;
      return false;
   case 235:
      /* SetLocalDimmingAMD */
      if (!device || device->AMD_display_native_hdr) return true;
      return false;
   case 236:
      /* GetCalibratedTimestampsEXT */
      if (!device || device->EXT_calibrated_timestamps) return true;
      return false;
   case 237:
      /* SetDebugUtilsObjectNameEXT */
      if (instance->EXT_debug_utils) return true;
      return false;
   case 238:
      /* SetDebugUtilsObjectTagEXT */
      if (instance->EXT_debug_utils) return true;
      return false;
   case 239:
      /* QueueBeginDebugUtilsLabelEXT */
      if (instance->EXT_debug_utils) return true;
      return false;
   case 240:
      /* QueueEndDebugUtilsLabelEXT */
      if (instance->EXT_debug_utils) return true;
      return false;
   case 241:
      /* QueueInsertDebugUtilsLabelEXT */
      if (instance->EXT_debug_utils) return true;
      return false;
   case 242:
      /* CmdBeginDebugUtilsLabelEXT */
      if (instance->EXT_debug_utils) return true;
      return false;
   case 243:
      /* CmdEndDebugUtilsLabelEXT */
      if (instance->EXT_debug_utils) return true;
      return false;
   case 244:
      /* CmdInsertDebugUtilsLabelEXT */
      if (instance->EXT_debug_utils) return true;
      return false;
   case 245:
      /* GetMemoryHostPointerPropertiesEXT */
      if (!device || device->EXT_external_memory_host) return true;
      return false;
   case 246:
      /* CmdWriteBufferMarkerAMD */
      if (!device || device->AMD_buffer_marker) return true;
      return false;
   case 247:
      /* CreateRenderPass2 */
      return VK_MAKE_VERSION(1, 2, 0) <= core_version;
   case 248:
      /* CreateRenderPass2KHR */
      if (!device || device->KHR_create_renderpass2) return true;
      return false;
   case 249:
      /* CmdBeginRenderPass2 */
      return VK_MAKE_VERSION(1, 2, 0) <= core_version;
   case 250:
      /* CmdBeginRenderPass2KHR */
      if (!device || device->KHR_create_renderpass2) return true;
      return false;
   case 251:
      /* CmdNextSubpass2 */
      return VK_MAKE_VERSION(1, 2, 0) <= core_version;
   case 252:
      /* CmdNextSubpass2KHR */
      if (!device || device->KHR_create_renderpass2) return true;
      return false;
   case 253:
      /* CmdEndRenderPass2 */
      return VK_MAKE_VERSION(1, 2, 0) <= core_version;
   case 254:
      /* CmdEndRenderPass2KHR */
      if (!device || device->KHR_create_renderpass2) return true;
      return false;
   case 255:
      /* GetSemaphoreCounterValue */
      return VK_MAKE_VERSION(1, 2, 0) <= core_version;
   case 256:
      /* GetSemaphoreCounterValueKHR */
      if (!device || device->KHR_timeline_semaphore) return true;
      return false;
   case 257:
      /* WaitSemaphores */
      return VK_MAKE_VERSION(1, 2, 0) <= core_version;
   case 258:
      /* WaitSemaphoresKHR */
      if (!device || device->KHR_timeline_semaphore) return true;
      return false;
   case 259:
      /* SignalSemaphore */
      return VK_MAKE_VERSION(1, 2, 0) <= core_version;
   case 260:
      /* SignalSemaphoreKHR */
      if (!device || device->KHR_timeline_semaphore) return true;
      return false;
   case 261:
      /* GetAndroidHardwareBufferPropertiesANDROID */
      if (!device || device->ANDROID_external_memory_android_hardware_buffer) return true;
      return false;
   case 262:
      /* GetMemoryAndroidHardwareBufferANDROID */
      if (!device || device->ANDROID_external_memory_android_hardware_buffer) return true;
      return false;
   case 263:
      /* CmdDrawIndirectCount */
      return VK_MAKE_VERSION(1, 2, 0) <= core_version;
   case 264:
      /* CmdDrawIndirectCountKHR */
      if (!device || device->KHR_draw_indirect_count) return true;
      return false;
   case 265:
      /* CmdDrawIndirectCountAMD */
      if (!device || device->AMD_draw_indirect_count) return true;
      return false;
   case 266:
      /* CmdDrawIndexedIndirectCount */
      return VK_MAKE_VERSION(1, 2, 0) <= core_version;
   case 267:
      /* CmdDrawIndexedIndirectCountKHR */
      if (!device || device->KHR_draw_indirect_count) return true;
      return false;
   case 268:
      /* CmdDrawIndexedIndirectCountAMD */
      if (!device || device->AMD_draw_indirect_count) return true;
      return false;
   case 269:
      /* CmdSetCheckpointNV */
      if (!device || device->NV_device_diagnostic_checkpoints) return true;
      return false;
   case 270:
      /* GetQueueCheckpointDataNV */
      if (!device || device->NV_device_diagnostic_checkpoints) return true;
      return false;
   case 271:
      /* CmdBindTransformFeedbackBuffersEXT */
      if (!device || device->EXT_transform_feedback) return true;
      return false;
   case 272:
      /* CmdBeginTransformFeedbackEXT */
      if (!device || device->EXT_transform_feedback) return true;
      return false;
   case 273:
      /* CmdEndTransformFeedbackEXT */
      if (!device || device->EXT_transform_feedback) return true;
      return false;
   case 274:
      /* CmdBeginQueryIndexedEXT */
      if (!device || device->EXT_transform_feedback) return true;
      return false;
   case 275:
      /* CmdEndQueryIndexedEXT */
      if (!device || device->EXT_transform_feedback) return true;
      return false;
   case 276:
      /* CmdDrawIndirectByteCountEXT */
      if (!device || device->EXT_transform_feedback) return true;
      return false;
   case 277:
      /* CmdSetExclusiveScissorNV */
      if (!device || device->NV_scissor_exclusive) return true;
      return false;
   case 278:
      /* CmdSetExclusiveScissorEnableNV */
      if (!device || device->NV_scissor_exclusive) return true;
      return false;
   case 279:
      /* CmdBindShadingRateImageNV */
      if (!device || device->NV_shading_rate_image) return true;
      return false;
   case 280:
      /* CmdSetViewportShadingRatePaletteNV */
      if (!device || device->NV_shading_rate_image) return true;
      return false;
   case 281:
      /* CmdSetCoarseSampleOrderNV */
      if (!device || device->NV_shading_rate_image) return true;
      return false;
   case 282:
      /* CmdDrawMeshTasksNV */
      if (!device || device->NV_mesh_shader) return true;
      return false;
   case 283:
      /* CmdDrawMeshTasksIndirectNV */
      if (!device || device->NV_mesh_shader) return true;
      return false;
   case 284:
      /* CmdDrawMeshTasksIndirectCountNV */
      if (!device || device->NV_mesh_shader) return true;
      return false;
   case 285:
      /* CmdDrawMeshTasksEXT */
      if (!device || device->EXT_mesh_shader) return true;
      return false;
   case 286:
      /* CmdDrawMeshTasksIndirectEXT */
      if (!device || device->EXT_mesh_shader) return true;
      return false;
   case 287:
      /* CmdDrawMeshTasksIndirectCountEXT */
      if (!device || device->EXT_mesh_shader) return true;
      return false;
   case 288:
      /* CompileDeferredNV */
      if (!device || device->NV_ray_tracing) return true;
      return false;
   case 289:
      /* CreateAccelerationStructureNV */
      if (!device || device->NV_ray_tracing) return true;
      return false;
   case 290:
      /* CmdBindInvocationMaskHUAWEI */
      if (!device || device->HUAWEI_invocation_mask) return true;
      return false;
   case 291:
      /* DestroyAccelerationStructureKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 292:
      /* DestroyAccelerationStructureNV */
      if (!device || device->NV_ray_tracing) return true;
      return false;
   case 293:
      /* GetAccelerationStructureMemoryRequirementsNV */
      if (!device || device->NV_ray_tracing) return true;
      return false;
   case 294:
      /* BindAccelerationStructureMemoryNV */
      if (!device || device->NV_ray_tracing) return true;
      return false;
   case 295:
      /* CmdCopyAccelerationStructureNV */
      if (!device || device->NV_ray_tracing) return true;
      return false;
   case 296:
      /* CmdCopyAccelerationStructureKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 297:
      /* CopyAccelerationStructureKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 298:
      /* CmdCopyAccelerationStructureToMemoryKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 299:
      /* CopyAccelerationStructureToMemoryKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 300:
      /* CmdCopyMemoryToAccelerationStructureKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 301:
      /* CopyMemoryToAccelerationStructureKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 302:
      /* CmdWriteAccelerationStructuresPropertiesKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 303:
      /* CmdWriteAccelerationStructuresPropertiesNV */
      if (!device || device->NV_ray_tracing) return true;
      return false;
   case 304:
      /* CmdBuildAccelerationStructureNV */
      if (!device || device->NV_ray_tracing) return true;
      return false;
   case 305:
      /* WriteAccelerationStructuresPropertiesKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 306:
      /* CmdTraceRaysKHR */
      if (!device || device->KHR_ray_tracing_pipeline) return true;
      return false;
   case 307:
      /* CmdTraceRaysNV */
      if (!device || device->NV_ray_tracing) return true;
      return false;
   case 308:
      /* GetRayTracingShaderGroupHandlesKHR */
      if (!device || device->KHR_ray_tracing_pipeline) return true;
      return false;
   case 309:
      /* GetRayTracingShaderGroupHandlesNV */
      if (!device || device->NV_ray_tracing) return true;
      return false;
   case 310:
      /* GetRayTracingCaptureReplayShaderGroupHandlesKHR */
      if (!device || device->KHR_ray_tracing_pipeline) return true;
      return false;
   case 311:
      /* GetAccelerationStructureHandleNV */
      if (!device || device->NV_ray_tracing) return true;
      return false;
   case 312:
      /* CreateRayTracingPipelinesNV */
      if (!device || device->NV_ray_tracing) return true;
      return false;
   case 313:
      /* CreateRayTracingPipelinesKHR */
      if (!device || device->KHR_ray_tracing_pipeline) return true;
      return false;
   case 314:
      /* CmdTraceRaysIndirectKHR */
      if (!device || device->KHR_ray_tracing_pipeline) return true;
      return false;
   case 315:
      /* CmdTraceRaysIndirect2KHR */
      if (!device || device->KHR_ray_tracing_maintenance1) return true;
      return false;
   case 316:
      /* GetDeviceAccelerationStructureCompatibilityKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 317:
      /* GetRayTracingShaderGroupStackSizeKHR */
      if (!device || device->KHR_ray_tracing_pipeline) return true;
      return false;
   case 318:
      /* CmdSetRayTracingPipelineStackSizeKHR */
      if (!device || device->KHR_ray_tracing_pipeline) return true;
      return false;
   case 319:
      /* GetImageViewHandleNVX */
      if (!device || device->NVX_image_view_handle) return true;
      return false;
   case 320:
      /* GetImageViewAddressNVX */
      if (!device || device->NVX_image_view_handle) return true;
      return false;
   case 321:
      /* GetDeviceGroupSurfacePresentModes2EXT */
      if (!device || device->EXT_full_screen_exclusive) return true;
      return false;
   case 322:
      /* AcquireFullScreenExclusiveModeEXT */
      if (!device || device->EXT_full_screen_exclusive) return true;
      return false;
   case 323:
      /* ReleaseFullScreenExclusiveModeEXT */
      if (!device || device->EXT_full_screen_exclusive) return true;
      return false;
   case 324:
      /* AcquireProfilingLockKHR */
      if (!device || device->KHR_performance_query) return true;
      return false;
   case 325:
      /* ReleaseProfilingLockKHR */
      if (!device || device->KHR_performance_query) return true;
      return false;
   case 326:
      /* GetImageDrmFormatModifierPropertiesEXT */
      if (!device || device->EXT_image_drm_format_modifier) return true;
      return false;
   case 327:
      /* GetBufferOpaqueCaptureAddress */
      return VK_MAKE_VERSION(1, 2, 0) <= core_version;
   case 328:
      /* GetBufferOpaqueCaptureAddressKHR */
      if (!device || device->KHR_buffer_device_address) return true;
      return false;
   case 329:
      /* GetBufferDeviceAddress */
      return VK_MAKE_VERSION(1, 2, 0) <= core_version;
   case 330:
      /* GetBufferDeviceAddressKHR */
      if (!device || device->KHR_buffer_device_address) return true;
      return false;
   case 331:
      /* GetBufferDeviceAddressEXT */
      if (!device || device->EXT_buffer_device_address) return true;
      return false;
   case 332:
      /* InitializePerformanceApiINTEL */
      if (!device || device->INTEL_performance_query) return true;
      return false;
   case 333:
      /* UninitializePerformanceApiINTEL */
      if (!device || device->INTEL_performance_query) return true;
      return false;
   case 334:
      /* CmdSetPerformanceMarkerINTEL */
      if (!device || device->INTEL_performance_query) return true;
      return false;
   case 335:
      /* CmdSetPerformanceStreamMarkerINTEL */
      if (!device || device->INTEL_performance_query) return true;
      return false;
   case 336:
      /* CmdSetPerformanceOverrideINTEL */
      if (!device || device->INTEL_performance_query) return true;
      return false;
   case 337:
      /* AcquirePerformanceConfigurationINTEL */
      if (!device || device->INTEL_performance_query) return true;
      return false;
   case 338:
      /* ReleasePerformanceConfigurationINTEL */
      if (!device || device->INTEL_performance_query) return true;
      return false;
   case 339:
      /* QueueSetPerformanceConfigurationINTEL */
      if (!device || device->INTEL_performance_query) return true;
      return false;
   case 340:
      /* GetPerformanceParameterINTEL */
      if (!device || device->INTEL_performance_query) return true;
      return false;
   case 341:
      /* GetDeviceMemoryOpaqueCaptureAddress */
      return VK_MAKE_VERSION(1, 2, 0) <= core_version;
   case 342:
      /* GetDeviceMemoryOpaqueCaptureAddressKHR */
      if (!device || device->KHR_buffer_device_address) return true;
      return false;
   case 343:
      /* GetPipelineExecutablePropertiesKHR */
      if (!device || device->KHR_pipeline_executable_properties) return true;
      return false;
   case 344:
      /* GetPipelineExecutableStatisticsKHR */
      if (!device || device->KHR_pipeline_executable_properties) return true;
      return false;
   case 345:
      /* GetPipelineExecutableInternalRepresentationsKHR */
      if (!device || device->KHR_pipeline_executable_properties) return true;
      return false;
   case 346:
      /* CmdSetLineStippleEXT */
      if (!device || device->EXT_line_rasterization) return true;
      return false;
   case 347:
      /* CreateAccelerationStructureKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 348:
      /* CmdBuildAccelerationStructuresKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 349:
      /* CmdBuildAccelerationStructuresIndirectKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 350:
      /* BuildAccelerationStructuresKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 351:
      /* GetAccelerationStructureDeviceAddressKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 352:
      /* CreateDeferredOperationKHR */
      if (!device || device->KHR_deferred_host_operations) return true;
      return false;
   case 353:
      /* DestroyDeferredOperationKHR */
      if (!device || device->KHR_deferred_host_operations) return true;
      return false;
   case 354:
      /* GetDeferredOperationMaxConcurrencyKHR */
      if (!device || device->KHR_deferred_host_operations) return true;
      return false;
   case 355:
      /* GetDeferredOperationResultKHR */
      if (!device || device->KHR_deferred_host_operations) return true;
      return false;
   case 356:
      /* DeferredOperationJoinKHR */
      if (!device || device->KHR_deferred_host_operations) return true;
      return false;
   case 357:
      /* GetPipelineIndirectMemoryRequirementsNV */
      if (!device || device->NV_device_generated_commands_compute) return true;
      return false;
   case 358:
      /* GetPipelineIndirectDeviceAddressNV */
      if (!device || device->NV_device_generated_commands_compute) return true;
      return false;
   case 359:
      /* CmdSetCullMode */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 360:
      /* CmdSetCullModeEXT */
      if (!device || device->EXT_extended_dynamic_state) return true;
      if (!device || device->EXT_shader_object) return true;
      return false;
   case 361:
      /* CmdSetFrontFace */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 362:
      /* CmdSetFrontFaceEXT */
      if (!device || device->EXT_extended_dynamic_state) return true;
      if (!device || device->EXT_shader_object) return true;
      return false;
   case 363:
      /* CmdSetPrimitiveTopology */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 364:
      /* CmdSetPrimitiveTopologyEXT */
      if (!device || device->EXT_extended_dynamic_state) return true;
      if (!device || device->EXT_shader_object) return true;
      return false;
   case 365:
      /* CmdSetViewportWithCount */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 366:
      /* CmdSetViewportWithCountEXT */
      if (!device || device->EXT_extended_dynamic_state) return true;
      if (!device || device->EXT_shader_object) return true;
      return false;
   case 367:
      /* CmdSetScissorWithCount */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 368:
      /* CmdSetScissorWithCountEXT */
      if (!device || device->EXT_extended_dynamic_state) return true;
      if (!device || device->EXT_shader_object) return true;
      return false;
   case 369:
      /* CmdBindIndexBuffer2KHR */
      if (!device || device->KHR_maintenance5) return true;
      return false;
   case 370:
      /* CmdBindVertexBuffers2 */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 371:
      /* CmdBindVertexBuffers2EXT */
      if (!device || device->EXT_extended_dynamic_state) return true;
      if (!device || device->EXT_shader_object) return true;
      return false;
   case 372:
      /* CmdSetDepthTestEnable */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 373:
      /* CmdSetDepthTestEnableEXT */
      if (!device || device->EXT_extended_dynamic_state) return true;
      if (!device || device->EXT_shader_object) return true;
      return false;
   case 374:
      /* CmdSetDepthWriteEnable */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 375:
      /* CmdSetDepthWriteEnableEXT */
      if (!device || device->EXT_extended_dynamic_state) return true;
      if (!device || device->EXT_shader_object) return true;
      return false;
   case 376:
      /* CmdSetDepthCompareOp */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 377:
      /* CmdSetDepthCompareOpEXT */
      if (!device || device->EXT_extended_dynamic_state) return true;
      if (!device || device->EXT_shader_object) return true;
      return false;
   case 378:
      /* CmdSetDepthBoundsTestEnable */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 379:
      /* CmdSetDepthBoundsTestEnableEXT */
      if (!device || device->EXT_extended_dynamic_state) return true;
      if (!device || device->EXT_shader_object) return true;
      return false;
   case 380:
      /* CmdSetStencilTestEnable */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 381:
      /* CmdSetStencilTestEnableEXT */
      if (!device || device->EXT_extended_dynamic_state) return true;
      if (!device || device->EXT_shader_object) return true;
      return false;
   case 382:
      /* CmdSetStencilOp */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 383:
      /* CmdSetStencilOpEXT */
      if (!device || device->EXT_extended_dynamic_state) return true;
      if (!device || device->EXT_shader_object) return true;
      return false;
   case 384:
      /* CmdSetPatchControlPointsEXT */
      if (!device || device->EXT_extended_dynamic_state2) return true;
      if (!device || device->EXT_shader_object) return true;
      return false;
   case 385:
      /* CmdSetRasterizerDiscardEnable */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 386:
      /* CmdSetRasterizerDiscardEnableEXT */
      if (!device || device->EXT_extended_dynamic_state2) return true;
      if (!device || device->EXT_shader_object) return true;
      return false;
   case 387:
      /* CmdSetDepthBiasEnable */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 388:
      /* CmdSetDepthBiasEnableEXT */
      if (!device || device->EXT_extended_dynamic_state2) return true;
      if (!device || device->EXT_shader_object) return true;
      return false;
   case 389:
      /* CmdSetLogicOpEXT */
      if (!device || device->EXT_extended_dynamic_state2) return true;
      if (!device || device->EXT_shader_object) return true;
      return false;
   case 390:
      /* CmdSetPrimitiveRestartEnable */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 391:
      /* CmdSetPrimitiveRestartEnableEXT */
      if (!device || device->EXT_extended_dynamic_state2) return true;
      if (!device || device->EXT_shader_object) return true;
      return false;
   case 392:
      /* CmdSetTessellationDomainOriginEXT */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      if (!device || device->EXT_shader_object) return true;
      return false;
   case 393:
      /* CmdSetDepthClampEnableEXT */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      if (!device || device->EXT_shader_object) return true;
      return false;
   case 394:
      /* CmdSetPolygonModeEXT */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      if (!device || device->EXT_shader_object) return true;
      return false;
   case 395:
      /* CmdSetRasterizationSamplesEXT */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      if (!device || device->EXT_shader_object) return true;
      return false;
   case 396:
      /* CmdSetSampleMaskEXT */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      if (!device || device->EXT_shader_object) return true;
      return false;
   case 397:
      /* CmdSetAlphaToCoverageEnableEXT */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      if (!device || device->EXT_shader_object) return true;
      return false;
   case 398:
      /* CmdSetAlphaToOneEnableEXT */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      if (!device || device->EXT_shader_object) return true;
      return false;
   case 399:
      /* CmdSetLogicOpEnableEXT */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      if (!device || device->EXT_shader_object) return true;
      return false;
   case 400:
      /* CmdSetColorBlendEnableEXT */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      if (!device || device->EXT_shader_object) return true;
      return false;
   case 401:
      /* CmdSetColorBlendEquationEXT */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      if (!device || device->EXT_shader_object) return true;
      return false;
   case 402:
      /* CmdSetColorWriteMaskEXT */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      if (!device || device->EXT_shader_object) return true;
      return false;
   case 403:
      /* CmdSetRasterizationStreamEXT */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      if (!device || device->EXT_shader_object) return true;
      return false;
   case 404:
      /* CmdSetConservativeRasterizationModeEXT */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      if (!device || device->EXT_shader_object) return true;
      return false;
   case 405:
      /* CmdSetExtraPrimitiveOverestimationSizeEXT */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      if (!device || device->EXT_shader_object) return true;
      return false;
   case 406:
      /* CmdSetDepthClipEnableEXT */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      if (!device || device->EXT_shader_object) return true;
      return false;
   case 407:
      /* CmdSetSampleLocationsEnableEXT */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      if (!device || device->EXT_shader_object) return true;
      return false;
   case 408:
      /* CmdSetColorBlendAdvancedEXT */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      if (!device || device->EXT_shader_object) return true;
      return false;
   case 409:
      /* CmdSetProvokingVertexModeEXT */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      if (!device || device->EXT_shader_object) return true;
      return false;
   case 410:
      /* CmdSetLineRasterizationModeEXT */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      if (!device || device->EXT_shader_object) return true;
      return false;
   case 411:
      /* CmdSetLineStippleEnableEXT */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      if (!device || device->EXT_shader_object) return true;
      return false;
   case 412:
      /* CmdSetDepthClipNegativeOneToOneEXT */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      if (!device || device->EXT_shader_object) return true;
      return false;
   case 413:
      /* CmdSetViewportWScalingEnableNV */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      if (!device || device->EXT_shader_object) return true;
      return false;
   case 414:
      /* CmdSetViewportSwizzleNV */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      if (!device || device->EXT_shader_object) return true;
      return false;
   case 415:
      /* CmdSetCoverageToColorEnableNV */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      if (!device || device->EXT_shader_object) return true;
      return false;
   case 416:
      /* CmdSetCoverageToColorLocationNV */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      if (!device || device->EXT_shader_object) return true;
      return false;
   case 417:
      /* CmdSetCoverageModulationModeNV */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      if (!device || device->EXT_shader_object) return true;
      return false;
   case 418:
      /* CmdSetCoverageModulationTableEnableNV */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      if (!device || device->EXT_shader_object) return true;
      return false;
   case 419:
      /* CmdSetCoverageModulationTableNV */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      if (!device || device->EXT_shader_object) return true;
      return false;
   case 420:
      /* CmdSetShadingRateImageEnableNV */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      if (!device || device->EXT_shader_object) return true;
      return false;
   case 421:
      /* CmdSetCoverageReductionModeNV */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      if (!device || device->EXT_shader_object) return true;
      return false;
   case 422:
      /* CmdSetRepresentativeFragmentTestEnableNV */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      if (!device || device->EXT_shader_object) return true;
      return false;
   case 423:
      /* CreatePrivateDataSlot */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 424:
      /* CreatePrivateDataSlotEXT */
      if (!device || device->EXT_private_data) return true;
      return false;
   case 425:
      /* DestroyPrivateDataSlot */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 426:
      /* DestroyPrivateDataSlotEXT */
      if (!device || device->EXT_private_data) return true;
      return false;
   case 427:
      /* SetPrivateData */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 428:
      /* SetPrivateDataEXT */
      if (!device || device->EXT_private_data) return true;
      return false;
   case 429:
      /* GetPrivateData */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 430:
      /* GetPrivateDataEXT */
      if (!device || device->EXT_private_data) return true;
      return false;
   case 431:
      /* CmdCopyBuffer2 */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 432:
      /* CmdCopyBuffer2KHR */
      if (!device || device->KHR_copy_commands2) return true;
      return false;
   case 433:
      /* CmdCopyImage2 */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 434:
      /* CmdCopyImage2KHR */
      if (!device || device->KHR_copy_commands2) return true;
      return false;
   case 435:
      /* CmdBlitImage2 */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 436:
      /* CmdBlitImage2KHR */
      if (!device || device->KHR_copy_commands2) return true;
      return false;
   case 437:
      /* CmdCopyBufferToImage2 */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 438:
      /* CmdCopyBufferToImage2KHR */
      if (!device || device->KHR_copy_commands2) return true;
      return false;
   case 439:
      /* CmdCopyImageToBuffer2 */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 440:
      /* CmdCopyImageToBuffer2KHR */
      if (!device || device->KHR_copy_commands2) return true;
      return false;
   case 441:
      /* CmdResolveImage2 */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 442:
      /* CmdResolveImage2KHR */
      if (!device || device->KHR_copy_commands2) return true;
      return false;
   case 443:
      /* CmdSetFragmentShadingRateKHR */
      if (!device || device->KHR_fragment_shading_rate) return true;
      return false;
   case 444:
      /* CmdSetFragmentShadingRateEnumNV */
      if (!device || device->NV_fragment_shading_rate_enums) return true;
      return false;
   case 445:
      /* GetAccelerationStructureBuildSizesKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 446:
      /* CmdSetVertexInputEXT */
      if (!device || device->EXT_vertex_input_dynamic_state) return true;
      if (!device || device->EXT_shader_object) return true;
      return false;
   case 447:
      /* CmdSetColorWriteEnableEXT */
      if (!device || device->EXT_color_write_enable) return true;
      return false;
   case 448:
      /* CmdSetEvent2 */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 449:
      /* CmdSetEvent2KHR */
      if (!device || device->KHR_synchronization2) return true;
      return false;
   case 450:
      /* CmdResetEvent2 */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 451:
      /* CmdResetEvent2KHR */
      if (!device || device->KHR_synchronization2) return true;
      return false;
   case 452:
      /* CmdWaitEvents2 */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 453:
      /* CmdWaitEvents2KHR */
      if (!device || device->KHR_synchronization2) return true;
      return false;
   case 454:
      /* CmdPipelineBarrier2 */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 455:
      /* CmdPipelineBarrier2KHR */
      if (!device || device->KHR_synchronization2) return true;
      return false;
   case 456:
      /* QueueSubmit2 */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 457:
      /* QueueSubmit2KHR */
      if (!device || device->KHR_synchronization2) return true;
      return false;
   case 458:
      /* CmdWriteTimestamp2 */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 459:
      /* CmdWriteTimestamp2KHR */
      if (!device || device->KHR_synchronization2) return true;
      return false;
   case 460:
      /* CmdWriteBufferMarker2AMD */
      if (!device || device->KHR_synchronization2) return true;
      return false;
   case 461:
      /* GetQueueCheckpointData2NV */
      if (!device || device->KHR_synchronization2) return true;
      return false;
   case 462:
      /* CopyMemoryToImageEXT */
      if (!device || device->EXT_host_image_copy) return true;
      return false;
   case 463:
      /* CopyImageToMemoryEXT */
      if (!device || device->EXT_host_image_copy) return true;
      return false;
   case 464:
      /* CopyImageToImageEXT */
      if (!device || device->EXT_host_image_copy) return true;
      return false;
   case 465:
      /* TransitionImageLayoutEXT */
      if (!device || device->EXT_host_image_copy) return true;
      return false;
   case 466:
      /* CreateVideoSessionKHR */
      if (!device || device->KHR_video_queue) return true;
      return false;
   case 467:
      /* DestroyVideoSessionKHR */
      if (!device || device->KHR_video_queue) return true;
      return false;
   case 468:
      /* CreateVideoSessionParametersKHR */
      if (!device || device->KHR_video_queue) return true;
      return false;
   case 469:
      /* UpdateVideoSessionParametersKHR */
      if (!device || device->KHR_video_queue) return true;
      return false;
   case 470:
      /* DestroyVideoSessionParametersKHR */
      if (!device || device->KHR_video_queue) return true;
      return false;
   case 471:
      /* GetVideoSessionMemoryRequirementsKHR */
      if (!device || device->KHR_video_queue) return true;
      return false;
   case 472:
      /* BindVideoSessionMemoryKHR */
      if (!device || device->KHR_video_queue) return true;
      return false;
   case 473:
      /* CmdDecodeVideoKHR */
      if (!device || device->KHR_video_decode_queue) return true;
      return false;
   case 474:
      /* CmdBeginVideoCodingKHR */
      if (!device || device->KHR_video_queue) return true;
      return false;
   case 475:
      /* CmdControlVideoCodingKHR */
      if (!device || device->KHR_video_queue) return true;
      return false;
   case 476:
      /* CmdEndVideoCodingKHR */
      if (!device || device->KHR_video_queue) return true;
      return false;
   case 477:
      /* CmdDecompressMemoryNV */
      if (!device || device->NV_memory_decompression) return true;
      return false;
   case 478:
      /* CmdDecompressMemoryIndirectCountNV */
      if (!device || device->NV_memory_decompression) return true;
      return false;
   case 479:
      /* CreateCuModuleNVX */
      if (!device || device->NVX_binary_import) return true;
      return false;
   case 480:
      /* CreateCuFunctionNVX */
      if (!device || device->NVX_binary_import) return true;
      return false;
   case 481:
      /* DestroyCuModuleNVX */
      if (!device || device->NVX_binary_import) return true;
      return false;
   case 482:
      /* DestroyCuFunctionNVX */
      if (!device || device->NVX_binary_import) return true;
      return false;
   case 483:
      /* CmdCuLaunchKernelNVX */
      if (!device || device->NVX_binary_import) return true;
      return false;
   case 484:
      /* GetDescriptorSetLayoutSizeEXT */
      if (!device || device->EXT_descriptor_buffer) return true;
      return false;
   case 485:
      /* GetDescriptorSetLayoutBindingOffsetEXT */
      if (!device || device->EXT_descriptor_buffer) return true;
      return false;
   case 486:
      /* GetDescriptorEXT */
      if (!device || device->EXT_descriptor_buffer) return true;
      return false;
   case 487:
      /* CmdBindDescriptorBuffersEXT */
      if (!device || device->EXT_descriptor_buffer) return true;
      return false;
   case 488:
      /* CmdSetDescriptorBufferOffsetsEXT */
      if (!device || device->EXT_descriptor_buffer) return true;
      return false;
   case 489:
      /* CmdBindDescriptorBufferEmbeddedSamplersEXT */
      if (!device || device->EXT_descriptor_buffer) return true;
      return false;
   case 490:
      /* GetBufferOpaqueCaptureDescriptorDataEXT */
      if (!device || device->EXT_descriptor_buffer) return true;
      return false;
   case 491:
      /* GetImageOpaqueCaptureDescriptorDataEXT */
      if (!device || device->EXT_descriptor_buffer) return true;
      return false;
   case 492:
      /* GetImageViewOpaqueCaptureDescriptorDataEXT */
      if (!device || device->EXT_descriptor_buffer) return true;
      return false;
   case 493:
      /* GetSamplerOpaqueCaptureDescriptorDataEXT */
      if (!device || device->EXT_descriptor_buffer) return true;
      return false;
   case 494:
      /* GetAccelerationStructureOpaqueCaptureDescriptorDataEXT */
      if (!device || device->EXT_descriptor_buffer) return true;
      return false;
   case 495:
      /* SetDeviceMemoryPriorityEXT */
      if (!device || device->EXT_pageable_device_local_memory) return true;
      return false;
   case 496:
      /* WaitForPresentKHR */
      if (!device || device->KHR_present_wait) return true;
      return false;
   case 497:
      /* CreateBufferCollectionFUCHSIA */
      if (!device || device->FUCHSIA_buffer_collection) return true;
      return false;
   case 498:
      /* SetBufferCollectionBufferConstraintsFUCHSIA */
      if (!device || device->FUCHSIA_buffer_collection) return true;
      return false;
   case 499:
      /* SetBufferCollectionImageConstraintsFUCHSIA */
      if (!device || device->FUCHSIA_buffer_collection) return true;
      return false;
   case 500:
      /* DestroyBufferCollectionFUCHSIA */
      if (!device || device->FUCHSIA_buffer_collection) return true;
      return false;
   case 501:
      /* GetBufferCollectionPropertiesFUCHSIA */
      if (!device || device->FUCHSIA_buffer_collection) return true;
      return false;
   case 502:
      /* CmdBeginRendering */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 503:
      /* CmdBeginRenderingKHR */
      if (!device || device->KHR_dynamic_rendering) return true;
      return false;
   case 504:
      /* CmdEndRendering */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 505:
      /* CmdEndRenderingKHR */
      if (!device || device->KHR_dynamic_rendering) return true;
      return false;
   case 506:
      /* GetDescriptorSetLayoutHostMappingInfoVALVE */
      if (!device || device->VALVE_descriptor_set_host_mapping) return true;
      return false;
   case 507:
      /* GetDescriptorSetHostMappingVALVE */
      if (!device || device->VALVE_descriptor_set_host_mapping) return true;
      return false;
   case 508:
      /* CreateMicromapEXT */
      if (!device || device->EXT_opacity_micromap) return true;
      return false;
   case 509:
      /* CmdBuildMicromapsEXT */
      if (!device || device->EXT_opacity_micromap) return true;
      return false;
   case 510:
      /* BuildMicromapsEXT */
      if (!device || device->EXT_opacity_micromap) return true;
      return false;
   case 511:
      /* DestroyMicromapEXT */
      if (!device || device->EXT_opacity_micromap) return true;
      return false;
   case 512:
      /* CmdCopyMicromapEXT */
      if (!device || device->EXT_opacity_micromap) return true;
      return false;
   case 513:
      /* CopyMicromapEXT */
      if (!device || device->EXT_opacity_micromap) return true;
      return false;
   case 514:
      /* CmdCopyMicromapToMemoryEXT */
      if (!device || device->EXT_opacity_micromap) return true;
      return false;
   case 515:
      /* CopyMicromapToMemoryEXT */
      if (!device || device->EXT_opacity_micromap) return true;
      return false;
   case 516:
      /* CmdCopyMemoryToMicromapEXT */
      if (!device || device->EXT_opacity_micromap) return true;
      return false;
   case 517:
      /* CopyMemoryToMicromapEXT */
      if (!device || device->EXT_opacity_micromap) return true;
      return false;
   case 518:
      /* CmdWriteMicromapsPropertiesEXT */
      if (!device || device->EXT_opacity_micromap) return true;
      return false;
   case 519:
      /* WriteMicromapsPropertiesEXT */
      if (!device || device->EXT_opacity_micromap) return true;
      return false;
   case 520:
      /* GetDeviceMicromapCompatibilityEXT */
      if (!device || device->EXT_opacity_micromap) return true;
      return false;
   case 521:
      /* GetMicromapBuildSizesEXT */
      if (!device || device->EXT_opacity_micromap) return true;
      return false;
   case 522:
      /* GetShaderModuleIdentifierEXT */
      if (!device || device->EXT_shader_module_identifier) return true;
      return false;
   case 523:
      /* GetShaderModuleCreateInfoIdentifierEXT */
      if (!device || device->EXT_shader_module_identifier) return true;
      return false;
   case 524:
      /* GetImageSubresourceLayout2KHR */
      if (!device || device->KHR_maintenance5) return true;
      return false;
   case 525:
      /* GetImageSubresourceLayout2EXT */
      if (!device || device->EXT_host_image_copy) return true;
      if (!device || device->EXT_image_compression_control) return true;
      return false;
   case 526:
      /* GetPipelinePropertiesEXT */
      if (!device || device->EXT_pipeline_properties) return true;
      return false;
   case 527:
      /* ExportMetalObjectsEXT */
      if (!device || device->EXT_metal_objects) return true;
      return false;
   case 528:
      /* GetFramebufferTilePropertiesQCOM */
      if (!device || device->QCOM_tile_properties) return true;
      return false;
   case 529:
      /* GetDynamicRenderingTilePropertiesQCOM */
      if (!device || device->QCOM_tile_properties) return true;
      return false;
   case 530:
      /* CreateOpticalFlowSessionNV */
      if (!device || device->NV_optical_flow) return true;
      return false;
   case 531:
      /* DestroyOpticalFlowSessionNV */
      if (!device || device->NV_optical_flow) return true;
      return false;
   case 532:
      /* BindOpticalFlowSessionImageNV */
      if (!device || device->NV_optical_flow) return true;
      return false;
   case 533:
      /* CmdOpticalFlowExecuteNV */
      if (!device || device->NV_optical_flow) return true;
      return false;
   case 534:
      /* GetDeviceFaultInfoEXT */
      if (!device || device->EXT_device_fault) return true;
      return false;
   case 535:
      /* CmdSetDepthBias2EXT */
      if (!device || device->EXT_depth_bias_control) return true;
      return false;
   case 536:
      /* ReleaseSwapchainImagesEXT */
      if (!device || device->EXT_swapchain_maintenance1) return true;
      return false;
   case 537:
      /* GetDeviceImageSubresourceLayoutKHR */
      if (!device || device->KHR_maintenance5) return true;
      return false;
   case 538:
      /* MapMemory2KHR */
      if (!device || device->KHR_map_memory2) return true;
      return false;
   case 539:
      /* UnmapMemory2KHR */
      if (!device || device->KHR_map_memory2) return true;
      return false;
   case 540:
      /* CreateShadersEXT */
      if (!device || device->EXT_shader_object) return true;
      return false;
   case 541:
      /* DestroyShaderEXT */
      if (!device || device->EXT_shader_object) return true;
      return false;
   case 542:
      /* GetShaderBinaryDataEXT */
      if (!device || device->EXT_shader_object) return true;
      return false;
   case 543:
      /* CmdBindShadersEXT */
      if (!device || device->EXT_shader_object) return true;
      return false;
   case 544:
      /* GetScreenBufferPropertiesQNX */
      if (!device || device->QNX_external_memory_screen_buffer) return true;
      return false;
   case 545:
      /* SetLatencySleepModeNV */
      if (!device || device->NV_low_latency2) return true;
      return false;
   case 546:
      /* LatencySleepNV */
      if (!device || device->NV_low_latency2) return true;
      return false;
   case 547:
      /* SetLatencyMarkerNV */
      if (!device || device->NV_low_latency2) return true;
      return false;
   case 548:
      /* GetLatencyTimingsNV */
      if (!device || device->NV_low_latency2) return true;
      return false;
   case 549:
      /* QueueNotifyOutOfBandNV */
      if (!device || device->NV_low_latency2) return true;
      return false;
   default:
      return false;
   }
}

#ifdef _MSC_VER
VKAPI_ATTR void VKAPI_CALL vk_entrypoint_stub(void)
{
   unreachable("Entrypoint not implemented");
}

static const void *get_function_target(const void *func)
{
   const uint8_t *address = func;
#ifdef _M_X64
   /* Incremental linking may indirect through relative jump */
   if (*address == 0xE9)
   {
      /* Compute JMP target if the first byte is opcode 0xE9 */
      uint32_t offset;
      memcpy(&offset, address + 1, 4);
      address += offset + 5;
   }
#else
   /* Add other platforms here if necessary */
#endif
   return address;
}

static bool vk_function_is_stub(PFN_vkVoidFunction func)
{
   return (func == vk_entrypoint_stub) || (get_function_target(func) == get_function_target(vk_entrypoint_stub));
}
#endif




void vk_instance_dispatch_table_from_entrypoints(
    struct vk_instance_dispatch_table *dispatch_table,
    const struct vk_instance_entrypoint_table *entrypoint_table,
    bool overwrite)
{
    PFN_vkVoidFunction *disp = (PFN_vkVoidFunction *)dispatch_table;
    PFN_vkVoidFunction *entry = (PFN_vkVoidFunction *)entrypoint_table;

    if (overwrite) {
        memset(dispatch_table, 0, sizeof(*dispatch_table));
        for (unsigned i = 0; i < ARRAY_SIZE(instance_compaction_table); i++) {
#ifdef _MSC_VER
            assert(entry[i] != NULL);
            if (vk_function_is_stub(entry[i]))
#else
            if (entry[i] == NULL)
#endif
                continue;
            unsigned disp_index = instance_compaction_table[i];
            assert(disp[disp_index] == NULL);
            disp[disp_index] = entry[i];
        }
    } else {
        for (unsigned i = 0; i < ARRAY_SIZE(instance_compaction_table); i++) {
            unsigned disp_index = instance_compaction_table[i];
#ifdef _MSC_VER
            assert(entry[i] != NULL);
            if (disp[disp_index] == NULL && !vk_function_is_stub(entry[i]))
#else
            if (disp[disp_index] == NULL)
#endif
                disp[disp_index] = entry[i];
        }
    }
}


void vk_physical_device_dispatch_table_from_entrypoints(
    struct vk_physical_device_dispatch_table *dispatch_table,
    const struct vk_physical_device_entrypoint_table *entrypoint_table,
    bool overwrite)
{
    PFN_vkVoidFunction *disp = (PFN_vkVoidFunction *)dispatch_table;
    PFN_vkVoidFunction *entry = (PFN_vkVoidFunction *)entrypoint_table;

    if (overwrite) {
        memset(dispatch_table, 0, sizeof(*dispatch_table));
        for (unsigned i = 0; i < ARRAY_SIZE(physical_device_compaction_table); i++) {
#ifdef _MSC_VER
            assert(entry[i] != NULL);
            if (vk_function_is_stub(entry[i]))
#else
            if (entry[i] == NULL)
#endif
                continue;
            unsigned disp_index = physical_device_compaction_table[i];
            assert(disp[disp_index] == NULL);
            disp[disp_index] = entry[i];
        }
    } else {
        for (unsigned i = 0; i < ARRAY_SIZE(physical_device_compaction_table); i++) {
            unsigned disp_index = physical_device_compaction_table[i];
#ifdef _MSC_VER
            assert(entry[i] != NULL);
            if (disp[disp_index] == NULL && !vk_function_is_stub(entry[i]))
#else
            if (disp[disp_index] == NULL)
#endif
                disp[disp_index] = entry[i];
        }
    }
}


void vk_device_dispatch_table_from_entrypoints(
    struct vk_device_dispatch_table *dispatch_table,
    const struct vk_device_entrypoint_table *entrypoint_table,
    bool overwrite)
{
    PFN_vkVoidFunction *disp = (PFN_vkVoidFunction *)dispatch_table;
    PFN_vkVoidFunction *entry = (PFN_vkVoidFunction *)entrypoint_table;

    if (overwrite) {
        memset(dispatch_table, 0, sizeof(*dispatch_table));
        for (unsigned i = 0; i < ARRAY_SIZE(device_compaction_table); i++) {
#ifdef _MSC_VER
            assert(entry[i] != NULL);
            if (vk_function_is_stub(entry[i]))
#else
            if (entry[i] == NULL)
#endif
                continue;
            unsigned disp_index = device_compaction_table[i];
            assert(disp[disp_index] == NULL);
            disp[disp_index] = entry[i];
        }
    } else {
        for (unsigned i = 0; i < ARRAY_SIZE(device_compaction_table); i++) {
            unsigned disp_index = device_compaction_table[i];
#ifdef _MSC_VER
            assert(entry[i] != NULL);
            if (disp[disp_index] == NULL && !vk_function_is_stub(entry[i]))
#else
            if (disp[disp_index] == NULL)
#endif
                disp[disp_index] = entry[i];
        }
    }
}





static PFN_vkVoidFunction
vk_instance_dispatch_table_get_for_entry_index(
    const struct vk_instance_dispatch_table *table, int entry_index)
{
    assert(entry_index < ARRAY_SIZE(instance_compaction_table));
    int disp_index = instance_compaction_table[entry_index];
    return ((PFN_vkVoidFunction *)table)[disp_index];
}

PFN_vkVoidFunction
vk_instance_dispatch_table_get(
    const struct vk_instance_dispatch_table *table, const char *name)
{
    int entry_index = instance_string_map_lookup(name);
    if (entry_index < 0)
        return NULL;

    return vk_instance_dispatch_table_get_for_entry_index(table, entry_index);
}


static PFN_vkVoidFunction
vk_physical_device_dispatch_table_get_for_entry_index(
    const struct vk_physical_device_dispatch_table *table, int entry_index)
{
    assert(entry_index < ARRAY_SIZE(physical_device_compaction_table));
    int disp_index = physical_device_compaction_table[entry_index];
    return ((PFN_vkVoidFunction *)table)[disp_index];
}

PFN_vkVoidFunction
vk_physical_device_dispatch_table_get(
    const struct vk_physical_device_dispatch_table *table, const char *name)
{
    int entry_index = physical_device_string_map_lookup(name);
    if (entry_index < 0)
        return NULL;

    return vk_physical_device_dispatch_table_get_for_entry_index(table, entry_index);
}


static PFN_vkVoidFunction
vk_device_dispatch_table_get_for_entry_index(
    const struct vk_device_dispatch_table *table, int entry_index)
{
    assert(entry_index < ARRAY_SIZE(device_compaction_table));
    int disp_index = device_compaction_table[entry_index];
    return ((PFN_vkVoidFunction *)table)[disp_index];
}

PFN_vkVoidFunction
vk_device_dispatch_table_get(
    const struct vk_device_dispatch_table *table, const char *name)
{
    int entry_index = device_string_map_lookup(name);
    if (entry_index < 0)
        return NULL;

    return vk_device_dispatch_table_get_for_entry_index(table, entry_index);
}


PFN_vkVoidFunction
vk_instance_dispatch_table_get_if_supported(
    const struct vk_instance_dispatch_table *table,
    const char *name,
    uint32_t core_version,
    const struct vk_instance_extension_table *instance_exts)
{
    int entry_index = instance_string_map_lookup(name);
    if (entry_index < 0)
        return NULL;

    if (!vk_instance_entrypoint_is_enabled(entry_index, core_version,
                                           instance_exts))
        return NULL;

    return vk_instance_dispatch_table_get_for_entry_index(table, entry_index);
}

PFN_vkVoidFunction
vk_physical_device_dispatch_table_get_if_supported(
    const struct vk_physical_device_dispatch_table *table,
    const char *name,
    uint32_t core_version,
    const struct vk_instance_extension_table *instance_exts)
{
    int entry_index = physical_device_string_map_lookup(name);
    if (entry_index < 0)
        return NULL;

    if (!vk_physical_device_entrypoint_is_enabled(entry_index, core_version,
                                                  instance_exts))
        return NULL;

    return vk_physical_device_dispatch_table_get_for_entry_index(table, entry_index);
}

PFN_vkVoidFunction
vk_device_dispatch_table_get_if_supported(
    const struct vk_device_dispatch_table *table,
    const char *name,
    uint32_t core_version,
    const struct vk_instance_extension_table *instance_exts,
    const struct vk_device_extension_table *device_exts)
{
    int entry_index = device_string_map_lookup(name);
    if (entry_index < 0)
        return NULL;

    if (!vk_device_entrypoint_is_enabled(entry_index, core_version,
                                         instance_exts, device_exts))
        return NULL;

    return vk_device_dispatch_table_get_for_entry_index(table, entry_index);
}
