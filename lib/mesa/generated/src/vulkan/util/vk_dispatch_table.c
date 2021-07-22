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

#include "vk_device.h"
#include "vk_dispatch_table.h"
#include "vk_instance.h"
#include "vk_object.h"
#include "vk_physical_device.h"

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
    table->GetPhysicalDeviceToolPropertiesEXT = (PFN_vkGetPhysicalDeviceToolPropertiesEXT) gpa(obj, "vkGetPhysicalDeviceToolPropertiesEXT");
    table->GetPhysicalDeviceFragmentShadingRatesKHR = (PFN_vkGetPhysicalDeviceFragmentShadingRatesKHR) gpa(obj, "vkGetPhysicalDeviceFragmentShadingRatesKHR");
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
    table->CmdDrawIndirect = (PFN_vkCmdDrawIndirect) gpa(obj, "vkCmdDrawIndirect");
    table->CmdDrawIndexedIndirect = (PFN_vkCmdDrawIndexedIndirect) gpa(obj, "vkCmdDrawIndexedIndirect");
    table->CmdDispatch = (PFN_vkCmdDispatch) gpa(obj, "vkCmdDispatch");
    table->CmdDispatchIndirect = (PFN_vkCmdDispatchIndirect) gpa(obj, "vkCmdDispatchIndirect");
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
#ifdef VK_USE_PLATFORM_WIN32_KHR
    table->GetSemaphoreWin32HandleKHR = (PFN_vkGetSemaphoreWin32HandleKHR) gpa(obj, "vkGetSemaphoreWin32HandleKHR");
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
    table->ImportSemaphoreWin32HandleKHR = (PFN_vkImportSemaphoreWin32HandleKHR) gpa(obj, "vkImportSemaphoreWin32HandleKHR");
#endif
    table->GetSemaphoreFdKHR = (PFN_vkGetSemaphoreFdKHR) gpa(obj, "vkGetSemaphoreFdKHR");
    table->ImportSemaphoreFdKHR = (PFN_vkImportSemaphoreFdKHR) gpa(obj, "vkImportSemaphoreFdKHR");
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
    table->CmdSetCullModeEXT = (PFN_vkCmdSetCullModeEXT) gpa(obj, "vkCmdSetCullModeEXT");
    table->CmdSetFrontFaceEXT = (PFN_vkCmdSetFrontFaceEXT) gpa(obj, "vkCmdSetFrontFaceEXT");
    table->CmdSetPrimitiveTopologyEXT = (PFN_vkCmdSetPrimitiveTopologyEXT) gpa(obj, "vkCmdSetPrimitiveTopologyEXT");
    table->CmdSetViewportWithCountEXT = (PFN_vkCmdSetViewportWithCountEXT) gpa(obj, "vkCmdSetViewportWithCountEXT");
    table->CmdSetScissorWithCountEXT = (PFN_vkCmdSetScissorWithCountEXT) gpa(obj, "vkCmdSetScissorWithCountEXT");
    table->CmdBindVertexBuffers2EXT = (PFN_vkCmdBindVertexBuffers2EXT) gpa(obj, "vkCmdBindVertexBuffers2EXT");
    table->CmdSetDepthTestEnableEXT = (PFN_vkCmdSetDepthTestEnableEXT) gpa(obj, "vkCmdSetDepthTestEnableEXT");
    table->CmdSetDepthWriteEnableEXT = (PFN_vkCmdSetDepthWriteEnableEXT) gpa(obj, "vkCmdSetDepthWriteEnableEXT");
    table->CmdSetDepthCompareOpEXT = (PFN_vkCmdSetDepthCompareOpEXT) gpa(obj, "vkCmdSetDepthCompareOpEXT");
    table->CmdSetDepthBoundsTestEnableEXT = (PFN_vkCmdSetDepthBoundsTestEnableEXT) gpa(obj, "vkCmdSetDepthBoundsTestEnableEXT");
    table->CmdSetStencilTestEnableEXT = (PFN_vkCmdSetStencilTestEnableEXT) gpa(obj, "vkCmdSetStencilTestEnableEXT");
    table->CmdSetStencilOpEXT = (PFN_vkCmdSetStencilOpEXT) gpa(obj, "vkCmdSetStencilOpEXT");
    table->CreatePrivateDataSlotEXT = (PFN_vkCreatePrivateDataSlotEXT) gpa(obj, "vkCreatePrivateDataSlotEXT");
    table->DestroyPrivateDataSlotEXT = (PFN_vkDestroyPrivateDataSlotEXT) gpa(obj, "vkDestroyPrivateDataSlotEXT");
    table->SetPrivateDataEXT = (PFN_vkSetPrivateDataEXT) gpa(obj, "vkSetPrivateDataEXT");
    table->GetPrivateDataEXT = (PFN_vkGetPrivateDataEXT) gpa(obj, "vkGetPrivateDataEXT");
    table->CmdCopyBuffer2KHR = (PFN_vkCmdCopyBuffer2KHR) gpa(obj, "vkCmdCopyBuffer2KHR");
    table->CmdCopyImage2KHR = (PFN_vkCmdCopyImage2KHR) gpa(obj, "vkCmdCopyImage2KHR");
    table->CmdBlitImage2KHR = (PFN_vkCmdBlitImage2KHR) gpa(obj, "vkCmdBlitImage2KHR");
    table->CmdCopyBufferToImage2KHR = (PFN_vkCmdCopyBufferToImage2KHR) gpa(obj, "vkCmdCopyBufferToImage2KHR");
    table->CmdCopyImageToBuffer2KHR = (PFN_vkCmdCopyImageToBuffer2KHR) gpa(obj, "vkCmdCopyImageToBuffer2KHR");
    table->CmdResolveImage2KHR = (PFN_vkCmdResolveImage2KHR) gpa(obj, "vkCmdResolveImage2KHR");
    table->CmdSetFragmentShadingRateKHR = (PFN_vkCmdSetFragmentShadingRateKHR) gpa(obj, "vkCmdSetFragmentShadingRateKHR");
    table->CmdSetFragmentShadingRateEnumNV = (PFN_vkCmdSetFragmentShadingRateEnumNV) gpa(obj, "vkCmdSetFragmentShadingRateEnumNV");
    table->GetAccelerationStructureBuildSizesKHR = (PFN_vkGetAccelerationStructureBuildSizesKHR) gpa(obj, "vkGetAccelerationStructureBuildSizesKHR");
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
    { 26, 0x987ef56, 18 }, /* vkCreateDebugReportCallbackEXT */
    { 57, 0xb674c135, 26 }, /* vkCreateDebugUtilsMessengerEXT */
    { 88, 0x2aff82da, 15 }, /* vkCreateDirectFBSurfaceEXT */
    { 115, 0x7ac4dacb, 8 }, /* vkCreateDisplayPlaneSurfaceKHR */
    { 146, 0x475d2386, 29 }, /* vkCreateHeadlessSurfaceEXT */
    { 173, 0x12f99669, 23 }, /* vkCreateIOSSurfaceMVK */
    { 195, 0xdee614a2, 16 }, /* vkCreateImagePipeSurfaceFUCHSIA */
    { 227, 0x38a581a6, 0 }, /* vkCreateInstance */
    { 244, 0x30feffa7, 24 }, /* vkCreateMacOSSurfaceMVK */
    { 268, 0xa436224a, 25 }, /* vkCreateMetalSurfaceEXT */
    { 292, 0x14361e93, 17 }, /* vkCreateStreamDescriptorSurfaceGGP */
    { 327, 0xba0e6b39, 10 }, /* vkCreateViSurfaceNN */
    { 347, 0x2b2a4b79, 11 }, /* vkCreateWaylandSurfaceKHR */
    { 373, 0xfa2ba1e2, 12 }, /* vkCreateWin32SurfaceKHR */
    { 397, 0xc5e5b106, 14 }, /* vkCreateXcbSurfaceKHR */
    { 419, 0xa693bc66, 13 }, /* vkCreateXlibSurfaceKHR */
    { 442, 0xa4e75334, 20 }, /* vkDebugReportMessageEXT */
    { 466, 0x43d4c4e2, 19 }, /* vkDestroyDebugReportCallbackEXT */
    { 498, 0xf0c196c1, 27 }, /* vkDestroyDebugUtilsMessengerEXT */
    { 530, 0x9bd21af2, 1 }, /* vkDestroyInstance */
    { 548, 0xf204ce7d, 9 }, /* vkDestroySurfaceKHR */
    { 568, 0xeb27627e, 6 }, /* vkEnumerateInstanceExtensionProperties */
    { 607, 0x81f69d8, 5 }, /* vkEnumerateInstanceLayerProperties */
    { 642, 0xd0481e5c, 4 }, /* vkEnumerateInstanceVersion */
    { 669, 0x270514f0, 21 }, /* vkEnumeratePhysicalDeviceGroups */
    { 701, 0x549ce595, 22 }, /* vkEnumeratePhysicalDeviceGroupsKHR */
    { 736, 0x5787c327, 2 }, /* vkEnumeratePhysicalDevices */
    { 763, 0x3d2ae9ad, 3 }, /* vkGetInstanceProcAddr */
    { 785, 0x92943f0d, 28 }, /* vkSubmitDebugUtilsMessageEXT */
};

/* Hash table stats:
 * size 30 entries
 * collisions entries:
 *     0      24
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
    0x0013,
    none,
    none,
    none,
    none,
    0x0005,
    none,
    0x000e,
    none,
    0x000a,
    0x0004,
    0x000d,
    0x001d,
    0x0000,
    none,
    none,
    none,
    none,
    0x000b,
    none,
    0x001a,
    0x0001,
    none,
    0x0017,
    0x000f,
    0x0003,
    0x0012,
    0x0018,
    none,
    none,
    0x0010,
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
    0x001c,
    none,
    none,
    0x0019,
    none,
    0x0014,
    none,
    0x0011,
    0x0002,
    none,
    none,
    none,
    0x000c,
    0x001b,
    none,
    none,
    0x0015,
    0x0016,
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
    "vkGetPhysicalDeviceToolPropertiesEXT\0"
    "vkGetPhysicalDeviceWaylandPresentationSupportKHR\0"
    "vkGetPhysicalDeviceWin32PresentationSupportKHR\0"
    "vkGetPhysicalDeviceXcbPresentationSupportKHR\0"
    "vkGetPhysicalDeviceXlibPresentationSupportKHR\0"
    "vkGetRandROutputDisplayEXT\0"
    "vkGetWinrtDisplayNV\0"
    "vkReleaseDisplayEXT\0"
;

static const struct string_map_entry physical_device_string_map_entries[] = {
    { 0, 0xdfb5cfcd, 49 }, /* vkAcquireWinrtDisplayNV */
    { 24, 0x60df100d, 47 }, /* vkAcquireXlibDisplayEXT */
    { 48, 0x85ed23f, 6 }, /* vkCreateDevice */
    { 63, 0xcc0bde41, 14 }, /* vkCreateDisplayModeKHR */
    { 86, 0x5fd13eed, 8 }, /* vkEnumerateDeviceExtensionProperties */
    { 123, 0x2f8566e7, 7 }, /* vkEnumerateDeviceLayerProperties */
    { 156, 0x8d3d4995, 63 }, /* vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR */
    { 220, 0x3e613e42, 58 }, /* vkGetDisplayModeProperties2KHR */
    { 251, 0x36b8a8de, 13 }, /* vkGetDisplayModePropertiesKHR */
    { 281, 0xff1655a4, 59 }, /* vkGetDisplayPlaneCapabilities2KHR */
    { 315, 0x4b60d48c, 15 }, /* vkGetDisplayPlaneCapabilitiesKHR */
    { 348, 0xabef4889, 12 }, /* vkGetDisplayPlaneSupportedDisplaysKHR */
    { 386, 0xea07da1a, 60 }, /* vkGetPhysicalDeviceCalibrateableTimeDomainsEXT */
    { 433, 0x3c5ac109, 61 }, /* vkGetPhysicalDeviceCooperativeMatrixPropertiesNV */
    { 482, 0x8d00453f, 24 }, /* vkGetPhysicalDeviceDirectFBPresentationSupportEXT */
    { 532, 0xb7bc4386, 57 }, /* vkGetPhysicalDeviceDisplayPlaneProperties2KHR */
    { 578, 0xb9b8ddba, 11 }, /* vkGetPhysicalDeviceDisplayPlanePropertiesKHR */
    { 623, 0x540c0372, 56 }, /* vkGetPhysicalDeviceDisplayProperties2KHR */
    { 664, 0xfa0cd2e, 10 }, /* vkGetPhysicalDeviceDisplayPropertiesKHR */
    { 704, 0x944476dc, 40 }, /* vkGetPhysicalDeviceExternalBufferProperties */
    { 748, 0xee68b389, 41 }, /* vkGetPhysicalDeviceExternalBufferPropertiesKHR */
    { 795, 0x3bc965eb, 44 }, /* vkGetPhysicalDeviceExternalFenceProperties */
    { 838, 0x99b35492, 45 }, /* vkGetPhysicalDeviceExternalFencePropertiesKHR */
    { 884, 0xc8420c4c, 25 }, /* vkGetPhysicalDeviceExternalImageFormatPropertiesNV */
    { 935, 0xcf251b0e, 42 }, /* vkGetPhysicalDeviceExternalSemaphoreProperties */
    { 982, 0x984c3fa7, 43 }, /* vkGetPhysicalDeviceExternalSemaphorePropertiesKHR */
    { 1032, 0x113e2f33, 3 }, /* vkGetPhysicalDeviceFeatures */
    { 1060, 0x63c068a7, 26 }, /* vkGetPhysicalDeviceFeatures2 */
    { 1089, 0x6a9a3636, 27 }, /* vkGetPhysicalDeviceFeatures2KHR */
    { 1121, 0x3e54b398, 4 }, /* vkGetPhysicalDeviceFormatProperties */
    { 1157, 0xca3bb9da, 30 }, /* vkGetPhysicalDeviceFormatProperties2 */
    { 1194, 0x9099cbbb, 31 }, /* vkGetPhysicalDeviceFormatProperties2KHR */
    { 1234, 0x6f0a9ed6, 67 }, /* vkGetPhysicalDeviceFragmentShadingRatesKHR */
    { 1277, 0xdd36a867, 5 }, /* vkGetPhysicalDeviceImageFormatProperties */
    { 1318, 0x35d260d3, 32 }, /* vkGetPhysicalDeviceImageFormatProperties2 */
    { 1360, 0x102ff7ea, 33 }, /* vkGetPhysicalDeviceImageFormatProperties2KHR */
    { 1405, 0xa90da4da, 2 }, /* vkGetPhysicalDeviceMemoryProperties */
    { 1441, 0xcb4cc208, 36 }, /* vkGetPhysicalDeviceMemoryProperties2 */
    { 1478, 0xc8c3da3d, 37 }, /* vkGetPhysicalDeviceMemoryProperties2KHR */
    { 1518, 0x219aa0b9, 53 }, /* vkGetPhysicalDeviceMultisamplePropertiesEXT */
    { 1562, 0x100341b4, 52 }, /* vkGetPhysicalDevicePresentRectanglesKHR */
    { 1602, 0x52fe22c9, 0 }, /* vkGetPhysicalDeviceProperties */
    { 1632, 0x6c4d8ee1, 28 }, /* vkGetPhysicalDeviceProperties2 */
    { 1663, 0xcd15838c, 29 }, /* vkGetPhysicalDeviceProperties2KHR */
    { 1697, 0x7c7c9a0f, 64 }, /* vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR */
    { 1753, 0x4e5fc88a, 1 }, /* vkGetPhysicalDeviceQueueFamilyProperties */
    { 1794, 0xcad374d8, 34 }, /* vkGetPhysicalDeviceQueueFamilyProperties2 */
    { 1836, 0x5ceb2bed, 35 }, /* vkGetPhysicalDeviceQueueFamilyProperties2KHR */
    { 1881, 0x272ef8ef, 9 }, /* vkGetPhysicalDeviceSparseImageFormatProperties */
    { 1928, 0xebddba0b, 38 }, /* vkGetPhysicalDeviceSparseImageFormatProperties2 */
    { 1976, 0x8746ed72, 39 }, /* vkGetPhysicalDeviceSparseImageFormatProperties2KHR */
    { 2027, 0x432ca8f7, 65 }, /* vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV */
    { 2093, 0x5a5fba04, 51 }, /* vkGetPhysicalDeviceSurfaceCapabilities2EXT */
    { 2136, 0x9497e378, 54 }, /* vkGetPhysicalDeviceSurfaceCapabilities2KHR */
    { 2179, 0x77890558, 17 }, /* vkGetPhysicalDeviceSurfaceCapabilitiesKHR */
    { 2221, 0xd00b7188, 55 }, /* vkGetPhysicalDeviceSurfaceFormats2KHR */
    { 2259, 0xe32227c8, 18 }, /* vkGetPhysicalDeviceSurfaceFormatsKHR */
    { 2296, 0x33557b83, 62 }, /* vkGetPhysicalDeviceSurfacePresentModes2EXT */
    { 2339, 0x31c3cbd1, 19 }, /* vkGetPhysicalDeviceSurfacePresentModesKHR */
    { 2381, 0x1a687885, 16 }, /* vkGetPhysicalDeviceSurfaceSupportKHR */
    { 2418, 0xd1685100, 66 }, /* vkGetPhysicalDeviceToolPropertiesEXT */
    { 2455, 0x84e085ac, 20 }, /* vkGetPhysicalDeviceWaylandPresentationSupportKHR */
    { 2504, 0x80e72505, 21 }, /* vkGetPhysicalDeviceWin32PresentationSupportKHR */
    { 2551, 0x41782cb9, 23 }, /* vkGetPhysicalDeviceXcbPresentationSupportKHR */
    { 2596, 0x34a063ab, 22 }, /* vkGetPhysicalDeviceXlibPresentationSupportKHR */
    { 2642, 0xb87cdd6c, 48 }, /* vkGetRandROutputDisplayEXT */
    { 2669, 0x613996b5, 50 }, /* vkGetWinrtDisplayNV */
    { 2689, 0x4207f4f1, 46 }, /* vkReleaseDisplayEXT */
};

/* Hash table stats:
 * size 68 entries
 * collisions entries:
 *     0      52
 *     1      6
 *     2      7
 *     3      3
 *     4      0
 *     5      0
 *     6      0
 *     7      0
 *     8      0
 *     9+     0
 */

