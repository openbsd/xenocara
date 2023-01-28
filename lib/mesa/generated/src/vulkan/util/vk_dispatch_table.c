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
#ifdef VK_ENABLE_BETA_EXTENSIONS
    table->GetPhysicalDeviceVideoCapabilitiesKHR = (PFN_vkGetPhysicalDeviceVideoCapabilitiesKHR) gpa(obj, "vkGetPhysicalDeviceVideoCapabilitiesKHR");
#endif
#ifdef VK_ENABLE_BETA_EXTENSIONS
    table->GetPhysicalDeviceVideoFormatPropertiesKHR = (PFN_vkGetPhysicalDeviceVideoFormatPropertiesKHR) gpa(obj, "vkGetPhysicalDeviceVideoFormatPropertiesKHR");
#endif
    table->AcquireDrmDisplayEXT = (PFN_vkAcquireDrmDisplayEXT) gpa(obj, "vkAcquireDrmDisplayEXT");
    table->GetDrmDisplayEXT = (PFN_vkGetDrmDisplayEXT) gpa(obj, "vkGetDrmDisplayEXT");
    table->GetPhysicalDeviceOpticalFlowImageFormatsNV = (PFN_vkGetPhysicalDeviceOpticalFlowImageFormatsNV) gpa(obj, "vkGetPhysicalDeviceOpticalFlowImageFormatsNV");
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
    table->CreateCommandPool = (PFN_vkCreateCommandPool) gpa(obj, "vkCreateCommandPool");
    table->DestroyCommandPool = (PFN_vkDestroyCommandPool) gpa(obj, "vkDestroyCommandPool");
    table->ResetCommandPool = (PFN_vkResetCommandPool) gpa(obj, "vkResetCommandPool");
    table->AllocateCommandBuffers = (PFN_vkAllocateCommandBuffers) gpa(obj, "vkAllocateCommandBuffers");
    table->FreeCommandBuffers = (PFN_vkFreeCommandBuffers) gpa(obj, "vkFreeCommandBuffers");
    table->BeginCommandBuffer = (PFN_vkBeginCommandBuffer) gpa(obj, "vkBeginCommandBuffer");
    table->EndCommandBuffer = (PFN_vkEndCommandBuffer) gpa(obj, "vkEndCommandBuffer");
    table->ResetCommandBuffer = (PFN_vkResetCommandBuffer) gpa(obj, "vkResetCommandBuffer");
    table->CmdBindPipeline = (PFN_vkCmdBindPipeline) gpa(obj, "vkCmdBindPipeline");
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
    table->CmdCopyBuffer = (PFN_vkCmdCopyBuffer) gpa(obj, "vkCmdCopyBuffer");
    table->CmdCopyImage = (PFN_vkCmdCopyImage) gpa(obj, "vkCmdCopyImage");
    table->CmdBlitImage = (PFN_vkCmdBlitImage) gpa(obj, "vkCmdBlitImage");
    table->CmdCopyBufferToImage = (PFN_vkCmdCopyBufferToImage) gpa(obj, "vkCmdCopyBufferToImage");
    table->CmdCopyImageToBuffer = (PFN_vkCmdCopyImageToBuffer) gpa(obj, "vkCmdCopyImageToBuffer");
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
        table->CreatePrivateDataSlot = (PFN_vkCreatePrivateDataSlot) gpa(obj, "vkCreatePrivateDataSlot");
    if (table->CreatePrivateDataSlot == NULL) {
        table->CreatePrivateDataSlot = (PFN_vkCreatePrivateDataSlot) gpa(obj, "vkCreatePrivateDataSlotEXT");
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
#ifdef VK_ENABLE_BETA_EXTENSIONS
    table->CreateVideoSessionKHR = (PFN_vkCreateVideoSessionKHR) gpa(obj, "vkCreateVideoSessionKHR");
#endif
#ifdef VK_ENABLE_BETA_EXTENSIONS
    table->DestroyVideoSessionKHR = (PFN_vkDestroyVideoSessionKHR) gpa(obj, "vkDestroyVideoSessionKHR");
#endif
#ifdef VK_ENABLE_BETA_EXTENSIONS
    table->CreateVideoSessionParametersKHR = (PFN_vkCreateVideoSessionParametersKHR) gpa(obj, "vkCreateVideoSessionParametersKHR");
#endif
#ifdef VK_ENABLE_BETA_EXTENSIONS
    table->UpdateVideoSessionParametersKHR = (PFN_vkUpdateVideoSessionParametersKHR) gpa(obj, "vkUpdateVideoSessionParametersKHR");
#endif
#ifdef VK_ENABLE_BETA_EXTENSIONS
    table->DestroyVideoSessionParametersKHR = (PFN_vkDestroyVideoSessionParametersKHR) gpa(obj, "vkDestroyVideoSessionParametersKHR");
#endif
#ifdef VK_ENABLE_BETA_EXTENSIONS
    table->GetVideoSessionMemoryRequirementsKHR = (PFN_vkGetVideoSessionMemoryRequirementsKHR) gpa(obj, "vkGetVideoSessionMemoryRequirementsKHR");
#endif
#ifdef VK_ENABLE_BETA_EXTENSIONS
    table->BindVideoSessionMemoryKHR = (PFN_vkBindVideoSessionMemoryKHR) gpa(obj, "vkBindVideoSessionMemoryKHR");
#endif
#ifdef VK_ENABLE_BETA_EXTENSIONS
    table->CmdDecodeVideoKHR = (PFN_vkCmdDecodeVideoKHR) gpa(obj, "vkCmdDecodeVideoKHR");
#endif
#ifdef VK_ENABLE_BETA_EXTENSIONS
    table->CmdBeginVideoCodingKHR = (PFN_vkCmdBeginVideoCodingKHR) gpa(obj, "vkCmdBeginVideoCodingKHR");
#endif
#ifdef VK_ENABLE_BETA_EXTENSIONS
    table->CmdControlVideoCodingKHR = (PFN_vkCmdControlVideoCodingKHR) gpa(obj, "vkCmdControlVideoCodingKHR");
#endif
#ifdef VK_ENABLE_BETA_EXTENSIONS
    table->CmdEndVideoCodingKHR = (PFN_vkCmdEndVideoCodingKHR) gpa(obj, "vkCmdEndVideoCodingKHR");
#endif
#ifdef VK_ENABLE_BETA_EXTENSIONS
    table->CmdEncodeVideoKHR = (PFN_vkCmdEncodeVideoKHR) gpa(obj, "vkCmdEncodeVideoKHR");
#endif
    table->CreateCuModuleNVX = (PFN_vkCreateCuModuleNVX) gpa(obj, "vkCreateCuModuleNVX");
    table->CreateCuFunctionNVX = (PFN_vkCreateCuFunctionNVX) gpa(obj, "vkCreateCuFunctionNVX");
    table->DestroyCuModuleNVX = (PFN_vkDestroyCuModuleNVX) gpa(obj, "vkDestroyCuModuleNVX");
    table->DestroyCuFunctionNVX = (PFN_vkDestroyCuFunctionNVX) gpa(obj, "vkDestroyCuFunctionNVX");
    table->CmdCuLaunchKernelNVX = (PFN_vkCmdCuLaunchKernelNVX) gpa(obj, "vkCmdCuLaunchKernelNVX");
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
    table->GetImageSubresourceLayout2EXT = (PFN_vkGetImageSubresourceLayout2EXT) gpa(obj, "vkGetImageSubresourceLayout2EXT");
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
    { 475, 0x3c5ac109, 62 }, /* vkGetPhysicalDeviceCooperativeMatrixPropertiesNV */
    { 524, 0x8d00453f, 24 }, /* vkGetPhysicalDeviceDirectFBPresentationSupportEXT */
    { 574, 0xb7bc4386, 58 }, /* vkGetPhysicalDeviceDisplayPlaneProperties2KHR */
    { 620, 0xb9b8ddba, 11 }, /* vkGetPhysicalDeviceDisplayPlanePropertiesKHR */
    { 665, 0x540c0372, 57 }, /* vkGetPhysicalDeviceDisplayProperties2KHR */
    { 706, 0xfa0cd2e, 10 }, /* vkGetPhysicalDeviceDisplayPropertiesKHR */
    { 746, 0x944476dc, 41 }, /* vkGetPhysicalDeviceExternalBufferProperties */
    { 790, 0xee68b389, 42 }, /* vkGetPhysicalDeviceExternalBufferPropertiesKHR */
    { 837, 0x3bc965eb, 45 }, /* vkGetPhysicalDeviceExternalFenceProperties */
    { 880, 0x99b35492, 46 }, /* vkGetPhysicalDeviceExternalFencePropertiesKHR */
    { 926, 0xc8420c4c, 26 }, /* vkGetPhysicalDeviceExternalImageFormatPropertiesNV */
    { 977, 0xcf251b0e, 43 }, /* vkGetPhysicalDeviceExternalSemaphoreProperties */
    { 1024, 0x984c3fa7, 44 }, /* vkGetPhysicalDeviceExternalSemaphorePropertiesKHR */
    { 1074, 0x113e2f33, 3 }, /* vkGetPhysicalDeviceFeatures */
    { 1102, 0x63c068a7, 27 }, /* vkGetPhysicalDeviceFeatures2 */
    { 1131, 0x6a9a3636, 28 }, /* vkGetPhysicalDeviceFeatures2KHR */
    { 1163, 0x3e54b398, 4 }, /* vkGetPhysicalDeviceFormatProperties */
    { 1199, 0xca3bb9da, 31 }, /* vkGetPhysicalDeviceFormatProperties2 */
    { 1236, 0x9099cbbb, 32 }, /* vkGetPhysicalDeviceFormatProperties2KHR */
    { 1276, 0x6f0a9ed6, 69 }, /* vkGetPhysicalDeviceFragmentShadingRatesKHR */
    { 1319, 0xdd36a867, 5 }, /* vkGetPhysicalDeviceImageFormatProperties */
    { 1360, 0x35d260d3, 33 }, /* vkGetPhysicalDeviceImageFormatProperties2 */
    { 1402, 0x102ff7ea, 34 }, /* vkGetPhysicalDeviceImageFormatProperties2KHR */
    { 1447, 0xa90da4da, 2 }, /* vkGetPhysicalDeviceMemoryProperties */
    { 1483, 0xcb4cc208, 37 }, /* vkGetPhysicalDeviceMemoryProperties2 */
    { 1520, 0xc8c3da3d, 38 }, /* vkGetPhysicalDeviceMemoryProperties2KHR */
    { 1560, 0x219aa0b9, 54 }, /* vkGetPhysicalDeviceMultisamplePropertiesEXT */
    { 1604, 0x521044d1, 74 }, /* vkGetPhysicalDeviceOpticalFlowImageFormatsNV */
    { 1649, 0x100341b4, 53 }, /* vkGetPhysicalDevicePresentRectanglesKHR */
    { 1689, 0x52fe22c9, 0 }, /* vkGetPhysicalDeviceProperties */
    { 1719, 0x6c4d8ee1, 29 }, /* vkGetPhysicalDeviceProperties2 */
    { 1750, 0xcd15838c, 30 }, /* vkGetPhysicalDeviceProperties2KHR */
    { 1784, 0x7c7c9a0f, 65 }, /* vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR */
    { 1840, 0x4e5fc88a, 1 }, /* vkGetPhysicalDeviceQueueFamilyProperties */
    { 1881, 0xcad374d8, 35 }, /* vkGetPhysicalDeviceQueueFamilyProperties2 */
    { 1923, 0x5ceb2bed, 36 }, /* vkGetPhysicalDeviceQueueFamilyProperties2KHR */
    { 1968, 0xb5c7dc78, 25 }, /* vkGetPhysicalDeviceScreenPresentationSupportQNX */
    { 2016, 0x272ef8ef, 9 }, /* vkGetPhysicalDeviceSparseImageFormatProperties */
    { 2063, 0xebddba0b, 39 }, /* vkGetPhysicalDeviceSparseImageFormatProperties2 */
    { 2111, 0x8746ed72, 40 }, /* vkGetPhysicalDeviceSparseImageFormatProperties2KHR */
    { 2162, 0x432ca8f7, 66 }, /* vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV */
    { 2228, 0x5a5fba04, 52 }, /* vkGetPhysicalDeviceSurfaceCapabilities2EXT */
    { 2271, 0x9497e378, 55 }, /* vkGetPhysicalDeviceSurfaceCapabilities2KHR */
    { 2314, 0x77890558, 17 }, /* vkGetPhysicalDeviceSurfaceCapabilitiesKHR */
    { 2356, 0xd00b7188, 56 }, /* vkGetPhysicalDeviceSurfaceFormats2KHR */
    { 2394, 0xe32227c8, 18 }, /* vkGetPhysicalDeviceSurfaceFormatsKHR */
    { 2431, 0x33557b83, 63 }, /* vkGetPhysicalDeviceSurfacePresentModes2EXT */
    { 2474, 0x31c3cbd1, 19 }, /* vkGetPhysicalDeviceSurfacePresentModesKHR */
    { 2516, 0x1a687885, 16 }, /* vkGetPhysicalDeviceSurfaceSupportKHR */
    { 2553, 0x7b5f3fb9, 67 }, /* vkGetPhysicalDeviceToolProperties */
    { 2587, 0xd1685100, 68 }, /* vkGetPhysicalDeviceToolPropertiesEXT */
    { 2624, 0x8ee6bf8a, 70 }, /* vkGetPhysicalDeviceVideoCapabilitiesKHR */
    { 2664, 0xbb7625d6, 71 }, /* vkGetPhysicalDeviceVideoFormatPropertiesKHR */
    { 2708, 0x84e085ac, 20 }, /* vkGetPhysicalDeviceWaylandPresentationSupportKHR */
    { 2757, 0x80e72505, 21 }, /* vkGetPhysicalDeviceWin32PresentationSupportKHR */
    { 2804, 0x41782cb9, 23 }, /* vkGetPhysicalDeviceXcbPresentationSupportKHR */
    { 2849, 0x34a063ab, 22 }, /* vkGetPhysicalDeviceXlibPresentationSupportKHR */
    { 2895, 0xb87cdd6c, 49 }, /* vkGetRandROutputDisplayEXT */
    { 2922, 0x613996b5, 51 }, /* vkGetWinrtDisplayNV */
    { 2942, 0x4207f4f1, 47 }, /* vkReleaseDisplayEXT */
};

/* Hash table stats:
 * size 75 entries
 * collisions entries:
 *     0      53
 *     1      10
 *     2      8
 *     3      2
 *     4      0
 *     5      0
 *     6      0
 *     7      0
 *     8      1
 *     9+     1
 */

