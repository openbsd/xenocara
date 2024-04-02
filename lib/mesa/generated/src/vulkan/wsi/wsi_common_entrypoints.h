
/* Copyright © 2015-2021 Intel Corporation
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
/* This file generated from vk_entrypoints_gen.py, don't edit directly. */

#include "vk_dispatch_table.h"


#ifndef WSI_COMMON_ENTRYPOINTS_H
#define WSI_COMMON_ENTRYPOINTS_H

#ifdef __cplusplus
extern "C" {
#endif

/* clang wants function declarations in the header to have weak attribute */
#if !defined(_MSC_VER) && !defined(__MINGW32__)
#define ATTR_WEAK __attribute__ ((weak))
#else
#define ATTR_WEAK
#endif

extern const struct vk_instance_entrypoint_table wsi_instance_entrypoints;

extern const struct vk_physical_device_entrypoint_table wsi_physical_device_entrypoints;

extern const struct vk_device_entrypoint_table wsi_device_entrypoints;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateInstance(const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance);
  VKAPI_ATTR void VKAPI_CALL wsi_DestroyInstance(VkInstance instance, const VkAllocationCallbacks* pAllocator);
  VKAPI_ATTR VkResult VKAPI_CALL wsi_EnumeratePhysicalDevices(VkInstance instance, uint32_t* pPhysicalDeviceCount, VkPhysicalDevice* pPhysicalDevices);
  VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL wsi_GetInstanceProcAddr(VkInstance instance, const char* pName);
  VKAPI_ATTR VkResult VKAPI_CALL wsi_EnumerateInstanceVersion(uint32_t* pApiVersion);
  VKAPI_ATTR VkResult VKAPI_CALL wsi_EnumerateInstanceLayerProperties(uint32_t* pPropertyCount, VkLayerProperties* pProperties);
  VKAPI_ATTR VkResult VKAPI_CALL wsi_EnumerateInstanceExtensionProperties(const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties);
#ifdef VK_USE_PLATFORM_ANDROID_KHR
  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateAndroidSurfaceKHR(VkInstance instance, const VkAndroidSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_ANDROID_KHR
  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateDisplayPlaneSurfaceKHR(VkInstance instance, const VkDisplaySurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
  VKAPI_ATTR void VKAPI_CALL wsi_DestroySurfaceKHR(VkInstance instance, VkSurfaceKHR surface, const VkAllocationCallbacks* pAllocator);
#ifdef VK_USE_PLATFORM_VI_NN
  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateViSurfaceNN(VkInstance instance, const VkViSurfaceCreateInfoNN* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_VI_NN
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateWaylandSurfaceKHR(VkInstance instance, const VkWaylandSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_WAYLAND_KHR
#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateWin32SurfaceKHR(VkInstance instance, const VkWin32SurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_WIN32_KHR
#ifdef VK_USE_PLATFORM_XLIB_KHR
  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateXlibSurfaceKHR(VkInstance instance, const VkXlibSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_XLIB_KHR
#ifdef VK_USE_PLATFORM_XCB_KHR
  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateXcbSurfaceKHR(VkInstance instance, const VkXcbSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_XCB_KHR
#ifdef VK_USE_PLATFORM_DIRECTFB_EXT
  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateDirectFBSurfaceEXT(VkInstance instance, const VkDirectFBSurfaceCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_DIRECTFB_EXT
#ifdef VK_USE_PLATFORM_FUCHSIA
  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateImagePipeSurfaceFUCHSIA(VkInstance instance, const VkImagePipeSurfaceCreateInfoFUCHSIA* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_FUCHSIA
#ifdef VK_USE_PLATFORM_GGP
  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateStreamDescriptorSurfaceGGP(VkInstance instance, const VkStreamDescriptorSurfaceCreateInfoGGP* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_GGP
#ifdef VK_USE_PLATFORM_SCREEN_QNX
  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateScreenSurfaceQNX(VkInstance instance, const VkScreenSurfaceCreateInfoQNX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_SCREEN_QNX
  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);
  VKAPI_ATTR void VKAPI_CALL wsi_DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator);
  VKAPI_ATTR void VKAPI_CALL wsi_DebugReportMessageEXT(VkInstance instance, VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage);
  VKAPI_ATTR VkResult VKAPI_CALL wsi_EnumeratePhysicalDeviceGroups(VkInstance instance, uint32_t* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties);
  VKAPI_ATTR VkResult VKAPI_CALL wsi_EnumeratePhysicalDeviceGroupsKHR(VkInstance instance, uint32_t* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties);
#ifdef VK_USE_PLATFORM_IOS_MVK
  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateIOSSurfaceMVK(VkInstance instance, const VkIOSSurfaceCreateInfoMVK* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_IOS_MVK
#ifdef VK_USE_PLATFORM_MACOS_MVK
  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateMacOSSurfaceMVK(VkInstance instance, const VkMacOSSurfaceCreateInfoMVK* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_MACOS_MVK
#ifdef VK_USE_PLATFORM_METAL_EXT
  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateMetalSurfaceEXT(VkInstance instance, const VkMetalSurfaceCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_METAL_EXT
  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pMessenger);
  VKAPI_ATTR void VKAPI_CALL wsi_DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT messenger, const VkAllocationCallbacks* pAllocator);
  VKAPI_ATTR void VKAPI_CALL wsi_SubmitDebugUtilsMessageEXT(VkInstance instance, VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData);
  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateHeadlessSurfaceEXT(VkInstance instance, const VkHeadlessSurfaceCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);

  VKAPI_ATTR void VKAPI_CALL wsi_GetPhysicalDeviceProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties* pProperties);
  VKAPI_ATTR void VKAPI_CALL wsi_GetPhysicalDeviceQueueFamilyProperties(VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties* pQueueFamilyProperties);
  VKAPI_ATTR void VKAPI_CALL wsi_GetPhysicalDeviceMemoryProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties* pMemoryProperties);
  VKAPI_ATTR void VKAPI_CALL wsi_GetPhysicalDeviceFeatures(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures* pFeatures);
  VKAPI_ATTR void VKAPI_CALL wsi_GetPhysicalDeviceFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties* pFormatProperties);
  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetPhysicalDeviceImageFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkImageFormatProperties* pImageFormatProperties);
  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateDevice(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDevice* pDevice);
  VKAPI_ATTR VkResult VKAPI_CALL wsi_EnumerateDeviceLayerProperties(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkLayerProperties* pProperties);
  VKAPI_ATTR VkResult VKAPI_CALL wsi_EnumerateDeviceExtensionProperties(VkPhysicalDevice physicalDevice, const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties);
  VKAPI_ATTR void VKAPI_CALL wsi_GetPhysicalDeviceSparseImageFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkSampleCountFlagBits samples, VkImageUsageFlags usage, VkImageTiling tiling, uint32_t* pPropertyCount, VkSparseImageFormatProperties* pProperties);
  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetPhysicalDeviceDisplayPropertiesKHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPropertiesKHR* pProperties);
  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetPhysicalDeviceDisplayPlanePropertiesKHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPlanePropertiesKHR* pProperties);
  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetDisplayPlaneSupportedDisplaysKHR(VkPhysicalDevice physicalDevice, uint32_t planeIndex, uint32_t* pDisplayCount, VkDisplayKHR* pDisplays);
  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetDisplayModePropertiesKHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModePropertiesKHR* pProperties);
  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateDisplayModeKHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, const VkDisplayModeCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDisplayModeKHR* pMode);
  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetDisplayPlaneCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkDisplayModeKHR mode, uint32_t planeIndex, VkDisplayPlaneCapabilitiesKHR* pCapabilities);
  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetPhysicalDeviceSurfaceSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported);
  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetPhysicalDeviceSurfaceCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR* pSurfaceCapabilities);
  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetPhysicalDeviceSurfaceFormatsKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pSurfaceFormatCount, VkSurfaceFormatKHR* pSurfaceFormats);
  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetPhysicalDeviceSurfacePresentModesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes);
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
  VKAPI_ATTR VkBool32 VKAPI_CALL wsi_GetPhysicalDeviceWaylandPresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, struct wl_display* display);
#endif // VK_USE_PLATFORM_WAYLAND_KHR
#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkBool32 VKAPI_CALL wsi_GetPhysicalDeviceWin32PresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex);
#endif // VK_USE_PLATFORM_WIN32_KHR
#ifdef VK_USE_PLATFORM_XLIB_KHR
  VKAPI_ATTR VkBool32 VKAPI_CALL wsi_GetPhysicalDeviceXlibPresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, Display* dpy, VisualID visualID);
#endif // VK_USE_PLATFORM_XLIB_KHR
#ifdef VK_USE_PLATFORM_XCB_KHR
  VKAPI_ATTR VkBool32 VKAPI_CALL wsi_GetPhysicalDeviceXcbPresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, xcb_connection_t* connection, xcb_visualid_t visual_id);
#endif // VK_USE_PLATFORM_XCB_KHR
#ifdef VK_USE_PLATFORM_DIRECTFB_EXT
  VKAPI_ATTR VkBool32 VKAPI_CALL wsi_GetPhysicalDeviceDirectFBPresentationSupportEXT(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, IDirectFB* dfb);
#endif // VK_USE_PLATFORM_DIRECTFB_EXT
#ifdef VK_USE_PLATFORM_SCREEN_QNX
  VKAPI_ATTR VkBool32 VKAPI_CALL wsi_GetPhysicalDeviceScreenPresentationSupportQNX(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, struct _screen_window* window);
