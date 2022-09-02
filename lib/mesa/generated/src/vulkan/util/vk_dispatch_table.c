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
    { 1604, 0x100341b4, 53 }, /* vkGetPhysicalDevicePresentRectanglesKHR */
    { 1644, 0x52fe22c9, 0 }, /* vkGetPhysicalDeviceProperties */
    { 1674, 0x6c4d8ee1, 29 }, /* vkGetPhysicalDeviceProperties2 */
    { 1705, 0xcd15838c, 30 }, /* vkGetPhysicalDeviceProperties2KHR */
    { 1739, 0x7c7c9a0f, 65 }, /* vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR */
    { 1795, 0x4e5fc88a, 1 }, /* vkGetPhysicalDeviceQueueFamilyProperties */
    { 1836, 0xcad374d8, 35 }, /* vkGetPhysicalDeviceQueueFamilyProperties2 */
    { 1878, 0x5ceb2bed, 36 }, /* vkGetPhysicalDeviceQueueFamilyProperties2KHR */
    { 1923, 0xb5c7dc78, 25 }, /* vkGetPhysicalDeviceScreenPresentationSupportQNX */
    { 1971, 0x272ef8ef, 9 }, /* vkGetPhysicalDeviceSparseImageFormatProperties */
    { 2018, 0xebddba0b, 39 }, /* vkGetPhysicalDeviceSparseImageFormatProperties2 */
    { 2066, 0x8746ed72, 40 }, /* vkGetPhysicalDeviceSparseImageFormatProperties2KHR */
    { 2117, 0x432ca8f7, 66 }, /* vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV */
    { 2183, 0x5a5fba04, 52 }, /* vkGetPhysicalDeviceSurfaceCapabilities2EXT */
    { 2226, 0x9497e378, 55 }, /* vkGetPhysicalDeviceSurfaceCapabilities2KHR */
    { 2269, 0x77890558, 17 }, /* vkGetPhysicalDeviceSurfaceCapabilitiesKHR */
    { 2311, 0xd00b7188, 56 }, /* vkGetPhysicalDeviceSurfaceFormats2KHR */
    { 2349, 0xe32227c8, 18 }, /* vkGetPhysicalDeviceSurfaceFormatsKHR */
    { 2386, 0x33557b83, 63 }, /* vkGetPhysicalDeviceSurfacePresentModes2EXT */
    { 2429, 0x31c3cbd1, 19 }, /* vkGetPhysicalDeviceSurfacePresentModesKHR */
    { 2471, 0x1a687885, 16 }, /* vkGetPhysicalDeviceSurfaceSupportKHR */
    { 2508, 0x7b5f3fb9, 67 }, /* vkGetPhysicalDeviceToolProperties */
    { 2542, 0xd1685100, 68 }, /* vkGetPhysicalDeviceToolPropertiesEXT */
    { 2579, 0x8ee6bf8a, 70 }, /* vkGetPhysicalDeviceVideoCapabilitiesKHR */
    { 2619, 0xbb7625d6, 71 }, /* vkGetPhysicalDeviceVideoFormatPropertiesKHR */
    { 2663, 0x84e085ac, 20 }, /* vkGetPhysicalDeviceWaylandPresentationSupportKHR */
    { 2712, 0x80e72505, 21 }, /* vkGetPhysicalDeviceWin32PresentationSupportKHR */
    { 2759, 0x41782cb9, 23 }, /* vkGetPhysicalDeviceXcbPresentationSupportKHR */
    { 2804, 0x34a063ab, 22 }, /* vkGetPhysicalDeviceXlibPresentationSupportKHR */
    { 2850, 0xb87cdd6c, 49 }, /* vkGetRandROutputDisplayEXT */
    { 2877, 0x613996b5, 51 }, /* vkGetWinrtDisplayNV */
    { 2897, 0x4207f4f1, 47 }, /* vkReleaseDisplayEXT */
};

/* Hash table stats:
 * size 74 entries
 * collisions entries:
 *     0      53
 *     1      9
 *     2      8
 *     3      2
 *     4      0
 *     5      0
 *     6      0
 *     7      1
 *     8      0
 *     9+     1
 */