#define none 0xffff
static const uint16_t physical_device_string_map[128] = {
    0x0024,
    none,
    none,
    0x0039,
    0x0034,
    0x0032,
    0x000f,
    none,
    0x0025,
    0x000b,
    0x002d,
    0x0031,
    0x000a,
    0x0001,
    0x0018,
    0x002c,
    none,
    none,
    0x0016,
    0x002f,
    none,
    0x0006,
    none,
    none,
    0x001d,
    none,
    0x000c,
    0x0037,
    0x000d,
    none,
    none,
    0x002b,
    none,
    none,
    none,
    none,
    0x0009,
    none,
    0x003c,
    0x0019,
    none,
    none,
    none,
    0x003b,
    0x003d,
    none,
    0x0012,
    0x0014,
    none,
    none,
    none,
    0x001a,
    0x0028,
    0x0042,
    0x001c,
    none,
    none,
    0x0027,
    0x0010,
    0x001f,
    none,
    0x0026,
    0x003e,
    0x0002,
    none,
    0x0003,
    0x0007,
    none,
    none,
    none,
    none,
    none,
    0x0038,
    0x0029,
    none,
    none,
    0x0017,
    0x0000,
    none,
    none,
    none,
    0x003a,
    0x000e,
    0x0022,
    none,
    none,
    0x0020,
    none,
    0x002e,
    none,
    0x001e,
    none,
    0x0013,
    none,
    0x0008,
    0x003f,
    0x001b,
    0x002a,
    none,
    none,
    0x0040,
    none,
    none,
    0x0005,
    none,
    none,
    0x0023,
    0x0015,
    0x0041,
    0x0004,
    none,
    0x0030,
    none,
    0x0043,
    0x0011,
    none,
    none,
    none,
    none,
    0x0033,
    0x0035,
    none,
    0x0021,
    none,
    none,
    none,
    0x0036,
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
    "vkBuildAccelerationStructuresKHR\0"
    "vkCmdBeginConditionalRenderingEXT\0"
    "vkCmdBeginDebugUtilsLabelEXT\0"
    "vkCmdBeginQuery\0"
    "vkCmdBeginQueryIndexedEXT\0"
    "vkCmdBeginRenderPass\0"
    "vkCmdBeginRenderPass2\0"
    "vkCmdBeginRenderPass2KHR\0"
    "vkCmdBeginTransformFeedbackEXT\0"
    "vkCmdBindDescriptorSets\0"
    "vkCmdBindIndexBuffer\0"
    "vkCmdBindPipeline\0"
    "vkCmdBindPipelineShaderGroupNV\0"
    "vkCmdBindShadingRateImageNV\0"
    "vkCmdBindTransformFeedbackBuffersEXT\0"
    "vkCmdBindVertexBuffers\0"
    "vkCmdBindVertexBuffers2EXT\0"
    "vkCmdBlitImage\0"
    "vkCmdBlitImage2KHR\0"
    "vkCmdBuildAccelerationStructureNV\0"
    "vkCmdBuildAccelerationStructuresIndirectKHR\0"
    "vkCmdBuildAccelerationStructuresKHR\0"
    "vkCmdClearAttachments\0"
    "vkCmdClearColorImage\0"
    "vkCmdClearDepthStencilImage\0"
    "vkCmdCopyAccelerationStructureKHR\0"
    "vkCmdCopyAccelerationStructureNV\0"
    "vkCmdCopyAccelerationStructureToMemoryKHR\0"
    "vkCmdCopyBuffer\0"
    "vkCmdCopyBuffer2KHR\0"
    "vkCmdCopyBufferToImage\0"
    "vkCmdCopyBufferToImage2KHR\0"
    "vkCmdCopyImage\0"
    "vkCmdCopyImage2KHR\0"
    "vkCmdCopyImageToBuffer\0"
    "vkCmdCopyImageToBuffer2KHR\0"
    "vkCmdCopyMemoryToAccelerationStructureKHR\0"
    "vkCmdCopyQueryPoolResults\0"
    "vkCmdDebugMarkerBeginEXT\0"
    "vkCmdDebugMarkerEndEXT\0"
    "vkCmdDebugMarkerInsertEXT\0"
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
    "vkCmdEndConditionalRenderingEXT\0"
    "vkCmdEndDebugUtilsLabelEXT\0"
    "vkCmdEndQuery\0"
    "vkCmdEndQueryIndexedEXT\0"
    "vkCmdEndRenderPass\0"
    "vkCmdEndRenderPass2\0"
    "vkCmdEndRenderPass2KHR\0"
    "vkCmdEndTransformFeedbackEXT\0"
    "vkCmdExecuteCommands\0"
    "vkCmdExecuteGeneratedCommandsNV\0"
    "vkCmdFillBuffer\0"
    "vkCmdInsertDebugUtilsLabelEXT\0"
    "vkCmdNextSubpass\0"
    "vkCmdNextSubpass2\0"
    "vkCmdNextSubpass2KHR\0"
    "vkCmdPipelineBarrier\0"
    "vkCmdPreprocessGeneratedCommandsNV\0"
    "vkCmdPushConstants\0"
    "vkCmdPushDescriptorSetKHR\0"
    "vkCmdPushDescriptorSetWithTemplateKHR\0"
    "vkCmdResetEvent\0"
    "vkCmdResetQueryPool\0"
    "vkCmdResolveImage\0"
    "vkCmdResolveImage2KHR\0"
    "vkCmdSetBlendConstants\0"
    "vkCmdSetCheckpointNV\0"
    "vkCmdSetCoarseSampleOrderNV\0"
    "vkCmdSetCullModeEXT\0"
    "vkCmdSetDepthBias\0"
    "vkCmdSetDepthBounds\0"
    "vkCmdSetDepthBoundsTestEnableEXT\0"
    "vkCmdSetDepthCompareOpEXT\0"
    "vkCmdSetDepthTestEnableEXT\0"
    "vkCmdSetDepthWriteEnableEXT\0"
    "vkCmdSetDeviceMask\0"
    "vkCmdSetDeviceMaskKHR\0"
    "vkCmdSetDiscardRectangleEXT\0"
    "vkCmdSetEvent\0"
    "vkCmdSetExclusiveScissorNV\0"
    "vkCmdSetFragmentShadingRateEnumNV\0"
    "vkCmdSetFragmentShadingRateKHR\0"
    "vkCmdSetFrontFaceEXT\0"
    "vkCmdSetLineStippleEXT\0"
    "vkCmdSetLineWidth\0"
    "vkCmdSetPerformanceMarkerINTEL\0"
    "vkCmdSetPerformanceOverrideINTEL\0"
    "vkCmdSetPerformanceStreamMarkerINTEL\0"
    "vkCmdSetPrimitiveTopologyEXT\0"
    "vkCmdSetRayTracingPipelineStackSizeKHR\0"
    "vkCmdSetSampleLocationsEXT\0"
    "vkCmdSetScissor\0"
    "vkCmdSetScissorWithCountEXT\0"
    "vkCmdSetStencilCompareMask\0"
    "vkCmdSetStencilOpEXT\0"
    "vkCmdSetStencilReference\0"
    "vkCmdSetStencilTestEnableEXT\0"
    "vkCmdSetStencilWriteMask\0"
    "vkCmdSetViewport\0"
    "vkCmdSetViewportShadingRatePaletteNV\0"
    "vkCmdSetViewportWScalingNV\0"
    "vkCmdSetViewportWithCountEXT\0"
    "vkCmdTraceRaysIndirectKHR\0"
    "vkCmdTraceRaysKHR\0"
    "vkCmdTraceRaysNV\0"
    "vkCmdUpdateBuffer\0"
    "vkCmdWaitEvents\0"
    "vkCmdWriteAccelerationStructuresPropertiesKHR\0"
    "vkCmdWriteAccelerationStructuresPropertiesNV\0"
    "vkCmdWriteBufferMarkerAMD\0"
    "vkCmdWriteTimestamp\0"
    "vkCompileDeferredNV\0"
    "vkCopyAccelerationStructureKHR\0"
    "vkCopyAccelerationStructureToMemoryKHR\0"
    "vkCopyMemoryToAccelerationStructureKHR\0"
    "vkCreateAccelerationStructureKHR\0"
    "vkCreateAccelerationStructureNV\0"
    "vkCreateBuffer\0"
    "vkCreateBufferView\0"
    "vkCreateCommandPool\0"
    "vkCreateComputePipelines\0"
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
    "vkDebugMarkerSetObjectNameEXT\0"
    "vkDebugMarkerSetObjectTagEXT\0"
    "vkDeferredOperationJoinKHR\0"
    "vkDestroyAccelerationStructureKHR\0"
    "vkDestroyAccelerationStructureNV\0"
    "vkDestroyBuffer\0"
    "vkDestroyBufferView\0"
    "vkDestroyCommandPool\0"
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
    "vkGetDescriptorSetLayoutSupport\0"
    "vkGetDescriptorSetLayoutSupportKHR\0"
    "vkGetDeviceAccelerationStructureCompatibilityKHR\0"
    "vkGetDeviceGroupPeerMemoryFeatures\0"
    "vkGetDeviceGroupPeerMemoryFeaturesKHR\0"
    "vkGetDeviceGroupPresentCapabilitiesKHR\0"
    "vkGetDeviceGroupSurfacePresentModes2EXT\0"
    "vkGetDeviceGroupSurfacePresentModesKHR\0"
    "vkGetDeviceMemoryCommitment\0"
    "vkGetDeviceMemoryOpaqueCaptureAddress\0"
    "vkGetDeviceMemoryOpaqueCaptureAddressKHR\0"
    "vkGetDeviceProcAddr\0"
    "vkGetDeviceQueue\0"
    "vkGetDeviceQueue2\0"
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
    "vkGetMemoryWin32HandleKHR\0"
    "vkGetMemoryWin32HandleNV\0"
    "vkGetMemoryWin32HandlePropertiesKHR\0"
    "vkGetPastPresentationTimingGOOGLE\0"
    "vkGetPerformanceParameterINTEL\0"
    "vkGetPipelineCacheData\0"
    "vkGetPipelineExecutableInternalRepresentationsKHR\0"
    "vkGetPipelineExecutablePropertiesKHR\0"
    "vkGetPipelineExecutableStatisticsKHR\0"
    "vkGetPrivateDataEXT\0"
    "vkGetQueryPoolResults\0"
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
    "vkGetShaderInfoAMD\0"
    "vkGetSwapchainCounterEXT\0"
    "vkGetSwapchainGrallocUsage2ANDROID\0"
    "vkGetSwapchainGrallocUsageANDROID\0"
    "vkGetSwapchainImagesKHR\0"
    "vkGetSwapchainStatusKHR\0"
    "vkGetValidationCacheDataEXT\0"
    "vkImportFenceFdKHR\0"
    "vkImportFenceWin32HandleKHR\0"
    "vkImportSemaphoreFdKHR\0"
    "vkImportSemaphoreWin32HandleKHR\0"
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
    "vkSetDebugUtilsObjectNameEXT\0"
    "vkSetDebugUtilsObjectTagEXT\0"
    "vkSetEvent\0"
    "vkSetHdrMetadataEXT\0"
    "vkSetLocalDimmingAMD\0"
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
    "vkWaitForFences\0"
    "vkWaitSemaphores\0"
    "vkWaitSemaphoresKHR\0"
    "vkWriteAccelerationStructuresPropertiesKHR\0"
;

static const struct string_map_entry device_string_map_entries[] = {
    { 0, 0x8a43a1cc, 292 }, /* vkAcquireFullScreenExclusiveModeEXT */
    { 36, 0x6bf780dd, 208 }, /* vkAcquireImageANDROID */
    { 58, 0x82860572, 172 }, /* vkAcquireNextImage2KHR */
    { 81, 0xc3fedb2e, 129 }, /* vkAcquireNextImageKHR */
    { 103, 0x33d2767, 307 }, /* vkAcquirePerformanceConfigurationINTEL */
    { 142, 0xaf1d64ad, 294 }, /* vkAcquireProfilingLockKHR */
    { 168, 0x8c0c811a, 74 }, /* vkAllocateCommandBuffers */
    { 193, 0x4c449d3a, 63 }, /* vkAllocateDescriptorSets */
    { 218, 0x522b85d3, 6 }, /* vkAllocateMemory */
    { 235, 0xc54f7327, 76 }, /* vkBeginCommandBuffer */
    { 256, 0x3ec4e21a, 265 }, /* vkBindAccelerationStructureMemoryNV */
    { 292, 0x6bcbdcb, 14 }, /* vkBindBufferMemory */
    { 311, 0xc27aaf4f, 164 }, /* vkBindBufferMemory2 */
    { 331, 0x6878d3ce, 165 }, /* vkBindBufferMemory2KHR */
    { 354, 0x5caaae4a, 16 }, /* vkBindImageMemory */
    { 372, 0xa9097118, 166 }, /* vkBindImageMemory2 */
    { 391, 0xf18729ad, 167 }, /* vkBindImageMemory2KHR */
    { 413, 0xf7d6c55c, 320 }, /* vkBuildAccelerationStructuresKHR */
    { 446, 0xe561c19f, 115 }, /* vkCmdBeginConditionalRenderingEXT */
    { 480, 0x6184193f, 218 }, /* vkCmdBeginDebugUtilsLabelEXT */
    { 509, 0xf5064ea4, 113 }, /* vkCmdBeginQuery */
    { 525, 0x73251a2c, 250 }, /* vkCmdBeginQueryIndexedEXT */
    { 551, 0xcb7a58e3, 121 }, /* vkCmdBeginRenderPass */
    { 572, 0x9c876577, 225 }, /* vkCmdBeginRenderPass2 */
    { 594, 0x8b6b4de6, 226 }, /* vkCmdBeginRenderPass2KHR */
    { 619, 0xb217c94, 248 }, /* vkCmdBeginTransformFeedbackEXT */
    { 650, 0x28c7a5da, 89 }, /* vkCmdBindDescriptorSets */
    { 674, 0x4c22d870, 90 }, /* vkCmdBindIndexBuffer */
    { 695, 0x3af9fd84, 79 }, /* vkCmdBindPipeline */
    { 713, 0x353570d6, 139 }, /* vkCmdBindPipelineShaderGroupNV */
    { 744, 0xbae753eb, 254 }, /* vkCmdBindShadingRateImageNV */
    { 772, 0x98fdb5cd, 247 }, /* vkCmdBindTransformFeedbackBuffersEXT */
    { 809, 0xa9c83f1d, 91 }, /* vkCmdBindVertexBuffers */
    { 832, 0x30a5f2ec, 332 }, /* vkCmdBindVertexBuffers2EXT */
    { 859, 0x331ebf89, 100 }, /* vkCmdBlitImage */
    { 874, 0x785f984c, 345 }, /* vkCmdBlitImage2KHR */
    { 893, 0x8e9d180a, 275 }, /* vkCmdBuildAccelerationStructureNV */
    { 927, 0xb02d6bee, 319 }, /* vkCmdBuildAccelerationStructuresIndirectKHR */
    { 971, 0x25a727dc, 318 }, /* vkCmdBuildAccelerationStructuresKHR */
    { 1007, 0x93cb5cb8, 107 }, /* vkCmdClearAttachments */
    { 1029, 0xb4bc8d08, 105 }, /* vkCmdClearColorImage */
    { 1050, 0x4f88e4ba, 106 }, /* vkCmdClearDepthStencilImage */
    { 1078, 0xe2d4fe2c, 267 }, /* vkCmdCopyAccelerationStructureKHR */
    { 1112, 0x84ab5629, 266 }, /* vkCmdCopyAccelerationStructureNV */
    { 1145, 0x46b2a8a0, 269 }, /* vkCmdCopyAccelerationStructureToMemoryKHR */
    { 1187, 0xc939a0da, 98 }, /* vkCmdCopyBuffer */
    { 1203, 0x90c5563d, 343 }, /* vkCmdCopyBuffer2KHR */
    { 1223, 0x929847e, 101 }, /* vkCmdCopyBufferToImage */
    { 1246, 0x1e9f6861, 346 }, /* vkCmdCopyBufferToImage2KHR */
    { 1273, 0x278effa9, 99 }, /* vkCmdCopyImage */
    { 1288, 0xdad52c6c, 344 }, /* vkCmdCopyImage2KHR */
    { 1307, 0x68cddbac, 102 }, /* vkCmdCopyImageToBuffer */
    { 1330, 0x2db6484f, 347 }, /* vkCmdCopyImageToBuffer2KHR */
    { 1357, 0xa76c5fd8, 271 }, /* vkCmdCopyMemoryToAccelerationStructureKHR */
    { 1399, 0xdee8c6d4, 119 }, /* vkCmdCopyQueryPoolResults */
    { 1425, 0xaec8bb10, 133 }, /* vkCmdDebugMarkerBeginEXT */
    { 1450, 0xd6a1433e, 134 }, /* vkCmdDebugMarkerEndEXT */
    { 1473, 0x173d440, 135 }, /* vkCmdDebugMarkerInsertEXT */
    { 1499, 0xbd58e867, 96 }, /* vkCmdDispatch */
    { 1513, 0xfb767220, 173 }, /* vkCmdDispatchBase */
    { 1531, 0x402403e5, 174 }, /* vkCmdDispatchBaseKHR */
    { 1552, 0xd6353005, 97 }, /* vkCmdDispatchIndirect */
    { 1574, 0x9912c1a1, 92 }, /* vkCmdDraw */
    { 1584, 0xbe5a8058, 93 }, /* vkCmdDrawIndexed */
    { 1601, 0x94e7ed36, 95 }, /* vkCmdDrawIndexedIndirect */
    { 1626, 0xb4acef41, 242 }, /* vkCmdDrawIndexedIndirectCount */
    { 1656, 0xc86e9287, 244 }, /* vkCmdDrawIndexedIndirectCountAMD */
    { 1689, 0xda9e8a2c, 243 }, /* vkCmdDrawIndexedIndirectCountKHR */
    { 1722, 0xe9ac41bf, 94 }, /* vkCmdDrawIndirect */
    { 1740, 0x80c3b089, 252 }, /* vkCmdDrawIndirectByteCountEXT */
    { 1770, 0x40079990, 239 }, /* vkCmdDrawIndirectCount */
    { 1793, 0xe5ad0a50, 241 }, /* vkCmdDrawIndirectCountAMD */
    { 1819, 0xf7dd01f5, 240 }, /* vkCmdDrawIndirectCountKHR */
    { 1845, 0xf21f391, 259 }, /* vkCmdDrawMeshTasksIndirectCountNV */
    { 1879, 0xaecd0a06, 258 }, /* vkCmdDrawMeshTasksIndirectNV */
    { 1908, 0xfba21ac8, 257 }, /* vkCmdDrawMeshTasksNV */
    { 1929, 0x18c8217d, 116 }, /* vkCmdEndConditionalRenderingEXT */
    { 1961, 0x29875911, 219 }, /* vkCmdEndDebugUtilsLabelEXT */
    { 1988, 0xd556fd22, 114 }, /* vkCmdEndQuery */
    { 2002, 0xd5c2f48a, 251 }, /* vkCmdEndQueryIndexedEXT */
    { 2026, 0xdcdb0235, 123 }, /* vkCmdEndRenderPass */
    { 2045, 0x1cbf9115, 229 }, /* vkCmdEndRenderPass2 */
    { 2065, 0x57eebe78, 230 }, /* vkCmdEndRenderPass2KHR */
    { 2088, 0xf008d706, 249 }, /* vkCmdEndTransformFeedbackEXT */
    { 2117, 0x9eaabe40, 124 }, /* vkCmdExecuteCommands */
    { 2138, 0xe02372d7, 137 }, /* vkCmdExecuteGeneratedCommandsNV */
    { 2170, 0x5bdd2ae0, 104 }, /* vkCmdFillBuffer */
    { 2186, 0xce6aa7d1, 220 }, /* vkCmdInsertDebugUtilsLabelEXT */
    { 2216, 0x2eeec2f9, 122 }, /* vkCmdNextSubpass */
    { 2233, 0xd4fc131, 227 }, /* vkCmdNextSubpass2 */
    { 2251, 0x25b621bc, 228 }, /* vkCmdNextSubpass2KHR */
    { 2272, 0x97fccfe8, 112 }, /* vkCmdPipelineBarrier */
    { 2293, 0x26eff1e6, 138 }, /* vkCmdPreprocessGeneratedCommandsNV */
    { 2328, 0xb1c6b468, 120 }, /* vkCmdPushConstants */
    { 2347, 0xf17232a1, 143 }, /* vkCmdPushDescriptorSetKHR */
    { 2373, 0x3d528981, 181 }, /* vkCmdPushDescriptorSetWithTemplateKHR */
    { 2411, 0x4fccce28, 110 }, /* vkCmdResetEvent */
    { 2427, 0x2f614082, 117 }, /* vkCmdResetQueryPool */
    { 2447, 0x671bb594, 108 }, /* vkCmdResolveImage */
    { 2465, 0x9fea6337, 348 }, /* vkCmdResolveImage2KHR */
    { 2487, 0x1c989dfb, 84 }, /* vkCmdSetBlendConstants */
    { 2510, 0x4331556d, 245 }, /* vkCmdSetCheckpointNV */
    { 2531, 0xcd76e1c0, 256 }, /* vkCmdSetCoarseSampleOrderNV */
    { 2559, 0xb7fcea1f, 327 }, /* vkCmdSetCullModeEXT */
    { 2579, 0x30f14d07, 83 }, /* vkCmdSetDepthBias */
    { 2597, 0x7b3a8a63, 85 }, /* vkCmdSetDepthBounds */
    { 2617, 0x3f2ddb1, 336 }, /* vkCmdSetDepthBoundsTestEnableEXT */
    { 2650, 0x2f377e41, 335 }, /* vkCmdSetDepthCompareOpEXT */
    { 2676, 0x57c5efe6, 333 }, /* vkCmdSetDepthTestEnableEXT */
    { 2703, 0xbe217905, 334 }, /* vkCmdSetDepthWriteEnableEXT */
    { 2731, 0xaecdae87, 168 }, /* vkCmdSetDeviceMask */
    { 2750, 0xfbb79356, 169 }, /* vkCmdSetDeviceMaskKHR */
    { 2772, 0x64df188b, 187 }, /* vkCmdSetDiscardRectangleEXT */
    { 2800, 0xe257f075, 109 }, /* vkCmdSetEvent */
    { 2814, 0xb2537e63, 253 }, /* vkCmdSetExclusiveScissorNV */
    { 2841, 0x7670296e, 350 }, /* vkCmdSetFragmentShadingRateEnumNV */
    { 2875, 0x4c696cd8, 349 }, /* vkCmdSetFragmentShadingRateKHR */
    { 2906, 0xa7a7a090, 328 }, /* vkCmdSetFrontFaceEXT */
    { 2927, 0xbdaa62f9, 316 }, /* vkCmdSetLineStippleEXT */
    { 2950, 0x32282165, 82 }, /* vkCmdSetLineWidth */
    { 2968, 0x4eb21af9, 304 }, /* vkCmdSetPerformanceMarkerINTEL */
    { 2999, 0x30d793c7, 306 }, /* vkCmdSetPerformanceOverrideINTEL */
    { 3032, 0xc50b03a9, 305 }, /* vkCmdSetPerformanceStreamMarkerINTEL */
    { 3069, 0x1dacaf8, 329 }, /* vkCmdSetPrimitiveTopologyEXT */
    { 3098, 0xd056ef9b, 288 }, /* vkCmdSetRayTracingPipelineStackSizeKHR */
    { 3137, 0xa9e2c72, 188 }, /* vkCmdSetSampleLocationsEXT */
    { 3164, 0x48f28c7f, 81 }, /* vkCmdSetScissor */
    { 3180, 0xf349b42f, 331 }, /* vkCmdSetScissorWithCountEXT */
    { 3208, 0xa8f534e2, 86 }, /* vkCmdSetStencilCompareMask */
    { 3235, 0xbb885f19, 338 }, /* vkCmdSetStencilOpEXT */
    { 3256, 0x83e2b024, 88 }, /* vkCmdSetStencilReference */
    { 3281, 0x16cc6095, 337 }, /* vkCmdSetStencilTestEnableEXT */
    { 3310, 0xe7c4b134, 87 }, /* vkCmdSetStencilWriteMask */
    { 3335, 0x53d6c2b, 80 }, /* vkCmdSetViewport */
    { 3352, 0x54d063a4, 255 }, /* vkCmdSetViewportShadingRatePaletteNV */
    { 3389, 0x60ee2453, 186 }, /* vkCmdSetViewportWScalingNV */
    { 3416, 0xa3d72e5b, 330 }, /* vkCmdSetViewportWithCountEXT */
    { 3445, 0xaf8c1f1e, 285 }, /* vkCmdTraceRaysIndirectKHR */
    { 3471, 0x5eb65f0c, 277 }, /* vkCmdTraceRaysKHR */
    { 3489, 0xe8687c49, 278 }, /* vkCmdTraceRaysNV */
    { 3506, 0xd2986b5e, 103 }, /* vkCmdUpdateBuffer */
    { 3524, 0x3b9346b3, 111 }, /* vkCmdWaitEvents */
    { 3540, 0x9ecb3888, 273 }, /* vkCmdWriteAccelerationStructuresPropertiesKHR */
    { 3586, 0xd2925ead, 274 }, /* vkCmdWriteAccelerationStructuresPropertiesNV */
    { 3631, 0x447be82c, 222 }, /* vkCmdWriteBufferMarkerAMD */
    { 3657, 0xec4d324c, 118 }, /* vkCmdWriteTimestamp */
    { 3677, 0xbad693ed, 260 }, /* vkCompileDeferredNV */
    { 3697, 0x45e623ac, 268 }, /* vkCopyAccelerationStructureKHR */
    { 3728, 0x9726ae20, 270 }, /* vkCopyAccelerationStructureToMemoryKHR */
    { 3767, 0xf7e06558, 272 }, /* vkCopyMemoryToAccelerationStructureKHR */
    { 3806, 0x30dceabb, 317 }, /* vkCreateAccelerationStructureKHR */
    { 3839, 0x9dc98a12, 261 }, /* vkCreateAccelerationStructureNV */
    { 3871, 0x7d4282b9, 36 }, /* vkCreateBuffer */
    { 3886, 0x925bd256, 38 }, /* vkCreateBufferView */
    { 3905, 0x820fe476, 71 }, /* vkCreateCommandPool */
    { 3925, 0xf70c85eb, 52 }, /* vkCreateComputePipelines */
    { 3950, 0x3f5d1a36, 322 }, /* vkCreateDeferredOperationKHR */
    { 3979, 0xfb95a8a4, 60 }, /* vkCreateDescriptorPool */
    { 4002, 0x3c14cc74, 58 }, /* vkCreateDescriptorSetLayout */
    { 4030, 0xad3ce733, 175 }, /* vkCreateDescriptorUpdateTemplate */
    { 4063, 0x5189488a, 176 }, /* vkCreateDescriptorUpdateTemplateKHR */
    { 4099, 0xe7188731, 26 }, /* vkCreateEvent */
    { 4113, 0x958af968, 19 }, /* vkCreateFence */
    { 4127, 0x887a38c4, 66 }, /* vkCreateFramebuffer */
    { 4147, 0x4b59f96d, 51 }, /* vkCreateGraphicsPipelines */
    { 4173, 0x652128c2, 40 }, /* vkCreateImage */
    { 4187, 0xdce077ff, 43 }, /* vkCreateImageView */
    { 4205, 0x3bc09b11, 141 }, /* vkCreateIndirectCommandsLayoutNV */
    { 4238, 0xcbf6489f, 47 }, /* vkCreatePipelineCache */
    { 4260, 0x451ef1ed, 54 }, /* vkCreatePipelineLayout */
    { 4283, 0xc06d475f, 339 }, /* vkCreatePrivateDataSlotEXT */
    { 4310, 0x5edcd92b, 31 }, /* vkCreateQueryPool */
    { 4328, 0x28847755, 284 }, /* vkCreateRayTracingPipelinesKHR */
    { 4359, 0x12bca48, 283 }, /* vkCreateRayTracingPipelinesNV */
    { 4389, 0x109a9c18, 68 }, /* vkCreateRenderPass */
    { 4408, 0x46b16d5a, 223 }, /* vkCreateRenderPass2 */
    { 4428, 0xfa16043b, 224 }, /* vkCreateRenderPass2KHR */
    { 4451, 0x13cf03f, 56 }, /* vkCreateSampler */
    { 4467, 0xe6a58c26, 195 }, /* vkCreateSamplerYcbcrConversion */
    { 4498, 0x7482104f, 196 }, /* vkCreateSamplerYcbcrConversionKHR */
    { 4532, 0xf2065e5b, 24 }, /* vkCreateSemaphore */
    { 4550, 0xa0d3cea2, 45 }, /* vkCreateShaderModule */
    { 4571, 0x47655c4a, 125 }, /* vkCreateSharedSwapchainsKHR */
    { 4599, 0xcdefcaa8, 126 }, /* vkCreateSwapchainKHR */
    { 4620, 0x591d7ed9, 200 }, /* vkCreateValidationCacheEXT */
    { 4647, 0xe206fb25, 131 }, /* vkDebugMarkerSetObjectNameEXT */
    { 4677, 0x30799448, 132 }, /* vkDebugMarkerSetObjectTagEXT */
    { 4706, 0x8c8648b8, 326 }, /* vkDeferredOperationJoinKHR */
    { 4733, 0x3eccc207, 262 }, /* vkDestroyAccelerationStructureKHR */
    { 4767, 0x693f9d26, 263 }, /* vkDestroyAccelerationStructureNV */
    { 4800, 0x94a07a45, 37 }, /* vkDestroyBuffer */
    { 4816, 0x98b27962, 39 }, /* vkDestroyBufferView */
    { 4836, 0xd5d83a0a, 72 }, /* vkDestroyCommandPool */
    { 4857, 0x7d549a02, 323 }, /* vkDestroyDeferredOperationKHR */
    { 4887, 0x47bdaf30, 61 }, /* vkDestroyDescriptorPool */
    { 4911, 0xa4227b08, 59 }, /* vkDestroyDescriptorSetLayout */
    { 4940, 0xbb2cbe7f, 177 }, /* vkDestroyDescriptorUpdateTemplate */
    { 4974, 0xaa83901e, 178 }, /* vkDestroyDescriptorUpdateTemplateKHR */
    { 5011, 0x1fbcc9cb, 1 }, /* vkDestroyDevice */
    { 5027, 0x4df27c05, 27 }, /* vkDestroyEvent */
    { 5042, 0xfc64ee3c, 20 }, /* vkDestroyFence */
    { 5057, 0xdc428e58, 67 }, /* vkDestroyFramebuffer */
    { 5078, 0xcbfb1d96, 41 }, /* vkDestroyImage */
    { 5093, 0xb5853953, 44 }, /* vkDestroyImageView */
    { 5112, 0x49b0725d, 142 }, /* vkDestroyIndirectCommandsLayoutNV */
    { 5146, 0x6aac68af, 53 }, /* vkDestroyPipeline */
    { 5164, 0x4112a673, 48 }, /* vkDestroyPipelineCache */
    { 5187, 0x9146f879, 55 }, /* vkDestroyPipelineLayout */
    { 5211, 0xe18d5d6b, 340 }, /* vkDestroyPrivateDataSlotEXT */
    { 5239, 0x37819a7f, 32 }, /* vkDestroyQueryPool */
    { 5258, 0x16f14324, 69 }, /* vkDestroyRenderPass */
    { 5278, 0x3b645153, 57 }, /* vkDestroySampler */
    { 5295, 0x20f261b2, 197 }, /* vkDestroySamplerYcbcrConversion */
    { 5327, 0xaaa623a3, 198 }, /* vkDestroySamplerYcbcrConversionKHR */
    { 5362, 0xcaab1faf, 25 }, /* vkDestroySemaphore */
    { 5381, 0x2d77af6e, 46 }, /* vkDestroyShaderModule */
    { 5403, 0x5a93ab74, 127 }, /* vkDestroySwapchainKHR */
    { 5425, 0x7a3d94e5, 201 }, /* vkDestroyValidationCacheEXT */
    { 5453, 0xd46c5f24, 5 }, /* vkDeviceWaitIdle */
    { 5470, 0xdbb064, 158 }, /* vkDisplayPowerControlEXT */
    { 5495, 0xaffb5725, 77 }, /* vkEndCommandBuffer */
    { 5514, 0xff52f051, 10 }, /* vkFlushMappedMemoryRanges */
    { 5540, 0xb9db2b91, 75 }, /* vkFreeCommandBuffers */
    { 5561, 0x7a1347b1, 64 }, /* vkFreeDescriptorSets */
    { 5582, 0x8f6f838a, 7 }, /* vkFreeMemory */
    { 5595, 0x4dbe8d2f, 351 }, /* vkGetAccelerationStructureBuildSizesKHR */
    { 5635, 0x1a50de81, 321 }, /* vkGetAccelerationStructureDeviceAddressKHR */
    { 5678, 0xd26f255a, 282 }, /* vkGetAccelerationStructureHandleNV */
    { 5713, 0x5d79203, 264 }, /* vkGetAccelerationStructureMemoryRequirementsNV */
    { 5760, 0xb891b5e, 237 }, /* vkGetAndroidHardwareBufferPropertiesANDROID */
    { 5804, 0x7022f0cd, 299 }, /* vkGetBufferDeviceAddress */
    { 5829, 0x3703280c, 301 }, /* vkGetBufferDeviceAddressEXT */
    { 5857, 0x713b5180, 300 }, /* vkGetBufferDeviceAddressKHR */
    { 5885, 0xab98422a, 13 }, /* vkGetBufferMemoryRequirements */
    { 5915, 0xd1fd0638, 189 }, /* vkGetBufferMemoryRequirements2 */
    { 5946, 0x78dbe98d, 190 }, /* vkGetBufferMemoryRequirements2KHR */
    { 5980, 0x2a5545a0, 297 }, /* vkGetBufferOpaqueCaptureAddress */
    { 6012, 0xddac1c65, 298 }, /* vkGetBufferOpaqueCaptureAddressKHR */
    { 6047, 0xcf3070fe, 212 }, /* vkGetCalibratedTimestampsEXT */
    { 6076, 0x7d902967, 324 }, /* vkGetDeferredOperationMaxConcurrencyKHR */
    { 6116, 0xf2144be9, 325 }, /* vkGetDeferredOperationResultKHR */
    { 6148, 0xfeac9573, 204 }, /* vkGetDescriptorSetLayoutSupport */
    { 6180, 0xd7e44a, 205 }, /* vkGetDescriptorSetLayoutSupportKHR */
    { 6215, 0xe86334c9, 286 }, /* vkGetDeviceAccelerationStructureCompatibilityKHR */
    { 6264, 0x2e218c10, 162 }, /* vkGetDeviceGroupPeerMemoryFeatures */
    { 6299, 0xa3809375, 163 }, /* vkGetDeviceGroupPeerMemoryFeaturesKHR */
    { 6337, 0xf72c87d4, 170 }, /* vkGetDeviceGroupPresentCapabilitiesKHR */
    { 6376, 0x41b28e81, 291 }, /* vkGetDeviceGroupSurfacePresentModes2EXT */
    { 6416, 0x6b9448c3, 171 }, /* vkGetDeviceGroupSurfacePresentModesKHR */
    { 6455, 0x46e38db5, 12 }, /* vkGetDeviceMemoryCommitment */
    { 6483, 0x9a0fe777, 311 }, /* vkGetDeviceMemoryOpaqueCaptureAddress */
    { 6521, 0x49339be6, 312 }, /* vkGetDeviceMemoryOpaqueCaptureAddressKHR */
    { 6562, 0xba013486, 0 }, /* vkGetDeviceProcAddr */
    { 6582, 0xcc920d9a, 2 }, /* vkGetDeviceQueue */
    { 6599, 0xb11a6348, 199 }, /* vkGetDeviceQueue2 */
    { 6617, 0x96d834b, 28 }, /* vkGetEventStatus */
    { 6634, 0x69a5d6af, 156 }, /* vkGetFenceFdKHR */
    { 6650, 0x5f391892, 22 }, /* vkGetFenceStatus */
    { 6667, 0x8963de2, 154 }, /* vkGetFenceWin32HandleKHR */
    { 6692, 0xac420aaf, 140 }, /* vkGetGeneratedCommandsMemoryRequirementsNV */
    { 6735, 0x12fa78a3, 296 }, /* vkGetImageDrmFormatModifierPropertiesEXT */
    { 6776, 0x916f1e63, 15 }, /* vkGetImageMemoryRequirements */
    { 6805, 0x56e213f7, 191 }, /* vkGetImageMemoryRequirements2 */
    { 6835, 0x8de28366, 192 }, /* vkGetImageMemoryRequirements2KHR */
    { 6868, 0x15855f5b, 17 }, /* vkGetImageSparseMemoryRequirements */
    { 6903, 0xbd4e3d3f, 193 }, /* vkGetImageSparseMemoryRequirements2 */
    { 6939, 0x3df40f5e, 194 }, /* vkGetImageSparseMemoryRequirements2KHR */
    { 6978, 0x9163b686, 42 }, /* vkGetImageSubresourceLayout */
    { 7006, 0xed8f1d33, 290 }, /* vkGetImageViewAddressNVX */
    { 7031, 0x20caa1e1, 289 }, /* vkGetImageViewHandleNVX */
    { 7055, 0x71220e82, 238 }, /* vkGetMemoryAndroidHardwareBufferANDROID */
    { 7095, 0x503c14c5, 148 }, /* vkGetMemoryFdKHR */
    { 7112, 0xb028a792, 149 }, /* vkGetMemoryFdPropertiesKHR */
    { 7139, 0x7030ee5b, 221 }, /* vkGetMemoryHostPointerPropertiesEXT */
    { 7175, 0x45fc7e1c, 146 }, /* vkGetMemoryWin32HandleKHR */
    { 7201, 0xc8795b9, 136 }, /* vkGetMemoryWin32HandleNV */
    { 7226, 0xb8f59859, 147 }, /* vkGetMemoryWin32HandlePropertiesKHR */
    { 7262, 0x19616a98, 185 }, /* vkGetPastPresentationTimingGOOGLE */
    { 7296, 0x1ec6c4ec, 310 }, /* vkGetPerformanceParameterINTEL */
    { 7327, 0x2092a349, 49 }, /* vkGetPipelineCacheData */
    { 7350, 0x8b20fc09, 315 }, /* vkGetPipelineExecutableInternalRepresentationsKHR */
    { 7400, 0x748dd8cd, 313 }, /* vkGetPipelineExecutablePropertiesKHR */
    { 7437, 0x5c4d6435, 314 }, /* vkGetPipelineExecutableStatisticsKHR */
    { 7474, 0x2dc1491d, 342 }, /* vkGetPrivateDataEXT */
    { 7494, 0xbf3f2cb3, 33 }, /* vkGetQueryPoolResults */
    { 7516, 0x428d4692, 246 }, /* vkGetQueueCheckpointDataNV */
    { 7543, 0x4b32ff8, 281 }, /* vkGetRayTracingCaptureReplayShaderGroupHandlesKHR */
    { 7593, 0x4693e853, 279 }, /* vkGetRayTracingShaderGroupHandlesKHR */
    { 7630, 0x3b54d93a, 280 }, /* vkGetRayTracingShaderGroupHandlesNV */
    { 7666, 0x2f709815, 287 }, /* vkGetRayTracingShaderGroupStackSizeKHR */
    { 7705, 0x85a9d101, 184 }, /* vkGetRefreshCycleDurationGOOGLE */
    { 7737, 0xa9820d22, 70 }, /* vkGetRenderAreaGranularity */
    { 7764, 0xd05a61a0, 231 }, /* vkGetSemaphoreCounterValue */
    { 7791, 0xf3c26065, 232 }, /* vkGetSemaphoreCounterValueKHR */
    { 7821, 0x3e0e9884, 152 }, /* vkGetSemaphoreFdKHR */
    { 7841, 0xd04be5e5, 150 }, /* vkGetSemaphoreWin32HandleKHR */
    { 7870, 0x5330743c, 210 }, /* vkGetShaderInfoAMD */
    { 7889, 0xa4aeb5a, 161 }, /* vkGetSwapchainCounterEXT */
    { 7914, 0x219d929, 207 }, /* vkGetSwapchainGrallocUsage2ANDROID */
    { 7949, 0x4979c9a3, 206 }, /* vkGetSwapchainGrallocUsageANDROID */
    { 7983, 0x57695f28, 128 }, /* vkGetSwapchainImagesKHR */
    { 8007, 0x66ae725e, 183 }, /* vkGetSwapchainStatusKHR */
    { 8031, 0xbbc9f99f, 202 }, /* vkGetValidationCacheDataEXT */
    { 8059, 0x51df0390, 157 }, /* vkImportFenceFdKHR */
    { 8078, 0x1bcbb079, 155 }, /* vkImportFenceWin32HandleKHR */
    { 8106, 0x36337c05, 153 }, /* vkImportSemaphoreFdKHR */
    { 8129, 0x7e2cfcdc, 151 }, /* vkImportSemaphoreWin32HandleKHR */
    { 8161, 0x65a01d77, 302 }, /* vkInitializePerformanceApiINTEL */
    { 8193, 0x1e115cca, 11 }, /* vkInvalidateMappedMemoryRanges */
    { 8224, 0xcb977bd8, 8 }, /* vkMapMemory */
    { 8236, 0xc3499606, 50 }, /* vkMergePipelineCaches */
    { 8258, 0xe8fe1154, 203 }, /* vkMergeValidationCachesEXT */
    { 8285, 0xcb7dc88, 215 }, /* vkQueueBeginDebugUtilsLabelEXT */
    { 8316, 0xc3628a09, 18 }, /* vkQueueBindSparse */
    { 8334, 0xf130b20a, 216 }, /* vkQueueEndDebugUtilsLabelEXT */
    { 8363, 0x56027200, 217 }, /* vkQueueInsertDebugUtilsLabelEXT */
    { 8395, 0xfc5fb6ce, 130 }, /* vkQueuePresentKHR */
    { 8413, 0xf8499f82, 309 }, /* vkQueueSetPerformanceConfigurationINTEL */
    { 8453, 0xa0313eef, 209 }, /* vkQueueSignalReleaseImageANDROID */
    { 8486, 0xfa4713ec, 3 }, /* vkQueueSubmit */
    { 8500, 0x6f8fc2a5, 4 }, /* vkQueueWaitIdle */
    { 8516, 0x26cc78f5, 159 }, /* vkRegisterDeviceEventEXT */
    { 8541, 0x4a0bd849, 160 }, /* vkRegisterDisplayEventEXT */
    { 8567, 0x13814325, 293 }, /* vkReleaseFullScreenExclusiveModeEXT */
    { 8603, 0x28575036, 308 }, /* vkReleasePerformanceConfigurationINTEL */
    { 8642, 0x8bdecb76, 295 }, /* vkReleaseProfilingLockKHR */
    { 8668, 0x847dc731, 78 }, /* vkResetCommandBuffer */
    { 8689, 0x6da9f7fd, 73 }, /* vkResetCommandPool */
    { 8708, 0x9bd85f5, 62 }, /* vkResetDescriptorPool */
    { 8730, 0x6d373ba8, 30 }, /* vkResetEvent */
    { 8743, 0x684781dc, 21 }, /* vkResetFences */
    { 8757, 0x4e671e02, 34 }, /* vkResetQueryPool */
    { 8774, 0xe6701e5f, 35 }, /* vkResetQueryPoolEXT */
    { 8794, 0x180cec44, 213 }, /* vkSetDebugUtilsObjectNameEXT */
    { 8823, 0x15942821, 214 }, /* vkSetDebugUtilsObjectTagEXT */
    { 8851, 0x592ae5f5, 29 }, /* vkSetEvent */
    { 8862, 0xa20f1ea9, 182 }, /* vkSetHdrMetadataEXT */
    { 8882, 0xbd1cd781, 211 }, /* vkSetLocalDimmingAMD */
    { 8903, 0x23456729, 341 }, /* vkSetPrivateDataEXT */
    { 8923, 0xcd347297, 235 }, /* vkSignalSemaphore */
    { 8941, 0x8fef55c6, 236 }, /* vkSignalSemaphoreKHR */
    { 8962, 0xfef2fb38, 144 }, /* vkTrimCommandPool */
    { 8980, 0x51177c8d, 145 }, /* vkTrimCommandPoolKHR */
    { 9001, 0x408975ae, 303 }, /* vkUninitializePerformanceApiINTEL */
    { 9035, 0x1a1a0e2f, 9 }, /* vkUnmapMemory */
    { 9049, 0x5349c9d, 179 }, /* vkUpdateDescriptorSetWithTemplate */
    { 9083, 0x214ad230, 180 }, /* vkUpdateDescriptorSetWithTemplateKHR */
    { 9120, 0xbfd090ae, 65 }, /* vkUpdateDescriptorSets */
    { 9143, 0x19d64c81, 23 }, /* vkWaitForFences */
    { 9159, 0x74368ad9, 233 }, /* vkWaitSemaphores */
    { 9176, 0x2bc77454, 234 }, /* vkWaitSemaphoresKHR */
    { 9196, 0x8bc9ae08, 276 }, /* vkWriteAccelerationStructuresPropertiesKHR */
};

/* Hash table stats:
 * size 352 entries
 * collisions entries:
 *     0      237
 *     1      51
 *     2      26
 *     3      17
 *     4      5
 *     5      4
 *     6      2
 *     7      4
 *     8      1
 *     9+     5
 */

#define none 0xffff
static const uint16_t device_string_map[512] = {
    0x00a9,
    none,
    0x00c1,
    0x00e4,
    none,
    0x003d,
    0x004a,
    0x00bc,
    0x0148,
    0x0118,
    0x0024,
    0x0127,
    0x006c,
    none,
    none,
    none,
    0x00f4,
    0x0135,
    0x0097,
    0x013b,
    none,
    0x0121,
    none,
    none,
    0x00ae,
    0x0136,
    0x000a,
    0x014f,
    0x0112,
    0x00c0,
    0x00c5,
    0x0067,
    0x003b,
    0x014e,
    none,
    none,
    0x0082,
    none,
    0x00b2,
    none,
    0x0060,
    0x002b,
    0x00e9,
    0x0085,
    0x0015,
    none,
    0x015f,
    0x007f,
    0x013a,
    none,
    0x00e7,
    0x0094,
    none,
    0x0050,
    0x009c,
    none,
    0x00ea,
    none,
    none,
    0x00b0,
    0x00c8,
    0x002e,
    0x00c7,
    0x002a,
    0x0039,
    0x006b,
    0x0139,
    0x015a,
    0x014d,
    0x00be,
    none,
    none,
    0x00ad,
    0x008b,
    0x000e,
    none,
    0x0023,
    none,
    0x014b,
    0x0034,
    0x0047,
    0x00dd,
    0x0043,
    0x0054,
    0x015e,
    0x0158,
    0x0099,
    none,
    0x003f,
    0x0114,
    none,
    0x0088,
    0x0142,
    0x00b6,
    0x012d,
    0x0091,
    none,
    0x0030,
    0x00b3,
    0x0069,
    0x00db,
    0x0090,
    0x0087,
    0x003a,
    0x005d,
    none,
    none,
    0x00c9,
    0x0032,
    none,
    0x00b4,
    0x0144,
    0x001b,
    none,
    0x007d,
    0x00ce,
    0x009e,
    0x0071,
    0x0072,
    0x0131,
    0x0052,
    0x00cf,
    none,
    none,
    none,
    none,
    0x002f,
    0x007e,
    none,
    0x00ba,
    0x0061,
    0x00cc,
    0x0126,
    0x014c,
    0x00fc,
    0x0042,
    0x008e,
    0x0045,
    0x004f,
    0x0070,
    0x011f,
    0x0156,
    none,
    none,
    0x0075,
    0x00fb,
    0x00c4,
    none,
    0x0019,
    0x0083,
    0x00f2,
    0x0153,
    0x0115,
    0x010b,
    0x006e,
    0x0128,
    0x009a,
    0x00a0,
    0x00b1,
    0x00a8,
    0x002c,
    0x005e,
    0x00b5,
    0x0104,
    0x0014,
    0x00d1,
    none,
    0x00e2,
    0x00b7,
    0x0150,
    none,
    none,
    none,
    0x0005,
    0x0138,
    0x00cd,
    0x0159,
    0x00ed,
    0x0130,
    0x008d,
    none,
    none,
    none,
    0x009d,
    0x0027,
    0x0098,
    0x0029,
    0x0096,
    none,
    none,
    none,
    none,
    0x008f,
    0x015b,
    0x00a5,
    0x00f8,
    0x00a3,
    0x010f,
    0x011c,
    none,
    0x004b,
    0x00f3,
    0x0134,
    0x00bb,
    none,
    0x00e6,
    0x010e,
    none,
    none,
    none,
    none,
    none,
    0x0036,
    0x0100,
    0x001d,
    0x0055,
    0x0074,
    0x00b8,
    0x002d,
    none,
    0x0132,
    0x0001,
    0x0101,
    none,
    0x0056,
    0x013c,
    0x0080,
    0x0016,
    none,
    0x00d9,
    none,
    none,
    0x0103,
    none,
    0x0125,
    none,
    0x0021,
    none,
    none,
    0x013e,
    none,
    0x011d,
    none,
    0x00f7,
    none,
    0x0141,
    none,
    none,
    0x007b,
    0x0058,
    none,
    0x0105,
    none,
    none,
    0x00ee,
    0x0116,
    none,
    0x0122,
    none,
    none,
    0x0140,
    0x006d,
    0x0053,
    0x0068,
    0x0028,
    none,
    none,
    none,
    0x0076,
    none,
    none,
    none,
    0x0037,
    0x004d,
    0x015d,
    none,
    none,
    0x0051,
    none,
    none,
    0x000f,
    0x0081,
    0x0006,
    0x00c3,
    none,
    0x0020,
    0x0089,
    0x0078,
    none,
    none,
    0x004e,
    none,
    0x00a7,
    0x00b9,
    0x00bd,
    0x0009,
    0x012c,
    0x012a,
    none,
    0x00ab,
    0x0111,
    none,
    0x0003,
    0x00e1,
    0x00c2,
    0x0059,
    0x008a,
    0x009f,
    0x0084,
    0x0123,
    0x0040,
    0x0063,
    0x00dc,
    none,
    0x0007,
    none,
    0x0152,
    none,
    0x0038,
    0x0013,
    none,
    0x0041,
    none,
    0x011b,
    0x00a1,
    none,
    0x010c,
    none,
    0x00fe,
    0x0117,
    0x00d2,
    0x00ff,
    none,
    0x0120,
    none,
    0x000c,
    none,
    none,
    0x0109,
    0x00cb,
    0x0137,
    0x00ac,
    0x006f,
    0x0146,
    0x0095,
    none,
    0x00af,
    0x0108,
    0x0011,
    0x00da,
    0x008c,
    0x00aa,
    none,
    none,
    0x00bf,
    none,
    none,
    0x0077,
    0x00d3,
    0x0004,
    0x00a2,
    none,
    none,
    0x00d0,
    none,
    0x0065,
    0x0073,
    none,
    none,
    0x00e5,
    0x0002,
    0x00f1,
    0x00d8,
    0x00f5,
    0x0145,
    0x0017,
    0x0119,
    0x0107,
    0x00ef,
    none,
    none,
    0x004c,
    none,
    none,
    0x00a4,
    0x005f,
    0x013d,
    none,
    0x001c,
    none,
    none,
    none,
    none,
    0x0022,
    0x00e0,
    0x011a,
    none,
    0x00eb,
    none,
    none,
    0x0046,
    0x0049,
    0x0110,
    0x00e3,
    0x0062,
    none,
    0x00ca,
    0x010a,
    none,
    none,
    0x00fd,
    0x007c,
    none,
    0x00fa,
    0x015c,
    0x0012,
    0x00ec,
    0x003e,
    none,
    0x00d5,
    0x0086,
    none,
    0x00e8,
    0x00d7,
    0x0149,
    0x0031,
    0x0143,
    none,
    0x0033,
    0x0010,
    0x0157,
    0x00d6,
    0x0133,
    0x006a,
    0x00d4,
    0x0124,
    none,
    0x00f9,
    0x012b,
    0x00de,
    none,
    0x0113,
    0x0151,
    none,
    0x005a,
    0x0155,
    none,
    0x0044,
    0x0066,
    none,
    none,
    none,
    0x00df,
    0x012e,
    0x0154,
    0x0079,
    none,
    0x012f,
    none,
    0x000b,
    0x0000,
    0x001f,
    0x000d,
    0x007a,
    none,
    0x0057,
    0x0093,
    0x0008,
    0x00f6,
    none,
    none,
    none,
    0x0035,
    none,
    0x001a,
    none,
    0x0026,
    none,
    0x00c6,
    0x0129,
    none,
    0x010d,
    0x0102,
    none,
    none,
    0x003c,
    0x0018,
    none,
    0x005b,
    0x00f0,
    none,
    0x001e,
    0x013f,
    0x0092,
    0x0025,
    0x014a,
    none,
    none,
    none,
    none,
    none,
    0x0048,
    none,
    0x0106,
    0x011e,
    0x005c,
    none,
    0x0064,
    none,
    0x0147,
    0x009b,
    0x00a6,
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
        i = device_string_map[h & 511];
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
    21,
    22,
    23,
    24,
    25,
    26,
    27,
    28,
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
    155,
    156,
    157,
    158,
    159,
    160,
    160,
    161,
    161,
    162,
    162,
    163,
    163,
    164,
    165,
    166,
    167,
    167,
    168,
    168,
    169,
    169,
    170,
    170,
    171,
    172,
    173,
    174,
    175,
    176,
    177,
    178,
    179,
    179,
    180,
    180,
    181,
    181,
    182,
    182,
    183,
    183,
    184,
    185,
    186,
    187,
    188,
    189,
    189,
    190,
    191,
    192,
    193,
    194,
    195,
    196,
    197,
    198,
    199,
    200,
    201,
    202,
    203,
    204,
    205,
    206,
    207,
    207,
    208,
    208,
    209,
    209,
    210,
    210,
    211,
    211,
    212,
    212,
    213,
    213,
    214,
    215,
    216,
    216,
    216,
    217,
    217,
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
    269,
    270,
    270,
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
    313,
    314,
    315,
    316,
    317,
    318,
    319,
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
      /* CreateDebugReportCallbackEXT */
      if (instance->EXT_debug_report) return true;
      return false;
   case 19:
      /* DestroyDebugReportCallbackEXT */
      if (instance->EXT_debug_report) return true;
      return false;
   case 20:
      /* DebugReportMessageEXT */
      if (instance->EXT_debug_report) return true;
      return false;
   case 21:
      /* EnumeratePhysicalDeviceGroups */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 22:
      /* EnumeratePhysicalDeviceGroupsKHR */
      if (instance->KHR_device_group_creation) return true;
      return false;
   case 23:
      /* CreateIOSSurfaceMVK */
      if (instance->MVK_ios_surface) return true;
      return false;
   case 24:
      /* CreateMacOSSurfaceMVK */
      if (instance->MVK_macos_surface) return true;
      return false;
   case 25:
      /* CreateMetalSurfaceEXT */
      if (instance->EXT_metal_surface) return true;
      return false;
   case 26:
      /* CreateDebugUtilsMessengerEXT */
      if (instance->EXT_debug_utils) return true;
      return false;
   case 27:
      /* DestroyDebugUtilsMessengerEXT */
      if (instance->EXT_debug_utils) return true;
      return false;
   case 28:
      /* SubmitDebugUtilsMessageEXT */
      if (instance->EXT_debug_utils) return true;
      return false;
   case 29:
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
      /* GetPhysicalDeviceExternalImageFormatPropertiesNV */
      if (instance->NV_external_memory_capabilities) return true;
      return false;
   case 26:
      /* GetPhysicalDeviceFeatures2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 27:
      /* GetPhysicalDeviceFeatures2KHR */
      if (instance->KHR_get_physical_device_properties2) return true;
      return false;
   case 28:
      /* GetPhysicalDeviceProperties2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 29:
      /* GetPhysicalDeviceProperties2KHR */
      if (instance->KHR_get_physical_device_properties2) return true;
      return false;
   case 30:
      /* GetPhysicalDeviceFormatProperties2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 31:
      /* GetPhysicalDeviceFormatProperties2KHR */
      if (instance->KHR_get_physical_device_properties2) return true;
      return false;
   case 32:
      /* GetPhysicalDeviceImageFormatProperties2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 33:
      /* GetPhysicalDeviceImageFormatProperties2KHR */
      if (instance->KHR_get_physical_device_properties2) return true;
      return false;
   case 34:
      /* GetPhysicalDeviceQueueFamilyProperties2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 35:
      /* GetPhysicalDeviceQueueFamilyProperties2KHR */
      if (instance->KHR_get_physical_device_properties2) return true;
      return false;
   case 36:
      /* GetPhysicalDeviceMemoryProperties2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 37:
      /* GetPhysicalDeviceMemoryProperties2KHR */
      if (instance->KHR_get_physical_device_properties2) return true;
      return false;
   case 38:
      /* GetPhysicalDeviceSparseImageFormatProperties2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 39:
      /* GetPhysicalDeviceSparseImageFormatProperties2KHR */
      if (instance->KHR_get_physical_device_properties2) return true;
      return false;
   case 40:
      /* GetPhysicalDeviceExternalBufferProperties */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 41:
      /* GetPhysicalDeviceExternalBufferPropertiesKHR */
      if (instance->KHR_external_memory_capabilities) return true;
      return false;
   case 42:
      /* GetPhysicalDeviceExternalSemaphoreProperties */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 43:
      /* GetPhysicalDeviceExternalSemaphorePropertiesKHR */
      if (instance->KHR_external_semaphore_capabilities) return true;
      return false;
   case 44:
      /* GetPhysicalDeviceExternalFenceProperties */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 45:
      /* GetPhysicalDeviceExternalFencePropertiesKHR */
      if (instance->KHR_external_fence_capabilities) return true;
      return false;
   case 46:
      /* ReleaseDisplayEXT */
      if (instance->EXT_direct_mode_display) return true;
      return false;
   case 47:
      /* AcquireXlibDisplayEXT */
      if (instance->EXT_acquire_xlib_display) return true;
      return false;
   case 48:
      /* GetRandROutputDisplayEXT */
      if (instance->EXT_acquire_xlib_display) return true;
      return false;
   case 49:
      /* AcquireWinrtDisplayNV */
      /* All device extensions are considered enabled at the instance level */
      return true;
      return false;
   case 50:
      /* GetWinrtDisplayNV */
      /* All device extensions are considered enabled at the instance level */
      return true;
      return false;
   case 51:
      /* GetPhysicalDeviceSurfaceCapabilities2EXT */
      if (instance->EXT_display_surface_counter) return true;
      return false;
   case 52:
      /* GetPhysicalDevicePresentRectanglesKHR */
      /* All device extensions are considered enabled at the instance level */
      return true;
      /* All device extensions are considered enabled at the instance level */
      return true;
      return false;
   case 53:
      /* GetPhysicalDeviceMultisamplePropertiesEXT */
      /* All device extensions are considered enabled at the instance level */
      return true;
      return false;
   case 54:
      /* GetPhysicalDeviceSurfaceCapabilities2KHR */
      if (instance->KHR_get_surface_capabilities2) return true;
      return false;
   case 55:
      /* GetPhysicalDeviceSurfaceFormats2KHR */
      if (instance->KHR_get_surface_capabilities2) return true;
      return false;
   case 56:
      /* GetPhysicalDeviceDisplayProperties2KHR */
      if (instance->KHR_get_display_properties2) return true;
      return false;
   case 57:
      /* GetPhysicalDeviceDisplayPlaneProperties2KHR */
      if (instance->KHR_get_display_properties2) return true;
      return false;
   case 58:
      /* GetDisplayModeProperties2KHR */
      if (instance->KHR_get_display_properties2) return true;
      return false;
   case 59:
      /* GetDisplayPlaneCapabilities2KHR */
      if (instance->KHR_get_display_properties2) return true;
      return false;
   case 60:
      /* GetPhysicalDeviceCalibrateableTimeDomainsEXT */
      /* All device extensions are considered enabled at the instance level */
      return true;
      return false;
   case 61:
      /* GetPhysicalDeviceCooperativeMatrixPropertiesNV */
      /* All device extensions are considered enabled at the instance level */
      return true;
      return false;
   case 62:
      /* GetPhysicalDeviceSurfacePresentModes2EXT */
      /* All device extensions are considered enabled at the instance level */
      return true;
      return false;
   case 63:
      /* EnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR */
      /* All device extensions are considered enabled at the instance level */
      return true;
      return false;
   case 64:
      /* GetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR */
      /* All device extensions are considered enabled at the instance level */
      return true;
      return false;
   case 65:
      /* GetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV */
      /* All device extensions are considered enabled at the instance level */
      return true;
      return false;
   case 66:
      /* GetPhysicalDeviceToolPropertiesEXT */
      /* All device extensions are considered enabled at the instance level */
      return true;
      return false;
   case 67:
      /* GetPhysicalDeviceFragmentShadingRatesKHR */
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
      /* DestroyPipeline */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 54:
      /* CreatePipelineLayout */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 55:
      /* DestroyPipelineLayout */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 56:
      /* CreateSampler */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 57:
      /* DestroySampler */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 58:
      /* CreateDescriptorSetLayout */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 59:
      /* DestroyDescriptorSetLayout */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 60:
      /* CreateDescriptorPool */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 61:
      /* DestroyDescriptorPool */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 62:
      /* ResetDescriptorPool */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 63:
      /* AllocateDescriptorSets */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 64:
      /* FreeDescriptorSets */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 65:
      /* UpdateDescriptorSets */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 66:
      /* CreateFramebuffer */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 67:
      /* DestroyFramebuffer */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 68:
      /* CreateRenderPass */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 69:
      /* DestroyRenderPass */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 70:
      /* GetRenderAreaGranularity */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 71:
      /* CreateCommandPool */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 72:
      /* DestroyCommandPool */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 73:
      /* ResetCommandPool */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 74:
      /* AllocateCommandBuffers */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 75:
      /* FreeCommandBuffers */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 76:
      /* BeginCommandBuffer */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 77:
      /* EndCommandBuffer */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 78:
      /* ResetCommandBuffer */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 79:
      /* CmdBindPipeline */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 80:
      /* CmdSetViewport */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 81:
      /* CmdSetScissor */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 82:
      /* CmdSetLineWidth */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 83:
      /* CmdSetDepthBias */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 84:
      /* CmdSetBlendConstants */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 85:
      /* CmdSetDepthBounds */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 86:
      /* CmdSetStencilCompareMask */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 87:
      /* CmdSetStencilWriteMask */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 88:
      /* CmdSetStencilReference */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 89:
      /* CmdBindDescriptorSets */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 90:
      /* CmdBindIndexBuffer */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 91:
      /* CmdBindVertexBuffers */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 92:
      /* CmdDraw */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 93:
      /* CmdDrawIndexed */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 94:
      /* CmdDrawIndirect */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 95:
      /* CmdDrawIndexedIndirect */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 96:
      /* CmdDispatch */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 97:
      /* CmdDispatchIndirect */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 98:
      /* CmdCopyBuffer */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 99:
      /* CmdCopyImage */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 100:
      /* CmdBlitImage */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 101:
      /* CmdCopyBufferToImage */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 102:
      /* CmdCopyImageToBuffer */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 103:
      /* CmdUpdateBuffer */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 104:
      /* CmdFillBuffer */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 105:
      /* CmdClearColorImage */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 106:
      /* CmdClearDepthStencilImage */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 107:
      /* CmdClearAttachments */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 108:
      /* CmdResolveImage */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 109:
      /* CmdSetEvent */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 110:
      /* CmdResetEvent */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 111:
      /* CmdWaitEvents */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 112:
      /* CmdPipelineBarrier */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 113:
      /* CmdBeginQuery */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 114:
      /* CmdEndQuery */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 115:
      /* CmdBeginConditionalRenderingEXT */
      if (!device || device->EXT_conditional_rendering) return true;
      return false;
   case 116:
      /* CmdEndConditionalRenderingEXT */
      if (!device || device->EXT_conditional_rendering) return true;
      return false;
   case 117:
      /* CmdResetQueryPool */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 118:
      /* CmdWriteTimestamp */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 119:
      /* CmdCopyQueryPoolResults */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 120:
      /* CmdPushConstants */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 121:
      /* CmdBeginRenderPass */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 122:
      /* CmdNextSubpass */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 123:
      /* CmdEndRenderPass */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 124:
      /* CmdExecuteCommands */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 125:
      /* CreateSharedSwapchainsKHR */
      if (!device || device->KHR_display_swapchain) return true;
      return false;
   case 126:
      /* CreateSwapchainKHR */
      if (!device || device->KHR_swapchain) return true;
      return false;
   case 127:
      /* DestroySwapchainKHR */
      if (!device || device->KHR_swapchain) return true;
      return false;
   case 128:
      /* GetSwapchainImagesKHR */
      if (!device || device->KHR_swapchain) return true;
      return false;
   case 129:
      /* AcquireNextImageKHR */
      if (!device || device->KHR_swapchain) return true;
      return false;
   case 130:
      /* QueuePresentKHR */
      if (!device || device->KHR_swapchain) return true;
      return false;
   case 131:
      /* DebugMarkerSetObjectNameEXT */
      if (!device || device->EXT_debug_marker) return true;
      return false;
   case 132:
      /* DebugMarkerSetObjectTagEXT */
      if (!device || device->EXT_debug_marker) return true;
      return false;
   case 133:
      /* CmdDebugMarkerBeginEXT */
      if (!device || device->EXT_debug_marker) return true;
      return false;
   case 134:
      /* CmdDebugMarkerEndEXT */
      if (!device || device->EXT_debug_marker) return true;
      return false;
   case 135:
      /* CmdDebugMarkerInsertEXT */
      if (!device || device->EXT_debug_marker) return true;
      return false;
   case 136:
      /* GetMemoryWin32HandleNV */
      if (!device || device->NV_external_memory_win32) return true;
      return false;
   case 137:
      /* CmdExecuteGeneratedCommandsNV */
      if (!device || device->NV_device_generated_commands) return true;
      return false;
   case 138:
      /* CmdPreprocessGeneratedCommandsNV */
      if (!device || device->NV_device_generated_commands) return true;
      return false;
   case 139:
      /* CmdBindPipelineShaderGroupNV */
      if (!device || device->NV_device_generated_commands) return true;
      return false;
   case 140:
      /* GetGeneratedCommandsMemoryRequirementsNV */
      if (!device || device->NV_device_generated_commands) return true;
      return false;
   case 141:
      /* CreateIndirectCommandsLayoutNV */
      if (!device || device->NV_device_generated_commands) return true;
      return false;
   case 142:
      /* DestroyIndirectCommandsLayoutNV */
      if (!device || device->NV_device_generated_commands) return true;
      return false;
   case 143:
      /* CmdPushDescriptorSetKHR */
      if (!device || device->KHR_push_descriptor) return true;
      return false;
   case 144:
      /* TrimCommandPool */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 145:
      /* TrimCommandPoolKHR */
      if (!device || device->KHR_maintenance1) return true;
      return false;
   case 146:
      /* GetMemoryWin32HandleKHR */
      if (!device || device->KHR_external_memory_win32) return true;
      return false;
   case 147:
      /* GetMemoryWin32HandlePropertiesKHR */
      if (!device || device->KHR_external_memory_win32) return true;
      return false;
   case 148:
      /* GetMemoryFdKHR */
      if (!device || device->KHR_external_memory_fd) return true;
      return false;
   case 149:
      /* GetMemoryFdPropertiesKHR */
      if (!device || device->KHR_external_memory_fd) return true;
      return false;
   case 150:
      /* GetSemaphoreWin32HandleKHR */
      if (!device || device->KHR_external_semaphore_win32) return true;
      return false;
   case 151:
      /* ImportSemaphoreWin32HandleKHR */
      if (!device || device->KHR_external_semaphore_win32) return true;
      return false;
   case 152:
      /* GetSemaphoreFdKHR */
      if (!device || device->KHR_external_semaphore_fd) return true;
      return false;
   case 153:
      /* ImportSemaphoreFdKHR */
      if (!device || device->KHR_external_semaphore_fd) return true;
      return false;
   case 154:
      /* GetFenceWin32HandleKHR */
      if (!device || device->KHR_external_fence_win32) return true;
      return false;
   case 155:
      /* ImportFenceWin32HandleKHR */
      if (!device || device->KHR_external_fence_win32) return true;
      return false;
   case 156:
      /* GetFenceFdKHR */
      if (!device || device->KHR_external_fence_fd) return true;
      return false;
   case 157:
      /* ImportFenceFdKHR */
      if (!device || device->KHR_external_fence_fd) return true;
      return false;
   case 158:
      /* DisplayPowerControlEXT */
      if (!device || device->EXT_display_control) return true;
      return false;
   case 159:
      /* RegisterDeviceEventEXT */
      if (!device || device->EXT_display_control) return true;
      return false;
   case 160:
      /* RegisterDisplayEventEXT */
      if (!device || device->EXT_display_control) return true;
      return false;
   case 161:
      /* GetSwapchainCounterEXT */
      if (!device || device->EXT_display_control) return true;
      return false;
   case 162:
      /* GetDeviceGroupPeerMemoryFeatures */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 163:
      /* GetDeviceGroupPeerMemoryFeaturesKHR */
      if (!device || device->KHR_device_group) return true;
      return false;
   case 164:
      /* BindBufferMemory2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 165:
      /* BindBufferMemory2KHR */
      if (!device || device->KHR_bind_memory2) return true;
      return false;
   case 166:
      /* BindImageMemory2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 167:
      /* BindImageMemory2KHR */
      if (!device || device->KHR_bind_memory2) return true;
      return false;
   case 168:
      /* CmdSetDeviceMask */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 169:
      /* CmdSetDeviceMaskKHR */
      if (!device || device->KHR_device_group) return true;
      return false;
   case 170:
      /* GetDeviceGroupPresentCapabilitiesKHR */
      if (!device || device->KHR_swapchain) return true;
      if (!device || device->KHR_device_group) return true;
      return false;
   case 171:
      /* GetDeviceGroupSurfacePresentModesKHR */
      if (!device || device->KHR_swapchain) return true;
      if (!device || device->KHR_device_group) return true;
      return false;
   case 172:
      /* AcquireNextImage2KHR */
      if (!device || device->KHR_swapchain) return true;
      if (!device || device->KHR_device_group) return true;
      return false;
   case 173:
      /* CmdDispatchBase */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 174:
      /* CmdDispatchBaseKHR */
      if (!device || device->KHR_device_group) return true;
      return false;
   case 175:
      /* CreateDescriptorUpdateTemplate */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 176:
      /* CreateDescriptorUpdateTemplateKHR */
      if (!device || device->KHR_descriptor_update_template) return true;
      return false;
   case 177:
      /* DestroyDescriptorUpdateTemplate */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 178:
      /* DestroyDescriptorUpdateTemplateKHR */
      if (!device || device->KHR_descriptor_update_template) return true;
      return false;
   case 179:
      /* UpdateDescriptorSetWithTemplate */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 180:
      /* UpdateDescriptorSetWithTemplateKHR */
      if (!device || device->KHR_descriptor_update_template) return true;
      return false;
   case 181:
      /* CmdPushDescriptorSetWithTemplateKHR */
      if (!device || device->KHR_push_descriptor) return true;
      if (!device || device->KHR_push_descriptor) return true;
      if (!device || device->KHR_descriptor_update_template) return true;
      return false;
   case 182:
      /* SetHdrMetadataEXT */
      if (!device || device->EXT_hdr_metadata) return true;
      return false;
   case 183:
      /* GetSwapchainStatusKHR */
      if (!device || device->KHR_shared_presentable_image) return true;
      return false;
   case 184:
      /* GetRefreshCycleDurationGOOGLE */
      if (!device || device->GOOGLE_display_timing) return true;
      return false;
   case 185:
      /* GetPastPresentationTimingGOOGLE */
      if (!device || device->GOOGLE_display_timing) return true;
      return false;
   case 186:
      /* CmdSetViewportWScalingNV */
      if (!device || device->NV_clip_space_w_scaling) return true;
      return false;
   case 187:
      /* CmdSetDiscardRectangleEXT */
      if (!device || device->EXT_discard_rectangles) return true;
      return false;
   case 188:
      /* CmdSetSampleLocationsEXT */
      if (!device || device->EXT_sample_locations) return true;
      return false;
   case 189:
      /* GetBufferMemoryRequirements2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 190:
      /* GetBufferMemoryRequirements2KHR */
      if (!device || device->KHR_get_memory_requirements2) return true;
      return false;
   case 191:
      /* GetImageMemoryRequirements2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 192:
      /* GetImageMemoryRequirements2KHR */
      if (!device || device->KHR_get_memory_requirements2) return true;
      return false;
   case 193:
      /* GetImageSparseMemoryRequirements2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 194:
      /* GetImageSparseMemoryRequirements2KHR */
      if (!device || device->KHR_get_memory_requirements2) return true;
      return false;
   case 195:
      /* CreateSamplerYcbcrConversion */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 196:
      /* CreateSamplerYcbcrConversionKHR */
      if (!device || device->KHR_sampler_ycbcr_conversion) return true;
      return false;
   case 197:
      /* DestroySamplerYcbcrConversion */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 198:
      /* DestroySamplerYcbcrConversionKHR */
      if (!device || device->KHR_sampler_ycbcr_conversion) return true;
      return false;
   case 199:
      /* GetDeviceQueue2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 200:
      /* CreateValidationCacheEXT */
      if (!device || device->EXT_validation_cache) return true;
      return false;
   case 201:
      /* DestroyValidationCacheEXT */
      if (!device || device->EXT_validation_cache) return true;
      return false;
   case 202:
      /* GetValidationCacheDataEXT */
      if (!device || device->EXT_validation_cache) return true;
      return false;
   case 203:
      /* MergeValidationCachesEXT */
      if (!device || device->EXT_validation_cache) return true;
      return false;
   case 204:
      /* GetDescriptorSetLayoutSupport */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 205:
      /* GetDescriptorSetLayoutSupportKHR */
      if (!device || device->KHR_maintenance3) return true;
      return false;
   case 206:
      /* GetSwapchainGrallocUsageANDROID */
      return true;
   case 207:
      /* GetSwapchainGrallocUsage2ANDROID */
      return true;
   case 208:
      /* AcquireImageANDROID */
      return true;
   case 209:
      /* QueueSignalReleaseImageANDROID */
      return true;
   case 210:
      /* GetShaderInfoAMD */
      if (!device || device->AMD_shader_info) return true;
      return false;
   case 211:
      /* SetLocalDimmingAMD */
      if (!device || device->AMD_display_native_hdr) return true;
      return false;
   case 212:
      /* GetCalibratedTimestampsEXT */
      if (!device || device->EXT_calibrated_timestamps) return true;
      return false;
   case 213:
      /* SetDebugUtilsObjectNameEXT */
      if (instance->EXT_debug_utils) return true;
      return false;
   case 214:
      /* SetDebugUtilsObjectTagEXT */
      if (instance->EXT_debug_utils) return true;
      return false;
   case 215:
      /* QueueBeginDebugUtilsLabelEXT */
      if (instance->EXT_debug_utils) return true;
      return false;
   case 216:
      /* QueueEndDebugUtilsLabelEXT */
      if (instance->EXT_debug_utils) return true;
      return false;
   case 217:
      /* QueueInsertDebugUtilsLabelEXT */
      if (instance->EXT_debug_utils) return true;
      return false;
   case 218:
      /* CmdBeginDebugUtilsLabelEXT */
      if (instance->EXT_debug_utils) return true;
      return false;
   case 219:
      /* CmdEndDebugUtilsLabelEXT */
      if (instance->EXT_debug_utils) return true;
      return false;
   case 220:
      /* CmdInsertDebugUtilsLabelEXT */
      if (instance->EXT_debug_utils) return true;
      return false;
   case 221:
      /* GetMemoryHostPointerPropertiesEXT */
      if (!device || device->EXT_external_memory_host) return true;
      return false;
   case 222:
      /* CmdWriteBufferMarkerAMD */
      if (!device || device->AMD_buffer_marker) return true;
      return false;
   case 223:
      /* CreateRenderPass2 */
      return VK_MAKE_VERSION(1, 2, 0) <= core_version;
   case 224:
      /* CreateRenderPass2KHR */
      if (!device || device->KHR_create_renderpass2) return true;
      return false;
   case 225:
      /* CmdBeginRenderPass2 */
      return VK_MAKE_VERSION(1, 2, 0) <= core_version;
   case 226:
      /* CmdBeginRenderPass2KHR */
      if (!device || device->KHR_create_renderpass2) return true;
      return false;
   case 227:
      /* CmdNextSubpass2 */
      return VK_MAKE_VERSION(1, 2, 0) <= core_version;
   case 228:
      /* CmdNextSubpass2KHR */
      if (!device || device->KHR_create_renderpass2) return true;
      return false;
   case 229:
      /* CmdEndRenderPass2 */
      return VK_MAKE_VERSION(1, 2, 0) <= core_version;
   case 230:
      /* CmdEndRenderPass2KHR */
      if (!device || device->KHR_create_renderpass2) return true;
      return false;
   case 231:
      /* GetSemaphoreCounterValue */
      return VK_MAKE_VERSION(1, 2, 0) <= core_version;
   case 232:
      /* GetSemaphoreCounterValueKHR */
      if (!device || device->KHR_timeline_semaphore) return true;
      return false;
   case 233:
      /* WaitSemaphores */
      return VK_MAKE_VERSION(1, 2, 0) <= core_version;
   case 234:
      /* WaitSemaphoresKHR */
      if (!device || device->KHR_timeline_semaphore) return true;
      return false;
   case 235:
      /* SignalSemaphore */
      return VK_MAKE_VERSION(1, 2, 0) <= core_version;
   case 236:
      /* SignalSemaphoreKHR */
      if (!device || device->KHR_timeline_semaphore) return true;
      return false;
   case 237:
      /* GetAndroidHardwareBufferPropertiesANDROID */
      if (!device || device->ANDROID_external_memory_android_hardware_buffer) return true;
      return false;
   case 238:
      /* GetMemoryAndroidHardwareBufferANDROID */
      if (!device || device->ANDROID_external_memory_android_hardware_buffer) return true;
      return false;
   case 239:
      /* CmdDrawIndirectCount */
      return VK_MAKE_VERSION(1, 2, 0) <= core_version;
   case 240:
      /* CmdDrawIndirectCountKHR */
      if (!device || device->KHR_draw_indirect_count) return true;
      return false;
   case 241:
      /* CmdDrawIndirectCountAMD */
      if (!device || device->AMD_draw_indirect_count) return true;
      return false;
   case 242:
      /* CmdDrawIndexedIndirectCount */
      return VK_MAKE_VERSION(1, 2, 0) <= core_version;
   case 243:
      /* CmdDrawIndexedIndirectCountKHR */
      if (!device || device->KHR_draw_indirect_count) return true;
      return false;
   case 244:
      /* CmdDrawIndexedIndirectCountAMD */
      if (!device || device->AMD_draw_indirect_count) return true;
      return false;
   case 245:
      /* CmdSetCheckpointNV */
      if (!device || device->NV_device_diagnostic_checkpoints) return true;
      return false;
   case 246:
      /* GetQueueCheckpointDataNV */
      if (!device || device->NV_device_diagnostic_checkpoints) return true;
      return false;
   case 247:
      /* CmdBindTransformFeedbackBuffersEXT */
      if (!device || device->EXT_transform_feedback) return true;
      return false;
   case 248:
      /* CmdBeginTransformFeedbackEXT */
      if (!device || device->EXT_transform_feedback) return true;
      return false;
   case 249:
      /* CmdEndTransformFeedbackEXT */
      if (!device || device->EXT_transform_feedback) return true;
      return false;
   case 250:
      /* CmdBeginQueryIndexedEXT */
      if (!device || device->EXT_transform_feedback) return true;
      return false;
   case 251:
      /* CmdEndQueryIndexedEXT */
      if (!device || device->EXT_transform_feedback) return true;
      return false;
   case 252:
      /* CmdDrawIndirectByteCountEXT */
      if (!device || device->EXT_transform_feedback) return true;
      return false;
   case 253:
      /* CmdSetExclusiveScissorNV */
      if (!device || device->NV_scissor_exclusive) return true;
      return false;
   case 254:
      /* CmdBindShadingRateImageNV */
      if (!device || device->NV_shading_rate_image) return true;
      return false;
   case 255:
      /* CmdSetViewportShadingRatePaletteNV */
      if (!device || device->NV_shading_rate_image) return true;
      return false;
   case 256:
      /* CmdSetCoarseSampleOrderNV */
      if (!device || device->NV_shading_rate_image) return true;
      return false;
   case 257:
      /* CmdDrawMeshTasksNV */
      if (!device || device->NV_mesh_shader) return true;
      return false;
   case 258:
      /* CmdDrawMeshTasksIndirectNV */
      if (!device || device->NV_mesh_shader) return true;
      return false;
   case 259:
      /* CmdDrawMeshTasksIndirectCountNV */
      if (!device || device->NV_mesh_shader) return true;
      return false;
   case 260:
      /* CompileDeferredNV */
      if (!device || device->NV_ray_tracing) return true;
      return false;
   case 261:
      /* CreateAccelerationStructureNV */
      if (!device || device->NV_ray_tracing) return true;
      return false;
   case 262:
      /* DestroyAccelerationStructureKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 263:
      /* DestroyAccelerationStructureNV */
      if (!device || device->NV_ray_tracing) return true;
      return false;
   case 264:
      /* GetAccelerationStructureMemoryRequirementsNV */
      if (!device || device->NV_ray_tracing) return true;
      return false;
   case 265:
      /* BindAccelerationStructureMemoryNV */
      if (!device || device->NV_ray_tracing) return true;
      return false;
   case 266:
      /* CmdCopyAccelerationStructureNV */
      if (!device || device->NV_ray_tracing) return true;
      return false;
   case 267:
      /* CmdCopyAccelerationStructureKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 268:
      /* CopyAccelerationStructureKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 269:
      /* CmdCopyAccelerationStructureToMemoryKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 270:
      /* CopyAccelerationStructureToMemoryKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 271:
      /* CmdCopyMemoryToAccelerationStructureKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 272:
      /* CopyMemoryToAccelerationStructureKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 273:
      /* CmdWriteAccelerationStructuresPropertiesKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 274:
      /* CmdWriteAccelerationStructuresPropertiesNV */
      if (!device || device->NV_ray_tracing) return true;
      return false;
   case 275:
      /* CmdBuildAccelerationStructureNV */
      if (!device || device->NV_ray_tracing) return true;
      return false;
   case 276:
      /* WriteAccelerationStructuresPropertiesKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 277:
      /* CmdTraceRaysKHR */
      if (!device || device->KHR_ray_tracing_pipeline) return true;
      return false;
   case 278:
      /* CmdTraceRaysNV */
      if (!device || device->NV_ray_tracing) return true;
      return false;
   case 279:
      /* GetRayTracingShaderGroupHandlesKHR */
      if (!device || device->KHR_ray_tracing_pipeline) return true;
      return false;
   case 280:
      /* GetRayTracingShaderGroupHandlesNV */
      if (!device || device->NV_ray_tracing) return true;
      return false;
   case 281:
      /* GetRayTracingCaptureReplayShaderGroupHandlesKHR */
      if (!device || device->KHR_ray_tracing_pipeline) return true;
      return false;
   case 282:
      /* GetAccelerationStructureHandleNV */
      if (!device || device->NV_ray_tracing) return true;
      return false;
   case 283:
      /* CreateRayTracingPipelinesNV */
      if (!device || device->NV_ray_tracing) return true;
      return false;
   case 284:
      /* CreateRayTracingPipelinesKHR */
      if (!device || device->KHR_ray_tracing_pipeline) return true;
      return false;
   case 285:
      /* CmdTraceRaysIndirectKHR */
      if (!device || device->KHR_ray_tracing_pipeline) return true;
      return false;
   case 286:
      /* GetDeviceAccelerationStructureCompatibilityKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 287:
      /* GetRayTracingShaderGroupStackSizeKHR */
      if (!device || device->KHR_ray_tracing_pipeline) return true;
      return false;
   case 288:
      /* CmdSetRayTracingPipelineStackSizeKHR */
      if (!device || device->KHR_ray_tracing_pipeline) return true;
      return false;
   case 289:
      /* GetImageViewHandleNVX */
      if (!device || device->NVX_image_view_handle) return true;
      return false;
   case 290:
      /* GetImageViewAddressNVX */
      if (!device || device->NVX_image_view_handle) return true;
      return false;
   case 291:
      /* GetDeviceGroupSurfacePresentModes2EXT */
      if (!device || device->EXT_full_screen_exclusive) return true;
      if (!device || device->EXT_full_screen_exclusive) return true;
      return false;
   case 292:
      /* AcquireFullScreenExclusiveModeEXT */
      if (!device || device->EXT_full_screen_exclusive) return true;
      return false;
   case 293:
      /* ReleaseFullScreenExclusiveModeEXT */
      if (!device || device->EXT_full_screen_exclusive) return true;
      return false;
   case 294:
      /* AcquireProfilingLockKHR */
      if (!device || device->KHR_performance_query) return true;
      return false;
   case 295:
      /* ReleaseProfilingLockKHR */
      if (!device || device->KHR_performance_query) return true;
      return false;
   case 296:
      /* GetImageDrmFormatModifierPropertiesEXT */
      if (!device || device->EXT_image_drm_format_modifier) return true;
      return false;
   case 297:
      /* GetBufferOpaqueCaptureAddress */
      return VK_MAKE_VERSION(1, 2, 0) <= core_version;
   case 298:
      /* GetBufferOpaqueCaptureAddressKHR */
      if (!device || device->KHR_buffer_device_address) return true;
      return false;
   case 299:
      /* GetBufferDeviceAddress */
      return VK_MAKE_VERSION(1, 2, 0) <= core_version;
   case 300:
      /* GetBufferDeviceAddressKHR */
      if (!device || device->KHR_buffer_device_address) return true;
      return false;
   case 301:
      /* GetBufferDeviceAddressEXT */
      if (!device || device->EXT_buffer_device_address) return true;
      return false;
   case 302:
      /* InitializePerformanceApiINTEL */
      if (!device || device->INTEL_performance_query) return true;
      return false;
   case 303:
      /* UninitializePerformanceApiINTEL */
      if (!device || device->INTEL_performance_query) return true;
      return false;
   case 304:
      /* CmdSetPerformanceMarkerINTEL */
      if (!device || device->INTEL_performance_query) return true;
      return false;
   case 305:
      /* CmdSetPerformanceStreamMarkerINTEL */
      if (!device || device->INTEL_performance_query) return true;
      return false;
   case 306:
      /* CmdSetPerformanceOverrideINTEL */
      if (!device || device->INTEL_performance_query) return true;
      return false;
   case 307:
      /* AcquirePerformanceConfigurationINTEL */
      if (!device || device->INTEL_performance_query) return true;
      return false;
   case 308:
      /* ReleasePerformanceConfigurationINTEL */
      if (!device || device->INTEL_performance_query) return true;
      return false;
   case 309:
      /* QueueSetPerformanceConfigurationINTEL */
      if (!device || device->INTEL_performance_query) return true;
      return false;
   case 310:
      /* GetPerformanceParameterINTEL */
      if (!device || device->INTEL_performance_query) return true;
      return false;
   case 311:
      /* GetDeviceMemoryOpaqueCaptureAddress */
      return VK_MAKE_VERSION(1, 2, 0) <= core_version;
   case 312:
      /* GetDeviceMemoryOpaqueCaptureAddressKHR */
      if (!device || device->KHR_buffer_device_address) return true;
      return false;
   case 313:
      /* GetPipelineExecutablePropertiesKHR */
      if (!device || device->KHR_pipeline_executable_properties) return true;
      return false;
   case 314:
      /* GetPipelineExecutableStatisticsKHR */
      if (!device || device->KHR_pipeline_executable_properties) return true;
      return false;
   case 315:
      /* GetPipelineExecutableInternalRepresentationsKHR */
      if (!device || device->KHR_pipeline_executable_properties) return true;
      return false;
   case 316:
      /* CmdSetLineStippleEXT */
      if (!device || device->EXT_line_rasterization) return true;
      return false;
   case 317:
      /* CreateAccelerationStructureKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 318:
      /* CmdBuildAccelerationStructuresKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 319:
      /* CmdBuildAccelerationStructuresIndirectKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 320:
      /* BuildAccelerationStructuresKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 321:
      /* GetAccelerationStructureDeviceAddressKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   case 322:
      /* CreateDeferredOperationKHR */
      if (!device || device->KHR_deferred_host_operations) return true;
      return false;
   case 323:
      /* DestroyDeferredOperationKHR */
      if (!device || device->KHR_deferred_host_operations) return true;
      return false;
   case 324:
      /* GetDeferredOperationMaxConcurrencyKHR */
      if (!device || device->KHR_deferred_host_operations) return true;
      return false;
   case 325:
      /* GetDeferredOperationResultKHR */
      if (!device || device->KHR_deferred_host_operations) return true;
      return false;
   case 326:
      /* DeferredOperationJoinKHR */
      if (!device || device->KHR_deferred_host_operations) return true;
      return false;
   case 327:
      /* CmdSetCullModeEXT */
      if (!device || device->EXT_extended_dynamic_state) return true;
      return false;
   case 328:
      /* CmdSetFrontFaceEXT */
      if (!device || device->EXT_extended_dynamic_state) return true;
      return false;
   case 329:
      /* CmdSetPrimitiveTopologyEXT */
      if (!device || device->EXT_extended_dynamic_state) return true;
      return false;
   case 330:
      /* CmdSetViewportWithCountEXT */
      if (!device || device->EXT_extended_dynamic_state) return true;
      return false;
   case 331:
      /* CmdSetScissorWithCountEXT */
      if (!device || device->EXT_extended_dynamic_state) return true;
      return false;
   case 332:
      /* CmdBindVertexBuffers2EXT */
      if (!device || device->EXT_extended_dynamic_state) return true;
      return false;
   case 333:
      /* CmdSetDepthTestEnableEXT */
      if (!device || device->EXT_extended_dynamic_state) return true;
      return false;
   case 334:
      /* CmdSetDepthWriteEnableEXT */
      if (!device || device->EXT_extended_dynamic_state) return true;
      return false;
   case 335:
      /* CmdSetDepthCompareOpEXT */
      if (!device || device->EXT_extended_dynamic_state) return true;
      return false;
   case 336:
      /* CmdSetDepthBoundsTestEnableEXT */
      if (!device || device->EXT_extended_dynamic_state) return true;
      return false;
   case 337:
      /* CmdSetStencilTestEnableEXT */
      if (!device || device->EXT_extended_dynamic_state) return true;
      return false;
   case 338:
      /* CmdSetStencilOpEXT */
      if (!device || device->EXT_extended_dynamic_state) return true;
      return false;
   case 339:
      /* CreatePrivateDataSlotEXT */
      if (!device || device->EXT_private_data) return true;
      return false;
   case 340:
      /* DestroyPrivateDataSlotEXT */
      if (!device || device->EXT_private_data) return true;
      return false;
   case 341:
      /* SetPrivateDataEXT */
      if (!device || device->EXT_private_data) return true;
      return false;
   case 342:
      /* GetPrivateDataEXT */
      if (!device || device->EXT_private_data) return true;
      return false;
   case 343:
      /* CmdCopyBuffer2KHR */
      if (!device || device->KHR_copy_commands2) return true;
      return false;
   case 344:
      /* CmdCopyImage2KHR */
      if (!device || device->KHR_copy_commands2) return true;
      return false;
   case 345:
      /* CmdBlitImage2KHR */
      if (!device || device->KHR_copy_commands2) return true;
      return false;
   case 346:
      /* CmdCopyBufferToImage2KHR */
      if (!device || device->KHR_copy_commands2) return true;
      return false;
   case 347:
      /* CmdCopyImageToBuffer2KHR */
      if (!device || device->KHR_copy_commands2) return true;
      return false;
   case 348:
      /* CmdResolveImage2KHR */
      if (!device || device->KHR_copy_commands2) return true;
      return false;
   case 349:
      /* CmdSetFragmentShadingRateKHR */
      if (!device || device->KHR_fragment_shading_rate) return true;
      return false;
   case 350:
      /* CmdSetFragmentShadingRateEnumNV */
      if (!device || device->NV_fragment_shading_rate_enums) return true;
      return false;
   case 351:
      /* GetAccelerationStructureBuildSizesKHR */
      if (!device || device->KHR_acceleration_structure) return true;
      return false;
   default:
      return false;
   }
}




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
            const uintptr_t zero = 0;
            if (entry[i] == NULL || memcmp(entry[i], &zero, sizeof(zero)) == 0)
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
            if (disp[disp_index] == NULL)
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
            const uintptr_t zero = 0;
            if (entry[i] == NULL || memcmp(entry[i], &zero, sizeof(zero)) == 0)
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
            if (disp[disp_index] == NULL)
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
            const uintptr_t zero = 0;
            if (entry[i] == NULL || memcmp(entry[i], &zero, sizeof(zero)) == 0)
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
            if (disp[disp_index] == NULL)
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