#define none 0xffff
static const uint16_t physical_device_string_map[128] = {
    0x0026,
    none,
    none,
    0x003d,
    0x0038,
    0x0036,
    0x0011,
    none,
    0x0027,
    0x000c,
    0x0030,
    0x0035,
    0x000b,
    0x0000,
    0x001a,
    0x002f,
    none,
    none,
    0x0018,
    0x0032,
    none,
    0x0007,
    none,
    none,
    0x001f,
    none,
    0x000e,
    0x003b,
    0x000f,
    0x0042,
    0x0039,
    0x002e,
    0x0002,
    none,
    none,
    none,
    0x000a,
    none,
    0x0041,
    0x001b,
    none,
    none,
    none,
    0x003f,
    0x0044,
    none,
    0x0014,
    0x0016,
    0x0046,
    none,
    none,
    0x001c,
    0x002b,
    0x0049,
    0x001e,
    none,
    none,
    0x0029,
    0x0012,
    0x0021,
    none,
    0x0028,
    0x0045,
    0x0003,
    none,
    0x0004,
    0x0008,
    0x0047,
    none,
    none,
    none,
    none,
    0x003c,
    0x002c,
    none,
    none,
    0x0019,
    0x0001,
    none,
    none,
    none,
    0x002a,
    0x0010,
    0x0024,
    none,
    none,
    0x0022,
    none,
    0x0031,
    none,
    0x0020,
    none,
    0x0015,
    none,
    0x0009,
    0x0040,
    0x001d,
    0x002d,
    none,
    none,
    0x003e,
    0x000d,
    none,
    0x0006,
    none,
    0x0043,
    0x0025,
    0x0017,
    0x0048,
    0x0005,
    none,
    0x0034,
    none,
    0x004a,
    0x0013,
    none,
    none,
    none,
    none,
    0x0037,
    0x0033,
    none,
    0x0023,
    none,
    none,
    none,
    0x003a,
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
    "vkCmdBindDescriptorSets\0"
    "vkCmdBindIndexBuffer\0"
    "vkCmdBindInvocationMaskHUAWEI\0"
    "vkCmdBindPipeline\0"
    "vkCmdBindPipelineShaderGroupNV\0"
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
    "vkCmdCopyMemoryToAccelerationStructureKHR\0"
    "vkCmdCopyMemoryToMicromapEXT\0"
    "vkCmdCopyMicromapEXT\0"
    "vkCmdCopyMicromapToMemoryEXT\0"
    "vkCmdCopyQueryPoolResults\0"
    "vkCmdCuLaunchKernelNVX\0"
    "vkCmdDebugMarkerBeginEXT\0"
    "vkCmdDebugMarkerEndEXT\0"
    "vkCmdDebugMarkerInsertEXT\0"
    "vkCmdDecodeVideoKHR\0"
    "vkCmdDispatch\0"
    "vkCmdDispatchBase\0"
    "vkCmdDispatchBaseKHR\0"
    "vkCmdDispatchIndirect\0"
    "vkCmdDraw\0"
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
    "vkCmdEncodeVideoKHR\0"
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
    "vkCmdSetDeviceMask\0"
    "vkCmdSetDeviceMaskKHR\0"
    "vkCmdSetDiscardRectangleEXT\0"
    "vkCmdSetEvent\0"
    "vkCmdSetEvent2\0"
    "vkCmdSetEvent2KHR\0"
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
    "vkCopyMemoryToAccelerationStructureKHR\0"
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
    "vkGetCalibratedTimestampsEXT\0"
    "vkGetDeferredOperationMaxConcurrencyKHR\0"
    "vkGetDeferredOperationResultKHR\0"
    "vkGetDescriptorSetHostMappingVALVE\0"
    "vkGetDescriptorSetLayoutHostMappingInfoVALVE\0"
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
    "vkGetImageSparseMemoryRequirements\0"
    "vkGetImageSparseMemoryRequirements2\0"
    "vkGetImageSparseMemoryRequirements2KHR\0"
    "vkGetImageSubresourceLayout\0"
    "vkGetImageSubresourceLayout2EXT\0"
    "vkGetImageViewAddressNVX\0"
    "vkGetImageViewHandleNVX\0"
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
    "vkGetSemaphoreCounterValue\0"
    "vkGetSemaphoreCounterValueKHR\0"
    "vkGetSemaphoreFdKHR\0"
    "vkGetSemaphoreWin32HandleKHR\0"
    "vkGetSemaphoreZirconHandleFUCHSIA\0"
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
    "vkMapMemory\0"
    "vkMergePipelineCaches\0"
    "vkMergeValidationCachesEXT\0"
    "vkQueueBeginDebugUtilsLabelEXT\0"
    "vkQueueBindSparse\0"
    "vkQueueEndDebugUtilsLabelEXT\0"
    "vkQueueInsertDebugUtilsLabelEXT\0"
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
    "vkSetLocalDimmingAMD\0"
    "vkSetPrivateData\0"
    "vkSetPrivateDataEXT\0"
    "vkSignalSemaphore\0"
    "vkSignalSemaphoreKHR\0"
    "vkTrimCommandPool\0"
    "vkTrimCommandPoolKHR\0"
    "vkUninitializePerformanceApiINTEL\0"
    "vkUnmapMemory\0"
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
    { 0, 0x8a43a1cc, 312 }, /* vkAcquireFullScreenExclusiveModeEXT */
    { 36, 0x6bf780dd, 223 }, /* vkAcquireImageANDROID */
    { 58, 0x82860572, 181 }, /* vkAcquireNextImage2KHR */
    { 81, 0xc3fedb2e, 133 }, /* vkAcquireNextImageKHR */
    { 103, 0x33d2767, 327 }, /* vkAcquirePerformanceConfigurationINTEL */
    { 142, 0xaf1d64ad, 314 }, /* vkAcquireProfilingLockKHR */
    { 168, 0x8c0c811a, 75 }, /* vkAllocateCommandBuffers */
    { 193, 0x4c449d3a, 64 }, /* vkAllocateDescriptorSets */
    { 218, 0x522b85d3, 6 }, /* vkAllocateMemory */
    { 235, 0xc54f7327, 77 }, /* vkBeginCommandBuffer */
    { 256, 0x3ec4e21a, 284 }, /* vkBindAccelerationStructureMemoryNV */
    { 292, 0x6bcbdcb, 14 }, /* vkBindBufferMemory */
    { 311, 0xc27aaf4f, 173 }, /* vkBindBufferMemory2 */
    { 331, 0x6878d3ce, 174 }, /* vkBindBufferMemory2KHR */
    { 354, 0x5caaae4a, 16 }, /* vkBindImageMemory */
    { 372, 0xa9097118, 175 }, /* vkBindImageMemory2 */
    { 391, 0xf18729ad, 176 }, /* vkBindImageMemory2KHR */
    { 413, 0xc3b6afe1, 502 }, /* vkBindOpticalFlowSessionImageNV */
    { 445, 0x61c0a1e7, 455 }, /* vkBindVideoSessionMemoryKHR */
    { 473, 0xf7d6c55c, 340 }, /* vkBuildAccelerationStructuresKHR */
    { 506, 0x9aa65b87, 481 }, /* vkBuildMicromapsEXT */
    { 526, 0xe561c19f, 119 }, /* vkCmdBeginConditionalRenderingEXT */
    { 560, 0x6184193f, 233 }, /* vkCmdBeginDebugUtilsLabelEXT */
    { 589, 0xf5064ea4, 117 }, /* vkCmdBeginQuery */
    { 605, 0x73251a2c, 265 }, /* vkCmdBeginQueryIndexedEXT */
    { 631, 0xcb7a58e3, 125 }, /* vkCmdBeginRenderPass */
    { 652, 0x9c876577, 240 }, /* vkCmdBeginRenderPass2 */
    { 674, 0x8b6b4de6, 241 }, /* vkCmdBeginRenderPass2KHR */
    { 699, 0x385cfdb8, 473 }, /* vkCmdBeginRendering */
    { 719, 0x50d17e0d, 474 }, /* vkCmdBeginRenderingKHR */
    { 742, 0xb217c94, 263 }, /* vkCmdBeginTransformFeedbackEXT */
    { 773, 0xd941eabc, 457 }, /* vkCmdBeginVideoCodingKHR */
    { 798, 0x28c7a5da, 90 }, /* vkCmdBindDescriptorSets */
    { 822, 0x4c22d870, 91 }, /* vkCmdBindIndexBuffer */
    { 843, 0xa8f55bdd, 280 }, /* vkCmdBindInvocationMaskHUAWEI */
    { 873, 0x3af9fd84, 80 }, /* vkCmdBindPipeline */
    { 891, 0x353570d6, 143 }, /* vkCmdBindPipelineShaderGroupNV */
    { 922, 0xbae753eb, 269 }, /* vkCmdBindShadingRateImageNV */
    { 950, 0x98fdb5cd, 262 }, /* vkCmdBindTransformFeedbackBuffersEXT */
    { 987, 0xa9c83f1d, 92 }, /* vkCmdBindVertexBuffers */
    { 1010, 0x4c6b42ed, 357 }, /* vkCmdBindVertexBuffers2 */
    { 1034, 0x30a5f2ec, 358 }, /* vkCmdBindVertexBuffers2EXT */
    { 1061, 0x331ebf89, 104 }, /* vkCmdBlitImage */
    { 1076, 0xb4185c21, 422 }, /* vkCmdBlitImage2 */
    { 1092, 0x785f984c, 423 }, /* vkCmdBlitImage2KHR */
    { 1111, 0x8e9d180a, 294 }, /* vkCmdBuildAccelerationStructureNV */
    { 1145, 0xb02d6bee, 339 }, /* vkCmdBuildAccelerationStructuresIndirectKHR */
    { 1189, 0x25a727dc, 338 }, /* vkCmdBuildAccelerationStructuresKHR */
    { 1225, 0x331d3d07, 480 }, /* vkCmdBuildMicromapsEXT */
    { 1248, 0x93cb5cb8, 111 }, /* vkCmdClearAttachments */
    { 1270, 0xb4bc8d08, 109 }, /* vkCmdClearColorImage */
    { 1291, 0x4f88e4ba, 110 }, /* vkCmdClearDepthStencilImage */
    { 1319, 0x2a5f6f70, 458 }, /* vkCmdControlVideoCodingKHR */
    { 1346, 0xe2d4fe2c, 286 }, /* vkCmdCopyAccelerationStructureKHR */
    { 1380, 0x84ab5629, 285 }, /* vkCmdCopyAccelerationStructureNV */
    { 1413, 0x46b2a8a0, 288 }, /* vkCmdCopyAccelerationStructureToMemoryKHR */
    { 1455, 0xc939a0da, 102 }, /* vkCmdCopyBuffer */
    { 1471, 0xa419e608, 418 }, /* vkCmdCopyBuffer2 */
    { 1488, 0x90c5563d, 419 }, /* vkCmdCopyBuffer2KHR */
    { 1508, 0x929847e, 105 }, /* vkCmdCopyBufferToImage */
    { 1531, 0x4a8ce444, 424 }, /* vkCmdCopyBufferToImage2 */
    { 1555, 0x1e9f6861, 425 }, /* vkCmdCopyBufferToImage2KHR */
    { 1582, 0x278effa9, 103 }, /* vkCmdCopyImage */
    { 1597, 0x942b5301, 420 }, /* vkCmdCopyImage2 */
    { 1613, 0xdad52c6c, 421 }, /* vkCmdCopyImage2KHR */
    { 1632, 0x68cddbac, 106 }, /* vkCmdCopyImageToBuffer */
    { 1655, 0x83c9426, 426 }, /* vkCmdCopyImageToBuffer2 */
    { 1679, 0x2db6484f, 427 }, /* vkCmdCopyImageToBuffer2KHR */
    { 1706, 0xa76c5fd8, 290 }, /* vkCmdCopyMemoryToAccelerationStructureKHR */
    { 1748, 0x47d6c41b, 487 }, /* vkCmdCopyMemoryToMicromapEXT */
    { 1777, 0x8c3811e7, 483 }, /* vkCmdCopyMicromapEXT */
    { 1798, 0x715dff1b, 485 }, /* vkCmdCopyMicromapToMemoryEXT */
    { 1827, 0xdee8c6d4, 123 }, /* vkCmdCopyQueryPoolResults */
    { 1853, 0x29000809, 465 }, /* vkCmdCuLaunchKernelNVX */
    { 1876, 0xaec8bb10, 137 }, /* vkCmdDebugMarkerBeginEXT */
    { 1901, 0xd6a1433e, 138 }, /* vkCmdDebugMarkerEndEXT */
    { 1924, 0x173d440, 139 }, /* vkCmdDebugMarkerInsertEXT */
    { 1950, 0x5c38928d, 456 }, /* vkCmdDecodeVideoKHR */
    { 1970, 0xbd58e867, 99 }, /* vkCmdDispatch */
    { 1984, 0xfb767220, 182 }, /* vkCmdDispatchBase */
    { 2002, 0x402403e5, 183 }, /* vkCmdDispatchBaseKHR */
    { 2023, 0xd6353005, 100 }, /* vkCmdDispatchIndirect */
    { 2045, 0x9912c1a1, 93 }, /* vkCmdDraw */
    { 2055, 0xbe5a8058, 94 }, /* vkCmdDrawIndexed */
    { 2072, 0x94e7ed36, 98 }, /* vkCmdDrawIndexedIndirect */
    { 2097, 0xb4acef41, 257 }, /* vkCmdDrawIndexedIndirectCount */
    { 2127, 0xc86e9287, 259 }, /* vkCmdDrawIndexedIndirectCountAMD */
    { 2160, 0xda9e8a2c, 258 }, /* vkCmdDrawIndexedIndirectCountKHR */
    { 2193, 0xe9ac41bf, 97 }, /* vkCmdDrawIndirect */
    { 2211, 0x80c3b089, 267 }, /* vkCmdDrawIndirectByteCountEXT */
    { 2241, 0x40079990, 254 }, /* vkCmdDrawIndirectCount */
    { 2264, 0xe5ad0a50, 256 }, /* vkCmdDrawIndirectCountAMD */
    { 2290, 0xf7dd01f5, 255 }, /* vkCmdDrawIndirectCountKHR */
    { 2316, 0xfa045961, 275 }, /* vkCmdDrawMeshTasksEXT */
    { 2338, 0xac310210, 277 }, /* vkCmdDrawMeshTasksIndirectCountEXT */
    { 2373, 0xf21f391, 274 }, /* vkCmdDrawMeshTasksIndirectCountNV */
    { 2407, 0x6c0d4cb3, 276 }, /* vkCmdDrawMeshTasksIndirectEXT */
    { 2437, 0xaecd0a06, 273 }, /* vkCmdDrawMeshTasksIndirectNV */
    { 2466, 0xfba21ac8, 272 }, /* vkCmdDrawMeshTasksNV */
    { 2487, 0xa6c231d9, 95 }, /* vkCmdDrawMultiEXT */
    { 2505, 0xb6bd0f40, 96 }, /* vkCmdDrawMultiIndexedEXT */
    { 2530, 0x552d044d, 460 }, /* vkCmdEncodeVideoKHR */
    { 2550, 0x18c8217d, 120 }, /* vkCmdEndConditionalRenderingEXT */
    { 2582, 0x29875911, 234 }, /* vkCmdEndDebugUtilsLabelEXT */
    { 2609, 0xd556fd22, 118 }, /* vkCmdEndQuery */
    { 2623, 0xd5c2f48a, 266 }, /* vkCmdEndQueryIndexedEXT */
    { 2647, 0xdcdb0235, 127 }, /* vkCmdEndRenderPass */
    { 2666, 0x1cbf9115, 244 }, /* vkCmdEndRenderPass2 */
    { 2686, 0x57eebe78, 245 }, /* vkCmdEndRenderPass2KHR */
    { 2709, 0x22c5e6f6, 475 }, /* vkCmdEndRendering */
    { 2727, 0xabf9ff, 476 }, /* vkCmdEndRenderingKHR */
    { 2748, 0xf008d706, 264 }, /* vkCmdEndTransformFeedbackEXT */
    { 2777, 0xa5c55b4e, 459 }, /* vkCmdEndVideoCodingKHR */
    { 2800, 0x9eaabe40, 128 }, /* vkCmdExecuteCommands */
    { 2821, 0xe02372d7, 141 }, /* vkCmdExecuteGeneratedCommandsNV */
    { 2853, 0x5bdd2ae0, 108 }, /* vkCmdFillBuffer */
    { 2869, 0xce6aa7d1, 235 }, /* vkCmdInsertDebugUtilsLabelEXT */
    { 2899, 0x2eeec2f9, 126 }, /* vkCmdNextSubpass */
    { 2916, 0xd4fc131, 242 }, /* vkCmdNextSubpass2 */
    { 2934, 0x25b621bc, 243 }, /* vkCmdNextSubpass2KHR */
    { 2955, 0x9fa9b32c, 503 }, /* vkCmdOpticalFlowExecuteNV */
    { 2981, 0x97fccfe8, 116 }, /* vkCmdPipelineBarrier */
    { 3002, 0x43d8c70a, 441 }, /* vkCmdPipelineBarrier2 */
    { 3024, 0x9654ba0b, 442 }, /* vkCmdPipelineBarrier2KHR */
    { 3049, 0x26eff1e6, 142 }, /* vkCmdPreprocessGeneratedCommandsNV */
    { 3084, 0xb1c6b468, 124 }, /* vkCmdPushConstants */
    { 3103, 0xf17232a1, 147 }, /* vkCmdPushDescriptorSetKHR */
    { 3129, 0x3d528981, 190 }, /* vkCmdPushDescriptorSetWithTemplateKHR */
    { 3167, 0x4fccce28, 114 }, /* vkCmdResetEvent */
    { 3183, 0x735fc6ca, 437 }, /* vkCmdResetEvent2 */
    { 3200, 0x950a204b, 438 }, /* vkCmdResetEvent2KHR */
    { 3220, 0x2f614082, 121 }, /* vkCmdResetQueryPool */
    { 3240, 0x671bb594, 112 }, /* vkCmdResolveImage */
    { 3258, 0xe79d80fe, 428 }, /* vkCmdResolveImage2 */
    { 3277, 0x9fea6337, 429 }, /* vkCmdResolveImage2KHR */
    { 3299, 0x8344384a, 385 }, /* vkCmdSetAlphaToCoverageEnableEXT */
    { 3332, 0x246d31e4, 386 }, /* vkCmdSetAlphaToOneEnableEXT */
    { 3360, 0x1c989dfb, 85 }, /* vkCmdSetBlendConstants */
    { 3383, 0x4331556d, 260 }, /* vkCmdSetCheckpointNV */
    { 3404, 0xcd76e1c0, 271 }, /* vkCmdSetCoarseSampleOrderNV */
    { 3432, 0x67adf3f4, 396 }, /* vkCmdSetColorBlendAdvancedEXT */
    { 3462, 0x7242b4bb, 388 }, /* vkCmdSetColorBlendEnableEXT */
    { 3490, 0x6828ae82, 389 }, /* vkCmdSetColorBlendEquationEXT */
    { 3520, 0x5402d31d, 434 }, /* vkCmdSetColorWriteEnableEXT */
    { 3548, 0x80a5014c, 390 }, /* vkCmdSetColorWriteMaskEXT */
    { 3574, 0xbd4e36a9, 392 }, /* vkCmdSetConservativeRasterizationModeEXT */
    { 3615, 0xd1185034, 405 }, /* vkCmdSetCoverageModulationModeNV */
    { 3648, 0x58921b18, 406 }, /* vkCmdSetCoverageModulationTableEnableNV */
    { 3688, 0xc32a1ef5, 407 }, /* vkCmdSetCoverageModulationTableNV */
    { 3722, 0x64509d31, 409 }, /* vkCmdSetCoverageReductionModeNV */
    { 3754, 0xb956bfce, 403 }, /* vkCmdSetCoverageToColorEnableNV */
    { 3786, 0xf35aaa00, 404 }, /* vkCmdSetCoverageToColorLocationNV */
    { 3820, 0xcf691c42, 347 }, /* vkCmdSetCullMode */
    { 3837, 0xb7fcea1f, 348 }, /* vkCmdSetCullModeEXT */
    { 3857, 0x30f14d07, 84 }, /* vkCmdSetDepthBias */
    { 3875, 0xc07e1b1a, 374 }, /* vkCmdSetDepthBiasEnable */
    { 3899, 0x5d604307, 375 }, /* vkCmdSetDepthBiasEnableEXT */
    { 3926, 0x7b3a8a63, 86 }, /* vkCmdSetDepthBounds */
    { 3946, 0xe72cce0, 365 }, /* vkCmdSetDepthBoundsTestEnable */
    { 3976, 0x3f2ddb1, 366 }, /* vkCmdSetDepthBoundsTestEnableEXT */
    { 4009, 0x6a365461, 381 }, /* vkCmdSetDepthClampEnableEXT */
    { 4037, 0x88cb8180, 394 }, /* vkCmdSetDepthClipEnableEXT */
    { 4064, 0x69601111, 400 }, /* vkCmdSetDepthClipNegativeOneToOneEXT */
    { 4101, 0xda98add0, 363 }, /* vkCmdSetDepthCompareOp */
    { 4124, 0x2f377e41, 364 }, /* vkCmdSetDepthCompareOpEXT */
    { 4150, 0x68666de3, 359 }, /* vkCmdSetDepthTestEnable */
    { 4174, 0x57c5efe6, 360 }, /* vkCmdSetDepthTestEnableEXT */
    { 4201, 0x373bda6c, 361 }, /* vkCmdSetDepthWriteEnable */
    { 4226, 0xbe217905, 362 }, /* vkCmdSetDepthWriteEnableEXT */
    { 4254, 0xaecdae87, 177 }, /* vkCmdSetDeviceMask */
    { 4273, 0xfbb79356, 178 }, /* vkCmdSetDeviceMaskKHR */
    { 4295, 0x64df188b, 196 }, /* vkCmdSetDiscardRectangleEXT */
    { 4323, 0xe257f075, 113 }, /* vkCmdSetEvent */
    { 4337, 0x3d5620d5, 435 }, /* vkCmdSetEvent2 */
    { 4352, 0xa3c714b8, 436 }, /* vkCmdSetEvent2KHR */
    { 4370, 0xb2537e63, 268 }, /* vkCmdSetExclusiveScissorNV */
    { 4397, 0x22d38855, 393 }, /* vkCmdSetExtraPrimitiveOverestimationSizeEXT */
    { 4441, 0x7670296e, 431 }, /* vkCmdSetFragmentShadingRateEnumNV */
    { 4475, 0x4c696cd8, 430 }, /* vkCmdSetFragmentShadingRateKHR */
    { 4506, 0x4cd999a9, 349 }, /* vkCmdSetFrontFace */
    { 4524, 0xa7a7a090, 350 }, /* vkCmdSetFrontFaceEXT */
    { 4545, 0x82fb3a20, 398 }, /* vkCmdSetLineRasterizationModeEXT */
    { 4578, 0xbdaa62f9, 336 }, /* vkCmdSetLineStippleEXT */
    { 4601, 0x36f713ae, 399 }, /* vkCmdSetLineStippleEnableEXT */
    { 4630, 0x32282165, 83 }, /* vkCmdSetLineWidth */
    { 4648, 0x7689581f, 376 }, /* vkCmdSetLogicOpEXT */
    { 4667, 0xc1fbf774, 387 }, /* vkCmdSetLogicOpEnableEXT */
    { 4692, 0x58604abc, 371 }, /* vkCmdSetPatchControlPointsEXT */
    { 4722, 0x4eb21af9, 324 }, /* vkCmdSetPerformanceMarkerINTEL */
    { 4753, 0x30d793c7, 326 }, /* vkCmdSetPerformanceOverrideINTEL */
    { 4786, 0xc50b03a9, 325 }, /* vkCmdSetPerformanceStreamMarkerINTEL */
    { 4823, 0x966edf9, 382 }, /* vkCmdSetPolygonModeEXT */
    { 4846, 0x2b3504c0, 377 }, /* vkCmdSetPrimitiveRestartEnable */
    { 4877, 0x28d998d1, 378 }, /* vkCmdSetPrimitiveRestartEnableEXT */
    { 4911, 0xb9524b01, 351 }, /* vkCmdSetPrimitiveTopology */
    { 4937, 0x1dacaf8, 352 }, /* vkCmdSetPrimitiveTopologyEXT */
    { 4966, 0x710ab2e2, 397 }, /* vkCmdSetProvokingVertexModeEXT */
    { 4997, 0xbe15d782, 383 }, /* vkCmdSetRasterizationSamplesEXT */
    { 5029, 0xc1c028f, 391 }, /* vkCmdSetRasterizationStreamEXT */
    { 5060, 0x81319b79, 372 }, /* vkCmdSetRasterizerDiscardEnable */
    { 5092, 0x1f7bb40, 373 }, /* vkCmdSetRasterizerDiscardEnableEXT */
    { 5127, 0xd056ef9b, 308 }, /* vkCmdSetRayTracingPipelineStackSizeKHR */
    { 5166, 0xb8eb12ff, 410 }, /* vkCmdSetRepresentativeFragmentTestEnableNV */
    { 5209, 0xa9e2c72, 197 }, /* vkCmdSetSampleLocationsEXT */
    { 5236, 0x3b21c717, 395 }, /* vkCmdSetSampleLocationsEnableEXT */
    { 5269, 0xd997f166, 384 }, /* vkCmdSetSampleMaskEXT */
    { 5291, 0x48f28c7f, 82 }, /* vkCmdSetScissor */
    { 5307, 0x159097b2, 355 }, /* vkCmdSetScissorWithCount */
    { 5332, 0xf349b42f, 356 }, /* vkCmdSetScissorWithCountEXT */
    { 5360, 0x9300b169, 408 }, /* vkCmdSetShadingRateImageEnableNV */
    { 5393, 0xa8f534e2, 87 }, /* vkCmdSetStencilCompareMask */
    { 5420, 0x43020f38, 369 }, /* vkCmdSetStencilOp */
    { 5438, 0xbb885f19, 370 }, /* vkCmdSetStencilOpEXT */
    { 5459, 0x83e2b024, 89 }, /* vkCmdSetStencilReference */
    { 5484, 0x63fedc5c, 367 }, /* vkCmdSetStencilTestEnable */
    { 5510, 0x16cc6095, 368 }, /* vkCmdSetStencilTestEnableEXT */
    { 5539, 0xe7c4b134, 88 }, /* vkCmdSetStencilWriteMask */
    { 5564, 0xf2d7ac79, 380 }, /* vkCmdSetTessellationDomainOriginEXT */
    { 5600, 0x9dd954c8, 433 }, /* vkCmdSetVertexInputEXT */
    { 5623, 0x53d6c2b, 81 }, /* vkCmdSetViewport */
    { 5640, 0x54d063a4, 270 }, /* vkCmdSetViewportShadingRatePaletteNV */
    { 5677, 0x5118219b, 402 }, /* vkCmdSetViewportSwizzleNV */
    { 5703, 0xad68ff96, 401 }, /* vkCmdSetViewportWScalingEnableNV */
    { 5736, 0x60ee2453, 195 }, /* vkCmdSetViewportWScalingNV */
    { 5763, 0xbdea58a6, 353 }, /* vkCmdSetViewportWithCount */
    { 5789, 0xa3d72e5b, 354 }, /* vkCmdSetViewportWithCountEXT */
    { 5818, 0xf2c7909d, 101 }, /* vkCmdSubpassShadingHUAWEI */
    { 5844, 0x519b0602, 305 }, /* vkCmdTraceRaysIndirect2KHR */
    { 5871, 0xaf8c1f1e, 304 }, /* vkCmdTraceRaysIndirectKHR */
    { 5897, 0x5eb65f0c, 296 }, /* vkCmdTraceRaysKHR */
    { 5915, 0xe8687c49, 297 }, /* vkCmdTraceRaysNV */
    { 5932, 0xd2986b5e, 107 }, /* vkCmdUpdateBuffer */
    { 5950, 0x3b9346b3, 115 }, /* vkCmdWaitEvents */
    { 5966, 0xcd17b527, 439 }, /* vkCmdWaitEvents2 */
    { 5983, 0x8c98fdb6, 440 }, /* vkCmdWaitEvents2KHR */
    { 6003, 0x9ecb3888, 292 }, /* vkCmdWriteAccelerationStructuresPropertiesKHR */
    { 6049, 0xd2925ead, 293 }, /* vkCmdWriteAccelerationStructuresPropertiesNV */
    { 6094, 0xe277f952, 447 }, /* vkCmdWriteBufferMarker2AMD */
    { 6121, 0x447be82c, 237 }, /* vkCmdWriteBufferMarkerAMD */
    { 6147, 0xa6efed5d, 489 }, /* vkCmdWriteMicromapsPropertiesEXT */
    { 6180, 0xec4d324c, 122 }, /* vkCmdWriteTimestamp */
    { 6200, 0xcff32086, 445 }, /* vkCmdWriteTimestamp2 */
    { 6221, 0xa43a02ef, 446 }, /* vkCmdWriteTimestamp2KHR */
    { 6245, 0xbad693ed, 278 }, /* vkCompileDeferredNV */
    { 6265, 0x45e623ac, 287 }, /* vkCopyAccelerationStructureKHR */
    { 6296, 0x9726ae20, 289 }, /* vkCopyAccelerationStructureToMemoryKHR */
    { 6335, 0xf7e06558, 291 }, /* vkCopyMemoryToAccelerationStructureKHR */
    { 6374, 0xa8f9ba9b, 488 }, /* vkCopyMemoryToMicromapEXT */
    { 6400, 0x5aa2e867, 484 }, /* vkCopyMicromapEXT */
    { 6418, 0xd280f59b, 486 }, /* vkCopyMicromapToMemoryEXT */
    { 6444, 0x30dceabb, 337 }, /* vkCreateAccelerationStructureKHR */
    { 6477, 0x9dc98a12, 279 }, /* vkCreateAccelerationStructureNV */
    { 6509, 0x7d4282b9, 36 }, /* vkCreateBuffer */
    { 6524, 0xaba0b50, 468 }, /* vkCreateBufferCollectionFUCHSIA */
    { 6556, 0x925bd256, 38 }, /* vkCreateBufferView */
    { 6575, 0x820fe476, 72 }, /* vkCreateCommandPool */
    { 6595, 0xf70c85eb, 52 }, /* vkCreateComputePipelines */
    { 6620, 0x9bff3a5d, 462 }, /* vkCreateCuFunctionNVX */
    { 6642, 0xf3c09939, 461 }, /* vkCreateCuModuleNVX */
    { 6662, 0x3f5d1a36, 342 }, /* vkCreateDeferredOperationKHR */
    { 6691, 0xfb95a8a4, 61 }, /* vkCreateDescriptorPool */
    { 6714, 0x3c14cc74, 59 }, /* vkCreateDescriptorSetLayout */
    { 6742, 0xad3ce733, 184 }, /* vkCreateDescriptorUpdateTemplate */
    { 6775, 0x5189488a, 185 }, /* vkCreateDescriptorUpdateTemplateKHR */
    { 6811, 0xe7188731, 26 }, /* vkCreateEvent */
    { 6825, 0x958af968, 19 }, /* vkCreateFence */
    { 6839, 0x887a38c4, 67 }, /* vkCreateFramebuffer */
    { 6859, 0x4b59f96d, 51 }, /* vkCreateGraphicsPipelines */
    { 6885, 0x652128c2, 40 }, /* vkCreateImage */
    { 6899, 0xdce077ff, 43 }, /* vkCreateImageView */
    { 6917, 0x3bc09b11, 145 }, /* vkCreateIndirectCommandsLayoutNV */
    { 6950, 0x4dea1af0, 479 }, /* vkCreateMicromapEXT */
    { 6970, 0x7a80ce69, 500 }, /* vkCreateOpticalFlowSessionNV */
    { 6999, 0xcbf6489f, 47 }, /* vkCreatePipelineCache */
    { 7021, 0x451ef1ed, 55 }, /* vkCreatePipelineLayout */
    { 7044, 0xd1450d02, 379 }, /* vkCreatePrivateDataSlot */
    { 7068, 0xc06d475f, 411 }, /* vkCreatePrivateDataSlotEXT */
    { 7095, 0x5edcd92b, 31 }, /* vkCreateQueryPool */
    { 7113, 0x28847755, 303 }, /* vkCreateRayTracingPipelinesKHR */
    { 7144, 0x12bca48, 302 }, /* vkCreateRayTracingPipelinesNV */
    { 7174, 0x109a9c18, 69 }, /* vkCreateRenderPass */
    { 7193, 0x46b16d5a, 238 }, /* vkCreateRenderPass2 */
    { 7213, 0xfa16043b, 239 }, /* vkCreateRenderPass2KHR */
    { 7236, 0x13cf03f, 57 }, /* vkCreateSampler */
    { 7252, 0xe6a58c26, 210 }, /* vkCreateSamplerYcbcrConversion */
    { 7283, 0x7482104f, 211 }, /* vkCreateSamplerYcbcrConversionKHR */
    { 7317, 0xf2065e5b, 24 }, /* vkCreateSemaphore */
    { 7335, 0xa0d3cea2, 45 }, /* vkCreateShaderModule */
    { 7356, 0x47655c4a, 129 }, /* vkCreateSharedSwapchainsKHR */
    { 7384, 0xcdefcaa8, 130 }, /* vkCreateSwapchainKHR */
    { 7405, 0x591d7ed9, 215 }, /* vkCreateValidationCacheEXT */
    { 7432, 0xcddb2969, 449 }, /* vkCreateVideoSessionKHR */
    { 7456, 0x83987bd7, 451 }, /* vkCreateVideoSessionParametersKHR */
    { 7490, 0xe206fb25, 135 }, /* vkDebugMarkerSetObjectNameEXT */
    { 7520, 0x30799448, 136 }, /* vkDebugMarkerSetObjectTagEXT */
    { 7549, 0x8c8648b8, 346 }, /* vkDeferredOperationJoinKHR */
    { 7576, 0x3eccc207, 281 }, /* vkDestroyAccelerationStructureKHR */
    { 7610, 0x693f9d26, 282 }, /* vkDestroyAccelerationStructureNV */
    { 7643, 0x94a07a45, 37 }, /* vkDestroyBuffer */
    { 7659, 0xd6301e64, 471 }, /* vkDestroyBufferCollectionFUCHSIA */
    { 7692, 0x98b27962, 39 }, /* vkDestroyBufferView */
    { 7712, 0xd5d83a0a, 73 }, /* vkDestroyCommandPool */
    { 7733, 0x111b9831, 464 }, /* vkDestroyCuFunctionNVX */
    { 7756, 0x4788eecd, 463 }, /* vkDestroyCuModuleNVX */
    { 7777, 0x7d549a02, 343 }, /* vkDestroyDeferredOperationKHR */
    { 7807, 0x47bdaf30, 62 }, /* vkDestroyDescriptorPool */
    { 7831, 0xa4227b08, 60 }, /* vkDestroyDescriptorSetLayout */
    { 7860, 0xbb2cbe7f, 186 }, /* vkDestroyDescriptorUpdateTemplate */
    { 7894, 0xaa83901e, 187 }, /* vkDestroyDescriptorUpdateTemplateKHR */
    { 7931, 0x1fbcc9cb, 1 }, /* vkDestroyDevice */
    { 7947, 0x4df27c05, 27 }, /* vkDestroyEvent */
    { 7962, 0xfc64ee3c, 20 }, /* vkDestroyFence */
    { 7977, 0xdc428e58, 68 }, /* vkDestroyFramebuffer */
    { 7998, 0xcbfb1d96, 41 }, /* vkDestroyImage */
    { 8013, 0xb5853953, 44 }, /* vkDestroyImageView */
    { 8032, 0x49b0725d, 146 }, /* vkDestroyIndirectCommandsLayoutNV */
    { 8066, 0xa1b27084, 482 }, /* vkDestroyMicromapEXT */
    { 8087, 0xb8784e35, 501 }, /* vkDestroyOpticalFlowSessionNV */
    { 8117, 0x6aac68af, 54 }, /* vkDestroyPipeline */
    { 8135, 0x4112a673, 48 }, /* vkDestroyPipelineCache */
    { 8158, 0x9146f879, 56 }, /* vkDestroyPipelineLayout */
    { 8182, 0x9fc42716, 412 }, /* vkDestroyPrivateDataSlot */
    { 8207, 0xe18d5d6b, 413 }, /* vkDestroyPrivateDataSlotEXT */
    { 8235, 0x37819a7f, 32 }, /* vkDestroyQueryPool */
    { 8254, 0x16f14324, 70 }, /* vkDestroyRenderPass */
    { 8274, 0x3b645153, 58 }, /* vkDestroySampler */
    { 8291, 0x20f261b2, 212 }, /* vkDestroySamplerYcbcrConversion */
    { 8323, 0xaaa623a3, 213 }, /* vkDestroySamplerYcbcrConversionKHR */
    { 8358, 0xcaab1faf, 25 }, /* vkDestroySemaphore */
    { 8377, 0x2d77af6e, 46 }, /* vkDestroyShaderModule */
    { 8399, 0x5a93ab74, 131 }, /* vkDestroySwapchainKHR */
    { 8421, 0x7a3d94e5, 216 }, /* vkDestroyValidationCacheEXT */
    { 8449, 0x9c5a437d, 450 }, /* vkDestroyVideoSessionKHR */
    { 8474, 0xb9bc8f2b, 453 }, /* vkDestroyVideoSessionParametersKHR */
    { 8509, 0xd46c5f24, 5 }, /* vkDeviceWaitIdle */
    { 8526, 0xdbb064, 167 }, /* vkDisplayPowerControlEXT */
    { 8551, 0xaffb5725, 78 }, /* vkEndCommandBuffer */
    { 8570, 0xa59ad883, 497 }, /* vkExportMetalObjectsEXT */
    { 8594, 0xff52f051, 10 }, /* vkFlushMappedMemoryRanges */
    { 8620, 0xb9db2b91, 76 }, /* vkFreeCommandBuffers */
    { 8641, 0x7a1347b1, 65 }, /* vkFreeDescriptorSets */
    { 8662, 0x8f6f838a, 7 }, /* vkFreeMemory */
    { 8675, 0x4dbe8d2f, 432 }, /* vkGetAccelerationStructureBuildSizesKHR */
    { 8715, 0x1a50de81, 341 }, /* vkGetAccelerationStructureDeviceAddressKHR */
    { 8758, 0xd26f255a, 301 }, /* vkGetAccelerationStructureHandleNV */
    { 8793, 0x5d79203, 283 }, /* vkGetAccelerationStructureMemoryRequirementsNV */
    { 8840, 0xb891b5e, 252 }, /* vkGetAndroidHardwareBufferPropertiesANDROID */
    { 8884, 0x14b1e93d, 472 }, /* vkGetBufferCollectionPropertiesFUCHSIA */
    { 8923, 0x7022f0cd, 319 }, /* vkGetBufferDeviceAddress */
    { 8948, 0x3703280c, 321 }, /* vkGetBufferDeviceAddressEXT */
    { 8976, 0x713b5180, 320 }, /* vkGetBufferDeviceAddressKHR */
    { 9004, 0xab98422a, 13 }, /* vkGetBufferMemoryRequirements */
    { 9034, 0xd1fd0638, 198 }, /* vkGetBufferMemoryRequirements2 */
    { 9065, 0x78dbe98d, 199 }, /* vkGetBufferMemoryRequirements2KHR */
    { 9099, 0x2a5545a0, 317 }, /* vkGetBufferOpaqueCaptureAddress */
    { 9131, 0xddac1c65, 318 }, /* vkGetBufferOpaqueCaptureAddressKHR */
    { 9166, 0xcf3070fe, 227 }, /* vkGetCalibratedTimestampsEXT */
    { 9195, 0x7d902967, 344 }, /* vkGetDeferredOperationMaxConcurrencyKHR */
    { 9235, 0xf2144be9, 345 }, /* vkGetDeferredOperationResultKHR */
    { 9267, 0x19faddac, 478 }, /* vkGetDescriptorSetHostMappingVALVE */
    { 9302, 0x5485f810, 477 }, /* vkGetDescriptorSetLayoutHostMappingInfoVALVE */
    { 9347, 0xfeac9573, 219 }, /* vkGetDescriptorSetLayoutSupport */
    { 9379, 0xd7e44a, 220 }, /* vkGetDescriptorSetLayoutSupportKHR */
    { 9414, 0xe86334c9, 306 }, /* vkGetDeviceAccelerationStructureCompatibilityKHR */
    { 9463, 0xec1a1918, 204 }, /* vkGetDeviceBufferMemoryRequirements */
    { 9499, 0xa56ac1ad, 205 }, /* vkGetDeviceBufferMemoryRequirementsKHR */
    { 9538, 0x8d21a400, 504 }, /* vkGetDeviceFaultInfoEXT */
    { 9562, 0x2e218c10, 171 }, /* vkGetDeviceGroupPeerMemoryFeatures */
    { 9597, 0xa3809375, 172 }, /* vkGetDeviceGroupPeerMemoryFeaturesKHR */
    { 9635, 0xf72c87d4, 179 }, /* vkGetDeviceGroupPresentCapabilitiesKHR */
    { 9674, 0x41b28e81, 311 }, /* vkGetDeviceGroupSurfacePresentModes2EXT */
    { 9714, 0x6b9448c3, 180 }, /* vkGetDeviceGroupSurfacePresentModesKHR */
    { 9753, 0x5f18b6e5, 206 }, /* vkGetDeviceImageMemoryRequirements */
    { 9788, 0x3a2c5528, 207 }, /* vkGetDeviceImageMemoryRequirementsKHR */
    { 9826, 0x1ac18abd, 208 }, /* vkGetDeviceImageSparseMemoryRequirements */
    { 9867, 0xb8906110, 209 }, /* vkGetDeviceImageSparseMemoryRequirementsKHR */
    { 9911, 0x46e38db5, 12 }, /* vkGetDeviceMemoryCommitment */
    { 9939, 0x9a0fe777, 331 }, /* vkGetDeviceMemoryOpaqueCaptureAddress */
    { 9977, 0x49339be6, 332 }, /* vkGetDeviceMemoryOpaqueCaptureAddressKHR */
    { 10018, 0x31e1f1f0, 491 }, /* vkGetDeviceMicromapCompatibilityEXT */
    { 10054, 0xba013486, 0 }, /* vkGetDeviceProcAddr */
    { 10074, 0xcc920d9a, 2 }, /* vkGetDeviceQueue */
    { 10091, 0xb11a6348, 214 }, /* vkGetDeviceQueue2 */
    { 10109, 0x9d280cca, 53 }, /* vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI */
    { 10157, 0x4711995f, 499 }, /* vkGetDynamicRenderingTilePropertiesQCOM */
    { 10197, 0x96d834b, 28 }, /* vkGetEventStatus */
    { 10214, 0x69a5d6af, 165 }, /* vkGetFenceFdKHR */
    { 10230, 0x5f391892, 22 }, /* vkGetFenceStatus */
    { 10247, 0x8963de2, 163 }, /* vkGetFenceWin32HandleKHR */
    { 10272, 0xb332dbcd, 498 }, /* vkGetFramebufferTilePropertiesQCOM */
    { 10307, 0xac420aaf, 144 }, /* vkGetGeneratedCommandsMemoryRequirementsNV */
    { 10350, 0x12fa78a3, 316 }, /* vkGetImageDrmFormatModifierPropertiesEXT */
    { 10391, 0x916f1e63, 15 }, /* vkGetImageMemoryRequirements */
    { 10420, 0x56e213f7, 200 }, /* vkGetImageMemoryRequirements2 */
    { 10450, 0x8de28366, 201 }, /* vkGetImageMemoryRequirements2KHR */
    { 10483, 0x15855f5b, 17 }, /* vkGetImageSparseMemoryRequirements */
    { 10518, 0xbd4e3d3f, 202 }, /* vkGetImageSparseMemoryRequirements2 */
    { 10554, 0x3df40f5e, 203 }, /* vkGetImageSparseMemoryRequirements2KHR */
    { 10593, 0x9163b686, 42 }, /* vkGetImageSubresourceLayout */
    { 10621, 0x1a0c05f5, 495 }, /* vkGetImageSubresourceLayout2EXT */
    { 10653, 0xed8f1d33, 310 }, /* vkGetImageViewAddressNVX */
    { 10678, 0x20caa1e1, 309 }, /* vkGetImageViewHandleNVX */
    { 10702, 0x71220e82, 253 }, /* vkGetMemoryAndroidHardwareBufferANDROID */
    { 10742, 0x503c14c5, 152 }, /* vkGetMemoryFdKHR */
    { 10759, 0xb028a792, 153 }, /* vkGetMemoryFdPropertiesKHR */
    { 10786, 0x7030ee5b, 236 }, /* vkGetMemoryHostPointerPropertiesEXT */
    { 10822, 0x93d6c7a4, 156 }, /* vkGetMemoryRemoteAddressNV */
    { 10849, 0x45fc7e1c, 150 }, /* vkGetMemoryWin32HandleKHR */
    { 10875, 0xc8795b9, 140 }, /* vkGetMemoryWin32HandleNV */
    { 10900, 0xb8f59859, 151 }, /* vkGetMemoryWin32HandlePropertiesKHR */
    { 10936, 0x4540b38e, 154 }, /* vkGetMemoryZirconHandleFUCHSIA */
    { 10967, 0x5a4149eb, 155 }, /* vkGetMemoryZirconHandlePropertiesFUCHSIA */
    { 11008, 0x715aea54, 492 }, /* vkGetMicromapBuildSizesEXT */
    { 11035, 0x19616a98, 194 }, /* vkGetPastPresentationTimingGOOGLE */
    { 11069, 0x1ec6c4ec, 330 }, /* vkGetPerformanceParameterINTEL */
    { 11100, 0x2092a349, 49 }, /* vkGetPipelineCacheData */
    { 11123, 0x8b20fc09, 335 }, /* vkGetPipelineExecutableInternalRepresentationsKHR */
    { 11173, 0x748dd8cd, 333 }, /* vkGetPipelineExecutablePropertiesKHR */
    { 11210, 0x5c4d6435, 334 }, /* vkGetPipelineExecutableStatisticsKHR */
    { 11247, 0xd93861f3, 496 }, /* vkGetPipelinePropertiesEXT */
    { 11274, 0xa60eca94, 416 }, /* vkGetPrivateData */
    { 11291, 0x2dc1491d, 417 }, /* vkGetPrivateDataEXT */
    { 11311, 0xbf3f2cb3, 33 }, /* vkGetQueryPoolResults */
    { 11333, 0xf6cef020, 448 }, /* vkGetQueueCheckpointData2NV */
    { 11361, 0x428d4692, 261 }, /* vkGetQueueCheckpointDataNV */
    { 11388, 0x4b32ff8, 300 }, /* vkGetRayTracingCaptureReplayShaderGroupHandlesKHR */
    { 11438, 0x4693e853, 298 }, /* vkGetRayTracingShaderGroupHandlesKHR */
    { 11475, 0x3b54d93a, 299 }, /* vkGetRayTracingShaderGroupHandlesNV */
    { 11511, 0x2f709815, 307 }, /* vkGetRayTracingShaderGroupStackSizeKHR */
    { 11550, 0x85a9d101, 193 }, /* vkGetRefreshCycleDurationGOOGLE */
    { 11582, 0xa9820d22, 71 }, /* vkGetRenderAreaGranularity */
    { 11609, 0xd05a61a0, 246 }, /* vkGetSemaphoreCounterValue */
    { 11636, 0xf3c26065, 247 }, /* vkGetSemaphoreCounterValueKHR */
    { 11666, 0x3e0e9884, 159 }, /* vkGetSemaphoreFdKHR */
    { 11686, 0xd04be5e5, 157 }, /* vkGetSemaphoreWin32HandleKHR */
    { 11715, 0x37c0989d, 161 }, /* vkGetSemaphoreZirconHandleFUCHSIA */
    { 11749, 0x5330743c, 225 }, /* vkGetShaderInfoAMD */
    { 11768, 0x81d5d7e4, 494 }, /* vkGetShaderModuleCreateInfoIdentifierEXT */
    { 11809, 0xb7334436, 493 }, /* vkGetShaderModuleIdentifierEXT */
    { 11840, 0xa4aeb5a, 170 }, /* vkGetSwapchainCounterEXT */
    { 11865, 0x219d929, 222 }, /* vkGetSwapchainGrallocUsage2ANDROID */
    { 11900, 0x4979c9a3, 221 }, /* vkGetSwapchainGrallocUsageANDROID */
    { 11934, 0x57695f28, 132 }, /* vkGetSwapchainImagesKHR */
    { 11958, 0x66ae725e, 192 }, /* vkGetSwapchainStatusKHR */
    { 11982, 0xbbc9f99f, 217 }, /* vkGetValidationCacheDataEXT */
    { 12010, 0xd8960270, 454 }, /* vkGetVideoSessionMemoryRequirementsKHR */
    { 12049, 0x51df0390, 166 }, /* vkImportFenceFdKHR */
    { 12068, 0x1bcbb079, 164 }, /* vkImportFenceWin32HandleKHR */
    { 12096, 0x36337c05, 160 }, /* vkImportSemaphoreFdKHR */
    { 12119, 0x7e2cfcdc, 158 }, /* vkImportSemaphoreWin32HandleKHR */
    { 12151, 0x4d1996ce, 162 }, /* vkImportSemaphoreZirconHandleFUCHSIA */
    { 12188, 0x65a01d77, 322 }, /* vkInitializePerformanceApiINTEL */
    { 12220, 0x1e115cca, 11 }, /* vkInvalidateMappedMemoryRanges */
    { 12251, 0xcb977bd8, 8 }, /* vkMapMemory */
    { 12263, 0xc3499606, 50 }, /* vkMergePipelineCaches */
    { 12285, 0xe8fe1154, 218 }, /* vkMergeValidationCachesEXT */
    { 12312, 0xcb7dc88, 230 }, /* vkQueueBeginDebugUtilsLabelEXT */
    { 12343, 0xc3628a09, 18 }, /* vkQueueBindSparse */
    { 12361, 0xf130b20a, 231 }, /* vkQueueEndDebugUtilsLabelEXT */
    { 12390, 0x56027200, 232 }, /* vkQueueInsertDebugUtilsLabelEXT */
    { 12422, 0xfc5fb6ce, 134 }, /* vkQueuePresentKHR */
    { 12440, 0xf8499f82, 329 }, /* vkQueueSetPerformanceConfigurationINTEL */
    { 12480, 0xa0313eef, 224 }, /* vkQueueSignalReleaseImageANDROID */
    { 12513, 0xfa4713ec, 3 }, /* vkQueueSubmit */
    { 12527, 0xbf0609e6, 443 }, /* vkQueueSubmit2 */
    { 12542, 0xc2dd288f, 444 }, /* vkQueueSubmit2KHR */
    { 12560, 0x6f8fc2a5, 4 }, /* vkQueueWaitIdle */
    { 12576, 0x26cc78f5, 168 }, /* vkRegisterDeviceEventEXT */
    { 12601, 0x4a0bd849, 169 }, /* vkRegisterDisplayEventEXT */
    { 12627, 0x13814325, 313 }, /* vkReleaseFullScreenExclusiveModeEXT */
    { 12663, 0x28575036, 328 }, /* vkReleasePerformanceConfigurationINTEL */
    { 12702, 0x8bdecb76, 315 }, /* vkReleaseProfilingLockKHR */
    { 12728, 0x847dc731, 79 }, /* vkResetCommandBuffer */
    { 12749, 0x6da9f7fd, 74 }, /* vkResetCommandPool */
    { 12768, 0x9bd85f5, 63 }, /* vkResetDescriptorPool */
    { 12790, 0x6d373ba8, 30 }, /* vkResetEvent */
    { 12803, 0x684781dc, 21 }, /* vkResetFences */
    { 12817, 0x4e671e02, 34 }, /* vkResetQueryPool */
    { 12834, 0xe6701e5f, 35 }, /* vkResetQueryPoolEXT */
    { 12854, 0x3c97f0dc, 469 }, /* vkSetBufferCollectionBufferConstraintsFUCHSIA */
    { 12900, 0xcae02471, 470 }, /* vkSetBufferCollectionImageConstraintsFUCHSIA */
    { 12945, 0x180cec44, 228 }, /* vkSetDebugUtilsObjectNameEXT */
    { 12974, 0x15942821, 229 }, /* vkSetDebugUtilsObjectTagEXT */
    { 13002, 0xa42f1309, 466 }, /* vkSetDeviceMemoryPriorityEXT */
    { 13031, 0x592ae5f5, 29 }, /* vkSetEvent */
    { 13042, 0xa20f1ea9, 191 }, /* vkSetHdrMetadataEXT */
    { 13062, 0xbd1cd781, 226 }, /* vkSetLocalDimmingAMD */
    { 13083, 0x5cf49ca8, 414 }, /* vkSetPrivateData */
    { 13100, 0x23456729, 415 }, /* vkSetPrivateDataEXT */
    { 13120, 0xcd347297, 250 }, /* vkSignalSemaphore */
    { 13138, 0x8fef55c6, 251 }, /* vkSignalSemaphoreKHR */
    { 13159, 0xfef2fb38, 148 }, /* vkTrimCommandPool */
    { 13177, 0x51177c8d, 149 }, /* vkTrimCommandPoolKHR */
    { 13198, 0x408975ae, 323 }, /* vkUninitializePerformanceApiINTEL */
    { 13232, 0x1a1a0e2f, 9 }, /* vkUnmapMemory */
    { 13246, 0x5349c9d, 188 }, /* vkUpdateDescriptorSetWithTemplate */
    { 13280, 0x214ad230, 189 }, /* vkUpdateDescriptorSetWithTemplateKHR */
    { 13317, 0xbfd090ae, 66 }, /* vkUpdateDescriptorSets */
    { 13340, 0x8e570a3a, 452 }, /* vkUpdateVideoSessionParametersKHR */
    { 13374, 0x19d64c81, 23 }, /* vkWaitForFences */
    { 13390, 0x19c136b1, 467 }, /* vkWaitForPresentKHR */
    { 13410, 0x74368ad9, 248 }, /* vkWaitSemaphores */
    { 13427, 0x2bc77454, 249 }, /* vkWaitSemaphoresKHR */
    { 13447, 0x8bc9ae08, 295 }, /* vkWriteAccelerationStructuresPropertiesKHR */
    { 13490, 0xc43df3dd, 490 }, /* vkWriteMicromapsPropertiesEXT */
};