#define none 0xffff
static const uint16_t physical_device_string_map[128] = {
    0x0026,
    none,
    none,
    0x003c,
    0x0037,
    0x0035,
    0x0011,
    none,
    0x0027,
    0x000c,
    0x002f,
    0x0034,
    0x000b,
    0x0000,
    0x001a,
    0x002e,
    none,
    none,
    0x0018,
    0x0031,
    none,
    0x0007,
    none,
    none,
    0x001f,
    none,
    0x000e,
    0x003a,
    0x000f,
    0x0041,
    0x0038,
    0x002d,
    0x0002,
    none,
    none,
    none,
    0x000a,
    none,
    0x0040,
    0x001b,
    none,
    none,
    none,
    0x003e,
    0x0043,
    none,
    0x0014,
    0x0016,
    0x0046,
    none,
    none,
    0x001c,
    0x002a,
    0x0048,
    0x001e,
    none,
    none,
    0x0029,
    0x0012,
    0x0021,
    none,
    0x0028,
    0x0044,
    0x0003,
    none,
    0x0004,
    0x0008,
    none,
    none,
    none,
    none,
    none,
    0x003b,
    0x002b,
    none,
    none,
    0x0019,
    0x0001,
    none,
    none,
    none,
    0x003d,
    0x0010,
    0x0024,
    none,
    none,
    0x0022,
    none,
    0x0030,
    none,
    0x0020,
    none,
    0x0015,
    none,
    0x0009,
    0x003f,
    0x001d,
    0x002c,
    none,
    none,
    0x0045,
    0x000d,
    none,
    0x0006,
    none,
    0x0042,
    0x0025,
    0x0017,
    0x0047,
    0x0005,
    none,
    0x0033,
    none,
    0x0049,
    0x0013,
    none,
    none,
    none,
    none,
    0x0036,
    0x0032,
    none,
    0x0023,
    none,
    none,
    none,
    0x0039,
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
    "vkBindVideoSessionMemoryKHR\0"
    "vkBuildAccelerationStructuresKHR\0"
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
    "vkCmdDrawMeshTasksIndirectCountNV\0"
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
    "vkCmdSetBlendConstants\0"
    "vkCmdSetCheckpointNV\0"
    "vkCmdSetCoarseSampleOrderNV\0"
    "vkCmdSetColorWriteEnableEXT\0"
    "vkCmdSetCullMode\0"
    "vkCmdSetCullModeEXT\0"
    "vkCmdSetDepthBias\0"
    "vkCmdSetDepthBiasEnable\0"
    "vkCmdSetDepthBiasEnableEXT\0"
    "vkCmdSetDepthBounds\0"
    "vkCmdSetDepthBoundsTestEnable\0"
    "vkCmdSetDepthBoundsTestEnableEXT\0"
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
    "vkCmdSetFragmentShadingRateEnumNV\0"
    "vkCmdSetFragmentShadingRateKHR\0"
    "vkCmdSetFrontFace\0"
    "vkCmdSetFrontFaceEXT\0"
    "vkCmdSetLineStippleEXT\0"
    "vkCmdSetLineWidth\0"
    "vkCmdSetLogicOpEXT\0"
    "vkCmdSetPatchControlPointsEXT\0"
    "vkCmdSetPerformanceMarkerINTEL\0"
    "vkCmdSetPerformanceOverrideINTEL\0"
    "vkCmdSetPerformanceStreamMarkerINTEL\0"
    "vkCmdSetPrimitiveRestartEnable\0"
    "vkCmdSetPrimitiveRestartEnableEXT\0"
    "vkCmdSetPrimitiveTopology\0"
    "vkCmdSetPrimitiveTopologyEXT\0"
    "vkCmdSetRasterizerDiscardEnable\0"
    "vkCmdSetRasterizerDiscardEnableEXT\0"
    "vkCmdSetRayTracingPipelineStackSizeKHR\0"
    "vkCmdSetSampleLocationsEXT\0"
    "vkCmdSetScissor\0"
    "vkCmdSetScissorWithCount\0"
    "vkCmdSetScissorWithCountEXT\0"
    "vkCmdSetStencilCompareMask\0"
    "vkCmdSetStencilOp\0"
    "vkCmdSetStencilOpEXT\0"
    "vkCmdSetStencilReference\0"
    "vkCmdSetStencilTestEnable\0"
    "vkCmdSetStencilTestEnableEXT\0"
    "vkCmdSetStencilWriteMask\0"
    "vkCmdSetVertexInputEXT\0"
    "vkCmdSetViewport\0"
    "vkCmdSetViewportShadingRatePaletteNV\0"
    "vkCmdSetViewportWScalingNV\0"
    "vkCmdSetViewportWithCount\0"
    "vkCmdSetViewportWithCountEXT\0"
    "vkCmdSubpassShadingHUAWEI\0"
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
    "vkCmdWriteTimestamp\0"
    "vkCmdWriteTimestamp2\0"
    "vkCmdWriteTimestamp2KHR\0"
    "vkCompileDeferredNV\0"
    "vkCopyAccelerationStructureKHR\0"
    "vkCopyAccelerationStructureToMemoryKHR\0"
    "vkCopyMemoryToAccelerationStructureKHR\0"
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
    "vkGetDeviceProcAddr\0"
    "vkGetDeviceQueue\0"
    "vkGetDeviceQueue2\0"
    "vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI\0"
    "vkGetEventStatus\0"
    "vkGetFenceFdKHR\0"
    "vkGetFenceStatus\0"
    "vkGetFenceWin32HandleKHR\0"
    "vkGetGeneratedCommandsMemoryRequirementsNV\0"
    "vkGetImageDrmFormatModifierPropertiesEXT\0"
    "vkGetImageMemoryRequirements\0"
    "vkGetImageMemoryRequirements2\0"
    "vkGetImageMemoryRequirements2KHR\0"
    "vkGetImageSparseMemoryRequirements\0"
    "vkGetImageSparseMemoryRequirements2\0"
    "vkGetImageSparseMemoryRequirements2KHR\0"
    "vkGetImageSubresourceLayout\0"
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
    "vkGetPastPresentationTimingGOOGLE\0"
    "vkGetPerformanceParameterINTEL\0"
    "vkGetPipelineCacheData\0"
    "vkGetPipelineExecutableInternalRepresentationsKHR\0"
    "vkGetPipelineExecutablePropertiesKHR\0"
    "vkGetPipelineExecutableStatisticsKHR\0"
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
;

static const struct string_map_entry device_string_map_entries[] = {
    { 0, 0x8a43a1cc, 308 }, /* vkAcquireFullScreenExclusiveModeEXT */
    { 36, 0x6bf780dd, 223 }, /* vkAcquireImageANDROID */
    { 58, 0x82860572, 181 }, /* vkAcquireNextImage2KHR */
    { 81, 0xc3fedb2e, 133 }, /* vkAcquireNextImageKHR */
    { 103, 0x33d2767, 323 }, /* vkAcquirePerformanceConfigurationINTEL */
    { 142, 0xaf1d64ad, 310 }, /* vkAcquireProfilingLockKHR */
    { 168, 0x8c0c811a, 75 }, /* vkAllocateCommandBuffers */
    { 193, 0x4c449d3a, 64 }, /* vkAllocateDescriptorSets */
    { 218, 0x522b85d3, 6 }, /* vkAllocateMemory */
    { 235, 0xc54f7327, 77 }, /* vkBeginCommandBuffer */
    { 256, 0x3ec4e21a, 281 }, /* vkBindAccelerationStructureMemoryNV */
    { 292, 0x6bcbdcb, 14 }, /* vkBindBufferMemory */
    { 311, 0xc27aaf4f, 173 }, /* vkBindBufferMemory2 */
    { 331, 0x6878d3ce, 174 }, /* vkBindBufferMemory2KHR */
    { 354, 0x5caaae4a, 16 }, /* vkBindImageMemory */
    { 372, 0xa9097118, 175 }, /* vkBindImageMemory2 */
    { 391, 0xf18729ad, 176 }, /* vkBindImageMemory2KHR */
    { 413, 0x61c0a1e7, 420 }, /* vkBindVideoSessionMemoryKHR */
    { 441, 0xf7d6c55c, 336 }, /* vkBuildAccelerationStructuresKHR */
    { 474, 0xe561c19f, 119 }, /* vkCmdBeginConditionalRenderingEXT */
    { 508, 0x6184193f, 233 }, /* vkCmdBeginDebugUtilsLabelEXT */
    { 537, 0xf5064ea4, 117 }, /* vkCmdBeginQuery */
    { 553, 0x73251a2c, 265 }, /* vkCmdBeginQueryIndexedEXT */
    { 579, 0xcb7a58e3, 125 }, /* vkCmdBeginRenderPass */
    { 600, 0x9c876577, 240 }, /* vkCmdBeginRenderPass2 */
    { 622, 0x8b6b4de6, 241 }, /* vkCmdBeginRenderPass2KHR */
    { 647, 0x385cfdb8, 438 }, /* vkCmdBeginRendering */
    { 667, 0x50d17e0d, 439 }, /* vkCmdBeginRenderingKHR */
    { 690, 0xb217c94, 263 }, /* vkCmdBeginTransformFeedbackEXT */
    { 721, 0xd941eabc, 422 }, /* vkCmdBeginVideoCodingKHR */
    { 746, 0x28c7a5da, 90 }, /* vkCmdBindDescriptorSets */
    { 770, 0x4c22d870, 91 }, /* vkCmdBindIndexBuffer */
    { 791, 0xa8f55bdd, 277 }, /* vkCmdBindInvocationMaskHUAWEI */
    { 821, 0x3af9fd84, 80 }, /* vkCmdBindPipeline */
    { 839, 0x353570d6, 143 }, /* vkCmdBindPipelineShaderGroupNV */
    { 870, 0xbae753eb, 269 }, /* vkCmdBindShadingRateImageNV */
    { 898, 0x98fdb5cd, 262 }, /* vkCmdBindTransformFeedbackBuffersEXT */
    { 935, 0xa9c83f1d, 92 }, /* vkCmdBindVertexBuffers */
    { 958, 0x4c6b42ed, 353 }, /* vkCmdBindVertexBuffers2 */
    { 982, 0x30a5f2ec, 354 }, /* vkCmdBindVertexBuffers2EXT */
    { 1009, 0x331ebf89, 104 }, /* vkCmdBlitImage */
    { 1024, 0xb4185c21, 387 }, /* vkCmdBlitImage2 */
    { 1040, 0x785f984c, 388 }, /* vkCmdBlitImage2KHR */
    { 1059, 0x8e9d180a, 291 }, /* vkCmdBuildAccelerationStructureNV */
    { 1093, 0xb02d6bee, 335 }, /* vkCmdBuildAccelerationStructuresIndirectKHR */
    { 1137, 0x25a727dc, 334 }, /* vkCmdBuildAccelerationStructuresKHR */
    { 1173, 0x93cb5cb8, 111 }, /* vkCmdClearAttachments */
    { 1195, 0xb4bc8d08, 109 }, /* vkCmdClearColorImage */
    { 1216, 0x4f88e4ba, 110 }, /* vkCmdClearDepthStencilImage */
    { 1244, 0x2a5f6f70, 423 }, /* vkCmdControlVideoCodingKHR */
    { 1271, 0xe2d4fe2c, 283 }, /* vkCmdCopyAccelerationStructureKHR */
    { 1305, 0x84ab5629, 282 }, /* vkCmdCopyAccelerationStructureNV */
    { 1338, 0x46b2a8a0, 285 }, /* vkCmdCopyAccelerationStructureToMemoryKHR */
    { 1380, 0xc939a0da, 102 }, /* vkCmdCopyBuffer */
    { 1396, 0xa419e608, 383 }, /* vkCmdCopyBuffer2 */
    { 1413, 0x90c5563d, 384 }, /* vkCmdCopyBuffer2KHR */
    { 1433, 0x929847e, 105 }, /* vkCmdCopyBufferToImage */
    { 1456, 0x4a8ce444, 389 }, /* vkCmdCopyBufferToImage2 */
    { 1480, 0x1e9f6861, 390 }, /* vkCmdCopyBufferToImage2KHR */
    { 1507, 0x278effa9, 103 }, /* vkCmdCopyImage */
    { 1522, 0x942b5301, 385 }, /* vkCmdCopyImage2 */
    { 1538, 0xdad52c6c, 386 }, /* vkCmdCopyImage2KHR */
    { 1557, 0x68cddbac, 106 }, /* vkCmdCopyImageToBuffer */
    { 1580, 0x83c9426, 391 }, /* vkCmdCopyImageToBuffer2 */
    { 1604, 0x2db6484f, 392 }, /* vkCmdCopyImageToBuffer2KHR */
    { 1631, 0xa76c5fd8, 287 }, /* vkCmdCopyMemoryToAccelerationStructureKHR */
    { 1673, 0xdee8c6d4, 123 }, /* vkCmdCopyQueryPoolResults */
    { 1699, 0x29000809, 430 }, /* vkCmdCuLaunchKernelNVX */
    { 1722, 0xaec8bb10, 137 }, /* vkCmdDebugMarkerBeginEXT */
    { 1747, 0xd6a1433e, 138 }, /* vkCmdDebugMarkerEndEXT */
    { 1770, 0x173d440, 139 }, /* vkCmdDebugMarkerInsertEXT */
    { 1796, 0x5c38928d, 421 }, /* vkCmdDecodeVideoKHR */
    { 1816, 0xbd58e867, 99 }, /* vkCmdDispatch */
    { 1830, 0xfb767220, 182 }, /* vkCmdDispatchBase */
    { 1848, 0x402403e5, 183 }, /* vkCmdDispatchBaseKHR */
    { 1869, 0xd6353005, 100 }, /* vkCmdDispatchIndirect */
    { 1891, 0x9912c1a1, 93 }, /* vkCmdDraw */
    { 1901, 0xbe5a8058, 94 }, /* vkCmdDrawIndexed */
    { 1918, 0x94e7ed36, 98 }, /* vkCmdDrawIndexedIndirect */
    { 1943, 0xb4acef41, 257 }, /* vkCmdDrawIndexedIndirectCount */
    { 1973, 0xc86e9287, 259 }, /* vkCmdDrawIndexedIndirectCountAMD */
    { 2006, 0xda9e8a2c, 258 }, /* vkCmdDrawIndexedIndirectCountKHR */
    { 2039, 0xe9ac41bf, 97 }, /* vkCmdDrawIndirect */
    { 2057, 0x80c3b089, 267 }, /* vkCmdDrawIndirectByteCountEXT */
    { 2087, 0x40079990, 254 }, /* vkCmdDrawIndirectCount */
    { 2110, 0xe5ad0a50, 256 }, /* vkCmdDrawIndirectCountAMD */
    { 2136, 0xf7dd01f5, 255 }, /* vkCmdDrawIndirectCountKHR */
    { 2162, 0xf21f391, 274 }, /* vkCmdDrawMeshTasksIndirectCountNV */
    { 2196, 0xaecd0a06, 273 }, /* vkCmdDrawMeshTasksIndirectNV */
    { 2225, 0xfba21ac8, 272 }, /* vkCmdDrawMeshTasksNV */
    { 2246, 0xa6c231d9, 95 }, /* vkCmdDrawMultiEXT */
    { 2264, 0xb6bd0f40, 96 }, /* vkCmdDrawMultiIndexedEXT */
    { 2289, 0x552d044d, 425 }, /* vkCmdEncodeVideoKHR */
    { 2309, 0x18c8217d, 120 }, /* vkCmdEndConditionalRenderingEXT */
    { 2341, 0x29875911, 234 }, /* vkCmdEndDebugUtilsLabelEXT */
    { 2368, 0xd556fd22, 118 }, /* vkCmdEndQuery */
    { 2382, 0xd5c2f48a, 266 }, /* vkCmdEndQueryIndexedEXT */
    { 2406, 0xdcdb0235, 127 }, /* vkCmdEndRenderPass */
    { 2425, 0x1cbf9115, 244 }, /* vkCmdEndRenderPass2 */
    { 2445, 0x57eebe78, 245 }, /* vkCmdEndRenderPass2KHR */
    { 2468, 0x22c5e6f6, 440 }, /* vkCmdEndRendering */
    { 2486, 0xabf9ff, 441 }, /* vkCmdEndRenderingKHR */
    { 2507, 0xf008d706, 264 }, /* vkCmdEndTransformFeedbackEXT */
    { 2536, 0xa5c55b4e, 424 }, /* vkCmdEndVideoCodingKHR */
    { 2559, 0x9eaabe40, 128 }, /* vkCmdExecuteCommands */
    { 2580, 0xe02372d7, 141 }, /* vkCmdExecuteGeneratedCommandsNV */
    { 2612, 0x5bdd2ae0, 108 }, /* vkCmdFillBuffer */
    { 2628, 0xce6aa7d1, 235 }, /* vkCmdInsertDebugUtilsLabelEXT */
    { 2658, 0x2eeec2f9, 126 }, /* vkCmdNextSubpass */
    { 2675, 0xd4fc131, 242 }, /* vkCmdNextSubpass2 */
    { 2693, 0x25b621bc, 243 }, /* vkCmdNextSubpass2KHR */
    { 2714, 0x97fccfe8, 116 }, /* vkCmdPipelineBarrier */
    { 2735, 0x43d8c70a, 406 }, /* vkCmdPipelineBarrier2 */
    { 2757, 0x9654ba0b, 407 }, /* vkCmdPipelineBarrier2KHR */
    { 2782, 0x26eff1e6, 142 }, /* vkCmdPreprocessGeneratedCommandsNV */
    { 2817, 0xb1c6b468, 124 }, /* vkCmdPushConstants */
    { 2836, 0xf17232a1, 147 }, /* vkCmdPushDescriptorSetKHR */
    { 2862, 0x3d528981, 190 }, /* vkCmdPushDescriptorSetWithTemplateKHR */
    { 2900, 0x4fccce28, 114 }, /* vkCmdResetEvent */
    { 2916, 0x735fc6ca, 402 }, /* vkCmdResetEvent2 */
    { 2933, 0x950a204b, 403 }, /* vkCmdResetEvent2KHR */
    { 2953, 0x2f614082, 121 }, /* vkCmdResetQueryPool */
    { 2973, 0x671bb594, 112 }, /* vkCmdResolveImage */
    { 2991, 0xe79d80fe, 393 }, /* vkCmdResolveImage2 */
    { 3010, 0x9fea6337, 394 }, /* vkCmdResolveImage2KHR */
    { 3032, 0x1c989dfb, 85 }, /* vkCmdSetBlendConstants */
    { 3055, 0x4331556d, 260 }, /* vkCmdSetCheckpointNV */
    { 3076, 0xcd76e1c0, 271 }, /* vkCmdSetCoarseSampleOrderNV */
    { 3104, 0x5402d31d, 399 }, /* vkCmdSetColorWriteEnableEXT */
    { 3132, 0xcf691c42, 343 }, /* vkCmdSetCullMode */
    { 3149, 0xb7fcea1f, 344 }, /* vkCmdSetCullModeEXT */
    { 3169, 0x30f14d07, 84 }, /* vkCmdSetDepthBias */
    { 3187, 0xc07e1b1a, 370 }, /* vkCmdSetDepthBiasEnable */
    { 3211, 0x5d604307, 371 }, /* vkCmdSetDepthBiasEnableEXT */
    { 3238, 0x7b3a8a63, 86 }, /* vkCmdSetDepthBounds */
    { 3258, 0xe72cce0, 361 }, /* vkCmdSetDepthBoundsTestEnable */
    { 3288, 0x3f2ddb1, 362 }, /* vkCmdSetDepthBoundsTestEnableEXT */
    { 3321, 0xda98add0, 359 }, /* vkCmdSetDepthCompareOp */
    { 3344, 0x2f377e41, 360 }, /* vkCmdSetDepthCompareOpEXT */
    { 3370, 0x68666de3, 355 }, /* vkCmdSetDepthTestEnable */
    { 3394, 0x57c5efe6, 356 }, /* vkCmdSetDepthTestEnableEXT */
    { 3421, 0x373bda6c, 357 }, /* vkCmdSetDepthWriteEnable */
    { 3446, 0xbe217905, 358 }, /* vkCmdSetDepthWriteEnableEXT */
    { 3474, 0xaecdae87, 177 }, /* vkCmdSetDeviceMask */
    { 3493, 0xfbb79356, 178 }, /* vkCmdSetDeviceMaskKHR */
    { 3515, 0x64df188b, 196 }, /* vkCmdSetDiscardRectangleEXT */
    { 3543, 0xe257f075, 113 }, /* vkCmdSetEvent */
    { 3557, 0x3d5620d5, 400 }, /* vkCmdSetEvent2 */
    { 3572, 0xa3c714b8, 401 }, /* vkCmdSetEvent2KHR */
    { 3590, 0xb2537e63, 268 }, /* vkCmdSetExclusiveScissorNV */
    { 3617, 0x7670296e, 396 }, /* vkCmdSetFragmentShadingRateEnumNV */
    { 3651, 0x4c696cd8, 395 }, /* vkCmdSetFragmentShadingRateKHR */
    { 3682, 0x4cd999a9, 345 }, /* vkCmdSetFrontFace */
    { 3700, 0xa7a7a090, 346 }, /* vkCmdSetFrontFaceEXT */
    { 3721, 0xbdaa62f9, 332 }, /* vkCmdSetLineStippleEXT */
    { 3744, 0x32282165, 83 }, /* vkCmdSetLineWidth */
    { 3762, 0x7689581f, 372 }, /* vkCmdSetLogicOpEXT */
    { 3781, 0x58604abc, 367 }, /* vkCmdSetPatchControlPointsEXT */
    { 3811, 0x4eb21af9, 320 }, /* vkCmdSetPerformanceMarkerINTEL */
    { 3842, 0x30d793c7, 322 }, /* vkCmdSetPerformanceOverrideINTEL */
    { 3875, 0xc50b03a9, 321 }, /* vkCmdSetPerformanceStreamMarkerINTEL */
    { 3912, 0x2b3504c0, 373 }, /* vkCmdSetPrimitiveRestartEnable */
    { 3943, 0x28d998d1, 374 }, /* vkCmdSetPrimitiveRestartEnableEXT */
    { 3977, 0xb9524b01, 347 }, /* vkCmdSetPrimitiveTopology */
    { 4003, 0x1dacaf8, 348 }, /* vkCmdSetPrimitiveTopologyEXT */
    { 4032, 0x81319b79, 368 }, /* vkCmdSetRasterizerDiscardEnable */
    { 4064, 0x1f7bb40, 369 }, /* vkCmdSetRasterizerDiscardEnableEXT */
    { 4099, 0xd056ef9b, 304 }, /* vkCmdSetRayTracingPipelineStackSizeKHR */
    { 4138, 0xa9e2c72, 197 }, /* vkCmdSetSampleLocationsEXT */
    { 4165, 0x48f28c7f, 82 }, /* vkCmdSetScissor */
    { 4181, 0x159097b2, 351 }, /* vkCmdSetScissorWithCount */
    { 4206, 0xf349b42f, 352 }, /* vkCmdSetScissorWithCountEXT */
    { 4234, 0xa8f534e2, 87 }, /* vkCmdSetStencilCompareMask */
    { 4261, 0x43020f38, 365 }, /* vkCmdSetStencilOp */
    { 4279, 0xbb885f19, 366 }, /* vkCmdSetStencilOpEXT */
    { 4300, 0x83e2b024, 89 }, /* vkCmdSetStencilReference */
    { 4325, 0x63fedc5c, 363 }, /* vkCmdSetStencilTestEnable */
    { 4351, 0x16cc6095, 364 }, /* vkCmdSetStencilTestEnableEXT */
    { 4380, 0xe7c4b134, 88 }, /* vkCmdSetStencilWriteMask */
    { 4405, 0x9dd954c8, 398 }, /* vkCmdSetVertexInputEXT */
    { 4428, 0x53d6c2b, 81 }, /* vkCmdSetViewport */
    { 4445, 0x54d063a4, 270 }, /* vkCmdSetViewportShadingRatePaletteNV */
    { 4482, 0x60ee2453, 195 }, /* vkCmdSetViewportWScalingNV */
    { 4509, 0xbdea58a6, 349 }, /* vkCmdSetViewportWithCount */
    { 4535, 0xa3d72e5b, 350 }, /* vkCmdSetViewportWithCountEXT */
    { 4564, 0xf2c7909d, 101 }, /* vkCmdSubpassShadingHUAWEI */
    { 4590, 0xaf8c1f1e, 301 }, /* vkCmdTraceRaysIndirectKHR */
    { 4616, 0x5eb65f0c, 293 }, /* vkCmdTraceRaysKHR */
    { 4634, 0xe8687c49, 294 }, /* vkCmdTraceRaysNV */
    { 4651, 0xd2986b5e, 107 }, /* vkCmdUpdateBuffer */
    { 4669, 0x3b9346b3, 115 }, /* vkCmdWaitEvents */
    { 4685, 0xcd17b527, 404 }, /* vkCmdWaitEvents2 */
    { 4702, 0x8c98fdb6, 405 }, /* vkCmdWaitEvents2KHR */
    { 4722, 0x9ecb3888, 289 }, /* vkCmdWriteAccelerationStructuresPropertiesKHR */
    { 4768, 0xd2925ead, 290 }, /* vkCmdWriteAccelerationStructuresPropertiesNV */
    { 4813, 0xe277f952, 412 }, /* vkCmdWriteBufferMarker2AMD */
    { 4840, 0x447be82c, 237 }, /* vkCmdWriteBufferMarkerAMD */
    { 4866, 0xec4d324c, 122 }, /* vkCmdWriteTimestamp */
    { 4886, 0xcff32086, 410 }, /* vkCmdWriteTimestamp2 */
    { 4907, 0xa43a02ef, 411 }, /* vkCmdWriteTimestamp2KHR */
    { 4931, 0xbad693ed, 275 }, /* vkCompileDeferredNV */
    { 4951, 0x45e623ac, 284 }, /* vkCopyAccelerationStructureKHR */
    { 4982, 0x9726ae20, 286 }, /* vkCopyAccelerationStructureToMemoryKHR */
    { 5021, 0xf7e06558, 288 }, /* vkCopyMemoryToAccelerationStructureKHR */
    { 5060, 0x30dceabb, 333 }, /* vkCreateAccelerationStructureKHR */
    { 5093, 0x9dc98a12, 276 }, /* vkCreateAccelerationStructureNV */
    { 5125, 0x7d4282b9, 36 }, /* vkCreateBuffer */
    { 5140, 0xaba0b50, 433 }, /* vkCreateBufferCollectionFUCHSIA */
    { 5172, 0x925bd256, 38 }, /* vkCreateBufferView */
    { 5191, 0x820fe476, 72 }, /* vkCreateCommandPool */
    { 5211, 0xf70c85eb, 52 }, /* vkCreateComputePipelines */
    { 5236, 0x9bff3a5d, 427 }, /* vkCreateCuFunctionNVX */
    { 5258, 0xf3c09939, 426 }, /* vkCreateCuModuleNVX */
    { 5278, 0x3f5d1a36, 338 }, /* vkCreateDeferredOperationKHR */
    { 5307, 0xfb95a8a4, 61 }, /* vkCreateDescriptorPool */
    { 5330, 0x3c14cc74, 59 }, /* vkCreateDescriptorSetLayout */
    { 5358, 0xad3ce733, 184 }, /* vkCreateDescriptorUpdateTemplate */
    { 5391, 0x5189488a, 185 }, /* vkCreateDescriptorUpdateTemplateKHR */
    { 5427, 0xe7188731, 26 }, /* vkCreateEvent */
    { 5441, 0x958af968, 19 }, /* vkCreateFence */
    { 5455, 0x887a38c4, 67 }, /* vkCreateFramebuffer */
    { 5475, 0x4b59f96d, 51 }, /* vkCreateGraphicsPipelines */
    { 5501, 0x652128c2, 40 }, /* vkCreateImage */
    { 5515, 0xdce077ff, 43 }, /* vkCreateImageView */
    { 5533, 0x3bc09b11, 145 }, /* vkCreateIndirectCommandsLayoutNV */
    { 5566, 0xcbf6489f, 47 }, /* vkCreatePipelineCache */
    { 5588, 0x451ef1ed, 55 }, /* vkCreatePipelineLayout */
    { 5611, 0xd1450d02, 375 }, /* vkCreatePrivateDataSlot */
    { 5635, 0xc06d475f, 376 }, /* vkCreatePrivateDataSlotEXT */
    { 5662, 0x5edcd92b, 31 }, /* vkCreateQueryPool */
    { 5680, 0x28847755, 300 }, /* vkCreateRayTracingPipelinesKHR */
    { 5711, 0x12bca48, 299 }, /* vkCreateRayTracingPipelinesNV */
    { 5741, 0x109a9c18, 69 }, /* vkCreateRenderPass */
    { 5760, 0x46b16d5a, 238 }, /* vkCreateRenderPass2 */
    { 5780, 0xfa16043b, 239 }, /* vkCreateRenderPass2KHR */
    { 5803, 0x13cf03f, 57 }, /* vkCreateSampler */
    { 5819, 0xe6a58c26, 210 }, /* vkCreateSamplerYcbcrConversion */
    { 5850, 0x7482104f, 211 }, /* vkCreateSamplerYcbcrConversionKHR */
    { 5884, 0xf2065e5b, 24 }, /* vkCreateSemaphore */
    { 5902, 0xa0d3cea2, 45 }, /* vkCreateShaderModule */
    { 5923, 0x47655c4a, 129 }, /* vkCreateSharedSwapchainsKHR */
    { 5951, 0xcdefcaa8, 130 }, /* vkCreateSwapchainKHR */
    { 5972, 0x591d7ed9, 215 }, /* vkCreateValidationCacheEXT */
    { 5999, 0xcddb2969, 414 }, /* vkCreateVideoSessionKHR */
    { 6023, 0x83987bd7, 416 }, /* vkCreateVideoSessionParametersKHR */
    { 6057, 0xe206fb25, 135 }, /* vkDebugMarkerSetObjectNameEXT */
    { 6087, 0x30799448, 136 }, /* vkDebugMarkerSetObjectTagEXT */
    { 6116, 0x8c8648b8, 342 }, /* vkDeferredOperationJoinKHR */
    { 6143, 0x3eccc207, 278 }, /* vkDestroyAccelerationStructureKHR */
    { 6177, 0x693f9d26, 279 }, /* vkDestroyAccelerationStructureNV */
    { 6210, 0x94a07a45, 37 }, /* vkDestroyBuffer */
    { 6226, 0xd6301e64, 436 }, /* vkDestroyBufferCollectionFUCHSIA */
    { 6259, 0x98b27962, 39 }, /* vkDestroyBufferView */
    { 6279, 0xd5d83a0a, 73 }, /* vkDestroyCommandPool */
    { 6300, 0x111b9831, 429 }, /* vkDestroyCuFunctionNVX */
    { 6323, 0x4788eecd, 428 }, /* vkDestroyCuModuleNVX */
    { 6344, 0x7d549a02, 339 }, /* vkDestroyDeferredOperationKHR */
    { 6374, 0x47bdaf30, 62 }, /* vkDestroyDescriptorPool */
    { 6398, 0xa4227b08, 60 }, /* vkDestroyDescriptorSetLayout */
    { 6427, 0xbb2cbe7f, 186 }, /* vkDestroyDescriptorUpdateTemplate */
    { 6461, 0xaa83901e, 187 }, /* vkDestroyDescriptorUpdateTemplateKHR */
    { 6498, 0x1fbcc9cb, 1 }, /* vkDestroyDevice */
    { 6514, 0x4df27c05, 27 }, /* vkDestroyEvent */
    { 6529, 0xfc64ee3c, 20 }, /* vkDestroyFence */
    { 6544, 0xdc428e58, 68 }, /* vkDestroyFramebuffer */
    { 6565, 0xcbfb1d96, 41 }, /* vkDestroyImage */
    { 6580, 0xb5853953, 44 }, /* vkDestroyImageView */
    { 6599, 0x49b0725d, 146 }, /* vkDestroyIndirectCommandsLayoutNV */
    { 6633, 0x6aac68af, 54 }, /* vkDestroyPipeline */
    { 6651, 0x4112a673, 48 }, /* vkDestroyPipelineCache */
    { 6674, 0x9146f879, 56 }, /* vkDestroyPipelineLayout */
    { 6698, 0x9fc42716, 377 }, /* vkDestroyPrivateDataSlot */
    { 6723, 0xe18d5d6b, 378 }, /* vkDestroyPrivateDataSlotEXT */
    { 6751, 0x37819a7f, 32 }, /* vkDestroyQueryPool */
    { 6770, 0x16f14324, 70 }, /* vkDestroyRenderPass */
    { 6790, 0x3b645153, 58 }, /* vkDestroySampler */
    { 6807, 0x20f261b2, 212 }, /* vkDestroySamplerYcbcrConversion */
    { 6839, 0xaaa623a3, 213 }, /* vkDestroySamplerYcbcrConversionKHR */
    { 6874, 0xcaab1faf, 25 }, /* vkDestroySemaphore */
    { 6893, 0x2d77af6e, 46 }, /* vkDestroyShaderModule */
    { 6915, 0x5a93ab74, 131 }, /* vkDestroySwapchainKHR */
    { 6937, 0x7a3d94e5, 216 }, /* vkDestroyValidationCacheEXT */
    { 6965, 0x9c5a437d, 415 }, /* vkDestroyVideoSessionKHR */
    { 6990, 0xb9bc8f2b, 418 }, /* vkDestroyVideoSessionParametersKHR */
    { 7025, 0xd46c5f24, 5 }, /* vkDeviceWaitIdle */
    { 7042, 0xdbb064, 167 }, /* vkDisplayPowerControlEXT */
    { 7067, 0xaffb5725, 78 }, /* vkEndCommandBuffer */
    { 7086, 0xff52f051, 10 }, /* vkFlushMappedMemoryRanges */
    { 7112, 0xb9db2b91, 76 }, /* vkFreeCommandBuffers */
    { 7133, 0x7a1347b1, 65 }, /* vkFreeDescriptorSets */
    { 7154, 0x8f6f838a, 7 }, /* vkFreeMemory */
    { 7167, 0x4dbe8d2f, 397 }, /* vkGetAccelerationStructureBuildSizesKHR */
    { 7207, 0x1a50de81, 337 }, /* vkGetAccelerationStructureDeviceAddressKHR */
    { 7250, 0xd26f255a, 298 }, /* vkGetAccelerationStructureHandleNV */
    { 7285, 0x5d79203, 280 }, /* vkGetAccelerationStructureMemoryRequirementsNV */
    { 7332, 0xb891b5e, 252 }, /* vkGetAndroidHardwareBufferPropertiesANDROID */
    { 7376, 0x14b1e93d, 437 }, /* vkGetBufferCollectionPropertiesFUCHSIA */
    { 7415, 0x7022f0cd, 315 }, /* vkGetBufferDeviceAddress */
    { 7440, 0x3703280c, 317 }, /* vkGetBufferDeviceAddressEXT */
    { 7468, 0x713b5180, 316 }, /* vkGetBufferDeviceAddressKHR */
    { 7496, 0xab98422a, 13 }, /* vkGetBufferMemoryRequirements */
    { 7526, 0xd1fd0638, 198 }, /* vkGetBufferMemoryRequirements2 */
    { 7557, 0x78dbe98d, 199 }, /* vkGetBufferMemoryRequirements2KHR */
    { 7591, 0x2a5545a0, 313 }, /* vkGetBufferOpaqueCaptureAddress */
    { 7623, 0xddac1c65, 314 }, /* vkGetBufferOpaqueCaptureAddressKHR */
    { 7658, 0xcf3070fe, 227 }, /* vkGetCalibratedTimestampsEXT */
    { 7687, 0x7d902967, 340 }, /* vkGetDeferredOperationMaxConcurrencyKHR */
    { 7727, 0xf2144be9, 341 }, /* vkGetDeferredOperationResultKHR */
    { 7759, 0x19faddac, 443 }, /* vkGetDescriptorSetHostMappingVALVE */
    { 7794, 0x5485f810, 442 }, /* vkGetDescriptorSetLayoutHostMappingInfoVALVE */
    { 7839, 0xfeac9573, 219 }, /* vkGetDescriptorSetLayoutSupport */
    { 7871, 0xd7e44a, 220 }, /* vkGetDescriptorSetLayoutSupportKHR */
    { 7906, 0xe86334c9, 302 }, /* vkGetDeviceAccelerationStructureCompatibilityKHR */
    { 7955, 0xec1a1918, 204 }, /* vkGetDeviceBufferMemoryRequirements */
    { 7991, 0xa56ac1ad, 205 }, /* vkGetDeviceBufferMemoryRequirementsKHR */
    { 8030, 0x2e218c10, 171 }, /* vkGetDeviceGroupPeerMemoryFeatures */
    { 8065, 0xa3809375, 172 }, /* vkGetDeviceGroupPeerMemoryFeaturesKHR */
    { 8103, 0xf72c87d4, 179 }, /* vkGetDeviceGroupPresentCapabilitiesKHR */
    { 8142, 0x41b28e81, 307 }, /* vkGetDeviceGroupSurfacePresentModes2EXT */
    { 8182, 0x6b9448c3, 180 }, /* vkGetDeviceGroupSurfacePresentModesKHR */
    { 8221, 0x5f18b6e5, 206 }, /* vkGetDeviceImageMemoryRequirements */
    { 8256, 0x3a2c5528, 207 }, /* vkGetDeviceImageMemoryRequirementsKHR */
    { 8294, 0x1ac18abd, 208 }, /* vkGetDeviceImageSparseMemoryRequirements */
    { 8335, 0xb8906110, 209 }, /* vkGetDeviceImageSparseMemoryRequirementsKHR */
    { 8379, 0x46e38db5, 12 }, /* vkGetDeviceMemoryCommitment */
    { 8407, 0x9a0fe777, 327 }, /* vkGetDeviceMemoryOpaqueCaptureAddress */
    { 8445, 0x49339be6, 328 }, /* vkGetDeviceMemoryOpaqueCaptureAddressKHR */
    { 8486, 0xba013486, 0 }, /* vkGetDeviceProcAddr */
    { 8506, 0xcc920d9a, 2 }, /* vkGetDeviceQueue */
    { 8523, 0xb11a6348, 214 }, /* vkGetDeviceQueue2 */
    { 8541, 0x9d280cca, 53 }, /* vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI */
    { 8589, 0x96d834b, 28 }, /* vkGetEventStatus */
    { 8606, 0x69a5d6af, 165 }, /* vkGetFenceFdKHR */
    { 8622, 0x5f391892, 22 }, /* vkGetFenceStatus */
    { 8639, 0x8963de2, 163 }, /* vkGetFenceWin32HandleKHR */
    { 8664, 0xac420aaf, 144 }, /* vkGetGeneratedCommandsMemoryRequirementsNV */
    { 8707, 0x12fa78a3, 312 }, /* vkGetImageDrmFormatModifierPropertiesEXT */
    { 8748, 0x916f1e63, 15 }, /* vkGetImageMemoryRequirements */
    { 8777, 0x56e213f7, 200 }, /* vkGetImageMemoryRequirements2 */
    { 8807, 0x8de28366, 201 }, /* vkGetImageMemoryRequirements2KHR */
    { 8840, 0x15855f5b, 17 }, /* vkGetImageSparseMemoryRequirements */
    { 8875, 0xbd4e3d3f, 202 }, /* vkGetImageSparseMemoryRequirements2 */
    { 8911, 0x3df40f5e, 203 }, /* vkGetImageSparseMemoryRequirements2KHR */
    { 8950, 0x9163b686, 42 }, /* vkGetImageSubresourceLayout */
    { 8978, 0xed8f1d33, 306 }, /* vkGetImageViewAddressNVX */
    { 9003, 0x20caa1e1, 305 }, /* vkGetImageViewHandleNVX */
    { 9027, 0x71220e82, 253 }, /* vkGetMemoryAndroidHardwareBufferANDROID */
    { 9067, 0x503c14c5, 152 }, /* vkGetMemoryFdKHR */
    { 9084, 0xb028a792, 153 }, /* vkGetMemoryFdPropertiesKHR */
    { 9111, 0x7030ee5b, 236 }, /* vkGetMemoryHostPointerPropertiesEXT */
    { 9147, 0x93d6c7a4, 156 }, /* vkGetMemoryRemoteAddressNV */
    { 9174, 0x45fc7e1c, 150 }, /* vkGetMemoryWin32HandleKHR */
    { 9200, 0xc8795b9, 140 }, /* vkGetMemoryWin32HandleNV */
    { 9225, 0xb8f59859, 151 }, /* vkGetMemoryWin32HandlePropertiesKHR */
    { 9261, 0x4540b38e, 154 }, /* vkGetMemoryZirconHandleFUCHSIA */
    { 9292, 0x5a4149eb, 155 }, /* vkGetMemoryZirconHandlePropertiesFUCHSIA */
    { 9333, 0x19616a98, 194 }, /* vkGetPastPresentationTimingGOOGLE */
    { 9367, 0x1ec6c4ec, 326 }, /* vkGetPerformanceParameterINTEL */
    { 9398, 0x2092a349, 49 }, /* vkGetPipelineCacheData */
    { 9421, 0x8b20fc09, 331 }, /* vkGetPipelineExecutableInternalRepresentationsKHR */
    { 9471, 0x748dd8cd, 329 }, /* vkGetPipelineExecutablePropertiesKHR */
    { 9508, 0x5c4d6435, 330 }, /* vkGetPipelineExecutableStatisticsKHR */
    { 9545, 0xa60eca94, 381 }, /* vkGetPrivateData */
    { 9562, 0x2dc1491d, 382 }, /* vkGetPrivateDataEXT */
    { 9582, 0xbf3f2cb3, 33 }, /* vkGetQueryPoolResults */
    { 9604, 0xf6cef020, 413 }, /* vkGetQueueCheckpointData2NV */
    { 9632, 0x428d4692, 261 }, /* vkGetQueueCheckpointDataNV */
    { 9659, 0x4b32ff8, 297 }, /* vkGetRayTracingCaptureReplayShaderGroupHandlesKHR */
    { 9709, 0x4693e853, 295 }, /* vkGetRayTracingShaderGroupHandlesKHR */
    { 9746, 0x3b54d93a, 296 }, /* vkGetRayTracingShaderGroupHandlesNV */
    { 9782, 0x2f709815, 303 }, /* vkGetRayTracingShaderGroupStackSizeKHR */
    { 9821, 0x85a9d101, 193 }, /* vkGetRefreshCycleDurationGOOGLE */
    { 9853, 0xa9820d22, 71 }, /* vkGetRenderAreaGranularity */
    { 9880, 0xd05a61a0, 246 }, /* vkGetSemaphoreCounterValue */
    { 9907, 0xf3c26065, 247 }, /* vkGetSemaphoreCounterValueKHR */
    { 9937, 0x3e0e9884, 159 }, /* vkGetSemaphoreFdKHR */
    { 9957, 0xd04be5e5, 157 }, /* vkGetSemaphoreWin32HandleKHR */
    { 9986, 0x37c0989d, 161 }, /* vkGetSemaphoreZirconHandleFUCHSIA */
    { 10020, 0x5330743c, 225 }, /* vkGetShaderInfoAMD */
    { 10039, 0xa4aeb5a, 170 }, /* vkGetSwapchainCounterEXT */
    { 10064, 0x219d929, 222 }, /* vkGetSwapchainGrallocUsage2ANDROID */
    { 10099, 0x4979c9a3, 221 }, /* vkGetSwapchainGrallocUsageANDROID */
    { 10133, 0x57695f28, 132 }, /* vkGetSwapchainImagesKHR */
    { 10157, 0x66ae725e, 192 }, /* vkGetSwapchainStatusKHR */
    { 10181, 0xbbc9f99f, 217 }, /* vkGetValidationCacheDataEXT */
    { 10209, 0xd8960270, 419 }, /* vkGetVideoSessionMemoryRequirementsKHR */
    { 10248, 0x51df0390, 166 }, /* vkImportFenceFdKHR */
    { 10267, 0x1bcbb079, 164 }, /* vkImportFenceWin32HandleKHR */
    { 10295, 0x36337c05, 160 }, /* vkImportSemaphoreFdKHR */
    { 10318, 0x7e2cfcdc, 158 }, /* vkImportSemaphoreWin32HandleKHR */
    { 10350, 0x4d1996ce, 162 }, /* vkImportSemaphoreZirconHandleFUCHSIA */
    { 10387, 0x65a01d77, 318 }, /* vkInitializePerformanceApiINTEL */
    { 10419, 0x1e115cca, 11 }, /* vkInvalidateMappedMemoryRanges */
    { 10450, 0xcb977bd8, 8 }, /* vkMapMemory */
    { 10462, 0xc3499606, 50 }, /* vkMergePipelineCaches */
    { 10484, 0xe8fe1154, 218 }, /* vkMergeValidationCachesEXT */
    { 10511, 0xcb7dc88, 230 }, /* vkQueueBeginDebugUtilsLabelEXT */
    { 10542, 0xc3628a09, 18 }, /* vkQueueBindSparse */
    { 10560, 0xf130b20a, 231 }, /* vkQueueEndDebugUtilsLabelEXT */
    { 10589, 0x56027200, 232 }, /* vkQueueInsertDebugUtilsLabelEXT */
    { 10621, 0xfc5fb6ce, 134 }, /* vkQueuePresentKHR */
    { 10639, 0xf8499f82, 325 }, /* vkQueueSetPerformanceConfigurationINTEL */
    { 10679, 0xa0313eef, 224 }, /* vkQueueSignalReleaseImageANDROID */
    { 10712, 0xfa4713ec, 3 }, /* vkQueueSubmit */
    { 10726, 0xbf0609e6, 408 }, /* vkQueueSubmit2 */
    { 10741, 0xc2dd288f, 409 }, /* vkQueueSubmit2KHR */
    { 10759, 0x6f8fc2a5, 4 }, /* vkQueueWaitIdle */
    { 10775, 0x26cc78f5, 168 }, /* vkRegisterDeviceEventEXT */
    { 10800, 0x4a0bd849, 169 }, /* vkRegisterDisplayEventEXT */
    { 10826, 0x13814325, 309 }, /* vkReleaseFullScreenExclusiveModeEXT */
    { 10862, 0x28575036, 324 }, /* vkReleasePerformanceConfigurationINTEL */
    { 10901, 0x8bdecb76, 311 }, /* vkReleaseProfilingLockKHR */
    { 10927, 0x847dc731, 79 }, /* vkResetCommandBuffer */
    { 10948, 0x6da9f7fd, 74 }, /* vkResetCommandPool */
    { 10967, 0x9bd85f5, 63 }, /* vkResetDescriptorPool */
    { 10989, 0x6d373ba8, 30 }, /* vkResetEvent */
    { 11002, 0x684781dc, 21 }, /* vkResetFences */
    { 11016, 0x4e671e02, 34 }, /* vkResetQueryPool */
    { 11033, 0xe6701e5f, 35 }, /* vkResetQueryPoolEXT */
    { 11053, 0x3c97f0dc, 434 }, /* vkSetBufferCollectionBufferConstraintsFUCHSIA */
    { 11099, 0xcae02471, 435 }, /* vkSetBufferCollectionImageConstraintsFUCHSIA */
    { 11144, 0x180cec44, 228 }, /* vkSetDebugUtilsObjectNameEXT */
    { 11173, 0x15942821, 229 }, /* vkSetDebugUtilsObjectTagEXT */
    { 11201, 0xa42f1309, 431 }, /* vkSetDeviceMemoryPriorityEXT */
    { 11230, 0x592ae5f5, 29 }, /* vkSetEvent */
    { 11241, 0xa20f1ea9, 191 }, /* vkSetHdrMetadataEXT */
    { 11261, 0xbd1cd781, 226 }, /* vkSetLocalDimmingAMD */
    { 11282, 0x5cf49ca8, 379 }, /* vkSetPrivateData */
    { 11299, 0x23456729, 380 }, /* vkSetPrivateDataEXT */
    { 11319, 0xcd347297, 250 }, /* vkSignalSemaphore */
    { 11337, 0x8fef55c6, 251 }, /* vkSignalSemaphoreKHR */
    { 11358, 0xfef2fb38, 148 }, /* vkTrimCommandPool */
    { 11376, 0x51177c8d, 149 }, /* vkTrimCommandPoolKHR */
    { 11397, 0x408975ae, 319 }, /* vkUninitializePerformanceApiINTEL */
    { 11431, 0x1a1a0e2f, 9 }, /* vkUnmapMemory */
    { 11445, 0x5349c9d, 188 }, /* vkUpdateDescriptorSetWithTemplate */
    { 11479, 0x214ad230, 189 }, /* vkUpdateDescriptorSetWithTemplateKHR */
    { 11516, 0xbfd090ae, 66 }, /* vkUpdateDescriptorSets */
    { 11539, 0x8e570a3a, 417 }, /* vkUpdateVideoSessionParametersKHR */
    { 11573, 0x19d64c81, 23 }, /* vkWaitForFences */
    { 11589, 0x19c136b1, 432 }, /* vkWaitForPresentKHR */
    { 11609, 0x74368ad9, 248 }, /* vkWaitSemaphores */
    { 11626, 0x2bc77454, 249 }, /* vkWaitSemaphoresKHR */
    { 11646, 0x8bc9ae08, 292 }, /* vkWriteAccelerationStructuresPropertiesKHR */
};

/* Hash table stats:
 * size 444 entries
 * collisions entries:
 *     0      357
 *     1      51
 *     2      21
 *     3      6
 *     4      4
 *     5      2
 *     6      1
 *     7      2
 *     8      0
 *     9+     0
 */

#define none 0xffff
static const uint16_t device_string_map[1024] = {
    none,
    none,
    none,
    none,
    none,
    0x004b,
    none,
    none,
    none,
    0x0043,
    0x002b,
    none,
    0x012a,
    none,
    none,
    none,
    0x0135,
    none,
    none,
    none,
    none,
    0x0172,
    none,
    none,
    0x00e8,
    none,
    none,
    none,
    0x0167,
    none,
    0x0104,
    0x009c,
    0x016d,
    0x0029,
    none,
    0x013b,
    0x00af,
    none,
    0x003f,
    none,
    none,
    none,
    none,
    0x00b4,
    0x00c4,
    none,
    none,
    0x00ab,
    none,
    0x00fe,
    none,
    none,
    0x01a6,
    0x0169,
    0x019a,
    none,
    none,
    0x00ec,
    none,
    0x00ea,
    0x017a,
    none,
    0x0106,
    0x00eb,
    0x0046,
    none,
    0x0081,
    none,
    0x0039,
    none,
    none,
    none,
    0x00f6,
    0x00bc,
    0x00f0,
    0x0078,
    0x002a,
    0x005c,
    none,
    0x0040,
    none,
    0x011f,
    none,
    0x00b6,
    0x01ba,
    none,
    none,
    0x01a5,
    0x004d,
    0x0161,
    none,
    none,
    0x00b0,
    0x0137,
    none,
    none,
    none,
    0x003a,
    0x00ed,
    none,
    0x011d,
    0x0130,
    0x0170,
    0x0048,
    0x0073,
    none,
    none,
    none,
    0x003d,
    none,
    none,
    0x0198,
    0x001f,
    0x01a4,
    0x00a8,
    none,
    0x00d7,
    0x0092,
    0x00d1,
    0x0184,
    0x0176,
    0x010e,
    none,
    none,
    none,
    none,
    0x0038,
    0x00a9,
    none,
    0x01b7,
    0x0079,
    none,
    0x0177,
    none,
    0x00c6,
    none,
    0x00c1,
    0x0053,
    0x0060,
    0x0091,
    0x0183,
    0x01b0,
    none,
    0x0195,
    0x0099,
    none,
    0x014d,
    none,
    0x001c,
    0x00b1,
    none,
    none,
    none,
    0x0147,
    none,
    0x018c,
    none,
    0x00b9,
    none,
    0x00e1,
    0x0034,
    none,
    none,
    0x0150,
    0x00d6,
    none,
    0x00b7,
    none,
    0x01ab,
    none,
    none,
    none,
    none,
    0x0005,
    0x01b5,
    0x010c,
    0x00d9,
    none,
    none,
    0x016c,
    none,
    none,
    none,
    none,
    0x002e,
    none,
    0x0030,
    none,
    none,
    none,
    none,
    none,
    0x00a1,
    none,
    0x00de,
    0x013f,
    0x00dc,
    0x015b,
    none,
    none,
    0x00b3,
    0x0138,
    0x014a,
    0x0094,
    none,
    0x0129,
    none,
    none,
    none,
    0x00a2,
    none,
    none,
    none,
    0x0093,
    0x0022,
    none,
    0x0097,
    none,
    0x0035,
    none,
    0x0185,
    0x0001,
    0x00f7,
    none,
    0x0087,
    none,
    0x00ac,
    0x0017,
    none,
    0x0119,
    none,
    none,
    none,
    0x0179,
    none,
    none,
    0x0165,
    none,
    none,
    0x01a3,
    0x0188,
    none,
    none,
    0x0168,
    none,
    0x0197,
    none,
    none,
    none,
    none,
    none,
    none,
    0x01b3,
    none,
    0x007b,
    none,
    none,
    0x0173,
    0x00e3,
    none,
    none,
    0x008e,
    none,
    0x0083,
    0x002f,
    none,
    none,
    none,
    none,
    none,
    none,
    none,
    0x0143,
    0x005e,
    none,
    none,
    none,
    0x0062,
    none,
    none,
    0x000f,
    none,
    0x0006,
    none,
    none,
    0x016b,
    none,
    none,
    none,
    none,
    0x005f,
    none,
    0x0131,
    none,
    0x00f9,
    0x00bf,
    0x0141,
    0x017c,
    none,
    0x00e5,
    none,
    none,
    none,
    0x0123,
    none,
    0x006d,
    none,
    0x0158,
    0x00b2,
    0x0174,
    0x004e,
    none,
    none,
    0x00d4,
    0x0007,
    none,
    none,
    0x0128,
    0x0139,
    0x0014,
    none,
    none,
    none,
    none,
    none,
    none,
    none,
    none,
    none,
    none,
    none,
    none,
    none,
    0x0171,
    none,
    none,
    none,
    none,
    0x00c3,
    0x010a,
    0x018b,
    none,
    none,
    none,
    0x00cb,
    none,
    0x00e9,
    none,
    0x0012,
    none,
    none,
    none,
    none,
    none,
    0x00fc,
    none,
    none,
    0x009b,
    0x0113,
    0x0132,
    0x00db,
    0x00f3,
    none,
    0x0110,
    none,
    0x007e,
    0x0096,
    none,
    none,
    none,
    0x0002,
    0x0136,
    none,
    none,
    none,
    0x0018,
    0x0155,
    none,
    none,
    none,
    none,
    0x005d,
    none,
    none,
    0x00dd,
    0x0075,
    none,
    none,
    0x0021,
    none,
    none,
    none,
    none,
    none,
    0x0187,
    none,
    none,
    0x012e,
    none,
    none,
    0x0054,
    none,
    none,
    0x0125,
    0x007a,
    none,
    0x0109,
    none,
    none,
    none,
    0x0148,
    none,
    none,
    none,
    none,
    0x0013,
    0x012f,
    0x004c,
    none,
    0x017d,
    none,
    none,
    0x012b,
    none,
    none,
    0x0098,
    none,
    none,
    0x0134,
    0x0010,
    0x01b1,
    none,
    none,
    0x0088,
    0x0114,
    0x0175,
    none,
    0x0144,
    0x00c0,
    none,
    0x001a,
    0x0160,
    none,
    none,
    0x006e,
    none,
    none,
    0x0052,
    0x007f,
    none,
    none,
    none,
    none,
    0x0180,
    0x01ae,
    none,
    none,
    none,
    none,
    0x000b,
    0x0000,
    0x0024,
    none,
    none,
    0x0089,
    none,
    none,
    0x0008,
    none,
    none,
    none,
    none,
    none,
    0x005a,
    0x001e,
    none,
    0x01a0,
    none,
    0x0105,
    none,
    none,
    0x0159,
    0x014e,
    0x008b,
    none,
    0x0178,
    0x0019,
    0x0011,
    none,
    none,
    none,
    0x00d2,
    none,
    0x00e2,
    none,
    none,
    none,
    none,
    none,
    none,
    none,
    0x0056,
    none,
    none,
    none,
    0x0072,
    none,
    0x007d,
    none,
    none,
    0x0163,
    0x0065,
    0x018f,
    none,
    0x0100,
    0x0126,
    none,
    none,
    0x0058,
    0x00f8,
    0x0036,
    0x018d,
    0x00fd,
    0x0071,
    0x013a,
    0x001b,
    none,
    none,
    none,
    none,
    0x00cd,
    none,
    none,
    0x01a1,
    none,
    none,
    none,
    0x018a,
    0x000a,
    0x019e,
    0x015f,
    0x018e,
    none,
    0x0082,
    0x0049,
    none,
    none,
    none,
    none,
    none,
    none,
    none,
    0x0076,
    0x0033,
    0x012c,
    none,
    0x0016,
    none,
    0x01a8,
    0x01b2,
    0x01b4,
    none,
    0x0194,
    0x00ca,
    none,
    0x0061,
    0x00d5,
    none,
    0x012d,
    none,
    0x01b6,
    none,
    0x0107,
    0x0037,
    none,
    0x0032,
    0x0068,
    0x008a,
    none,
    none,
    none,
    0x00fa,
    none,
    none,
    0x00e7,
    none,
    0x000e,
    none,
    0x00c5,
    none,
    none,
    none,
    0x0055,
    none,
    0x0051,
    none,
    0x01bb,
    none,
    0x00d0,
    none,
    0x0108,
    none,
    none,
    0x00b8,
    none,
    0x00d3,
    0x017f,
    0x01a2,
    none,
    none,
    none,
    0x0086,
    0x00fb,
    none,
    none,
    none,
    none,
    none,
    none,
    none,
    0x008d,
    none,
    0x00ee,
    none,
    0x010b,
    none,
    none,
    0x010d,
    none,
    none,
    0x0095,
    none,
    0x0063,
    none,
    none,
    none,
    none,
    none,
    none,
    0x0103,
    none,
    0x0124,
    0x015a,
    0x0181,
    none,
    none,
    0x0157,
    0x0050,
    none,
    0x0151,
    none,
    none,
    none,
    0x0047,
    none,
    none,
    none,
    none,
    0x0111,
    none,
    0x013e,
    none,
    none,
    0x01ad,
    0x0164,
    none,
    0x008f,
    none,
    none,
    none,
    none,
    none,
    none,
    0x0074,
    0x00ef,
    none,
    0x0015,
    0x016e,
    none,
    0x015d,
    0x00f1,
    0x01a9,
    none,
    none,
    none,
    0x00c2,
    none,
    0x014c,
    none,
    0x01b8,
    none,
    0x00be,
    none,
    none,
    none,
    none,
    0x0196,
    0x00ce,
    0x016a,
    0x00cc,
    0x001d,
    0x0142,
    none,
    none,
    none,
    none,
    0x014f,
    none,
    none,
    none,
    none,
    none,
    0x0059,
    none,
    0x0077,
    none,
    none,
    0x00ff,
    0x0186,
    0x009d,
    none,
    none,
    none,
    none,
    0x0042,
    none,
    none,
    0x0069,
    none,
    0x00f2,
    none,
    none,
    none,
    none,
    none,
    none,
    0x006a,
    0x0190,
    none,
    none,
    none,
    0x0140,
    none,
    none,
    none,
    none,
    none,
    none,
    0x0027,
    0x0026,
    none,
    0x00c7,
    none,
    none,
    none,
    none,
    none,
    none,
    0x0064,
    none,
    0x00a4,
    0x006c,
    none,
    none,
    none,
    none,
    none,
    0x01b9,
    none,
    0x003c,
    0x0192,
    none,
    none,
    none,
    0x0066,
    0x0085,
    0x0102,
    0x01a7,
    0x0070,
    none,
    0x009a,
    none,
    none,
    none,
    0x0044,
    0x00e0,
    none,
    none,
    0x00a3,
    none,
    0x010f,
    none,
    none,
    0x00ae,
    0x0084,
    none,
    none,
    0x0025,
    0x00ba,
    0x009e,
    none,
    none,
    none,
    none,
    0x0112,
    0x00f5,
    none,
    0x0009,
    0x017e,
    0x01ac,
    none,
    0x011b,
    none,
    none,
    0x0003,
    none,
    0x0080,
    0x00da,
    0x00bb,
    0x00d8,
    none,
    none,
    none,
    0x007c,
    0x00ad,
    none,
    none,
    none,
    none,
    none,
    0x0045,
    none,
    0x005b,
    0x004f,
    none,
    0x0101,
    0x019c,
    none,
    none,
    none,
    0x0149,
    0x0166,
    0x011c,
    0x011e,
    none,
    none,
    0x0067,
    0x000c,
    0x00cf,
    none,
    none,
    0x00a6,
    none,
    0x00e6,
    0x0090,
    none,
    none,
    none,
    0x017b,
    0x0154,
    none,
    none,
    0x00bd,
    0x00e4,
    none,
    none,
    none,
    none,
    none,
    none,
    0x0153,
    0x0004,
    none,
    none,
    none,
    none,
    none,
    none,
    0x0117,
    none,
    0x0031,
    0x0127,
    none,
    none,
    0x0118,
    0x013c,
    0x019b,
    0x0145,
    none,
    0x00a5,
    none,
    none,
    none,
    0x011a,
    none,
    none,
    none,
    0x01aa,
    0x0191,
    none,
    0x014b,
    none,
    none,
    none,
    none,
    0x0028,
    0x0122,
    none,
    none,
    none,
    0x0162,
    none,
    0x0182,
    0x0057,
    0x015c,
    none,
    none,
    none,
    none,
    0x0156,
    none,
    none,
    none,
    0x00a7,
    none,
    none,
    none,
    none,
    none,
    none,
    none,
    0x0115,
    0x00b5,
    none,
    none,
    none,
    0x019f,
    0x003b,
    0x0199,
    none,
    0x003e,
    none,
    none,
    0x0116,
    none,
    0x0121,
    0x00aa,
    none,
    none,
    none,
    none,
    0x0120,
    none,
    none,
    none,
    none,
    0x00a0,
    0x01af,
    none,
    0x00c9,
    none,
    none,
    none,
    none,
    none,
    none,
    none,
    0x009f,
    none,
    none,
    0x015e,
    none,
    none,
    none,
    0x000d,
    none,
    none,
    0x006b,
    none,
    none,
    0x013d,
    none,
    none,
    0x00f4,
    0x0041,
    none,
    none,
    none,
    0x002d,
    0x0020,
    none,
    none,
    none,
    none,
    none,
    none,
    none,
    0x004a,
    0x008c,
    none,
    0x006f,
    0x0133,
    none,
    0x0023,
    0x0193,
    0x00c8,
    0x002c,
    none,
    none,
    none,
    none,
    none,
    none,
    none,
    none,
    0x0152,
    0x016f,
    0x0146,
    none,
    none,
    none,
    0x019d,
    0x0189,
    0x00df,
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
    279,
    280,
    281,
    282,
    282,
    283,
    283,
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
    293,
    294,
    295,
    296,
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
    308,
    309,
    309,
    310,
    310,
    311,
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
    321,
    321,
    322,
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
    336,
    337,
    338,
    339,
    340,
    340,
    341,
    341,
    342,
    342,
    343,
    343,
    344,
    344,
    345,
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
    372,
    373,
    373,
    374,
    375,
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
      /* CompileDeferredNV */
      if (!device || device->NV_ray_tracing) return true;
      return false;
   case 276:
      /* CreateAccelerationStructureNV */
      if (!device || device->NV_ray_tracing) return true;
      return false;
   case 277:
      /* CmdBindInvocationMaskHUAWEI */
      if (!device || device->HUAWEI_invocation_mask) return true;
      return false;
   case 278:
      /* DestroyAccelerationStructureKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 279:
      /* DestroyAccelerationStructureNV */
      if (!device || device->NV_ray_tracing) return true;
      return false;
   case 280:
      /* GetAccelerationStructureMemoryRequirementsNV */
      if (!device || device->NV_ray_tracing) return true;
      return false;
   case 281:
      /* BindAccelerationStructureMemoryNV */
      if (!device || device->NV_ray_tracing) return true;
      return false;
   case 282:
      /* CmdCopyAccelerationStructureNV */
      if (!device || device->NV_ray_tracing) return true;
      return false;
   case 283:
      /* CmdCopyAccelerationStructureKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 284:
      /* CopyAccelerationStructureKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 285:
      /* CmdCopyAccelerationStructureToMemoryKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 286:
      /* CopyAccelerationStructureToMemoryKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 287:
      /* CmdCopyMemoryToAccelerationStructureKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 288:
      /* CopyMemoryToAccelerationStructureKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 289:
      /* CmdWriteAccelerationStructuresPropertiesKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 290:
      /* CmdWriteAccelerationStructuresPropertiesNV */
      if (!device || device->NV_ray_tracing) return true;
      return false;
   case 291:
      /* CmdBuildAccelerationStructureNV */
      if (!device || device->NV_ray_tracing) return true;
      return false;
   case 292:
      /* WriteAccelerationStructuresPropertiesKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 293:
      /* CmdTraceRaysKHR */
      if (!device || device->KHR_ray_tracing_pipeline) return true;
      return false;
   case 294:
      /* CmdTraceRaysNV */
      if (!device || device->NV_ray_tracing) return true;
      return false;
   case 295:
      /* GetRayTracingShaderGroupHandlesKHR */
      if (!device || device->KHR_ray_tracing_pipeline) return true;
      return false;
   case 296:
      /* GetRayTracingShaderGroupHandlesNV */
      if (!device || device->NV_ray_tracing) return true;
      return false;
   case 297:
      /* GetRayTracingCaptureReplayShaderGroupHandlesKHR */
      if (!device || device->KHR_ray_tracing_pipeline) return true;
      return false;
   case 298:
      /* GetAccelerationStructureHandleNV */
      if (!device || device->NV_ray_tracing) return true;
      return false;
   case 299:
      /* CreateRayTracingPipelinesNV */
      if (!device || device->NV_ray_tracing) return true;
      return false;
   case 300:
      /* CreateRayTracingPipelinesKHR */
      if (!device || device->KHR_ray_tracing_pipeline) return true;
      return false;
   case 301:
      /* CmdTraceRaysIndirectKHR */
      if (!device || device->KHR_ray_tracing_pipeline) return true;
      return false;
   case 302:
      /* GetDeviceAccelerationStructureCompatibilityKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 303:
      /* GetRayTracingShaderGroupStackSizeKHR */
      if (!device || device->KHR_ray_tracing_pipeline) return true;
      return false;
   case 304:
      /* CmdSetRayTracingPipelineStackSizeKHR */
      if (!device || device->KHR_ray_tracing_pipeline) return true;
      return false;
   case 305:
      /* GetImageViewHandleNVX */
      if (!device || device->NVX_image_view_handle) return true;
      return false;
   case 306:
      /* GetImageViewAddressNVX */
      if (!device || device->NVX_image_view_handle) return true;
      return false;
   case 307:
      /* GetDeviceGroupSurfacePresentModes2EXT */
      if (!device || device->EXT_full_screen_exclusive) return true;
      if (!device || device->EXT_full_screen_exclusive) return true;
      return false;
   case 308:
      /* AcquireFullScreenExclusiveModeEXT */
      if (!device || device->EXT_full_screen_exclusive) return true;
      return false;
   case 309:
      /* ReleaseFullScreenExclusiveModeEXT */
      if (!device || device->EXT_full_screen_exclusive) return true;
      return false;
   case 310:
      /* AcquireProfilingLockKHR */
      if (!device || device->KHR_performance_query) return true;
      return false;
   case 311:
      /* ReleaseProfilingLockKHR */
      if (!device || device->KHR_performance_query) return true;
      return false;
   case 312:
      /* GetImageDrmFormatModifierPropertiesEXT */
      if (!device || device->EXT_image_drm_format_modifier) return true;
      return false;
   case 313:
      /* GetBufferOpaqueCaptureAddress */
      return VK_MAKE_VERSION(1, 2, 0) <= core_version;
   case 314:
      /* GetBufferOpaqueCaptureAddressKHR */
      if (!device || device->KHR_buffer_device_address) return true;
      return false;
   case 315:
      /* GetBufferDeviceAddress */
      return VK_MAKE_VERSION(1, 2, 0) <= core_version;
   case 316:
      /* GetBufferDeviceAddressKHR */
      if (!device || device->KHR_buffer_device_address) return true;
      return false;
   case 317:
      /* GetBufferDeviceAddressEXT */
      if (!device || device->EXT_buffer_device_address) return true;
      return false;
   case 318:
      /* InitializePerformanceApiINTEL */
      if (!device || device->INTEL_performance_query) return true;
      return false;
   case 319:
      /* UninitializePerformanceApiINTEL */
      if (!device || device->INTEL_performance_query) return true;
      return false;
   case 320:
      /* CmdSetPerformanceMarkerINTEL */
      if (!device || device->INTEL_performance_query) return true;
      return false;
   case 321:
      /* CmdSetPerformanceStreamMarkerINTEL */
      if (!device || device->INTEL_performance_query) return true;
      return false;
   case 322:
      /* CmdSetPerformanceOverrideINTEL */
      if (!device || device->INTEL_performance_query) return true;
      return false;
   case 323:
      /* AcquirePerformanceConfigurationINTEL */
      if (!device || device->INTEL_performance_query) return true;
      return false;
   case 324:
      /* ReleasePerformanceConfigurationINTEL */
      if (!device || device->INTEL_performance_query) return true;
      return false;
   case 325:
      /* QueueSetPerformanceConfigurationINTEL */
      if (!device || device->INTEL_performance_query) return true;
      return false;
   case 326:
      /* GetPerformanceParameterINTEL */
      if (!device || device->INTEL_performance_query) return true;
      return false;
   case 327:
      /* GetDeviceMemoryOpaqueCaptureAddress */
      return VK_MAKE_VERSION(1, 2, 0) <= core_version;
   case 328:
      /* GetDeviceMemoryOpaqueCaptureAddressKHR */
      if (!device || device->KHR_buffer_device_address) return true;
      return false;
   case 329:
      /* GetPipelineExecutablePropertiesKHR */
      if (!device || device->KHR_pipeline_executable_properties) return true;
      return false;
   case 330:
      /* GetPipelineExecutableStatisticsKHR */
      if (!device || device->KHR_pipeline_executable_properties) return true;
      return false;
   case 331:
      /* GetPipelineExecutableInternalRepresentationsKHR */
      if (!device || device->KHR_pipeline_executable_properties) return true;
      return false;
   case 332:
      /* CmdSetLineStippleEXT */
      if (!device || device->EXT_line_rasterization) return true;
      return false;
   case 333:
      /* CreateAccelerationStructureKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 334:
      /* CmdBuildAccelerationStructuresKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 335:
      /* CmdBuildAccelerationStructuresIndirectKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 336:
      /* BuildAccelerationStructuresKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 337:
      /* GetAccelerationStructureDeviceAddressKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 338:
      /* CreateDeferredOperationKHR */
      if (!device || device->KHR_deferred_host_operations) return true;
      return false;
   case 339:
      /* DestroyDeferredOperationKHR */
      if (!device || device->KHR_deferred_host_operations) return true;
      return false;
   case 340:
      /* GetDeferredOperationMaxConcurrencyKHR */
      if (!device || device->KHR_deferred_host_operations) return true;
      return false;
   case 341:
      /* GetDeferredOperationResultKHR */
      if (!device || device->KHR_deferred_host_operations) return true;
      return false;
   case 342:
      /* DeferredOperationJoinKHR */
      if (!device || device->KHR_deferred_host_operations) return true;
      return false;
   case 343:
      /* CmdSetCullMode */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 344:
      /* CmdSetCullModeEXT */
      if (!device || device->EXT_extended_dynamic_state) return true;
      return false;
   case 345:
      /* CmdSetFrontFace */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 346:
      /* CmdSetFrontFaceEXT */
      if (!device || device->EXT_extended_dynamic_state) return true;
      return false;
   case 347:
      /* CmdSetPrimitiveTopology */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 348:
      /* CmdSetPrimitiveTopologyEXT */
      if (!device || device->EXT_extended_dynamic_state) return true;
      return false;
   case 349:
      /* CmdSetViewportWithCount */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 350:
      /* CmdSetViewportWithCountEXT */
      if (!device || device->EXT_extended_dynamic_state) return true;
      return false;
   case 351:
      /* CmdSetScissorWithCount */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 352:
      /* CmdSetScissorWithCountEXT */
      if (!device || device->EXT_extended_dynamic_state) return true;
      return false;
   case 353:
      /* CmdBindVertexBuffers2 */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 354:
      /* CmdBindVertexBuffers2EXT */
      if (!device || device->EXT_extended_dynamic_state) return true;
      return false;
   case 355:
      /* CmdSetDepthTestEnable */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 356:
      /* CmdSetDepthTestEnableEXT */
      if (!device || device->EXT_extended_dynamic_state) return true;
      return false;
   case 357:
      /* CmdSetDepthWriteEnable */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 358:
      /* CmdSetDepthWriteEnableEXT */
      if (!device || device->EXT_extended_dynamic_state) return true;
      return false;
   case 359:
      /* CmdSetDepthCompareOp */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 360:
      /* CmdSetDepthCompareOpEXT */
      if (!device || device->EXT_extended_dynamic_state) return true;
      return false;
   case 361:
      /* CmdSetDepthBoundsTestEnable */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 362:
      /* CmdSetDepthBoundsTestEnableEXT */
      if (!device || device->EXT_extended_dynamic_state) return true;
      return false;
   case 363:
      /* CmdSetStencilTestEnable */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 364:
      /* CmdSetStencilTestEnableEXT */
      if (!device || device->EXT_extended_dynamic_state) return true;
      return false;
   case 365:
      /* CmdSetStencilOp */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 366:
      /* CmdSetStencilOpEXT */
      if (!device || device->EXT_extended_dynamic_state) return true;
      return false;
   case 367:
      /* CmdSetPatchControlPointsEXT */
      if (!device || device->EXT_extended_dynamic_state2) return true;
      return false;
   case 368:
      /* CmdSetRasterizerDiscardEnable */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 369:
      /* CmdSetRasterizerDiscardEnableEXT */
      if (!device || device->EXT_extended_dynamic_state2) return true;
      return false;
   case 370:
      /* CmdSetDepthBiasEnable */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 371:
      /* CmdSetDepthBiasEnableEXT */
      if (!device || device->EXT_extended_dynamic_state2) return true;
      return false;
   case 372:
      /* CmdSetLogicOpEXT */
      if (!device || device->EXT_extended_dynamic_state2) return true;
      return false;
   case 373:
      /* CmdSetPrimitiveRestartEnable */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 374:
      /* CmdSetPrimitiveRestartEnableEXT */
      if (!device || device->EXT_extended_dynamic_state2) return true;
      return false;
   case 375:
      /* CreatePrivateDataSlot */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 376:
      /* CreatePrivateDataSlotEXT */
      if (!device || device->EXT_private_data) return true;
      return false;
   case 377:
      /* DestroyPrivateDataSlot */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 378:
      /* DestroyPrivateDataSlotEXT */
      if (!device || device->EXT_private_data) return true;
      return false;
   case 379:
      /* SetPrivateData */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 380:
      /* SetPrivateDataEXT */
      if (!device || device->EXT_private_data) return true;
      return false;
   case 381:
      /* GetPrivateData */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 382:
      /* GetPrivateDataEXT */
      if (!device || device->EXT_private_data) return true;
      return false;
   case 383:
      /* CmdCopyBuffer2 */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 384:
      /* CmdCopyBuffer2KHR */
      if (!device || device->KHR_copy_commands2) return true;
      return false;
   case 385:
      /* CmdCopyImage2 */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 386:
      /* CmdCopyImage2KHR */
      if (!device || device->KHR_copy_commands2) return true;
      return false;
   case 387:
      /* CmdBlitImage2 */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 388:
      /* CmdBlitImage2KHR */
      if (!device || device->KHR_copy_commands2) return true;
      return false;
   case 389:
      /* CmdCopyBufferToImage2 */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 390:
      /* CmdCopyBufferToImage2KHR */
      if (!device || device->KHR_copy_commands2) return true;
      return false;
   case 391:
      /* CmdCopyImageToBuffer2 */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 392:
      /* CmdCopyImageToBuffer2KHR */
      if (!device || device->KHR_copy_commands2) return true;
      return false;
   case 393:
      /* CmdResolveImage2 */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 394:
      /* CmdResolveImage2KHR */
      if (!device || device->KHR_copy_commands2) return true;
      return false;
   case 395:
      /* CmdSetFragmentShadingRateKHR */
      if (!device || device->KHR_fragment_shading_rate) return true;
      return false;
   case 396:
      /* CmdSetFragmentShadingRateEnumNV */
      if (!device || device->NV_fragment_shading_rate_enums) return true;
      return false;
   case 397:
      /* GetAccelerationStructureBuildSizesKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 398:
      /* CmdSetVertexInputEXT */
      if (!device || device->EXT_vertex_input_dynamic_state) return true;
      return false;
   case 399:
      /* CmdSetColorWriteEnableEXT */
      if (!device || device->EXT_color_write_enable) return true;
      return false;
   case 400:
      /* CmdSetEvent2 */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 401:
      /* CmdSetEvent2KHR */
      if (!device || device->KHR_synchronization2) return true;
      return false;
   case 402:
      /* CmdResetEvent2 */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 403:
      /* CmdResetEvent2KHR */
      if (!device || device->KHR_synchronization2) return true;
      return false;
   case 404:
      /* CmdWaitEvents2 */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 405:
      /* CmdWaitEvents2KHR */
      if (!device || device->KHR_synchronization2) return true;
      return false;
   case 406:
      /* CmdPipelineBarrier2 */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 407:
      /* CmdPipelineBarrier2KHR */
      if (!device || device->KHR_synchronization2) return true;
      return false;
   case 408:
      /* QueueSubmit2 */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 409:
      /* QueueSubmit2KHR */
      if (!device || device->KHR_synchronization2) return true;
      return false;
   case 410:
      /* CmdWriteTimestamp2 */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 411:
      /* CmdWriteTimestamp2KHR */
      if (!device || device->KHR_synchronization2) return true;
      return false;
   case 412:
      /* CmdWriteBufferMarker2AMD */
      if (!device || device->KHR_synchronization2) return true;
      return false;
   case 413:
      /* GetQueueCheckpointData2NV */
      if (!device || device->KHR_synchronization2) return true;
      return false;
   case 414:
      /* CreateVideoSessionKHR */
      if (!device || device->KHR_video_queue) return true;
      return false;
   case 415:
      /* DestroyVideoSessionKHR */
      if (!device || device->KHR_video_queue) return true;
      return false;
   case 416:
      /* CreateVideoSessionParametersKHR */
      if (!device || device->KHR_video_queue) return true;
      return false;
   case 417:
      /* UpdateVideoSessionParametersKHR */
      if (!device || device->KHR_video_queue) return true;
      return false;
   case 418:
      /* DestroyVideoSessionParametersKHR */
      if (!device || device->KHR_video_queue) return true;
      return false;
   case 419:
      /* GetVideoSessionMemoryRequirementsKHR */
      if (!device || device->KHR_video_queue) return true;
      return false;
   case 420:
      /* BindVideoSessionMemoryKHR */
      if (!device || device->KHR_video_queue) return true;
      return false;
   case 421:
      /* CmdDecodeVideoKHR */
      if (!device || device->KHR_video_decode_queue) return true;
      return false;
   case 422:
      /* CmdBeginVideoCodingKHR */
      if (!device || device->KHR_video_queue) return true;
      return false;
   case 423:
      /* CmdControlVideoCodingKHR */
      if (!device || device->KHR_video_queue) return true;
      return false;
   case 424:
      /* CmdEndVideoCodingKHR */
      if (!device || device->KHR_video_queue) return true;
      return false;
   case 425:
      /* CmdEncodeVideoKHR */
      if (!device || device->KHR_video_encode_queue) return true;
      return false;
   case 426:
      /* CreateCuModuleNVX */
      if (!device || device->NVX_binary_import) return true;
      return false;
   case 427:
      /* CreateCuFunctionNVX */
      if (!device || device->NVX_binary_import) return true;
      return false;
   case 428:
      /* DestroyCuModuleNVX */
      if (!device || device->NVX_binary_import) return true;
      return false;
   case 429:
      /* DestroyCuFunctionNVX */
      if (!device || device->NVX_binary_import) return true;
      return false;
   case 430:
      /* CmdCuLaunchKernelNVX */
      if (!device || device->NVX_binary_import) return true;
      return false;
   case 431:
      /* SetDeviceMemoryPriorityEXT */
      if (!device || device->EXT_pageable_device_local_memory) return true;
      return false;
   case 432:
      /* WaitForPresentKHR */
      if (!device || device->KHR_present_wait) return true;
      return false;
   case 433:
      /* CreateBufferCollectionFUCHSIA */
      if (!device || device->FUCHSIA_buffer_collection) return true;
      return false;
   case 434:
      /* SetBufferCollectionBufferConstraintsFUCHSIA */
      if (!device || device->FUCHSIA_buffer_collection) return true;
      return false;
   case 435:
      /* SetBufferCollectionImageConstraintsFUCHSIA */
      if (!device || device->FUCHSIA_buffer_collection) return true;
      return false;
   case 436:
      /* DestroyBufferCollectionFUCHSIA */
      if (!device || device->FUCHSIA_buffer_collection) return true;
      return false;
   case 437:
      /* GetBufferCollectionPropertiesFUCHSIA */
      if (!device || device->FUCHSIA_buffer_collection) return true;
      return false;
   case 438:
      /* CmdBeginRendering */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 439:
      /* CmdBeginRenderingKHR */
      if (!device || device->KHR_dynamic_rendering) return true;
      return false;
   case 440:
      /* CmdEndRendering */
      return VK_MAKE_VERSION(1, 3, 0) <= core_version;
   case 441:
      /* CmdEndRenderingKHR */
      if (!device || device->KHR_dynamic_rendering) return true;
      return false;
   case 442:
      /* GetDescriptorSetLayoutHostMappingInfoVALVE */
      if (!device || device->VALVE_descriptor_set_host_mapping) return true;
      return false;
   case 443:
      /* GetDescriptorSetHostMappingVALVE */
      if (!device || device->VALVE_descriptor_set_host_mapping) return true;
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
            if (entry[i] == vk_entrypoint_stub)
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
            if (disp[disp_index] == NULL && entry[i] != vk_entrypoint_stub)
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
            if (entry[i] == vk_entrypoint_stub)
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
            if (disp[disp_index] == NULL && entry[i] != vk_entrypoint_stub)
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
            if (entry[i] == vk_entrypoint_stub)
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
            if (disp[disp_index] == NULL && entry[i] != vk_entrypoint_stub)
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