static VKAPI_ATTR void VKAPI_CALL
vk_tramp_GetPhysicalDeviceProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties* pProperties)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    vk_physical_device->dispatch_table.GetPhysicalDeviceProperties(physicalDevice, pProperties);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_GetPhysicalDeviceQueueFamilyProperties(VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties* pQueueFamilyProperties)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    vk_physical_device->dispatch_table.GetPhysicalDeviceQueueFamilyProperties(physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_GetPhysicalDeviceMemoryProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties* pMemoryProperties)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    vk_physical_device->dispatch_table.GetPhysicalDeviceMemoryProperties(physicalDevice, pMemoryProperties);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_GetPhysicalDeviceFeatures(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures* pFeatures)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    vk_physical_device->dispatch_table.GetPhysicalDeviceFeatures(physicalDevice, pFeatures);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_GetPhysicalDeviceFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties* pFormatProperties)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    vk_physical_device->dispatch_table.GetPhysicalDeviceFormatProperties(physicalDevice, format, pFormatProperties);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetPhysicalDeviceImageFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkImageFormatProperties* pImageFormatProperties)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    return vk_physical_device->dispatch_table.GetPhysicalDeviceImageFormatProperties(physicalDevice, format, type, tiling, usage, flags, pImageFormatProperties);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_CreateDevice(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDevice* pDevice)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    return vk_physical_device->dispatch_table.CreateDevice(physicalDevice, pCreateInfo, pAllocator, pDevice);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_EnumerateDeviceLayerProperties(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkLayerProperties* pProperties)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    return vk_physical_device->dispatch_table.EnumerateDeviceLayerProperties(physicalDevice, pPropertyCount, pProperties);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_EnumerateDeviceExtensionProperties(VkPhysicalDevice physicalDevice, const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    return vk_physical_device->dispatch_table.EnumerateDeviceExtensionProperties(physicalDevice, pLayerName, pPropertyCount, pProperties);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_GetPhysicalDeviceSparseImageFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkSampleCountFlagBits samples, VkImageUsageFlags usage, VkImageTiling tiling, uint32_t* pPropertyCount, VkSparseImageFormatProperties* pProperties)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    vk_physical_device->dispatch_table.GetPhysicalDeviceSparseImageFormatProperties(physicalDevice, format, type, samples, usage, tiling, pPropertyCount, pProperties);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetPhysicalDeviceDisplayPropertiesKHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPropertiesKHR* pProperties)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    return vk_physical_device->dispatch_table.GetPhysicalDeviceDisplayPropertiesKHR(physicalDevice, pPropertyCount, pProperties);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetPhysicalDeviceDisplayPlanePropertiesKHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPlanePropertiesKHR* pProperties)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    return vk_physical_device->dispatch_table.GetPhysicalDeviceDisplayPlanePropertiesKHR(physicalDevice, pPropertyCount, pProperties);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetDisplayPlaneSupportedDisplaysKHR(VkPhysicalDevice physicalDevice, uint32_t planeIndex, uint32_t* pDisplayCount, VkDisplayKHR* pDisplays)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    return vk_physical_device->dispatch_table.GetDisplayPlaneSupportedDisplaysKHR(physicalDevice, planeIndex, pDisplayCount, pDisplays);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetDisplayModePropertiesKHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModePropertiesKHR* pProperties)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    return vk_physical_device->dispatch_table.GetDisplayModePropertiesKHR(physicalDevice, display, pPropertyCount, pProperties);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_CreateDisplayModeKHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, const VkDisplayModeCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDisplayModeKHR* pMode)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    return vk_physical_device->dispatch_table.CreateDisplayModeKHR(physicalDevice, display, pCreateInfo, pAllocator, pMode);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetDisplayPlaneCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkDisplayModeKHR mode, uint32_t planeIndex, VkDisplayPlaneCapabilitiesKHR* pCapabilities)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    return vk_physical_device->dispatch_table.GetDisplayPlaneCapabilitiesKHR(physicalDevice, mode, planeIndex, pCapabilities);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetPhysicalDeviceSurfaceSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    return vk_physical_device->dispatch_table.GetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyIndex, surface, pSupported);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetPhysicalDeviceSurfaceCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR* pSurfaceCapabilities)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    return vk_physical_device->dispatch_table.GetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, pSurfaceCapabilities);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetPhysicalDeviceSurfaceFormatsKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pSurfaceFormatCount, VkSurfaceFormatKHR* pSurfaceFormats)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    return vk_physical_device->dispatch_table.GetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, pSurfaceFormatCount, pSurfaceFormats);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetPhysicalDeviceSurfacePresentModesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    return vk_physical_device->dispatch_table.GetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, pPresentModeCount, pPresentModes);
}
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
static VKAPI_ATTR VkBool32 VKAPI_CALL
vk_tramp_GetPhysicalDeviceWaylandPresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, struct wl_display* display)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    return vk_physical_device->dispatch_table.GetPhysicalDeviceWaylandPresentationSupportKHR(physicalDevice, queueFamilyIndex, display);
}
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
static VKAPI_ATTR VkBool32 VKAPI_CALL
vk_tramp_GetPhysicalDeviceWin32PresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    return vk_physical_device->dispatch_table.GetPhysicalDeviceWin32PresentationSupportKHR(physicalDevice, queueFamilyIndex);
}
#endif
#ifdef VK_USE_PLATFORM_XLIB_KHR
static VKAPI_ATTR VkBool32 VKAPI_CALL
vk_tramp_GetPhysicalDeviceXlibPresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, Display* dpy, VisualID visualID)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    return vk_physical_device->dispatch_table.GetPhysicalDeviceXlibPresentationSupportKHR(physicalDevice, queueFamilyIndex, dpy, visualID);
}
#endif
#ifdef VK_USE_PLATFORM_XCB_KHR
static VKAPI_ATTR VkBool32 VKAPI_CALL
vk_tramp_GetPhysicalDeviceXcbPresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, xcb_connection_t* connection, xcb_visualid_t visual_id)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    return vk_physical_device->dispatch_table.GetPhysicalDeviceXcbPresentationSupportKHR(physicalDevice, queueFamilyIndex, connection, visual_id);
}
#endif
#ifdef VK_USE_PLATFORM_DIRECTFB_EXT
static VKAPI_ATTR VkBool32 VKAPI_CALL
vk_tramp_GetPhysicalDeviceDirectFBPresentationSupportEXT(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, IDirectFB* dfb)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    return vk_physical_device->dispatch_table.GetPhysicalDeviceDirectFBPresentationSupportEXT(physicalDevice, queueFamilyIndex, dfb);
}
#endif
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetPhysicalDeviceExternalImageFormatPropertiesNV(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkExternalMemoryHandleTypeFlagsNV externalHandleType, VkExternalImageFormatPropertiesNV* pExternalImageFormatProperties)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    return vk_physical_device->dispatch_table.GetPhysicalDeviceExternalImageFormatPropertiesNV(physicalDevice, format, type, tiling, usage, flags, externalHandleType, pExternalImageFormatProperties);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_GetPhysicalDeviceFeatures2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    vk_physical_device->dispatch_table.GetPhysicalDeviceFeatures2(physicalDevice, pFeatures);
}
    static VKAPI_ATTR void VKAPI_CALL