/* Hash table stats:
 * size 505 entries
 * collisions entries:
 *     0      388
 *     1      64
 *     2      26
 *     3      11
 *     4      4
 *     5      3
 *     6      4
 *     7      4
 *     8      0
 *     9+     1
 */

#define none 0xffff
static const uint16_t device_string_map[1024] = {
    0x016e,
    none,
    none,
    none,
    none,
    0x0051,
    none,
    0x0096,
    none,
    0x0049,
    0x002d,
    none,
    0x015d,
    none,
    none,
    none,
    0x0168,
    none,
    none,
    none,
    none,
    0x01ac,
    none,
    none,
    0x0118,
    none,
    none,
    0x0045,
    0x01a0,
    none,
    0x0134,
    0x00b9,
    0x01a7,
    0x002b,
    none,
    0x016f,
    0x00d5,
    none,
    0x0042,
    none,
    none,
    none,
    none,
    0x00db,
    0x00ee,
    none,
    none,
    0x00d0,
    none,
    0x012e,
    none,
    none,
    0x0092,
    0x01a2,
    0x01b6,
    none,
    none,
    0x011c,
    none,
    0x011a,
    0x01b4,
    none,
    0x0136,
    0x011b,
    0x004c,
    none,
    0x0098,
    none,
    0x003c,
    none,
    none,
    0x01e2,
    0x0126,
    0x00e6,
    0x0087,
    0x0082,
    0x002c,
    0x0065,
    none,
    0x0043,
    none,
    0x0152,
    none,
    0x00df,
    0x01f6,
    0x00b0,
    none,
    0x01e1,
    0x0053,
    0x0199,
    none,
    none,
    0x00d6,
    0x0120,
    none,
    none,
    none,
    0x003d,
    0x011d,
    none,
    0x014f,
    0x0163,
    0x01aa,
    0x004e,
    0x007d,
    none,
    none,
    none,
    0x0040,
    none,
    none,
    0x01d4,
    0x0021,
    0x01e0,
    0x00cb,
    none,
    0x00a0,
    0x00ac,
    0x00ff,
    0x01c0,
    0x01b0,
    0x00d9,
    0x00f8,
    none,
    none,
    none,
    0x003b,
    0x00ce,
    none,
    0x01f3,
    0x0083,
    0x0151,
    0x013c,
    none,
    0x00f1,
    0x0105,
    0x00eb,
    0x0059,
    0x0069,
    0x00ab,
    0x0140,
    0x01ec,
    none,
    0x01d1,
    0x00b4,
    none,
    0x0183,
    none,
    0x001e,
    0x00d7,
    0x016a,
    0x01b1,
    none,
    0x017c,
    none,
    0x01c8,
    none,
    0x00e2,
    none,
    0x0111,
    0x0037,
    none,
    none,
    0x0187,
    0x0104,
    none,
    0x00e0,
    none,
    0x01d6,
    none,
    none,
    none,
    none,
    0x0005,
    0x01f1,
    0x013e,
    0x0107,
    none,
    0x01bf,
    0x0060,
    none,
    none,
    none,
    none,
    0x0031,
    none,
    0x0033,
    0x008d,
    none,
    none,
    none,
    none,
    0x00c0,
    none,
    0x010c,
    0x0173,
    0x010a,
    0x0193,
    0x01a6,
    none,
    0x00da,
    0x016b,
    0x017f,
    0x00ae,
    none,
    0x015c,
    0x01e7,
    none,
    none,
    0x00c1,
    none,
    none,
    none,
    0x00ad,
    0x0024,
    none,
    0x00b2,
    none,
    0x0038,
    none,
    0x01c1,
    0x0001,
    0x0127,
    none,
    0x009e,
    none,
    0x00d2,
    0x0019,
    none,
    0x014b,
    none,
    none,
    none,
    0x01b3,
    none,
    none,
    0x019e,
    none,
    none,
    0x01df,
    0x01c4,
    none,
    none,
    0x01a1,
    none,
    0x01d3,
    none,
    none,
    none,
    none,
    none,
    none,
    0x01ef,
    none,
    0x0085,
    none,
    none,
    0x01ad,
    0x0113,
    none,
    none,
    0x00a8,
    none,
    0x0030,
    0x0032,
    none,
    none,
    none,
    none,
    none,
    none,
    none,
    0x0177,
    0x0067,
    none,
    none,
    none,
    0x006b,
    none,
    none,
    0x000f,
    none,
    0x0006,
    none,
    none,
    0x01a5,
    none,
    none,
    none,
    none,
    0x0068,
    none,
    0x00a2,
    none,
    0x0129,
    0x00e9,
    0x0175,
    0x01b8,
    none,
    0x0115,
    none,
    0x009a,
    none,
    0x0156,
    none,
    0x0076,
    none,
    0x0190,
    0x00d8,
    0x01ae,
    0x0054,
    0x0164,
    none,
    0x0102,
    0x0007,
    none,
    none,
    0x015b,
    0x016c,
    0x0016,
    none,
    none,
    none,
    none,
    0x0095,
    none,
    none,
    none,
    none,
    none,
    none,
    none,
    0x0090,
    0x01ab,
    none,
    none,
    none,
    none,
    0x00ed,
    0x013a,
    0x01c7,
    none,
    none,
    none,
    0x00f6,
    none,
    0x0119,
    none,
    0x0013,
    0x00ef,
    none,
    0x0180,
    none,
    0x005d,
    0x012c,
    none,
    none,
    0x00b8,
    0x00cd,
    0x0165,
    0x0109,
    0x00d1,
    none,
    0x0142,
    none,
    0x008a,
    0x00b1,
    none,
    none,
    none,
    0x0002,
    0x0169,
    none,
    none,
    none,
    0x001a,
    0x018c,
    0x0145,
    none,
    none,
    0x0123,
    0x0066,
    none,
    none,
    0x00a1,
    0x007f,
    none,
    none,
    0x0023,
    none,
    none,
    none,
    none,
    none,
    0x01c3,
    none,
    none,
    0x0161,
    none,
    none,
    0x005a,
    none,
    none,
    0x010b,
    0x0084,
    none,
    0x0139,
    none,
    none,
    none,
    0x017d,
    0x00dd,
    none,
    none,
    none,
    0x0015,
    0x0162,
    0x0052,
    none,
    0x01b9,
    none,
    none,
    0x0158,
    none,
    none,
    0x00b3,
    none,
    none,
    0x0167,
    0x0010,
    0x00f9,
    none,
    none,
    0x009f,
    0x0146,
    0x01af,
    none,
    0x0178,
    0x00ea,
    none,
    0x001c,
    0x015e,
    none,
    none,
    0x0077,
    none,
    none,
    0x0058,
    0x008b,
    0x01ed,
    none,
    none,
    none,
    0x01bc,
    0x01ea,
    none,
    none,
    none,
    none,
    0x000b,
    0x0000,
    0x0026,
    none,
    none,
    0x00a3,
    none,
    none,
    0x0008,
    none,
    none,
    none,
    none,
    none,
    0x0063,
    0x0020,
    none,
    0x01dc,
    none,
    0x0135,
    0x0198,
    none,
    0x0191,
    0x0184,
    0x00a5,
    0x0088,
    0x01b2,
    0x001b,
    0x0012,
    none,
    none,
    none,
    0x0100,
    none,
    0x0112,
    none,
    none,
    0x017b,
    none,
    none,
    0x01a3,
    none,
    0x005c,
    none,
    none,
    none,
    0x007c,
    0x0046,
    0x0089,
    none,
    none,
    0x019b,
    0x006e,
    0x0097,
    none,
    0x00e3,
    0x0159,
    none,
    none,
    0x0061,
    0x0128,
    0x0039,
    0x01c9,
    0x012d,
    0x007b,
    0x00bf,
    0x001d,
    none,
    none,
    0x005e,
    none,
    0x00fb,
    0x01cb,
    none,
    0x0130,
    none,
    none,
    none,
    0x01c6,
    0x000a,
    0x018f,
    0x0197,
    0x01ca,
    none,
    0x0099,
    0x004f,
    none,
    none,
    none,
    none,
    none,
    none,
    none,
    0x0080,
    0x0036,
    0x015f,
    none,
    0x0018,
    none,
    0x01da,
    0x01ee,
    0x01f0,
    none,
    0x016d,
    0x00b5,
    none,
    0x006a,
    0x0103,
    none,
    0x0160,
    none,
    0x01f2,
    0x01dd,
    0x0137,
    0x003a,
    none,
    0x0035,
    0x0071,
    0x00a4,
    none,
    none,
    none,
    0x012a,
    0x00f5,
    none,
    0x0117,
    none,
    0x000e,
    none,
    0x00f0,
    none,
    none,
    none,
    0x005b,
    none,
    0x0057,
    none,
    0x019c,
    none,
    0x00fe,
    none,
    0x0138,
    none,
    none,
    0x00e1,
    none,
    0x0101,
    0x01bb,
    0x01de,
    none,
    none,
    none,
    0x009d,
    0x012b,
    none,
    none,
    0x01e4,
    none,
    0x0110,
    none,
    0x01d0,
    0x00a7,
    none,
    0x011e,
    none,
    0x013b,
    none,
    none,
    0x013f,
    none,
    none,
    0x00af,
    none,
    0x006c,
    none,
    0x01f7,
    none,
    none,
    none,
    none,
    0x0133,
    none,
    0x013d,
    0x008e,
    0x01bd,
    none,
    none,
    0x018e,
    0x0056,
    none,
    0x0188,
    none,
    none,
    none,
    0x004d,
    none,
    0x00c6,
    none,
    none,
    0x0143,
    none,
    0x0157,
    0x0192,
    none,
    0x01e9,
    0x019d,
    none,
    0x00a9,
    0x00f7,
    none,
    none,
    none,
    none,
    none,
    0x007e,
    0x011f,
    none,
    0x0017,
    0x01a8,
    none,
    0x0172,
    0x0121,
    0x0091,
    none,
    none,
    none,
    0x00ec,
    none,
    0x0182,
    none,
    0x01f4,
    none,
    0x00e8,
    none,
    none,
    none,
    none,
    0x01d2,
    0x00fc,
    0x0195,
    0x00fa,
    0x001f,
    0x0176,
    none,
    none,
    none,
    none,
    0x0186,
    none,
    none,
    none,
    none,
    none,
    0x0062,
    none,
    0x0081,
    none,
    none,
    0x012f,
    0x01c2,
    0x00bb,
    none,
    none,
    none,
    none,
    0x0048,
    none,
    none,
    0x0072,
    none,
    0x0122,
    none,
    none,
    none,
    none,
    none,
    none,
    0x0073,
    0x01cc,
    0x00c4,
    none,
    none,
    0x0174,
    none,
    none,
    none,
    none,
    none,
    none,
    0x0029,
    0x0028,
    none,
    0x00f2,
    0x010f,
    none,
    none,
    0x01a4,
    none,
    0x0094,
    0x006d,
    none,
    0x00c3,
    0x0075,
    none,
    none,
    none,
    none,
    none,
    0x00ca,
    none,
    0x003f,
    0x01ce,
    none,
    none,
    none,
    0x006f,
    0x009c,
    0x0132,
    0x01e3,
    0x007a,
    none,
    0x00b6,
    none,
    none,
    none,
    0x004a,
    0x010e,
    0x01f5,
    none,
    0x00c2,
    none,
    0x0141,
    0x00cc,
    0x0093,
    0x00d4,
    0x009b,
    0x0047,
    none,
    0x0027,
    0x00e4,
    0x00bc,
    none,
    none,
    none,
    none,
    0x0144,
    0x0125,
    none,
    0x0009,
    0x01ba,
    0x01e8,
    none,
    0x014d,
    0x0078,
    none,
    0x0003,
    none,
    0x008f,
    0x0108,
    0x00e5,
    0x0106,
    none,
    none,
    none,
    0x0086,
    0x00d3,
    none,
    none,
    none,
    none,
    none,
    0x004b,
    none,
    0x0064,
    0x0055,
    none,
    0x0131,
    0x01d8,
    none,
    none,
    none,
    0x017e,
    0x019f,
    0x014e,
    0x0150,
    none,
    none,
    0x0070,
    0x000c,
    0x00fd,
    none,
    none,
    0x00c8,
    0x01e5,
    0x0116,
    0x00aa,
    none,
    none,
    none,
    0x01b7,
    0x018b,
    none,
    none,
    0x00e7,
    0x0114,
    none,
    none,
    none,
    none,
    none,
    none,
    0x018a,
    0x0004,
    none,
    none,
    none,
    none,
    none,
    none,
    0x0149,
    none,
    0x0034,
    0x015a,
    none,
    none,
    0x00ba,
    0x0170,
    0x01d7,
    0x0179,
    none,
    0x00c7,
    none,
    none,
    none,
    0x014c,
    none,
    none,
    none,
    0x01e6,
    0x00c5,
    none,
    0x0181,
    none,
    none,
    0x0014,
    none,
    0x002a,
    0x0155,
    none,
    none,
    none,
    0x019a,
    none,
    0x01be,
    0x005f,
    0x0194,
    none,
    none,
    0x01cd,
    0x00de,
    0x018d,
    none,
    none,
    0x014a,
    0x00c9,
    none,
    none,
    none,
    none,
    none,
    none,
    none,
    0x0147,
    0x00dc,
    none,
    none,
    none,
    0x01db,
    0x003e,
    0x01d5,
    none,
    0x0041,
    none,
    0x00b7,
    0x0148,
    none,
    0x0154,
    0x00cf,
    none,
    none,
    none,
    none,
    0x0153,
    none,
    none,
    none,
    none,
    0x00be,
    0x01eb,
    none,
    0x00f4,
    none,
    none,
    none,
    none,
    none,
    none,
    none,
    0x00bd,
    none,
    none,
    0x0196,
    none,
    none,
    0x0185,
    0x000d,
    none,
    none,
    0x0074,
    none,
    none,
    0x0171,
    none,
    none,
    0x0124,
    0x0044,
    none,
    none,
    none,
    0x002f,
    0x0022,
    none,
    none,
    none,
    0x0011,
    none,
    none,
    0x01b5,
    0x0050,
    0x00a6,
    none,
    0x0079,
    0x0166,
    none,
    0x0025,
    0x01cf,
    0x00f3,
    0x002e,
    none,
    0x01f8,
    none,
    none,
    none,
    0x008c,
    none,
    none,
    0x0189,
    0x01a9,
    0x017a,
    none,
    none,
    none,
    0x01d9,
    0x01c5,
    0x010d,
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
    147,
    148,
    149,
    150,
    151,
    152,
    153,
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
    169,
    170,
    170,
    171,
    171,
    172,
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
    184,
    185,
    186,
    187,
    188,
    188,
    189,
    189,
    190,
    190,
    191,
    191,
    192,
    192,
    193,
    193,
    194,
    194,
    195,
    195,
    196,
    197,
    198,
    199,
    200,
    201,
    201,
    202,
    203,
    204,
    205,
    206,
    207,
    208,
    209,
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
    219,
    220,
    220,
    221,
    221,
    222,
    222,
    223,
    223,
    224,
    224,
    225,
    225,
    226,
    227,
    228,
    228,
    228,
    229,
    229,
    229,
    230,
    231,
    232,
    233,
    234,
    235,
    236,
    237,
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
    279,
    280,
    281,
    282,
    283,
    284,
    285,
    286,
    286,
    287,
    287,
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
    308,
    309,
    310,
    311,
    312,
    312,
    313,
    313,
    314,
    314,
    315,
    315,
    316,
    316,
    317,
    317,
    318,
    318,
    319,
    319,
    320,
    320,
    321,
    321,
    322,
    322,
    323,
    323,
    324,
    325,
    325,
    326,
    326,
    327,
    328,
    328,
    329,
    330,
    331,
    332,
    333,
    334,
    335,
    336,
    337,
    338,
    339,
    340,
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
    329,
    361,
    361,
    362,
    362,
    363,
    363,
    364,
    364,
    365,
    365,
    366,
    366,
    367,
    367,
    368,
    368,
    369,
    369,
    370,
    371,
    372,
    373,
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
    382,
    383,
    384,
    385,
    386,
    387,
    388,
    389,
    390,
    391,
    392,
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
    407,
    408,
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
      /* CreateCommandPool */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 73:
      /* DestroyCommandPool */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 74:
      /* ResetCommandPool */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 75:
      /* AllocateCommandBuffers */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 76:
      /* FreeCommandBuffers */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 77:
      /* BeginCommandBuffer */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 78:
      /* EndCommandBuffer */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 79:
      /* ResetCommandBuffer */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 80:
      /* CmdBindPipeline */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 81:
      /* CmdSetViewport */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 82:
      /* CmdSetScissor */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 83:
      /* CmdSetLineWidth */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 84:
      /* CmdSetDepthBias */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 85:
      /* CmdSetBlendConstants */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 86:
      /* CmdSetDepthBounds */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 87:
      /* CmdSetStencilCompareMask */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 88:
      /* CmdSetStencilWriteMask */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 89:
      /* CmdSetStencilReference */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 90:
      /* CmdBindDescriptorSets */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 91:
      /* CmdBindIndexBuffer */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 92:
      /* CmdBindVertexBuffers */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 93:
      /* CmdDraw */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 94:
      /* CmdDrawIndexed */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 95:
      /* CmdDrawMultiEXT */
      if (!device || device->EXT_multi_draw) return true;
      return false;
   case 96:
      /* CmdDrawMultiIndexedEXT */
      if (!device || device->EXT_multi_draw) return true;
      return false;
   case 97:
      /* CmdDrawIndirect */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 98:
      /* CmdDrawIndexedIndirect */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 99:
      /* CmdDispatch */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 100:
      /* CmdDispatchIndirect */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 101:
      /* CmdSubpassShadingHUAWEI */
      if (!device || device->HUAWEI_subpass_shading) return true;
      return false;
   case 102:
      /* CmdCopyBuffer */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 103:
      /* CmdCopyImage */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 104:
      /* CmdBlitImage */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 105:
      /* CmdCopyBufferToImage */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 106:
      /* CmdCopyImageToBuffer */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 107:
      /* CmdUpdateBuffer */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 108:
      /* CmdFillBuffer */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 109:
      /* CmdClearColorImage */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 110:
      /* CmdClearDepthStencilImage */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 111:
      /* CmdClearAttachments */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 112:
      /* CmdResolveImage */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 113:
      /* CmdSetEvent */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 114:
      /* CmdResetEvent */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 115:
      /* CmdWaitEvents */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 116:
      /* CmdPipelineBarrier */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 117:
      /* CmdBeginQuery */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 118:
      /* CmdEndQuery */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 119:
      /* CmdBeginConditionalRenderingEXT */
      if (!device || device->EXT_conditional_rendering) return true;
      return false;
   case 120:
      /* CmdEndConditionalRenderingEXT */
      if (!device || device->EXT_conditional_rendering) return true;
      return false;
   case 121:
      /* CmdResetQueryPool */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 122:
      /* CmdWriteTimestamp */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 123:
      /* CmdCopyQueryPoolResults */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 124:
      /* CmdPushConstants */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 125:
      /* CmdBeginRenderPass */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 126:
      /* CmdNextSubpass */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 127:
      /* CmdEndRenderPass */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 128:
      /* CmdExecuteCommands */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 129:
      /* CreateSharedSwapchainsKHR */
      if (!device || device->KHR_display_swapchain) return true;
      return false;
   case 130:
      /* CreateSwapchainKHR */
      if (!device || device->KHR_swapchain) return true;
      return false;
   case 131:
      /* DestroySwapchainKHR */
      if (!device || device->KHR_swapchain) return true;
      return false;
   case 132:
      /* GetSwapchainImagesKHR */
      if (!device || device->KHR_swapchain) return true;
      return false;
   case 133:
      /* AcquireNextImageKHR */
      if (!device || device->KHR_swapchain) return true;
      return false;
   case 134:
      /* QueuePresentKHR */
      if (!device || device->KHR_swapchain) return true;
      return false;
   case 135:
      /* DebugMarkerSetObjectNameEXT */
      if (!device || device->EXT_debug_marker) return true;
      return false;
   case 136:
      /* DebugMarkerSetObjectTagEXT */
      if (!device || device->EXT_debug_marker) return true;
      return false;
   case 137:
      /* CmdDebugMarkerBeginEXT */
      if (!device || device->EXT_debug_marker) return true;
      return false;
   case 138:
      /* CmdDebugMarkerEndEXT */
      if (!device || device->EXT_debug_marker) return true;
      return false;
   case 139:
      /* CmdDebugMarkerInsertEXT */
      if (!device || device->EXT_debug_marker) return true;
      return false;
   case 140:
      /* GetMemoryWin32HandleNV */
      if (!device || device->NV_external_memory_win32) return true;
      return false;
   case 141:
      /* CmdExecuteGeneratedCommandsNV */
      if (!device || device->NV_device_generated_commands) return true;
      return false;
   case 142:
      /* CmdPreprocessGeneratedCommandsNV */
      if (!device || device->NV_device_generated_commands) return true;
      return false;
   case 143:
      /* CmdBindPipelineShaderGroupNV */
      if (!device || device->NV_device_generated_commands) return true;
      return false;
   case 144:
      /* GetGeneratedCommandsMemoryRequirementsNV */
      if (!device || device->NV_device_generated_commands) return true;
      return false;
   case 145:
      /* CreateIndirectCommandsLayoutNV */
      if (!device || device->NV_device_generated_commands) return true;
      return false;
   case 146:
      /* DestroyIndirectCommandsLayoutNV */
      if (!device || device->NV_device_generated_commands) return true;
      return false;
   case 147:
      /* CmdPushDescriptorSetKHR */
      if (!device || device->KHR_push_descriptor) return true;
      return false;
   case 148:
      /* TrimCommandPool */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 149:
      /* TrimCommandPoolKHR */
      if (!device || device->KHR_maintenance1) return true;
      return false;
   case 150:
      /* GetMemoryWin32HandleKHR */
      if (!device || device->KHR_external_memory_win32) return true;
      return false;
   case 151:
      /* GetMemoryWin32HandlePropertiesKHR */
      if (!device || device->KHR_external_memory_win32) return true;
      return false;
   case 152:
      /* GetMemoryFdKHR */
      if (!device || device->KHR_external_memory_fd) return true;
      return false;
   case 153:
      /* GetMemoryFdPropertiesKHR */
      if (!device || device->KHR_external_memory_fd) return true;
      return false;
   case 154:
      /* GetMemoryZirconHandleFUCHSIA */
      if (!device || device->FUCHSIA_external_memory) return true;
      return false;
   case 155:
      /* GetMemoryZirconHandlePropertiesFUCHSIA */
      if (!device || device->FUCHSIA_external_memory) return true;
      return false;
   case 156:
      /* GetMemoryRemoteAddressNV */
      if (!device || device->NV_external_memory_rdma) return true;
      return false;
   case 157:
      /* GetSemaphoreWin32HandleKHR */
      if (!device || device->KHR_external_semaphore_win32) return true;
      return false;
   case 158:
      /* ImportSemaphoreWin32HandleKHR */
      if (!device || device->KHR_external_semaphore_win32) return true;
      return false;
   case 159:
      /* GetSemaphoreFdKHR */
      if (!device || device->KHR_external_semaphore_fd) return true;
      return false;
   case 160:
      /* ImportSemaphoreFdKHR */
      if (!device || device->KHR_external_semaphore_fd) return true;
      return false;
   case 161:
      /* GetSemaphoreZirconHandleFUCHSIA */
      if (!device || device->FUCHSIA_external_semaphore) return true;
      return false;
   case 162:
      /* ImportSemaphoreZirconHandleFUCHSIA */
      if (!device || device->FUCHSIA_external_semaphore) return true;
      return false;
   case 163:
      /* GetFenceWin32HandleKHR */
      if (!device || device->KHR_external_fence_win32) return true;
      return false;
   case 164:
      /* ImportFenceWin32HandleKHR */
      if (!device || device->KHR_external_fence_win32) return true;
      return false;
   case 165:
      /* GetFenceFdKHR */
      if (!device || device->KHR_external_fence_fd) return true;
      return false;
   case 166:
      /* ImportFenceFdKHR */
      if (!device || device->KHR_external_fence_fd) return true;
      return false;
   case 167:
      /* DisplayPowerControlEXT */
      if (!device || device->EXT_display_control) return true;
      return false;
   case 168:
      /* RegisterDeviceEventEXT */
      if (!device || device->EXT_display_control) return true;
      return false;
   case 169:
      /* RegisterDisplayEventEXT */
      if (!device || device->EXT_display_control) return true;
      return false;
   case 170:
      /* GetSwapchainCounterEXT */
      if (!device || device->EXT_display_control) return true;
      return false;
   case 171:
      /* GetDeviceGroupPeerMemoryFeatures */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 172:
      /* GetDeviceGroupPeerMemoryFeaturesKHR */
      if (!device || device->KHR_device_group) return true;
      return false;
   case 173:
      /* BindBufferMemory2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 174:
      /* BindBufferMemory2KHR */
      if (!device || device->KHR_bind_memory2) return true;
      return false;
   case 175:
      /* BindImageMemory2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 176:
      /* BindImageMemory2KHR */
      if (!device || device->KHR_bind_memory2) return true;
      return false;
   case 177:
      /* CmdSetDeviceMask */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 178:
      /* CmdSetDeviceMaskKHR */
      if (!device || device->KHR_device_group) return true;
      return false;
   case 179:
      /* GetDeviceGroupPresentCapabilitiesKHR */
      if (!device || device->KHR_swapchain) return true;
      if (!device || device->KHR_device_group) return true;
      return false;
   case 180:
      /* GetDeviceGroupSurfacePresentModesKHR */
      if (!device || device->KHR_swapchain) return true;
      if (!device || device->KHR_device_group) return true;
      return false;
   case 181:
      /* AcquireNextImage2KHR */
      if (!device || device->KHR_swapchain) return true;
      if (!device || device->KHR_device_group) return true;
      return false;
   case 182:
      /* CmdDispatchBase */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 183:
      /* CmdDispatchBaseKHR */
      if (!device || device->KHR_device_group) return true;
      return false;
   case 184:
      /* CreateDescriptorUpdateTemplate */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 185:
      /* CreateDescriptorUpdateTemplateKHR */
      if (!device || device->KHR_descriptor_update_template) return true;
      return false;
   case 186:
      /* DestroyDescriptorUpdateTemplate */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 187:
      /* DestroyDescriptorUpdateTemplateKHR */
      if (!device || device->KHR_descriptor_update_template) return true;
      return false;
   case 188:
      /* UpdateDescriptorSetWithTemplate */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 189:
      /* UpdateDescriptorSetWithTemplateKHR */
      if (!device || device->KHR_descriptor_update_template) return true;
      return false;
   case 190:
      /* CmdPushDescriptorSetWithTemplateKHR */
      if (!device || device->KHR_push_descriptor) return true;
      if (!device || device->KHR_push_descriptor) return true;
      if (!device || device->KHR_descriptor_update_template) return true;
      return false;
   case 191:
      /* SetHdrMetadataEXT */
      if (!device || device->EXT_hdr_metadata) return true;
      return false;
   case 192:
      /* GetSwapchainStatusKHR */
      if (!device || device->KHR_shared_presentable_image) return true;
      return false;
   case 193:
      /* GetRefreshCycleDurationGOOGLE */
      if (!device || device->GOOGLE_display_timing) return true;
      return false;
   case 194:
      /* GetPastPresentationTimingGOOGLE */
      if (!device || device->GOOGLE_display_timing) return true;
      return false;
   case 195:
      /* CmdSetViewportWScalingNV */
      if (!device || device->NV_clip_space_w_scaling) return true;
      return false;
   case 196:
      /* CmdSetDiscardRectangleEXT */
      if (!device || device->EXT_discard_rectangles) return true;
      return false;
   case 197:
      /* CmdSetSampleLocationsEXT */
      if (!device || device->EXT_sample_locations) return true;
      return false;
   case 198:
      /* GetBufferMemoryRequirements2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 199:
      /* GetBufferMemoryRequirements2KHR */
      if (!device || device->KHR_get_memory_requirements2) return true;
      return false;
   case 200:
      /* GetImageMemoryRequirements2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 201:
      /* GetImageMemoryRequirements2KHR */
      if (!device || device->KHR_get_memory_requirements2) return true;
      return false;
   case 202:
      /* GetImageSparseMemoryRequirements2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 203:
      /* GetImageSparseMemoryRequirements2KHR */
      if (!device || device->KHR_get_memory_requirements2) return true;
      return false;
   case 204:
      /* GetDeviceBufferMemoryRequirements */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 205:
      /* GetDeviceBufferMemoryRequirementsKHR */
      if (!device || device->KHR_maintenance4) return true;
      return false;
   case 206:
      /* GetDeviceImageMemoryRequirements */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 207:
      /* GetDeviceImageMemoryRequirementsKHR */
      if (!device || device->KHR_maintenance4) return true;
      return false;
   case 208:
      /* GetDeviceImageSparseMemoryRequirements */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 209:
      /* GetDeviceImageSparseMemoryRequirementsKHR */
      if (!device || device->KHR_maintenance4) return true;
      return false;
   case 210:
      /* CreateSamplerYcbcrConversion */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 211:
      /* CreateSamplerYcbcrConversionKHR */
      if (!device || device->KHR_sampler_ycbcr_conversion) return true;
      return false;
   case 212:
      /* DestroySamplerYcbcrConversion */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 213:
      /* DestroySamplerYcbcrConversionKHR */
      if (!device || device->KHR_sampler_ycbcr_conversion) return true;
      return false;
   case 214:
      /* GetDeviceQueue2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 215:
      /* CreateValidationCacheEXT */
      if (!device || device->EXT_validation_cache) return true;
      return false;
   case 216:
      /* DestroyValidationCacheEXT */
      if (!device || device->EXT_validation_cache) return true;
      return false;
   case 217:
      /* GetValidationCacheDataEXT */
      if (!device || device->EXT_validation_cache) return true;
      return false;
   case 218:
      /* MergeValidationCachesEXT */
      if (!device || device->EXT_validation_cache) return true;
      return false;
   case 219:
      /* GetDescriptorSetLayoutSupport */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 220:
      /* GetDescriptorSetLayoutSupportKHR */
      if (!device || device->KHR_maintenance3) return true;
      return false;
   case 221:
      /* GetSwapchainGrallocUsageANDROID */
      return true;
   case 222:
      /* GetSwapchainGrallocUsage2ANDROID */
      return true;
   case 223:
      /* AcquireImageANDROID */
      return true;
   case 224:
      /* QueueSignalReleaseImageANDROID */
      return true;
   case 225:
      /* GetShaderInfoAMD */
      if (!device || device->AMD_shader_info) return true;
      return false;
   case 226:
      /* SetLocalDimmingAMD */
      if (!device || device->AMD_display_native_hdr) return true;
      return false;
   case 227:
      /* GetCalibratedTimestampsEXT */
      if (!device || device->EXT_calibrated_timestamps) return true;
      return false;
   case 228:
      /* SetDebugUtilsObjectNameEXT */
      if (instance->EXT_debug_utils) return true;
      return false;
   case 229:
      /* SetDebugUtilsObjectTagEXT */
      if (instance->EXT_debug_utils) return true;
      return false;
   case 230:
      /* QueueBeginDebugUtilsLabelEXT */
      if (instance->EXT_debug_utils) return true;
      return false;
   case 231:
      /* QueueEndDebugUtilsLabelEXT */
      if (instance->EXT_debug_utils) return true;
      return false;
   case 232:
      /* QueueInsertDebugUtilsLabelEXT */
      if (instance->EXT_debug_utils) return true;
      return false;
   case 233:
      /* CmdBeginDebugUtilsLabelEXT */
      if (instance->EXT_debug_utils) return true;
      return false;
   case 234:
      /* CmdEndDebugUtilsLabelEXT */
      if (instance->EXT_debug_utils) return true;
      return false;
   case 235:
      /* CmdInsertDebugUtilsLabelEXT */
      if (instance->EXT_debug_utils) return true;
      return false;
   case 236:
      /* GetMemoryHostPointerPropertiesEXT */
      if (!device || device->EXT_external_memory_host) return true;
      return false;
   case 237:
      /* CmdWriteBufferMarkerAMD */
      if (!device || device->AMD_buffer_marker) return true;
      return false;
   case 238:
      /* CreateRenderPass2 */
      return VK_MAKE_VERSION(1, 2, 0) <= core_version;
   case 239:
      /* CreateRenderPass2KHR */
      if (!device || device->KHR_create_renderpass2) return true;
      return false;
   case 240:
      /* CmdBeginRenderPass2 */
      return VK_MAKE_VERSION(1, 2, 0) <= core_version;
   case 241:
      /* CmdBeginRenderPass2KHR */
      if (!device || device->KHR_create_renderpass2) return true;
      return false;
   case 242:
      /* CmdNextSubpass2 */
      return VK_MAKE_VERSION(1, 2, 0) <= core_version;
   case 243:
      /* CmdNextSubpass2KHR */
      if (!device || device->KHR_create_renderpass2) return true;
      return false;
   case 244:
      /* CmdEndRenderPass2 */
      return VK_MAKE_VERSION(1, 2, 0) <= core_version;
   case 245:
      /* CmdEndRenderPass2KHR */
      if (!device || device->KHR_create_renderpass2) return true;
      return false;
   case 246:
      /* GetSemaphoreCounterValue */
      return VK_MAKE_VERSION(1, 2, 0) <= core_version;
   case 247:
      /* GetSemaphoreCounterValueKHR */
      if (!device || device->KHR_timeline_semaphore) return true;
      return false;
   case 248:
      /* WaitSemaphores */
      return VK_MAKE_VERSION(1, 2, 0) <= core_version;
   case 249:
      /* WaitSemaphoresKHR */
      if (!device || device->KHR_timeline_semaphore) return true;
      return false;
   case 250:
      /* SignalSemaphore */
      return VK_MAKE_VERSION(1, 2, 0) <= core_version;
   case 251:
      /* SignalSemaphoreKHR */
      if (!device || device->KHR_timeline_semaphore) return true;
      return false;
   case 252:
      /* GetAndroidHardwareBufferPropertiesANDROID */
      if (!device || device->ANDROID_external_memory_android_hardware_buffer) return true;
      return false;
   case 253:
      /* GetMemoryAndroidHardwareBufferANDROID */
      if (!device || device->ANDROID_external_memory_android_hardware_buffer) return true;
      return false;
   case 254:
      /* CmdDrawIndirectCount */
      return VK_MAKE_VERSION(1, 2, 0) <= core_version;
   case 255:
      /* CmdDrawIndirectCountKHR */
      if (!device || device->KHR_draw_indirect_count) return true;
      return false;
   case 256:
      /* CmdDrawIndirectCountAMD */
      if (!device || device->AMD_draw_indirect_count) return true;
      return false;
   case 257:
      /* CmdDrawIndexedIndirectCount */
      return VK_MAKE_VERSION(1, 2, 0) <= core_version;
   case 258:
      /* CmdDrawIndexedIndirectCountKHR */
      if (!device || device->KHR_draw_indirect_count) return true;
      return false;
   case 259:
      /* CmdDrawIndexedIndirectCountAMD */
      if (!device || device->AMD_draw_indirect_count) return true;
      return false;
   case 260:
      /* CmdSetCheckpointNV */
      if (!device || device->NV_device_diagnostic_checkpoints) return true;
      return false;
   case 261:
      /* GetQueueCheckpointDataNV */
      if (!device || device->NV_device_diagnostic_checkpoints) return true;
      return false;
   case 262:
      /* CmdBindTransformFeedbackBuffersEXT */
      if (!device || device->EXT_transform_feedback) return true;
      return false;
   case 263:
      /* CmdBeginTransformFeedbackEXT */
      if (!device || device->EXT_transform_feedback) return true;
      return false;
   case 264:
      /* CmdEndTransformFeedbackEXT */
      if (!device || device->EXT_transform_feedback) return true;
      return false;
   case 265:
      /* CmdBeginQueryIndexedEXT */
      if (!device || device->EXT_transform_feedback) return true;
      return false;
   case 266:
      /* CmdEndQueryIndexedEXT */
      if (!device || device->EXT_transform_feedback) return true;
      return false;
   case 267:
      /* CmdDrawIndirectByteCountEXT */
      if (!device || device->EXT_transform_feedback) return true;
      return false;
   case 268:
      /* CmdSetExclusiveScissorNV */
      if (!device || device->NV_scissor_exclusive) return true;
      return false;
   case 269:
      /* CmdBindShadingRateImageNV */
      if (!device || device->NV_shading_rate_image) return true;
      return false;
   case 270:
      /* CmdSetViewportShadingRatePaletteNV */
      if (!device || device->NV_shading_rate_image) return true;
      return false;
   case 271:
      /* CmdSetCoarseSampleOrderNV */
      if (!device || device->NV_shading_rate_image) return true;
      return false;
   case 272:
      /* CmdDrawMeshTasksNV */
      if (!device || device->NV_mesh_shader) return true;
      return false;
   case 273:
      /* CmdDrawMeshTasksIndirectNV */
      if (!device || device->NV_mesh_shader) return true;
      return false;
   case 274:
      /* CmdDrawMeshTasksIndirectCountNV */
      if (!device || device->NV_mesh_shader) return true;
      return false;
   case 275:
      /* CmdDrawMeshTasksEXT */
      if (!device || device->EXT_mesh_shader) return true;
      return false;
   case 276:
      /* CmdDrawMeshTasksIndirectEXT */
      if (!device || device->EXT_mesh_shader) return true;
      return false;
   case 277:
      /* CmdDrawMeshTasksIndirectCountEXT */
      if (!device || device->EXT_mesh_shader) return true;
      return false;
   case 278:
      /* CompileDeferredNV */
      if (!device || device->NV_ray_tracing) return true;
      return false;
   case 279:
      /* CreateAccelerationStructureNV */
      if (!device || device->NV_ray_tracing) return true;
      return false;
   case 280:
      /* CmdBindInvocationMaskHUAWEI */
      if (!device || device->HUAWEI_invocation_mask) return true;
      return false;
   case 281:
      /* DestroyAccelerationStructureKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 282:
      /* DestroyAccelerationStructureNV */
      if (!device || device->NV_ray_tracing) return true;
      return false;
   case 283:
      /* GetAccelerationStructureMemoryRequirementsNV */
      if (!device || device->NV_ray_tracing) return true;
      return false;
   case 284:
      /* BindAccelerationStructureMemoryNV */
      if (!device || device->NV_ray_tracing) return true;
      return false;
   case 285:
      /* CmdCopyAccelerationStructureNV */
      if (!device || device->NV_ray_tracing) return true;
      return false;
   case 286:
      /* CmdCopyAccelerationStructureKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 287:
      /* CopyAccelerationStructureKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 288:
      /* CmdCopyAccelerationStructureToMemoryKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 289:
      /* CopyAccelerationStructureToMemoryKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 290:
      /* CmdCopyMemoryToAccelerationStructureKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 291:
      /* CopyMemoryToAccelerationStructureKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 292:
      /* CmdWriteAccelerationStructuresPropertiesKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 293:
      /* CmdWriteAccelerationStructuresPropertiesNV */
      if (!device || device->NV_ray_tracing) return true;
      return false;
   case 294:
      /* CmdBuildAccelerationStructureNV */
      if (!device || device->NV_ray_tracing) return true;
      return false;
   case 295:
      /* WriteAccelerationStructuresPropertiesKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 296:
      /* CmdTraceRaysKHR */
      if (!device || device->KHR_ray_tracing_pipeline) return true;
      return false;
   case 297:
      /* CmdTraceRaysNV */
      if (!device || device->NV_ray_tracing) return true;
      return false;
   case 298:
      /* GetRayTracingShaderGroupHandlesKHR */
      if (!device || device->KHR_ray_tracing_pipeline) return true;
      return false;
   case 299:
      /* GetRayTracingShaderGroupHandlesNV */
      if (!device || device->NV_ray_tracing) return true;
      return false;
   case 300:
      /* GetRayTracingCaptureReplayShaderGroupHandlesKHR */
      if (!device || device->KHR_ray_tracing_pipeline) return true;
      return false;
   case 301:
      /* GetAccelerationStructureHandleNV */
      if (!device || device->NV_ray_tracing) return true;
      return false;
   case 302:
      /* CreateRayTracingPipelinesNV */
      if (!device || device->NV_ray_tracing) return true;
      return false;
   case 303:
      /* CreateRayTracingPipelinesKHR */
      if (!device || device->KHR_ray_tracing_pipeline) return true;
      return false;
   case 304:
      /* CmdTraceRaysIndirectKHR */
      if (!device || device->KHR_ray_tracing_pipeline) return true;
      return false;
   case 305:
      /* CmdTraceRaysIndirect2KHR */
      if (!device || device->KHR_ray_tracing_maintenance1) return true;
      return false;
   case 306:
      /* GetDeviceAccelerationStructureCompatibilityKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 307:
      /* GetRayTracingShaderGroupStackSizeKHR */
      if (!device || device->KHR_ray_tracing_pipeline) return true;
      return false;
   case 308:
      /* CmdSetRayTracingPipelineStackSizeKHR */
      if (!device || device->KHR_ray_tracing_pipeline) return true;
      return false;
   case 309:
      /* GetImageViewHandleNVX */
      if (!device || device->NVX_image_view_handle) return true;
      return false;
   case 310:
      /* GetImageViewAddressNVX */
      if (!device || device->NVX_image_view_handle) return true;
      return false;
   case 311:
      /* GetDeviceGroupSurfacePresentModes2EXT */
      if (!device || device->EXT_full_screen_exclusive) return true;
      if (!device || device->EXT_full_screen_exclusive) return true;
      return false;
   case 312:
      /* AcquireFullScreenExclusiveModeEXT */
      if (!device || device->EXT_full_screen_exclusive) return true;
      return false;
   case 313:
      /* ReleaseFullScreenExclusiveModeEXT */
      if (!device || device->EXT_full_screen_exclusive) return true;
      return false;
   case 314:
      /* AcquireProfilingLockKHR */
      if (!device || device->KHR_performance_query) return true;
      return false;
   case 315:
      /* ReleaseProfilingLockKHR */
      if (!device || device->KHR_performance_query) return true;
      return false;
   case 316:
      /* GetImageDrmFormatModifierPropertiesEXT */
      if (!device || device->EXT_image_drm_format_modifier) return true;
      return false;
   case 317:
      /* GetBufferOpaqueCaptureAddress */
      return VK_MAKE_VERSION(1, 2, 0) <= core_version;
   case 318:
      /* GetBufferOpaqueCaptureAddressKHR */
      if (!device || device->KHR_buffer_device_address) return true;
      return false;
   case 319:
      /* GetBufferDeviceAddress */
      return VK_MAKE_VERSION(1, 2, 0) <= core_version;
   case 320:
      /* GetBufferDeviceAddressKHR */
      if (!device || device->KHR_buffer_device_address) return true;
      return false;
   case 321:
      /* GetBufferDeviceAddressEXT */
      if (!device || device->EXT_buffer_device_address) return true;
      return false;
   case 322:
      /* InitializePerformanceApiINTEL */
      if (!device || device->INTEL_performance_query) return true;
      return false;
   case 323:
      /* UninitializePerformanceApiINTEL */
      if (!device || device->INTEL_performance_query) return true;
      return false;
   case 324:
      /* CmdSetPerformanceMarkerINTEL */
      if (!device || device->INTEL_performance_query) return true;
      return false;
   case 325:
      /* CmdSetPerformanceStreamMarkerINTEL */
      if (!device || device->INTEL_performance_query) return true;
      return false;
   case 326:
      /* CmdSetPerformanceOverrideINTEL */
      if (!device || device->INTEL_performance_query) return true;
      return false;
   case 327:
      /* AcquirePerformanceConfigurationINTEL */
      if (!device || device->INTEL_performance_query) return true;
      return false;
   case 328:
      /* ReleasePerformanceConfigurationINTEL */
      if (!device || device->INTEL_performance_query) return true;
      return false;
   case 329:
      /* QueueSetPerformanceConfigurationINTEL */
      if (!device || device->INTEL_performance_query) return true;
      return false;
   case 330:
      /* GetPerformanceParameterINTEL */
      if (!device || device->INTEL_performance_query) return true;
      return false;
   case 331:
      /* GetDeviceMemoryOpaqueCaptureAddress */
      return VK_MAKE_VERSION(1, 2, 0) <= core_version;
   case 332:
      /* GetDeviceMemoryOpaqueCaptureAddressKHR */
      if (!device || device->KHR_buffer_device_address) return true;
      return false;
   case 333:
      /* GetPipelineExecutablePropertiesKHR */
      if (!device || device->KHR_pipeline_executable_properties) return true;
      return false;
   case 334:
      /* GetPipelineExecutableStatisticsKHR */
      if (!device || device->KHR_pipeline_executable_properties) return true;
      return false;
   case 335:
      /* GetPipelineExecutableInternalRepresentationsKHR */
      if (!device || device->KHR_pipeline_executable_properties) return true;
      return false;
   case 336:
      /* CmdSetLineStippleEXT */
      if (!device || device->EXT_line_rasterization) return true;
      return false;
   case 337:
      /* CreateAccelerationStructureKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 338:
      /* CmdBuildAccelerationStructuresKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 339:
      /* CmdBuildAccelerationStructuresIndirectKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 340:
      /* BuildAccelerationStructuresKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 341:
      /* GetAccelerationStructureDeviceAddressKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 342:
      /* CreateDeferredOperationKHR */
      if (!device || device->KHR_deferred_host_operations) return true;
      return false;
   case 343:
      /* DestroyDeferredOperationKHR */
      if (!device || device->KHR_deferred_host_operations) return true;
      return false;
   case 344:
      /* GetDeferredOperationMaxConcurrencyKHR */
      if (!device || device->KHR_deferred_host_operations) return true;
      return false;
   case 345:
      /* GetDeferredOperationResultKHR */
      if (!device || device->KHR_deferred_host_operations) return true;
      return false;
   case 346:
      /* DeferredOperationJoinKHR */
      if (!device || device->KHR_deferred_host_operations) return true;
      return false;
   case 347:
      /* CmdSetCullMode */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 348:
      /* CmdSetCullModeEXT */
      if (!device || device->EXT_extended_dynamic_state) return true;
      return false;
   case 349:
      /* CmdSetFrontFace */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 350:
      /* CmdSetFrontFaceEXT */
      if (!device || device->EXT_extended_dynamic_state) return true;
      return false;
   case 351:
      /* CmdSetPrimitiveTopology */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 352:
      /* CmdSetPrimitiveTopologyEXT */
      if (!device || device->EXT_extended_dynamic_state) return true;
      return false;
   case 353:
      /* CmdSetViewportWithCount */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 354:
      /* CmdSetViewportWithCountEXT */
      if (!device || device->EXT_extended_dynamic_state) return true;
      return false;
   case 355:
      /* CmdSetScissorWithCount */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 356:
      /* CmdSetScissorWithCountEXT */
      if (!device || device->EXT_extended_dynamic_state) return true;
      return false;
   case 357:
      /* CmdBindVertexBuffers2 */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 358:
      /* CmdBindVertexBuffers2EXT */
      if (!device || device->EXT_extended_dynamic_state) return true;
      return false;
   case 359:
      /* CmdSetDepthTestEnable */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 360:
      /* CmdSetDepthTestEnableEXT */
      if (!device || device->EXT_extended_dynamic_state) return true;
      return false;
   case 361:
      /* CmdSetDepthWriteEnable */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 362:
      /* CmdSetDepthWriteEnableEXT */
      if (!device || device->EXT_extended_dynamic_state) return true;
      return false;
   case 363:
      /* CmdSetDepthCompareOp */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 364:
      /* CmdSetDepthCompareOpEXT */
      if (!device || device->EXT_extended_dynamic_state) return true;
      return false;
   case 365:
      /* CmdSetDepthBoundsTestEnable */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 366:
      /* CmdSetDepthBoundsTestEnableEXT */
      if (!device || device->EXT_extended_dynamic_state) return true;
      return false;
   case 367:
      /* CmdSetStencilTestEnable */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 368:
      /* CmdSetStencilTestEnableEXT */
      if (!device || device->EXT_extended_dynamic_state) return true;
      return false;
   case 369:
      /* CmdSetStencilOp */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 370:
      /* CmdSetStencilOpEXT */
      if (!device || device->EXT_extended_dynamic_state) return true;
      return false;
   case 371:
      /* CmdSetPatchControlPointsEXT */
      if (!device || device->EXT_extended_dynamic_state2) return true;
      return false;
   case 372:
      /* CmdSetRasterizerDiscardEnable */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 373:
      /* CmdSetRasterizerDiscardEnableEXT */
      if (!device || device->EXT_extended_dynamic_state2) return true;
      return false;
   case 374:
      /* CmdSetDepthBiasEnable */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 375:
      /* CmdSetDepthBiasEnableEXT */
      if (!device || device->EXT_extended_dynamic_state2) return true;
      return false;
   case 376:
      /* CmdSetLogicOpEXT */
      if (!device || device->EXT_extended_dynamic_state2) return true;
      return false;
   case 377:
      /* CmdSetPrimitiveRestartEnable */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 378:
      /* CmdSetPrimitiveRestartEnableEXT */
      if (!device || device->EXT_extended_dynamic_state2) return true;
      return false;
   case 379:
      /* CreatePrivateDataSlot */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 380:
      /* CmdSetTessellationDomainOriginEXT */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      return false;
   case 381:
      /* CmdSetDepthClampEnableEXT */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      return false;
   case 382:
      /* CmdSetPolygonModeEXT */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      return false;
   case 383:
      /* CmdSetRasterizationSamplesEXT */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      return false;
   case 384:
      /* CmdSetSampleMaskEXT */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      return false;
   case 385:
      /* CmdSetAlphaToCoverageEnableEXT */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      return false;
   case 386:
      /* CmdSetAlphaToOneEnableEXT */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      return false;
   case 387:
      /* CmdSetLogicOpEnableEXT */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      return false;
   case 388:
      /* CmdSetColorBlendEnableEXT */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      return false;
   case 389:
      /* CmdSetColorBlendEquationEXT */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      return false;
   case 390:
      /* CmdSetColorWriteMaskEXT */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      return false;
   case 391:
      /* CmdSetRasterizationStreamEXT */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      return false;
   case 392:
      /* CmdSetConservativeRasterizationModeEXT */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      return false;
   case 393:
      /* CmdSetExtraPrimitiveOverestimationSizeEXT */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      return false;
   case 394:
      /* CmdSetDepthClipEnableEXT */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      return false;
   case 395:
      /* CmdSetSampleLocationsEnableEXT */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      return false;
   case 396:
      /* CmdSetColorBlendAdvancedEXT */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      return false;
   case 397:
      /* CmdSetProvokingVertexModeEXT */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      return false;
   case 398:
      /* CmdSetLineRasterizationModeEXT */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      return false;
   case 399:
      /* CmdSetLineStippleEnableEXT */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      return false;
   case 400:
      /* CmdSetDepthClipNegativeOneToOneEXT */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      return false;
   case 401:
      /* CmdSetViewportWScalingEnableNV */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      return false;
   case 402:
      /* CmdSetViewportSwizzleNV */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      return false;
   case 403:
      /* CmdSetCoverageToColorEnableNV */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      return false;
   case 404:
      /* CmdSetCoverageToColorLocationNV */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      return false;
   case 405:
      /* CmdSetCoverageModulationModeNV */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      return false;
   case 406:
      /* CmdSetCoverageModulationTableEnableNV */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      return false;
   case 407:
      /* CmdSetCoverageModulationTableNV */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      return false;
   case 408:
      /* CmdSetShadingRateImageEnableNV */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      return false;
   case 409:
      /* CmdSetCoverageReductionModeNV */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      return false;
   case 410:
      /* CmdSetRepresentativeFragmentTestEnableNV */
      if (!device || device->EXT_extended_dynamic_state3) return true;
      return false;
   case 411:
      /* CreatePrivateDataSlotEXT */
      if (!device || device->EXT_private_data) return true;
      return false;
   case 412:
      /* DestroyPrivateDataSlot */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 413:
      /* DestroyPrivateDataSlotEXT */
      if (!device || device->EXT_private_data) return true;
      return false;
   case 414:
      /* SetPrivateData */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 415:
      /* SetPrivateDataEXT */
      if (!device || device->EXT_private_data) return true;
      return false;
   case 416:
      /* GetPrivateData */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 417:
      /* GetPrivateDataEXT */
      if (!device || device->EXT_private_data) return true;
      return false;
   case 418:
      /* CmdCopyBuffer2 */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 419:
      /* CmdCopyBuffer2KHR */
      if (!device || device->KHR_copy_commands2) return true;
      return false;
   case 420:
      /* CmdCopyImage2 */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 421:
      /* CmdCopyImage2KHR */
      if (!device || device->KHR_copy_commands2) return true;
      return false;
   case 422:
      /* CmdBlitImage2 */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 423:
      /* CmdBlitImage2KHR */
      if (!device || device->KHR_copy_commands2) return true;
      return false;
   case 424:
      /* CmdCopyBufferToImage2 */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 425:
      /* CmdCopyBufferToImage2KHR */
      if (!device || device->KHR_copy_commands2) return true;
      return false;
   case 426:
      /* CmdCopyImageToBuffer2 */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 427:
      /* CmdCopyImageToBuffer2KHR */
      if (!device || device->KHR_copy_commands2) return true;
      return false;
   case 428:
      /* CmdResolveImage2 */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 429:
      /* CmdResolveImage2KHR */
      if (!device || device->KHR_copy_commands2) return true;
      return false;
   case 430:
      /* CmdSetFragmentShadingRateKHR */
      if (!device || device->KHR_fragment_shading_rate) return true;
      return false;
   case 431:
      /* CmdSetFragmentShadingRateEnumNV */
      if (!device || device->NV_fragment_shading_rate_enums) return true;
      return false;
   case 432:
      /* GetAccelerationStructureBuildSizesKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 433:
      /* CmdSetVertexInputEXT */
      if (!device || device->EXT_vertex_input_dynamic_state) return true;
      return false;
   case 434:
      /* CmdSetColorWriteEnableEXT */
      if (!device || device->EXT_color_write_enable) return true;
      return false;
   case 435:
      /* CmdSetEvent2 */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 436:
      /* CmdSetEvent2KHR */
      if (!device || device->KHR_synchronization2) return true;
      return false;
   case 437:
      /* CmdResetEvent2 */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 438:
      /* CmdResetEvent2KHR */
      if (!device || device->KHR_synchronization2) return true;
      return false;
   case 439:
      /* CmdWaitEvents2 */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 440:
      /* CmdWaitEvents2KHR */
      if (!device || device->KHR_synchronization2) return true;
      return false;
   case 441:
      /* CmdPipelineBarrier2 */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 442:
      /* CmdPipelineBarrier2KHR */
      if (!device || device->KHR_synchronization2) return true;
      return false;
   case 443:
      /* QueueSubmit2 */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 444:
      /* QueueSubmit2KHR */
      if (!device || device->KHR_synchronization2) return true;
      return false;
   case 445:
      /* CmdWriteTimestamp2 */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 446:
      /* CmdWriteTimestamp2KHR */
      if (!device || device->KHR_synchronization2) return true;
      return false;
   case 447:
      /* CmdWriteBufferMarker2AMD */
      if (!device || device->KHR_synchronization2) return true;
      return false;
   case 448:
      /* GetQueueCheckpointData2NV */
      if (!device || device->KHR_synchronization2) return true;
      return false;
   case 449:
      /* CreateVideoSessionKHR */
      if (!device || device->KHR_video_queue) return true;
      return false;
   case 450:
      /* DestroyVideoSessionKHR */
      if (!device || device->KHR_video_queue) return true;
      return false;
   case 451:
      /* CreateVideoSessionParametersKHR */
      if (!device || device->KHR_video_queue) return true;
      return false;
   case 452:
      /* UpdateVideoSessionParametersKHR */
      if (!device || device->KHR_video_queue) return true;
      return false;
   case 453:
      /* DestroyVideoSessionParametersKHR */
      if (!device || device->KHR_video_queue) return true;
      return false;
   case 454:
      /* GetVideoSessionMemoryRequirementsKHR */
      if (!device || device->KHR_video_queue) return true;
      return false;
   case 455:
      /* BindVideoSessionMemoryKHR */
      if (!device || device->KHR_video_queue) return true;
      return false;
   case 456:
      /* CmdDecodeVideoKHR */
      if (!device || device->KHR_video_decode_queue) return true;
      return false;
   case 457:
      /* CmdBeginVideoCodingKHR */
      if (!device || device->KHR_video_queue) return true;
      return false;
   case 458:
      /* CmdControlVideoCodingKHR */
      if (!device || device->KHR_video_queue) return true;
      return false;
   case 459:
      /* CmdEndVideoCodingKHR */
      if (!device || device->KHR_video_queue) return true;
      return false;
   case 460:
      /* CmdEncodeVideoKHR */
      if (!device || device->KHR_video_encode_queue) return true;
      return false;
   case 461:
      /* CreateCuModuleNVX */
      if (!device || device->NVX_binary_import) return true;
      return false;
   case 462:
      /* CreateCuFunctionNVX */
      if (!device || device->NVX_binary_import) return true;
      return false;
   case 463:
      /* DestroyCuModuleNVX */
      if (!device || device->NVX_binary_import) return true;
      return false;
   case 464:
      /* DestroyCuFunctionNVX */
      if (!device || device->NVX_binary_import) return true;
      return false;
   case 465:
      /* CmdCuLaunchKernelNVX */
      if (!device || device->NVX_binary_import) return true;
      return false;
   case 466:
      /* SetDeviceMemoryPriorityEXT */
      if (!device || device->EXT_pageable_device_local_memory) return true;
      return false;
   case 467:
      /* WaitForPresentKHR */
      if (!device || device->KHR_present_wait) return true;
      return false;
   case 468:
      /* CreateBufferCollectionFUCHSIA */
      if (!device || device->FUCHSIA_buffer_collection) return true;
      return false;
   case 469:
      /* SetBufferCollectionBufferConstraintsFUCHSIA */
      if (!device || device->FUCHSIA_buffer_collection) return true;
      return false;
   case 470:
      /* SetBufferCollectionImageConstraintsFUCHSIA */
      if (!device || device->FUCHSIA_buffer_collection) return true;
      return false;
   case 471:
      /* DestroyBufferCollectionFUCHSIA */
      if (!device || device->FUCHSIA_buffer_collection) return true;
      return false;
   case 472:
      /* GetBufferCollectionPropertiesFUCHSIA */
      if (!device || device->FUCHSIA_buffer_collection) return true;
      return false;
   case 473:
      /* CmdBeginRendering */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 474:
      /* CmdBeginRenderingKHR */
      if (!device || device->KHR_dynamic_rendering) return true;
      return false;
   case 475:
      /* CmdEndRendering */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 476:
      /* CmdEndRenderingKHR */
      if (!device || device->KHR_dynamic_rendering) return true;
      return false;
   case 477:
      /* GetDescriptorSetLayoutHostMappingInfoVALVE */
      if (!device || device->VALVE_descriptor_set_host_mapping) return true;
      return false;
   case 478:
      /* GetDescriptorSetHostMappingVALVE */
      if (!device || device->VALVE_descriptor_set_host_mapping) return true;
      return false;
   case 479:
      /* CreateMicromapEXT */
      if (!device || device->EXT_opacity_micromap) return true;
      return false;
   case 480:
      /* CmdBuildMicromapsEXT */
      if (!device || device->EXT_opacity_micromap) return true;
      return false;
   case 481:
      /* BuildMicromapsEXT */
      if (!device || device->EXT_opacity_micromap) return true;
      return false;
   case 482:
      /* DestroyMicromapEXT */
      if (!device || device->EXT_opacity_micromap) return true;
      return false;
   case 483:
      /* CmdCopyMicromapEXT */
      if (!device || device->EXT_opacity_micromap) return true;
      return false;
   case 484:
      /* CopyMicromapEXT */
      if (!device || device->EXT_opacity_micromap) return true;
      return false;
   case 485:
      /* CmdCopyMicromapToMemoryEXT */
      if (!device || device->EXT_opacity_micromap) return true;
      return false;
   case 486:
      /* CopyMicromapToMemoryEXT */
      if (!device || device->EXT_opacity_micromap) return true;
      return false;
   case 487:
      /* CmdCopyMemoryToMicromapEXT */
      if (!device || device->EXT_opacity_micromap) return true;
      return false;
   case 488:
      /* CopyMemoryToMicromapEXT */
      if (!device || device->EXT_opacity_micromap) return true;
      return false;
   case 489:
      /* CmdWriteMicromapsPropertiesEXT */
      if (!device || device->EXT_opacity_micromap) return true;
      return false;
   case 490:
      /* WriteMicromapsPropertiesEXT */
      if (!device || device->EXT_opacity_micromap) return true;
      return false;
   case 491:
      /* GetDeviceMicromapCompatibilityEXT */
      if (!device || device->EXT_opacity_micromap) return true;
      return false;
   case 492:
      /* GetMicromapBuildSizesEXT */
      if (!device || device->EXT_opacity_micromap) return true;
      return false;
   case 493:
      /* GetShaderModuleIdentifierEXT */
      if (!device || device->EXT_shader_module_identifier) return true;
      return false;
   case 494:
      /* GetShaderModuleCreateInfoIdentifierEXT */
      if (!device || device->EXT_shader_module_identifier) return true;
      return false;
   case 495:
      /* GetImageSubresourceLayout2EXT */
      if (!device || device->EXT_image_compression_control) return true;
      return false;
   case 496:
      /* GetPipelinePropertiesEXT */
      if (!device || device->EXT_pipeline_properties) return true;
      return false;
   case 497:
      /* ExportMetalObjectsEXT */
      if (!device || device->EXT_metal_objects) return true;
      return false;
   case 498:
      /* GetFramebufferTilePropertiesQCOM */
      if (!device || device->QCOM_tile_properties) return true;
      return false;
   case 499:
      /* GetDynamicRenderingTilePropertiesQCOM */
      if (!device || device->QCOM_tile_properties) return true;
      return false;
   case 500:
      /* CreateOpticalFlowSessionNV */
      if (!device || device->NV_optical_flow) return true;
      return false;
   case 501:
      /* DestroyOpticalFlowSessionNV */
      if (!device || device->NV_optical_flow) return true;
      return false;
   case 502:
      /* BindOpticalFlowSessionImageNV */
      if (!device || device->NV_optical_flow) return true;
      return false;
   case 503:
      /* CmdOpticalFlowExecuteNV */
      if (!device || device->NV_optical_flow) return true;
      return false;
   case 504:
      /* GetDeviceFaultInfoEXT */
      if (!device || device->EXT_device_fault) return true;
      return false;
   default:
      return false;
   }
}

#ifdef _MSC_VER
VKAPI_ATTR void VKAPI_CALL vk_entrypoint_stub(void)
{
   unreachable(!"Entrypoint not implemented");
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