#endif // VK_USE_PLATFORM_SCREEN_QNX
  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetPhysicalDeviceExternalImageFormatPropertiesNV(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkExternalMemoryHandleTypeFlagsNV externalHandleType, VkExternalImageFormatPropertiesNV* pExternalImageFormatProperties);
  VKAPI_ATTR void VKAPI_CALL wsi_GetPhysicalDeviceFeatures2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures);
  VKAPI_ATTR void VKAPI_CALL wsi_GetPhysicalDeviceFeatures2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures);
  VKAPI_ATTR void VKAPI_CALL wsi_GetPhysicalDeviceProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties);
  VKAPI_ATTR void VKAPI_CALL wsi_GetPhysicalDeviceProperties2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties);
  VKAPI_ATTR void VKAPI_CALL wsi_GetPhysicalDeviceFormatProperties2(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2* pFormatProperties);
  VKAPI_ATTR void VKAPI_CALL wsi_GetPhysicalDeviceFormatProperties2KHR(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2* pFormatProperties);
  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetPhysicalDeviceImageFormatProperties2(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties);
  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetPhysicalDeviceImageFormatProperties2KHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties);
  VKAPI_ATTR void VKAPI_CALL wsi_GetPhysicalDeviceQueueFamilyProperties2(VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties2* pQueueFamilyProperties);
  VKAPI_ATTR void VKAPI_CALL wsi_GetPhysicalDeviceQueueFamilyProperties2KHR(VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties2* pQueueFamilyProperties);
  VKAPI_ATTR void VKAPI_CALL wsi_GetPhysicalDeviceMemoryProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties);
  VKAPI_ATTR void VKAPI_CALL wsi_GetPhysicalDeviceMemoryProperties2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties);
  VKAPI_ATTR void VKAPI_CALL wsi_GetPhysicalDeviceSparseImageFormatProperties2(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo, uint32_t* pPropertyCount, VkSparseImageFormatProperties2* pProperties);
  VKAPI_ATTR void VKAPI_CALL wsi_GetPhysicalDeviceSparseImageFormatProperties2KHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo, uint32_t* pPropertyCount, VkSparseImageFormatProperties2* pProperties);
  VKAPI_ATTR void VKAPI_CALL wsi_GetPhysicalDeviceExternalBufferProperties(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties);
  VKAPI_ATTR void VKAPI_CALL wsi_GetPhysicalDeviceExternalBufferPropertiesKHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties);
  VKAPI_ATTR void VKAPI_CALL wsi_GetPhysicalDeviceExternalSemaphoreProperties(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties);
  VKAPI_ATTR void VKAPI_CALL wsi_GetPhysicalDeviceExternalSemaphorePropertiesKHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties);
  VKAPI_ATTR void VKAPI_CALL wsi_GetPhysicalDeviceExternalFenceProperties(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties);
  VKAPI_ATTR void VKAPI_CALL wsi_GetPhysicalDeviceExternalFencePropertiesKHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties);
  VKAPI_ATTR VkResult VKAPI_CALL wsi_ReleaseDisplayEXT(VkPhysicalDevice physicalDevice, VkDisplayKHR display);
#ifdef VK_USE_PLATFORM_XLIB_XRANDR_EXT
  VKAPI_ATTR VkResult VKAPI_CALL wsi_AcquireXlibDisplayEXT(VkPhysicalDevice physicalDevice, Display* dpy, VkDisplayKHR display);
#endif // VK_USE_PLATFORM_XLIB_XRANDR_EXT
#ifdef VK_USE_PLATFORM_XLIB_XRANDR_EXT
  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetRandROutputDisplayEXT(VkPhysicalDevice physicalDevice, Display* dpy, RROutput rrOutput, VkDisplayKHR* pDisplay);
#endif // VK_USE_PLATFORM_XLIB_XRANDR_EXT
#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL wsi_AcquireWinrtDisplayNV(VkPhysicalDevice physicalDevice, VkDisplayKHR display);
#endif // VK_USE_PLATFORM_WIN32_KHR
#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetWinrtDisplayNV(VkPhysicalDevice physicalDevice, uint32_t deviceRelativeId, VkDisplayKHR* pDisplay);
#endif // VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetPhysicalDeviceSurfaceCapabilities2EXT(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilities2EXT* pSurfaceCapabilities);
  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetPhysicalDevicePresentRectanglesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pRectCount, VkRect2D* pRects);
  VKAPI_ATTR void VKAPI_CALL wsi_GetPhysicalDeviceMultisamplePropertiesEXT(VkPhysicalDevice physicalDevice, VkSampleCountFlagBits samples, VkMultisamplePropertiesEXT* pMultisampleProperties);
  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetPhysicalDeviceSurfaceCapabilities2KHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkSurfaceCapabilities2KHR* pSurfaceCapabilities);
  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetPhysicalDeviceSurfaceFormats2KHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pSurfaceFormatCount, VkSurfaceFormat2KHR* pSurfaceFormats);
  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetPhysicalDeviceDisplayProperties2KHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayProperties2KHR* pProperties);
  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetPhysicalDeviceDisplayPlaneProperties2KHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPlaneProperties2KHR* pProperties);
  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetDisplayModeProperties2KHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModeProperties2KHR* pProperties);
  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetDisplayPlaneCapabilities2KHR(VkPhysicalDevice physicalDevice, const VkDisplayPlaneInfo2KHR* pDisplayPlaneInfo, VkDisplayPlaneCapabilities2KHR* pCapabilities);
  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetPhysicalDeviceCalibrateableTimeDomainsEXT(VkPhysicalDevice physicalDevice, uint32_t* pTimeDomainCount, VkTimeDomainEXT* pTimeDomains);
  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetPhysicalDeviceCooperativeMatrixPropertiesNV(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkCooperativeMatrixPropertiesNV* pProperties);