vk_tramp_GetPhysicalDeviceProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    vk_physical_device->dispatch_table.GetPhysicalDeviceProperties2(physicalDevice, pProperties);
}
    static VKAPI_ATTR void VKAPI_CALL
vk_tramp_GetPhysicalDeviceFormatProperties2(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2* pFormatProperties)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    vk_physical_device->dispatch_table.GetPhysicalDeviceFormatProperties2(physicalDevice, format, pFormatProperties);
}
    static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetPhysicalDeviceImageFormatProperties2(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    return vk_physical_device->dispatch_table.GetPhysicalDeviceImageFormatProperties2(physicalDevice, pImageFormatInfo, pImageFormatProperties);
}
    static VKAPI_ATTR void VKAPI_CALL
vk_tramp_GetPhysicalDeviceQueueFamilyProperties2(VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties2* pQueueFamilyProperties)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    vk_physical_device->dispatch_table.GetPhysicalDeviceQueueFamilyProperties2(physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
}
    static VKAPI_ATTR void VKAPI_CALL
vk_tramp_GetPhysicalDeviceMemoryProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    vk_physical_device->dispatch_table.GetPhysicalDeviceMemoryProperties2(physicalDevice, pMemoryProperties);
}
    static VKAPI_ATTR void VKAPI_CALL