#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetPhysicalDeviceSurfacePresentModes2EXT(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes);
#endif // VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL wsi_EnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, uint32_t* pCounterCount, VkPerformanceCounterKHR* pCounters, VkPerformanceCounterDescriptionKHR* pCounterDescriptions);
  VKAPI_ATTR void VKAPI_CALL wsi_GetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR(VkPhysicalDevice physicalDevice, const VkQueryPoolPerformanceCreateInfoKHR* pPerformanceQueryCreateInfo, uint32_t* pNumPasses);
  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV(VkPhysicalDevice physicalDevice, uint32_t* pCombinationCount, VkFramebufferMixedSamplesCombinationNV* pCombinations);
  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetPhysicalDeviceToolProperties(VkPhysicalDevice physicalDevice, uint32_t* pToolCount, VkPhysicalDeviceToolProperties* pToolProperties);
  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetPhysicalDeviceToolPropertiesEXT(VkPhysicalDevice physicalDevice, uint32_t* pToolCount, VkPhysicalDeviceToolProperties* pToolProperties);
  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetPhysicalDeviceFragmentShadingRatesKHR(VkPhysicalDevice physicalDevice, uint32_t* pFragmentShadingRateCount, VkPhysicalDeviceFragmentShadingRateKHR* pFragmentShadingRates);
  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetPhysicalDeviceVideoCapabilitiesKHR(VkPhysicalDevice physicalDevice, const VkVideoProfileInfoKHR* pVideoProfile, VkVideoCapabilitiesKHR* pCapabilities);
  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetPhysicalDeviceVideoFormatPropertiesKHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceVideoFormatInfoKHR* pVideoFormatInfo, uint32_t* pVideoFormatPropertyCount, VkVideoFormatPropertiesKHR* pVideoFormatProperties);
  VKAPI_ATTR VkResult VKAPI_CALL wsi_AcquireDrmDisplayEXT(VkPhysicalDevice physicalDevice, int32_t drmFd, VkDisplayKHR display);
  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetDrmDisplayEXT(VkPhysicalDevice physicalDevice, int32_t drmFd, uint32_t connectorId, VkDisplayKHR* display);
  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetPhysicalDeviceOpticalFlowImageFormatsNV(VkPhysicalDevice physicalDevice, const VkOpticalFlowImageFormatInfoNV* pOpticalFlowImageFormatInfo, uint32_t* pFormatCount, VkOpticalFlowImageFormatPropertiesNV* pImageFormatProperties);
  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetPhysicalDeviceCooperativeMatrixPropertiesKHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkCooperativeMatrixPropertiesKHR* pProperties);

  VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL wsi_GetDeviceProcAddr(VkDevice device, const char* pName) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_DestroyDevice(VkDevice device, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_GetDeviceQueue(VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_QueueSubmit(VkQueue queue, uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_QueueWaitIdle(VkQueue queue) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_DeviceWaitIdle(VkDevice device) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_AllocateMemory(VkDevice device, const VkMemoryAllocateInfo* pAllocateInfo, const VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_FreeMemory(VkDevice device, VkDeviceMemory memory, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_MapMemory(VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_UnmapMemory(VkDevice device, VkDeviceMemory memory) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_FlushMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_InvalidateMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_GetDeviceMemoryCommitment(VkDevice device, VkDeviceMemory memory, VkDeviceSize* pCommittedMemoryInBytes) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_GetBufferMemoryRequirements(VkDevice device, VkBuffer buffer, VkMemoryRequirements* pMemoryRequirements) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_BindBufferMemory(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_GetImageMemoryRequirements(VkDevice device, VkImage image, VkMemoryRequirements* pMemoryRequirements) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_BindImageMemory(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_GetImageSparseMemoryRequirements(VkDevice device, VkImage image, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements* pSparseMemoryRequirements) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_QueueBindSparse(VkQueue queue, uint32_t bindInfoCount, const VkBindSparseInfo* pBindInfo, VkFence fence) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateFence(VkDevice device, const VkFenceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_DestroyFence(VkDevice device, VkFence fence, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_ResetFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetFenceStatus(VkDevice device, VkFence fence) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_WaitForFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences, VkBool32 waitAll, uint64_t timeout) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateSemaphore(VkDevice device, const VkSemaphoreCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_DestroySemaphore(VkDevice device, VkSemaphore semaphore, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateEvent(VkDevice device, const VkEventCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkEvent* pEvent) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_DestroyEvent(VkDevice device, VkEvent event, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetEventStatus(VkDevice device, VkEvent event) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_SetEvent(VkDevice device, VkEvent event) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_ResetEvent(VkDevice device, VkEvent event) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateQueryPool(VkDevice device, const VkQueryPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkQueryPool* pQueryPool) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_DestroyQueryPool(VkDevice device, VkQueryPool queryPool, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetQueryPoolResults(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, VkDeviceSize stride, VkQueryResultFlags flags) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_ResetQueryPool(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_ResetQueryPoolEXT(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateBuffer(VkDevice device, const VkBufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_DestroyBuffer(VkDevice device, VkBuffer buffer, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateBufferView(VkDevice device, const VkBufferViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferView* pView) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_DestroyBufferView(VkDevice device, VkBufferView bufferView, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateImage(VkDevice device, const VkImageCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImage* pImage) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_DestroyImage(VkDevice device, VkImage image, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_GetImageSubresourceLayout(VkDevice device, VkImage image, const VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateImageView(VkDevice device, const VkImageViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImageView* pView) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_DestroyImageView(VkDevice device, VkImageView imageView, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateShaderModule(VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_DestroyShaderModule(VkDevice device, VkShaderModule shaderModule, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreatePipelineCache(VkDevice device, const VkPipelineCacheCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineCache* pPipelineCache) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_DestroyPipelineCache(VkDevice device, VkPipelineCache pipelineCache, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetPipelineCacheData(VkDevice device, VkPipelineCache pipelineCache, size_t* pDataSize, void* pData) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_MergePipelineCaches(VkDevice device, VkPipelineCache dstCache, uint32_t srcCacheCount, const VkPipelineCache* pSrcCaches) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateGraphicsPipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkGraphicsPipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateComputePipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkComputePipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI(VkDevice device, VkRenderPass renderpass, VkExtent2D* pMaxWorkgroupSize) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_DestroyPipeline(VkDevice device, VkPipeline pipeline, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreatePipelineLayout(VkDevice device, const VkPipelineLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_DestroyPipelineLayout(VkDevice device, VkPipelineLayout pipelineLayout, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateSampler(VkDevice device, const VkSamplerCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSampler* pSampler) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_DestroySampler(VkDevice device, VkSampler sampler, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateDescriptorSetLayout(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorSetLayout* pSetLayout) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_DestroyDescriptorSetLayout(VkDevice device, VkDescriptorSetLayout descriptorSetLayout, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateDescriptorPool(VkDevice device, const VkDescriptorPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorPool* pDescriptorPool) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_DestroyDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_ResetDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags flags) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_AllocateDescriptorSets(VkDevice device, const VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_FreeDescriptorSets(VkDevice device, VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_UpdateDescriptorSets(VkDevice device, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, const VkCopyDescriptorSet* pDescriptorCopies) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateFramebuffer(VkDevice device, const VkFramebufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFramebuffer* pFramebuffer) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_DestroyFramebuffer(VkDevice device, VkFramebuffer framebuffer, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateRenderPass(VkDevice device, const VkRenderPassCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_DestroyRenderPass(VkDevice device, VkRenderPass renderPass, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_GetRenderAreaGranularity(VkDevice device, VkRenderPass renderPass, VkExtent2D* pGranularity) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_GetRenderingAreaGranularityKHR(VkDevice device, const VkRenderingAreaInfoKHR* pRenderingAreaInfo, VkExtent2D* pGranularity) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateCommandPool(VkDevice device, const VkCommandPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_DestroyCommandPool(VkDevice device, VkCommandPool commandPool, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_ResetCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolResetFlags flags) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_AllocateCommandBuffers(VkDevice device, const VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_FreeCommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_BeginCommandBuffer(VkCommandBuffer commandBuffer, const VkCommandBufferBeginInfo* pBeginInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_EndCommandBuffer(VkCommandBuffer commandBuffer) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_ResetCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdBindPipeline(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetAttachmentFeedbackLoopEnableEXT(VkCommandBuffer commandBuffer, VkImageAspectFlags aspectMask) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetViewport(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pViewports) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetScissor(VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pScissors) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetLineWidth(VkCommandBuffer commandBuffer, float lineWidth) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetDepthBias(VkCommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetBlendConstants(VkCommandBuffer commandBuffer, const float blendConstants[4]) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetDepthBounds(VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetStencilCompareMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetStencilWriteMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetStencilReference(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdBindDescriptorSets(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdBindIndexBuffer(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdBindVertexBuffers(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdDraw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdDrawIndexed(VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdDrawMultiEXT(VkCommandBuffer commandBuffer, uint32_t drawCount, const VkMultiDrawInfoEXT* pVertexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdDrawMultiIndexedEXT(VkCommandBuffer commandBuffer, uint32_t drawCount, const VkMultiDrawIndexedInfoEXT* pIndexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride, const int32_t* pVertexOffset) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdDrawIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdDrawIndexedIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdDispatch(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdDispatchIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSubpassShadingHUAWEI(VkCommandBuffer commandBuffer) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdDrawClusterHUAWEI(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdDrawClusterIndirectHUAWEI(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdUpdatePipelineIndirectBufferNV(VkCommandBuffer commandBuffer, VkPipelineBindPoint           pipelineBindPoint, VkPipeline                    pipeline) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdCopyBuffer(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdCopyImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdBlitImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdCopyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdCopyImageToBuffer(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdCopyMemoryIndirectNV(VkCommandBuffer commandBuffer, VkDeviceAddress copyBufferAddress, uint32_t copyCount, uint32_t stride) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdCopyMemoryToImageIndirectNV(VkCommandBuffer commandBuffer, VkDeviceAddress copyBufferAddress, uint32_t copyCount, uint32_t stride, VkImage dstImage, VkImageLayout dstImageLayout, const VkImageSubresourceLayers* pImageSubresources) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdUpdateBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdFillBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdClearColorImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdClearDepthStencilImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdClearAttachments(VkCommandBuffer commandBuffer, uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdResolveImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageResolve* pRegions) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdResetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdWaitEvents(VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdPipelineBarrier(VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdBeginQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdEndQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdBeginConditionalRenderingEXT(VkCommandBuffer commandBuffer, const VkConditionalRenderingBeginInfoEXT* pConditionalRenderingBegin) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdEndConditionalRenderingEXT(VkCommandBuffer commandBuffer) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdResetQueryPool(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdWriteTimestamp(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdCopyQueryPoolResults(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdPushConstants(VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdBeginRenderPass(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdNextSubpass(VkCommandBuffer commandBuffer, VkSubpassContents contents) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdEndRenderPass(VkCommandBuffer commandBuffer) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdExecuteCommands(VkCommandBuffer commandBuffer, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateSharedSwapchainsKHR(VkDevice device, uint32_t swapchainCount, const VkSwapchainCreateInfoKHR* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchains) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateSwapchainKHR(VkDevice device, const VkSwapchainCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_DestroySwapchainKHR(VkDevice device, VkSwapchainKHR swapchain, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetSwapchainImagesKHR(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_AcquireNextImageKHR(VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_QueuePresentKHR(VkQueue queue, const VkPresentInfoKHR* pPresentInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_DebugMarkerSetObjectNameEXT(VkDevice device, const VkDebugMarkerObjectNameInfoEXT* pNameInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_DebugMarkerSetObjectTagEXT(VkDevice device, const VkDebugMarkerObjectTagInfoEXT* pTagInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdDebugMarkerBeginEXT(VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdDebugMarkerEndEXT(VkCommandBuffer commandBuffer) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdDebugMarkerInsertEXT(VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo) ATTR_WEAK;


#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetMemoryWin32HandleNV(VkDevice device, VkDeviceMemory memory, VkExternalMemoryHandleTypeFlagsNV handleType, HANDLE* pHandle) ATTR_WEAK;


#endif // VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR void VKAPI_CALL wsi_CmdExecuteGeneratedCommandsNV(VkCommandBuffer commandBuffer, VkBool32 isPreprocessed, const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdPreprocessGeneratedCommandsNV(VkCommandBuffer commandBuffer, const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdBindPipelineShaderGroupNV(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline, uint32_t groupIndex) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_GetGeneratedCommandsMemoryRequirementsNV(VkDevice device, const VkGeneratedCommandsMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateIndirectCommandsLayoutNV(VkDevice device, const VkIndirectCommandsLayoutCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkIndirectCommandsLayoutNV* pIndirectCommandsLayout) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_DestroyIndirectCommandsLayoutNV(VkDevice device, VkIndirectCommandsLayoutNV indirectCommandsLayout, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdPushDescriptorSetKHR(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_TrimCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_TrimCommandPoolKHR(VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags) ATTR_WEAK;


#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetMemoryWin32HandleKHR(VkDevice device, const VkMemoryGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle) ATTR_WEAK;


#endif // VK_USE_PLATFORM_WIN32_KHR
#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetMemoryWin32HandlePropertiesKHR(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, HANDLE handle, VkMemoryWin32HandlePropertiesKHR* pMemoryWin32HandleProperties) ATTR_WEAK;


#endif // VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetMemoryFdKHR(VkDevice device, const VkMemoryGetFdInfoKHR* pGetFdInfo, int* pFd) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetMemoryFdPropertiesKHR(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, int fd, VkMemoryFdPropertiesKHR* pMemoryFdProperties) ATTR_WEAK;


#ifdef VK_USE_PLATFORM_FUCHSIA
  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetMemoryZirconHandleFUCHSIA(VkDevice device, const VkMemoryGetZirconHandleInfoFUCHSIA* pGetZirconHandleInfo, zx_handle_t* pZirconHandle) ATTR_WEAK;


#endif // VK_USE_PLATFORM_FUCHSIA
#ifdef VK_USE_PLATFORM_FUCHSIA
  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetMemoryZirconHandlePropertiesFUCHSIA(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, zx_handle_t zirconHandle, VkMemoryZirconHandlePropertiesFUCHSIA* pMemoryZirconHandleProperties) ATTR_WEAK;


#endif // VK_USE_PLATFORM_FUCHSIA
  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetMemoryRemoteAddressNV(VkDevice device, const VkMemoryGetRemoteAddressInfoNV* pMemoryGetRemoteAddressInfo, VkRemoteAddressNV* pAddress) ATTR_WEAK;


#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetSemaphoreWin32HandleKHR(VkDevice device, const VkSemaphoreGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle) ATTR_WEAK;


#endif // VK_USE_PLATFORM_WIN32_KHR
#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL wsi_ImportSemaphoreWin32HandleKHR(VkDevice device, const VkImportSemaphoreWin32HandleInfoKHR* pImportSemaphoreWin32HandleInfo) ATTR_WEAK;


#endif // VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetSemaphoreFdKHR(VkDevice device, const VkSemaphoreGetFdInfoKHR* pGetFdInfo, int* pFd) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_ImportSemaphoreFdKHR(VkDevice device, const VkImportSemaphoreFdInfoKHR* pImportSemaphoreFdInfo) ATTR_WEAK;


#ifdef VK_USE_PLATFORM_FUCHSIA
  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetSemaphoreZirconHandleFUCHSIA(VkDevice device, const VkSemaphoreGetZirconHandleInfoFUCHSIA* pGetZirconHandleInfo, zx_handle_t* pZirconHandle) ATTR_WEAK;


#endif // VK_USE_PLATFORM_FUCHSIA
#ifdef VK_USE_PLATFORM_FUCHSIA
  VKAPI_ATTR VkResult VKAPI_CALL wsi_ImportSemaphoreZirconHandleFUCHSIA(VkDevice device, const VkImportSemaphoreZirconHandleInfoFUCHSIA* pImportSemaphoreZirconHandleInfo) ATTR_WEAK;


#endif // VK_USE_PLATFORM_FUCHSIA
#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetFenceWin32HandleKHR(VkDevice device, const VkFenceGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle) ATTR_WEAK;


#endif // VK_USE_PLATFORM_WIN32_KHR
#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL wsi_ImportFenceWin32HandleKHR(VkDevice device, const VkImportFenceWin32HandleInfoKHR* pImportFenceWin32HandleInfo) ATTR_WEAK;


#endif // VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetFenceFdKHR(VkDevice device, const VkFenceGetFdInfoKHR* pGetFdInfo, int* pFd) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_ImportFenceFdKHR(VkDevice device, const VkImportFenceFdInfoKHR* pImportFenceFdInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_DisplayPowerControlEXT(VkDevice device, VkDisplayKHR display, const VkDisplayPowerInfoEXT* pDisplayPowerInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_RegisterDeviceEventEXT(VkDevice device, const VkDeviceEventInfoEXT* pDeviceEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_RegisterDisplayEventEXT(VkDevice device, VkDisplayKHR display, const VkDisplayEventInfoEXT* pDisplayEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetSwapchainCounterEXT(VkDevice device, VkSwapchainKHR swapchain, VkSurfaceCounterFlagBitsEXT counter, uint64_t* pCounterValue) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_GetDeviceGroupPeerMemoryFeatures(VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_GetDeviceGroupPeerMemoryFeaturesKHR(VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_BindBufferMemory2(VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_BindBufferMemory2KHR(VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_BindImageMemory2(VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_BindImageMemory2KHR(VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetDeviceMask(VkCommandBuffer commandBuffer, uint32_t deviceMask) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetDeviceMaskKHR(VkCommandBuffer commandBuffer, uint32_t deviceMask) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetDeviceGroupPresentCapabilitiesKHR(VkDevice device, VkDeviceGroupPresentCapabilitiesKHR* pDeviceGroupPresentCapabilities) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetDeviceGroupSurfacePresentModesKHR(VkDevice device, VkSurfaceKHR surface, VkDeviceGroupPresentModeFlagsKHR* pModes) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_AcquireNextImage2KHR(VkDevice device, const VkAcquireNextImageInfoKHR* pAcquireInfo, uint32_t* pImageIndex) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdDispatchBase(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdDispatchBaseKHR(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateDescriptorUpdateTemplate(VkDevice device, const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateDescriptorUpdateTemplateKHR(VkDevice device, const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_DestroyDescriptorUpdateTemplate(VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_DestroyDescriptorUpdateTemplateKHR(VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_UpdateDescriptorSetWithTemplate(VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_UpdateDescriptorSetWithTemplateKHR(VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdPushDescriptorSetWithTemplateKHR(VkCommandBuffer commandBuffer, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, const void* pData) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_SetHdrMetadataEXT(VkDevice device, uint32_t swapchainCount, const VkSwapchainKHR* pSwapchains, const VkHdrMetadataEXT* pMetadata) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetSwapchainStatusKHR(VkDevice device, VkSwapchainKHR swapchain) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetRefreshCycleDurationGOOGLE(VkDevice device, VkSwapchainKHR swapchain, VkRefreshCycleDurationGOOGLE* pDisplayTimingProperties) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetPastPresentationTimingGOOGLE(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pPresentationTimingCount, VkPastPresentationTimingGOOGLE* pPresentationTimings) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetViewportWScalingNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewportWScalingNV* pViewportWScalings) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetDiscardRectangleEXT(VkCommandBuffer commandBuffer, uint32_t firstDiscardRectangle, uint32_t discardRectangleCount, const VkRect2D* pDiscardRectangles) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetDiscardRectangleEnableEXT(VkCommandBuffer commandBuffer, VkBool32 discardRectangleEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetDiscardRectangleModeEXT(VkCommandBuffer commandBuffer, VkDiscardRectangleModeEXT discardRectangleMode) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetSampleLocationsEXT(VkCommandBuffer commandBuffer, const VkSampleLocationsInfoEXT* pSampleLocationsInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_GetBufferMemoryRequirements2(VkDevice device, const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_GetBufferMemoryRequirements2KHR(VkDevice device, const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_GetImageMemoryRequirements2(VkDevice device, const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_GetImageMemoryRequirements2KHR(VkDevice device, const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_GetImageSparseMemoryRequirements2(VkDevice device, const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_GetImageSparseMemoryRequirements2KHR(VkDevice device, const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_GetDeviceBufferMemoryRequirements(VkDevice device, const VkDeviceBufferMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_GetDeviceBufferMemoryRequirementsKHR(VkDevice device, const VkDeviceBufferMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_GetDeviceImageMemoryRequirements(VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_GetDeviceImageMemoryRequirementsKHR(VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_GetDeviceImageSparseMemoryRequirements(VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_GetDeviceImageSparseMemoryRequirementsKHR(VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateSamplerYcbcrConversion(VkDevice device, const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateSamplerYcbcrConversionKHR(VkDevice device, const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_DestroySamplerYcbcrConversion(VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_DestroySamplerYcbcrConversionKHR(VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_GetDeviceQueue2(VkDevice device, const VkDeviceQueueInfo2* pQueueInfo, VkQueue* pQueue) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateValidationCacheEXT(VkDevice device, const VkValidationCacheCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkValidationCacheEXT* pValidationCache) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_DestroyValidationCacheEXT(VkDevice device, VkValidationCacheEXT validationCache, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetValidationCacheDataEXT(VkDevice device, VkValidationCacheEXT validationCache, size_t* pDataSize, void* pData) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_MergeValidationCachesEXT(VkDevice device, VkValidationCacheEXT dstCache, uint32_t srcCacheCount, const VkValidationCacheEXT* pSrcCaches) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_GetDescriptorSetLayoutSupport(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_GetDescriptorSetLayoutSupportKHR(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport) ATTR_WEAK;


#ifdef VK_USE_PLATFORM_ANDROID_KHR
  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetSwapchainGrallocUsageANDROID(VkDevice device, VkFormat format, VkImageUsageFlags imageUsage, int* grallocUsage) ATTR_WEAK;


#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetSwapchainGrallocUsage2ANDROID(VkDevice device, VkFormat format, VkImageUsageFlags imageUsage, VkSwapchainImageUsageFlagsANDROID swapchainImageUsage, uint64_t* grallocConsumerUsage, uint64_t* grallocProducerUsage) ATTR_WEAK;


#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
  VKAPI_ATTR VkResult VKAPI_CALL wsi_AcquireImageANDROID(VkDevice device, VkImage image, int nativeFenceFd, VkSemaphore semaphore, VkFence fence) ATTR_WEAK;


#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
  VKAPI_ATTR VkResult VKAPI_CALL wsi_QueueSignalReleaseImageANDROID(VkQueue queue, uint32_t waitSemaphoreCount, const VkSemaphore* pWaitSemaphores, VkImage image, int* pNativeFenceFd) ATTR_WEAK;


#endif // VK_USE_PLATFORM_ANDROID_KHR
  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetShaderInfoAMD(VkDevice device, VkPipeline pipeline, VkShaderStageFlagBits shaderStage, VkShaderInfoTypeAMD infoType, size_t* pInfoSize, void* pInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_SetLocalDimmingAMD(VkDevice device, VkSwapchainKHR swapChain, VkBool32 localDimmingEnable) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetCalibratedTimestampsEXT(VkDevice device, uint32_t timestampCount, const VkCalibratedTimestampInfoEXT* pTimestampInfos, uint64_t* pTimestamps, uint64_t* pMaxDeviation) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_SetDebugUtilsObjectNameEXT(VkDevice device, const VkDebugUtilsObjectNameInfoEXT* pNameInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_SetDebugUtilsObjectTagEXT(VkDevice device, const VkDebugUtilsObjectTagInfoEXT* pTagInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_QueueBeginDebugUtilsLabelEXT(VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_QueueEndDebugUtilsLabelEXT(VkQueue queue) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_QueueInsertDebugUtilsLabelEXT(VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdBeginDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdEndDebugUtilsLabelEXT(VkCommandBuffer commandBuffer) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdInsertDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetMemoryHostPointerPropertiesEXT(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, const void* pHostPointer, VkMemoryHostPointerPropertiesEXT* pMemoryHostPointerProperties) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdWriteBufferMarkerAMD(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateRenderPass2(VkDevice device, const VkRenderPassCreateInfo2* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateRenderPass2KHR(VkDevice device, const VkRenderPassCreateInfo2* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdBeginRenderPass2(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo*      pRenderPassBegin, const VkSubpassBeginInfo*      pSubpassBeginInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdBeginRenderPass2KHR(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo*      pRenderPassBegin, const VkSubpassBeginInfo*      pSubpassBeginInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdNextSubpass2(VkCommandBuffer commandBuffer, const VkSubpassBeginInfo*      pSubpassBeginInfo, const VkSubpassEndInfo*        pSubpassEndInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdNextSubpass2KHR(VkCommandBuffer commandBuffer, const VkSubpassBeginInfo*      pSubpassBeginInfo, const VkSubpassEndInfo*        pSubpassEndInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdEndRenderPass2(VkCommandBuffer commandBuffer, const VkSubpassEndInfo*        pSubpassEndInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdEndRenderPass2KHR(VkCommandBuffer commandBuffer, const VkSubpassEndInfo*        pSubpassEndInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetSemaphoreCounterValue(VkDevice device, VkSemaphore semaphore, uint64_t* pValue) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetSemaphoreCounterValueKHR(VkDevice device, VkSemaphore semaphore, uint64_t* pValue) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_WaitSemaphores(VkDevice device, const VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_WaitSemaphoresKHR(VkDevice device, const VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_SignalSemaphore(VkDevice device, const VkSemaphoreSignalInfo* pSignalInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_SignalSemaphoreKHR(VkDevice device, const VkSemaphoreSignalInfo* pSignalInfo) ATTR_WEAK;


#ifdef VK_USE_PLATFORM_ANDROID_KHR
  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetAndroidHardwareBufferPropertiesANDROID(VkDevice device, const struct AHardwareBuffer* buffer, VkAndroidHardwareBufferPropertiesANDROID* pProperties) ATTR_WEAK;


#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetMemoryAndroidHardwareBufferANDROID(VkDevice device, const VkMemoryGetAndroidHardwareBufferInfoANDROID* pInfo, struct AHardwareBuffer** pBuffer) ATTR_WEAK;


#endif // VK_USE_PLATFORM_ANDROID_KHR
  VKAPI_ATTR void VKAPI_CALL wsi_CmdDrawIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdDrawIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdDrawIndirectCountAMD(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdDrawIndexedIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdDrawIndexedIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdDrawIndexedIndirectCountAMD(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetCheckpointNV(VkCommandBuffer commandBuffer, const void* pCheckpointMarker) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_GetQueueCheckpointDataNV(VkQueue queue, uint32_t* pCheckpointDataCount, VkCheckpointDataNV* pCheckpointData) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdBindTransformFeedbackBuffersEXT(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdBeginTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdEndTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdBeginQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags, uint32_t index) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdEndQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, uint32_t index) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdDrawIndirectByteCountEXT(VkCommandBuffer commandBuffer, uint32_t instanceCount, uint32_t firstInstance, VkBuffer counterBuffer, VkDeviceSize counterBufferOffset, uint32_t counterOffset, uint32_t vertexStride) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetExclusiveScissorNV(VkCommandBuffer commandBuffer, uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, const VkRect2D* pExclusiveScissors) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetExclusiveScissorEnableNV(VkCommandBuffer commandBuffer, uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, const VkBool32* pExclusiveScissorEnables) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdBindShadingRateImageNV(VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetViewportShadingRatePaletteNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkShadingRatePaletteNV* pShadingRatePalettes) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetCoarseSampleOrderNV(VkCommandBuffer commandBuffer, VkCoarseSampleOrderTypeNV sampleOrderType, uint32_t customSampleOrderCount, const VkCoarseSampleOrderCustomNV* pCustomSampleOrders) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdDrawMeshTasksNV(VkCommandBuffer commandBuffer, uint32_t taskCount, uint32_t firstTask) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdDrawMeshTasksIndirectNV(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdDrawMeshTasksIndirectCountNV(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdDrawMeshTasksEXT(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdDrawMeshTasksIndirectEXT(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdDrawMeshTasksIndirectCountEXT(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CompileDeferredNV(VkDevice device, VkPipeline pipeline, uint32_t shader) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateAccelerationStructureNV(VkDevice device, const VkAccelerationStructureCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkAccelerationStructureNV* pAccelerationStructure) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdBindInvocationMaskHUAWEI(VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_DestroyAccelerationStructureKHR(VkDevice device, VkAccelerationStructureKHR accelerationStructure, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_DestroyAccelerationStructureNV(VkDevice device, VkAccelerationStructureNV accelerationStructure, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_GetAccelerationStructureMemoryRequirementsNV(VkDevice device, const VkAccelerationStructureMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2KHR* pMemoryRequirements) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_BindAccelerationStructureMemoryNV(VkDevice device, uint32_t bindInfoCount, const VkBindAccelerationStructureMemoryInfoNV* pBindInfos) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdCopyAccelerationStructureNV(VkCommandBuffer commandBuffer, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkCopyAccelerationStructureModeKHR mode) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdCopyAccelerationStructureKHR(VkCommandBuffer commandBuffer, const VkCopyAccelerationStructureInfoKHR* pInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CopyAccelerationStructureKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyAccelerationStructureInfoKHR* pInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdCopyAccelerationStructureToMemoryKHR(VkCommandBuffer commandBuffer, const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CopyAccelerationStructureToMemoryKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdCopyMemoryToAccelerationStructureKHR(VkCommandBuffer commandBuffer, const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CopyMemoryToAccelerationStructureKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdWriteAccelerationStructuresPropertiesKHR(VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, const VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdWriteAccelerationStructuresPropertiesNV(VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, const VkAccelerationStructureNV* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdBuildAccelerationStructureNV(VkCommandBuffer commandBuffer, const VkAccelerationStructureInfoNV* pInfo, VkBuffer instanceData, VkDeviceSize instanceOffset, VkBool32 update, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkBuffer scratch, VkDeviceSize scratchOffset) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_WriteAccelerationStructuresPropertiesKHR(VkDevice device, uint32_t accelerationStructureCount, const VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType  queryType, size_t       dataSize, void* pData, size_t stride) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdTraceRaysKHR(VkCommandBuffer commandBuffer, const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, uint32_t width, uint32_t height, uint32_t depth) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdTraceRaysNV(VkCommandBuffer commandBuffer, VkBuffer raygenShaderBindingTableBuffer, VkDeviceSize raygenShaderBindingOffset, VkBuffer missShaderBindingTableBuffer, VkDeviceSize missShaderBindingOffset, VkDeviceSize missShaderBindingStride, VkBuffer hitShaderBindingTableBuffer, VkDeviceSize hitShaderBindingOffset, VkDeviceSize hitShaderBindingStride, VkBuffer callableShaderBindingTableBuffer, VkDeviceSize callableShaderBindingOffset, VkDeviceSize callableShaderBindingStride, uint32_t width, uint32_t height, uint32_t depth) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetRayTracingShaderGroupHandlesKHR(VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetRayTracingShaderGroupHandlesNV(VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetRayTracingCaptureReplayShaderGroupHandlesKHR(VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetAccelerationStructureHandleNV(VkDevice device, VkAccelerationStructureNV accelerationStructure, size_t dataSize, void* pData) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateRayTracingPipelinesNV(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkRayTracingPipelineCreateInfoNV* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateRayTracingPipelinesKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkRayTracingPipelineCreateInfoKHR* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdTraceRaysIndirectKHR(VkCommandBuffer commandBuffer, const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, VkDeviceAddress indirectDeviceAddress) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdTraceRaysIndirect2KHR(VkCommandBuffer commandBuffer, VkDeviceAddress indirectDeviceAddress) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_GetDeviceAccelerationStructureCompatibilityKHR(VkDevice device, const VkAccelerationStructureVersionInfoKHR* pVersionInfo, VkAccelerationStructureCompatibilityKHR* pCompatibility) ATTR_WEAK;


  VKAPI_ATTR VkDeviceSize VKAPI_CALL wsi_GetRayTracingShaderGroupStackSizeKHR(VkDevice device, VkPipeline pipeline, uint32_t group, VkShaderGroupShaderKHR groupShader) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetRayTracingPipelineStackSizeKHR(VkCommandBuffer commandBuffer, uint32_t pipelineStackSize) ATTR_WEAK;


  VKAPI_ATTR uint32_t VKAPI_CALL wsi_GetImageViewHandleNVX(VkDevice device, const VkImageViewHandleInfoNVX* pInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetImageViewAddressNVX(VkDevice device, VkImageView imageView, VkImageViewAddressPropertiesNVX* pProperties) ATTR_WEAK;


#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetDeviceGroupSurfacePresentModes2EXT(VkDevice device, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkDeviceGroupPresentModeFlagsKHR* pModes) ATTR_WEAK;


#endif // VK_USE_PLATFORM_WIN32_KHR
#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL wsi_AcquireFullScreenExclusiveModeEXT(VkDevice device, VkSwapchainKHR swapchain) ATTR_WEAK;


#endif // VK_USE_PLATFORM_WIN32_KHR
#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL wsi_ReleaseFullScreenExclusiveModeEXT(VkDevice device, VkSwapchainKHR swapchain) ATTR_WEAK;


#endif // VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL wsi_AcquireProfilingLockKHR(VkDevice device, const VkAcquireProfilingLockInfoKHR* pInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_ReleaseProfilingLockKHR(VkDevice device) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetImageDrmFormatModifierPropertiesEXT(VkDevice device, VkImage image, VkImageDrmFormatModifierPropertiesEXT* pProperties) ATTR_WEAK;


  VKAPI_ATTR uint64_t VKAPI_CALL wsi_GetBufferOpaqueCaptureAddress(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) ATTR_WEAK;


  VKAPI_ATTR uint64_t VKAPI_CALL wsi_GetBufferOpaqueCaptureAddressKHR(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) ATTR_WEAK;


  VKAPI_ATTR VkDeviceAddress VKAPI_CALL wsi_GetBufferDeviceAddress(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) ATTR_WEAK;


  VKAPI_ATTR VkDeviceAddress VKAPI_CALL wsi_GetBufferDeviceAddressKHR(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) ATTR_WEAK;


  VKAPI_ATTR VkDeviceAddress VKAPI_CALL wsi_GetBufferDeviceAddressEXT(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_InitializePerformanceApiINTEL(VkDevice device, const VkInitializePerformanceApiInfoINTEL* pInitializeInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_UninitializePerformanceApiINTEL(VkDevice device) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CmdSetPerformanceMarkerINTEL(VkCommandBuffer commandBuffer, const VkPerformanceMarkerInfoINTEL* pMarkerInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CmdSetPerformanceStreamMarkerINTEL(VkCommandBuffer commandBuffer, const VkPerformanceStreamMarkerInfoINTEL* pMarkerInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CmdSetPerformanceOverrideINTEL(VkCommandBuffer commandBuffer, const VkPerformanceOverrideInfoINTEL* pOverrideInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_AcquirePerformanceConfigurationINTEL(VkDevice device, const VkPerformanceConfigurationAcquireInfoINTEL* pAcquireInfo, VkPerformanceConfigurationINTEL* pConfiguration) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_ReleasePerformanceConfigurationINTEL(VkDevice device, VkPerformanceConfigurationINTEL configuration) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_QueueSetPerformanceConfigurationINTEL(VkQueue queue, VkPerformanceConfigurationINTEL configuration) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetPerformanceParameterINTEL(VkDevice device, VkPerformanceParameterTypeINTEL parameter, VkPerformanceValueINTEL* pValue) ATTR_WEAK;


  VKAPI_ATTR uint64_t VKAPI_CALL wsi_GetDeviceMemoryOpaqueCaptureAddress(VkDevice device, const VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo) ATTR_WEAK;


  VKAPI_ATTR uint64_t VKAPI_CALL wsi_GetDeviceMemoryOpaqueCaptureAddressKHR(VkDevice device, const VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetPipelineExecutablePropertiesKHR(VkDevice                        device, const VkPipelineInfoKHR*        pPipelineInfo, uint32_t* pExecutableCount, VkPipelineExecutablePropertiesKHR* pProperties) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetPipelineExecutableStatisticsKHR(VkDevice                        device, const VkPipelineExecutableInfoKHR*  pExecutableInfo, uint32_t* pStatisticCount, VkPipelineExecutableStatisticKHR* pStatistics) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetPipelineExecutableInternalRepresentationsKHR(VkDevice                        device, const VkPipelineExecutableInfoKHR*  pExecutableInfo, uint32_t* pInternalRepresentationCount, VkPipelineExecutableInternalRepresentationKHR* pInternalRepresentations) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetLineStippleEXT(VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateAccelerationStructureKHR(VkDevice                                           device, const VkAccelerationStructureCreateInfoKHR*        pCreateInfo, const VkAllocationCallbacks*       pAllocator, VkAccelerationStructureKHR*                        pAccelerationStructure) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdBuildAccelerationStructuresKHR(VkCommandBuffer                                    commandBuffer, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdBuildAccelerationStructuresIndirectKHR(VkCommandBuffer                  commandBuffer, uint32_t                                           infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkDeviceAddress*             pIndirectDeviceAddresses, const uint32_t*                    pIndirectStrides, const uint32_t* const*             ppMaxPrimitiveCounts) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_BuildAccelerationStructuresKHR(VkDevice                                           device, VkDeferredOperationKHR deferredOperation, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos) ATTR_WEAK;


  VKAPI_ATTR VkDeviceAddress VKAPI_CALL wsi_GetAccelerationStructureDeviceAddressKHR(VkDevice device, const VkAccelerationStructureDeviceAddressInfoKHR* pInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateDeferredOperationKHR(VkDevice device, const VkAllocationCallbacks* pAllocator, VkDeferredOperationKHR* pDeferredOperation) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_DestroyDeferredOperationKHR(VkDevice device, VkDeferredOperationKHR operation, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR uint32_t VKAPI_CALL wsi_GetDeferredOperationMaxConcurrencyKHR(VkDevice device, VkDeferredOperationKHR operation) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetDeferredOperationResultKHR(VkDevice device, VkDeferredOperationKHR operation) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_DeferredOperationJoinKHR(VkDevice device, VkDeferredOperationKHR operation) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_GetPipelineIndirectMemoryRequirementsNV(VkDevice device, const VkComputePipelineCreateInfo* pCreateInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;


  VKAPI_ATTR VkDeviceAddress VKAPI_CALL wsi_GetPipelineIndirectDeviceAddressNV(VkDevice device, const VkPipelineIndirectDeviceAddressInfoNV* pInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetCullMode(VkCommandBuffer commandBuffer, VkCullModeFlags cullMode) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetCullModeEXT(VkCommandBuffer commandBuffer, VkCullModeFlags cullMode) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetFrontFace(VkCommandBuffer commandBuffer, VkFrontFace frontFace) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetFrontFaceEXT(VkCommandBuffer commandBuffer, VkFrontFace frontFace) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetPrimitiveTopology(VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetPrimitiveTopologyEXT(VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetViewportWithCount(VkCommandBuffer commandBuffer, uint32_t viewportCount, const VkViewport* pViewports) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetViewportWithCountEXT(VkCommandBuffer commandBuffer, uint32_t viewportCount, const VkViewport* pViewports) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetScissorWithCount(VkCommandBuffer commandBuffer, uint32_t scissorCount, const VkRect2D* pScissors) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetScissorWithCountEXT(VkCommandBuffer commandBuffer, uint32_t scissorCount, const VkRect2D* pScissors) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdBindIndexBuffer2KHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkDeviceSize size, VkIndexType indexType) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdBindVertexBuffers2(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes, const VkDeviceSize* pStrides) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdBindVertexBuffers2EXT(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes, const VkDeviceSize* pStrides) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetDepthTestEnable(VkCommandBuffer commandBuffer, VkBool32 depthTestEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetDepthTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthTestEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetDepthWriteEnable(VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetDepthWriteEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetDepthCompareOp(VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetDepthCompareOpEXT(VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetDepthBoundsTestEnable(VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetDepthBoundsTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetStencilTestEnable(VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetStencilTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetStencilOp(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetStencilOpEXT(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetPatchControlPointsEXT(VkCommandBuffer commandBuffer, uint32_t patchControlPoints) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetRasterizerDiscardEnable(VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetRasterizerDiscardEnableEXT(VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetDepthBiasEnable(VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetDepthBiasEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetLogicOpEXT(VkCommandBuffer commandBuffer, VkLogicOp logicOp) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetPrimitiveRestartEnable(VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetPrimitiveRestartEnableEXT(VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetTessellationDomainOriginEXT(VkCommandBuffer commandBuffer, VkTessellationDomainOrigin domainOrigin) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetDepthClampEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthClampEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetPolygonModeEXT(VkCommandBuffer commandBuffer, VkPolygonMode polygonMode) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetRasterizationSamplesEXT(VkCommandBuffer commandBuffer, VkSampleCountFlagBits  rasterizationSamples) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetSampleMaskEXT(VkCommandBuffer commandBuffer, VkSampleCountFlagBits  samples, const VkSampleMask*    pSampleMask) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetAlphaToCoverageEnableEXT(VkCommandBuffer commandBuffer, VkBool32 alphaToCoverageEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetAlphaToOneEnableEXT(VkCommandBuffer commandBuffer, VkBool32 alphaToOneEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetLogicOpEnableEXT(VkCommandBuffer commandBuffer, VkBool32 logicOpEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetColorBlendEnableEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkBool32* pColorBlendEnables) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetColorBlendEquationEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorBlendEquationEXT* pColorBlendEquations) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetColorWriteMaskEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorComponentFlags* pColorWriteMasks) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetRasterizationStreamEXT(VkCommandBuffer commandBuffer, uint32_t rasterizationStream) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetConservativeRasterizationModeEXT(VkCommandBuffer commandBuffer, VkConservativeRasterizationModeEXT conservativeRasterizationMode) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetExtraPrimitiveOverestimationSizeEXT(VkCommandBuffer commandBuffer, float extraPrimitiveOverestimationSize) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetDepthClipEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthClipEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetSampleLocationsEnableEXT(VkCommandBuffer commandBuffer, VkBool32 sampleLocationsEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetColorBlendAdvancedEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorBlendAdvancedEXT* pColorBlendAdvanced) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetProvokingVertexModeEXT(VkCommandBuffer commandBuffer, VkProvokingVertexModeEXT provokingVertexMode) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetLineRasterizationModeEXT(VkCommandBuffer commandBuffer, VkLineRasterizationModeEXT lineRasterizationMode) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetLineStippleEnableEXT(VkCommandBuffer commandBuffer, VkBool32 stippledLineEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetDepthClipNegativeOneToOneEXT(VkCommandBuffer commandBuffer, VkBool32 negativeOneToOne) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetViewportWScalingEnableNV(VkCommandBuffer commandBuffer, VkBool32 viewportWScalingEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetViewportSwizzleNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewportSwizzleNV* pViewportSwizzles) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetCoverageToColorEnableNV(VkCommandBuffer commandBuffer, VkBool32 coverageToColorEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetCoverageToColorLocationNV(VkCommandBuffer commandBuffer, uint32_t coverageToColorLocation) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetCoverageModulationModeNV(VkCommandBuffer commandBuffer, VkCoverageModulationModeNV coverageModulationMode) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetCoverageModulationTableEnableNV(VkCommandBuffer commandBuffer, VkBool32 coverageModulationTableEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetCoverageModulationTableNV(VkCommandBuffer commandBuffer, uint32_t coverageModulationTableCount, const float* pCoverageModulationTable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetShadingRateImageEnableNV(VkCommandBuffer commandBuffer, VkBool32 shadingRateImageEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetCoverageReductionModeNV(VkCommandBuffer commandBuffer, VkCoverageReductionModeNV coverageReductionMode) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetRepresentativeFragmentTestEnableNV(VkCommandBuffer commandBuffer, VkBool32 representativeFragmentTestEnable) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreatePrivateDataSlot(VkDevice device, const VkPrivateDataSlotCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPrivateDataSlot* pPrivateDataSlot) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreatePrivateDataSlotEXT(VkDevice device, const VkPrivateDataSlotCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPrivateDataSlot* pPrivateDataSlot) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_DestroyPrivateDataSlot(VkDevice device, VkPrivateDataSlot privateDataSlot, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_DestroyPrivateDataSlotEXT(VkDevice device, VkPrivateDataSlot privateDataSlot, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_SetPrivateData(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t data) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_SetPrivateDataEXT(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t data) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_GetPrivateData(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t* pData) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_GetPrivateDataEXT(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t* pData) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdCopyBuffer2(VkCommandBuffer commandBuffer, const VkCopyBufferInfo2* pCopyBufferInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdCopyBuffer2KHR(VkCommandBuffer commandBuffer, const VkCopyBufferInfo2* pCopyBufferInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdCopyImage2(VkCommandBuffer commandBuffer, const VkCopyImageInfo2* pCopyImageInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdCopyImage2KHR(VkCommandBuffer commandBuffer, const VkCopyImageInfo2* pCopyImageInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdBlitImage2(VkCommandBuffer commandBuffer, const VkBlitImageInfo2* pBlitImageInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdBlitImage2KHR(VkCommandBuffer commandBuffer, const VkBlitImageInfo2* pBlitImageInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdCopyBufferToImage2(VkCommandBuffer commandBuffer, const VkCopyBufferToImageInfo2* pCopyBufferToImageInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdCopyBufferToImage2KHR(VkCommandBuffer commandBuffer, const VkCopyBufferToImageInfo2* pCopyBufferToImageInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdCopyImageToBuffer2(VkCommandBuffer commandBuffer, const VkCopyImageToBufferInfo2* pCopyImageToBufferInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdCopyImageToBuffer2KHR(VkCommandBuffer commandBuffer, const VkCopyImageToBufferInfo2* pCopyImageToBufferInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdResolveImage2(VkCommandBuffer commandBuffer, const VkResolveImageInfo2* pResolveImageInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdResolveImage2KHR(VkCommandBuffer commandBuffer, const VkResolveImageInfo2* pResolveImageInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetFragmentShadingRateKHR(VkCommandBuffer           commandBuffer, const VkExtent2D*                           pFragmentSize, const VkFragmentShadingRateCombinerOpKHR    combinerOps[2]) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetFragmentShadingRateEnumNV(VkCommandBuffer           commandBuffer, VkFragmentShadingRateNV                     shadingRate, const VkFragmentShadingRateCombinerOpKHR    combinerOps[2]) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_GetAccelerationStructureBuildSizesKHR(VkDevice                                            device, VkAccelerationStructureBuildTypeKHR                 buildType, const VkAccelerationStructureBuildGeometryInfoKHR*  pBuildInfo, const uint32_t*  pMaxPrimitiveCounts, VkAccelerationStructureBuildSizesInfoKHR*           pSizeInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetVertexInputEXT(VkCommandBuffer commandBuffer, uint32_t vertexBindingDescriptionCount, const VkVertexInputBindingDescription2EXT* pVertexBindingDescriptions, uint32_t vertexAttributeDescriptionCount, const VkVertexInputAttributeDescription2EXT* pVertexAttributeDescriptions) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetColorWriteEnableEXT(VkCommandBuffer       commandBuffer, uint32_t                                attachmentCount, const VkBool32*   pColorWriteEnables) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetEvent2(VkCommandBuffer                   commandBuffer, VkEvent                                             event, const VkDependencyInfo*                             pDependencyInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetEvent2KHR(VkCommandBuffer                   commandBuffer, VkEvent                                             event, const VkDependencyInfo*                             pDependencyInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdResetEvent2(VkCommandBuffer                   commandBuffer, VkEvent                                             event, VkPipelineStageFlags2               stageMask) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdResetEvent2KHR(VkCommandBuffer                   commandBuffer, VkEvent                                             event, VkPipelineStageFlags2               stageMask) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdWaitEvents2(VkCommandBuffer                   commandBuffer, uint32_t                                            eventCount, const VkEvent*                     pEvents, const VkDependencyInfo*            pDependencyInfos) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdWaitEvents2KHR(VkCommandBuffer                   commandBuffer, uint32_t                                            eventCount, const VkEvent*                     pEvents, const VkDependencyInfo*            pDependencyInfos) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdPipelineBarrier2(VkCommandBuffer                   commandBuffer, const VkDependencyInfo*                             pDependencyInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdPipelineBarrier2KHR(VkCommandBuffer                   commandBuffer, const VkDependencyInfo*                             pDependencyInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_QueueSubmit2(VkQueue                           queue, uint32_t                            submitCount, const VkSubmitInfo2*              pSubmits, VkFence           fence) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_QueueSubmit2KHR(VkQueue                           queue, uint32_t                            submitCount, const VkSubmitInfo2*              pSubmits, VkFence           fence) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdWriteTimestamp2(VkCommandBuffer                   commandBuffer, VkPipelineStageFlags2               stage, VkQueryPool                                         queryPool, uint32_t                                            query) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdWriteTimestamp2KHR(VkCommandBuffer                   commandBuffer, VkPipelineStageFlags2               stage, VkQueryPool                                         queryPool, uint32_t                                            query) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdWriteBufferMarker2AMD(VkCommandBuffer                   commandBuffer, VkPipelineStageFlags2               stage, VkBuffer                                            dstBuffer, VkDeviceSize                                        dstOffset, uint32_t                                            marker) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_GetQueueCheckpointData2NV(VkQueue queue, uint32_t* pCheckpointDataCount, VkCheckpointData2NV* pCheckpointData) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CopyMemoryToImageEXT(VkDevice device, const VkCopyMemoryToImageInfoEXT* pCopyMemoryToImageInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CopyImageToMemoryEXT(VkDevice device, const VkCopyImageToMemoryInfoEXT* pCopyImageToMemoryInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CopyImageToImageEXT(VkDevice device, const VkCopyImageToImageInfoEXT* pCopyImageToImageInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_TransitionImageLayoutEXT(VkDevice device, uint32_t transitionCount, const VkHostImageLayoutTransitionInfoEXT* pTransitions) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateVideoSessionKHR(VkDevice device, const VkVideoSessionCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkVideoSessionKHR* pVideoSession) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_DestroyVideoSessionKHR(VkDevice device, VkVideoSessionKHR videoSession, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateVideoSessionParametersKHR(VkDevice device, const VkVideoSessionParametersCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkVideoSessionParametersKHR* pVideoSessionParameters) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_UpdateVideoSessionParametersKHR(VkDevice device, VkVideoSessionParametersKHR videoSessionParameters, const VkVideoSessionParametersUpdateInfoKHR* pUpdateInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_DestroyVideoSessionParametersKHR(VkDevice device, VkVideoSessionParametersKHR videoSessionParameters, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetVideoSessionMemoryRequirementsKHR(VkDevice device, VkVideoSessionKHR videoSession, uint32_t* pMemoryRequirementsCount, VkVideoSessionMemoryRequirementsKHR* pMemoryRequirements) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_BindVideoSessionMemoryKHR(VkDevice device, VkVideoSessionKHR videoSession, uint32_t bindSessionMemoryInfoCount, const VkBindVideoSessionMemoryInfoKHR* pBindSessionMemoryInfos) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdDecodeVideoKHR(VkCommandBuffer commandBuffer, const VkVideoDecodeInfoKHR* pDecodeInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdBeginVideoCodingKHR(VkCommandBuffer commandBuffer, const VkVideoBeginCodingInfoKHR* pBeginInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdControlVideoCodingKHR(VkCommandBuffer commandBuffer, const VkVideoCodingControlInfoKHR* pCodingControlInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdEndVideoCodingKHR(VkCommandBuffer commandBuffer, const VkVideoEndCodingInfoKHR* pEndCodingInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdDecompressMemoryNV(VkCommandBuffer commandBuffer, uint32_t decompressRegionCount, const VkDecompressMemoryRegionNV* pDecompressMemoryRegions) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdDecompressMemoryIndirectCountNV(VkCommandBuffer commandBuffer, VkDeviceAddress indirectCommandsAddress, VkDeviceAddress indirectCommandsCountAddress, uint32_t stride) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateCuModuleNVX(VkDevice device, const VkCuModuleCreateInfoNVX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCuModuleNVX* pModule) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateCuFunctionNVX(VkDevice device, const VkCuFunctionCreateInfoNVX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCuFunctionNVX* pFunction) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_DestroyCuModuleNVX(VkDevice device, VkCuModuleNVX module, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_DestroyCuFunctionNVX(VkDevice device, VkCuFunctionNVX function, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdCuLaunchKernelNVX(VkCommandBuffer commandBuffer, const VkCuLaunchInfoNVX* pLaunchInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_GetDescriptorSetLayoutSizeEXT(VkDevice device, VkDescriptorSetLayout layout, VkDeviceSize* pLayoutSizeInBytes) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_GetDescriptorSetLayoutBindingOffsetEXT(VkDevice device, VkDescriptorSetLayout layout, uint32_t binding, VkDeviceSize* pOffset) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_GetDescriptorEXT(VkDevice device, const VkDescriptorGetInfoEXT* pDescriptorInfo, size_t dataSize, void* pDescriptor) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdBindDescriptorBuffersEXT(VkCommandBuffer commandBuffer, uint32_t bufferCount, const VkDescriptorBufferBindingInfoEXT* pBindingInfos) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetDescriptorBufferOffsetsEXT(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t setCount, const uint32_t* pBufferIndices, const VkDeviceSize* pOffsets) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdBindDescriptorBufferEmbeddedSamplersEXT(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetBufferOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkBufferCaptureDescriptorDataInfoEXT* pInfo, void* pData) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetImageOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkImageCaptureDescriptorDataInfoEXT* pInfo, void* pData) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetImageViewOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkImageViewCaptureDescriptorDataInfoEXT* pInfo, void* pData) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetSamplerOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkSamplerCaptureDescriptorDataInfoEXT* pInfo, void* pData) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetAccelerationStructureOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkAccelerationStructureCaptureDescriptorDataInfoEXT* pInfo, void* pData) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_SetDeviceMemoryPriorityEXT(VkDevice       device, VkDeviceMemory memory, float          priority) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_WaitForPresentKHR(VkDevice device, VkSwapchainKHR swapchain, uint64_t presentId, uint64_t timeout) ATTR_WEAK;


#ifdef VK_USE_PLATFORM_FUCHSIA
  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateBufferCollectionFUCHSIA(VkDevice device, const VkBufferCollectionCreateInfoFUCHSIA* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferCollectionFUCHSIA* pCollection) ATTR_WEAK;


#endif // VK_USE_PLATFORM_FUCHSIA
#ifdef VK_USE_PLATFORM_FUCHSIA
  VKAPI_ATTR VkResult VKAPI_CALL wsi_SetBufferCollectionBufferConstraintsFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, const VkBufferConstraintsInfoFUCHSIA* pBufferConstraintsInfo) ATTR_WEAK;


#endif // VK_USE_PLATFORM_FUCHSIA
#ifdef VK_USE_PLATFORM_FUCHSIA
  VKAPI_ATTR VkResult VKAPI_CALL wsi_SetBufferCollectionImageConstraintsFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, const VkImageConstraintsInfoFUCHSIA* pImageConstraintsInfo) ATTR_WEAK;


#endif // VK_USE_PLATFORM_FUCHSIA
#ifdef VK_USE_PLATFORM_FUCHSIA
  VKAPI_ATTR void VKAPI_CALL wsi_DestroyBufferCollectionFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


#endif // VK_USE_PLATFORM_FUCHSIA
#ifdef VK_USE_PLATFORM_FUCHSIA
  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetBufferCollectionPropertiesFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, VkBufferCollectionPropertiesFUCHSIA* pProperties) ATTR_WEAK;


#endif // VK_USE_PLATFORM_FUCHSIA
  VKAPI_ATTR void VKAPI_CALL wsi_CmdBeginRendering(VkCommandBuffer                   commandBuffer, const VkRenderingInfo*                              pRenderingInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdBeginRenderingKHR(VkCommandBuffer                   commandBuffer, const VkRenderingInfo*                              pRenderingInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdEndRendering(VkCommandBuffer                   commandBuffer) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdEndRenderingKHR(VkCommandBuffer                   commandBuffer) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_GetDescriptorSetLayoutHostMappingInfoVALVE(VkDevice device, const VkDescriptorSetBindingReferenceVALVE* pBindingReference, VkDescriptorSetLayoutHostMappingInfoVALVE* pHostMapping) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_GetDescriptorSetHostMappingVALVE(VkDevice device, VkDescriptorSet descriptorSet, void** ppData) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateMicromapEXT(VkDevice                                           device, const VkMicromapCreateInfoEXT*        pCreateInfo, const VkAllocationCallbacks*       pAllocator, VkMicromapEXT*                        pMicromap) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdBuildMicromapsEXT(VkCommandBuffer                                    commandBuffer, uint32_t infoCount, const VkMicromapBuildInfoEXT* pInfos) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_BuildMicromapsEXT(VkDevice                                           device, VkDeferredOperationKHR deferredOperation, uint32_t infoCount, const VkMicromapBuildInfoEXT* pInfos) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_DestroyMicromapEXT(VkDevice device, VkMicromapEXT micromap, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdCopyMicromapEXT(VkCommandBuffer commandBuffer, const VkCopyMicromapInfoEXT* pInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CopyMicromapEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMicromapInfoEXT* pInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdCopyMicromapToMemoryEXT(VkCommandBuffer commandBuffer, const VkCopyMicromapToMemoryInfoEXT* pInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CopyMicromapToMemoryEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMicromapToMemoryInfoEXT* pInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdCopyMemoryToMicromapEXT(VkCommandBuffer commandBuffer, const VkCopyMemoryToMicromapInfoEXT* pInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CopyMemoryToMicromapEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMemoryToMicromapInfoEXT* pInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdWriteMicromapsPropertiesEXT(VkCommandBuffer commandBuffer, uint32_t micromapCount, const VkMicromapEXT* pMicromaps, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_WriteMicromapsPropertiesEXT(VkDevice device, uint32_t micromapCount, const VkMicromapEXT* pMicromaps, VkQueryType  queryType, size_t       dataSize, void* pData, size_t stride) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_GetDeviceMicromapCompatibilityEXT(VkDevice device, const VkMicromapVersionInfoEXT* pVersionInfo, VkAccelerationStructureCompatibilityKHR* pCompatibility) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_GetMicromapBuildSizesEXT(VkDevice                                            device, VkAccelerationStructureBuildTypeKHR                 buildType, const VkMicromapBuildInfoEXT*  pBuildInfo, VkMicromapBuildSizesInfoEXT*           pSizeInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_GetShaderModuleIdentifierEXT(VkDevice device, VkShaderModule shaderModule, VkShaderModuleIdentifierEXT* pIdentifier) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_GetShaderModuleCreateInfoIdentifierEXT(VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, VkShaderModuleIdentifierEXT* pIdentifier) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_GetImageSubresourceLayout2KHR(VkDevice device, VkImage image, const VkImageSubresource2KHR* pSubresource, VkSubresourceLayout2KHR* pLayout) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_GetImageSubresourceLayout2EXT(VkDevice device, VkImage image, const VkImageSubresource2KHR* pSubresource, VkSubresourceLayout2KHR* pLayout) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetPipelinePropertiesEXT(VkDevice device, const VkPipelineInfoEXT* pPipelineInfo, VkBaseOutStructure* pPipelineProperties) ATTR_WEAK;


#ifdef VK_USE_PLATFORM_METAL_EXT
  VKAPI_ATTR void VKAPI_CALL wsi_ExportMetalObjectsEXT(VkDevice device, VkExportMetalObjectsInfoEXT* pMetalObjectsInfo) ATTR_WEAK;


#endif // VK_USE_PLATFORM_METAL_EXT
  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetFramebufferTilePropertiesQCOM(VkDevice device, VkFramebuffer framebuffer, uint32_t* pPropertiesCount, VkTilePropertiesQCOM* pProperties) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetDynamicRenderingTilePropertiesQCOM(VkDevice device, const VkRenderingInfo* pRenderingInfo, VkTilePropertiesQCOM* pProperties) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateOpticalFlowSessionNV(VkDevice device, const VkOpticalFlowSessionCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkOpticalFlowSessionNV* pSession) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_DestroyOpticalFlowSessionNV(VkDevice device, VkOpticalFlowSessionNV session, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_BindOpticalFlowSessionImageNV(VkDevice device, VkOpticalFlowSessionNV session, VkOpticalFlowSessionBindingPointNV bindingPoint, VkImageView view, VkImageLayout layout) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdOpticalFlowExecuteNV(VkCommandBuffer commandBuffer, VkOpticalFlowSessionNV session, const VkOpticalFlowExecuteInfoNV* pExecuteInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetDeviceFaultInfoEXT(VkDevice device, VkDeviceFaultCountsEXT* pFaultCounts, VkDeviceFaultInfoEXT* pFaultInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdSetDepthBias2EXT(VkCommandBuffer commandBuffer, const VkDepthBiasInfoEXT*         pDepthBiasInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_ReleaseSwapchainImagesEXT(VkDevice device, const VkReleaseSwapchainImagesInfoEXT* pReleaseInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_GetDeviceImageSubresourceLayoutKHR(VkDevice device, const VkDeviceImageSubresourceInfoKHR* pInfo, VkSubresourceLayout2KHR* pLayout) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_MapMemory2KHR(VkDevice device, const VkMemoryMapInfoKHR* pMemoryMapInfo, void** ppData) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_UnmapMemory2KHR(VkDevice device, const VkMemoryUnmapInfoKHR* pMemoryUnmapInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_CreateShadersEXT(VkDevice device, uint32_t createInfoCount, const VkShaderCreateInfoEXT* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkShaderEXT* pShaders) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_DestroyShaderEXT(VkDevice device, VkShaderEXT shader, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetShaderBinaryDataEXT(VkDevice device, VkShaderEXT shader, size_t* pDataSize, void* pData) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_CmdBindShadersEXT(VkCommandBuffer commandBuffer, uint32_t stageCount, const VkShaderStageFlagBits* pStages, const VkShaderEXT* pShaders) ATTR_WEAK;


#ifdef VK_USE_PLATFORM_SCREEN_QNX
  VKAPI_ATTR VkResult VKAPI_CALL wsi_GetScreenBufferPropertiesQNX(VkDevice device, const struct _screen_buffer* buffer, VkScreenBufferPropertiesQNX* pProperties) ATTR_WEAK;


#endif // VK_USE_PLATFORM_SCREEN_QNX
  VKAPI_ATTR VkResult VKAPI_CALL wsi_SetLatencySleepModeNV(VkDevice device, VkSwapchainKHR swapchain, const VkLatencySleepModeInfoNV* pSleepModeInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL wsi_LatencySleepNV(VkDevice device, VkSwapchainKHR swapchain, const VkLatencySleepInfoNV* pSleepInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_SetLatencyMarkerNV(VkDevice device, VkSwapchainKHR swapchain, const VkSetLatencyMarkerInfoNV* pLatencyMarkerInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_GetLatencyTimingsNV(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pTimingCount, VkGetLatencyMarkerInfoNV* pLatencyMarkerInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL wsi_QueueNotifyOutOfBandNV(VkQueue queue, const VkOutOfBandQueueTypeInfoNV* pQueueTypeInfo) ATTR_WEAK;



#ifdef __cplusplus
}
#endif

#endif /* WSI_COMMON_ENTRYPOINTS_H */