vk_tramp_GetPhysicalDeviceSparseImageFormatProperties2(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo, uint32_t* pPropertyCount, VkSparseImageFormatProperties2* pProperties)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    vk_physical_device->dispatch_table.GetPhysicalDeviceSparseImageFormatProperties2(physicalDevice, pFormatInfo, pPropertyCount, pProperties);
}
    static VKAPI_ATTR void VKAPI_CALL
vk_tramp_GetPhysicalDeviceExternalBufferProperties(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    vk_physical_device->dispatch_table.GetPhysicalDeviceExternalBufferProperties(physicalDevice, pExternalBufferInfo, pExternalBufferProperties);
}
    static VKAPI_ATTR void VKAPI_CALL
vk_tramp_GetPhysicalDeviceExternalSemaphoreProperties(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    vk_physical_device->dispatch_table.GetPhysicalDeviceExternalSemaphoreProperties(physicalDevice, pExternalSemaphoreInfo, pExternalSemaphoreProperties);
}
    static VKAPI_ATTR void VKAPI_CALL
vk_tramp_GetPhysicalDeviceExternalFenceProperties(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    vk_physical_device->dispatch_table.GetPhysicalDeviceExternalFenceProperties(physicalDevice, pExternalFenceInfo, pExternalFenceProperties);
}
    static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_ReleaseDisplayEXT(VkPhysicalDevice physicalDevice, VkDisplayKHR display)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    return vk_physical_device->dispatch_table.ReleaseDisplayEXT(physicalDevice, display);
}
#ifdef VK_USE_PLATFORM_XLIB_XRANDR_EXT
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_AcquireXlibDisplayEXT(VkPhysicalDevice physicalDevice, Display* dpy, VkDisplayKHR display)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    return vk_physical_device->dispatch_table.AcquireXlibDisplayEXT(physicalDevice, dpy, display);
}
#endif
#ifdef VK_USE_PLATFORM_XLIB_XRANDR_EXT
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetRandROutputDisplayEXT(VkPhysicalDevice physicalDevice, Display* dpy, RROutput rrOutput, VkDisplayKHR* pDisplay)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    return vk_physical_device->dispatch_table.GetRandROutputDisplayEXT(physicalDevice, dpy, rrOutput, pDisplay);
}
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_AcquireWinrtDisplayNV(VkPhysicalDevice physicalDevice, VkDisplayKHR display)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    return vk_physical_device->dispatch_table.AcquireWinrtDisplayNV(physicalDevice, display);
}
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetWinrtDisplayNV(VkPhysicalDevice physicalDevice, uint32_t deviceRelativeId, VkDisplayKHR* pDisplay)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    return vk_physical_device->dispatch_table.GetWinrtDisplayNV(physicalDevice, deviceRelativeId, pDisplay);
}
#endif
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetPhysicalDeviceSurfaceCapabilities2EXT(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilities2EXT* pSurfaceCapabilities)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    return vk_physical_device->dispatch_table.GetPhysicalDeviceSurfaceCapabilities2EXT(physicalDevice, surface, pSurfaceCapabilities);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetPhysicalDevicePresentRectanglesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pRectCount, VkRect2D* pRects)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    return vk_physical_device->dispatch_table.GetPhysicalDevicePresentRectanglesKHR(physicalDevice, surface, pRectCount, pRects);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_GetPhysicalDeviceMultisamplePropertiesEXT(VkPhysicalDevice physicalDevice, VkSampleCountFlagBits samples, VkMultisamplePropertiesEXT* pMultisampleProperties)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    vk_physical_device->dispatch_table.GetPhysicalDeviceMultisamplePropertiesEXT(physicalDevice, samples, pMultisampleProperties);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetPhysicalDeviceSurfaceCapabilities2KHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkSurfaceCapabilities2KHR* pSurfaceCapabilities)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    return vk_physical_device->dispatch_table.GetPhysicalDeviceSurfaceCapabilities2KHR(physicalDevice, pSurfaceInfo, pSurfaceCapabilities);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetPhysicalDeviceSurfaceFormats2KHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pSurfaceFormatCount, VkSurfaceFormat2KHR* pSurfaceFormats)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    return vk_physical_device->dispatch_table.GetPhysicalDeviceSurfaceFormats2KHR(physicalDevice, pSurfaceInfo, pSurfaceFormatCount, pSurfaceFormats);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetPhysicalDeviceDisplayProperties2KHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayProperties2KHR* pProperties)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    return vk_physical_device->dispatch_table.GetPhysicalDeviceDisplayProperties2KHR(physicalDevice, pPropertyCount, pProperties);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetPhysicalDeviceDisplayPlaneProperties2KHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPlaneProperties2KHR* pProperties)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    return vk_physical_device->dispatch_table.GetPhysicalDeviceDisplayPlaneProperties2KHR(physicalDevice, pPropertyCount, pProperties);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetDisplayModeProperties2KHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModeProperties2KHR* pProperties)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    return vk_physical_device->dispatch_table.GetDisplayModeProperties2KHR(physicalDevice, display, pPropertyCount, pProperties);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetDisplayPlaneCapabilities2KHR(VkPhysicalDevice physicalDevice, const VkDisplayPlaneInfo2KHR* pDisplayPlaneInfo, VkDisplayPlaneCapabilities2KHR* pCapabilities)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    return vk_physical_device->dispatch_table.GetDisplayPlaneCapabilities2KHR(physicalDevice, pDisplayPlaneInfo, pCapabilities);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetPhysicalDeviceCalibrateableTimeDomainsEXT(VkPhysicalDevice physicalDevice, uint32_t* pTimeDomainCount, VkTimeDomainEXT* pTimeDomains)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    return vk_physical_device->dispatch_table.GetPhysicalDeviceCalibrateableTimeDomainsEXT(physicalDevice, pTimeDomainCount, pTimeDomains);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetPhysicalDeviceCooperativeMatrixPropertiesNV(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkCooperativeMatrixPropertiesNV* pProperties)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    return vk_physical_device->dispatch_table.GetPhysicalDeviceCooperativeMatrixPropertiesNV(physicalDevice, pPropertyCount, pProperties);
}
#ifdef VK_USE_PLATFORM_WIN32_KHR
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetPhysicalDeviceSurfacePresentModes2EXT(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    return vk_physical_device->dispatch_table.GetPhysicalDeviceSurfacePresentModes2EXT(physicalDevice, pSurfaceInfo, pPresentModeCount, pPresentModes);
}
#endif
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_EnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, uint32_t* pCounterCount, VkPerformanceCounterKHR* pCounters, VkPerformanceCounterDescriptionKHR* pCounterDescriptions)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    return vk_physical_device->dispatch_table.EnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR(physicalDevice, queueFamilyIndex, pCounterCount, pCounters, pCounterDescriptions);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_GetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR(VkPhysicalDevice physicalDevice, const VkQueryPoolPerformanceCreateInfoKHR* pPerformanceQueryCreateInfo, uint32_t* pNumPasses)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    vk_physical_device->dispatch_table.GetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR(physicalDevice, pPerformanceQueryCreateInfo, pNumPasses);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV(VkPhysicalDevice physicalDevice, uint32_t* pCombinationCount, VkFramebufferMixedSamplesCombinationNV* pCombinations)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    return vk_physical_device->dispatch_table.GetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV(physicalDevice, pCombinationCount, pCombinations);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetPhysicalDeviceToolPropertiesEXT(VkPhysicalDevice physicalDevice, uint32_t* pToolCount, VkPhysicalDeviceToolPropertiesEXT* pToolProperties)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    return vk_physical_device->dispatch_table.GetPhysicalDeviceToolPropertiesEXT(physicalDevice, pToolCount, pToolProperties);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetPhysicalDeviceFragmentShadingRatesKHR(VkPhysicalDevice physicalDevice, uint32_t* pFragmentShadingRateCount, VkPhysicalDeviceFragmentShadingRateKHR* pFragmentShadingRates)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    return vk_physical_device->dispatch_table.GetPhysicalDeviceFragmentShadingRatesKHR(physicalDevice, pFragmentShadingRateCount, pFragmentShadingRates);
}

struct vk_physical_device_dispatch_table vk_physical_device_trampolines = {
    .GetPhysicalDeviceProperties = vk_tramp_GetPhysicalDeviceProperties,
    .GetPhysicalDeviceQueueFamilyProperties = vk_tramp_GetPhysicalDeviceQueueFamilyProperties,
    .GetPhysicalDeviceMemoryProperties = vk_tramp_GetPhysicalDeviceMemoryProperties,
    .GetPhysicalDeviceFeatures = vk_tramp_GetPhysicalDeviceFeatures,
    .GetPhysicalDeviceFormatProperties = vk_tramp_GetPhysicalDeviceFormatProperties,
    .GetPhysicalDeviceImageFormatProperties = vk_tramp_GetPhysicalDeviceImageFormatProperties,
    .CreateDevice = vk_tramp_CreateDevice,
    .EnumerateDeviceLayerProperties = vk_tramp_EnumerateDeviceLayerProperties,
    .EnumerateDeviceExtensionProperties = vk_tramp_EnumerateDeviceExtensionProperties,
    .GetPhysicalDeviceSparseImageFormatProperties = vk_tramp_GetPhysicalDeviceSparseImageFormatProperties,
    .GetPhysicalDeviceDisplayPropertiesKHR = vk_tramp_GetPhysicalDeviceDisplayPropertiesKHR,
    .GetPhysicalDeviceDisplayPlanePropertiesKHR = vk_tramp_GetPhysicalDeviceDisplayPlanePropertiesKHR,
    .GetDisplayPlaneSupportedDisplaysKHR = vk_tramp_GetDisplayPlaneSupportedDisplaysKHR,
    .GetDisplayModePropertiesKHR = vk_tramp_GetDisplayModePropertiesKHR,
    .CreateDisplayModeKHR = vk_tramp_CreateDisplayModeKHR,
    .GetDisplayPlaneCapabilitiesKHR = vk_tramp_GetDisplayPlaneCapabilitiesKHR,
    .GetPhysicalDeviceSurfaceSupportKHR = vk_tramp_GetPhysicalDeviceSurfaceSupportKHR,
    .GetPhysicalDeviceSurfaceCapabilitiesKHR = vk_tramp_GetPhysicalDeviceSurfaceCapabilitiesKHR,
    .GetPhysicalDeviceSurfaceFormatsKHR = vk_tramp_GetPhysicalDeviceSurfaceFormatsKHR,
    .GetPhysicalDeviceSurfacePresentModesKHR = vk_tramp_GetPhysicalDeviceSurfacePresentModesKHR,
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
    .GetPhysicalDeviceWaylandPresentationSupportKHR = vk_tramp_GetPhysicalDeviceWaylandPresentationSupportKHR,
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
    .GetPhysicalDeviceWin32PresentationSupportKHR = vk_tramp_GetPhysicalDeviceWin32PresentationSupportKHR,
#endif
#ifdef VK_USE_PLATFORM_XLIB_KHR
    .GetPhysicalDeviceXlibPresentationSupportKHR = vk_tramp_GetPhysicalDeviceXlibPresentationSupportKHR,
#endif
#ifdef VK_USE_PLATFORM_XCB_KHR
    .GetPhysicalDeviceXcbPresentationSupportKHR = vk_tramp_GetPhysicalDeviceXcbPresentationSupportKHR,
#endif
#ifdef VK_USE_PLATFORM_DIRECTFB_EXT
    .GetPhysicalDeviceDirectFBPresentationSupportEXT = vk_tramp_GetPhysicalDeviceDirectFBPresentationSupportEXT,
#endif
    .GetPhysicalDeviceExternalImageFormatPropertiesNV = vk_tramp_GetPhysicalDeviceExternalImageFormatPropertiesNV,
    .GetPhysicalDeviceFeatures2 = vk_tramp_GetPhysicalDeviceFeatures2,
        .GetPhysicalDeviceProperties2 = vk_tramp_GetPhysicalDeviceProperties2,
        .GetPhysicalDeviceFormatProperties2 = vk_tramp_GetPhysicalDeviceFormatProperties2,
        .GetPhysicalDeviceImageFormatProperties2 = vk_tramp_GetPhysicalDeviceImageFormatProperties2,
        .GetPhysicalDeviceQueueFamilyProperties2 = vk_tramp_GetPhysicalDeviceQueueFamilyProperties2,
        .GetPhysicalDeviceMemoryProperties2 = vk_tramp_GetPhysicalDeviceMemoryProperties2,
        .GetPhysicalDeviceSparseImageFormatProperties2 = vk_tramp_GetPhysicalDeviceSparseImageFormatProperties2,
        .GetPhysicalDeviceExternalBufferProperties = vk_tramp_GetPhysicalDeviceExternalBufferProperties,
        .GetPhysicalDeviceExternalSemaphoreProperties = vk_tramp_GetPhysicalDeviceExternalSemaphoreProperties,
        .GetPhysicalDeviceExternalFenceProperties = vk_tramp_GetPhysicalDeviceExternalFenceProperties,
        .ReleaseDisplayEXT = vk_tramp_ReleaseDisplayEXT,
#ifdef VK_USE_PLATFORM_XLIB_XRANDR_EXT
    .AcquireXlibDisplayEXT = vk_tramp_AcquireXlibDisplayEXT,
#endif
#ifdef VK_USE_PLATFORM_XLIB_XRANDR_EXT
    .GetRandROutputDisplayEXT = vk_tramp_GetRandROutputDisplayEXT,
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
    .AcquireWinrtDisplayNV = vk_tramp_AcquireWinrtDisplayNV,
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
    .GetWinrtDisplayNV = vk_tramp_GetWinrtDisplayNV,
#endif
    .GetPhysicalDeviceSurfaceCapabilities2EXT = vk_tramp_GetPhysicalDeviceSurfaceCapabilities2EXT,
    .GetPhysicalDevicePresentRectanglesKHR = vk_tramp_GetPhysicalDevicePresentRectanglesKHR,
    .GetPhysicalDeviceMultisamplePropertiesEXT = vk_tramp_GetPhysicalDeviceMultisamplePropertiesEXT,
    .GetPhysicalDeviceSurfaceCapabilities2KHR = vk_tramp_GetPhysicalDeviceSurfaceCapabilities2KHR,
    .GetPhysicalDeviceSurfaceFormats2KHR = vk_tramp_GetPhysicalDeviceSurfaceFormats2KHR,
    .GetPhysicalDeviceDisplayProperties2KHR = vk_tramp_GetPhysicalDeviceDisplayProperties2KHR,
    .GetPhysicalDeviceDisplayPlaneProperties2KHR = vk_tramp_GetPhysicalDeviceDisplayPlaneProperties2KHR,
    .GetDisplayModeProperties2KHR = vk_tramp_GetDisplayModeProperties2KHR,
    .GetDisplayPlaneCapabilities2KHR = vk_tramp_GetDisplayPlaneCapabilities2KHR,
    .GetPhysicalDeviceCalibrateableTimeDomainsEXT = vk_tramp_GetPhysicalDeviceCalibrateableTimeDomainsEXT,
    .GetPhysicalDeviceCooperativeMatrixPropertiesNV = vk_tramp_GetPhysicalDeviceCooperativeMatrixPropertiesNV,
#ifdef VK_USE_PLATFORM_WIN32_KHR
    .GetPhysicalDeviceSurfacePresentModes2EXT = vk_tramp_GetPhysicalDeviceSurfacePresentModes2EXT,
#endif
    .EnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR = vk_tramp_EnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR,
    .GetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR = vk_tramp_GetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR,
    .GetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV = vk_tramp_GetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV,
    .GetPhysicalDeviceToolPropertiesEXT = vk_tramp_GetPhysicalDeviceToolPropertiesEXT,
    .GetPhysicalDeviceFragmentShadingRatesKHR = vk_tramp_GetPhysicalDeviceFragmentShadingRatesKHR,
};

static VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL
vk_tramp_GetDeviceProcAddr(VkDevice device, const char* pName)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetDeviceProcAddr(device, pName);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_DestroyDevice(VkDevice device, const VkAllocationCallbacks* pAllocator)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.DestroyDevice(device, pAllocator);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_GetDeviceQueue(VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.GetDeviceQueue(device, queueFamilyIndex, queueIndex, pQueue);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_QueueSubmit(VkQueue queue, uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)queue;
    return vk_object->device->dispatch_table.QueueSubmit(queue, submitCount, pSubmits, fence);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_QueueWaitIdle(VkQueue queue)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)queue;
    return vk_object->device->dispatch_table.QueueWaitIdle(queue);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_DeviceWaitIdle(VkDevice device)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.DeviceWaitIdle(device);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_AllocateMemory(VkDevice device, const VkMemoryAllocateInfo* pAllocateInfo, const VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.AllocateMemory(device, pAllocateInfo, pAllocator, pMemory);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_FreeMemory(VkDevice device, VkDeviceMemory memory, const VkAllocationCallbacks* pAllocator)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.FreeMemory(device, memory, pAllocator);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_MapMemory(VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.MapMemory(device, memory, offset, size, flags, ppData);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_UnmapMemory(VkDevice device, VkDeviceMemory memory)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.UnmapMemory(device, memory);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_FlushMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.FlushMappedMemoryRanges(device, memoryRangeCount, pMemoryRanges);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_InvalidateMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.InvalidateMappedMemoryRanges(device, memoryRangeCount, pMemoryRanges);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_GetDeviceMemoryCommitment(VkDevice device, VkDeviceMemory memory, VkDeviceSize* pCommittedMemoryInBytes)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.GetDeviceMemoryCommitment(device, memory, pCommittedMemoryInBytes);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_GetBufferMemoryRequirements(VkDevice device, VkBuffer buffer, VkMemoryRequirements* pMemoryRequirements)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.GetBufferMemoryRequirements(device, buffer, pMemoryRequirements);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_BindBufferMemory(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.BindBufferMemory(device, buffer, memory, memoryOffset);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_GetImageMemoryRequirements(VkDevice device, VkImage image, VkMemoryRequirements* pMemoryRequirements)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.GetImageMemoryRequirements(device, image, pMemoryRequirements);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_BindImageMemory(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.BindImageMemory(device, image, memory, memoryOffset);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_GetImageSparseMemoryRequirements(VkDevice device, VkImage image, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements* pSparseMemoryRequirements)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.GetImageSparseMemoryRequirements(device, image, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_QueueBindSparse(VkQueue queue, uint32_t bindInfoCount, const VkBindSparseInfo* pBindInfo, VkFence fence)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)queue;
    return vk_object->device->dispatch_table.QueueBindSparse(queue, bindInfoCount, pBindInfo, fence);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_CreateFence(VkDevice device, const VkFenceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.CreateFence(device, pCreateInfo, pAllocator, pFence);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_DestroyFence(VkDevice device, VkFence fence, const VkAllocationCallbacks* pAllocator)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.DestroyFence(device, fence, pAllocator);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_ResetFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.ResetFences(device, fenceCount, pFences);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetFenceStatus(VkDevice device, VkFence fence)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetFenceStatus(device, fence);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_WaitForFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences, VkBool32 waitAll, uint64_t timeout)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.WaitForFences(device, fenceCount, pFences, waitAll, timeout);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_CreateSemaphore(VkDevice device, const VkSemaphoreCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.CreateSemaphore(device, pCreateInfo, pAllocator, pSemaphore);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_DestroySemaphore(VkDevice device, VkSemaphore semaphore, const VkAllocationCallbacks* pAllocator)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.DestroySemaphore(device, semaphore, pAllocator);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_CreateEvent(VkDevice device, const VkEventCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkEvent* pEvent)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.CreateEvent(device, pCreateInfo, pAllocator, pEvent);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_DestroyEvent(VkDevice device, VkEvent event, const VkAllocationCallbacks* pAllocator)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.DestroyEvent(device, event, pAllocator);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetEventStatus(VkDevice device, VkEvent event)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetEventStatus(device, event);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_SetEvent(VkDevice device, VkEvent event)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.SetEvent(device, event);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_ResetEvent(VkDevice device, VkEvent event)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.ResetEvent(device, event);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_CreateQueryPool(VkDevice device, const VkQueryPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkQueryPool* pQueryPool)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.CreateQueryPool(device, pCreateInfo, pAllocator, pQueryPool);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_DestroyQueryPool(VkDevice device, VkQueryPool queryPool, const VkAllocationCallbacks* pAllocator)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.DestroyQueryPool(device, queryPool, pAllocator);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetQueryPoolResults(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, VkDeviceSize stride, VkQueryResultFlags flags)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetQueryPoolResults(device, queryPool, firstQuery, queryCount, dataSize, pData, stride, flags);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_ResetQueryPool(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.ResetQueryPool(device, queryPool, firstQuery, queryCount);
}
    static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_CreateBuffer(VkDevice device, const VkBufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.CreateBuffer(device, pCreateInfo, pAllocator, pBuffer);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_DestroyBuffer(VkDevice device, VkBuffer buffer, const VkAllocationCallbacks* pAllocator)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.DestroyBuffer(device, buffer, pAllocator);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_CreateBufferView(VkDevice device, const VkBufferViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferView* pView)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.CreateBufferView(device, pCreateInfo, pAllocator, pView);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_DestroyBufferView(VkDevice device, VkBufferView bufferView, const VkAllocationCallbacks* pAllocator)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.DestroyBufferView(device, bufferView, pAllocator);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_CreateImage(VkDevice device, const VkImageCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImage* pImage)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.CreateImage(device, pCreateInfo, pAllocator, pImage);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_DestroyImage(VkDevice device, VkImage image, const VkAllocationCallbacks* pAllocator)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.DestroyImage(device, image, pAllocator);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_GetImageSubresourceLayout(VkDevice device, VkImage image, const VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.GetImageSubresourceLayout(device, image, pSubresource, pLayout);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_CreateImageView(VkDevice device, const VkImageViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImageView* pView)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.CreateImageView(device, pCreateInfo, pAllocator, pView);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_DestroyImageView(VkDevice device, VkImageView imageView, const VkAllocationCallbacks* pAllocator)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.DestroyImageView(device, imageView, pAllocator);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_CreateShaderModule(VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.CreateShaderModule(device, pCreateInfo, pAllocator, pShaderModule);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_DestroyShaderModule(VkDevice device, VkShaderModule shaderModule, const VkAllocationCallbacks* pAllocator)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.DestroyShaderModule(device, shaderModule, pAllocator);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_CreatePipelineCache(VkDevice device, const VkPipelineCacheCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineCache* pPipelineCache)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.CreatePipelineCache(device, pCreateInfo, pAllocator, pPipelineCache);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_DestroyPipelineCache(VkDevice device, VkPipelineCache pipelineCache, const VkAllocationCallbacks* pAllocator)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.DestroyPipelineCache(device, pipelineCache, pAllocator);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetPipelineCacheData(VkDevice device, VkPipelineCache pipelineCache, size_t* pDataSize, void* pData)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetPipelineCacheData(device, pipelineCache, pDataSize, pData);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_MergePipelineCaches(VkDevice device, VkPipelineCache dstCache, uint32_t srcCacheCount, const VkPipelineCache* pSrcCaches)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.MergePipelineCaches(device, dstCache, srcCacheCount, pSrcCaches);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_CreateGraphicsPipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkGraphicsPipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.CreateGraphicsPipelines(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_CreateComputePipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkComputePipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.CreateComputePipelines(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_DestroyPipeline(VkDevice device, VkPipeline pipeline, const VkAllocationCallbacks* pAllocator)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.DestroyPipeline(device, pipeline, pAllocator);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_CreatePipelineLayout(VkDevice device, const VkPipelineLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.CreatePipelineLayout(device, pCreateInfo, pAllocator, pPipelineLayout);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_DestroyPipelineLayout(VkDevice device, VkPipelineLayout pipelineLayout, const VkAllocationCallbacks* pAllocator)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.DestroyPipelineLayout(device, pipelineLayout, pAllocator);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_CreateSampler(VkDevice device, const VkSamplerCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSampler* pSampler)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.CreateSampler(device, pCreateInfo, pAllocator, pSampler);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_DestroySampler(VkDevice device, VkSampler sampler, const VkAllocationCallbacks* pAllocator)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.DestroySampler(device, sampler, pAllocator);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_CreateDescriptorSetLayout(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorSetLayout* pSetLayout)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.CreateDescriptorSetLayout(device, pCreateInfo, pAllocator, pSetLayout);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_DestroyDescriptorSetLayout(VkDevice device, VkDescriptorSetLayout descriptorSetLayout, const VkAllocationCallbacks* pAllocator)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.DestroyDescriptorSetLayout(device, descriptorSetLayout, pAllocator);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_CreateDescriptorPool(VkDevice device, const VkDescriptorPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorPool* pDescriptorPool)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.CreateDescriptorPool(device, pCreateInfo, pAllocator, pDescriptorPool);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_DestroyDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, const VkAllocationCallbacks* pAllocator)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.DestroyDescriptorPool(device, descriptorPool, pAllocator);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_ResetDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags flags)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.ResetDescriptorPool(device, descriptorPool, flags);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_AllocateDescriptorSets(VkDevice device, const VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.AllocateDescriptorSets(device, pAllocateInfo, pDescriptorSets);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_FreeDescriptorSets(VkDevice device, VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.FreeDescriptorSets(device, descriptorPool, descriptorSetCount, pDescriptorSets);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_UpdateDescriptorSets(VkDevice device, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, const VkCopyDescriptorSet* pDescriptorCopies)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.UpdateDescriptorSets(device, descriptorWriteCount, pDescriptorWrites, descriptorCopyCount, pDescriptorCopies);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_CreateFramebuffer(VkDevice device, const VkFramebufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFramebuffer* pFramebuffer)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.CreateFramebuffer(device, pCreateInfo, pAllocator, pFramebuffer);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_DestroyFramebuffer(VkDevice device, VkFramebuffer framebuffer, const VkAllocationCallbacks* pAllocator)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.DestroyFramebuffer(device, framebuffer, pAllocator);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_CreateRenderPass(VkDevice device, const VkRenderPassCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.CreateRenderPass(device, pCreateInfo, pAllocator, pRenderPass);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_DestroyRenderPass(VkDevice device, VkRenderPass renderPass, const VkAllocationCallbacks* pAllocator)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.DestroyRenderPass(device, renderPass, pAllocator);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_GetRenderAreaGranularity(VkDevice device, VkRenderPass renderPass, VkExtent2D* pGranularity)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.GetRenderAreaGranularity(device, renderPass, pGranularity);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_CreateCommandPool(VkDevice device, const VkCommandPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.CreateCommandPool(device, pCreateInfo, pAllocator, pCommandPool);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_DestroyCommandPool(VkDevice device, VkCommandPool commandPool, const VkAllocationCallbacks* pAllocator)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.DestroyCommandPool(device, commandPool, pAllocator);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_ResetCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolResetFlags flags)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.ResetCommandPool(device, commandPool, flags);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_AllocateCommandBuffers(VkDevice device, const VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.AllocateCommandBuffers(device, pAllocateInfo, pCommandBuffers);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_FreeCommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.FreeCommandBuffers(device, commandPool, commandBufferCount, pCommandBuffers);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_BeginCommandBuffer(VkCommandBuffer commandBuffer, const VkCommandBufferBeginInfo* pBeginInfo)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    return vk_object->device->dispatch_table.BeginCommandBuffer(commandBuffer, pBeginInfo);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_EndCommandBuffer(VkCommandBuffer commandBuffer)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    return vk_object->device->dispatch_table.EndCommandBuffer(commandBuffer);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_ResetCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    return vk_object->device->dispatch_table.ResetCommandBuffer(commandBuffer, flags);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdBindPipeline(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdBindPipeline(commandBuffer, pipelineBindPoint, pipeline);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetViewport(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pViewports)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetViewport(commandBuffer, firstViewport, viewportCount, pViewports);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetScissor(VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pScissors)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetScissor(commandBuffer, firstScissor, scissorCount, pScissors);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetLineWidth(VkCommandBuffer commandBuffer, float lineWidth)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetLineWidth(commandBuffer, lineWidth);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetDepthBias(VkCommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetDepthBias(commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetBlendConstants(VkCommandBuffer commandBuffer, const float blendConstants[4])
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetBlendConstants(commandBuffer, blendConstants);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetDepthBounds(VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetDepthBounds(commandBuffer, minDepthBounds, maxDepthBounds);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetStencilCompareMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetStencilCompareMask(commandBuffer, faceMask, compareMask);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetStencilWriteMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetStencilWriteMask(commandBuffer, faceMask, writeMask);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetStencilReference(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetStencilReference(commandBuffer, faceMask, reference);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdBindDescriptorSets(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdBindDescriptorSets(commandBuffer, pipelineBindPoint, layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdBindIndexBuffer(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdBindIndexBuffer(commandBuffer, buffer, offset, indexType);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdBindVertexBuffers(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdBindVertexBuffers(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdDraw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdDraw(commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdDrawIndexed(VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdDrawIndexed(commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdDrawIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdDrawIndirect(commandBuffer, buffer, offset, drawCount, stride);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdDrawIndexedIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdDrawIndexedIndirect(commandBuffer, buffer, offset, drawCount, stride);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdDispatch(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdDispatch(commandBuffer, groupCountX, groupCountY, groupCountZ);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdDispatchIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdDispatchIndirect(commandBuffer, buffer, offset);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdCopyBuffer(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, regionCount, pRegions);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdCopyImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdCopyImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdBlitImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdBlitImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions, filter);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdCopyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdCopyBufferToImage(commandBuffer, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdCopyImageToBuffer(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdCopyImageToBuffer(commandBuffer, srcImage, srcImageLayout, dstBuffer, regionCount, pRegions);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdUpdateBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdUpdateBuffer(commandBuffer, dstBuffer, dstOffset, dataSize, pData);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdFillBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdFillBuffer(commandBuffer, dstBuffer, dstOffset, size, data);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdClearColorImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount, const VkImageSubresourceRange* pRanges)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdClearColorImage(commandBuffer, image, imageLayout, pColor, rangeCount, pRanges);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdClearDepthStencilImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const VkImageSubresourceRange* pRanges)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdClearDepthStencilImage(commandBuffer, image, imageLayout, pDepthStencil, rangeCount, pRanges);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdClearAttachments(VkCommandBuffer commandBuffer, uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdClearAttachments(commandBuffer, attachmentCount, pAttachments, rectCount, pRects);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdResolveImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageResolve* pRegions)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdResolveImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetEvent(commandBuffer, event, stageMask);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdResetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdResetEvent(commandBuffer, event, stageMask);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdWaitEvents(VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdWaitEvents(commandBuffer, eventCount, pEvents, srcStageMask, dstStageMask, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdPipelineBarrier(VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdPipelineBarrier(commandBuffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdBeginQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdBeginQuery(commandBuffer, queryPool, query, flags);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdEndQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdEndQuery(commandBuffer, queryPool, query);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdBeginConditionalRenderingEXT(VkCommandBuffer commandBuffer, const VkConditionalRenderingBeginInfoEXT* pConditionalRenderingBegin)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdBeginConditionalRenderingEXT(commandBuffer, pConditionalRenderingBegin);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdEndConditionalRenderingEXT(VkCommandBuffer commandBuffer)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdEndConditionalRenderingEXT(commandBuffer);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdResetQueryPool(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdResetQueryPool(commandBuffer, queryPool, firstQuery, queryCount);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdWriteTimestamp(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdWriteTimestamp(commandBuffer, pipelineStage, queryPool, query);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdCopyQueryPoolResults(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdCopyQueryPoolResults(commandBuffer, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, flags);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdPushConstants(VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdPushConstants(commandBuffer, layout, stageFlags, offset, size, pValues);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdBeginRenderPass(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdBeginRenderPass(commandBuffer, pRenderPassBegin, contents);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdNextSubpass(VkCommandBuffer commandBuffer, VkSubpassContents contents)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdNextSubpass(commandBuffer, contents);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdEndRenderPass(VkCommandBuffer commandBuffer)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdEndRenderPass(commandBuffer);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdExecuteCommands(VkCommandBuffer commandBuffer, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdExecuteCommands(commandBuffer, commandBufferCount, pCommandBuffers);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_CreateSharedSwapchainsKHR(VkDevice device, uint32_t swapchainCount, const VkSwapchainCreateInfoKHR* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchains)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.CreateSharedSwapchainsKHR(device, swapchainCount, pCreateInfos, pAllocator, pSwapchains);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_CreateSwapchainKHR(VkDevice device, const VkSwapchainCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.CreateSwapchainKHR(device, pCreateInfo, pAllocator, pSwapchain);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_DestroySwapchainKHR(VkDevice device, VkSwapchainKHR swapchain, const VkAllocationCallbacks* pAllocator)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.DestroySwapchainKHR(device, swapchain, pAllocator);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetSwapchainImagesKHR(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetSwapchainImagesKHR(device, swapchain, pSwapchainImageCount, pSwapchainImages);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_AcquireNextImageKHR(VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.AcquireNextImageKHR(device, swapchain, timeout, semaphore, fence, pImageIndex);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_QueuePresentKHR(VkQueue queue, const VkPresentInfoKHR* pPresentInfo)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)queue;
    return vk_object->device->dispatch_table.QueuePresentKHR(queue, pPresentInfo);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_DebugMarkerSetObjectNameEXT(VkDevice device, const VkDebugMarkerObjectNameInfoEXT* pNameInfo)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.DebugMarkerSetObjectNameEXT(device, pNameInfo);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_DebugMarkerSetObjectTagEXT(VkDevice device, const VkDebugMarkerObjectTagInfoEXT* pTagInfo)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.DebugMarkerSetObjectTagEXT(device, pTagInfo);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdDebugMarkerBeginEXT(VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdDebugMarkerBeginEXT(commandBuffer, pMarkerInfo);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdDebugMarkerEndEXT(VkCommandBuffer commandBuffer)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdDebugMarkerEndEXT(commandBuffer);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdDebugMarkerInsertEXT(VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdDebugMarkerInsertEXT(commandBuffer, pMarkerInfo);
}
#ifdef VK_USE_PLATFORM_WIN32_KHR
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetMemoryWin32HandleNV(VkDevice device, VkDeviceMemory memory, VkExternalMemoryHandleTypeFlagsNV handleType, HANDLE* pHandle)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetMemoryWin32HandleNV(device, memory, handleType, pHandle);
}
#endif
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdExecuteGeneratedCommandsNV(VkCommandBuffer commandBuffer, VkBool32 isPreprocessed, const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdExecuteGeneratedCommandsNV(commandBuffer, isPreprocessed, pGeneratedCommandsInfo);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdPreprocessGeneratedCommandsNV(VkCommandBuffer commandBuffer, const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdPreprocessGeneratedCommandsNV(commandBuffer, pGeneratedCommandsInfo);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdBindPipelineShaderGroupNV(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline, uint32_t groupIndex)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdBindPipelineShaderGroupNV(commandBuffer, pipelineBindPoint, pipeline, groupIndex);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_GetGeneratedCommandsMemoryRequirementsNV(VkDevice device, const VkGeneratedCommandsMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2* pMemoryRequirements)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.GetGeneratedCommandsMemoryRequirementsNV(device, pInfo, pMemoryRequirements);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_CreateIndirectCommandsLayoutNV(VkDevice device, const VkIndirectCommandsLayoutCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkIndirectCommandsLayoutNV* pIndirectCommandsLayout)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.CreateIndirectCommandsLayoutNV(device, pCreateInfo, pAllocator, pIndirectCommandsLayout);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_DestroyIndirectCommandsLayoutNV(VkDevice device, VkIndirectCommandsLayoutNV indirectCommandsLayout, const VkAllocationCallbacks* pAllocator)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.DestroyIndirectCommandsLayoutNV(device, indirectCommandsLayout, pAllocator);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdPushDescriptorSetKHR(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdPushDescriptorSetKHR(commandBuffer, pipelineBindPoint, layout, set, descriptorWriteCount, pDescriptorWrites);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_TrimCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.TrimCommandPool(device, commandPool, flags);
}
    #ifdef VK_USE_PLATFORM_WIN32_KHR
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetMemoryWin32HandleKHR(VkDevice device, const VkMemoryGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetMemoryWin32HandleKHR(device, pGetWin32HandleInfo, pHandle);
}
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetMemoryWin32HandlePropertiesKHR(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, HANDLE handle, VkMemoryWin32HandlePropertiesKHR* pMemoryWin32HandleProperties)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetMemoryWin32HandlePropertiesKHR(device, handleType, handle, pMemoryWin32HandleProperties);
}
#endif
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetMemoryFdKHR(VkDevice device, const VkMemoryGetFdInfoKHR* pGetFdInfo, int* pFd)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetMemoryFdKHR(device, pGetFdInfo, pFd);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetMemoryFdPropertiesKHR(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, int fd, VkMemoryFdPropertiesKHR* pMemoryFdProperties)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetMemoryFdPropertiesKHR(device, handleType, fd, pMemoryFdProperties);
}
#ifdef VK_USE_PLATFORM_WIN32_KHR
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetSemaphoreWin32HandleKHR(VkDevice device, const VkSemaphoreGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetSemaphoreWin32HandleKHR(device, pGetWin32HandleInfo, pHandle);
}
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_ImportSemaphoreWin32HandleKHR(VkDevice device, const VkImportSemaphoreWin32HandleInfoKHR* pImportSemaphoreWin32HandleInfo)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.ImportSemaphoreWin32HandleKHR(device, pImportSemaphoreWin32HandleInfo);
}
#endif
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetSemaphoreFdKHR(VkDevice device, const VkSemaphoreGetFdInfoKHR* pGetFdInfo, int* pFd)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetSemaphoreFdKHR(device, pGetFdInfo, pFd);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_ImportSemaphoreFdKHR(VkDevice device, const VkImportSemaphoreFdInfoKHR* pImportSemaphoreFdInfo)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.ImportSemaphoreFdKHR(device, pImportSemaphoreFdInfo);
}
#ifdef VK_USE_PLATFORM_WIN32_KHR
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetFenceWin32HandleKHR(VkDevice device, const VkFenceGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetFenceWin32HandleKHR(device, pGetWin32HandleInfo, pHandle);
}
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_ImportFenceWin32HandleKHR(VkDevice device, const VkImportFenceWin32HandleInfoKHR* pImportFenceWin32HandleInfo)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.ImportFenceWin32HandleKHR(device, pImportFenceWin32HandleInfo);
}
#endif
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetFenceFdKHR(VkDevice device, const VkFenceGetFdInfoKHR* pGetFdInfo, int* pFd)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetFenceFdKHR(device, pGetFdInfo, pFd);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_ImportFenceFdKHR(VkDevice device, const VkImportFenceFdInfoKHR* pImportFenceFdInfo)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.ImportFenceFdKHR(device, pImportFenceFdInfo);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_DisplayPowerControlEXT(VkDevice device, VkDisplayKHR display, const VkDisplayPowerInfoEXT* pDisplayPowerInfo)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.DisplayPowerControlEXT(device, display, pDisplayPowerInfo);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_RegisterDeviceEventEXT(VkDevice device, const VkDeviceEventInfoEXT* pDeviceEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.RegisterDeviceEventEXT(device, pDeviceEventInfo, pAllocator, pFence);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_RegisterDisplayEventEXT(VkDevice device, VkDisplayKHR display, const VkDisplayEventInfoEXT* pDisplayEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.RegisterDisplayEventEXT(device, display, pDisplayEventInfo, pAllocator, pFence);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetSwapchainCounterEXT(VkDevice device, VkSwapchainKHR swapchain, VkSurfaceCounterFlagBitsEXT counter, uint64_t* pCounterValue)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetSwapchainCounterEXT(device, swapchain, counter, pCounterValue);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_GetDeviceGroupPeerMemoryFeatures(VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.GetDeviceGroupPeerMemoryFeatures(device, heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures);
}
    static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_BindBufferMemory2(VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.BindBufferMemory2(device, bindInfoCount, pBindInfos);
}
    static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_BindImageMemory2(VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.BindImageMemory2(device, bindInfoCount, pBindInfos);
}
    static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetDeviceMask(VkCommandBuffer commandBuffer, uint32_t deviceMask)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetDeviceMask(commandBuffer, deviceMask);
}
    static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetDeviceGroupPresentCapabilitiesKHR(VkDevice device, VkDeviceGroupPresentCapabilitiesKHR* pDeviceGroupPresentCapabilities)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetDeviceGroupPresentCapabilitiesKHR(device, pDeviceGroupPresentCapabilities);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetDeviceGroupSurfacePresentModesKHR(VkDevice device, VkSurfaceKHR surface, VkDeviceGroupPresentModeFlagsKHR* pModes)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetDeviceGroupSurfacePresentModesKHR(device, surface, pModes);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_AcquireNextImage2KHR(VkDevice device, const VkAcquireNextImageInfoKHR* pAcquireInfo, uint32_t* pImageIndex)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.AcquireNextImage2KHR(device, pAcquireInfo, pImageIndex);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdDispatchBase(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdDispatchBase(commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
}
    static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_CreateDescriptorUpdateTemplate(VkDevice device, const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.CreateDescriptorUpdateTemplate(device, pCreateInfo, pAllocator, pDescriptorUpdateTemplate);
}
    static VKAPI_ATTR void VKAPI_CALL
vk_tramp_DestroyDescriptorUpdateTemplate(VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.DestroyDescriptorUpdateTemplate(device, descriptorUpdateTemplate, pAllocator);
}
    static VKAPI_ATTR void VKAPI_CALL
vk_tramp_UpdateDescriptorSetWithTemplate(VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.UpdateDescriptorSetWithTemplate(device, descriptorSet, descriptorUpdateTemplate, pData);
}
    static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdPushDescriptorSetWithTemplateKHR(VkCommandBuffer commandBuffer, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, const void* pData)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdPushDescriptorSetWithTemplateKHR(commandBuffer, descriptorUpdateTemplate, layout, set, pData);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_SetHdrMetadataEXT(VkDevice device, uint32_t swapchainCount, const VkSwapchainKHR* pSwapchains, const VkHdrMetadataEXT* pMetadata)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.SetHdrMetadataEXT(device, swapchainCount, pSwapchains, pMetadata);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetSwapchainStatusKHR(VkDevice device, VkSwapchainKHR swapchain)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetSwapchainStatusKHR(device, swapchain);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetRefreshCycleDurationGOOGLE(VkDevice device, VkSwapchainKHR swapchain, VkRefreshCycleDurationGOOGLE* pDisplayTimingProperties)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetRefreshCycleDurationGOOGLE(device, swapchain, pDisplayTimingProperties);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetPastPresentationTimingGOOGLE(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pPresentationTimingCount, VkPastPresentationTimingGOOGLE* pPresentationTimings)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetPastPresentationTimingGOOGLE(device, swapchain, pPresentationTimingCount, pPresentationTimings);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetViewportWScalingNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewportWScalingNV* pViewportWScalings)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetViewportWScalingNV(commandBuffer, firstViewport, viewportCount, pViewportWScalings);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetDiscardRectangleEXT(VkCommandBuffer commandBuffer, uint32_t firstDiscardRectangle, uint32_t discardRectangleCount, const VkRect2D* pDiscardRectangles)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetDiscardRectangleEXT(commandBuffer, firstDiscardRectangle, discardRectangleCount, pDiscardRectangles);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetSampleLocationsEXT(VkCommandBuffer commandBuffer, const VkSampleLocationsInfoEXT* pSampleLocationsInfo)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetSampleLocationsEXT(commandBuffer, pSampleLocationsInfo);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_GetBufferMemoryRequirements2(VkDevice device, const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.GetBufferMemoryRequirements2(device, pInfo, pMemoryRequirements);
}
    static VKAPI_ATTR void VKAPI_CALL
vk_tramp_GetImageMemoryRequirements2(VkDevice device, const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.GetImageMemoryRequirements2(device, pInfo, pMemoryRequirements);
}
    static VKAPI_ATTR void VKAPI_CALL
vk_tramp_GetImageSparseMemoryRequirements2(VkDevice device, const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.GetImageSparseMemoryRequirements2(device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
}
    static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_CreateSamplerYcbcrConversion(VkDevice device, const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.CreateSamplerYcbcrConversion(device, pCreateInfo, pAllocator, pYcbcrConversion);
}
    static VKAPI_ATTR void VKAPI_CALL
vk_tramp_DestroySamplerYcbcrConversion(VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.DestroySamplerYcbcrConversion(device, ycbcrConversion, pAllocator);
}
    static VKAPI_ATTR void VKAPI_CALL
vk_tramp_GetDeviceQueue2(VkDevice device, const VkDeviceQueueInfo2* pQueueInfo, VkQueue* pQueue)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.GetDeviceQueue2(device, pQueueInfo, pQueue);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_CreateValidationCacheEXT(VkDevice device, const VkValidationCacheCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkValidationCacheEXT* pValidationCache)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.CreateValidationCacheEXT(device, pCreateInfo, pAllocator, pValidationCache);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_DestroyValidationCacheEXT(VkDevice device, VkValidationCacheEXT validationCache, const VkAllocationCallbacks* pAllocator)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.DestroyValidationCacheEXT(device, validationCache, pAllocator);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetValidationCacheDataEXT(VkDevice device, VkValidationCacheEXT validationCache, size_t* pDataSize, void* pData)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetValidationCacheDataEXT(device, validationCache, pDataSize, pData);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_MergeValidationCachesEXT(VkDevice device, VkValidationCacheEXT dstCache, uint32_t srcCacheCount, const VkValidationCacheEXT* pSrcCaches)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.MergeValidationCachesEXT(device, dstCache, srcCacheCount, pSrcCaches);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_GetDescriptorSetLayoutSupport(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.GetDescriptorSetLayoutSupport(device, pCreateInfo, pSupport);
}
    #ifdef VK_USE_PLATFORM_ANDROID_KHR
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetSwapchainGrallocUsageANDROID(VkDevice device, VkFormat format, VkImageUsageFlags imageUsage, int* grallocUsage)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetSwapchainGrallocUsageANDROID(device, format, imageUsage, grallocUsage);
}
#endif
#ifdef VK_USE_PLATFORM_ANDROID_KHR
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetSwapchainGrallocUsage2ANDROID(VkDevice device, VkFormat format, VkImageUsageFlags imageUsage, VkSwapchainImageUsageFlagsANDROID swapchainImageUsage, uint64_t* grallocConsumerUsage, uint64_t* grallocProducerUsage)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetSwapchainGrallocUsage2ANDROID(device, format, imageUsage, swapchainImageUsage, grallocConsumerUsage, grallocProducerUsage);
}
#endif
#ifdef VK_USE_PLATFORM_ANDROID_KHR
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_AcquireImageANDROID(VkDevice device, VkImage image, int nativeFenceFd, VkSemaphore semaphore, VkFence fence)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.AcquireImageANDROID(device, image, nativeFenceFd, semaphore, fence);
}
#endif
#ifdef VK_USE_PLATFORM_ANDROID_KHR
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_QueueSignalReleaseImageANDROID(VkQueue queue, uint32_t waitSemaphoreCount, const VkSemaphore* pWaitSemaphores, VkImage image, int* pNativeFenceFd)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)queue;
    return vk_object->device->dispatch_table.QueueSignalReleaseImageANDROID(queue, waitSemaphoreCount, pWaitSemaphores, image, pNativeFenceFd);
}
#endif
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetShaderInfoAMD(VkDevice device, VkPipeline pipeline, VkShaderStageFlagBits shaderStage, VkShaderInfoTypeAMD infoType, size_t* pInfoSize, void* pInfo)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetShaderInfoAMD(device, pipeline, shaderStage, infoType, pInfoSize, pInfo);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_SetLocalDimmingAMD(VkDevice device, VkSwapchainKHR swapChain, VkBool32 localDimmingEnable)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.SetLocalDimmingAMD(device, swapChain, localDimmingEnable);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetCalibratedTimestampsEXT(VkDevice device, uint32_t timestampCount, const VkCalibratedTimestampInfoEXT* pTimestampInfos, uint64_t* pTimestamps, uint64_t* pMaxDeviation)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetCalibratedTimestampsEXT(device, timestampCount, pTimestampInfos, pTimestamps, pMaxDeviation);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_SetDebugUtilsObjectNameEXT(VkDevice device, const VkDebugUtilsObjectNameInfoEXT* pNameInfo)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.SetDebugUtilsObjectNameEXT(device, pNameInfo);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_SetDebugUtilsObjectTagEXT(VkDevice device, const VkDebugUtilsObjectTagInfoEXT* pTagInfo)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.SetDebugUtilsObjectTagEXT(device, pTagInfo);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_QueueBeginDebugUtilsLabelEXT(VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)queue;
    vk_object->device->dispatch_table.QueueBeginDebugUtilsLabelEXT(queue, pLabelInfo);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_QueueEndDebugUtilsLabelEXT(VkQueue queue)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)queue;
    vk_object->device->dispatch_table.QueueEndDebugUtilsLabelEXT(queue);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_QueueInsertDebugUtilsLabelEXT(VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)queue;
    vk_object->device->dispatch_table.QueueInsertDebugUtilsLabelEXT(queue, pLabelInfo);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdBeginDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdBeginDebugUtilsLabelEXT(commandBuffer, pLabelInfo);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdEndDebugUtilsLabelEXT(VkCommandBuffer commandBuffer)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdEndDebugUtilsLabelEXT(commandBuffer);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdInsertDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdInsertDebugUtilsLabelEXT(commandBuffer, pLabelInfo);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetMemoryHostPointerPropertiesEXT(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, const void* pHostPointer, VkMemoryHostPointerPropertiesEXT* pMemoryHostPointerProperties)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetMemoryHostPointerPropertiesEXT(device, handleType, pHostPointer, pMemoryHostPointerProperties);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdWriteBufferMarkerAMD(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdWriteBufferMarkerAMD(commandBuffer, pipelineStage, dstBuffer, dstOffset, marker);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_CreateRenderPass2(VkDevice device, const VkRenderPassCreateInfo2* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.CreateRenderPass2(device, pCreateInfo, pAllocator, pRenderPass);
}
    static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdBeginRenderPass2(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo*      pRenderPassBegin, const VkSubpassBeginInfo*      pSubpassBeginInfo)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdBeginRenderPass2(commandBuffer, pRenderPassBegin, pSubpassBeginInfo);
}
    static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdNextSubpass2(VkCommandBuffer commandBuffer, const VkSubpassBeginInfo*      pSubpassBeginInfo, const VkSubpassEndInfo*        pSubpassEndInfo)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdNextSubpass2(commandBuffer, pSubpassBeginInfo, pSubpassEndInfo);
}
    static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdEndRenderPass2(VkCommandBuffer commandBuffer, const VkSubpassEndInfo*        pSubpassEndInfo)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdEndRenderPass2(commandBuffer, pSubpassEndInfo);
}
    static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetSemaphoreCounterValue(VkDevice device, VkSemaphore semaphore, uint64_t* pValue)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetSemaphoreCounterValue(device, semaphore, pValue);
}
    static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_WaitSemaphores(VkDevice device, const VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.WaitSemaphores(device, pWaitInfo, timeout);
}
    static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_SignalSemaphore(VkDevice device, const VkSemaphoreSignalInfo* pSignalInfo)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.SignalSemaphore(device, pSignalInfo);
}
    #ifdef VK_USE_PLATFORM_ANDROID_KHR
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetAndroidHardwareBufferPropertiesANDROID(VkDevice device, const struct AHardwareBuffer* buffer, VkAndroidHardwareBufferPropertiesANDROID* pProperties)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetAndroidHardwareBufferPropertiesANDROID(device, buffer, pProperties);
}
#endif
#ifdef VK_USE_PLATFORM_ANDROID_KHR
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetMemoryAndroidHardwareBufferANDROID(VkDevice device, const VkMemoryGetAndroidHardwareBufferInfoANDROID* pInfo, struct AHardwareBuffer** pBuffer)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetMemoryAndroidHardwareBufferANDROID(device, pInfo, pBuffer);
}
#endif
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdDrawIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdDrawIndirectCount(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}
        static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdDrawIndexedIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdDrawIndexedIndirectCount(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}
        static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetCheckpointNV(VkCommandBuffer commandBuffer, const void* pCheckpointMarker)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetCheckpointNV(commandBuffer, pCheckpointMarker);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_GetQueueCheckpointDataNV(VkQueue queue, uint32_t* pCheckpointDataCount, VkCheckpointDataNV* pCheckpointData)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)queue;
    vk_object->device->dispatch_table.GetQueueCheckpointDataNV(queue, pCheckpointDataCount, pCheckpointData);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdBindTransformFeedbackBuffersEXT(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdBindTransformFeedbackBuffersEXT(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdBeginTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdBeginTransformFeedbackEXT(commandBuffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdEndTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdEndTransformFeedbackEXT(commandBuffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdBeginQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags, uint32_t index)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdBeginQueryIndexedEXT(commandBuffer, queryPool, query, flags, index);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdEndQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, uint32_t index)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdEndQueryIndexedEXT(commandBuffer, queryPool, query, index);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdDrawIndirectByteCountEXT(VkCommandBuffer commandBuffer, uint32_t instanceCount, uint32_t firstInstance, VkBuffer counterBuffer, VkDeviceSize counterBufferOffset, uint32_t counterOffset, uint32_t vertexStride)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdDrawIndirectByteCountEXT(commandBuffer, instanceCount, firstInstance, counterBuffer, counterBufferOffset, counterOffset, vertexStride);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetExclusiveScissorNV(VkCommandBuffer commandBuffer, uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, const VkRect2D* pExclusiveScissors)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetExclusiveScissorNV(commandBuffer, firstExclusiveScissor, exclusiveScissorCount, pExclusiveScissors);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdBindShadingRateImageNV(VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdBindShadingRateImageNV(commandBuffer, imageView, imageLayout);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetViewportShadingRatePaletteNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkShadingRatePaletteNV* pShadingRatePalettes)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetViewportShadingRatePaletteNV(commandBuffer, firstViewport, viewportCount, pShadingRatePalettes);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetCoarseSampleOrderNV(VkCommandBuffer commandBuffer, VkCoarseSampleOrderTypeNV sampleOrderType, uint32_t customSampleOrderCount, const VkCoarseSampleOrderCustomNV* pCustomSampleOrders)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetCoarseSampleOrderNV(commandBuffer, sampleOrderType, customSampleOrderCount, pCustomSampleOrders);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdDrawMeshTasksNV(VkCommandBuffer commandBuffer, uint32_t taskCount, uint32_t firstTask)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdDrawMeshTasksNV(commandBuffer, taskCount, firstTask);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdDrawMeshTasksIndirectNV(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdDrawMeshTasksIndirectNV(commandBuffer, buffer, offset, drawCount, stride);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdDrawMeshTasksIndirectCountNV(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdDrawMeshTasksIndirectCountNV(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_CompileDeferredNV(VkDevice device, VkPipeline pipeline, uint32_t shader)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.CompileDeferredNV(device, pipeline, shader);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_CreateAccelerationStructureNV(VkDevice device, const VkAccelerationStructureCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkAccelerationStructureNV* pAccelerationStructure)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.CreateAccelerationStructureNV(device, pCreateInfo, pAllocator, pAccelerationStructure);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_DestroyAccelerationStructureKHR(VkDevice device, VkAccelerationStructureKHR accelerationStructure, const VkAllocationCallbacks* pAllocator)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.DestroyAccelerationStructureKHR(device, accelerationStructure, pAllocator);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_DestroyAccelerationStructureNV(VkDevice device, VkAccelerationStructureNV accelerationStructure, const VkAllocationCallbacks* pAllocator)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.DestroyAccelerationStructureNV(device, accelerationStructure, pAllocator);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_GetAccelerationStructureMemoryRequirementsNV(VkDevice device, const VkAccelerationStructureMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2KHR* pMemoryRequirements)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.GetAccelerationStructureMemoryRequirementsNV(device, pInfo, pMemoryRequirements);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_BindAccelerationStructureMemoryNV(VkDevice device, uint32_t bindInfoCount, const VkBindAccelerationStructureMemoryInfoNV* pBindInfos)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.BindAccelerationStructureMemoryNV(device, bindInfoCount, pBindInfos);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdCopyAccelerationStructureNV(VkCommandBuffer commandBuffer, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkCopyAccelerationStructureModeKHR mode)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdCopyAccelerationStructureNV(commandBuffer, dst, src, mode);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdCopyAccelerationStructureKHR(VkCommandBuffer commandBuffer, const VkCopyAccelerationStructureInfoKHR* pInfo)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdCopyAccelerationStructureKHR(commandBuffer, pInfo);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_CopyAccelerationStructureKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyAccelerationStructureInfoKHR* pInfo)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.CopyAccelerationStructureKHR(device, deferredOperation, pInfo);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdCopyAccelerationStructureToMemoryKHR(VkCommandBuffer commandBuffer, const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdCopyAccelerationStructureToMemoryKHR(commandBuffer, pInfo);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_CopyAccelerationStructureToMemoryKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.CopyAccelerationStructureToMemoryKHR(device, deferredOperation, pInfo);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdCopyMemoryToAccelerationStructureKHR(VkCommandBuffer commandBuffer, const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdCopyMemoryToAccelerationStructureKHR(commandBuffer, pInfo);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_CopyMemoryToAccelerationStructureKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.CopyMemoryToAccelerationStructureKHR(device, deferredOperation, pInfo);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdWriteAccelerationStructuresPropertiesKHR(VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, const VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdWriteAccelerationStructuresPropertiesKHR(commandBuffer, accelerationStructureCount, pAccelerationStructures, queryType, queryPool, firstQuery);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdWriteAccelerationStructuresPropertiesNV(VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, const VkAccelerationStructureNV* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdWriteAccelerationStructuresPropertiesNV(commandBuffer, accelerationStructureCount, pAccelerationStructures, queryType, queryPool, firstQuery);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdBuildAccelerationStructureNV(VkCommandBuffer commandBuffer, const VkAccelerationStructureInfoNV* pInfo, VkBuffer instanceData, VkDeviceSize instanceOffset, VkBool32 update, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkBuffer scratch, VkDeviceSize scratchOffset)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdBuildAccelerationStructureNV(commandBuffer, pInfo, instanceData, instanceOffset, update, dst, src, scratch, scratchOffset);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_WriteAccelerationStructuresPropertiesKHR(VkDevice device, uint32_t accelerationStructureCount, const VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType  queryType, size_t       dataSize, void* pData, size_t stride)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.WriteAccelerationStructuresPropertiesKHR(device, accelerationStructureCount, pAccelerationStructures, queryType, dataSize, pData, stride);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdTraceRaysKHR(VkCommandBuffer commandBuffer, const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, uint32_t width, uint32_t height, uint32_t depth)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdTraceRaysKHR(commandBuffer, pRaygenShaderBindingTable, pMissShaderBindingTable, pHitShaderBindingTable, pCallableShaderBindingTable, width, height, depth);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdTraceRaysNV(VkCommandBuffer commandBuffer, VkBuffer raygenShaderBindingTableBuffer, VkDeviceSize raygenShaderBindingOffset, VkBuffer missShaderBindingTableBuffer, VkDeviceSize missShaderBindingOffset, VkDeviceSize missShaderBindingStride, VkBuffer hitShaderBindingTableBuffer, VkDeviceSize hitShaderBindingOffset, VkDeviceSize hitShaderBindingStride, VkBuffer callableShaderBindingTableBuffer, VkDeviceSize callableShaderBindingOffset, VkDeviceSize callableShaderBindingStride, uint32_t width, uint32_t height, uint32_t depth)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdTraceRaysNV(commandBuffer, raygenShaderBindingTableBuffer, raygenShaderBindingOffset, missShaderBindingTableBuffer, missShaderBindingOffset, missShaderBindingStride, hitShaderBindingTableBuffer, hitShaderBindingOffset, hitShaderBindingStride, callableShaderBindingTableBuffer, callableShaderBindingOffset, callableShaderBindingStride, width, height, depth);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetRayTracingShaderGroupHandlesKHR(VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetRayTracingShaderGroupHandlesKHR(device, pipeline, firstGroup, groupCount, dataSize, pData);
}
    static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetRayTracingCaptureReplayShaderGroupHandlesKHR(VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetRayTracingCaptureReplayShaderGroupHandlesKHR(device, pipeline, firstGroup, groupCount, dataSize, pData);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetAccelerationStructureHandleNV(VkDevice device, VkAccelerationStructureNV accelerationStructure, size_t dataSize, void* pData)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetAccelerationStructureHandleNV(device, accelerationStructure, dataSize, pData);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_CreateRayTracingPipelinesNV(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkRayTracingPipelineCreateInfoNV* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.CreateRayTracingPipelinesNV(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_CreateRayTracingPipelinesKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkRayTracingPipelineCreateInfoKHR* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.CreateRayTracingPipelinesKHR(device, deferredOperation, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdTraceRaysIndirectKHR(VkCommandBuffer commandBuffer, const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, VkDeviceAddress indirectDeviceAddress)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdTraceRaysIndirectKHR(commandBuffer, pRaygenShaderBindingTable, pMissShaderBindingTable, pHitShaderBindingTable, pCallableShaderBindingTable, indirectDeviceAddress);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_GetDeviceAccelerationStructureCompatibilityKHR(VkDevice device, const VkAccelerationStructureVersionInfoKHR* pVersionInfo, VkAccelerationStructureCompatibilityKHR* pCompatibility)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.GetDeviceAccelerationStructureCompatibilityKHR(device, pVersionInfo, pCompatibility);
}
static VKAPI_ATTR VkDeviceSize VKAPI_CALL
vk_tramp_GetRayTracingShaderGroupStackSizeKHR(VkDevice device, VkPipeline pipeline, uint32_t group, VkShaderGroupShaderKHR groupShader)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetRayTracingShaderGroupStackSizeKHR(device, pipeline, group, groupShader);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetRayTracingPipelineStackSizeKHR(VkCommandBuffer commandBuffer, uint32_t pipelineStackSize)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetRayTracingPipelineStackSizeKHR(commandBuffer, pipelineStackSize);
}
static VKAPI_ATTR uint32_t VKAPI_CALL
vk_tramp_GetImageViewHandleNVX(VkDevice device, const VkImageViewHandleInfoNVX* pInfo)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetImageViewHandleNVX(device, pInfo);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetImageViewAddressNVX(VkDevice device, VkImageView imageView, VkImageViewAddressPropertiesNVX* pProperties)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetImageViewAddressNVX(device, imageView, pProperties);
}
#ifdef VK_USE_PLATFORM_WIN32_KHR
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetDeviceGroupSurfacePresentModes2EXT(VkDevice device, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkDeviceGroupPresentModeFlagsKHR* pModes)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetDeviceGroupSurfacePresentModes2EXT(device, pSurfaceInfo, pModes);
}
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_AcquireFullScreenExclusiveModeEXT(VkDevice device, VkSwapchainKHR swapchain)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.AcquireFullScreenExclusiveModeEXT(device, swapchain);
}
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_ReleaseFullScreenExclusiveModeEXT(VkDevice device, VkSwapchainKHR swapchain)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.ReleaseFullScreenExclusiveModeEXT(device, swapchain);
}
#endif
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_AcquireProfilingLockKHR(VkDevice device, const VkAcquireProfilingLockInfoKHR* pInfo)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.AcquireProfilingLockKHR(device, pInfo);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_ReleaseProfilingLockKHR(VkDevice device)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.ReleaseProfilingLockKHR(device);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetImageDrmFormatModifierPropertiesEXT(VkDevice device, VkImage image, VkImageDrmFormatModifierPropertiesEXT* pProperties)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetImageDrmFormatModifierPropertiesEXT(device, image, pProperties);
}
static VKAPI_ATTR uint64_t VKAPI_CALL
vk_tramp_GetBufferOpaqueCaptureAddress(VkDevice device, const VkBufferDeviceAddressInfo* pInfo)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetBufferOpaqueCaptureAddress(device, pInfo);
}
    static VKAPI_ATTR VkDeviceAddress VKAPI_CALL
vk_tramp_GetBufferDeviceAddress(VkDevice device, const VkBufferDeviceAddressInfo* pInfo)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetBufferDeviceAddress(device, pInfo);
}
        static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_InitializePerformanceApiINTEL(VkDevice device, const VkInitializePerformanceApiInfoINTEL* pInitializeInfo)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.InitializePerformanceApiINTEL(device, pInitializeInfo);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_UninitializePerformanceApiINTEL(VkDevice device)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.UninitializePerformanceApiINTEL(device);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_CmdSetPerformanceMarkerINTEL(VkCommandBuffer commandBuffer, const VkPerformanceMarkerInfoINTEL* pMarkerInfo)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    return vk_object->device->dispatch_table.CmdSetPerformanceMarkerINTEL(commandBuffer, pMarkerInfo);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_CmdSetPerformanceStreamMarkerINTEL(VkCommandBuffer commandBuffer, const VkPerformanceStreamMarkerInfoINTEL* pMarkerInfo)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    return vk_object->device->dispatch_table.CmdSetPerformanceStreamMarkerINTEL(commandBuffer, pMarkerInfo);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_CmdSetPerformanceOverrideINTEL(VkCommandBuffer commandBuffer, const VkPerformanceOverrideInfoINTEL* pOverrideInfo)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    return vk_object->device->dispatch_table.CmdSetPerformanceOverrideINTEL(commandBuffer, pOverrideInfo);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_AcquirePerformanceConfigurationINTEL(VkDevice device, const VkPerformanceConfigurationAcquireInfoINTEL* pAcquireInfo, VkPerformanceConfigurationINTEL* pConfiguration)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.AcquirePerformanceConfigurationINTEL(device, pAcquireInfo, pConfiguration);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_ReleasePerformanceConfigurationINTEL(VkDevice device, VkPerformanceConfigurationINTEL configuration)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.ReleasePerformanceConfigurationINTEL(device, configuration);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_QueueSetPerformanceConfigurationINTEL(VkQueue queue, VkPerformanceConfigurationINTEL configuration)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)queue;
    return vk_object->device->dispatch_table.QueueSetPerformanceConfigurationINTEL(queue, configuration);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetPerformanceParameterINTEL(VkDevice device, VkPerformanceParameterTypeINTEL parameter, VkPerformanceValueINTEL* pValue)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetPerformanceParameterINTEL(device, parameter, pValue);
}
static VKAPI_ATTR uint64_t VKAPI_CALL
vk_tramp_GetDeviceMemoryOpaqueCaptureAddress(VkDevice device, const VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetDeviceMemoryOpaqueCaptureAddress(device, pInfo);
}
    static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetPipelineExecutablePropertiesKHR(VkDevice                        device, const VkPipelineInfoKHR*        pPipelineInfo, uint32_t* pExecutableCount, VkPipelineExecutablePropertiesKHR* pProperties)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetPipelineExecutablePropertiesKHR(device, pPipelineInfo, pExecutableCount, pProperties);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetPipelineExecutableStatisticsKHR(VkDevice                        device, const VkPipelineExecutableInfoKHR*  pExecutableInfo, uint32_t* pStatisticCount, VkPipelineExecutableStatisticKHR* pStatistics)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetPipelineExecutableStatisticsKHR(device, pExecutableInfo, pStatisticCount, pStatistics);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetPipelineExecutableInternalRepresentationsKHR(VkDevice                        device, const VkPipelineExecutableInfoKHR*  pExecutableInfo, uint32_t* pInternalRepresentationCount, VkPipelineExecutableInternalRepresentationKHR* pInternalRepresentations)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetPipelineExecutableInternalRepresentationsKHR(device, pExecutableInfo, pInternalRepresentationCount, pInternalRepresentations);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetLineStippleEXT(VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetLineStippleEXT(commandBuffer, lineStippleFactor, lineStipplePattern);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_CreateAccelerationStructureKHR(VkDevice                                           device, const VkAccelerationStructureCreateInfoKHR*        pCreateInfo, const VkAllocationCallbacks*       pAllocator, VkAccelerationStructureKHR*                        pAccelerationStructure)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.CreateAccelerationStructureKHR(device, pCreateInfo, pAllocator, pAccelerationStructure);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdBuildAccelerationStructuresKHR(VkCommandBuffer                                    commandBuffer, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdBuildAccelerationStructuresKHR(commandBuffer, infoCount, pInfos, ppBuildRangeInfos);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdBuildAccelerationStructuresIndirectKHR(VkCommandBuffer                  commandBuffer, uint32_t                                           infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkDeviceAddress*             pIndirectDeviceAddresses, const uint32_t*                    pIndirectStrides, const uint32_t* const*             ppMaxPrimitiveCounts)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdBuildAccelerationStructuresIndirectKHR(commandBuffer, infoCount, pInfos, pIndirectDeviceAddresses, pIndirectStrides, ppMaxPrimitiveCounts);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_BuildAccelerationStructuresKHR(VkDevice                                           device, VkDeferredOperationKHR deferredOperation, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.BuildAccelerationStructuresKHR(device, deferredOperation, infoCount, pInfos, ppBuildRangeInfos);
}
static VKAPI_ATTR VkDeviceAddress VKAPI_CALL
vk_tramp_GetAccelerationStructureDeviceAddressKHR(VkDevice device, const VkAccelerationStructureDeviceAddressInfoKHR* pInfo)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetAccelerationStructureDeviceAddressKHR(device, pInfo);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_CreateDeferredOperationKHR(VkDevice device, const VkAllocationCallbacks* pAllocator, VkDeferredOperationKHR* pDeferredOperation)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.CreateDeferredOperationKHR(device, pAllocator, pDeferredOperation);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_DestroyDeferredOperationKHR(VkDevice device, VkDeferredOperationKHR operation, const VkAllocationCallbacks* pAllocator)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.DestroyDeferredOperationKHR(device, operation, pAllocator);
}
static VKAPI_ATTR uint32_t VKAPI_CALL
vk_tramp_GetDeferredOperationMaxConcurrencyKHR(VkDevice device, VkDeferredOperationKHR operation)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetDeferredOperationMaxConcurrencyKHR(device, operation);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetDeferredOperationResultKHR(VkDevice device, VkDeferredOperationKHR operation)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetDeferredOperationResultKHR(device, operation);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_DeferredOperationJoinKHR(VkDevice device, VkDeferredOperationKHR operation)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.DeferredOperationJoinKHR(device, operation);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetCullModeEXT(VkCommandBuffer commandBuffer, VkCullModeFlags cullMode)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetCullModeEXT(commandBuffer, cullMode);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetFrontFaceEXT(VkCommandBuffer commandBuffer, VkFrontFace frontFace)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetFrontFaceEXT(commandBuffer, frontFace);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetPrimitiveTopologyEXT(VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetPrimitiveTopologyEXT(commandBuffer, primitiveTopology);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetViewportWithCountEXT(VkCommandBuffer commandBuffer, uint32_t viewportCount, const VkViewport* pViewports)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetViewportWithCountEXT(commandBuffer, viewportCount, pViewports);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetScissorWithCountEXT(VkCommandBuffer commandBuffer, uint32_t scissorCount, const VkRect2D* pScissors)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetScissorWithCountEXT(commandBuffer, scissorCount, pScissors);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdBindVertexBuffers2EXT(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes, const VkDeviceSize* pStrides)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdBindVertexBuffers2EXT(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes, pStrides);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetDepthTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthTestEnable)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetDepthTestEnableEXT(commandBuffer, depthTestEnable);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetDepthWriteEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetDepthWriteEnableEXT(commandBuffer, depthWriteEnable);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetDepthCompareOpEXT(VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetDepthCompareOpEXT(commandBuffer, depthCompareOp);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetDepthBoundsTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetDepthBoundsTestEnableEXT(commandBuffer, depthBoundsTestEnable);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetStencilTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetStencilTestEnableEXT(commandBuffer, stencilTestEnable);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetStencilOpEXT(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetStencilOpEXT(commandBuffer, faceMask, failOp, passOp, depthFailOp, compareOp);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_CreatePrivateDataSlotEXT(VkDevice device, const VkPrivateDataSlotCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPrivateDataSlotEXT* pPrivateDataSlot)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.CreatePrivateDataSlotEXT(device, pCreateInfo, pAllocator, pPrivateDataSlot);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_DestroyPrivateDataSlotEXT(VkDevice device, VkPrivateDataSlotEXT privateDataSlot, const VkAllocationCallbacks* pAllocator)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.DestroyPrivateDataSlotEXT(device, privateDataSlot, pAllocator);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_SetPrivateDataEXT(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlotEXT privateDataSlot, uint64_t data)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.SetPrivateDataEXT(device, objectType, objectHandle, privateDataSlot, data);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_GetPrivateDataEXT(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlotEXT privateDataSlot, uint64_t* pData)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.GetPrivateDataEXT(device, objectType, objectHandle, privateDataSlot, pData);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdCopyBuffer2KHR(VkCommandBuffer commandBuffer, const VkCopyBufferInfo2KHR* pCopyBufferInfo)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdCopyBuffer2KHR(commandBuffer, pCopyBufferInfo);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdCopyImage2KHR(VkCommandBuffer commandBuffer, const VkCopyImageInfo2KHR* pCopyImageInfo)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdCopyImage2KHR(commandBuffer, pCopyImageInfo);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdBlitImage2KHR(VkCommandBuffer commandBuffer, const VkBlitImageInfo2KHR* pBlitImageInfo)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdBlitImage2KHR(commandBuffer, pBlitImageInfo);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdCopyBufferToImage2KHR(VkCommandBuffer commandBuffer, const VkCopyBufferToImageInfo2KHR* pCopyBufferToImageInfo)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdCopyBufferToImage2KHR(commandBuffer, pCopyBufferToImageInfo);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdCopyImageToBuffer2KHR(VkCommandBuffer commandBuffer, const VkCopyImageToBufferInfo2KHR* pCopyImageToBufferInfo)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdCopyImageToBuffer2KHR(commandBuffer, pCopyImageToBufferInfo);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdResolveImage2KHR(VkCommandBuffer commandBuffer, const VkResolveImageInfo2KHR* pResolveImageInfo)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdResolveImage2KHR(commandBuffer, pResolveImageInfo);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetFragmentShadingRateKHR(VkCommandBuffer           commandBuffer, const VkExtent2D*                           pFragmentSize, const VkFragmentShadingRateCombinerOpKHR    combinerOps[2])
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetFragmentShadingRateKHR(commandBuffer, pFragmentSize, combinerOps);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetFragmentShadingRateEnumNV(VkCommandBuffer           commandBuffer, VkFragmentShadingRateNV                     shadingRate, const VkFragmentShadingRateCombinerOpKHR    combinerOps[2])
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetFragmentShadingRateEnumNV(commandBuffer, shadingRate, combinerOps);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_GetAccelerationStructureBuildSizesKHR(VkDevice                                            device, VkAccelerationStructureBuildTypeKHR                 buildType, const VkAccelerationStructureBuildGeometryInfoKHR*  pBuildInfo, const uint32_t*  pMaxPrimitiveCounts, VkAccelerationStructureBuildSizesInfoKHR*           pSizeInfo)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.GetAccelerationStructureBuildSizesKHR(device, buildType, pBuildInfo, pMaxPrimitiveCounts, pSizeInfo);
}

struct vk_device_dispatch_table vk_device_trampolines = {
    .GetDeviceProcAddr = vk_tramp_GetDeviceProcAddr,
    .DestroyDevice = vk_tramp_DestroyDevice,
    .GetDeviceQueue = vk_tramp_GetDeviceQueue,
    .QueueSubmit = vk_tramp_QueueSubmit,
    .QueueWaitIdle = vk_tramp_QueueWaitIdle,
    .DeviceWaitIdle = vk_tramp_DeviceWaitIdle,
    .AllocateMemory = vk_tramp_AllocateMemory,
    .FreeMemory = vk_tramp_FreeMemory,
    .MapMemory = vk_tramp_MapMemory,
    .UnmapMemory = vk_tramp_UnmapMemory,
    .FlushMappedMemoryRanges = vk_tramp_FlushMappedMemoryRanges,
    .InvalidateMappedMemoryRanges = vk_tramp_InvalidateMappedMemoryRanges,
    .GetDeviceMemoryCommitment = vk_tramp_GetDeviceMemoryCommitment,
    .GetBufferMemoryRequirements = vk_tramp_GetBufferMemoryRequirements,
    .BindBufferMemory = vk_tramp_BindBufferMemory,
    .GetImageMemoryRequirements = vk_tramp_GetImageMemoryRequirements,
    .BindImageMemory = vk_tramp_BindImageMemory,
    .GetImageSparseMemoryRequirements = vk_tramp_GetImageSparseMemoryRequirements,
    .QueueBindSparse = vk_tramp_QueueBindSparse,
    .CreateFence = vk_tramp_CreateFence,
    .DestroyFence = vk_tramp_DestroyFence,
    .ResetFences = vk_tramp_ResetFences,
    .GetFenceStatus = vk_tramp_GetFenceStatus,
    .WaitForFences = vk_tramp_WaitForFences,
    .CreateSemaphore = vk_tramp_CreateSemaphore,
    .DestroySemaphore = vk_tramp_DestroySemaphore,
    .CreateEvent = vk_tramp_CreateEvent,
    .DestroyEvent = vk_tramp_DestroyEvent,
    .GetEventStatus = vk_tramp_GetEventStatus,
    .SetEvent = vk_tramp_SetEvent,
    .ResetEvent = vk_tramp_ResetEvent,
    .CreateQueryPool = vk_tramp_CreateQueryPool,
    .DestroyQueryPool = vk_tramp_DestroyQueryPool,
    .GetQueryPoolResults = vk_tramp_GetQueryPoolResults,
    .ResetQueryPool = vk_tramp_ResetQueryPool,
        .CreateBuffer = vk_tramp_CreateBuffer,
    .DestroyBuffer = vk_tramp_DestroyBuffer,
    .CreateBufferView = vk_tramp_CreateBufferView,
    .DestroyBufferView = vk_tramp_DestroyBufferView,
    .CreateImage = vk_tramp_CreateImage,
    .DestroyImage = vk_tramp_DestroyImage,
    .GetImageSubresourceLayout = vk_tramp_GetImageSubresourceLayout,
    .CreateImageView = vk_tramp_CreateImageView,
    .DestroyImageView = vk_tramp_DestroyImageView,
    .CreateShaderModule = vk_tramp_CreateShaderModule,
    .DestroyShaderModule = vk_tramp_DestroyShaderModule,
    .CreatePipelineCache = vk_tramp_CreatePipelineCache,
    .DestroyPipelineCache = vk_tramp_DestroyPipelineCache,
    .GetPipelineCacheData = vk_tramp_GetPipelineCacheData,
    .MergePipelineCaches = vk_tramp_MergePipelineCaches,
    .CreateGraphicsPipelines = vk_tramp_CreateGraphicsPipelines,
    .CreateComputePipelines = vk_tramp_CreateComputePipelines,
    .DestroyPipeline = vk_tramp_DestroyPipeline,
    .CreatePipelineLayout = vk_tramp_CreatePipelineLayout,
    .DestroyPipelineLayout = vk_tramp_DestroyPipelineLayout,
    .CreateSampler = vk_tramp_CreateSampler,
    .DestroySampler = vk_tramp_DestroySampler,
    .CreateDescriptorSetLayout = vk_tramp_CreateDescriptorSetLayout,
    .DestroyDescriptorSetLayout = vk_tramp_DestroyDescriptorSetLayout,
    .CreateDescriptorPool = vk_tramp_CreateDescriptorPool,
    .DestroyDescriptorPool = vk_tramp_DestroyDescriptorPool,
    .ResetDescriptorPool = vk_tramp_ResetDescriptorPool,
    .AllocateDescriptorSets = vk_tramp_AllocateDescriptorSets,
    .FreeDescriptorSets = vk_tramp_FreeDescriptorSets,
    .UpdateDescriptorSets = vk_tramp_UpdateDescriptorSets,
    .CreateFramebuffer = vk_tramp_CreateFramebuffer,
    .DestroyFramebuffer = vk_tramp_DestroyFramebuffer,
    .CreateRenderPass = vk_tramp_CreateRenderPass,
    .DestroyRenderPass = vk_tramp_DestroyRenderPass,
    .GetRenderAreaGranularity = vk_tramp_GetRenderAreaGranularity,
    .CreateCommandPool = vk_tramp_CreateCommandPool,
    .DestroyCommandPool = vk_tramp_DestroyCommandPool,
    .ResetCommandPool = vk_tramp_ResetCommandPool,
    .AllocateCommandBuffers = vk_tramp_AllocateCommandBuffers,
    .FreeCommandBuffers = vk_tramp_FreeCommandBuffers,
    .BeginCommandBuffer = vk_tramp_BeginCommandBuffer,
    .EndCommandBuffer = vk_tramp_EndCommandBuffer,
    .ResetCommandBuffer = vk_tramp_ResetCommandBuffer,
    .CmdBindPipeline = vk_tramp_CmdBindPipeline,
    .CmdSetViewport = vk_tramp_CmdSetViewport,
    .CmdSetScissor = vk_tramp_CmdSetScissor,
    .CmdSetLineWidth = vk_tramp_CmdSetLineWidth,
    .CmdSetDepthBias = vk_tramp_CmdSetDepthBias,
    .CmdSetBlendConstants = vk_tramp_CmdSetBlendConstants,
    .CmdSetDepthBounds = vk_tramp_CmdSetDepthBounds,
    .CmdSetStencilCompareMask = vk_tramp_CmdSetStencilCompareMask,
    .CmdSetStencilWriteMask = vk_tramp_CmdSetStencilWriteMask,
    .CmdSetStencilReference = vk_tramp_CmdSetStencilReference,
    .CmdBindDescriptorSets = vk_tramp_CmdBindDescriptorSets,
    .CmdBindIndexBuffer = vk_tramp_CmdBindIndexBuffer,
    .CmdBindVertexBuffers = vk_tramp_CmdBindVertexBuffers,
    .CmdDraw = vk_tramp_CmdDraw,
    .CmdDrawIndexed = vk_tramp_CmdDrawIndexed,
    .CmdDrawIndirect = vk_tramp_CmdDrawIndirect,
    .CmdDrawIndexedIndirect = vk_tramp_CmdDrawIndexedIndirect,
    .CmdDispatch = vk_tramp_CmdDispatch,
    .CmdDispatchIndirect = vk_tramp_CmdDispatchIndirect,
    .CmdCopyBuffer = vk_tramp_CmdCopyBuffer,
    .CmdCopyImage = vk_tramp_CmdCopyImage,
    .CmdBlitImage = vk_tramp_CmdBlitImage,
    .CmdCopyBufferToImage = vk_tramp_CmdCopyBufferToImage,
    .CmdCopyImageToBuffer = vk_tramp_CmdCopyImageToBuffer,
    .CmdUpdateBuffer = vk_tramp_CmdUpdateBuffer,
    .CmdFillBuffer = vk_tramp_CmdFillBuffer,
    .CmdClearColorImage = vk_tramp_CmdClearColorImage,
    .CmdClearDepthStencilImage = vk_tramp_CmdClearDepthStencilImage,
    .CmdClearAttachments = vk_tramp_CmdClearAttachments,
    .CmdResolveImage = vk_tramp_CmdResolveImage,
    .CmdSetEvent = vk_tramp_CmdSetEvent,
    .CmdResetEvent = vk_tramp_CmdResetEvent,
    .CmdWaitEvents = vk_tramp_CmdWaitEvents,
    .CmdPipelineBarrier = vk_tramp_CmdPipelineBarrier,
    .CmdBeginQuery = vk_tramp_CmdBeginQuery,
    .CmdEndQuery = vk_tramp_CmdEndQuery,
    .CmdBeginConditionalRenderingEXT = vk_tramp_CmdBeginConditionalRenderingEXT,
    .CmdEndConditionalRenderingEXT = vk_tramp_CmdEndConditionalRenderingEXT,
    .CmdResetQueryPool = vk_tramp_CmdResetQueryPool,
    .CmdWriteTimestamp = vk_tramp_CmdWriteTimestamp,
    .CmdCopyQueryPoolResults = vk_tramp_CmdCopyQueryPoolResults,
    .CmdPushConstants = vk_tramp_CmdPushConstants,
    .CmdBeginRenderPass = vk_tramp_CmdBeginRenderPass,
    .CmdNextSubpass = vk_tramp_CmdNextSubpass,
    .CmdEndRenderPass = vk_tramp_CmdEndRenderPass,
    .CmdExecuteCommands = vk_tramp_CmdExecuteCommands,
    .CreateSharedSwapchainsKHR = vk_tramp_CreateSharedSwapchainsKHR,
    .CreateSwapchainKHR = vk_tramp_CreateSwapchainKHR,
    .DestroySwapchainKHR = vk_tramp_DestroySwapchainKHR,
    .GetSwapchainImagesKHR = vk_tramp_GetSwapchainImagesKHR,
    .AcquireNextImageKHR = vk_tramp_AcquireNextImageKHR,
    .QueuePresentKHR = vk_tramp_QueuePresentKHR,
    .DebugMarkerSetObjectNameEXT = vk_tramp_DebugMarkerSetObjectNameEXT,
    .DebugMarkerSetObjectTagEXT = vk_tramp_DebugMarkerSetObjectTagEXT,
    .CmdDebugMarkerBeginEXT = vk_tramp_CmdDebugMarkerBeginEXT,
    .CmdDebugMarkerEndEXT = vk_tramp_CmdDebugMarkerEndEXT,
    .CmdDebugMarkerInsertEXT = vk_tramp_CmdDebugMarkerInsertEXT,
#ifdef VK_USE_PLATFORM_WIN32_KHR
    .GetMemoryWin32HandleNV = vk_tramp_GetMemoryWin32HandleNV,
#endif
    .CmdExecuteGeneratedCommandsNV = vk_tramp_CmdExecuteGeneratedCommandsNV,
    .CmdPreprocessGeneratedCommandsNV = vk_tramp_CmdPreprocessGeneratedCommandsNV,
    .CmdBindPipelineShaderGroupNV = vk_tramp_CmdBindPipelineShaderGroupNV,
    .GetGeneratedCommandsMemoryRequirementsNV = vk_tramp_GetGeneratedCommandsMemoryRequirementsNV,
    .CreateIndirectCommandsLayoutNV = vk_tramp_CreateIndirectCommandsLayoutNV,
    .DestroyIndirectCommandsLayoutNV = vk_tramp_DestroyIndirectCommandsLayoutNV,
    .CmdPushDescriptorSetKHR = vk_tramp_CmdPushDescriptorSetKHR,
    .TrimCommandPool = vk_tramp_TrimCommandPool,
    #ifdef VK_USE_PLATFORM_WIN32_KHR
    .GetMemoryWin32HandleKHR = vk_tramp_GetMemoryWin32HandleKHR,
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
    .GetMemoryWin32HandlePropertiesKHR = vk_tramp_GetMemoryWin32HandlePropertiesKHR,
#endif
    .GetMemoryFdKHR = vk_tramp_GetMemoryFdKHR,
    .GetMemoryFdPropertiesKHR = vk_tramp_GetMemoryFdPropertiesKHR,
#ifdef VK_USE_PLATFORM_WIN32_KHR
    .GetSemaphoreWin32HandleKHR = vk_tramp_GetSemaphoreWin32HandleKHR,
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
    .ImportSemaphoreWin32HandleKHR = vk_tramp_ImportSemaphoreWin32HandleKHR,
#endif
    .GetSemaphoreFdKHR = vk_tramp_GetSemaphoreFdKHR,
    .ImportSemaphoreFdKHR = vk_tramp_ImportSemaphoreFdKHR,
#ifdef VK_USE_PLATFORM_WIN32_KHR
    .GetFenceWin32HandleKHR = vk_tramp_GetFenceWin32HandleKHR,
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
    .ImportFenceWin32HandleKHR = vk_tramp_ImportFenceWin32HandleKHR,
#endif
    .GetFenceFdKHR = vk_tramp_GetFenceFdKHR,
    .ImportFenceFdKHR = vk_tramp_ImportFenceFdKHR,
    .DisplayPowerControlEXT = vk_tramp_DisplayPowerControlEXT,
    .RegisterDeviceEventEXT = vk_tramp_RegisterDeviceEventEXT,
    .RegisterDisplayEventEXT = vk_tramp_RegisterDisplayEventEXT,
    .GetSwapchainCounterEXT = vk_tramp_GetSwapchainCounterEXT,
    .GetDeviceGroupPeerMemoryFeatures = vk_tramp_GetDeviceGroupPeerMemoryFeatures,
        .BindBufferMemory2 = vk_tramp_BindBufferMemory2,
        .BindImageMemory2 = vk_tramp_BindImageMemory2,
        .CmdSetDeviceMask = vk_tramp_CmdSetDeviceMask,
        .GetDeviceGroupPresentCapabilitiesKHR = vk_tramp_GetDeviceGroupPresentCapabilitiesKHR,
    .GetDeviceGroupSurfacePresentModesKHR = vk_tramp_GetDeviceGroupSurfacePresentModesKHR,
    .AcquireNextImage2KHR = vk_tramp_AcquireNextImage2KHR,
    .CmdDispatchBase = vk_tramp_CmdDispatchBase,
        .CreateDescriptorUpdateTemplate = vk_tramp_CreateDescriptorUpdateTemplate,
        .DestroyDescriptorUpdateTemplate = vk_tramp_DestroyDescriptorUpdateTemplate,
        .UpdateDescriptorSetWithTemplate = vk_tramp_UpdateDescriptorSetWithTemplate,
        .CmdPushDescriptorSetWithTemplateKHR = vk_tramp_CmdPushDescriptorSetWithTemplateKHR,
    .SetHdrMetadataEXT = vk_tramp_SetHdrMetadataEXT,
    .GetSwapchainStatusKHR = vk_tramp_GetSwapchainStatusKHR,
    .GetRefreshCycleDurationGOOGLE = vk_tramp_GetRefreshCycleDurationGOOGLE,
    .GetPastPresentationTimingGOOGLE = vk_tramp_GetPastPresentationTimingGOOGLE,
    .CmdSetViewportWScalingNV = vk_tramp_CmdSetViewportWScalingNV,
    .CmdSetDiscardRectangleEXT = vk_tramp_CmdSetDiscardRectangleEXT,
    .CmdSetSampleLocationsEXT = vk_tramp_CmdSetSampleLocationsEXT,
    .GetBufferMemoryRequirements2 = vk_tramp_GetBufferMemoryRequirements2,
        .GetImageMemoryRequirements2 = vk_tramp_GetImageMemoryRequirements2,
        .GetImageSparseMemoryRequirements2 = vk_tramp_GetImageSparseMemoryRequirements2,
        .CreateSamplerYcbcrConversion = vk_tramp_CreateSamplerYcbcrConversion,
        .DestroySamplerYcbcrConversion = vk_tramp_DestroySamplerYcbcrConversion,
        .GetDeviceQueue2 = vk_tramp_GetDeviceQueue2,
    .CreateValidationCacheEXT = vk_tramp_CreateValidationCacheEXT,
    .DestroyValidationCacheEXT = vk_tramp_DestroyValidationCacheEXT,
    .GetValidationCacheDataEXT = vk_tramp_GetValidationCacheDataEXT,
    .MergeValidationCachesEXT = vk_tramp_MergeValidationCachesEXT,
    .GetDescriptorSetLayoutSupport = vk_tramp_GetDescriptorSetLayoutSupport,
    #ifdef VK_USE_PLATFORM_ANDROID_KHR
    .GetSwapchainGrallocUsageANDROID = vk_tramp_GetSwapchainGrallocUsageANDROID,
#endif
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .GetSwapchainGrallocUsage2ANDROID = vk_tramp_GetSwapchainGrallocUsage2ANDROID,
#endif
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .AcquireImageANDROID = vk_tramp_AcquireImageANDROID,
#endif
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .QueueSignalReleaseImageANDROID = vk_tramp_QueueSignalReleaseImageANDROID,
#endif
    .GetShaderInfoAMD = vk_tramp_GetShaderInfoAMD,
    .SetLocalDimmingAMD = vk_tramp_SetLocalDimmingAMD,
    .GetCalibratedTimestampsEXT = vk_tramp_GetCalibratedTimestampsEXT,
    .SetDebugUtilsObjectNameEXT = vk_tramp_SetDebugUtilsObjectNameEXT,
    .SetDebugUtilsObjectTagEXT = vk_tramp_SetDebugUtilsObjectTagEXT,
    .QueueBeginDebugUtilsLabelEXT = vk_tramp_QueueBeginDebugUtilsLabelEXT,
    .QueueEndDebugUtilsLabelEXT = vk_tramp_QueueEndDebugUtilsLabelEXT,
    .QueueInsertDebugUtilsLabelEXT = vk_tramp_QueueInsertDebugUtilsLabelEXT,
    .CmdBeginDebugUtilsLabelEXT = vk_tramp_CmdBeginDebugUtilsLabelEXT,
    .CmdEndDebugUtilsLabelEXT = vk_tramp_CmdEndDebugUtilsLabelEXT,
    .CmdInsertDebugUtilsLabelEXT = vk_tramp_CmdInsertDebugUtilsLabelEXT,
    .GetMemoryHostPointerPropertiesEXT = vk_tramp_GetMemoryHostPointerPropertiesEXT,
    .CmdWriteBufferMarkerAMD = vk_tramp_CmdWriteBufferMarkerAMD,
    .CreateRenderPass2 = vk_tramp_CreateRenderPass2,
        .CmdBeginRenderPass2 = vk_tramp_CmdBeginRenderPass2,
        .CmdNextSubpass2 = vk_tramp_CmdNextSubpass2,
        .CmdEndRenderPass2 = vk_tramp_CmdEndRenderPass2,
        .GetSemaphoreCounterValue = vk_tramp_GetSemaphoreCounterValue,
        .WaitSemaphores = vk_tramp_WaitSemaphores,
        .SignalSemaphore = vk_tramp_SignalSemaphore,
    #ifdef VK_USE_PLATFORM_ANDROID_KHR
    .GetAndroidHardwareBufferPropertiesANDROID = vk_tramp_GetAndroidHardwareBufferPropertiesANDROID,
#endif
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .GetMemoryAndroidHardwareBufferANDROID = vk_tramp_GetMemoryAndroidHardwareBufferANDROID,
#endif
    .CmdDrawIndirectCount = vk_tramp_CmdDrawIndirectCount,
            .CmdDrawIndexedIndirectCount = vk_tramp_CmdDrawIndexedIndirectCount,
            .CmdSetCheckpointNV = vk_tramp_CmdSetCheckpointNV,
    .GetQueueCheckpointDataNV = vk_tramp_GetQueueCheckpointDataNV,
    .CmdBindTransformFeedbackBuffersEXT = vk_tramp_CmdBindTransformFeedbackBuffersEXT,
    .CmdBeginTransformFeedbackEXT = vk_tramp_CmdBeginTransformFeedbackEXT,
    .CmdEndTransformFeedbackEXT = vk_tramp_CmdEndTransformFeedbackEXT,
    .CmdBeginQueryIndexedEXT = vk_tramp_CmdBeginQueryIndexedEXT,
    .CmdEndQueryIndexedEXT = vk_tramp_CmdEndQueryIndexedEXT,
    .CmdDrawIndirectByteCountEXT = vk_tramp_CmdDrawIndirectByteCountEXT,
    .CmdSetExclusiveScissorNV = vk_tramp_CmdSetExclusiveScissorNV,
    .CmdBindShadingRateImageNV = vk_tramp_CmdBindShadingRateImageNV,
    .CmdSetViewportShadingRatePaletteNV = vk_tramp_CmdSetViewportShadingRatePaletteNV,
    .CmdSetCoarseSampleOrderNV = vk_tramp_CmdSetCoarseSampleOrderNV,
    .CmdDrawMeshTasksNV = vk_tramp_CmdDrawMeshTasksNV,
    .CmdDrawMeshTasksIndirectNV = vk_tramp_CmdDrawMeshTasksIndirectNV,
    .CmdDrawMeshTasksIndirectCountNV = vk_tramp_CmdDrawMeshTasksIndirectCountNV,
    .CompileDeferredNV = vk_tramp_CompileDeferredNV,
    .CreateAccelerationStructureNV = vk_tramp_CreateAccelerationStructureNV,
    .DestroyAccelerationStructureKHR = vk_tramp_DestroyAccelerationStructureKHR,
    .DestroyAccelerationStructureNV = vk_tramp_DestroyAccelerationStructureNV,
    .GetAccelerationStructureMemoryRequirementsNV = vk_tramp_GetAccelerationStructureMemoryRequirementsNV,
    .BindAccelerationStructureMemoryNV = vk_tramp_BindAccelerationStructureMemoryNV,
    .CmdCopyAccelerationStructureNV = vk_tramp_CmdCopyAccelerationStructureNV,
    .CmdCopyAccelerationStructureKHR = vk_tramp_CmdCopyAccelerationStructureKHR,
    .CopyAccelerationStructureKHR = vk_tramp_CopyAccelerationStructureKHR,
    .CmdCopyAccelerationStructureToMemoryKHR = vk_tramp_CmdCopyAccelerationStructureToMemoryKHR,
    .CopyAccelerationStructureToMemoryKHR = vk_tramp_CopyAccelerationStructureToMemoryKHR,
    .CmdCopyMemoryToAccelerationStructureKHR = vk_tramp_CmdCopyMemoryToAccelerationStructureKHR,
    .CopyMemoryToAccelerationStructureKHR = vk_tramp_CopyMemoryToAccelerationStructureKHR,
    .CmdWriteAccelerationStructuresPropertiesKHR = vk_tramp_CmdWriteAccelerationStructuresPropertiesKHR,
    .CmdWriteAccelerationStructuresPropertiesNV = vk_tramp_CmdWriteAccelerationStructuresPropertiesNV,
    .CmdBuildAccelerationStructureNV = vk_tramp_CmdBuildAccelerationStructureNV,
    .WriteAccelerationStructuresPropertiesKHR = vk_tramp_WriteAccelerationStructuresPropertiesKHR,
    .CmdTraceRaysKHR = vk_tramp_CmdTraceRaysKHR,
    .CmdTraceRaysNV = vk_tramp_CmdTraceRaysNV,
    .GetRayTracingShaderGroupHandlesKHR = vk_tramp_GetRayTracingShaderGroupHandlesKHR,
        .GetRayTracingCaptureReplayShaderGroupHandlesKHR = vk_tramp_GetRayTracingCaptureReplayShaderGroupHandlesKHR,
    .GetAccelerationStructureHandleNV = vk_tramp_GetAccelerationStructureHandleNV,
    .CreateRayTracingPipelinesNV = vk_tramp_CreateRayTracingPipelinesNV,
    .CreateRayTracingPipelinesKHR = vk_tramp_CreateRayTracingPipelinesKHR,
    .CmdTraceRaysIndirectKHR = vk_tramp_CmdTraceRaysIndirectKHR,
    .GetDeviceAccelerationStructureCompatibilityKHR = vk_tramp_GetDeviceAccelerationStructureCompatibilityKHR,
    .GetRayTracingShaderGroupStackSizeKHR = vk_tramp_GetRayTracingShaderGroupStackSizeKHR,
    .CmdSetRayTracingPipelineStackSizeKHR = vk_tramp_CmdSetRayTracingPipelineStackSizeKHR,
    .GetImageViewHandleNVX = vk_tramp_GetImageViewHandleNVX,
    .GetImageViewAddressNVX = vk_tramp_GetImageViewAddressNVX,
#ifdef VK_USE_PLATFORM_WIN32_KHR
    .GetDeviceGroupSurfacePresentModes2EXT = vk_tramp_GetDeviceGroupSurfacePresentModes2EXT,
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
    .AcquireFullScreenExclusiveModeEXT = vk_tramp_AcquireFullScreenExclusiveModeEXT,
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
    .ReleaseFullScreenExclusiveModeEXT = vk_tramp_ReleaseFullScreenExclusiveModeEXT,
#endif
    .AcquireProfilingLockKHR = vk_tramp_AcquireProfilingLockKHR,
    .ReleaseProfilingLockKHR = vk_tramp_ReleaseProfilingLockKHR,
    .GetImageDrmFormatModifierPropertiesEXT = vk_tramp_GetImageDrmFormatModifierPropertiesEXT,
    .GetBufferOpaqueCaptureAddress = vk_tramp_GetBufferOpaqueCaptureAddress,
        .GetBufferDeviceAddress = vk_tramp_GetBufferDeviceAddress,
            .InitializePerformanceApiINTEL = vk_tramp_InitializePerformanceApiINTEL,
    .UninitializePerformanceApiINTEL = vk_tramp_UninitializePerformanceApiINTEL,
    .CmdSetPerformanceMarkerINTEL = vk_tramp_CmdSetPerformanceMarkerINTEL,
    .CmdSetPerformanceStreamMarkerINTEL = vk_tramp_CmdSetPerformanceStreamMarkerINTEL,
    .CmdSetPerformanceOverrideINTEL = vk_tramp_CmdSetPerformanceOverrideINTEL,
    .AcquirePerformanceConfigurationINTEL = vk_tramp_AcquirePerformanceConfigurationINTEL,
    .ReleasePerformanceConfigurationINTEL = vk_tramp_ReleasePerformanceConfigurationINTEL,
    .QueueSetPerformanceConfigurationINTEL = vk_tramp_QueueSetPerformanceConfigurationINTEL,
    .GetPerformanceParameterINTEL = vk_tramp_GetPerformanceParameterINTEL,
    .GetDeviceMemoryOpaqueCaptureAddress = vk_tramp_GetDeviceMemoryOpaqueCaptureAddress,
        .GetPipelineExecutablePropertiesKHR = vk_tramp_GetPipelineExecutablePropertiesKHR,
    .GetPipelineExecutableStatisticsKHR = vk_tramp_GetPipelineExecutableStatisticsKHR,
    .GetPipelineExecutableInternalRepresentationsKHR = vk_tramp_GetPipelineExecutableInternalRepresentationsKHR,
    .CmdSetLineStippleEXT = vk_tramp_CmdSetLineStippleEXT,
    .CreateAccelerationStructureKHR = vk_tramp_CreateAccelerationStructureKHR,
    .CmdBuildAccelerationStructuresKHR = vk_tramp_CmdBuildAccelerationStructuresKHR,
    .CmdBuildAccelerationStructuresIndirectKHR = vk_tramp_CmdBuildAccelerationStructuresIndirectKHR,
    .BuildAccelerationStructuresKHR = vk_tramp_BuildAccelerationStructuresKHR,
    .GetAccelerationStructureDeviceAddressKHR = vk_tramp_GetAccelerationStructureDeviceAddressKHR,
    .CreateDeferredOperationKHR = vk_tramp_CreateDeferredOperationKHR,
    .DestroyDeferredOperationKHR = vk_tramp_DestroyDeferredOperationKHR,
    .GetDeferredOperationMaxConcurrencyKHR = vk_tramp_GetDeferredOperationMaxConcurrencyKHR,
    .GetDeferredOperationResultKHR = vk_tramp_GetDeferredOperationResultKHR,
    .DeferredOperationJoinKHR = vk_tramp_DeferredOperationJoinKHR,
    .CmdSetCullModeEXT = vk_tramp_CmdSetCullModeEXT,
    .CmdSetFrontFaceEXT = vk_tramp_CmdSetFrontFaceEXT,
    .CmdSetPrimitiveTopologyEXT = vk_tramp_CmdSetPrimitiveTopologyEXT,
    .CmdSetViewportWithCountEXT = vk_tramp_CmdSetViewportWithCountEXT,
    .CmdSetScissorWithCountEXT = vk_tramp_CmdSetScissorWithCountEXT,
    .CmdBindVertexBuffers2EXT = vk_tramp_CmdBindVertexBuffers2EXT,
    .CmdSetDepthTestEnableEXT = vk_tramp_CmdSetDepthTestEnableEXT,
    .CmdSetDepthWriteEnableEXT = vk_tramp_CmdSetDepthWriteEnableEXT,
    .CmdSetDepthCompareOpEXT = vk_tramp_CmdSetDepthCompareOpEXT,
    .CmdSetDepthBoundsTestEnableEXT = vk_tramp_CmdSetDepthBoundsTestEnableEXT,
    .CmdSetStencilTestEnableEXT = vk_tramp_CmdSetStencilTestEnableEXT,
    .CmdSetStencilOpEXT = vk_tramp_CmdSetStencilOpEXT,
    .CreatePrivateDataSlotEXT = vk_tramp_CreatePrivateDataSlotEXT,
    .DestroyPrivateDataSlotEXT = vk_tramp_DestroyPrivateDataSlotEXT,
    .SetPrivateDataEXT = vk_tramp_SetPrivateDataEXT,
    .GetPrivateDataEXT = vk_tramp_GetPrivateDataEXT,
    .CmdCopyBuffer2KHR = vk_tramp_CmdCopyBuffer2KHR,
    .CmdCopyImage2KHR = vk_tramp_CmdCopyImage2KHR,
    .CmdBlitImage2KHR = vk_tramp_CmdBlitImage2KHR,
    .CmdCopyBufferToImage2KHR = vk_tramp_CmdCopyBufferToImage2KHR,
    .CmdCopyImageToBuffer2KHR = vk_tramp_CmdCopyImageToBuffer2KHR,
    .CmdResolveImage2KHR = vk_tramp_CmdResolveImage2KHR,
    .CmdSetFragmentShadingRateKHR = vk_tramp_CmdSetFragmentShadingRateKHR,
    .CmdSetFragmentShadingRateEnumNV = vk_tramp_CmdSetFragmentShadingRateEnumNV,
    .GetAccelerationStructureBuildSizesKHR = vk_tramp_GetAccelerationStructureBuildSizesKHR,
};
