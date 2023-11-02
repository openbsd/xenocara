
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


#ifndef RADV_ENTRYPOINTS_H
#define RADV_ENTRYPOINTS_H

#ifdef __cplusplus
extern "C" {
#endif

/* clang wants function declarations in the header to have weak attribute */
#if !defined(_MSC_VER) && !defined(__MINGW32__)
#define ATTR_WEAK __attribute__ ((weak))
#else
#define ATTR_WEAK
#endif

extern const struct vk_instance_entrypoint_table radv_instance_entrypoints;

extern const struct vk_physical_device_entrypoint_table radv_physical_device_entrypoints;

extern const struct vk_device_entrypoint_table radv_device_entrypoints;
extern const struct vk_device_entrypoint_table sqtt_device_entrypoints;
extern const struct vk_device_entrypoint_table rra_device_entrypoints;
extern const struct vk_device_entrypoint_table rmv_device_entrypoints;
extern const struct vk_device_entrypoint_table metro_exodus_device_entrypoints;
extern const struct vk_device_entrypoint_table rage2_device_entrypoints;


  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateInstance(const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance);
  VKAPI_ATTR void VKAPI_CALL radv_DestroyInstance(VkInstance instance, const VkAllocationCallbacks* pAllocator);
  VKAPI_ATTR VkResult VKAPI_CALL radv_EnumeratePhysicalDevices(VkInstance instance, uint32_t* pPhysicalDeviceCount, VkPhysicalDevice* pPhysicalDevices);
  VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL radv_GetInstanceProcAddr(VkInstance instance, const char* pName);
  VKAPI_ATTR VkResult VKAPI_CALL radv_EnumerateInstanceVersion(uint32_t* pApiVersion);
  VKAPI_ATTR VkResult VKAPI_CALL radv_EnumerateInstanceLayerProperties(uint32_t* pPropertyCount, VkLayerProperties* pProperties);
  VKAPI_ATTR VkResult VKAPI_CALL radv_EnumerateInstanceExtensionProperties(const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties);
#ifdef VK_USE_PLATFORM_ANDROID_KHR
  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateAndroidSurfaceKHR(VkInstance instance, const VkAndroidSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_ANDROID_KHR
  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateDisplayPlaneSurfaceKHR(VkInstance instance, const VkDisplaySurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
  VKAPI_ATTR void VKAPI_CALL radv_DestroySurfaceKHR(VkInstance instance, VkSurfaceKHR surface, const VkAllocationCallbacks* pAllocator);
#ifdef VK_USE_PLATFORM_VI_NN
  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateViSurfaceNN(VkInstance instance, const VkViSurfaceCreateInfoNN* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_VI_NN
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateWaylandSurfaceKHR(VkInstance instance, const VkWaylandSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_WAYLAND_KHR
#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateWin32SurfaceKHR(VkInstance instance, const VkWin32SurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_WIN32_KHR
#ifdef VK_USE_PLATFORM_XLIB_KHR
  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateXlibSurfaceKHR(VkInstance instance, const VkXlibSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_XLIB_KHR
#ifdef VK_USE_PLATFORM_XCB_KHR
  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateXcbSurfaceKHR(VkInstance instance, const VkXcbSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_XCB_KHR
#ifdef VK_USE_PLATFORM_DIRECTFB_EXT
  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateDirectFBSurfaceEXT(VkInstance instance, const VkDirectFBSurfaceCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_DIRECTFB_EXT
#ifdef VK_USE_PLATFORM_FUCHSIA
  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateImagePipeSurfaceFUCHSIA(VkInstance instance, const VkImagePipeSurfaceCreateInfoFUCHSIA* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_FUCHSIA
#ifdef VK_USE_PLATFORM_GGP
  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateStreamDescriptorSurfaceGGP(VkInstance instance, const VkStreamDescriptorSurfaceCreateInfoGGP* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_GGP
#ifdef VK_USE_PLATFORM_SCREEN_QNX
  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateScreenSurfaceQNX(VkInstance instance, const VkScreenSurfaceCreateInfoQNX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_SCREEN_QNX
  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);
  VKAPI_ATTR void VKAPI_CALL radv_DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator);
  VKAPI_ATTR void VKAPI_CALL radv_DebugReportMessageEXT(VkInstance instance, VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage);
  VKAPI_ATTR VkResult VKAPI_CALL radv_EnumeratePhysicalDeviceGroups(VkInstance instance, uint32_t* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties);
  VKAPI_ATTR VkResult VKAPI_CALL radv_EnumeratePhysicalDeviceGroupsKHR(VkInstance instance, uint32_t* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties);
#ifdef VK_USE_PLATFORM_IOS_MVK
  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateIOSSurfaceMVK(VkInstance instance, const VkIOSSurfaceCreateInfoMVK* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_IOS_MVK
#ifdef VK_USE_PLATFORM_MACOS_MVK
  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateMacOSSurfaceMVK(VkInstance instance, const VkMacOSSurfaceCreateInfoMVK* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_MACOS_MVK
#ifdef VK_USE_PLATFORM_METAL_EXT
  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateMetalSurfaceEXT(VkInstance instance, const VkMetalSurfaceCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_METAL_EXT
  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pMessenger);
  VKAPI_ATTR void VKAPI_CALL radv_DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT messenger, const VkAllocationCallbacks* pAllocator);
  VKAPI_ATTR void VKAPI_CALL radv_SubmitDebugUtilsMessageEXT(VkInstance instance, VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData);
  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateHeadlessSurfaceEXT(VkInstance instance, const VkHeadlessSurfaceCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);

  VKAPI_ATTR void VKAPI_CALL radv_GetPhysicalDeviceProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties* pProperties);
  VKAPI_ATTR void VKAPI_CALL radv_GetPhysicalDeviceQueueFamilyProperties(VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties* pQueueFamilyProperties);
  VKAPI_ATTR void VKAPI_CALL radv_GetPhysicalDeviceMemoryProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties* pMemoryProperties);
  VKAPI_ATTR void VKAPI_CALL radv_GetPhysicalDeviceFeatures(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures* pFeatures);
  VKAPI_ATTR void VKAPI_CALL radv_GetPhysicalDeviceFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties* pFormatProperties);
  VKAPI_ATTR VkResult VKAPI_CALL radv_GetPhysicalDeviceImageFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkImageFormatProperties* pImageFormatProperties);
  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateDevice(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDevice* pDevice);
  VKAPI_ATTR VkResult VKAPI_CALL radv_EnumerateDeviceLayerProperties(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkLayerProperties* pProperties);
  VKAPI_ATTR VkResult VKAPI_CALL radv_EnumerateDeviceExtensionProperties(VkPhysicalDevice physicalDevice, const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties);
  VKAPI_ATTR void VKAPI_CALL radv_GetPhysicalDeviceSparseImageFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkSampleCountFlagBits samples, VkImageUsageFlags usage, VkImageTiling tiling, uint32_t* pPropertyCount, VkSparseImageFormatProperties* pProperties);
  VKAPI_ATTR VkResult VKAPI_CALL radv_GetPhysicalDeviceDisplayPropertiesKHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPropertiesKHR* pProperties);
  VKAPI_ATTR VkResult VKAPI_CALL radv_GetPhysicalDeviceDisplayPlanePropertiesKHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPlanePropertiesKHR* pProperties);
  VKAPI_ATTR VkResult VKAPI_CALL radv_GetDisplayPlaneSupportedDisplaysKHR(VkPhysicalDevice physicalDevice, uint32_t planeIndex, uint32_t* pDisplayCount, VkDisplayKHR* pDisplays);
  VKAPI_ATTR VkResult VKAPI_CALL radv_GetDisplayModePropertiesKHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModePropertiesKHR* pProperties);
  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateDisplayModeKHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, const VkDisplayModeCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDisplayModeKHR* pMode);
  VKAPI_ATTR VkResult VKAPI_CALL radv_GetDisplayPlaneCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkDisplayModeKHR mode, uint32_t planeIndex, VkDisplayPlaneCapabilitiesKHR* pCapabilities);
  VKAPI_ATTR VkResult VKAPI_CALL radv_GetPhysicalDeviceSurfaceSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported);
  VKAPI_ATTR VkResult VKAPI_CALL radv_GetPhysicalDeviceSurfaceCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR* pSurfaceCapabilities);
  VKAPI_ATTR VkResult VKAPI_CALL radv_GetPhysicalDeviceSurfaceFormatsKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pSurfaceFormatCount, VkSurfaceFormatKHR* pSurfaceFormats);
  VKAPI_ATTR VkResult VKAPI_CALL radv_GetPhysicalDeviceSurfacePresentModesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes);
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
  VKAPI_ATTR VkBool32 VKAPI_CALL radv_GetPhysicalDeviceWaylandPresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, struct wl_display* display);
#endif // VK_USE_PLATFORM_WAYLAND_KHR
#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkBool32 VKAPI_CALL radv_GetPhysicalDeviceWin32PresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex);
#endif // VK_USE_PLATFORM_WIN32_KHR
#ifdef VK_USE_PLATFORM_XLIB_KHR
  VKAPI_ATTR VkBool32 VKAPI_CALL radv_GetPhysicalDeviceXlibPresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, Display* dpy, VisualID visualID);
#endif // VK_USE_PLATFORM_XLIB_KHR
#ifdef VK_USE_PLATFORM_XCB_KHR
  VKAPI_ATTR VkBool32 VKAPI_CALL radv_GetPhysicalDeviceXcbPresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, xcb_connection_t* connection, xcb_visualid_t visual_id);
#endif // VK_USE_PLATFORM_XCB_KHR
#ifdef VK_USE_PLATFORM_DIRECTFB_EXT
  VKAPI_ATTR VkBool32 VKAPI_CALL radv_GetPhysicalDeviceDirectFBPresentationSupportEXT(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, IDirectFB* dfb);
#endif // VK_USE_PLATFORM_DIRECTFB_EXT
#ifdef VK_USE_PLATFORM_SCREEN_QNX
  VKAPI_ATTR VkBool32 VKAPI_CALL radv_GetPhysicalDeviceScreenPresentationSupportQNX(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, struct _screen_window* window);
#endif // VK_USE_PLATFORM_SCREEN_QNX
  VKAPI_ATTR VkResult VKAPI_CALL radv_GetPhysicalDeviceExternalImageFormatPropertiesNV(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkExternalMemoryHandleTypeFlagsNV externalHandleType, VkExternalImageFormatPropertiesNV* pExternalImageFormatProperties);
  VKAPI_ATTR void VKAPI_CALL radv_GetPhysicalDeviceFeatures2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures);
  VKAPI_ATTR void VKAPI_CALL radv_GetPhysicalDeviceFeatures2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures);
  VKAPI_ATTR void VKAPI_CALL radv_GetPhysicalDeviceProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties);
  VKAPI_ATTR void VKAPI_CALL radv_GetPhysicalDeviceProperties2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties);
  VKAPI_ATTR void VKAPI_CALL radv_GetPhysicalDeviceFormatProperties2(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2* pFormatProperties);
  VKAPI_ATTR void VKAPI_CALL radv_GetPhysicalDeviceFormatProperties2KHR(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2* pFormatProperties);
  VKAPI_ATTR VkResult VKAPI_CALL radv_GetPhysicalDeviceImageFormatProperties2(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties);
  VKAPI_ATTR VkResult VKAPI_CALL radv_GetPhysicalDeviceImageFormatProperties2KHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties);
  VKAPI_ATTR void VKAPI_CALL radv_GetPhysicalDeviceQueueFamilyProperties2(VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties2* pQueueFamilyProperties);
  VKAPI_ATTR void VKAPI_CALL radv_GetPhysicalDeviceQueueFamilyProperties2KHR(VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties2* pQueueFamilyProperties);
  VKAPI_ATTR void VKAPI_CALL radv_GetPhysicalDeviceMemoryProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties);
  VKAPI_ATTR void VKAPI_CALL radv_GetPhysicalDeviceMemoryProperties2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties);
  VKAPI_ATTR void VKAPI_CALL radv_GetPhysicalDeviceSparseImageFormatProperties2(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo, uint32_t* pPropertyCount, VkSparseImageFormatProperties2* pProperties);
  VKAPI_ATTR void VKAPI_CALL radv_GetPhysicalDeviceSparseImageFormatProperties2KHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo, uint32_t* pPropertyCount, VkSparseImageFormatProperties2* pProperties);
  VKAPI_ATTR void VKAPI_CALL radv_GetPhysicalDeviceExternalBufferProperties(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties);
  VKAPI_ATTR void VKAPI_CALL radv_GetPhysicalDeviceExternalBufferPropertiesKHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties);
  VKAPI_ATTR void VKAPI_CALL radv_GetPhysicalDeviceExternalSemaphoreProperties(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties);
  VKAPI_ATTR void VKAPI_CALL radv_GetPhysicalDeviceExternalSemaphorePropertiesKHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties);
  VKAPI_ATTR void VKAPI_CALL radv_GetPhysicalDeviceExternalFenceProperties(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties);
  VKAPI_ATTR void VKAPI_CALL radv_GetPhysicalDeviceExternalFencePropertiesKHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties);
  VKAPI_ATTR VkResult VKAPI_CALL radv_ReleaseDisplayEXT(VkPhysicalDevice physicalDevice, VkDisplayKHR display);
#ifdef VK_USE_PLATFORM_XLIB_XRANDR_EXT
  VKAPI_ATTR VkResult VKAPI_CALL radv_AcquireXlibDisplayEXT(VkPhysicalDevice physicalDevice, Display* dpy, VkDisplayKHR display);
#endif // VK_USE_PLATFORM_XLIB_XRANDR_EXT
#ifdef VK_USE_PLATFORM_XLIB_XRANDR_EXT
  VKAPI_ATTR VkResult VKAPI_CALL radv_GetRandROutputDisplayEXT(VkPhysicalDevice physicalDevice, Display* dpy, RROutput rrOutput, VkDisplayKHR* pDisplay);
#endif // VK_USE_PLATFORM_XLIB_XRANDR_EXT
#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL radv_AcquireWinrtDisplayNV(VkPhysicalDevice physicalDevice, VkDisplayKHR display);
#endif // VK_USE_PLATFORM_WIN32_KHR
#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL radv_GetWinrtDisplayNV(VkPhysicalDevice physicalDevice, uint32_t deviceRelativeId, VkDisplayKHR* pDisplay);
#endif // VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL radv_GetPhysicalDeviceSurfaceCapabilities2EXT(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilities2EXT* pSurfaceCapabilities);
  VKAPI_ATTR VkResult VKAPI_CALL radv_GetPhysicalDevicePresentRectanglesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pRectCount, VkRect2D* pRects);
  VKAPI_ATTR void VKAPI_CALL radv_GetPhysicalDeviceMultisamplePropertiesEXT(VkPhysicalDevice physicalDevice, VkSampleCountFlagBits samples, VkMultisamplePropertiesEXT* pMultisampleProperties);
  VKAPI_ATTR VkResult VKAPI_CALL radv_GetPhysicalDeviceSurfaceCapabilities2KHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkSurfaceCapabilities2KHR* pSurfaceCapabilities);
  VKAPI_ATTR VkResult VKAPI_CALL radv_GetPhysicalDeviceSurfaceFormats2KHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pSurfaceFormatCount, VkSurfaceFormat2KHR* pSurfaceFormats);
  VKAPI_ATTR VkResult VKAPI_CALL radv_GetPhysicalDeviceDisplayProperties2KHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayProperties2KHR* pProperties);
  VKAPI_ATTR VkResult VKAPI_CALL radv_GetPhysicalDeviceDisplayPlaneProperties2KHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPlaneProperties2KHR* pProperties);
  VKAPI_ATTR VkResult VKAPI_CALL radv_GetDisplayModeProperties2KHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModeProperties2KHR* pProperties);
  VKAPI_ATTR VkResult VKAPI_CALL radv_GetDisplayPlaneCapabilities2KHR(VkPhysicalDevice physicalDevice, const VkDisplayPlaneInfo2KHR* pDisplayPlaneInfo, VkDisplayPlaneCapabilities2KHR* pCapabilities);
  VKAPI_ATTR VkResult VKAPI_CALL radv_GetPhysicalDeviceCalibrateableTimeDomainsEXT(VkPhysicalDevice physicalDevice, uint32_t* pTimeDomainCount, VkTimeDomainEXT* pTimeDomains);
  VKAPI_ATTR VkResult VKAPI_CALL radv_GetPhysicalDeviceCooperativeMatrixPropertiesNV(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkCooperativeMatrixPropertiesNV* pProperties);
#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL radv_GetPhysicalDeviceSurfacePresentModes2EXT(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes);
#endif // VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL radv_EnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, uint32_t* pCounterCount, VkPerformanceCounterKHR* pCounters, VkPerformanceCounterDescriptionKHR* pCounterDescriptions);
  VKAPI_ATTR void VKAPI_CALL radv_GetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR(VkPhysicalDevice physicalDevice, const VkQueryPoolPerformanceCreateInfoKHR* pPerformanceQueryCreateInfo, uint32_t* pNumPasses);
  VKAPI_ATTR VkResult VKAPI_CALL radv_GetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV(VkPhysicalDevice physicalDevice, uint32_t* pCombinationCount, VkFramebufferMixedSamplesCombinationNV* pCombinations);
  VKAPI_ATTR VkResult VKAPI_CALL radv_GetPhysicalDeviceToolProperties(VkPhysicalDevice physicalDevice, uint32_t* pToolCount, VkPhysicalDeviceToolProperties* pToolProperties);
  VKAPI_ATTR VkResult VKAPI_CALL radv_GetPhysicalDeviceToolPropertiesEXT(VkPhysicalDevice physicalDevice, uint32_t* pToolCount, VkPhysicalDeviceToolProperties* pToolProperties);
  VKAPI_ATTR VkResult VKAPI_CALL radv_GetPhysicalDeviceFragmentShadingRatesKHR(VkPhysicalDevice physicalDevice, uint32_t* pFragmentShadingRateCount, VkPhysicalDeviceFragmentShadingRateKHR* pFragmentShadingRates);
  VKAPI_ATTR VkResult VKAPI_CALL radv_GetPhysicalDeviceVideoCapabilitiesKHR(VkPhysicalDevice physicalDevice, const VkVideoProfileInfoKHR* pVideoProfile, VkVideoCapabilitiesKHR* pCapabilities);
  VKAPI_ATTR VkResult VKAPI_CALL radv_GetPhysicalDeviceVideoFormatPropertiesKHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceVideoFormatInfoKHR* pVideoFormatInfo, uint32_t* pVideoFormatPropertyCount, VkVideoFormatPropertiesKHR* pVideoFormatProperties);
  VKAPI_ATTR VkResult VKAPI_CALL radv_AcquireDrmDisplayEXT(VkPhysicalDevice physicalDevice, int32_t drmFd, VkDisplayKHR display);
  VKAPI_ATTR VkResult VKAPI_CALL radv_GetDrmDisplayEXT(VkPhysicalDevice physicalDevice, int32_t drmFd, uint32_t connectorId, VkDisplayKHR* display);
  VKAPI_ATTR VkResult VKAPI_CALL radv_GetPhysicalDeviceOpticalFlowImageFormatsNV(VkPhysicalDevice physicalDevice, const VkOpticalFlowImageFormatInfoNV* pOpticalFlowImageFormatInfo, uint32_t* pFormatCount, VkOpticalFlowImageFormatPropertiesNV* pImageFormatProperties);

  VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL radv_GetDeviceProcAddr(VkDevice device, const char* pName) ATTR_WEAK;
  VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL sqtt_GetDeviceProcAddr(VkDevice device, const char* pName) ATTR_WEAK;
  VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL rra_GetDeviceProcAddr(VkDevice device, const char* pName) ATTR_WEAK;
  VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL rmv_GetDeviceProcAddr(VkDevice device, const char* pName) ATTR_WEAK;
  VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL metro_exodus_GetDeviceProcAddr(VkDevice device, const char* pName) ATTR_WEAK;
  VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL rage2_GetDeviceProcAddr(VkDevice device, const char* pName) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_DestroyDevice(VkDevice device, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_DestroyDevice(VkDevice device, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_DestroyDevice(VkDevice device, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_DestroyDevice(VkDevice device, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_DestroyDevice(VkDevice device, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_DestroyDevice(VkDevice device, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_GetDeviceQueue(VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_GetDeviceQueue(VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_GetDeviceQueue(VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_GetDeviceQueue(VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_GetDeviceQueue(VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_GetDeviceQueue(VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_QueueSubmit(VkQueue queue, uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_QueueSubmit(VkQueue queue, uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_QueueSubmit(VkQueue queue, uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_QueueSubmit(VkQueue queue, uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_QueueSubmit(VkQueue queue, uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_QueueSubmit(VkQueue queue, uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_QueueWaitIdle(VkQueue queue) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_QueueWaitIdle(VkQueue queue) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_QueueWaitIdle(VkQueue queue) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_QueueWaitIdle(VkQueue queue) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_QueueWaitIdle(VkQueue queue) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_QueueWaitIdle(VkQueue queue) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_DeviceWaitIdle(VkDevice device) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_DeviceWaitIdle(VkDevice device) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_DeviceWaitIdle(VkDevice device) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_DeviceWaitIdle(VkDevice device) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_DeviceWaitIdle(VkDevice device) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_DeviceWaitIdle(VkDevice device) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_AllocateMemory(VkDevice device, const VkMemoryAllocateInfo* pAllocateInfo, const VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_AllocateMemory(VkDevice device, const VkMemoryAllocateInfo* pAllocateInfo, const VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_AllocateMemory(VkDevice device, const VkMemoryAllocateInfo* pAllocateInfo, const VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_AllocateMemory(VkDevice device, const VkMemoryAllocateInfo* pAllocateInfo, const VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_AllocateMemory(VkDevice device, const VkMemoryAllocateInfo* pAllocateInfo, const VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_AllocateMemory(VkDevice device, const VkMemoryAllocateInfo* pAllocateInfo, const VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_FreeMemory(VkDevice device, VkDeviceMemory memory, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_FreeMemory(VkDevice device, VkDeviceMemory memory, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_FreeMemory(VkDevice device, VkDeviceMemory memory, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_FreeMemory(VkDevice device, VkDeviceMemory memory, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_FreeMemory(VkDevice device, VkDeviceMemory memory, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_FreeMemory(VkDevice device, VkDeviceMemory memory, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_MapMemory(VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_MapMemory(VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_MapMemory(VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_MapMemory(VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_MapMemory(VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_MapMemory(VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_UnmapMemory(VkDevice device, VkDeviceMemory memory) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_UnmapMemory(VkDevice device, VkDeviceMemory memory) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_UnmapMemory(VkDevice device, VkDeviceMemory memory) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_UnmapMemory(VkDevice device, VkDeviceMemory memory) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_UnmapMemory(VkDevice device, VkDeviceMemory memory) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_UnmapMemory(VkDevice device, VkDeviceMemory memory) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_FlushMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_FlushMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_FlushMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_FlushMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_FlushMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_FlushMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_InvalidateMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_InvalidateMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_InvalidateMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_InvalidateMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_InvalidateMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_InvalidateMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_GetDeviceMemoryCommitment(VkDevice device, VkDeviceMemory memory, VkDeviceSize* pCommittedMemoryInBytes) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_GetDeviceMemoryCommitment(VkDevice device, VkDeviceMemory memory, VkDeviceSize* pCommittedMemoryInBytes) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_GetDeviceMemoryCommitment(VkDevice device, VkDeviceMemory memory, VkDeviceSize* pCommittedMemoryInBytes) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_GetDeviceMemoryCommitment(VkDevice device, VkDeviceMemory memory, VkDeviceSize* pCommittedMemoryInBytes) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_GetDeviceMemoryCommitment(VkDevice device, VkDeviceMemory memory, VkDeviceSize* pCommittedMemoryInBytes) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_GetDeviceMemoryCommitment(VkDevice device, VkDeviceMemory memory, VkDeviceSize* pCommittedMemoryInBytes) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_GetBufferMemoryRequirements(VkDevice device, VkBuffer buffer, VkMemoryRequirements* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_GetBufferMemoryRequirements(VkDevice device, VkBuffer buffer, VkMemoryRequirements* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_GetBufferMemoryRequirements(VkDevice device, VkBuffer buffer, VkMemoryRequirements* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_GetBufferMemoryRequirements(VkDevice device, VkBuffer buffer, VkMemoryRequirements* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_GetBufferMemoryRequirements(VkDevice device, VkBuffer buffer, VkMemoryRequirements* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_GetBufferMemoryRequirements(VkDevice device, VkBuffer buffer, VkMemoryRequirements* pMemoryRequirements) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_BindBufferMemory(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_BindBufferMemory(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_BindBufferMemory(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_BindBufferMemory(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_BindBufferMemory(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_BindBufferMemory(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_GetImageMemoryRequirements(VkDevice device, VkImage image, VkMemoryRequirements* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_GetImageMemoryRequirements(VkDevice device, VkImage image, VkMemoryRequirements* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_GetImageMemoryRequirements(VkDevice device, VkImage image, VkMemoryRequirements* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_GetImageMemoryRequirements(VkDevice device, VkImage image, VkMemoryRequirements* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_GetImageMemoryRequirements(VkDevice device, VkImage image, VkMemoryRequirements* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_GetImageMemoryRequirements(VkDevice device, VkImage image, VkMemoryRequirements* pMemoryRequirements) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_BindImageMemory(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_BindImageMemory(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_BindImageMemory(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_BindImageMemory(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_BindImageMemory(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_BindImageMemory(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_GetImageSparseMemoryRequirements(VkDevice device, VkImage image, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements* pSparseMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_GetImageSparseMemoryRequirements(VkDevice device, VkImage image, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements* pSparseMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_GetImageSparseMemoryRequirements(VkDevice device, VkImage image, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements* pSparseMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_GetImageSparseMemoryRequirements(VkDevice device, VkImage image, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements* pSparseMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_GetImageSparseMemoryRequirements(VkDevice device, VkImage image, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements* pSparseMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_GetImageSparseMemoryRequirements(VkDevice device, VkImage image, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements* pSparseMemoryRequirements) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_QueueBindSparse(VkQueue queue, uint32_t bindInfoCount, const VkBindSparseInfo* pBindInfo, VkFence fence) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_QueueBindSparse(VkQueue queue, uint32_t bindInfoCount, const VkBindSparseInfo* pBindInfo, VkFence fence) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_QueueBindSparse(VkQueue queue, uint32_t bindInfoCount, const VkBindSparseInfo* pBindInfo, VkFence fence) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_QueueBindSparse(VkQueue queue, uint32_t bindInfoCount, const VkBindSparseInfo* pBindInfo, VkFence fence) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_QueueBindSparse(VkQueue queue, uint32_t bindInfoCount, const VkBindSparseInfo* pBindInfo, VkFence fence) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_QueueBindSparse(VkQueue queue, uint32_t bindInfoCount, const VkBindSparseInfo* pBindInfo, VkFence fence) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateFence(VkDevice device, const VkFenceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_CreateFence(VkDevice device, const VkFenceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_CreateFence(VkDevice device, const VkFenceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_CreateFence(VkDevice device, const VkFenceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_CreateFence(VkDevice device, const VkFenceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_CreateFence(VkDevice device, const VkFenceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_DestroyFence(VkDevice device, VkFence fence, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_DestroyFence(VkDevice device, VkFence fence, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_DestroyFence(VkDevice device, VkFence fence, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_DestroyFence(VkDevice device, VkFence fence, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_DestroyFence(VkDevice device, VkFence fence, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_DestroyFence(VkDevice device, VkFence fence, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_ResetFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_ResetFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_ResetFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_ResetFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_ResetFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_ResetFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_GetFenceStatus(VkDevice device, VkFence fence) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetFenceStatus(VkDevice device, VkFence fence) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetFenceStatus(VkDevice device, VkFence fence) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetFenceStatus(VkDevice device, VkFence fence) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetFenceStatus(VkDevice device, VkFence fence) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetFenceStatus(VkDevice device, VkFence fence) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_WaitForFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences, VkBool32 waitAll, uint64_t timeout) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_WaitForFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences, VkBool32 waitAll, uint64_t timeout) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_WaitForFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences, VkBool32 waitAll, uint64_t timeout) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_WaitForFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences, VkBool32 waitAll, uint64_t timeout) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_WaitForFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences, VkBool32 waitAll, uint64_t timeout) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_WaitForFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences, VkBool32 waitAll, uint64_t timeout) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateSemaphore(VkDevice device, const VkSemaphoreCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_CreateSemaphore(VkDevice device, const VkSemaphoreCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_CreateSemaphore(VkDevice device, const VkSemaphoreCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_CreateSemaphore(VkDevice device, const VkSemaphoreCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_CreateSemaphore(VkDevice device, const VkSemaphoreCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_CreateSemaphore(VkDevice device, const VkSemaphoreCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_DestroySemaphore(VkDevice device, VkSemaphore semaphore, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_DestroySemaphore(VkDevice device, VkSemaphore semaphore, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_DestroySemaphore(VkDevice device, VkSemaphore semaphore, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_DestroySemaphore(VkDevice device, VkSemaphore semaphore, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_DestroySemaphore(VkDevice device, VkSemaphore semaphore, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_DestroySemaphore(VkDevice device, VkSemaphore semaphore, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateEvent(VkDevice device, const VkEventCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkEvent* pEvent) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_CreateEvent(VkDevice device, const VkEventCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkEvent* pEvent) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_CreateEvent(VkDevice device, const VkEventCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkEvent* pEvent) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_CreateEvent(VkDevice device, const VkEventCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkEvent* pEvent) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_CreateEvent(VkDevice device, const VkEventCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkEvent* pEvent) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_CreateEvent(VkDevice device, const VkEventCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkEvent* pEvent) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_DestroyEvent(VkDevice device, VkEvent event, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_DestroyEvent(VkDevice device, VkEvent event, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_DestroyEvent(VkDevice device, VkEvent event, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_DestroyEvent(VkDevice device, VkEvent event, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_DestroyEvent(VkDevice device, VkEvent event, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_DestroyEvent(VkDevice device, VkEvent event, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_GetEventStatus(VkDevice device, VkEvent event) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetEventStatus(VkDevice device, VkEvent event) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetEventStatus(VkDevice device, VkEvent event) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetEventStatus(VkDevice device, VkEvent event) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetEventStatus(VkDevice device, VkEvent event) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetEventStatus(VkDevice device, VkEvent event) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_SetEvent(VkDevice device, VkEvent event) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_SetEvent(VkDevice device, VkEvent event) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_SetEvent(VkDevice device, VkEvent event) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_SetEvent(VkDevice device, VkEvent event) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_SetEvent(VkDevice device, VkEvent event) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_SetEvent(VkDevice device, VkEvent event) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_ResetEvent(VkDevice device, VkEvent event) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_ResetEvent(VkDevice device, VkEvent event) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_ResetEvent(VkDevice device, VkEvent event) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_ResetEvent(VkDevice device, VkEvent event) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_ResetEvent(VkDevice device, VkEvent event) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_ResetEvent(VkDevice device, VkEvent event) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateQueryPool(VkDevice device, const VkQueryPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkQueryPool* pQueryPool) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_CreateQueryPool(VkDevice device, const VkQueryPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkQueryPool* pQueryPool) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_CreateQueryPool(VkDevice device, const VkQueryPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkQueryPool* pQueryPool) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_CreateQueryPool(VkDevice device, const VkQueryPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkQueryPool* pQueryPool) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_CreateQueryPool(VkDevice device, const VkQueryPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkQueryPool* pQueryPool) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_CreateQueryPool(VkDevice device, const VkQueryPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkQueryPool* pQueryPool) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_DestroyQueryPool(VkDevice device, VkQueryPool queryPool, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_DestroyQueryPool(VkDevice device, VkQueryPool queryPool, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_DestroyQueryPool(VkDevice device, VkQueryPool queryPool, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_DestroyQueryPool(VkDevice device, VkQueryPool queryPool, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_DestroyQueryPool(VkDevice device, VkQueryPool queryPool, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_DestroyQueryPool(VkDevice device, VkQueryPool queryPool, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_GetQueryPoolResults(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, VkDeviceSize stride, VkQueryResultFlags flags) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetQueryPoolResults(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, VkDeviceSize stride, VkQueryResultFlags flags) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetQueryPoolResults(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, VkDeviceSize stride, VkQueryResultFlags flags) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetQueryPoolResults(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, VkDeviceSize stride, VkQueryResultFlags flags) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetQueryPoolResults(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, VkDeviceSize stride, VkQueryResultFlags flags) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetQueryPoolResults(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, VkDeviceSize stride, VkQueryResultFlags flags) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_ResetQueryPool(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_ResetQueryPool(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_ResetQueryPool(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_ResetQueryPool(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_ResetQueryPool(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_ResetQueryPool(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_ResetQueryPoolEXT(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_ResetQueryPoolEXT(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_ResetQueryPoolEXT(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_ResetQueryPoolEXT(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_ResetQueryPoolEXT(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_ResetQueryPoolEXT(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateBuffer(VkDevice device, const VkBufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_CreateBuffer(VkDevice device, const VkBufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_CreateBuffer(VkDevice device, const VkBufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_CreateBuffer(VkDevice device, const VkBufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_CreateBuffer(VkDevice device, const VkBufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_CreateBuffer(VkDevice device, const VkBufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_DestroyBuffer(VkDevice device, VkBuffer buffer, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_DestroyBuffer(VkDevice device, VkBuffer buffer, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_DestroyBuffer(VkDevice device, VkBuffer buffer, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_DestroyBuffer(VkDevice device, VkBuffer buffer, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_DestroyBuffer(VkDevice device, VkBuffer buffer, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_DestroyBuffer(VkDevice device, VkBuffer buffer, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateBufferView(VkDevice device, const VkBufferViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferView* pView) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_CreateBufferView(VkDevice device, const VkBufferViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferView* pView) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_CreateBufferView(VkDevice device, const VkBufferViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferView* pView) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_CreateBufferView(VkDevice device, const VkBufferViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferView* pView) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_CreateBufferView(VkDevice device, const VkBufferViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferView* pView) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_CreateBufferView(VkDevice device, const VkBufferViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferView* pView) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_DestroyBufferView(VkDevice device, VkBufferView bufferView, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_DestroyBufferView(VkDevice device, VkBufferView bufferView, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_DestroyBufferView(VkDevice device, VkBufferView bufferView, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_DestroyBufferView(VkDevice device, VkBufferView bufferView, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_DestroyBufferView(VkDevice device, VkBufferView bufferView, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_DestroyBufferView(VkDevice device, VkBufferView bufferView, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateImage(VkDevice device, const VkImageCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImage* pImage) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_CreateImage(VkDevice device, const VkImageCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImage* pImage) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_CreateImage(VkDevice device, const VkImageCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImage* pImage) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_CreateImage(VkDevice device, const VkImageCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImage* pImage) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_CreateImage(VkDevice device, const VkImageCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImage* pImage) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_CreateImage(VkDevice device, const VkImageCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImage* pImage) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_DestroyImage(VkDevice device, VkImage image, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_DestroyImage(VkDevice device, VkImage image, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_DestroyImage(VkDevice device, VkImage image, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_DestroyImage(VkDevice device, VkImage image, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_DestroyImage(VkDevice device, VkImage image, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_DestroyImage(VkDevice device, VkImage image, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_GetImageSubresourceLayout(VkDevice device, VkImage image, const VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_GetImageSubresourceLayout(VkDevice device, VkImage image, const VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_GetImageSubresourceLayout(VkDevice device, VkImage image, const VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_GetImageSubresourceLayout(VkDevice device, VkImage image, const VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_GetImageSubresourceLayout(VkDevice device, VkImage image, const VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_GetImageSubresourceLayout(VkDevice device, VkImage image, const VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateImageView(VkDevice device, const VkImageViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImageView* pView) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_CreateImageView(VkDevice device, const VkImageViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImageView* pView) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_CreateImageView(VkDevice device, const VkImageViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImageView* pView) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_CreateImageView(VkDevice device, const VkImageViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImageView* pView) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_CreateImageView(VkDevice device, const VkImageViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImageView* pView) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_CreateImageView(VkDevice device, const VkImageViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImageView* pView) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_DestroyImageView(VkDevice device, VkImageView imageView, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_DestroyImageView(VkDevice device, VkImageView imageView, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_DestroyImageView(VkDevice device, VkImageView imageView, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_DestroyImageView(VkDevice device, VkImageView imageView, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_DestroyImageView(VkDevice device, VkImageView imageView, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_DestroyImageView(VkDevice device, VkImageView imageView, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateShaderModule(VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_CreateShaderModule(VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_CreateShaderModule(VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_CreateShaderModule(VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_CreateShaderModule(VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_CreateShaderModule(VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_DestroyShaderModule(VkDevice device, VkShaderModule shaderModule, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_DestroyShaderModule(VkDevice device, VkShaderModule shaderModule, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_DestroyShaderModule(VkDevice device, VkShaderModule shaderModule, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_DestroyShaderModule(VkDevice device, VkShaderModule shaderModule, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_DestroyShaderModule(VkDevice device, VkShaderModule shaderModule, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_DestroyShaderModule(VkDevice device, VkShaderModule shaderModule, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_CreatePipelineCache(VkDevice device, const VkPipelineCacheCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineCache* pPipelineCache) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_CreatePipelineCache(VkDevice device, const VkPipelineCacheCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineCache* pPipelineCache) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_CreatePipelineCache(VkDevice device, const VkPipelineCacheCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineCache* pPipelineCache) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_CreatePipelineCache(VkDevice device, const VkPipelineCacheCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineCache* pPipelineCache) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_CreatePipelineCache(VkDevice device, const VkPipelineCacheCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineCache* pPipelineCache) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_CreatePipelineCache(VkDevice device, const VkPipelineCacheCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineCache* pPipelineCache) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_DestroyPipelineCache(VkDevice device, VkPipelineCache pipelineCache, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_DestroyPipelineCache(VkDevice device, VkPipelineCache pipelineCache, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_DestroyPipelineCache(VkDevice device, VkPipelineCache pipelineCache, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_DestroyPipelineCache(VkDevice device, VkPipelineCache pipelineCache, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_DestroyPipelineCache(VkDevice device, VkPipelineCache pipelineCache, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_DestroyPipelineCache(VkDevice device, VkPipelineCache pipelineCache, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_GetPipelineCacheData(VkDevice device, VkPipelineCache pipelineCache, size_t* pDataSize, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetPipelineCacheData(VkDevice device, VkPipelineCache pipelineCache, size_t* pDataSize, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetPipelineCacheData(VkDevice device, VkPipelineCache pipelineCache, size_t* pDataSize, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetPipelineCacheData(VkDevice device, VkPipelineCache pipelineCache, size_t* pDataSize, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetPipelineCacheData(VkDevice device, VkPipelineCache pipelineCache, size_t* pDataSize, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetPipelineCacheData(VkDevice device, VkPipelineCache pipelineCache, size_t* pDataSize, void* pData) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_MergePipelineCaches(VkDevice device, VkPipelineCache dstCache, uint32_t srcCacheCount, const VkPipelineCache* pSrcCaches) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_MergePipelineCaches(VkDevice device, VkPipelineCache dstCache, uint32_t srcCacheCount, const VkPipelineCache* pSrcCaches) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_MergePipelineCaches(VkDevice device, VkPipelineCache dstCache, uint32_t srcCacheCount, const VkPipelineCache* pSrcCaches) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_MergePipelineCaches(VkDevice device, VkPipelineCache dstCache, uint32_t srcCacheCount, const VkPipelineCache* pSrcCaches) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_MergePipelineCaches(VkDevice device, VkPipelineCache dstCache, uint32_t srcCacheCount, const VkPipelineCache* pSrcCaches) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_MergePipelineCaches(VkDevice device, VkPipelineCache dstCache, uint32_t srcCacheCount, const VkPipelineCache* pSrcCaches) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateGraphicsPipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkGraphicsPipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_CreateGraphicsPipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkGraphicsPipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_CreateGraphicsPipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkGraphicsPipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_CreateGraphicsPipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkGraphicsPipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_CreateGraphicsPipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkGraphicsPipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_CreateGraphicsPipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkGraphicsPipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateComputePipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkComputePipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_CreateComputePipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkComputePipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_CreateComputePipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkComputePipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_CreateComputePipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkComputePipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_CreateComputePipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkComputePipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_CreateComputePipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkComputePipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_GetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI(VkDevice device, VkRenderPass renderpass, VkExtent2D* pMaxWorkgroupSize) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI(VkDevice device, VkRenderPass renderpass, VkExtent2D* pMaxWorkgroupSize) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI(VkDevice device, VkRenderPass renderpass, VkExtent2D* pMaxWorkgroupSize) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI(VkDevice device, VkRenderPass renderpass, VkExtent2D* pMaxWorkgroupSize) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI(VkDevice device, VkRenderPass renderpass, VkExtent2D* pMaxWorkgroupSize) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI(VkDevice device, VkRenderPass renderpass, VkExtent2D* pMaxWorkgroupSize) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_DestroyPipeline(VkDevice device, VkPipeline pipeline, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_DestroyPipeline(VkDevice device, VkPipeline pipeline, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_DestroyPipeline(VkDevice device, VkPipeline pipeline, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_DestroyPipeline(VkDevice device, VkPipeline pipeline, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_DestroyPipeline(VkDevice device, VkPipeline pipeline, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_DestroyPipeline(VkDevice device, VkPipeline pipeline, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_CreatePipelineLayout(VkDevice device, const VkPipelineLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_CreatePipelineLayout(VkDevice device, const VkPipelineLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_CreatePipelineLayout(VkDevice device, const VkPipelineLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_CreatePipelineLayout(VkDevice device, const VkPipelineLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_CreatePipelineLayout(VkDevice device, const VkPipelineLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_CreatePipelineLayout(VkDevice device, const VkPipelineLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_DestroyPipelineLayout(VkDevice device, VkPipelineLayout pipelineLayout, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_DestroyPipelineLayout(VkDevice device, VkPipelineLayout pipelineLayout, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_DestroyPipelineLayout(VkDevice device, VkPipelineLayout pipelineLayout, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_DestroyPipelineLayout(VkDevice device, VkPipelineLayout pipelineLayout, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_DestroyPipelineLayout(VkDevice device, VkPipelineLayout pipelineLayout, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_DestroyPipelineLayout(VkDevice device, VkPipelineLayout pipelineLayout, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateSampler(VkDevice device, const VkSamplerCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSampler* pSampler) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_CreateSampler(VkDevice device, const VkSamplerCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSampler* pSampler) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_CreateSampler(VkDevice device, const VkSamplerCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSampler* pSampler) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_CreateSampler(VkDevice device, const VkSamplerCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSampler* pSampler) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_CreateSampler(VkDevice device, const VkSamplerCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSampler* pSampler) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_CreateSampler(VkDevice device, const VkSamplerCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSampler* pSampler) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_DestroySampler(VkDevice device, VkSampler sampler, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_DestroySampler(VkDevice device, VkSampler sampler, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_DestroySampler(VkDevice device, VkSampler sampler, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_DestroySampler(VkDevice device, VkSampler sampler, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_DestroySampler(VkDevice device, VkSampler sampler, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_DestroySampler(VkDevice device, VkSampler sampler, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateDescriptorSetLayout(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorSetLayout* pSetLayout) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_CreateDescriptorSetLayout(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorSetLayout* pSetLayout) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_CreateDescriptorSetLayout(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorSetLayout* pSetLayout) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_CreateDescriptorSetLayout(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorSetLayout* pSetLayout) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_CreateDescriptorSetLayout(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorSetLayout* pSetLayout) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_CreateDescriptorSetLayout(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorSetLayout* pSetLayout) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_DestroyDescriptorSetLayout(VkDevice device, VkDescriptorSetLayout descriptorSetLayout, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_DestroyDescriptorSetLayout(VkDevice device, VkDescriptorSetLayout descriptorSetLayout, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_DestroyDescriptorSetLayout(VkDevice device, VkDescriptorSetLayout descriptorSetLayout, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_DestroyDescriptorSetLayout(VkDevice device, VkDescriptorSetLayout descriptorSetLayout, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_DestroyDescriptorSetLayout(VkDevice device, VkDescriptorSetLayout descriptorSetLayout, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_DestroyDescriptorSetLayout(VkDevice device, VkDescriptorSetLayout descriptorSetLayout, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateDescriptorPool(VkDevice device, const VkDescriptorPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorPool* pDescriptorPool) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_CreateDescriptorPool(VkDevice device, const VkDescriptorPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorPool* pDescriptorPool) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_CreateDescriptorPool(VkDevice device, const VkDescriptorPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorPool* pDescriptorPool) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_CreateDescriptorPool(VkDevice device, const VkDescriptorPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorPool* pDescriptorPool) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_CreateDescriptorPool(VkDevice device, const VkDescriptorPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorPool* pDescriptorPool) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_CreateDescriptorPool(VkDevice device, const VkDescriptorPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorPool* pDescriptorPool) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_DestroyDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_DestroyDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_DestroyDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_DestroyDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_DestroyDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_DestroyDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_ResetDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags flags) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_ResetDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags flags) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_ResetDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags flags) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_ResetDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags flags) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_ResetDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags flags) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_ResetDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags flags) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_AllocateDescriptorSets(VkDevice device, const VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_AllocateDescriptorSets(VkDevice device, const VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_AllocateDescriptorSets(VkDevice device, const VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_AllocateDescriptorSets(VkDevice device, const VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_AllocateDescriptorSets(VkDevice device, const VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_AllocateDescriptorSets(VkDevice device, const VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_FreeDescriptorSets(VkDevice device, VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_FreeDescriptorSets(VkDevice device, VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_FreeDescriptorSets(VkDevice device, VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_FreeDescriptorSets(VkDevice device, VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_FreeDescriptorSets(VkDevice device, VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_FreeDescriptorSets(VkDevice device, VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_UpdateDescriptorSets(VkDevice device, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, const VkCopyDescriptorSet* pDescriptorCopies) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_UpdateDescriptorSets(VkDevice device, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, const VkCopyDescriptorSet* pDescriptorCopies) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_UpdateDescriptorSets(VkDevice device, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, const VkCopyDescriptorSet* pDescriptorCopies) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_UpdateDescriptorSets(VkDevice device, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, const VkCopyDescriptorSet* pDescriptorCopies) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_UpdateDescriptorSets(VkDevice device, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, const VkCopyDescriptorSet* pDescriptorCopies) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_UpdateDescriptorSets(VkDevice device, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, const VkCopyDescriptorSet* pDescriptorCopies) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateFramebuffer(VkDevice device, const VkFramebufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFramebuffer* pFramebuffer) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_CreateFramebuffer(VkDevice device, const VkFramebufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFramebuffer* pFramebuffer) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_CreateFramebuffer(VkDevice device, const VkFramebufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFramebuffer* pFramebuffer) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_CreateFramebuffer(VkDevice device, const VkFramebufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFramebuffer* pFramebuffer) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_CreateFramebuffer(VkDevice device, const VkFramebufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFramebuffer* pFramebuffer) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_CreateFramebuffer(VkDevice device, const VkFramebufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFramebuffer* pFramebuffer) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_DestroyFramebuffer(VkDevice device, VkFramebuffer framebuffer, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_DestroyFramebuffer(VkDevice device, VkFramebuffer framebuffer, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_DestroyFramebuffer(VkDevice device, VkFramebuffer framebuffer, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_DestroyFramebuffer(VkDevice device, VkFramebuffer framebuffer, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_DestroyFramebuffer(VkDevice device, VkFramebuffer framebuffer, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_DestroyFramebuffer(VkDevice device, VkFramebuffer framebuffer, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateRenderPass(VkDevice device, const VkRenderPassCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_CreateRenderPass(VkDevice device, const VkRenderPassCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_CreateRenderPass(VkDevice device, const VkRenderPassCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_CreateRenderPass(VkDevice device, const VkRenderPassCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_CreateRenderPass(VkDevice device, const VkRenderPassCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_CreateRenderPass(VkDevice device, const VkRenderPassCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_DestroyRenderPass(VkDevice device, VkRenderPass renderPass, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_DestroyRenderPass(VkDevice device, VkRenderPass renderPass, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_DestroyRenderPass(VkDevice device, VkRenderPass renderPass, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_DestroyRenderPass(VkDevice device, VkRenderPass renderPass, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_DestroyRenderPass(VkDevice device, VkRenderPass renderPass, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_DestroyRenderPass(VkDevice device, VkRenderPass renderPass, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_GetRenderAreaGranularity(VkDevice device, VkRenderPass renderPass, VkExtent2D* pGranularity) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_GetRenderAreaGranularity(VkDevice device, VkRenderPass renderPass, VkExtent2D* pGranularity) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_GetRenderAreaGranularity(VkDevice device, VkRenderPass renderPass, VkExtent2D* pGranularity) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_GetRenderAreaGranularity(VkDevice device, VkRenderPass renderPass, VkExtent2D* pGranularity) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_GetRenderAreaGranularity(VkDevice device, VkRenderPass renderPass, VkExtent2D* pGranularity) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_GetRenderAreaGranularity(VkDevice device, VkRenderPass renderPass, VkExtent2D* pGranularity) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateCommandPool(VkDevice device, const VkCommandPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_CreateCommandPool(VkDevice device, const VkCommandPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_CreateCommandPool(VkDevice device, const VkCommandPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_CreateCommandPool(VkDevice device, const VkCommandPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_CreateCommandPool(VkDevice device, const VkCommandPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_CreateCommandPool(VkDevice device, const VkCommandPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_DestroyCommandPool(VkDevice device, VkCommandPool commandPool, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_DestroyCommandPool(VkDevice device, VkCommandPool commandPool, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_DestroyCommandPool(VkDevice device, VkCommandPool commandPool, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_DestroyCommandPool(VkDevice device, VkCommandPool commandPool, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_DestroyCommandPool(VkDevice device, VkCommandPool commandPool, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_DestroyCommandPool(VkDevice device, VkCommandPool commandPool, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_ResetCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolResetFlags flags) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_ResetCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolResetFlags flags) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_ResetCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolResetFlags flags) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_ResetCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolResetFlags flags) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_ResetCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolResetFlags flags) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_ResetCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolResetFlags flags) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_AllocateCommandBuffers(VkDevice device, const VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_AllocateCommandBuffers(VkDevice device, const VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_AllocateCommandBuffers(VkDevice device, const VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_AllocateCommandBuffers(VkDevice device, const VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_AllocateCommandBuffers(VkDevice device, const VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_AllocateCommandBuffers(VkDevice device, const VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_FreeCommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_FreeCommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_FreeCommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_FreeCommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_FreeCommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_FreeCommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_BeginCommandBuffer(VkCommandBuffer commandBuffer, const VkCommandBufferBeginInfo* pBeginInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_BeginCommandBuffer(VkCommandBuffer commandBuffer, const VkCommandBufferBeginInfo* pBeginInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_BeginCommandBuffer(VkCommandBuffer commandBuffer, const VkCommandBufferBeginInfo* pBeginInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_BeginCommandBuffer(VkCommandBuffer commandBuffer, const VkCommandBufferBeginInfo* pBeginInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_BeginCommandBuffer(VkCommandBuffer commandBuffer, const VkCommandBufferBeginInfo* pBeginInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_BeginCommandBuffer(VkCommandBuffer commandBuffer, const VkCommandBufferBeginInfo* pBeginInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_EndCommandBuffer(VkCommandBuffer commandBuffer) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_EndCommandBuffer(VkCommandBuffer commandBuffer) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_EndCommandBuffer(VkCommandBuffer commandBuffer) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_EndCommandBuffer(VkCommandBuffer commandBuffer) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_EndCommandBuffer(VkCommandBuffer commandBuffer) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_EndCommandBuffer(VkCommandBuffer commandBuffer) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_ResetCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_ResetCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_ResetCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_ResetCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_ResetCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_ResetCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdBindPipeline(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdBindPipeline(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdBindPipeline(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdBindPipeline(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdBindPipeline(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdBindPipeline(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetViewport(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pViewports) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetViewport(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pViewports) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetViewport(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pViewports) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetViewport(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pViewports) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetViewport(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pViewports) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetViewport(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pViewports) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetScissor(VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pScissors) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetScissor(VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pScissors) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetScissor(VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pScissors) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetScissor(VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pScissors) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetScissor(VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pScissors) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetScissor(VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pScissors) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetLineWidth(VkCommandBuffer commandBuffer, float lineWidth) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetLineWidth(VkCommandBuffer commandBuffer, float lineWidth) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetLineWidth(VkCommandBuffer commandBuffer, float lineWidth) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetLineWidth(VkCommandBuffer commandBuffer, float lineWidth) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetLineWidth(VkCommandBuffer commandBuffer, float lineWidth) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetLineWidth(VkCommandBuffer commandBuffer, float lineWidth) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetDepthBias(VkCommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetDepthBias(VkCommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetDepthBias(VkCommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetDepthBias(VkCommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetDepthBias(VkCommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetDepthBias(VkCommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetBlendConstants(VkCommandBuffer commandBuffer, const float blendConstants[4]) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetBlendConstants(VkCommandBuffer commandBuffer, const float blendConstants[4]) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetBlendConstants(VkCommandBuffer commandBuffer, const float blendConstants[4]) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetBlendConstants(VkCommandBuffer commandBuffer, const float blendConstants[4]) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetBlendConstants(VkCommandBuffer commandBuffer, const float blendConstants[4]) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetBlendConstants(VkCommandBuffer commandBuffer, const float blendConstants[4]) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetDepthBounds(VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetDepthBounds(VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetDepthBounds(VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetDepthBounds(VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetDepthBounds(VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetDepthBounds(VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetStencilCompareMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetStencilCompareMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetStencilCompareMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetStencilCompareMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetStencilCompareMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetStencilCompareMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetStencilWriteMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetStencilWriteMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetStencilWriteMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetStencilWriteMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetStencilWriteMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetStencilWriteMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetStencilReference(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetStencilReference(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetStencilReference(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetStencilReference(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetStencilReference(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetStencilReference(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdBindDescriptorSets(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdBindDescriptorSets(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdBindDescriptorSets(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdBindDescriptorSets(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdBindDescriptorSets(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdBindDescriptorSets(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdBindIndexBuffer(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdBindIndexBuffer(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdBindIndexBuffer(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdBindIndexBuffer(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdBindIndexBuffer(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdBindIndexBuffer(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdBindVertexBuffers(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdBindVertexBuffers(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdBindVertexBuffers(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdBindVertexBuffers(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdBindVertexBuffers(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdBindVertexBuffers(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdDraw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdDraw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdDraw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdDraw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdDraw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdDraw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdDrawIndexed(VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdDrawIndexed(VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdDrawIndexed(VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdDrawIndexed(VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdDrawIndexed(VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdDrawIndexed(VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdDrawMultiEXT(VkCommandBuffer commandBuffer, uint32_t drawCount, const VkMultiDrawInfoEXT* pVertexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdDrawMultiEXT(VkCommandBuffer commandBuffer, uint32_t drawCount, const VkMultiDrawInfoEXT* pVertexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdDrawMultiEXT(VkCommandBuffer commandBuffer, uint32_t drawCount, const VkMultiDrawInfoEXT* pVertexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdDrawMultiEXT(VkCommandBuffer commandBuffer, uint32_t drawCount, const VkMultiDrawInfoEXT* pVertexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdDrawMultiEXT(VkCommandBuffer commandBuffer, uint32_t drawCount, const VkMultiDrawInfoEXT* pVertexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdDrawMultiEXT(VkCommandBuffer commandBuffer, uint32_t drawCount, const VkMultiDrawInfoEXT* pVertexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdDrawMultiIndexedEXT(VkCommandBuffer commandBuffer, uint32_t drawCount, const VkMultiDrawIndexedInfoEXT* pIndexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride, const int32_t* pVertexOffset) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdDrawMultiIndexedEXT(VkCommandBuffer commandBuffer, uint32_t drawCount, const VkMultiDrawIndexedInfoEXT* pIndexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride, const int32_t* pVertexOffset) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdDrawMultiIndexedEXT(VkCommandBuffer commandBuffer, uint32_t drawCount, const VkMultiDrawIndexedInfoEXT* pIndexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride, const int32_t* pVertexOffset) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdDrawMultiIndexedEXT(VkCommandBuffer commandBuffer, uint32_t drawCount, const VkMultiDrawIndexedInfoEXT* pIndexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride, const int32_t* pVertexOffset) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdDrawMultiIndexedEXT(VkCommandBuffer commandBuffer, uint32_t drawCount, const VkMultiDrawIndexedInfoEXT* pIndexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride, const int32_t* pVertexOffset) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdDrawMultiIndexedEXT(VkCommandBuffer commandBuffer, uint32_t drawCount, const VkMultiDrawIndexedInfoEXT* pIndexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride, const int32_t* pVertexOffset) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdDrawIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdDrawIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdDrawIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdDrawIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdDrawIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdDrawIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdDrawIndexedIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdDrawIndexedIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdDrawIndexedIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdDrawIndexedIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdDrawIndexedIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdDrawIndexedIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdDispatch(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdDispatch(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdDispatch(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdDispatch(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdDispatch(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdDispatch(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdDispatchIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdDispatchIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdDispatchIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdDispatchIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdDispatchIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdDispatchIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSubpassShadingHUAWEI(VkCommandBuffer commandBuffer) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSubpassShadingHUAWEI(VkCommandBuffer commandBuffer) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSubpassShadingHUAWEI(VkCommandBuffer commandBuffer) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSubpassShadingHUAWEI(VkCommandBuffer commandBuffer) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSubpassShadingHUAWEI(VkCommandBuffer commandBuffer) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSubpassShadingHUAWEI(VkCommandBuffer commandBuffer) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdDrawClusterHUAWEI(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdDrawClusterHUAWEI(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdDrawClusterHUAWEI(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdDrawClusterHUAWEI(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdDrawClusterHUAWEI(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdDrawClusterHUAWEI(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdDrawClusterIndirectHUAWEI(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdDrawClusterIndirectHUAWEI(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdDrawClusterIndirectHUAWEI(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdDrawClusterIndirectHUAWEI(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdDrawClusterIndirectHUAWEI(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdDrawClusterIndirectHUAWEI(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdCopyBuffer(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdCopyBuffer(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdCopyBuffer(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdCopyBuffer(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdCopyBuffer(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdCopyBuffer(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdCopyImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdCopyImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdCopyImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdCopyImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdCopyImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdCopyImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdBlitImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdBlitImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdBlitImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdBlitImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdBlitImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdBlitImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdCopyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdCopyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdCopyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdCopyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdCopyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdCopyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdCopyImageToBuffer(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdCopyImageToBuffer(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdCopyImageToBuffer(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdCopyImageToBuffer(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdCopyImageToBuffer(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdCopyImageToBuffer(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdCopyMemoryIndirectNV(VkCommandBuffer commandBuffer, VkDeviceAddress copyBufferAddress, uint32_t copyCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdCopyMemoryIndirectNV(VkCommandBuffer commandBuffer, VkDeviceAddress copyBufferAddress, uint32_t copyCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdCopyMemoryIndirectNV(VkCommandBuffer commandBuffer, VkDeviceAddress copyBufferAddress, uint32_t copyCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdCopyMemoryIndirectNV(VkCommandBuffer commandBuffer, VkDeviceAddress copyBufferAddress, uint32_t copyCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdCopyMemoryIndirectNV(VkCommandBuffer commandBuffer, VkDeviceAddress copyBufferAddress, uint32_t copyCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdCopyMemoryIndirectNV(VkCommandBuffer commandBuffer, VkDeviceAddress copyBufferAddress, uint32_t copyCount, uint32_t stride) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdCopyMemoryToImageIndirectNV(VkCommandBuffer commandBuffer, VkDeviceAddress copyBufferAddress, uint32_t copyCount, uint32_t stride, VkImage dstImage, VkImageLayout dstImageLayout, const VkImageSubresourceLayers* pImageSubresources) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdCopyMemoryToImageIndirectNV(VkCommandBuffer commandBuffer, VkDeviceAddress copyBufferAddress, uint32_t copyCount, uint32_t stride, VkImage dstImage, VkImageLayout dstImageLayout, const VkImageSubresourceLayers* pImageSubresources) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdCopyMemoryToImageIndirectNV(VkCommandBuffer commandBuffer, VkDeviceAddress copyBufferAddress, uint32_t copyCount, uint32_t stride, VkImage dstImage, VkImageLayout dstImageLayout, const VkImageSubresourceLayers* pImageSubresources) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdCopyMemoryToImageIndirectNV(VkCommandBuffer commandBuffer, VkDeviceAddress copyBufferAddress, uint32_t copyCount, uint32_t stride, VkImage dstImage, VkImageLayout dstImageLayout, const VkImageSubresourceLayers* pImageSubresources) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdCopyMemoryToImageIndirectNV(VkCommandBuffer commandBuffer, VkDeviceAddress copyBufferAddress, uint32_t copyCount, uint32_t stride, VkImage dstImage, VkImageLayout dstImageLayout, const VkImageSubresourceLayers* pImageSubresources) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdCopyMemoryToImageIndirectNV(VkCommandBuffer commandBuffer, VkDeviceAddress copyBufferAddress, uint32_t copyCount, uint32_t stride, VkImage dstImage, VkImageLayout dstImageLayout, const VkImageSubresourceLayers* pImageSubresources) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdUpdateBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdUpdateBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdUpdateBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdUpdateBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdUpdateBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdUpdateBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdFillBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdFillBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdFillBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdFillBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdFillBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdFillBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdClearColorImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdClearColorImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdClearColorImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdClearColorImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdClearColorImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdClearColorImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdClearDepthStencilImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdClearDepthStencilImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdClearDepthStencilImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdClearDepthStencilImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdClearDepthStencilImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdClearDepthStencilImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdClearAttachments(VkCommandBuffer commandBuffer, uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdClearAttachments(VkCommandBuffer commandBuffer, uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdClearAttachments(VkCommandBuffer commandBuffer, uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdClearAttachments(VkCommandBuffer commandBuffer, uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdClearAttachments(VkCommandBuffer commandBuffer, uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdClearAttachments(VkCommandBuffer commandBuffer, uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdResolveImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageResolve* pRegions) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdResolveImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageResolve* pRegions) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdResolveImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageResolve* pRegions) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdResolveImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageResolve* pRegions) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdResolveImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageResolve* pRegions) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdResolveImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageResolve* pRegions) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdResetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdResetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdResetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdResetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdResetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdResetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdWaitEvents(VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdWaitEvents(VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdWaitEvents(VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdWaitEvents(VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdWaitEvents(VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdWaitEvents(VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdPipelineBarrier(VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdPipelineBarrier(VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdPipelineBarrier(VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdPipelineBarrier(VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdPipelineBarrier(VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdPipelineBarrier(VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdBeginQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdBeginQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdBeginQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdBeginQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdBeginQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdBeginQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdEndQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdEndQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdEndQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdEndQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdEndQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdEndQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdBeginConditionalRenderingEXT(VkCommandBuffer commandBuffer, const VkConditionalRenderingBeginInfoEXT* pConditionalRenderingBegin) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdBeginConditionalRenderingEXT(VkCommandBuffer commandBuffer, const VkConditionalRenderingBeginInfoEXT* pConditionalRenderingBegin) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdBeginConditionalRenderingEXT(VkCommandBuffer commandBuffer, const VkConditionalRenderingBeginInfoEXT* pConditionalRenderingBegin) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdBeginConditionalRenderingEXT(VkCommandBuffer commandBuffer, const VkConditionalRenderingBeginInfoEXT* pConditionalRenderingBegin) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdBeginConditionalRenderingEXT(VkCommandBuffer commandBuffer, const VkConditionalRenderingBeginInfoEXT* pConditionalRenderingBegin) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdBeginConditionalRenderingEXT(VkCommandBuffer commandBuffer, const VkConditionalRenderingBeginInfoEXT* pConditionalRenderingBegin) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdEndConditionalRenderingEXT(VkCommandBuffer commandBuffer) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdEndConditionalRenderingEXT(VkCommandBuffer commandBuffer) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdEndConditionalRenderingEXT(VkCommandBuffer commandBuffer) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdEndConditionalRenderingEXT(VkCommandBuffer commandBuffer) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdEndConditionalRenderingEXT(VkCommandBuffer commandBuffer) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdEndConditionalRenderingEXT(VkCommandBuffer commandBuffer) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdResetQueryPool(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdResetQueryPool(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdResetQueryPool(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdResetQueryPool(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdResetQueryPool(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdResetQueryPool(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdWriteTimestamp(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdWriteTimestamp(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdWriteTimestamp(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdWriteTimestamp(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdWriteTimestamp(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdWriteTimestamp(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdCopyQueryPoolResults(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdCopyQueryPoolResults(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdCopyQueryPoolResults(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdCopyQueryPoolResults(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdCopyQueryPoolResults(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdCopyQueryPoolResults(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdPushConstants(VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdPushConstants(VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdPushConstants(VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdPushConstants(VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdPushConstants(VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdPushConstants(VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdBeginRenderPass(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdBeginRenderPass(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdBeginRenderPass(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdBeginRenderPass(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdBeginRenderPass(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdBeginRenderPass(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdNextSubpass(VkCommandBuffer commandBuffer, VkSubpassContents contents) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdNextSubpass(VkCommandBuffer commandBuffer, VkSubpassContents contents) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdNextSubpass(VkCommandBuffer commandBuffer, VkSubpassContents contents) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdNextSubpass(VkCommandBuffer commandBuffer, VkSubpassContents contents) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdNextSubpass(VkCommandBuffer commandBuffer, VkSubpassContents contents) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdNextSubpass(VkCommandBuffer commandBuffer, VkSubpassContents contents) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdEndRenderPass(VkCommandBuffer commandBuffer) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdEndRenderPass(VkCommandBuffer commandBuffer) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdEndRenderPass(VkCommandBuffer commandBuffer) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdEndRenderPass(VkCommandBuffer commandBuffer) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdEndRenderPass(VkCommandBuffer commandBuffer) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdEndRenderPass(VkCommandBuffer commandBuffer) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdExecuteCommands(VkCommandBuffer commandBuffer, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdExecuteCommands(VkCommandBuffer commandBuffer, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdExecuteCommands(VkCommandBuffer commandBuffer, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdExecuteCommands(VkCommandBuffer commandBuffer, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdExecuteCommands(VkCommandBuffer commandBuffer, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdExecuteCommands(VkCommandBuffer commandBuffer, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateSharedSwapchainsKHR(VkDevice device, uint32_t swapchainCount, const VkSwapchainCreateInfoKHR* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchains) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_CreateSharedSwapchainsKHR(VkDevice device, uint32_t swapchainCount, const VkSwapchainCreateInfoKHR* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchains) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_CreateSharedSwapchainsKHR(VkDevice device, uint32_t swapchainCount, const VkSwapchainCreateInfoKHR* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchains) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_CreateSharedSwapchainsKHR(VkDevice device, uint32_t swapchainCount, const VkSwapchainCreateInfoKHR* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchains) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_CreateSharedSwapchainsKHR(VkDevice device, uint32_t swapchainCount, const VkSwapchainCreateInfoKHR* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchains) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_CreateSharedSwapchainsKHR(VkDevice device, uint32_t swapchainCount, const VkSwapchainCreateInfoKHR* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchains) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateSwapchainKHR(VkDevice device, const VkSwapchainCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_CreateSwapchainKHR(VkDevice device, const VkSwapchainCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_CreateSwapchainKHR(VkDevice device, const VkSwapchainCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_CreateSwapchainKHR(VkDevice device, const VkSwapchainCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_CreateSwapchainKHR(VkDevice device, const VkSwapchainCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_CreateSwapchainKHR(VkDevice device, const VkSwapchainCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_DestroySwapchainKHR(VkDevice device, VkSwapchainKHR swapchain, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_DestroySwapchainKHR(VkDevice device, VkSwapchainKHR swapchain, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_DestroySwapchainKHR(VkDevice device, VkSwapchainKHR swapchain, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_DestroySwapchainKHR(VkDevice device, VkSwapchainKHR swapchain, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_DestroySwapchainKHR(VkDevice device, VkSwapchainKHR swapchain, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_DestroySwapchainKHR(VkDevice device, VkSwapchainKHR swapchain, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_GetSwapchainImagesKHR(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetSwapchainImagesKHR(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetSwapchainImagesKHR(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetSwapchainImagesKHR(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetSwapchainImagesKHR(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetSwapchainImagesKHR(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_AcquireNextImageKHR(VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_AcquireNextImageKHR(VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_AcquireNextImageKHR(VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_AcquireNextImageKHR(VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_AcquireNextImageKHR(VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_AcquireNextImageKHR(VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_QueuePresentKHR(VkQueue queue, const VkPresentInfoKHR* pPresentInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_QueuePresentKHR(VkQueue queue, const VkPresentInfoKHR* pPresentInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_QueuePresentKHR(VkQueue queue, const VkPresentInfoKHR* pPresentInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_QueuePresentKHR(VkQueue queue, const VkPresentInfoKHR* pPresentInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_QueuePresentKHR(VkQueue queue, const VkPresentInfoKHR* pPresentInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_QueuePresentKHR(VkQueue queue, const VkPresentInfoKHR* pPresentInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_DebugMarkerSetObjectNameEXT(VkDevice device, const VkDebugMarkerObjectNameInfoEXT* pNameInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_DebugMarkerSetObjectNameEXT(VkDevice device, const VkDebugMarkerObjectNameInfoEXT* pNameInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_DebugMarkerSetObjectNameEXT(VkDevice device, const VkDebugMarkerObjectNameInfoEXT* pNameInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_DebugMarkerSetObjectNameEXT(VkDevice device, const VkDebugMarkerObjectNameInfoEXT* pNameInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_DebugMarkerSetObjectNameEXT(VkDevice device, const VkDebugMarkerObjectNameInfoEXT* pNameInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_DebugMarkerSetObjectNameEXT(VkDevice device, const VkDebugMarkerObjectNameInfoEXT* pNameInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_DebugMarkerSetObjectTagEXT(VkDevice device, const VkDebugMarkerObjectTagInfoEXT* pTagInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_DebugMarkerSetObjectTagEXT(VkDevice device, const VkDebugMarkerObjectTagInfoEXT* pTagInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_DebugMarkerSetObjectTagEXT(VkDevice device, const VkDebugMarkerObjectTagInfoEXT* pTagInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_DebugMarkerSetObjectTagEXT(VkDevice device, const VkDebugMarkerObjectTagInfoEXT* pTagInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_DebugMarkerSetObjectTagEXT(VkDevice device, const VkDebugMarkerObjectTagInfoEXT* pTagInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_DebugMarkerSetObjectTagEXT(VkDevice device, const VkDebugMarkerObjectTagInfoEXT* pTagInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdDebugMarkerBeginEXT(VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdDebugMarkerBeginEXT(VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdDebugMarkerBeginEXT(VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdDebugMarkerBeginEXT(VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdDebugMarkerBeginEXT(VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdDebugMarkerBeginEXT(VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdDebugMarkerEndEXT(VkCommandBuffer commandBuffer) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdDebugMarkerEndEXT(VkCommandBuffer commandBuffer) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdDebugMarkerEndEXT(VkCommandBuffer commandBuffer) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdDebugMarkerEndEXT(VkCommandBuffer commandBuffer) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdDebugMarkerEndEXT(VkCommandBuffer commandBuffer) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdDebugMarkerEndEXT(VkCommandBuffer commandBuffer) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdDebugMarkerInsertEXT(VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdDebugMarkerInsertEXT(VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdDebugMarkerInsertEXT(VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdDebugMarkerInsertEXT(VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdDebugMarkerInsertEXT(VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdDebugMarkerInsertEXT(VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo) ATTR_WEAK;


#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL radv_GetMemoryWin32HandleNV(VkDevice device, VkDeviceMemory memory, VkExternalMemoryHandleTypeFlagsNV handleType, HANDLE* pHandle) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetMemoryWin32HandleNV(VkDevice device, VkDeviceMemory memory, VkExternalMemoryHandleTypeFlagsNV handleType, HANDLE* pHandle) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetMemoryWin32HandleNV(VkDevice device, VkDeviceMemory memory, VkExternalMemoryHandleTypeFlagsNV handleType, HANDLE* pHandle) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetMemoryWin32HandleNV(VkDevice device, VkDeviceMemory memory, VkExternalMemoryHandleTypeFlagsNV handleType, HANDLE* pHandle) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetMemoryWin32HandleNV(VkDevice device, VkDeviceMemory memory, VkExternalMemoryHandleTypeFlagsNV handleType, HANDLE* pHandle) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetMemoryWin32HandleNV(VkDevice device, VkDeviceMemory memory, VkExternalMemoryHandleTypeFlagsNV handleType, HANDLE* pHandle) ATTR_WEAK;


#endif // VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR void VKAPI_CALL radv_CmdExecuteGeneratedCommandsNV(VkCommandBuffer commandBuffer, VkBool32 isPreprocessed, const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdExecuteGeneratedCommandsNV(VkCommandBuffer commandBuffer, VkBool32 isPreprocessed, const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdExecuteGeneratedCommandsNV(VkCommandBuffer commandBuffer, VkBool32 isPreprocessed, const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdExecuteGeneratedCommandsNV(VkCommandBuffer commandBuffer, VkBool32 isPreprocessed, const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdExecuteGeneratedCommandsNV(VkCommandBuffer commandBuffer, VkBool32 isPreprocessed, const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdExecuteGeneratedCommandsNV(VkCommandBuffer commandBuffer, VkBool32 isPreprocessed, const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdPreprocessGeneratedCommandsNV(VkCommandBuffer commandBuffer, const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdPreprocessGeneratedCommandsNV(VkCommandBuffer commandBuffer, const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdPreprocessGeneratedCommandsNV(VkCommandBuffer commandBuffer, const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdPreprocessGeneratedCommandsNV(VkCommandBuffer commandBuffer, const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdPreprocessGeneratedCommandsNV(VkCommandBuffer commandBuffer, const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdPreprocessGeneratedCommandsNV(VkCommandBuffer commandBuffer, const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdBindPipelineShaderGroupNV(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline, uint32_t groupIndex) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdBindPipelineShaderGroupNV(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline, uint32_t groupIndex) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdBindPipelineShaderGroupNV(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline, uint32_t groupIndex) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdBindPipelineShaderGroupNV(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline, uint32_t groupIndex) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdBindPipelineShaderGroupNV(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline, uint32_t groupIndex) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdBindPipelineShaderGroupNV(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline, uint32_t groupIndex) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_GetGeneratedCommandsMemoryRequirementsNV(VkDevice device, const VkGeneratedCommandsMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_GetGeneratedCommandsMemoryRequirementsNV(VkDevice device, const VkGeneratedCommandsMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_GetGeneratedCommandsMemoryRequirementsNV(VkDevice device, const VkGeneratedCommandsMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_GetGeneratedCommandsMemoryRequirementsNV(VkDevice device, const VkGeneratedCommandsMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_GetGeneratedCommandsMemoryRequirementsNV(VkDevice device, const VkGeneratedCommandsMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_GetGeneratedCommandsMemoryRequirementsNV(VkDevice device, const VkGeneratedCommandsMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateIndirectCommandsLayoutNV(VkDevice device, const VkIndirectCommandsLayoutCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkIndirectCommandsLayoutNV* pIndirectCommandsLayout) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_CreateIndirectCommandsLayoutNV(VkDevice device, const VkIndirectCommandsLayoutCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkIndirectCommandsLayoutNV* pIndirectCommandsLayout) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_CreateIndirectCommandsLayoutNV(VkDevice device, const VkIndirectCommandsLayoutCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkIndirectCommandsLayoutNV* pIndirectCommandsLayout) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_CreateIndirectCommandsLayoutNV(VkDevice device, const VkIndirectCommandsLayoutCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkIndirectCommandsLayoutNV* pIndirectCommandsLayout) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_CreateIndirectCommandsLayoutNV(VkDevice device, const VkIndirectCommandsLayoutCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkIndirectCommandsLayoutNV* pIndirectCommandsLayout) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_CreateIndirectCommandsLayoutNV(VkDevice device, const VkIndirectCommandsLayoutCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkIndirectCommandsLayoutNV* pIndirectCommandsLayout) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_DestroyIndirectCommandsLayoutNV(VkDevice device, VkIndirectCommandsLayoutNV indirectCommandsLayout, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_DestroyIndirectCommandsLayoutNV(VkDevice device, VkIndirectCommandsLayoutNV indirectCommandsLayout, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_DestroyIndirectCommandsLayoutNV(VkDevice device, VkIndirectCommandsLayoutNV indirectCommandsLayout, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_DestroyIndirectCommandsLayoutNV(VkDevice device, VkIndirectCommandsLayoutNV indirectCommandsLayout, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_DestroyIndirectCommandsLayoutNV(VkDevice device, VkIndirectCommandsLayoutNV indirectCommandsLayout, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_DestroyIndirectCommandsLayoutNV(VkDevice device, VkIndirectCommandsLayoutNV indirectCommandsLayout, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdPushDescriptorSetKHR(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdPushDescriptorSetKHR(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdPushDescriptorSetKHR(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdPushDescriptorSetKHR(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdPushDescriptorSetKHR(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdPushDescriptorSetKHR(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_TrimCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_TrimCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_TrimCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_TrimCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_TrimCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_TrimCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_TrimCommandPoolKHR(VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_TrimCommandPoolKHR(VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_TrimCommandPoolKHR(VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_TrimCommandPoolKHR(VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_TrimCommandPoolKHR(VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_TrimCommandPoolKHR(VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags) ATTR_WEAK;


#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL radv_GetMemoryWin32HandleKHR(VkDevice device, const VkMemoryGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetMemoryWin32HandleKHR(VkDevice device, const VkMemoryGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetMemoryWin32HandleKHR(VkDevice device, const VkMemoryGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetMemoryWin32HandleKHR(VkDevice device, const VkMemoryGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetMemoryWin32HandleKHR(VkDevice device, const VkMemoryGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetMemoryWin32HandleKHR(VkDevice device, const VkMemoryGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle) ATTR_WEAK;


#endif // VK_USE_PLATFORM_WIN32_KHR
#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL radv_GetMemoryWin32HandlePropertiesKHR(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, HANDLE handle, VkMemoryWin32HandlePropertiesKHR* pMemoryWin32HandleProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetMemoryWin32HandlePropertiesKHR(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, HANDLE handle, VkMemoryWin32HandlePropertiesKHR* pMemoryWin32HandleProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetMemoryWin32HandlePropertiesKHR(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, HANDLE handle, VkMemoryWin32HandlePropertiesKHR* pMemoryWin32HandleProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetMemoryWin32HandlePropertiesKHR(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, HANDLE handle, VkMemoryWin32HandlePropertiesKHR* pMemoryWin32HandleProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetMemoryWin32HandlePropertiesKHR(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, HANDLE handle, VkMemoryWin32HandlePropertiesKHR* pMemoryWin32HandleProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetMemoryWin32HandlePropertiesKHR(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, HANDLE handle, VkMemoryWin32HandlePropertiesKHR* pMemoryWin32HandleProperties) ATTR_WEAK;


#endif // VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL radv_GetMemoryFdKHR(VkDevice device, const VkMemoryGetFdInfoKHR* pGetFdInfo, int* pFd) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetMemoryFdKHR(VkDevice device, const VkMemoryGetFdInfoKHR* pGetFdInfo, int* pFd) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetMemoryFdKHR(VkDevice device, const VkMemoryGetFdInfoKHR* pGetFdInfo, int* pFd) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetMemoryFdKHR(VkDevice device, const VkMemoryGetFdInfoKHR* pGetFdInfo, int* pFd) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetMemoryFdKHR(VkDevice device, const VkMemoryGetFdInfoKHR* pGetFdInfo, int* pFd) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetMemoryFdKHR(VkDevice device, const VkMemoryGetFdInfoKHR* pGetFdInfo, int* pFd) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_GetMemoryFdPropertiesKHR(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, int fd, VkMemoryFdPropertiesKHR* pMemoryFdProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetMemoryFdPropertiesKHR(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, int fd, VkMemoryFdPropertiesKHR* pMemoryFdProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetMemoryFdPropertiesKHR(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, int fd, VkMemoryFdPropertiesKHR* pMemoryFdProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetMemoryFdPropertiesKHR(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, int fd, VkMemoryFdPropertiesKHR* pMemoryFdProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetMemoryFdPropertiesKHR(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, int fd, VkMemoryFdPropertiesKHR* pMemoryFdProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetMemoryFdPropertiesKHR(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, int fd, VkMemoryFdPropertiesKHR* pMemoryFdProperties) ATTR_WEAK;


#ifdef VK_USE_PLATFORM_FUCHSIA
  VKAPI_ATTR VkResult VKAPI_CALL radv_GetMemoryZirconHandleFUCHSIA(VkDevice device, const VkMemoryGetZirconHandleInfoFUCHSIA* pGetZirconHandleInfo, zx_handle_t* pZirconHandle) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetMemoryZirconHandleFUCHSIA(VkDevice device, const VkMemoryGetZirconHandleInfoFUCHSIA* pGetZirconHandleInfo, zx_handle_t* pZirconHandle) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetMemoryZirconHandleFUCHSIA(VkDevice device, const VkMemoryGetZirconHandleInfoFUCHSIA* pGetZirconHandleInfo, zx_handle_t* pZirconHandle) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetMemoryZirconHandleFUCHSIA(VkDevice device, const VkMemoryGetZirconHandleInfoFUCHSIA* pGetZirconHandleInfo, zx_handle_t* pZirconHandle) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetMemoryZirconHandleFUCHSIA(VkDevice device, const VkMemoryGetZirconHandleInfoFUCHSIA* pGetZirconHandleInfo, zx_handle_t* pZirconHandle) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetMemoryZirconHandleFUCHSIA(VkDevice device, const VkMemoryGetZirconHandleInfoFUCHSIA* pGetZirconHandleInfo, zx_handle_t* pZirconHandle) ATTR_WEAK;


#endif // VK_USE_PLATFORM_FUCHSIA
#ifdef VK_USE_PLATFORM_FUCHSIA
  VKAPI_ATTR VkResult VKAPI_CALL radv_GetMemoryZirconHandlePropertiesFUCHSIA(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, zx_handle_t zirconHandle, VkMemoryZirconHandlePropertiesFUCHSIA* pMemoryZirconHandleProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetMemoryZirconHandlePropertiesFUCHSIA(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, zx_handle_t zirconHandle, VkMemoryZirconHandlePropertiesFUCHSIA* pMemoryZirconHandleProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetMemoryZirconHandlePropertiesFUCHSIA(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, zx_handle_t zirconHandle, VkMemoryZirconHandlePropertiesFUCHSIA* pMemoryZirconHandleProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetMemoryZirconHandlePropertiesFUCHSIA(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, zx_handle_t zirconHandle, VkMemoryZirconHandlePropertiesFUCHSIA* pMemoryZirconHandleProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetMemoryZirconHandlePropertiesFUCHSIA(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, zx_handle_t zirconHandle, VkMemoryZirconHandlePropertiesFUCHSIA* pMemoryZirconHandleProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetMemoryZirconHandlePropertiesFUCHSIA(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, zx_handle_t zirconHandle, VkMemoryZirconHandlePropertiesFUCHSIA* pMemoryZirconHandleProperties) ATTR_WEAK;


#endif // VK_USE_PLATFORM_FUCHSIA
  VKAPI_ATTR VkResult VKAPI_CALL radv_GetMemoryRemoteAddressNV(VkDevice device, const VkMemoryGetRemoteAddressInfoNV* pMemoryGetRemoteAddressInfo, VkRemoteAddressNV* pAddress) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetMemoryRemoteAddressNV(VkDevice device, const VkMemoryGetRemoteAddressInfoNV* pMemoryGetRemoteAddressInfo, VkRemoteAddressNV* pAddress) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetMemoryRemoteAddressNV(VkDevice device, const VkMemoryGetRemoteAddressInfoNV* pMemoryGetRemoteAddressInfo, VkRemoteAddressNV* pAddress) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetMemoryRemoteAddressNV(VkDevice device, const VkMemoryGetRemoteAddressInfoNV* pMemoryGetRemoteAddressInfo, VkRemoteAddressNV* pAddress) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetMemoryRemoteAddressNV(VkDevice device, const VkMemoryGetRemoteAddressInfoNV* pMemoryGetRemoteAddressInfo, VkRemoteAddressNV* pAddress) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetMemoryRemoteAddressNV(VkDevice device, const VkMemoryGetRemoteAddressInfoNV* pMemoryGetRemoteAddressInfo, VkRemoteAddressNV* pAddress) ATTR_WEAK;


#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL radv_GetSemaphoreWin32HandleKHR(VkDevice device, const VkSemaphoreGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetSemaphoreWin32HandleKHR(VkDevice device, const VkSemaphoreGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetSemaphoreWin32HandleKHR(VkDevice device, const VkSemaphoreGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetSemaphoreWin32HandleKHR(VkDevice device, const VkSemaphoreGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetSemaphoreWin32HandleKHR(VkDevice device, const VkSemaphoreGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetSemaphoreWin32HandleKHR(VkDevice device, const VkSemaphoreGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle) ATTR_WEAK;


#endif // VK_USE_PLATFORM_WIN32_KHR
#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL radv_ImportSemaphoreWin32HandleKHR(VkDevice device, const VkImportSemaphoreWin32HandleInfoKHR* pImportSemaphoreWin32HandleInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_ImportSemaphoreWin32HandleKHR(VkDevice device, const VkImportSemaphoreWin32HandleInfoKHR* pImportSemaphoreWin32HandleInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_ImportSemaphoreWin32HandleKHR(VkDevice device, const VkImportSemaphoreWin32HandleInfoKHR* pImportSemaphoreWin32HandleInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_ImportSemaphoreWin32HandleKHR(VkDevice device, const VkImportSemaphoreWin32HandleInfoKHR* pImportSemaphoreWin32HandleInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_ImportSemaphoreWin32HandleKHR(VkDevice device, const VkImportSemaphoreWin32HandleInfoKHR* pImportSemaphoreWin32HandleInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_ImportSemaphoreWin32HandleKHR(VkDevice device, const VkImportSemaphoreWin32HandleInfoKHR* pImportSemaphoreWin32HandleInfo) ATTR_WEAK;


#endif // VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL radv_GetSemaphoreFdKHR(VkDevice device, const VkSemaphoreGetFdInfoKHR* pGetFdInfo, int* pFd) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetSemaphoreFdKHR(VkDevice device, const VkSemaphoreGetFdInfoKHR* pGetFdInfo, int* pFd) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetSemaphoreFdKHR(VkDevice device, const VkSemaphoreGetFdInfoKHR* pGetFdInfo, int* pFd) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetSemaphoreFdKHR(VkDevice device, const VkSemaphoreGetFdInfoKHR* pGetFdInfo, int* pFd) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetSemaphoreFdKHR(VkDevice device, const VkSemaphoreGetFdInfoKHR* pGetFdInfo, int* pFd) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetSemaphoreFdKHR(VkDevice device, const VkSemaphoreGetFdInfoKHR* pGetFdInfo, int* pFd) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_ImportSemaphoreFdKHR(VkDevice device, const VkImportSemaphoreFdInfoKHR* pImportSemaphoreFdInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_ImportSemaphoreFdKHR(VkDevice device, const VkImportSemaphoreFdInfoKHR* pImportSemaphoreFdInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_ImportSemaphoreFdKHR(VkDevice device, const VkImportSemaphoreFdInfoKHR* pImportSemaphoreFdInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_ImportSemaphoreFdKHR(VkDevice device, const VkImportSemaphoreFdInfoKHR* pImportSemaphoreFdInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_ImportSemaphoreFdKHR(VkDevice device, const VkImportSemaphoreFdInfoKHR* pImportSemaphoreFdInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_ImportSemaphoreFdKHR(VkDevice device, const VkImportSemaphoreFdInfoKHR* pImportSemaphoreFdInfo) ATTR_WEAK;


#ifdef VK_USE_PLATFORM_FUCHSIA
  VKAPI_ATTR VkResult VKAPI_CALL radv_GetSemaphoreZirconHandleFUCHSIA(VkDevice device, const VkSemaphoreGetZirconHandleInfoFUCHSIA* pGetZirconHandleInfo, zx_handle_t* pZirconHandle) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetSemaphoreZirconHandleFUCHSIA(VkDevice device, const VkSemaphoreGetZirconHandleInfoFUCHSIA* pGetZirconHandleInfo, zx_handle_t* pZirconHandle) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetSemaphoreZirconHandleFUCHSIA(VkDevice device, const VkSemaphoreGetZirconHandleInfoFUCHSIA* pGetZirconHandleInfo, zx_handle_t* pZirconHandle) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetSemaphoreZirconHandleFUCHSIA(VkDevice device, const VkSemaphoreGetZirconHandleInfoFUCHSIA* pGetZirconHandleInfo, zx_handle_t* pZirconHandle) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetSemaphoreZirconHandleFUCHSIA(VkDevice device, const VkSemaphoreGetZirconHandleInfoFUCHSIA* pGetZirconHandleInfo, zx_handle_t* pZirconHandle) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetSemaphoreZirconHandleFUCHSIA(VkDevice device, const VkSemaphoreGetZirconHandleInfoFUCHSIA* pGetZirconHandleInfo, zx_handle_t* pZirconHandle) ATTR_WEAK;


#endif // VK_USE_PLATFORM_FUCHSIA
#ifdef VK_USE_PLATFORM_FUCHSIA
  VKAPI_ATTR VkResult VKAPI_CALL radv_ImportSemaphoreZirconHandleFUCHSIA(VkDevice device, const VkImportSemaphoreZirconHandleInfoFUCHSIA* pImportSemaphoreZirconHandleInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_ImportSemaphoreZirconHandleFUCHSIA(VkDevice device, const VkImportSemaphoreZirconHandleInfoFUCHSIA* pImportSemaphoreZirconHandleInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_ImportSemaphoreZirconHandleFUCHSIA(VkDevice device, const VkImportSemaphoreZirconHandleInfoFUCHSIA* pImportSemaphoreZirconHandleInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_ImportSemaphoreZirconHandleFUCHSIA(VkDevice device, const VkImportSemaphoreZirconHandleInfoFUCHSIA* pImportSemaphoreZirconHandleInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_ImportSemaphoreZirconHandleFUCHSIA(VkDevice device, const VkImportSemaphoreZirconHandleInfoFUCHSIA* pImportSemaphoreZirconHandleInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_ImportSemaphoreZirconHandleFUCHSIA(VkDevice device, const VkImportSemaphoreZirconHandleInfoFUCHSIA* pImportSemaphoreZirconHandleInfo) ATTR_WEAK;


#endif // VK_USE_PLATFORM_FUCHSIA
#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL radv_GetFenceWin32HandleKHR(VkDevice device, const VkFenceGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetFenceWin32HandleKHR(VkDevice device, const VkFenceGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetFenceWin32HandleKHR(VkDevice device, const VkFenceGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetFenceWin32HandleKHR(VkDevice device, const VkFenceGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetFenceWin32HandleKHR(VkDevice device, const VkFenceGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetFenceWin32HandleKHR(VkDevice device, const VkFenceGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle) ATTR_WEAK;


#endif // VK_USE_PLATFORM_WIN32_KHR
#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL radv_ImportFenceWin32HandleKHR(VkDevice device, const VkImportFenceWin32HandleInfoKHR* pImportFenceWin32HandleInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_ImportFenceWin32HandleKHR(VkDevice device, const VkImportFenceWin32HandleInfoKHR* pImportFenceWin32HandleInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_ImportFenceWin32HandleKHR(VkDevice device, const VkImportFenceWin32HandleInfoKHR* pImportFenceWin32HandleInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_ImportFenceWin32HandleKHR(VkDevice device, const VkImportFenceWin32HandleInfoKHR* pImportFenceWin32HandleInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_ImportFenceWin32HandleKHR(VkDevice device, const VkImportFenceWin32HandleInfoKHR* pImportFenceWin32HandleInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_ImportFenceWin32HandleKHR(VkDevice device, const VkImportFenceWin32HandleInfoKHR* pImportFenceWin32HandleInfo) ATTR_WEAK;


#endif // VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL radv_GetFenceFdKHR(VkDevice device, const VkFenceGetFdInfoKHR* pGetFdInfo, int* pFd) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetFenceFdKHR(VkDevice device, const VkFenceGetFdInfoKHR* pGetFdInfo, int* pFd) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetFenceFdKHR(VkDevice device, const VkFenceGetFdInfoKHR* pGetFdInfo, int* pFd) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetFenceFdKHR(VkDevice device, const VkFenceGetFdInfoKHR* pGetFdInfo, int* pFd) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetFenceFdKHR(VkDevice device, const VkFenceGetFdInfoKHR* pGetFdInfo, int* pFd) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetFenceFdKHR(VkDevice device, const VkFenceGetFdInfoKHR* pGetFdInfo, int* pFd) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_ImportFenceFdKHR(VkDevice device, const VkImportFenceFdInfoKHR* pImportFenceFdInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_ImportFenceFdKHR(VkDevice device, const VkImportFenceFdInfoKHR* pImportFenceFdInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_ImportFenceFdKHR(VkDevice device, const VkImportFenceFdInfoKHR* pImportFenceFdInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_ImportFenceFdKHR(VkDevice device, const VkImportFenceFdInfoKHR* pImportFenceFdInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_ImportFenceFdKHR(VkDevice device, const VkImportFenceFdInfoKHR* pImportFenceFdInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_ImportFenceFdKHR(VkDevice device, const VkImportFenceFdInfoKHR* pImportFenceFdInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_DisplayPowerControlEXT(VkDevice device, VkDisplayKHR display, const VkDisplayPowerInfoEXT* pDisplayPowerInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_DisplayPowerControlEXT(VkDevice device, VkDisplayKHR display, const VkDisplayPowerInfoEXT* pDisplayPowerInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_DisplayPowerControlEXT(VkDevice device, VkDisplayKHR display, const VkDisplayPowerInfoEXT* pDisplayPowerInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_DisplayPowerControlEXT(VkDevice device, VkDisplayKHR display, const VkDisplayPowerInfoEXT* pDisplayPowerInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_DisplayPowerControlEXT(VkDevice device, VkDisplayKHR display, const VkDisplayPowerInfoEXT* pDisplayPowerInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_DisplayPowerControlEXT(VkDevice device, VkDisplayKHR display, const VkDisplayPowerInfoEXT* pDisplayPowerInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_RegisterDeviceEventEXT(VkDevice device, const VkDeviceEventInfoEXT* pDeviceEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_RegisterDeviceEventEXT(VkDevice device, const VkDeviceEventInfoEXT* pDeviceEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_RegisterDeviceEventEXT(VkDevice device, const VkDeviceEventInfoEXT* pDeviceEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_RegisterDeviceEventEXT(VkDevice device, const VkDeviceEventInfoEXT* pDeviceEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_RegisterDeviceEventEXT(VkDevice device, const VkDeviceEventInfoEXT* pDeviceEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_RegisterDeviceEventEXT(VkDevice device, const VkDeviceEventInfoEXT* pDeviceEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_RegisterDisplayEventEXT(VkDevice device, VkDisplayKHR display, const VkDisplayEventInfoEXT* pDisplayEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_RegisterDisplayEventEXT(VkDevice device, VkDisplayKHR display, const VkDisplayEventInfoEXT* pDisplayEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_RegisterDisplayEventEXT(VkDevice device, VkDisplayKHR display, const VkDisplayEventInfoEXT* pDisplayEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_RegisterDisplayEventEXT(VkDevice device, VkDisplayKHR display, const VkDisplayEventInfoEXT* pDisplayEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_RegisterDisplayEventEXT(VkDevice device, VkDisplayKHR display, const VkDisplayEventInfoEXT* pDisplayEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_RegisterDisplayEventEXT(VkDevice device, VkDisplayKHR display, const VkDisplayEventInfoEXT* pDisplayEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_GetSwapchainCounterEXT(VkDevice device, VkSwapchainKHR swapchain, VkSurfaceCounterFlagBitsEXT counter, uint64_t* pCounterValue) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetSwapchainCounterEXT(VkDevice device, VkSwapchainKHR swapchain, VkSurfaceCounterFlagBitsEXT counter, uint64_t* pCounterValue) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetSwapchainCounterEXT(VkDevice device, VkSwapchainKHR swapchain, VkSurfaceCounterFlagBitsEXT counter, uint64_t* pCounterValue) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetSwapchainCounterEXT(VkDevice device, VkSwapchainKHR swapchain, VkSurfaceCounterFlagBitsEXT counter, uint64_t* pCounterValue) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetSwapchainCounterEXT(VkDevice device, VkSwapchainKHR swapchain, VkSurfaceCounterFlagBitsEXT counter, uint64_t* pCounterValue) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetSwapchainCounterEXT(VkDevice device, VkSwapchainKHR swapchain, VkSurfaceCounterFlagBitsEXT counter, uint64_t* pCounterValue) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_GetDeviceGroupPeerMemoryFeatures(VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_GetDeviceGroupPeerMemoryFeatures(VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_GetDeviceGroupPeerMemoryFeatures(VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_GetDeviceGroupPeerMemoryFeatures(VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_GetDeviceGroupPeerMemoryFeatures(VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_GetDeviceGroupPeerMemoryFeatures(VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_GetDeviceGroupPeerMemoryFeaturesKHR(VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_GetDeviceGroupPeerMemoryFeaturesKHR(VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_GetDeviceGroupPeerMemoryFeaturesKHR(VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_GetDeviceGroupPeerMemoryFeaturesKHR(VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_GetDeviceGroupPeerMemoryFeaturesKHR(VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_GetDeviceGroupPeerMemoryFeaturesKHR(VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_BindBufferMemory2(VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_BindBufferMemory2(VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_BindBufferMemory2(VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_BindBufferMemory2(VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_BindBufferMemory2(VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_BindBufferMemory2(VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_BindBufferMemory2KHR(VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_BindBufferMemory2KHR(VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_BindBufferMemory2KHR(VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_BindBufferMemory2KHR(VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_BindBufferMemory2KHR(VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_BindBufferMemory2KHR(VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_BindImageMemory2(VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_BindImageMemory2(VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_BindImageMemory2(VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_BindImageMemory2(VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_BindImageMemory2(VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_BindImageMemory2(VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_BindImageMemory2KHR(VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_BindImageMemory2KHR(VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_BindImageMemory2KHR(VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_BindImageMemory2KHR(VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_BindImageMemory2KHR(VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_BindImageMemory2KHR(VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetDeviceMask(VkCommandBuffer commandBuffer, uint32_t deviceMask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetDeviceMask(VkCommandBuffer commandBuffer, uint32_t deviceMask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetDeviceMask(VkCommandBuffer commandBuffer, uint32_t deviceMask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetDeviceMask(VkCommandBuffer commandBuffer, uint32_t deviceMask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetDeviceMask(VkCommandBuffer commandBuffer, uint32_t deviceMask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetDeviceMask(VkCommandBuffer commandBuffer, uint32_t deviceMask) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetDeviceMaskKHR(VkCommandBuffer commandBuffer, uint32_t deviceMask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetDeviceMaskKHR(VkCommandBuffer commandBuffer, uint32_t deviceMask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetDeviceMaskKHR(VkCommandBuffer commandBuffer, uint32_t deviceMask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetDeviceMaskKHR(VkCommandBuffer commandBuffer, uint32_t deviceMask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetDeviceMaskKHR(VkCommandBuffer commandBuffer, uint32_t deviceMask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetDeviceMaskKHR(VkCommandBuffer commandBuffer, uint32_t deviceMask) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_GetDeviceGroupPresentCapabilitiesKHR(VkDevice device, VkDeviceGroupPresentCapabilitiesKHR* pDeviceGroupPresentCapabilities) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetDeviceGroupPresentCapabilitiesKHR(VkDevice device, VkDeviceGroupPresentCapabilitiesKHR* pDeviceGroupPresentCapabilities) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetDeviceGroupPresentCapabilitiesKHR(VkDevice device, VkDeviceGroupPresentCapabilitiesKHR* pDeviceGroupPresentCapabilities) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetDeviceGroupPresentCapabilitiesKHR(VkDevice device, VkDeviceGroupPresentCapabilitiesKHR* pDeviceGroupPresentCapabilities) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetDeviceGroupPresentCapabilitiesKHR(VkDevice device, VkDeviceGroupPresentCapabilitiesKHR* pDeviceGroupPresentCapabilities) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetDeviceGroupPresentCapabilitiesKHR(VkDevice device, VkDeviceGroupPresentCapabilitiesKHR* pDeviceGroupPresentCapabilities) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_GetDeviceGroupSurfacePresentModesKHR(VkDevice device, VkSurfaceKHR surface, VkDeviceGroupPresentModeFlagsKHR* pModes) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetDeviceGroupSurfacePresentModesKHR(VkDevice device, VkSurfaceKHR surface, VkDeviceGroupPresentModeFlagsKHR* pModes) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetDeviceGroupSurfacePresentModesKHR(VkDevice device, VkSurfaceKHR surface, VkDeviceGroupPresentModeFlagsKHR* pModes) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetDeviceGroupSurfacePresentModesKHR(VkDevice device, VkSurfaceKHR surface, VkDeviceGroupPresentModeFlagsKHR* pModes) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetDeviceGroupSurfacePresentModesKHR(VkDevice device, VkSurfaceKHR surface, VkDeviceGroupPresentModeFlagsKHR* pModes) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetDeviceGroupSurfacePresentModesKHR(VkDevice device, VkSurfaceKHR surface, VkDeviceGroupPresentModeFlagsKHR* pModes) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_AcquireNextImage2KHR(VkDevice device, const VkAcquireNextImageInfoKHR* pAcquireInfo, uint32_t* pImageIndex) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_AcquireNextImage2KHR(VkDevice device, const VkAcquireNextImageInfoKHR* pAcquireInfo, uint32_t* pImageIndex) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_AcquireNextImage2KHR(VkDevice device, const VkAcquireNextImageInfoKHR* pAcquireInfo, uint32_t* pImageIndex) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_AcquireNextImage2KHR(VkDevice device, const VkAcquireNextImageInfoKHR* pAcquireInfo, uint32_t* pImageIndex) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_AcquireNextImage2KHR(VkDevice device, const VkAcquireNextImageInfoKHR* pAcquireInfo, uint32_t* pImageIndex) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_AcquireNextImage2KHR(VkDevice device, const VkAcquireNextImageInfoKHR* pAcquireInfo, uint32_t* pImageIndex) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdDispatchBase(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdDispatchBase(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdDispatchBase(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdDispatchBase(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdDispatchBase(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdDispatchBase(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdDispatchBaseKHR(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdDispatchBaseKHR(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdDispatchBaseKHR(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdDispatchBaseKHR(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdDispatchBaseKHR(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdDispatchBaseKHR(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateDescriptorUpdateTemplate(VkDevice device, const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_CreateDescriptorUpdateTemplate(VkDevice device, const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_CreateDescriptorUpdateTemplate(VkDevice device, const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_CreateDescriptorUpdateTemplate(VkDevice device, const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_CreateDescriptorUpdateTemplate(VkDevice device, const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_CreateDescriptorUpdateTemplate(VkDevice device, const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateDescriptorUpdateTemplateKHR(VkDevice device, const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_CreateDescriptorUpdateTemplateKHR(VkDevice device, const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_CreateDescriptorUpdateTemplateKHR(VkDevice device, const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_CreateDescriptorUpdateTemplateKHR(VkDevice device, const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_CreateDescriptorUpdateTemplateKHR(VkDevice device, const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_CreateDescriptorUpdateTemplateKHR(VkDevice device, const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_DestroyDescriptorUpdateTemplate(VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_DestroyDescriptorUpdateTemplate(VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_DestroyDescriptorUpdateTemplate(VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_DestroyDescriptorUpdateTemplate(VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_DestroyDescriptorUpdateTemplate(VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_DestroyDescriptorUpdateTemplate(VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_DestroyDescriptorUpdateTemplateKHR(VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_DestroyDescriptorUpdateTemplateKHR(VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_DestroyDescriptorUpdateTemplateKHR(VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_DestroyDescriptorUpdateTemplateKHR(VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_DestroyDescriptorUpdateTemplateKHR(VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_DestroyDescriptorUpdateTemplateKHR(VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_UpdateDescriptorSetWithTemplate(VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_UpdateDescriptorSetWithTemplate(VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_UpdateDescriptorSetWithTemplate(VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_UpdateDescriptorSetWithTemplate(VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_UpdateDescriptorSetWithTemplate(VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_UpdateDescriptorSetWithTemplate(VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_UpdateDescriptorSetWithTemplateKHR(VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_UpdateDescriptorSetWithTemplateKHR(VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_UpdateDescriptorSetWithTemplateKHR(VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_UpdateDescriptorSetWithTemplateKHR(VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_UpdateDescriptorSetWithTemplateKHR(VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_UpdateDescriptorSetWithTemplateKHR(VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdPushDescriptorSetWithTemplateKHR(VkCommandBuffer commandBuffer, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, const void* pData) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdPushDescriptorSetWithTemplateKHR(VkCommandBuffer commandBuffer, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, const void* pData) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdPushDescriptorSetWithTemplateKHR(VkCommandBuffer commandBuffer, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, const void* pData) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdPushDescriptorSetWithTemplateKHR(VkCommandBuffer commandBuffer, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, const void* pData) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdPushDescriptorSetWithTemplateKHR(VkCommandBuffer commandBuffer, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, const void* pData) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdPushDescriptorSetWithTemplateKHR(VkCommandBuffer commandBuffer, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, const void* pData) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_SetHdrMetadataEXT(VkDevice device, uint32_t swapchainCount, const VkSwapchainKHR* pSwapchains, const VkHdrMetadataEXT* pMetadata) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_SetHdrMetadataEXT(VkDevice device, uint32_t swapchainCount, const VkSwapchainKHR* pSwapchains, const VkHdrMetadataEXT* pMetadata) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_SetHdrMetadataEXT(VkDevice device, uint32_t swapchainCount, const VkSwapchainKHR* pSwapchains, const VkHdrMetadataEXT* pMetadata) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_SetHdrMetadataEXT(VkDevice device, uint32_t swapchainCount, const VkSwapchainKHR* pSwapchains, const VkHdrMetadataEXT* pMetadata) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_SetHdrMetadataEXT(VkDevice device, uint32_t swapchainCount, const VkSwapchainKHR* pSwapchains, const VkHdrMetadataEXT* pMetadata) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_SetHdrMetadataEXT(VkDevice device, uint32_t swapchainCount, const VkSwapchainKHR* pSwapchains, const VkHdrMetadataEXT* pMetadata) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_GetSwapchainStatusKHR(VkDevice device, VkSwapchainKHR swapchain) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetSwapchainStatusKHR(VkDevice device, VkSwapchainKHR swapchain) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetSwapchainStatusKHR(VkDevice device, VkSwapchainKHR swapchain) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetSwapchainStatusKHR(VkDevice device, VkSwapchainKHR swapchain) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetSwapchainStatusKHR(VkDevice device, VkSwapchainKHR swapchain) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetSwapchainStatusKHR(VkDevice device, VkSwapchainKHR swapchain) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_GetRefreshCycleDurationGOOGLE(VkDevice device, VkSwapchainKHR swapchain, VkRefreshCycleDurationGOOGLE* pDisplayTimingProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetRefreshCycleDurationGOOGLE(VkDevice device, VkSwapchainKHR swapchain, VkRefreshCycleDurationGOOGLE* pDisplayTimingProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetRefreshCycleDurationGOOGLE(VkDevice device, VkSwapchainKHR swapchain, VkRefreshCycleDurationGOOGLE* pDisplayTimingProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetRefreshCycleDurationGOOGLE(VkDevice device, VkSwapchainKHR swapchain, VkRefreshCycleDurationGOOGLE* pDisplayTimingProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetRefreshCycleDurationGOOGLE(VkDevice device, VkSwapchainKHR swapchain, VkRefreshCycleDurationGOOGLE* pDisplayTimingProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetRefreshCycleDurationGOOGLE(VkDevice device, VkSwapchainKHR swapchain, VkRefreshCycleDurationGOOGLE* pDisplayTimingProperties) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_GetPastPresentationTimingGOOGLE(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pPresentationTimingCount, VkPastPresentationTimingGOOGLE* pPresentationTimings) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetPastPresentationTimingGOOGLE(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pPresentationTimingCount, VkPastPresentationTimingGOOGLE* pPresentationTimings) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetPastPresentationTimingGOOGLE(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pPresentationTimingCount, VkPastPresentationTimingGOOGLE* pPresentationTimings) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetPastPresentationTimingGOOGLE(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pPresentationTimingCount, VkPastPresentationTimingGOOGLE* pPresentationTimings) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetPastPresentationTimingGOOGLE(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pPresentationTimingCount, VkPastPresentationTimingGOOGLE* pPresentationTimings) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetPastPresentationTimingGOOGLE(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pPresentationTimingCount, VkPastPresentationTimingGOOGLE* pPresentationTimings) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetViewportWScalingNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewportWScalingNV* pViewportWScalings) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetViewportWScalingNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewportWScalingNV* pViewportWScalings) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetViewportWScalingNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewportWScalingNV* pViewportWScalings) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetViewportWScalingNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewportWScalingNV* pViewportWScalings) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetViewportWScalingNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewportWScalingNV* pViewportWScalings) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetViewportWScalingNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewportWScalingNV* pViewportWScalings) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetDiscardRectangleEXT(VkCommandBuffer commandBuffer, uint32_t firstDiscardRectangle, uint32_t discardRectangleCount, const VkRect2D* pDiscardRectangles) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetDiscardRectangleEXT(VkCommandBuffer commandBuffer, uint32_t firstDiscardRectangle, uint32_t discardRectangleCount, const VkRect2D* pDiscardRectangles) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetDiscardRectangleEXT(VkCommandBuffer commandBuffer, uint32_t firstDiscardRectangle, uint32_t discardRectangleCount, const VkRect2D* pDiscardRectangles) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetDiscardRectangleEXT(VkCommandBuffer commandBuffer, uint32_t firstDiscardRectangle, uint32_t discardRectangleCount, const VkRect2D* pDiscardRectangles) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetDiscardRectangleEXT(VkCommandBuffer commandBuffer, uint32_t firstDiscardRectangle, uint32_t discardRectangleCount, const VkRect2D* pDiscardRectangles) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetDiscardRectangleEXT(VkCommandBuffer commandBuffer, uint32_t firstDiscardRectangle, uint32_t discardRectangleCount, const VkRect2D* pDiscardRectangles) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetDiscardRectangleEnableEXT(VkCommandBuffer commandBuffer, VkBool32 discardRectangleEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetDiscardRectangleEnableEXT(VkCommandBuffer commandBuffer, VkBool32 discardRectangleEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetDiscardRectangleEnableEXT(VkCommandBuffer commandBuffer, VkBool32 discardRectangleEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetDiscardRectangleEnableEXT(VkCommandBuffer commandBuffer, VkBool32 discardRectangleEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetDiscardRectangleEnableEXT(VkCommandBuffer commandBuffer, VkBool32 discardRectangleEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetDiscardRectangleEnableEXT(VkCommandBuffer commandBuffer, VkBool32 discardRectangleEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetDiscardRectangleModeEXT(VkCommandBuffer commandBuffer, VkDiscardRectangleModeEXT discardRectangleMode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetDiscardRectangleModeEXT(VkCommandBuffer commandBuffer, VkDiscardRectangleModeEXT discardRectangleMode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetDiscardRectangleModeEXT(VkCommandBuffer commandBuffer, VkDiscardRectangleModeEXT discardRectangleMode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetDiscardRectangleModeEXT(VkCommandBuffer commandBuffer, VkDiscardRectangleModeEXT discardRectangleMode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetDiscardRectangleModeEXT(VkCommandBuffer commandBuffer, VkDiscardRectangleModeEXT discardRectangleMode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetDiscardRectangleModeEXT(VkCommandBuffer commandBuffer, VkDiscardRectangleModeEXT discardRectangleMode) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetSampleLocationsEXT(VkCommandBuffer commandBuffer, const VkSampleLocationsInfoEXT* pSampleLocationsInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetSampleLocationsEXT(VkCommandBuffer commandBuffer, const VkSampleLocationsInfoEXT* pSampleLocationsInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetSampleLocationsEXT(VkCommandBuffer commandBuffer, const VkSampleLocationsInfoEXT* pSampleLocationsInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetSampleLocationsEXT(VkCommandBuffer commandBuffer, const VkSampleLocationsInfoEXT* pSampleLocationsInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetSampleLocationsEXT(VkCommandBuffer commandBuffer, const VkSampleLocationsInfoEXT* pSampleLocationsInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetSampleLocationsEXT(VkCommandBuffer commandBuffer, const VkSampleLocationsInfoEXT* pSampleLocationsInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_GetBufferMemoryRequirements2(VkDevice device, const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_GetBufferMemoryRequirements2(VkDevice device, const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_GetBufferMemoryRequirements2(VkDevice device, const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_GetBufferMemoryRequirements2(VkDevice device, const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_GetBufferMemoryRequirements2(VkDevice device, const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_GetBufferMemoryRequirements2(VkDevice device, const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_GetBufferMemoryRequirements2KHR(VkDevice device, const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_GetBufferMemoryRequirements2KHR(VkDevice device, const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_GetBufferMemoryRequirements2KHR(VkDevice device, const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_GetBufferMemoryRequirements2KHR(VkDevice device, const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_GetBufferMemoryRequirements2KHR(VkDevice device, const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_GetBufferMemoryRequirements2KHR(VkDevice device, const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_GetImageMemoryRequirements2(VkDevice device, const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_GetImageMemoryRequirements2(VkDevice device, const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_GetImageMemoryRequirements2(VkDevice device, const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_GetImageMemoryRequirements2(VkDevice device, const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_GetImageMemoryRequirements2(VkDevice device, const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_GetImageMemoryRequirements2(VkDevice device, const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_GetImageMemoryRequirements2KHR(VkDevice device, const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_GetImageMemoryRequirements2KHR(VkDevice device, const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_GetImageMemoryRequirements2KHR(VkDevice device, const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_GetImageMemoryRequirements2KHR(VkDevice device, const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_GetImageMemoryRequirements2KHR(VkDevice device, const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_GetImageMemoryRequirements2KHR(VkDevice device, const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_GetImageSparseMemoryRequirements2(VkDevice device, const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_GetImageSparseMemoryRequirements2(VkDevice device, const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_GetImageSparseMemoryRequirements2(VkDevice device, const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_GetImageSparseMemoryRequirements2(VkDevice device, const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_GetImageSparseMemoryRequirements2(VkDevice device, const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_GetImageSparseMemoryRequirements2(VkDevice device, const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_GetImageSparseMemoryRequirements2KHR(VkDevice device, const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_GetImageSparseMemoryRequirements2KHR(VkDevice device, const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_GetImageSparseMemoryRequirements2KHR(VkDevice device, const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_GetImageSparseMemoryRequirements2KHR(VkDevice device, const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_GetImageSparseMemoryRequirements2KHR(VkDevice device, const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_GetImageSparseMemoryRequirements2KHR(VkDevice device, const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_GetDeviceBufferMemoryRequirements(VkDevice device, const VkDeviceBufferMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_GetDeviceBufferMemoryRequirements(VkDevice device, const VkDeviceBufferMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_GetDeviceBufferMemoryRequirements(VkDevice device, const VkDeviceBufferMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_GetDeviceBufferMemoryRequirements(VkDevice device, const VkDeviceBufferMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_GetDeviceBufferMemoryRequirements(VkDevice device, const VkDeviceBufferMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_GetDeviceBufferMemoryRequirements(VkDevice device, const VkDeviceBufferMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_GetDeviceBufferMemoryRequirementsKHR(VkDevice device, const VkDeviceBufferMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_GetDeviceBufferMemoryRequirementsKHR(VkDevice device, const VkDeviceBufferMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_GetDeviceBufferMemoryRequirementsKHR(VkDevice device, const VkDeviceBufferMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_GetDeviceBufferMemoryRequirementsKHR(VkDevice device, const VkDeviceBufferMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_GetDeviceBufferMemoryRequirementsKHR(VkDevice device, const VkDeviceBufferMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_GetDeviceBufferMemoryRequirementsKHR(VkDevice device, const VkDeviceBufferMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_GetDeviceImageMemoryRequirements(VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_GetDeviceImageMemoryRequirements(VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_GetDeviceImageMemoryRequirements(VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_GetDeviceImageMemoryRequirements(VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_GetDeviceImageMemoryRequirements(VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_GetDeviceImageMemoryRequirements(VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_GetDeviceImageMemoryRequirementsKHR(VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_GetDeviceImageMemoryRequirementsKHR(VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_GetDeviceImageMemoryRequirementsKHR(VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_GetDeviceImageMemoryRequirementsKHR(VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_GetDeviceImageMemoryRequirementsKHR(VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_GetDeviceImageMemoryRequirementsKHR(VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_GetDeviceImageSparseMemoryRequirements(VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_GetDeviceImageSparseMemoryRequirements(VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_GetDeviceImageSparseMemoryRequirements(VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_GetDeviceImageSparseMemoryRequirements(VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_GetDeviceImageSparseMemoryRequirements(VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_GetDeviceImageSparseMemoryRequirements(VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_GetDeviceImageSparseMemoryRequirementsKHR(VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_GetDeviceImageSparseMemoryRequirementsKHR(VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_GetDeviceImageSparseMemoryRequirementsKHR(VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_GetDeviceImageSparseMemoryRequirementsKHR(VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_GetDeviceImageSparseMemoryRequirementsKHR(VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_GetDeviceImageSparseMemoryRequirementsKHR(VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateSamplerYcbcrConversion(VkDevice device, const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_CreateSamplerYcbcrConversion(VkDevice device, const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_CreateSamplerYcbcrConversion(VkDevice device, const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_CreateSamplerYcbcrConversion(VkDevice device, const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_CreateSamplerYcbcrConversion(VkDevice device, const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_CreateSamplerYcbcrConversion(VkDevice device, const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateSamplerYcbcrConversionKHR(VkDevice device, const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_CreateSamplerYcbcrConversionKHR(VkDevice device, const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_CreateSamplerYcbcrConversionKHR(VkDevice device, const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_CreateSamplerYcbcrConversionKHR(VkDevice device, const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_CreateSamplerYcbcrConversionKHR(VkDevice device, const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_CreateSamplerYcbcrConversionKHR(VkDevice device, const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_DestroySamplerYcbcrConversion(VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_DestroySamplerYcbcrConversion(VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_DestroySamplerYcbcrConversion(VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_DestroySamplerYcbcrConversion(VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_DestroySamplerYcbcrConversion(VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_DestroySamplerYcbcrConversion(VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_DestroySamplerYcbcrConversionKHR(VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_DestroySamplerYcbcrConversionKHR(VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_DestroySamplerYcbcrConversionKHR(VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_DestroySamplerYcbcrConversionKHR(VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_DestroySamplerYcbcrConversionKHR(VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_DestroySamplerYcbcrConversionKHR(VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_GetDeviceQueue2(VkDevice device, const VkDeviceQueueInfo2* pQueueInfo, VkQueue* pQueue) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_GetDeviceQueue2(VkDevice device, const VkDeviceQueueInfo2* pQueueInfo, VkQueue* pQueue) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_GetDeviceQueue2(VkDevice device, const VkDeviceQueueInfo2* pQueueInfo, VkQueue* pQueue) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_GetDeviceQueue2(VkDevice device, const VkDeviceQueueInfo2* pQueueInfo, VkQueue* pQueue) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_GetDeviceQueue2(VkDevice device, const VkDeviceQueueInfo2* pQueueInfo, VkQueue* pQueue) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_GetDeviceQueue2(VkDevice device, const VkDeviceQueueInfo2* pQueueInfo, VkQueue* pQueue) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateValidationCacheEXT(VkDevice device, const VkValidationCacheCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkValidationCacheEXT* pValidationCache) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_CreateValidationCacheEXT(VkDevice device, const VkValidationCacheCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkValidationCacheEXT* pValidationCache) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_CreateValidationCacheEXT(VkDevice device, const VkValidationCacheCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkValidationCacheEXT* pValidationCache) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_CreateValidationCacheEXT(VkDevice device, const VkValidationCacheCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkValidationCacheEXT* pValidationCache) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_CreateValidationCacheEXT(VkDevice device, const VkValidationCacheCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkValidationCacheEXT* pValidationCache) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_CreateValidationCacheEXT(VkDevice device, const VkValidationCacheCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkValidationCacheEXT* pValidationCache) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_DestroyValidationCacheEXT(VkDevice device, VkValidationCacheEXT validationCache, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_DestroyValidationCacheEXT(VkDevice device, VkValidationCacheEXT validationCache, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_DestroyValidationCacheEXT(VkDevice device, VkValidationCacheEXT validationCache, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_DestroyValidationCacheEXT(VkDevice device, VkValidationCacheEXT validationCache, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_DestroyValidationCacheEXT(VkDevice device, VkValidationCacheEXT validationCache, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_DestroyValidationCacheEXT(VkDevice device, VkValidationCacheEXT validationCache, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_GetValidationCacheDataEXT(VkDevice device, VkValidationCacheEXT validationCache, size_t* pDataSize, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetValidationCacheDataEXT(VkDevice device, VkValidationCacheEXT validationCache, size_t* pDataSize, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetValidationCacheDataEXT(VkDevice device, VkValidationCacheEXT validationCache, size_t* pDataSize, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetValidationCacheDataEXT(VkDevice device, VkValidationCacheEXT validationCache, size_t* pDataSize, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetValidationCacheDataEXT(VkDevice device, VkValidationCacheEXT validationCache, size_t* pDataSize, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetValidationCacheDataEXT(VkDevice device, VkValidationCacheEXT validationCache, size_t* pDataSize, void* pData) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_MergeValidationCachesEXT(VkDevice device, VkValidationCacheEXT dstCache, uint32_t srcCacheCount, const VkValidationCacheEXT* pSrcCaches) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_MergeValidationCachesEXT(VkDevice device, VkValidationCacheEXT dstCache, uint32_t srcCacheCount, const VkValidationCacheEXT* pSrcCaches) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_MergeValidationCachesEXT(VkDevice device, VkValidationCacheEXT dstCache, uint32_t srcCacheCount, const VkValidationCacheEXT* pSrcCaches) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_MergeValidationCachesEXT(VkDevice device, VkValidationCacheEXT dstCache, uint32_t srcCacheCount, const VkValidationCacheEXT* pSrcCaches) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_MergeValidationCachesEXT(VkDevice device, VkValidationCacheEXT dstCache, uint32_t srcCacheCount, const VkValidationCacheEXT* pSrcCaches) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_MergeValidationCachesEXT(VkDevice device, VkValidationCacheEXT dstCache, uint32_t srcCacheCount, const VkValidationCacheEXT* pSrcCaches) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_GetDescriptorSetLayoutSupport(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_GetDescriptorSetLayoutSupport(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_GetDescriptorSetLayoutSupport(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_GetDescriptorSetLayoutSupport(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_GetDescriptorSetLayoutSupport(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_GetDescriptorSetLayoutSupport(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_GetDescriptorSetLayoutSupportKHR(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_GetDescriptorSetLayoutSupportKHR(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_GetDescriptorSetLayoutSupportKHR(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_GetDescriptorSetLayoutSupportKHR(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_GetDescriptorSetLayoutSupportKHR(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_GetDescriptorSetLayoutSupportKHR(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport) ATTR_WEAK;


#ifdef VK_USE_PLATFORM_ANDROID_KHR
  VKAPI_ATTR VkResult VKAPI_CALL radv_GetSwapchainGrallocUsageANDROID(VkDevice device, VkFormat format, VkImageUsageFlags imageUsage, int* grallocUsage) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetSwapchainGrallocUsageANDROID(VkDevice device, VkFormat format, VkImageUsageFlags imageUsage, int* grallocUsage) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetSwapchainGrallocUsageANDROID(VkDevice device, VkFormat format, VkImageUsageFlags imageUsage, int* grallocUsage) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetSwapchainGrallocUsageANDROID(VkDevice device, VkFormat format, VkImageUsageFlags imageUsage, int* grallocUsage) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetSwapchainGrallocUsageANDROID(VkDevice device, VkFormat format, VkImageUsageFlags imageUsage, int* grallocUsage) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetSwapchainGrallocUsageANDROID(VkDevice device, VkFormat format, VkImageUsageFlags imageUsage, int* grallocUsage) ATTR_WEAK;


#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
  VKAPI_ATTR VkResult VKAPI_CALL radv_GetSwapchainGrallocUsage2ANDROID(VkDevice device, VkFormat format, VkImageUsageFlags imageUsage, VkSwapchainImageUsageFlagsANDROID swapchainImageUsage, uint64_t* grallocConsumerUsage, uint64_t* grallocProducerUsage) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetSwapchainGrallocUsage2ANDROID(VkDevice device, VkFormat format, VkImageUsageFlags imageUsage, VkSwapchainImageUsageFlagsANDROID swapchainImageUsage, uint64_t* grallocConsumerUsage, uint64_t* grallocProducerUsage) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetSwapchainGrallocUsage2ANDROID(VkDevice device, VkFormat format, VkImageUsageFlags imageUsage, VkSwapchainImageUsageFlagsANDROID swapchainImageUsage, uint64_t* grallocConsumerUsage, uint64_t* grallocProducerUsage) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetSwapchainGrallocUsage2ANDROID(VkDevice device, VkFormat format, VkImageUsageFlags imageUsage, VkSwapchainImageUsageFlagsANDROID swapchainImageUsage, uint64_t* grallocConsumerUsage, uint64_t* grallocProducerUsage) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetSwapchainGrallocUsage2ANDROID(VkDevice device, VkFormat format, VkImageUsageFlags imageUsage, VkSwapchainImageUsageFlagsANDROID swapchainImageUsage, uint64_t* grallocConsumerUsage, uint64_t* grallocProducerUsage) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetSwapchainGrallocUsage2ANDROID(VkDevice device, VkFormat format, VkImageUsageFlags imageUsage, VkSwapchainImageUsageFlagsANDROID swapchainImageUsage, uint64_t* grallocConsumerUsage, uint64_t* grallocProducerUsage) ATTR_WEAK;


#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
  VKAPI_ATTR VkResult VKAPI_CALL radv_AcquireImageANDROID(VkDevice device, VkImage image, int nativeFenceFd, VkSemaphore semaphore, VkFence fence) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_AcquireImageANDROID(VkDevice device, VkImage image, int nativeFenceFd, VkSemaphore semaphore, VkFence fence) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_AcquireImageANDROID(VkDevice device, VkImage image, int nativeFenceFd, VkSemaphore semaphore, VkFence fence) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_AcquireImageANDROID(VkDevice device, VkImage image, int nativeFenceFd, VkSemaphore semaphore, VkFence fence) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_AcquireImageANDROID(VkDevice device, VkImage image, int nativeFenceFd, VkSemaphore semaphore, VkFence fence) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_AcquireImageANDROID(VkDevice device, VkImage image, int nativeFenceFd, VkSemaphore semaphore, VkFence fence) ATTR_WEAK;


#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
  VKAPI_ATTR VkResult VKAPI_CALL radv_QueueSignalReleaseImageANDROID(VkQueue queue, uint32_t waitSemaphoreCount, const VkSemaphore* pWaitSemaphores, VkImage image, int* pNativeFenceFd) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_QueueSignalReleaseImageANDROID(VkQueue queue, uint32_t waitSemaphoreCount, const VkSemaphore* pWaitSemaphores, VkImage image, int* pNativeFenceFd) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_QueueSignalReleaseImageANDROID(VkQueue queue, uint32_t waitSemaphoreCount, const VkSemaphore* pWaitSemaphores, VkImage image, int* pNativeFenceFd) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_QueueSignalReleaseImageANDROID(VkQueue queue, uint32_t waitSemaphoreCount, const VkSemaphore* pWaitSemaphores, VkImage image, int* pNativeFenceFd) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_QueueSignalReleaseImageANDROID(VkQueue queue, uint32_t waitSemaphoreCount, const VkSemaphore* pWaitSemaphores, VkImage image, int* pNativeFenceFd) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_QueueSignalReleaseImageANDROID(VkQueue queue, uint32_t waitSemaphoreCount, const VkSemaphore* pWaitSemaphores, VkImage image, int* pNativeFenceFd) ATTR_WEAK;


#endif // VK_USE_PLATFORM_ANDROID_KHR
  VKAPI_ATTR VkResult VKAPI_CALL radv_GetShaderInfoAMD(VkDevice device, VkPipeline pipeline, VkShaderStageFlagBits shaderStage, VkShaderInfoTypeAMD infoType, size_t* pInfoSize, void* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetShaderInfoAMD(VkDevice device, VkPipeline pipeline, VkShaderStageFlagBits shaderStage, VkShaderInfoTypeAMD infoType, size_t* pInfoSize, void* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetShaderInfoAMD(VkDevice device, VkPipeline pipeline, VkShaderStageFlagBits shaderStage, VkShaderInfoTypeAMD infoType, size_t* pInfoSize, void* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetShaderInfoAMD(VkDevice device, VkPipeline pipeline, VkShaderStageFlagBits shaderStage, VkShaderInfoTypeAMD infoType, size_t* pInfoSize, void* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetShaderInfoAMD(VkDevice device, VkPipeline pipeline, VkShaderStageFlagBits shaderStage, VkShaderInfoTypeAMD infoType, size_t* pInfoSize, void* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetShaderInfoAMD(VkDevice device, VkPipeline pipeline, VkShaderStageFlagBits shaderStage, VkShaderInfoTypeAMD infoType, size_t* pInfoSize, void* pInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_SetLocalDimmingAMD(VkDevice device, VkSwapchainKHR swapChain, VkBool32 localDimmingEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_SetLocalDimmingAMD(VkDevice device, VkSwapchainKHR swapChain, VkBool32 localDimmingEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_SetLocalDimmingAMD(VkDevice device, VkSwapchainKHR swapChain, VkBool32 localDimmingEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_SetLocalDimmingAMD(VkDevice device, VkSwapchainKHR swapChain, VkBool32 localDimmingEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_SetLocalDimmingAMD(VkDevice device, VkSwapchainKHR swapChain, VkBool32 localDimmingEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_SetLocalDimmingAMD(VkDevice device, VkSwapchainKHR swapChain, VkBool32 localDimmingEnable) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_GetCalibratedTimestampsEXT(VkDevice device, uint32_t timestampCount, const VkCalibratedTimestampInfoEXT* pTimestampInfos, uint64_t* pTimestamps, uint64_t* pMaxDeviation) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetCalibratedTimestampsEXT(VkDevice device, uint32_t timestampCount, const VkCalibratedTimestampInfoEXT* pTimestampInfos, uint64_t* pTimestamps, uint64_t* pMaxDeviation) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetCalibratedTimestampsEXT(VkDevice device, uint32_t timestampCount, const VkCalibratedTimestampInfoEXT* pTimestampInfos, uint64_t* pTimestamps, uint64_t* pMaxDeviation) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetCalibratedTimestampsEXT(VkDevice device, uint32_t timestampCount, const VkCalibratedTimestampInfoEXT* pTimestampInfos, uint64_t* pTimestamps, uint64_t* pMaxDeviation) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetCalibratedTimestampsEXT(VkDevice device, uint32_t timestampCount, const VkCalibratedTimestampInfoEXT* pTimestampInfos, uint64_t* pTimestamps, uint64_t* pMaxDeviation) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetCalibratedTimestampsEXT(VkDevice device, uint32_t timestampCount, const VkCalibratedTimestampInfoEXT* pTimestampInfos, uint64_t* pTimestamps, uint64_t* pMaxDeviation) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_SetDebugUtilsObjectNameEXT(VkDevice device, const VkDebugUtilsObjectNameInfoEXT* pNameInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_SetDebugUtilsObjectNameEXT(VkDevice device, const VkDebugUtilsObjectNameInfoEXT* pNameInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_SetDebugUtilsObjectNameEXT(VkDevice device, const VkDebugUtilsObjectNameInfoEXT* pNameInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_SetDebugUtilsObjectNameEXT(VkDevice device, const VkDebugUtilsObjectNameInfoEXT* pNameInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_SetDebugUtilsObjectNameEXT(VkDevice device, const VkDebugUtilsObjectNameInfoEXT* pNameInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_SetDebugUtilsObjectNameEXT(VkDevice device, const VkDebugUtilsObjectNameInfoEXT* pNameInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_SetDebugUtilsObjectTagEXT(VkDevice device, const VkDebugUtilsObjectTagInfoEXT* pTagInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_SetDebugUtilsObjectTagEXT(VkDevice device, const VkDebugUtilsObjectTagInfoEXT* pTagInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_SetDebugUtilsObjectTagEXT(VkDevice device, const VkDebugUtilsObjectTagInfoEXT* pTagInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_SetDebugUtilsObjectTagEXT(VkDevice device, const VkDebugUtilsObjectTagInfoEXT* pTagInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_SetDebugUtilsObjectTagEXT(VkDevice device, const VkDebugUtilsObjectTagInfoEXT* pTagInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_SetDebugUtilsObjectTagEXT(VkDevice device, const VkDebugUtilsObjectTagInfoEXT* pTagInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_QueueBeginDebugUtilsLabelEXT(VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_QueueBeginDebugUtilsLabelEXT(VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_QueueBeginDebugUtilsLabelEXT(VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_QueueBeginDebugUtilsLabelEXT(VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_QueueBeginDebugUtilsLabelEXT(VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_QueueBeginDebugUtilsLabelEXT(VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_QueueEndDebugUtilsLabelEXT(VkQueue queue) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_QueueEndDebugUtilsLabelEXT(VkQueue queue) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_QueueEndDebugUtilsLabelEXT(VkQueue queue) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_QueueEndDebugUtilsLabelEXT(VkQueue queue) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_QueueEndDebugUtilsLabelEXT(VkQueue queue) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_QueueEndDebugUtilsLabelEXT(VkQueue queue) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_QueueInsertDebugUtilsLabelEXT(VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_QueueInsertDebugUtilsLabelEXT(VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_QueueInsertDebugUtilsLabelEXT(VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_QueueInsertDebugUtilsLabelEXT(VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_QueueInsertDebugUtilsLabelEXT(VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_QueueInsertDebugUtilsLabelEXT(VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdBeginDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdBeginDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdBeginDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdBeginDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdBeginDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdBeginDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdEndDebugUtilsLabelEXT(VkCommandBuffer commandBuffer) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdEndDebugUtilsLabelEXT(VkCommandBuffer commandBuffer) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdEndDebugUtilsLabelEXT(VkCommandBuffer commandBuffer) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdEndDebugUtilsLabelEXT(VkCommandBuffer commandBuffer) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdEndDebugUtilsLabelEXT(VkCommandBuffer commandBuffer) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdEndDebugUtilsLabelEXT(VkCommandBuffer commandBuffer) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdInsertDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdInsertDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdInsertDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdInsertDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdInsertDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdInsertDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_GetMemoryHostPointerPropertiesEXT(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, const void* pHostPointer, VkMemoryHostPointerPropertiesEXT* pMemoryHostPointerProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetMemoryHostPointerPropertiesEXT(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, const void* pHostPointer, VkMemoryHostPointerPropertiesEXT* pMemoryHostPointerProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetMemoryHostPointerPropertiesEXT(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, const void* pHostPointer, VkMemoryHostPointerPropertiesEXT* pMemoryHostPointerProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetMemoryHostPointerPropertiesEXT(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, const void* pHostPointer, VkMemoryHostPointerPropertiesEXT* pMemoryHostPointerProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetMemoryHostPointerPropertiesEXT(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, const void* pHostPointer, VkMemoryHostPointerPropertiesEXT* pMemoryHostPointerProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetMemoryHostPointerPropertiesEXT(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, const void* pHostPointer, VkMemoryHostPointerPropertiesEXT* pMemoryHostPointerProperties) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdWriteBufferMarkerAMD(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdWriteBufferMarkerAMD(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdWriteBufferMarkerAMD(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdWriteBufferMarkerAMD(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdWriteBufferMarkerAMD(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdWriteBufferMarkerAMD(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateRenderPass2(VkDevice device, const VkRenderPassCreateInfo2* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_CreateRenderPass2(VkDevice device, const VkRenderPassCreateInfo2* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_CreateRenderPass2(VkDevice device, const VkRenderPassCreateInfo2* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_CreateRenderPass2(VkDevice device, const VkRenderPassCreateInfo2* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_CreateRenderPass2(VkDevice device, const VkRenderPassCreateInfo2* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_CreateRenderPass2(VkDevice device, const VkRenderPassCreateInfo2* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateRenderPass2KHR(VkDevice device, const VkRenderPassCreateInfo2* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_CreateRenderPass2KHR(VkDevice device, const VkRenderPassCreateInfo2* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_CreateRenderPass2KHR(VkDevice device, const VkRenderPassCreateInfo2* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_CreateRenderPass2KHR(VkDevice device, const VkRenderPassCreateInfo2* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_CreateRenderPass2KHR(VkDevice device, const VkRenderPassCreateInfo2* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_CreateRenderPass2KHR(VkDevice device, const VkRenderPassCreateInfo2* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdBeginRenderPass2(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo*      pRenderPassBegin, const VkSubpassBeginInfo*      pSubpassBeginInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdBeginRenderPass2(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo*      pRenderPassBegin, const VkSubpassBeginInfo*      pSubpassBeginInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdBeginRenderPass2(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo*      pRenderPassBegin, const VkSubpassBeginInfo*      pSubpassBeginInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdBeginRenderPass2(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo*      pRenderPassBegin, const VkSubpassBeginInfo*      pSubpassBeginInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdBeginRenderPass2(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo*      pRenderPassBegin, const VkSubpassBeginInfo*      pSubpassBeginInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdBeginRenderPass2(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo*      pRenderPassBegin, const VkSubpassBeginInfo*      pSubpassBeginInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdBeginRenderPass2KHR(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo*      pRenderPassBegin, const VkSubpassBeginInfo*      pSubpassBeginInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdBeginRenderPass2KHR(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo*      pRenderPassBegin, const VkSubpassBeginInfo*      pSubpassBeginInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdBeginRenderPass2KHR(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo*      pRenderPassBegin, const VkSubpassBeginInfo*      pSubpassBeginInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdBeginRenderPass2KHR(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo*      pRenderPassBegin, const VkSubpassBeginInfo*      pSubpassBeginInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdBeginRenderPass2KHR(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo*      pRenderPassBegin, const VkSubpassBeginInfo*      pSubpassBeginInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdBeginRenderPass2KHR(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo*      pRenderPassBegin, const VkSubpassBeginInfo*      pSubpassBeginInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdNextSubpass2(VkCommandBuffer commandBuffer, const VkSubpassBeginInfo*      pSubpassBeginInfo, const VkSubpassEndInfo*        pSubpassEndInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdNextSubpass2(VkCommandBuffer commandBuffer, const VkSubpassBeginInfo*      pSubpassBeginInfo, const VkSubpassEndInfo*        pSubpassEndInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdNextSubpass2(VkCommandBuffer commandBuffer, const VkSubpassBeginInfo*      pSubpassBeginInfo, const VkSubpassEndInfo*        pSubpassEndInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdNextSubpass2(VkCommandBuffer commandBuffer, const VkSubpassBeginInfo*      pSubpassBeginInfo, const VkSubpassEndInfo*        pSubpassEndInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdNextSubpass2(VkCommandBuffer commandBuffer, const VkSubpassBeginInfo*      pSubpassBeginInfo, const VkSubpassEndInfo*        pSubpassEndInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdNextSubpass2(VkCommandBuffer commandBuffer, const VkSubpassBeginInfo*      pSubpassBeginInfo, const VkSubpassEndInfo*        pSubpassEndInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdNextSubpass2KHR(VkCommandBuffer commandBuffer, const VkSubpassBeginInfo*      pSubpassBeginInfo, const VkSubpassEndInfo*        pSubpassEndInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdNextSubpass2KHR(VkCommandBuffer commandBuffer, const VkSubpassBeginInfo*      pSubpassBeginInfo, const VkSubpassEndInfo*        pSubpassEndInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdNextSubpass2KHR(VkCommandBuffer commandBuffer, const VkSubpassBeginInfo*      pSubpassBeginInfo, const VkSubpassEndInfo*        pSubpassEndInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdNextSubpass2KHR(VkCommandBuffer commandBuffer, const VkSubpassBeginInfo*      pSubpassBeginInfo, const VkSubpassEndInfo*        pSubpassEndInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdNextSubpass2KHR(VkCommandBuffer commandBuffer, const VkSubpassBeginInfo*      pSubpassBeginInfo, const VkSubpassEndInfo*        pSubpassEndInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdNextSubpass2KHR(VkCommandBuffer commandBuffer, const VkSubpassBeginInfo*      pSubpassBeginInfo, const VkSubpassEndInfo*        pSubpassEndInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdEndRenderPass2(VkCommandBuffer commandBuffer, const VkSubpassEndInfo*        pSubpassEndInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdEndRenderPass2(VkCommandBuffer commandBuffer, const VkSubpassEndInfo*        pSubpassEndInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdEndRenderPass2(VkCommandBuffer commandBuffer, const VkSubpassEndInfo*        pSubpassEndInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdEndRenderPass2(VkCommandBuffer commandBuffer, const VkSubpassEndInfo*        pSubpassEndInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdEndRenderPass2(VkCommandBuffer commandBuffer, const VkSubpassEndInfo*        pSubpassEndInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdEndRenderPass2(VkCommandBuffer commandBuffer, const VkSubpassEndInfo*        pSubpassEndInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdEndRenderPass2KHR(VkCommandBuffer commandBuffer, const VkSubpassEndInfo*        pSubpassEndInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdEndRenderPass2KHR(VkCommandBuffer commandBuffer, const VkSubpassEndInfo*        pSubpassEndInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdEndRenderPass2KHR(VkCommandBuffer commandBuffer, const VkSubpassEndInfo*        pSubpassEndInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdEndRenderPass2KHR(VkCommandBuffer commandBuffer, const VkSubpassEndInfo*        pSubpassEndInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdEndRenderPass2KHR(VkCommandBuffer commandBuffer, const VkSubpassEndInfo*        pSubpassEndInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdEndRenderPass2KHR(VkCommandBuffer commandBuffer, const VkSubpassEndInfo*        pSubpassEndInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_GetSemaphoreCounterValue(VkDevice device, VkSemaphore semaphore, uint64_t* pValue) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetSemaphoreCounterValue(VkDevice device, VkSemaphore semaphore, uint64_t* pValue) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetSemaphoreCounterValue(VkDevice device, VkSemaphore semaphore, uint64_t* pValue) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetSemaphoreCounterValue(VkDevice device, VkSemaphore semaphore, uint64_t* pValue) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetSemaphoreCounterValue(VkDevice device, VkSemaphore semaphore, uint64_t* pValue) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetSemaphoreCounterValue(VkDevice device, VkSemaphore semaphore, uint64_t* pValue) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_GetSemaphoreCounterValueKHR(VkDevice device, VkSemaphore semaphore, uint64_t* pValue) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetSemaphoreCounterValueKHR(VkDevice device, VkSemaphore semaphore, uint64_t* pValue) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetSemaphoreCounterValueKHR(VkDevice device, VkSemaphore semaphore, uint64_t* pValue) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetSemaphoreCounterValueKHR(VkDevice device, VkSemaphore semaphore, uint64_t* pValue) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetSemaphoreCounterValueKHR(VkDevice device, VkSemaphore semaphore, uint64_t* pValue) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetSemaphoreCounterValueKHR(VkDevice device, VkSemaphore semaphore, uint64_t* pValue) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_WaitSemaphores(VkDevice device, const VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_WaitSemaphores(VkDevice device, const VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_WaitSemaphores(VkDevice device, const VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_WaitSemaphores(VkDevice device, const VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_WaitSemaphores(VkDevice device, const VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_WaitSemaphores(VkDevice device, const VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_WaitSemaphoresKHR(VkDevice device, const VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_WaitSemaphoresKHR(VkDevice device, const VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_WaitSemaphoresKHR(VkDevice device, const VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_WaitSemaphoresKHR(VkDevice device, const VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_WaitSemaphoresKHR(VkDevice device, const VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_WaitSemaphoresKHR(VkDevice device, const VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_SignalSemaphore(VkDevice device, const VkSemaphoreSignalInfo* pSignalInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_SignalSemaphore(VkDevice device, const VkSemaphoreSignalInfo* pSignalInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_SignalSemaphore(VkDevice device, const VkSemaphoreSignalInfo* pSignalInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_SignalSemaphore(VkDevice device, const VkSemaphoreSignalInfo* pSignalInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_SignalSemaphore(VkDevice device, const VkSemaphoreSignalInfo* pSignalInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_SignalSemaphore(VkDevice device, const VkSemaphoreSignalInfo* pSignalInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_SignalSemaphoreKHR(VkDevice device, const VkSemaphoreSignalInfo* pSignalInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_SignalSemaphoreKHR(VkDevice device, const VkSemaphoreSignalInfo* pSignalInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_SignalSemaphoreKHR(VkDevice device, const VkSemaphoreSignalInfo* pSignalInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_SignalSemaphoreKHR(VkDevice device, const VkSemaphoreSignalInfo* pSignalInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_SignalSemaphoreKHR(VkDevice device, const VkSemaphoreSignalInfo* pSignalInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_SignalSemaphoreKHR(VkDevice device, const VkSemaphoreSignalInfo* pSignalInfo) ATTR_WEAK;


#ifdef VK_USE_PLATFORM_ANDROID_KHR
  VKAPI_ATTR VkResult VKAPI_CALL radv_GetAndroidHardwareBufferPropertiesANDROID(VkDevice device, const struct AHardwareBuffer* buffer, VkAndroidHardwareBufferPropertiesANDROID* pProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetAndroidHardwareBufferPropertiesANDROID(VkDevice device, const struct AHardwareBuffer* buffer, VkAndroidHardwareBufferPropertiesANDROID* pProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetAndroidHardwareBufferPropertiesANDROID(VkDevice device, const struct AHardwareBuffer* buffer, VkAndroidHardwareBufferPropertiesANDROID* pProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetAndroidHardwareBufferPropertiesANDROID(VkDevice device, const struct AHardwareBuffer* buffer, VkAndroidHardwareBufferPropertiesANDROID* pProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetAndroidHardwareBufferPropertiesANDROID(VkDevice device, const struct AHardwareBuffer* buffer, VkAndroidHardwareBufferPropertiesANDROID* pProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetAndroidHardwareBufferPropertiesANDROID(VkDevice device, const struct AHardwareBuffer* buffer, VkAndroidHardwareBufferPropertiesANDROID* pProperties) ATTR_WEAK;


#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
  VKAPI_ATTR VkResult VKAPI_CALL radv_GetMemoryAndroidHardwareBufferANDROID(VkDevice device, const VkMemoryGetAndroidHardwareBufferInfoANDROID* pInfo, struct AHardwareBuffer** pBuffer) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetMemoryAndroidHardwareBufferANDROID(VkDevice device, const VkMemoryGetAndroidHardwareBufferInfoANDROID* pInfo, struct AHardwareBuffer** pBuffer) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetMemoryAndroidHardwareBufferANDROID(VkDevice device, const VkMemoryGetAndroidHardwareBufferInfoANDROID* pInfo, struct AHardwareBuffer** pBuffer) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetMemoryAndroidHardwareBufferANDROID(VkDevice device, const VkMemoryGetAndroidHardwareBufferInfoANDROID* pInfo, struct AHardwareBuffer** pBuffer) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetMemoryAndroidHardwareBufferANDROID(VkDevice device, const VkMemoryGetAndroidHardwareBufferInfoANDROID* pInfo, struct AHardwareBuffer** pBuffer) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetMemoryAndroidHardwareBufferANDROID(VkDevice device, const VkMemoryGetAndroidHardwareBufferInfoANDROID* pInfo, struct AHardwareBuffer** pBuffer) ATTR_WEAK;


#endif // VK_USE_PLATFORM_ANDROID_KHR
  VKAPI_ATTR void VKAPI_CALL radv_CmdDrawIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdDrawIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdDrawIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdDrawIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdDrawIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdDrawIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdDrawIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdDrawIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdDrawIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdDrawIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdDrawIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdDrawIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdDrawIndirectCountAMD(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdDrawIndirectCountAMD(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdDrawIndirectCountAMD(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdDrawIndirectCountAMD(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdDrawIndirectCountAMD(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdDrawIndirectCountAMD(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdDrawIndexedIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdDrawIndexedIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdDrawIndexedIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdDrawIndexedIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdDrawIndexedIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdDrawIndexedIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdDrawIndexedIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdDrawIndexedIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdDrawIndexedIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdDrawIndexedIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdDrawIndexedIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdDrawIndexedIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdDrawIndexedIndirectCountAMD(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdDrawIndexedIndirectCountAMD(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdDrawIndexedIndirectCountAMD(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdDrawIndexedIndirectCountAMD(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdDrawIndexedIndirectCountAMD(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdDrawIndexedIndirectCountAMD(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetCheckpointNV(VkCommandBuffer commandBuffer, const void* pCheckpointMarker) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetCheckpointNV(VkCommandBuffer commandBuffer, const void* pCheckpointMarker) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetCheckpointNV(VkCommandBuffer commandBuffer, const void* pCheckpointMarker) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetCheckpointNV(VkCommandBuffer commandBuffer, const void* pCheckpointMarker) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetCheckpointNV(VkCommandBuffer commandBuffer, const void* pCheckpointMarker) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetCheckpointNV(VkCommandBuffer commandBuffer, const void* pCheckpointMarker) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_GetQueueCheckpointDataNV(VkQueue queue, uint32_t* pCheckpointDataCount, VkCheckpointDataNV* pCheckpointData) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_GetQueueCheckpointDataNV(VkQueue queue, uint32_t* pCheckpointDataCount, VkCheckpointDataNV* pCheckpointData) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_GetQueueCheckpointDataNV(VkQueue queue, uint32_t* pCheckpointDataCount, VkCheckpointDataNV* pCheckpointData) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_GetQueueCheckpointDataNV(VkQueue queue, uint32_t* pCheckpointDataCount, VkCheckpointDataNV* pCheckpointData) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_GetQueueCheckpointDataNV(VkQueue queue, uint32_t* pCheckpointDataCount, VkCheckpointDataNV* pCheckpointData) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_GetQueueCheckpointDataNV(VkQueue queue, uint32_t* pCheckpointDataCount, VkCheckpointDataNV* pCheckpointData) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdBindTransformFeedbackBuffersEXT(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdBindTransformFeedbackBuffersEXT(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdBindTransformFeedbackBuffersEXT(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdBindTransformFeedbackBuffersEXT(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdBindTransformFeedbackBuffersEXT(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdBindTransformFeedbackBuffersEXT(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdBeginTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdBeginTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdBeginTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdBeginTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdBeginTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdBeginTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdEndTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdEndTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdEndTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdEndTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdEndTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdEndTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdBeginQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags, uint32_t index) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdBeginQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags, uint32_t index) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdBeginQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags, uint32_t index) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdBeginQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags, uint32_t index) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdBeginQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags, uint32_t index) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdBeginQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags, uint32_t index) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdEndQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, uint32_t index) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdEndQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, uint32_t index) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdEndQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, uint32_t index) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdEndQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, uint32_t index) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdEndQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, uint32_t index) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdEndQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, uint32_t index) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdDrawIndirectByteCountEXT(VkCommandBuffer commandBuffer, uint32_t instanceCount, uint32_t firstInstance, VkBuffer counterBuffer, VkDeviceSize counterBufferOffset, uint32_t counterOffset, uint32_t vertexStride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdDrawIndirectByteCountEXT(VkCommandBuffer commandBuffer, uint32_t instanceCount, uint32_t firstInstance, VkBuffer counterBuffer, VkDeviceSize counterBufferOffset, uint32_t counterOffset, uint32_t vertexStride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdDrawIndirectByteCountEXT(VkCommandBuffer commandBuffer, uint32_t instanceCount, uint32_t firstInstance, VkBuffer counterBuffer, VkDeviceSize counterBufferOffset, uint32_t counterOffset, uint32_t vertexStride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdDrawIndirectByteCountEXT(VkCommandBuffer commandBuffer, uint32_t instanceCount, uint32_t firstInstance, VkBuffer counterBuffer, VkDeviceSize counterBufferOffset, uint32_t counterOffset, uint32_t vertexStride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdDrawIndirectByteCountEXT(VkCommandBuffer commandBuffer, uint32_t instanceCount, uint32_t firstInstance, VkBuffer counterBuffer, VkDeviceSize counterBufferOffset, uint32_t counterOffset, uint32_t vertexStride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdDrawIndirectByteCountEXT(VkCommandBuffer commandBuffer, uint32_t instanceCount, uint32_t firstInstance, VkBuffer counterBuffer, VkDeviceSize counterBufferOffset, uint32_t counterOffset, uint32_t vertexStride) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetExclusiveScissorNV(VkCommandBuffer commandBuffer, uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, const VkRect2D* pExclusiveScissors) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetExclusiveScissorNV(VkCommandBuffer commandBuffer, uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, const VkRect2D* pExclusiveScissors) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetExclusiveScissorNV(VkCommandBuffer commandBuffer, uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, const VkRect2D* pExclusiveScissors) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetExclusiveScissorNV(VkCommandBuffer commandBuffer, uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, const VkRect2D* pExclusiveScissors) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetExclusiveScissorNV(VkCommandBuffer commandBuffer, uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, const VkRect2D* pExclusiveScissors) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetExclusiveScissorNV(VkCommandBuffer commandBuffer, uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, const VkRect2D* pExclusiveScissors) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetExclusiveScissorEnableNV(VkCommandBuffer commandBuffer, uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, const VkBool32* pExclusiveScissorEnables) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetExclusiveScissorEnableNV(VkCommandBuffer commandBuffer, uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, const VkBool32* pExclusiveScissorEnables) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetExclusiveScissorEnableNV(VkCommandBuffer commandBuffer, uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, const VkBool32* pExclusiveScissorEnables) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetExclusiveScissorEnableNV(VkCommandBuffer commandBuffer, uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, const VkBool32* pExclusiveScissorEnables) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetExclusiveScissorEnableNV(VkCommandBuffer commandBuffer, uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, const VkBool32* pExclusiveScissorEnables) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetExclusiveScissorEnableNV(VkCommandBuffer commandBuffer, uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, const VkBool32* pExclusiveScissorEnables) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdBindShadingRateImageNV(VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdBindShadingRateImageNV(VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdBindShadingRateImageNV(VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdBindShadingRateImageNV(VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdBindShadingRateImageNV(VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdBindShadingRateImageNV(VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetViewportShadingRatePaletteNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkShadingRatePaletteNV* pShadingRatePalettes) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetViewportShadingRatePaletteNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkShadingRatePaletteNV* pShadingRatePalettes) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetViewportShadingRatePaletteNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkShadingRatePaletteNV* pShadingRatePalettes) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetViewportShadingRatePaletteNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkShadingRatePaletteNV* pShadingRatePalettes) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetViewportShadingRatePaletteNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkShadingRatePaletteNV* pShadingRatePalettes) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetViewportShadingRatePaletteNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkShadingRatePaletteNV* pShadingRatePalettes) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetCoarseSampleOrderNV(VkCommandBuffer commandBuffer, VkCoarseSampleOrderTypeNV sampleOrderType, uint32_t customSampleOrderCount, const VkCoarseSampleOrderCustomNV* pCustomSampleOrders) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetCoarseSampleOrderNV(VkCommandBuffer commandBuffer, VkCoarseSampleOrderTypeNV sampleOrderType, uint32_t customSampleOrderCount, const VkCoarseSampleOrderCustomNV* pCustomSampleOrders) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetCoarseSampleOrderNV(VkCommandBuffer commandBuffer, VkCoarseSampleOrderTypeNV sampleOrderType, uint32_t customSampleOrderCount, const VkCoarseSampleOrderCustomNV* pCustomSampleOrders) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetCoarseSampleOrderNV(VkCommandBuffer commandBuffer, VkCoarseSampleOrderTypeNV sampleOrderType, uint32_t customSampleOrderCount, const VkCoarseSampleOrderCustomNV* pCustomSampleOrders) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetCoarseSampleOrderNV(VkCommandBuffer commandBuffer, VkCoarseSampleOrderTypeNV sampleOrderType, uint32_t customSampleOrderCount, const VkCoarseSampleOrderCustomNV* pCustomSampleOrders) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetCoarseSampleOrderNV(VkCommandBuffer commandBuffer, VkCoarseSampleOrderTypeNV sampleOrderType, uint32_t customSampleOrderCount, const VkCoarseSampleOrderCustomNV* pCustomSampleOrders) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdDrawMeshTasksNV(VkCommandBuffer commandBuffer, uint32_t taskCount, uint32_t firstTask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdDrawMeshTasksNV(VkCommandBuffer commandBuffer, uint32_t taskCount, uint32_t firstTask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdDrawMeshTasksNV(VkCommandBuffer commandBuffer, uint32_t taskCount, uint32_t firstTask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdDrawMeshTasksNV(VkCommandBuffer commandBuffer, uint32_t taskCount, uint32_t firstTask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdDrawMeshTasksNV(VkCommandBuffer commandBuffer, uint32_t taskCount, uint32_t firstTask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdDrawMeshTasksNV(VkCommandBuffer commandBuffer, uint32_t taskCount, uint32_t firstTask) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdDrawMeshTasksIndirectNV(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdDrawMeshTasksIndirectNV(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdDrawMeshTasksIndirectNV(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdDrawMeshTasksIndirectNV(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdDrawMeshTasksIndirectNV(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdDrawMeshTasksIndirectNV(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdDrawMeshTasksIndirectCountNV(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdDrawMeshTasksIndirectCountNV(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdDrawMeshTasksIndirectCountNV(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdDrawMeshTasksIndirectCountNV(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdDrawMeshTasksIndirectCountNV(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdDrawMeshTasksIndirectCountNV(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdDrawMeshTasksEXT(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdDrawMeshTasksEXT(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdDrawMeshTasksEXT(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdDrawMeshTasksEXT(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdDrawMeshTasksEXT(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdDrawMeshTasksEXT(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdDrawMeshTasksIndirectEXT(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdDrawMeshTasksIndirectEXT(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdDrawMeshTasksIndirectEXT(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdDrawMeshTasksIndirectEXT(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdDrawMeshTasksIndirectEXT(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdDrawMeshTasksIndirectEXT(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdDrawMeshTasksIndirectCountEXT(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdDrawMeshTasksIndirectCountEXT(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdDrawMeshTasksIndirectCountEXT(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdDrawMeshTasksIndirectCountEXT(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdDrawMeshTasksIndirectCountEXT(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdDrawMeshTasksIndirectCountEXT(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_CompileDeferredNV(VkDevice device, VkPipeline pipeline, uint32_t shader) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_CompileDeferredNV(VkDevice device, VkPipeline pipeline, uint32_t shader) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_CompileDeferredNV(VkDevice device, VkPipeline pipeline, uint32_t shader) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_CompileDeferredNV(VkDevice device, VkPipeline pipeline, uint32_t shader) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_CompileDeferredNV(VkDevice device, VkPipeline pipeline, uint32_t shader) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_CompileDeferredNV(VkDevice device, VkPipeline pipeline, uint32_t shader) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateAccelerationStructureNV(VkDevice device, const VkAccelerationStructureCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkAccelerationStructureNV* pAccelerationStructure) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_CreateAccelerationStructureNV(VkDevice device, const VkAccelerationStructureCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkAccelerationStructureNV* pAccelerationStructure) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_CreateAccelerationStructureNV(VkDevice device, const VkAccelerationStructureCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkAccelerationStructureNV* pAccelerationStructure) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_CreateAccelerationStructureNV(VkDevice device, const VkAccelerationStructureCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkAccelerationStructureNV* pAccelerationStructure) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_CreateAccelerationStructureNV(VkDevice device, const VkAccelerationStructureCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkAccelerationStructureNV* pAccelerationStructure) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_CreateAccelerationStructureNV(VkDevice device, const VkAccelerationStructureCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkAccelerationStructureNV* pAccelerationStructure) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdBindInvocationMaskHUAWEI(VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdBindInvocationMaskHUAWEI(VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdBindInvocationMaskHUAWEI(VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdBindInvocationMaskHUAWEI(VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdBindInvocationMaskHUAWEI(VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdBindInvocationMaskHUAWEI(VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_DestroyAccelerationStructureKHR(VkDevice device, VkAccelerationStructureKHR accelerationStructure, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_DestroyAccelerationStructureKHR(VkDevice device, VkAccelerationStructureKHR accelerationStructure, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_DestroyAccelerationStructureKHR(VkDevice device, VkAccelerationStructureKHR accelerationStructure, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_DestroyAccelerationStructureKHR(VkDevice device, VkAccelerationStructureKHR accelerationStructure, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_DestroyAccelerationStructureKHR(VkDevice device, VkAccelerationStructureKHR accelerationStructure, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_DestroyAccelerationStructureKHR(VkDevice device, VkAccelerationStructureKHR accelerationStructure, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_DestroyAccelerationStructureNV(VkDevice device, VkAccelerationStructureNV accelerationStructure, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_DestroyAccelerationStructureNV(VkDevice device, VkAccelerationStructureNV accelerationStructure, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_DestroyAccelerationStructureNV(VkDevice device, VkAccelerationStructureNV accelerationStructure, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_DestroyAccelerationStructureNV(VkDevice device, VkAccelerationStructureNV accelerationStructure, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_DestroyAccelerationStructureNV(VkDevice device, VkAccelerationStructureNV accelerationStructure, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_DestroyAccelerationStructureNV(VkDevice device, VkAccelerationStructureNV accelerationStructure, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_GetAccelerationStructureMemoryRequirementsNV(VkDevice device, const VkAccelerationStructureMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2KHR* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_GetAccelerationStructureMemoryRequirementsNV(VkDevice device, const VkAccelerationStructureMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2KHR* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_GetAccelerationStructureMemoryRequirementsNV(VkDevice device, const VkAccelerationStructureMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2KHR* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_GetAccelerationStructureMemoryRequirementsNV(VkDevice device, const VkAccelerationStructureMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2KHR* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_GetAccelerationStructureMemoryRequirementsNV(VkDevice device, const VkAccelerationStructureMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2KHR* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_GetAccelerationStructureMemoryRequirementsNV(VkDevice device, const VkAccelerationStructureMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2KHR* pMemoryRequirements) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_BindAccelerationStructureMemoryNV(VkDevice device, uint32_t bindInfoCount, const VkBindAccelerationStructureMemoryInfoNV* pBindInfos) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_BindAccelerationStructureMemoryNV(VkDevice device, uint32_t bindInfoCount, const VkBindAccelerationStructureMemoryInfoNV* pBindInfos) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_BindAccelerationStructureMemoryNV(VkDevice device, uint32_t bindInfoCount, const VkBindAccelerationStructureMemoryInfoNV* pBindInfos) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_BindAccelerationStructureMemoryNV(VkDevice device, uint32_t bindInfoCount, const VkBindAccelerationStructureMemoryInfoNV* pBindInfos) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_BindAccelerationStructureMemoryNV(VkDevice device, uint32_t bindInfoCount, const VkBindAccelerationStructureMemoryInfoNV* pBindInfos) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_BindAccelerationStructureMemoryNV(VkDevice device, uint32_t bindInfoCount, const VkBindAccelerationStructureMemoryInfoNV* pBindInfos) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdCopyAccelerationStructureNV(VkCommandBuffer commandBuffer, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkCopyAccelerationStructureModeKHR mode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdCopyAccelerationStructureNV(VkCommandBuffer commandBuffer, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkCopyAccelerationStructureModeKHR mode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdCopyAccelerationStructureNV(VkCommandBuffer commandBuffer, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkCopyAccelerationStructureModeKHR mode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdCopyAccelerationStructureNV(VkCommandBuffer commandBuffer, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkCopyAccelerationStructureModeKHR mode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdCopyAccelerationStructureNV(VkCommandBuffer commandBuffer, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkCopyAccelerationStructureModeKHR mode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdCopyAccelerationStructureNV(VkCommandBuffer commandBuffer, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkCopyAccelerationStructureModeKHR mode) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdCopyAccelerationStructureKHR(VkCommandBuffer commandBuffer, const VkCopyAccelerationStructureInfoKHR* pInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdCopyAccelerationStructureKHR(VkCommandBuffer commandBuffer, const VkCopyAccelerationStructureInfoKHR* pInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdCopyAccelerationStructureKHR(VkCommandBuffer commandBuffer, const VkCopyAccelerationStructureInfoKHR* pInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdCopyAccelerationStructureKHR(VkCommandBuffer commandBuffer, const VkCopyAccelerationStructureInfoKHR* pInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdCopyAccelerationStructureKHR(VkCommandBuffer commandBuffer, const VkCopyAccelerationStructureInfoKHR* pInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdCopyAccelerationStructureKHR(VkCommandBuffer commandBuffer, const VkCopyAccelerationStructureInfoKHR* pInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_CopyAccelerationStructureKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyAccelerationStructureInfoKHR* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_CopyAccelerationStructureKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyAccelerationStructureInfoKHR* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_CopyAccelerationStructureKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyAccelerationStructureInfoKHR* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_CopyAccelerationStructureKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyAccelerationStructureInfoKHR* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_CopyAccelerationStructureKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyAccelerationStructureInfoKHR* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_CopyAccelerationStructureKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyAccelerationStructureInfoKHR* pInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdCopyAccelerationStructureToMemoryKHR(VkCommandBuffer commandBuffer, const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdCopyAccelerationStructureToMemoryKHR(VkCommandBuffer commandBuffer, const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdCopyAccelerationStructureToMemoryKHR(VkCommandBuffer commandBuffer, const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdCopyAccelerationStructureToMemoryKHR(VkCommandBuffer commandBuffer, const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdCopyAccelerationStructureToMemoryKHR(VkCommandBuffer commandBuffer, const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdCopyAccelerationStructureToMemoryKHR(VkCommandBuffer commandBuffer, const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_CopyAccelerationStructureToMemoryKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_CopyAccelerationStructureToMemoryKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_CopyAccelerationStructureToMemoryKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_CopyAccelerationStructureToMemoryKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_CopyAccelerationStructureToMemoryKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_CopyAccelerationStructureToMemoryKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdCopyMemoryToAccelerationStructureKHR(VkCommandBuffer commandBuffer, const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdCopyMemoryToAccelerationStructureKHR(VkCommandBuffer commandBuffer, const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdCopyMemoryToAccelerationStructureKHR(VkCommandBuffer commandBuffer, const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdCopyMemoryToAccelerationStructureKHR(VkCommandBuffer commandBuffer, const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdCopyMemoryToAccelerationStructureKHR(VkCommandBuffer commandBuffer, const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdCopyMemoryToAccelerationStructureKHR(VkCommandBuffer commandBuffer, const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_CopyMemoryToAccelerationStructureKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_CopyMemoryToAccelerationStructureKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_CopyMemoryToAccelerationStructureKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_CopyMemoryToAccelerationStructureKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_CopyMemoryToAccelerationStructureKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_CopyMemoryToAccelerationStructureKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdWriteAccelerationStructuresPropertiesKHR(VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, const VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdWriteAccelerationStructuresPropertiesKHR(VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, const VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdWriteAccelerationStructuresPropertiesKHR(VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, const VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdWriteAccelerationStructuresPropertiesKHR(VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, const VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdWriteAccelerationStructuresPropertiesKHR(VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, const VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdWriteAccelerationStructuresPropertiesKHR(VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, const VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdWriteAccelerationStructuresPropertiesNV(VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, const VkAccelerationStructureNV* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdWriteAccelerationStructuresPropertiesNV(VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, const VkAccelerationStructureNV* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdWriteAccelerationStructuresPropertiesNV(VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, const VkAccelerationStructureNV* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdWriteAccelerationStructuresPropertiesNV(VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, const VkAccelerationStructureNV* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdWriteAccelerationStructuresPropertiesNV(VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, const VkAccelerationStructureNV* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdWriteAccelerationStructuresPropertiesNV(VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, const VkAccelerationStructureNV* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdBuildAccelerationStructureNV(VkCommandBuffer commandBuffer, const VkAccelerationStructureInfoNV* pInfo, VkBuffer instanceData, VkDeviceSize instanceOffset, VkBool32 update, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkBuffer scratch, VkDeviceSize scratchOffset) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdBuildAccelerationStructureNV(VkCommandBuffer commandBuffer, const VkAccelerationStructureInfoNV* pInfo, VkBuffer instanceData, VkDeviceSize instanceOffset, VkBool32 update, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkBuffer scratch, VkDeviceSize scratchOffset) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdBuildAccelerationStructureNV(VkCommandBuffer commandBuffer, const VkAccelerationStructureInfoNV* pInfo, VkBuffer instanceData, VkDeviceSize instanceOffset, VkBool32 update, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkBuffer scratch, VkDeviceSize scratchOffset) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdBuildAccelerationStructureNV(VkCommandBuffer commandBuffer, const VkAccelerationStructureInfoNV* pInfo, VkBuffer instanceData, VkDeviceSize instanceOffset, VkBool32 update, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkBuffer scratch, VkDeviceSize scratchOffset) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdBuildAccelerationStructureNV(VkCommandBuffer commandBuffer, const VkAccelerationStructureInfoNV* pInfo, VkBuffer instanceData, VkDeviceSize instanceOffset, VkBool32 update, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkBuffer scratch, VkDeviceSize scratchOffset) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdBuildAccelerationStructureNV(VkCommandBuffer commandBuffer, const VkAccelerationStructureInfoNV* pInfo, VkBuffer instanceData, VkDeviceSize instanceOffset, VkBool32 update, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkBuffer scratch, VkDeviceSize scratchOffset) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_WriteAccelerationStructuresPropertiesKHR(VkDevice device, uint32_t accelerationStructureCount, const VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType  queryType, size_t       dataSize, void* pData, size_t stride) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_WriteAccelerationStructuresPropertiesKHR(VkDevice device, uint32_t accelerationStructureCount, const VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType  queryType, size_t       dataSize, void* pData, size_t stride) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_WriteAccelerationStructuresPropertiesKHR(VkDevice device, uint32_t accelerationStructureCount, const VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType  queryType, size_t       dataSize, void* pData, size_t stride) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_WriteAccelerationStructuresPropertiesKHR(VkDevice device, uint32_t accelerationStructureCount, const VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType  queryType, size_t       dataSize, void* pData, size_t stride) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_WriteAccelerationStructuresPropertiesKHR(VkDevice device, uint32_t accelerationStructureCount, const VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType  queryType, size_t       dataSize, void* pData, size_t stride) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_WriteAccelerationStructuresPropertiesKHR(VkDevice device, uint32_t accelerationStructureCount, const VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType  queryType, size_t       dataSize, void* pData, size_t stride) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdTraceRaysKHR(VkCommandBuffer commandBuffer, const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, uint32_t width, uint32_t height, uint32_t depth) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdTraceRaysKHR(VkCommandBuffer commandBuffer, const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, uint32_t width, uint32_t height, uint32_t depth) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdTraceRaysKHR(VkCommandBuffer commandBuffer, const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, uint32_t width, uint32_t height, uint32_t depth) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdTraceRaysKHR(VkCommandBuffer commandBuffer, const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, uint32_t width, uint32_t height, uint32_t depth) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdTraceRaysKHR(VkCommandBuffer commandBuffer, const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, uint32_t width, uint32_t height, uint32_t depth) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdTraceRaysKHR(VkCommandBuffer commandBuffer, const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, uint32_t width, uint32_t height, uint32_t depth) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdTraceRaysNV(VkCommandBuffer commandBuffer, VkBuffer raygenShaderBindingTableBuffer, VkDeviceSize raygenShaderBindingOffset, VkBuffer missShaderBindingTableBuffer, VkDeviceSize missShaderBindingOffset, VkDeviceSize missShaderBindingStride, VkBuffer hitShaderBindingTableBuffer, VkDeviceSize hitShaderBindingOffset, VkDeviceSize hitShaderBindingStride, VkBuffer callableShaderBindingTableBuffer, VkDeviceSize callableShaderBindingOffset, VkDeviceSize callableShaderBindingStride, uint32_t width, uint32_t height, uint32_t depth) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdTraceRaysNV(VkCommandBuffer commandBuffer, VkBuffer raygenShaderBindingTableBuffer, VkDeviceSize raygenShaderBindingOffset, VkBuffer missShaderBindingTableBuffer, VkDeviceSize missShaderBindingOffset, VkDeviceSize missShaderBindingStride, VkBuffer hitShaderBindingTableBuffer, VkDeviceSize hitShaderBindingOffset, VkDeviceSize hitShaderBindingStride, VkBuffer callableShaderBindingTableBuffer, VkDeviceSize callableShaderBindingOffset, VkDeviceSize callableShaderBindingStride, uint32_t width, uint32_t height, uint32_t depth) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdTraceRaysNV(VkCommandBuffer commandBuffer, VkBuffer raygenShaderBindingTableBuffer, VkDeviceSize raygenShaderBindingOffset, VkBuffer missShaderBindingTableBuffer, VkDeviceSize missShaderBindingOffset, VkDeviceSize missShaderBindingStride, VkBuffer hitShaderBindingTableBuffer, VkDeviceSize hitShaderBindingOffset, VkDeviceSize hitShaderBindingStride, VkBuffer callableShaderBindingTableBuffer, VkDeviceSize callableShaderBindingOffset, VkDeviceSize callableShaderBindingStride, uint32_t width, uint32_t height, uint32_t depth) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdTraceRaysNV(VkCommandBuffer commandBuffer, VkBuffer raygenShaderBindingTableBuffer, VkDeviceSize raygenShaderBindingOffset, VkBuffer missShaderBindingTableBuffer, VkDeviceSize missShaderBindingOffset, VkDeviceSize missShaderBindingStride, VkBuffer hitShaderBindingTableBuffer, VkDeviceSize hitShaderBindingOffset, VkDeviceSize hitShaderBindingStride, VkBuffer callableShaderBindingTableBuffer, VkDeviceSize callableShaderBindingOffset, VkDeviceSize callableShaderBindingStride, uint32_t width, uint32_t height, uint32_t depth) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdTraceRaysNV(VkCommandBuffer commandBuffer, VkBuffer raygenShaderBindingTableBuffer, VkDeviceSize raygenShaderBindingOffset, VkBuffer missShaderBindingTableBuffer, VkDeviceSize missShaderBindingOffset, VkDeviceSize missShaderBindingStride, VkBuffer hitShaderBindingTableBuffer, VkDeviceSize hitShaderBindingOffset, VkDeviceSize hitShaderBindingStride, VkBuffer callableShaderBindingTableBuffer, VkDeviceSize callableShaderBindingOffset, VkDeviceSize callableShaderBindingStride, uint32_t width, uint32_t height, uint32_t depth) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdTraceRaysNV(VkCommandBuffer commandBuffer, VkBuffer raygenShaderBindingTableBuffer, VkDeviceSize raygenShaderBindingOffset, VkBuffer missShaderBindingTableBuffer, VkDeviceSize missShaderBindingOffset, VkDeviceSize missShaderBindingStride, VkBuffer hitShaderBindingTableBuffer, VkDeviceSize hitShaderBindingOffset, VkDeviceSize hitShaderBindingStride, VkBuffer callableShaderBindingTableBuffer, VkDeviceSize callableShaderBindingOffset, VkDeviceSize callableShaderBindingStride, uint32_t width, uint32_t height, uint32_t depth) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_GetRayTracingShaderGroupHandlesKHR(VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetRayTracingShaderGroupHandlesKHR(VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetRayTracingShaderGroupHandlesKHR(VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetRayTracingShaderGroupHandlesKHR(VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetRayTracingShaderGroupHandlesKHR(VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetRayTracingShaderGroupHandlesKHR(VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_GetRayTracingShaderGroupHandlesNV(VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetRayTracingShaderGroupHandlesNV(VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetRayTracingShaderGroupHandlesNV(VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetRayTracingShaderGroupHandlesNV(VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetRayTracingShaderGroupHandlesNV(VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetRayTracingShaderGroupHandlesNV(VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_GetRayTracingCaptureReplayShaderGroupHandlesKHR(VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetRayTracingCaptureReplayShaderGroupHandlesKHR(VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetRayTracingCaptureReplayShaderGroupHandlesKHR(VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetRayTracingCaptureReplayShaderGroupHandlesKHR(VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetRayTracingCaptureReplayShaderGroupHandlesKHR(VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetRayTracingCaptureReplayShaderGroupHandlesKHR(VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_GetAccelerationStructureHandleNV(VkDevice device, VkAccelerationStructureNV accelerationStructure, size_t dataSize, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetAccelerationStructureHandleNV(VkDevice device, VkAccelerationStructureNV accelerationStructure, size_t dataSize, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetAccelerationStructureHandleNV(VkDevice device, VkAccelerationStructureNV accelerationStructure, size_t dataSize, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetAccelerationStructureHandleNV(VkDevice device, VkAccelerationStructureNV accelerationStructure, size_t dataSize, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetAccelerationStructureHandleNV(VkDevice device, VkAccelerationStructureNV accelerationStructure, size_t dataSize, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetAccelerationStructureHandleNV(VkDevice device, VkAccelerationStructureNV accelerationStructure, size_t dataSize, void* pData) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateRayTracingPipelinesNV(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkRayTracingPipelineCreateInfoNV* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_CreateRayTracingPipelinesNV(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkRayTracingPipelineCreateInfoNV* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_CreateRayTracingPipelinesNV(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkRayTracingPipelineCreateInfoNV* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_CreateRayTracingPipelinesNV(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkRayTracingPipelineCreateInfoNV* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_CreateRayTracingPipelinesNV(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkRayTracingPipelineCreateInfoNV* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_CreateRayTracingPipelinesNV(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkRayTracingPipelineCreateInfoNV* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateRayTracingPipelinesKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkRayTracingPipelineCreateInfoKHR* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_CreateRayTracingPipelinesKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkRayTracingPipelineCreateInfoKHR* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_CreateRayTracingPipelinesKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkRayTracingPipelineCreateInfoKHR* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_CreateRayTracingPipelinesKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkRayTracingPipelineCreateInfoKHR* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_CreateRayTracingPipelinesKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkRayTracingPipelineCreateInfoKHR* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_CreateRayTracingPipelinesKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkRayTracingPipelineCreateInfoKHR* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdTraceRaysIndirectKHR(VkCommandBuffer commandBuffer, const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, VkDeviceAddress indirectDeviceAddress) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdTraceRaysIndirectKHR(VkCommandBuffer commandBuffer, const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, VkDeviceAddress indirectDeviceAddress) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdTraceRaysIndirectKHR(VkCommandBuffer commandBuffer, const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, VkDeviceAddress indirectDeviceAddress) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdTraceRaysIndirectKHR(VkCommandBuffer commandBuffer, const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, VkDeviceAddress indirectDeviceAddress) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdTraceRaysIndirectKHR(VkCommandBuffer commandBuffer, const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, VkDeviceAddress indirectDeviceAddress) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdTraceRaysIndirectKHR(VkCommandBuffer commandBuffer, const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, VkDeviceAddress indirectDeviceAddress) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdTraceRaysIndirect2KHR(VkCommandBuffer commandBuffer, VkDeviceAddress indirectDeviceAddress) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdTraceRaysIndirect2KHR(VkCommandBuffer commandBuffer, VkDeviceAddress indirectDeviceAddress) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdTraceRaysIndirect2KHR(VkCommandBuffer commandBuffer, VkDeviceAddress indirectDeviceAddress) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdTraceRaysIndirect2KHR(VkCommandBuffer commandBuffer, VkDeviceAddress indirectDeviceAddress) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdTraceRaysIndirect2KHR(VkCommandBuffer commandBuffer, VkDeviceAddress indirectDeviceAddress) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdTraceRaysIndirect2KHR(VkCommandBuffer commandBuffer, VkDeviceAddress indirectDeviceAddress) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_GetDeviceAccelerationStructureCompatibilityKHR(VkDevice device, const VkAccelerationStructureVersionInfoKHR* pVersionInfo, VkAccelerationStructureCompatibilityKHR* pCompatibility) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_GetDeviceAccelerationStructureCompatibilityKHR(VkDevice device, const VkAccelerationStructureVersionInfoKHR* pVersionInfo, VkAccelerationStructureCompatibilityKHR* pCompatibility) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_GetDeviceAccelerationStructureCompatibilityKHR(VkDevice device, const VkAccelerationStructureVersionInfoKHR* pVersionInfo, VkAccelerationStructureCompatibilityKHR* pCompatibility) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_GetDeviceAccelerationStructureCompatibilityKHR(VkDevice device, const VkAccelerationStructureVersionInfoKHR* pVersionInfo, VkAccelerationStructureCompatibilityKHR* pCompatibility) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_GetDeviceAccelerationStructureCompatibilityKHR(VkDevice device, const VkAccelerationStructureVersionInfoKHR* pVersionInfo, VkAccelerationStructureCompatibilityKHR* pCompatibility) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_GetDeviceAccelerationStructureCompatibilityKHR(VkDevice device, const VkAccelerationStructureVersionInfoKHR* pVersionInfo, VkAccelerationStructureCompatibilityKHR* pCompatibility) ATTR_WEAK;


  VKAPI_ATTR VkDeviceSize VKAPI_CALL radv_GetRayTracingShaderGroupStackSizeKHR(VkDevice device, VkPipeline pipeline, uint32_t group, VkShaderGroupShaderKHR groupShader) ATTR_WEAK;
  VKAPI_ATTR VkDeviceSize VKAPI_CALL sqtt_GetRayTracingShaderGroupStackSizeKHR(VkDevice device, VkPipeline pipeline, uint32_t group, VkShaderGroupShaderKHR groupShader) ATTR_WEAK;
  VKAPI_ATTR VkDeviceSize VKAPI_CALL rra_GetRayTracingShaderGroupStackSizeKHR(VkDevice device, VkPipeline pipeline, uint32_t group, VkShaderGroupShaderKHR groupShader) ATTR_WEAK;
  VKAPI_ATTR VkDeviceSize VKAPI_CALL rmv_GetRayTracingShaderGroupStackSizeKHR(VkDevice device, VkPipeline pipeline, uint32_t group, VkShaderGroupShaderKHR groupShader) ATTR_WEAK;
  VKAPI_ATTR VkDeviceSize VKAPI_CALL metro_exodus_GetRayTracingShaderGroupStackSizeKHR(VkDevice device, VkPipeline pipeline, uint32_t group, VkShaderGroupShaderKHR groupShader) ATTR_WEAK;
  VKAPI_ATTR VkDeviceSize VKAPI_CALL rage2_GetRayTracingShaderGroupStackSizeKHR(VkDevice device, VkPipeline pipeline, uint32_t group, VkShaderGroupShaderKHR groupShader) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetRayTracingPipelineStackSizeKHR(VkCommandBuffer commandBuffer, uint32_t pipelineStackSize) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetRayTracingPipelineStackSizeKHR(VkCommandBuffer commandBuffer, uint32_t pipelineStackSize) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetRayTracingPipelineStackSizeKHR(VkCommandBuffer commandBuffer, uint32_t pipelineStackSize) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetRayTracingPipelineStackSizeKHR(VkCommandBuffer commandBuffer, uint32_t pipelineStackSize) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetRayTracingPipelineStackSizeKHR(VkCommandBuffer commandBuffer, uint32_t pipelineStackSize) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetRayTracingPipelineStackSizeKHR(VkCommandBuffer commandBuffer, uint32_t pipelineStackSize) ATTR_WEAK;


  VKAPI_ATTR uint32_t VKAPI_CALL radv_GetImageViewHandleNVX(VkDevice device, const VkImageViewHandleInfoNVX* pInfo) ATTR_WEAK;
  VKAPI_ATTR uint32_t VKAPI_CALL sqtt_GetImageViewHandleNVX(VkDevice device, const VkImageViewHandleInfoNVX* pInfo) ATTR_WEAK;
  VKAPI_ATTR uint32_t VKAPI_CALL rra_GetImageViewHandleNVX(VkDevice device, const VkImageViewHandleInfoNVX* pInfo) ATTR_WEAK;
  VKAPI_ATTR uint32_t VKAPI_CALL rmv_GetImageViewHandleNVX(VkDevice device, const VkImageViewHandleInfoNVX* pInfo) ATTR_WEAK;
  VKAPI_ATTR uint32_t VKAPI_CALL metro_exodus_GetImageViewHandleNVX(VkDevice device, const VkImageViewHandleInfoNVX* pInfo) ATTR_WEAK;
  VKAPI_ATTR uint32_t VKAPI_CALL rage2_GetImageViewHandleNVX(VkDevice device, const VkImageViewHandleInfoNVX* pInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_GetImageViewAddressNVX(VkDevice device, VkImageView imageView, VkImageViewAddressPropertiesNVX* pProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetImageViewAddressNVX(VkDevice device, VkImageView imageView, VkImageViewAddressPropertiesNVX* pProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetImageViewAddressNVX(VkDevice device, VkImageView imageView, VkImageViewAddressPropertiesNVX* pProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetImageViewAddressNVX(VkDevice device, VkImageView imageView, VkImageViewAddressPropertiesNVX* pProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetImageViewAddressNVX(VkDevice device, VkImageView imageView, VkImageViewAddressPropertiesNVX* pProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetImageViewAddressNVX(VkDevice device, VkImageView imageView, VkImageViewAddressPropertiesNVX* pProperties) ATTR_WEAK;


#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL radv_GetDeviceGroupSurfacePresentModes2EXT(VkDevice device, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkDeviceGroupPresentModeFlagsKHR* pModes) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetDeviceGroupSurfacePresentModes2EXT(VkDevice device, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkDeviceGroupPresentModeFlagsKHR* pModes) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetDeviceGroupSurfacePresentModes2EXT(VkDevice device, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkDeviceGroupPresentModeFlagsKHR* pModes) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetDeviceGroupSurfacePresentModes2EXT(VkDevice device, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkDeviceGroupPresentModeFlagsKHR* pModes) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetDeviceGroupSurfacePresentModes2EXT(VkDevice device, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkDeviceGroupPresentModeFlagsKHR* pModes) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetDeviceGroupSurfacePresentModes2EXT(VkDevice device, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkDeviceGroupPresentModeFlagsKHR* pModes) ATTR_WEAK;


#endif // VK_USE_PLATFORM_WIN32_KHR
#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL radv_AcquireFullScreenExclusiveModeEXT(VkDevice device, VkSwapchainKHR swapchain) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_AcquireFullScreenExclusiveModeEXT(VkDevice device, VkSwapchainKHR swapchain) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_AcquireFullScreenExclusiveModeEXT(VkDevice device, VkSwapchainKHR swapchain) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_AcquireFullScreenExclusiveModeEXT(VkDevice device, VkSwapchainKHR swapchain) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_AcquireFullScreenExclusiveModeEXT(VkDevice device, VkSwapchainKHR swapchain) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_AcquireFullScreenExclusiveModeEXT(VkDevice device, VkSwapchainKHR swapchain) ATTR_WEAK;


#endif // VK_USE_PLATFORM_WIN32_KHR
#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL radv_ReleaseFullScreenExclusiveModeEXT(VkDevice device, VkSwapchainKHR swapchain) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_ReleaseFullScreenExclusiveModeEXT(VkDevice device, VkSwapchainKHR swapchain) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_ReleaseFullScreenExclusiveModeEXT(VkDevice device, VkSwapchainKHR swapchain) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_ReleaseFullScreenExclusiveModeEXT(VkDevice device, VkSwapchainKHR swapchain) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_ReleaseFullScreenExclusiveModeEXT(VkDevice device, VkSwapchainKHR swapchain) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_ReleaseFullScreenExclusiveModeEXT(VkDevice device, VkSwapchainKHR swapchain) ATTR_WEAK;


#endif // VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL radv_AcquireProfilingLockKHR(VkDevice device, const VkAcquireProfilingLockInfoKHR* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_AcquireProfilingLockKHR(VkDevice device, const VkAcquireProfilingLockInfoKHR* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_AcquireProfilingLockKHR(VkDevice device, const VkAcquireProfilingLockInfoKHR* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_AcquireProfilingLockKHR(VkDevice device, const VkAcquireProfilingLockInfoKHR* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_AcquireProfilingLockKHR(VkDevice device, const VkAcquireProfilingLockInfoKHR* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_AcquireProfilingLockKHR(VkDevice device, const VkAcquireProfilingLockInfoKHR* pInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_ReleaseProfilingLockKHR(VkDevice device) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_ReleaseProfilingLockKHR(VkDevice device) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_ReleaseProfilingLockKHR(VkDevice device) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_ReleaseProfilingLockKHR(VkDevice device) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_ReleaseProfilingLockKHR(VkDevice device) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_ReleaseProfilingLockKHR(VkDevice device) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_GetImageDrmFormatModifierPropertiesEXT(VkDevice device, VkImage image, VkImageDrmFormatModifierPropertiesEXT* pProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetImageDrmFormatModifierPropertiesEXT(VkDevice device, VkImage image, VkImageDrmFormatModifierPropertiesEXT* pProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetImageDrmFormatModifierPropertiesEXT(VkDevice device, VkImage image, VkImageDrmFormatModifierPropertiesEXT* pProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetImageDrmFormatModifierPropertiesEXT(VkDevice device, VkImage image, VkImageDrmFormatModifierPropertiesEXT* pProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetImageDrmFormatModifierPropertiesEXT(VkDevice device, VkImage image, VkImageDrmFormatModifierPropertiesEXT* pProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetImageDrmFormatModifierPropertiesEXT(VkDevice device, VkImage image, VkImageDrmFormatModifierPropertiesEXT* pProperties) ATTR_WEAK;


  VKAPI_ATTR uint64_t VKAPI_CALL radv_GetBufferOpaqueCaptureAddress(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) ATTR_WEAK;
  VKAPI_ATTR uint64_t VKAPI_CALL sqtt_GetBufferOpaqueCaptureAddress(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) ATTR_WEAK;
  VKAPI_ATTR uint64_t VKAPI_CALL rra_GetBufferOpaqueCaptureAddress(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) ATTR_WEAK;
  VKAPI_ATTR uint64_t VKAPI_CALL rmv_GetBufferOpaqueCaptureAddress(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) ATTR_WEAK;
  VKAPI_ATTR uint64_t VKAPI_CALL metro_exodus_GetBufferOpaqueCaptureAddress(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) ATTR_WEAK;
  VKAPI_ATTR uint64_t VKAPI_CALL rage2_GetBufferOpaqueCaptureAddress(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) ATTR_WEAK;


  VKAPI_ATTR uint64_t VKAPI_CALL radv_GetBufferOpaqueCaptureAddressKHR(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) ATTR_WEAK;
  VKAPI_ATTR uint64_t VKAPI_CALL sqtt_GetBufferOpaqueCaptureAddressKHR(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) ATTR_WEAK;
  VKAPI_ATTR uint64_t VKAPI_CALL rra_GetBufferOpaqueCaptureAddressKHR(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) ATTR_WEAK;
  VKAPI_ATTR uint64_t VKAPI_CALL rmv_GetBufferOpaqueCaptureAddressKHR(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) ATTR_WEAK;
  VKAPI_ATTR uint64_t VKAPI_CALL metro_exodus_GetBufferOpaqueCaptureAddressKHR(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) ATTR_WEAK;
  VKAPI_ATTR uint64_t VKAPI_CALL rage2_GetBufferOpaqueCaptureAddressKHR(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) ATTR_WEAK;


  VKAPI_ATTR VkDeviceAddress VKAPI_CALL radv_GetBufferDeviceAddress(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkDeviceAddress VKAPI_CALL sqtt_GetBufferDeviceAddress(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkDeviceAddress VKAPI_CALL rra_GetBufferDeviceAddress(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkDeviceAddress VKAPI_CALL rmv_GetBufferDeviceAddress(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkDeviceAddress VKAPI_CALL metro_exodus_GetBufferDeviceAddress(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkDeviceAddress VKAPI_CALL rage2_GetBufferDeviceAddress(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) ATTR_WEAK;


  VKAPI_ATTR VkDeviceAddress VKAPI_CALL radv_GetBufferDeviceAddressKHR(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkDeviceAddress VKAPI_CALL sqtt_GetBufferDeviceAddressKHR(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkDeviceAddress VKAPI_CALL rra_GetBufferDeviceAddressKHR(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkDeviceAddress VKAPI_CALL rmv_GetBufferDeviceAddressKHR(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkDeviceAddress VKAPI_CALL metro_exodus_GetBufferDeviceAddressKHR(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkDeviceAddress VKAPI_CALL rage2_GetBufferDeviceAddressKHR(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) ATTR_WEAK;


  VKAPI_ATTR VkDeviceAddress VKAPI_CALL radv_GetBufferDeviceAddressEXT(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkDeviceAddress VKAPI_CALL sqtt_GetBufferDeviceAddressEXT(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkDeviceAddress VKAPI_CALL rra_GetBufferDeviceAddressEXT(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkDeviceAddress VKAPI_CALL rmv_GetBufferDeviceAddressEXT(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkDeviceAddress VKAPI_CALL metro_exodus_GetBufferDeviceAddressEXT(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkDeviceAddress VKAPI_CALL rage2_GetBufferDeviceAddressEXT(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_InitializePerformanceApiINTEL(VkDevice device, const VkInitializePerformanceApiInfoINTEL* pInitializeInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_InitializePerformanceApiINTEL(VkDevice device, const VkInitializePerformanceApiInfoINTEL* pInitializeInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_InitializePerformanceApiINTEL(VkDevice device, const VkInitializePerformanceApiInfoINTEL* pInitializeInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_InitializePerformanceApiINTEL(VkDevice device, const VkInitializePerformanceApiInfoINTEL* pInitializeInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_InitializePerformanceApiINTEL(VkDevice device, const VkInitializePerformanceApiInfoINTEL* pInitializeInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_InitializePerformanceApiINTEL(VkDevice device, const VkInitializePerformanceApiInfoINTEL* pInitializeInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_UninitializePerformanceApiINTEL(VkDevice device) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_UninitializePerformanceApiINTEL(VkDevice device) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_UninitializePerformanceApiINTEL(VkDevice device) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_UninitializePerformanceApiINTEL(VkDevice device) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_UninitializePerformanceApiINTEL(VkDevice device) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_UninitializePerformanceApiINTEL(VkDevice device) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_CmdSetPerformanceMarkerINTEL(VkCommandBuffer commandBuffer, const VkPerformanceMarkerInfoINTEL* pMarkerInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_CmdSetPerformanceMarkerINTEL(VkCommandBuffer commandBuffer, const VkPerformanceMarkerInfoINTEL* pMarkerInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_CmdSetPerformanceMarkerINTEL(VkCommandBuffer commandBuffer, const VkPerformanceMarkerInfoINTEL* pMarkerInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_CmdSetPerformanceMarkerINTEL(VkCommandBuffer commandBuffer, const VkPerformanceMarkerInfoINTEL* pMarkerInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_CmdSetPerformanceMarkerINTEL(VkCommandBuffer commandBuffer, const VkPerformanceMarkerInfoINTEL* pMarkerInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_CmdSetPerformanceMarkerINTEL(VkCommandBuffer commandBuffer, const VkPerformanceMarkerInfoINTEL* pMarkerInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_CmdSetPerformanceStreamMarkerINTEL(VkCommandBuffer commandBuffer, const VkPerformanceStreamMarkerInfoINTEL* pMarkerInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_CmdSetPerformanceStreamMarkerINTEL(VkCommandBuffer commandBuffer, const VkPerformanceStreamMarkerInfoINTEL* pMarkerInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_CmdSetPerformanceStreamMarkerINTEL(VkCommandBuffer commandBuffer, const VkPerformanceStreamMarkerInfoINTEL* pMarkerInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_CmdSetPerformanceStreamMarkerINTEL(VkCommandBuffer commandBuffer, const VkPerformanceStreamMarkerInfoINTEL* pMarkerInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_CmdSetPerformanceStreamMarkerINTEL(VkCommandBuffer commandBuffer, const VkPerformanceStreamMarkerInfoINTEL* pMarkerInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_CmdSetPerformanceStreamMarkerINTEL(VkCommandBuffer commandBuffer, const VkPerformanceStreamMarkerInfoINTEL* pMarkerInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_CmdSetPerformanceOverrideINTEL(VkCommandBuffer commandBuffer, const VkPerformanceOverrideInfoINTEL* pOverrideInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_CmdSetPerformanceOverrideINTEL(VkCommandBuffer commandBuffer, const VkPerformanceOverrideInfoINTEL* pOverrideInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_CmdSetPerformanceOverrideINTEL(VkCommandBuffer commandBuffer, const VkPerformanceOverrideInfoINTEL* pOverrideInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_CmdSetPerformanceOverrideINTEL(VkCommandBuffer commandBuffer, const VkPerformanceOverrideInfoINTEL* pOverrideInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_CmdSetPerformanceOverrideINTEL(VkCommandBuffer commandBuffer, const VkPerformanceOverrideInfoINTEL* pOverrideInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_CmdSetPerformanceOverrideINTEL(VkCommandBuffer commandBuffer, const VkPerformanceOverrideInfoINTEL* pOverrideInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_AcquirePerformanceConfigurationINTEL(VkDevice device, const VkPerformanceConfigurationAcquireInfoINTEL* pAcquireInfo, VkPerformanceConfigurationINTEL* pConfiguration) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_AcquirePerformanceConfigurationINTEL(VkDevice device, const VkPerformanceConfigurationAcquireInfoINTEL* pAcquireInfo, VkPerformanceConfigurationINTEL* pConfiguration) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_AcquirePerformanceConfigurationINTEL(VkDevice device, const VkPerformanceConfigurationAcquireInfoINTEL* pAcquireInfo, VkPerformanceConfigurationINTEL* pConfiguration) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_AcquirePerformanceConfigurationINTEL(VkDevice device, const VkPerformanceConfigurationAcquireInfoINTEL* pAcquireInfo, VkPerformanceConfigurationINTEL* pConfiguration) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_AcquirePerformanceConfigurationINTEL(VkDevice device, const VkPerformanceConfigurationAcquireInfoINTEL* pAcquireInfo, VkPerformanceConfigurationINTEL* pConfiguration) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_AcquirePerformanceConfigurationINTEL(VkDevice device, const VkPerformanceConfigurationAcquireInfoINTEL* pAcquireInfo, VkPerformanceConfigurationINTEL* pConfiguration) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_ReleasePerformanceConfigurationINTEL(VkDevice device, VkPerformanceConfigurationINTEL configuration) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_ReleasePerformanceConfigurationINTEL(VkDevice device, VkPerformanceConfigurationINTEL configuration) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_ReleasePerformanceConfigurationINTEL(VkDevice device, VkPerformanceConfigurationINTEL configuration) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_ReleasePerformanceConfigurationINTEL(VkDevice device, VkPerformanceConfigurationINTEL configuration) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_ReleasePerformanceConfigurationINTEL(VkDevice device, VkPerformanceConfigurationINTEL configuration) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_ReleasePerformanceConfigurationINTEL(VkDevice device, VkPerformanceConfigurationINTEL configuration) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_QueueSetPerformanceConfigurationINTEL(VkQueue queue, VkPerformanceConfigurationINTEL configuration) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_QueueSetPerformanceConfigurationINTEL(VkQueue queue, VkPerformanceConfigurationINTEL configuration) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_QueueSetPerformanceConfigurationINTEL(VkQueue queue, VkPerformanceConfigurationINTEL configuration) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_QueueSetPerformanceConfigurationINTEL(VkQueue queue, VkPerformanceConfigurationINTEL configuration) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_QueueSetPerformanceConfigurationINTEL(VkQueue queue, VkPerformanceConfigurationINTEL configuration) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_QueueSetPerformanceConfigurationINTEL(VkQueue queue, VkPerformanceConfigurationINTEL configuration) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_GetPerformanceParameterINTEL(VkDevice device, VkPerformanceParameterTypeINTEL parameter, VkPerformanceValueINTEL* pValue) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetPerformanceParameterINTEL(VkDevice device, VkPerformanceParameterTypeINTEL parameter, VkPerformanceValueINTEL* pValue) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetPerformanceParameterINTEL(VkDevice device, VkPerformanceParameterTypeINTEL parameter, VkPerformanceValueINTEL* pValue) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetPerformanceParameterINTEL(VkDevice device, VkPerformanceParameterTypeINTEL parameter, VkPerformanceValueINTEL* pValue) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetPerformanceParameterINTEL(VkDevice device, VkPerformanceParameterTypeINTEL parameter, VkPerformanceValueINTEL* pValue) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetPerformanceParameterINTEL(VkDevice device, VkPerformanceParameterTypeINTEL parameter, VkPerformanceValueINTEL* pValue) ATTR_WEAK;


  VKAPI_ATTR uint64_t VKAPI_CALL radv_GetDeviceMemoryOpaqueCaptureAddress(VkDevice device, const VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo) ATTR_WEAK;
  VKAPI_ATTR uint64_t VKAPI_CALL sqtt_GetDeviceMemoryOpaqueCaptureAddress(VkDevice device, const VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo) ATTR_WEAK;
  VKAPI_ATTR uint64_t VKAPI_CALL rra_GetDeviceMemoryOpaqueCaptureAddress(VkDevice device, const VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo) ATTR_WEAK;
  VKAPI_ATTR uint64_t VKAPI_CALL rmv_GetDeviceMemoryOpaqueCaptureAddress(VkDevice device, const VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo) ATTR_WEAK;
  VKAPI_ATTR uint64_t VKAPI_CALL metro_exodus_GetDeviceMemoryOpaqueCaptureAddress(VkDevice device, const VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo) ATTR_WEAK;
  VKAPI_ATTR uint64_t VKAPI_CALL rage2_GetDeviceMemoryOpaqueCaptureAddress(VkDevice device, const VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo) ATTR_WEAK;


  VKAPI_ATTR uint64_t VKAPI_CALL radv_GetDeviceMemoryOpaqueCaptureAddressKHR(VkDevice device, const VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo) ATTR_WEAK;
  VKAPI_ATTR uint64_t VKAPI_CALL sqtt_GetDeviceMemoryOpaqueCaptureAddressKHR(VkDevice device, const VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo) ATTR_WEAK;
  VKAPI_ATTR uint64_t VKAPI_CALL rra_GetDeviceMemoryOpaqueCaptureAddressKHR(VkDevice device, const VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo) ATTR_WEAK;
  VKAPI_ATTR uint64_t VKAPI_CALL rmv_GetDeviceMemoryOpaqueCaptureAddressKHR(VkDevice device, const VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo) ATTR_WEAK;
  VKAPI_ATTR uint64_t VKAPI_CALL metro_exodus_GetDeviceMemoryOpaqueCaptureAddressKHR(VkDevice device, const VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo) ATTR_WEAK;
  VKAPI_ATTR uint64_t VKAPI_CALL rage2_GetDeviceMemoryOpaqueCaptureAddressKHR(VkDevice device, const VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_GetPipelineExecutablePropertiesKHR(VkDevice                        device, const VkPipelineInfoKHR*        pPipelineInfo, uint32_t* pExecutableCount, VkPipelineExecutablePropertiesKHR* pProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetPipelineExecutablePropertiesKHR(VkDevice                        device, const VkPipelineInfoKHR*        pPipelineInfo, uint32_t* pExecutableCount, VkPipelineExecutablePropertiesKHR* pProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetPipelineExecutablePropertiesKHR(VkDevice                        device, const VkPipelineInfoKHR*        pPipelineInfo, uint32_t* pExecutableCount, VkPipelineExecutablePropertiesKHR* pProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetPipelineExecutablePropertiesKHR(VkDevice                        device, const VkPipelineInfoKHR*        pPipelineInfo, uint32_t* pExecutableCount, VkPipelineExecutablePropertiesKHR* pProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetPipelineExecutablePropertiesKHR(VkDevice                        device, const VkPipelineInfoKHR*        pPipelineInfo, uint32_t* pExecutableCount, VkPipelineExecutablePropertiesKHR* pProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetPipelineExecutablePropertiesKHR(VkDevice                        device, const VkPipelineInfoKHR*        pPipelineInfo, uint32_t* pExecutableCount, VkPipelineExecutablePropertiesKHR* pProperties) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_GetPipelineExecutableStatisticsKHR(VkDevice                        device, const VkPipelineExecutableInfoKHR*  pExecutableInfo, uint32_t* pStatisticCount, VkPipelineExecutableStatisticKHR* pStatistics) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetPipelineExecutableStatisticsKHR(VkDevice                        device, const VkPipelineExecutableInfoKHR*  pExecutableInfo, uint32_t* pStatisticCount, VkPipelineExecutableStatisticKHR* pStatistics) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetPipelineExecutableStatisticsKHR(VkDevice                        device, const VkPipelineExecutableInfoKHR*  pExecutableInfo, uint32_t* pStatisticCount, VkPipelineExecutableStatisticKHR* pStatistics) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetPipelineExecutableStatisticsKHR(VkDevice                        device, const VkPipelineExecutableInfoKHR*  pExecutableInfo, uint32_t* pStatisticCount, VkPipelineExecutableStatisticKHR* pStatistics) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetPipelineExecutableStatisticsKHR(VkDevice                        device, const VkPipelineExecutableInfoKHR*  pExecutableInfo, uint32_t* pStatisticCount, VkPipelineExecutableStatisticKHR* pStatistics) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetPipelineExecutableStatisticsKHR(VkDevice                        device, const VkPipelineExecutableInfoKHR*  pExecutableInfo, uint32_t* pStatisticCount, VkPipelineExecutableStatisticKHR* pStatistics) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_GetPipelineExecutableInternalRepresentationsKHR(VkDevice                        device, const VkPipelineExecutableInfoKHR*  pExecutableInfo, uint32_t* pInternalRepresentationCount, VkPipelineExecutableInternalRepresentationKHR* pInternalRepresentations) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetPipelineExecutableInternalRepresentationsKHR(VkDevice                        device, const VkPipelineExecutableInfoKHR*  pExecutableInfo, uint32_t* pInternalRepresentationCount, VkPipelineExecutableInternalRepresentationKHR* pInternalRepresentations) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetPipelineExecutableInternalRepresentationsKHR(VkDevice                        device, const VkPipelineExecutableInfoKHR*  pExecutableInfo, uint32_t* pInternalRepresentationCount, VkPipelineExecutableInternalRepresentationKHR* pInternalRepresentations) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetPipelineExecutableInternalRepresentationsKHR(VkDevice                        device, const VkPipelineExecutableInfoKHR*  pExecutableInfo, uint32_t* pInternalRepresentationCount, VkPipelineExecutableInternalRepresentationKHR* pInternalRepresentations) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetPipelineExecutableInternalRepresentationsKHR(VkDevice                        device, const VkPipelineExecutableInfoKHR*  pExecutableInfo, uint32_t* pInternalRepresentationCount, VkPipelineExecutableInternalRepresentationKHR* pInternalRepresentations) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetPipelineExecutableInternalRepresentationsKHR(VkDevice                        device, const VkPipelineExecutableInfoKHR*  pExecutableInfo, uint32_t* pInternalRepresentationCount, VkPipelineExecutableInternalRepresentationKHR* pInternalRepresentations) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetLineStippleEXT(VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetLineStippleEXT(VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetLineStippleEXT(VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetLineStippleEXT(VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetLineStippleEXT(VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetLineStippleEXT(VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateAccelerationStructureKHR(VkDevice                                           device, const VkAccelerationStructureCreateInfoKHR*        pCreateInfo, const VkAllocationCallbacks*       pAllocator, VkAccelerationStructureKHR*                        pAccelerationStructure) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_CreateAccelerationStructureKHR(VkDevice                                           device, const VkAccelerationStructureCreateInfoKHR*        pCreateInfo, const VkAllocationCallbacks*       pAllocator, VkAccelerationStructureKHR*                        pAccelerationStructure) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_CreateAccelerationStructureKHR(VkDevice                                           device, const VkAccelerationStructureCreateInfoKHR*        pCreateInfo, const VkAllocationCallbacks*       pAllocator, VkAccelerationStructureKHR*                        pAccelerationStructure) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_CreateAccelerationStructureKHR(VkDevice                                           device, const VkAccelerationStructureCreateInfoKHR*        pCreateInfo, const VkAllocationCallbacks*       pAllocator, VkAccelerationStructureKHR*                        pAccelerationStructure) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_CreateAccelerationStructureKHR(VkDevice                                           device, const VkAccelerationStructureCreateInfoKHR*        pCreateInfo, const VkAllocationCallbacks*       pAllocator, VkAccelerationStructureKHR*                        pAccelerationStructure) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_CreateAccelerationStructureKHR(VkDevice                                           device, const VkAccelerationStructureCreateInfoKHR*        pCreateInfo, const VkAllocationCallbacks*       pAllocator, VkAccelerationStructureKHR*                        pAccelerationStructure) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdBuildAccelerationStructuresKHR(VkCommandBuffer                                    commandBuffer, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdBuildAccelerationStructuresKHR(VkCommandBuffer                                    commandBuffer, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdBuildAccelerationStructuresKHR(VkCommandBuffer                                    commandBuffer, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdBuildAccelerationStructuresKHR(VkCommandBuffer                                    commandBuffer, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdBuildAccelerationStructuresKHR(VkCommandBuffer                                    commandBuffer, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdBuildAccelerationStructuresKHR(VkCommandBuffer                                    commandBuffer, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdBuildAccelerationStructuresIndirectKHR(VkCommandBuffer                  commandBuffer, uint32_t                                           infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkDeviceAddress*             pIndirectDeviceAddresses, const uint32_t*                    pIndirectStrides, const uint32_t* const*             ppMaxPrimitiveCounts) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdBuildAccelerationStructuresIndirectKHR(VkCommandBuffer                  commandBuffer, uint32_t                                           infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkDeviceAddress*             pIndirectDeviceAddresses, const uint32_t*                    pIndirectStrides, const uint32_t* const*             ppMaxPrimitiveCounts) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdBuildAccelerationStructuresIndirectKHR(VkCommandBuffer                  commandBuffer, uint32_t                                           infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkDeviceAddress*             pIndirectDeviceAddresses, const uint32_t*                    pIndirectStrides, const uint32_t* const*             ppMaxPrimitiveCounts) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdBuildAccelerationStructuresIndirectKHR(VkCommandBuffer                  commandBuffer, uint32_t                                           infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkDeviceAddress*             pIndirectDeviceAddresses, const uint32_t*                    pIndirectStrides, const uint32_t* const*             ppMaxPrimitiveCounts) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdBuildAccelerationStructuresIndirectKHR(VkCommandBuffer                  commandBuffer, uint32_t                                           infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkDeviceAddress*             pIndirectDeviceAddresses, const uint32_t*                    pIndirectStrides, const uint32_t* const*             ppMaxPrimitiveCounts) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdBuildAccelerationStructuresIndirectKHR(VkCommandBuffer                  commandBuffer, uint32_t                                           infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkDeviceAddress*             pIndirectDeviceAddresses, const uint32_t*                    pIndirectStrides, const uint32_t* const*             ppMaxPrimitiveCounts) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_BuildAccelerationStructuresKHR(VkDevice                                           device, VkDeferredOperationKHR deferredOperation, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_BuildAccelerationStructuresKHR(VkDevice                                           device, VkDeferredOperationKHR deferredOperation, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_BuildAccelerationStructuresKHR(VkDevice                                           device, VkDeferredOperationKHR deferredOperation, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_BuildAccelerationStructuresKHR(VkDevice                                           device, VkDeferredOperationKHR deferredOperation, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_BuildAccelerationStructuresKHR(VkDevice                                           device, VkDeferredOperationKHR deferredOperation, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_BuildAccelerationStructuresKHR(VkDevice                                           device, VkDeferredOperationKHR deferredOperation, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos) ATTR_WEAK;


  VKAPI_ATTR VkDeviceAddress VKAPI_CALL radv_GetAccelerationStructureDeviceAddressKHR(VkDevice device, const VkAccelerationStructureDeviceAddressInfoKHR* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkDeviceAddress VKAPI_CALL sqtt_GetAccelerationStructureDeviceAddressKHR(VkDevice device, const VkAccelerationStructureDeviceAddressInfoKHR* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkDeviceAddress VKAPI_CALL rra_GetAccelerationStructureDeviceAddressKHR(VkDevice device, const VkAccelerationStructureDeviceAddressInfoKHR* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkDeviceAddress VKAPI_CALL rmv_GetAccelerationStructureDeviceAddressKHR(VkDevice device, const VkAccelerationStructureDeviceAddressInfoKHR* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkDeviceAddress VKAPI_CALL metro_exodus_GetAccelerationStructureDeviceAddressKHR(VkDevice device, const VkAccelerationStructureDeviceAddressInfoKHR* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkDeviceAddress VKAPI_CALL rage2_GetAccelerationStructureDeviceAddressKHR(VkDevice device, const VkAccelerationStructureDeviceAddressInfoKHR* pInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateDeferredOperationKHR(VkDevice device, const VkAllocationCallbacks* pAllocator, VkDeferredOperationKHR* pDeferredOperation) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_CreateDeferredOperationKHR(VkDevice device, const VkAllocationCallbacks* pAllocator, VkDeferredOperationKHR* pDeferredOperation) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_CreateDeferredOperationKHR(VkDevice device, const VkAllocationCallbacks* pAllocator, VkDeferredOperationKHR* pDeferredOperation) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_CreateDeferredOperationKHR(VkDevice device, const VkAllocationCallbacks* pAllocator, VkDeferredOperationKHR* pDeferredOperation) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_CreateDeferredOperationKHR(VkDevice device, const VkAllocationCallbacks* pAllocator, VkDeferredOperationKHR* pDeferredOperation) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_CreateDeferredOperationKHR(VkDevice device, const VkAllocationCallbacks* pAllocator, VkDeferredOperationKHR* pDeferredOperation) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_DestroyDeferredOperationKHR(VkDevice device, VkDeferredOperationKHR operation, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_DestroyDeferredOperationKHR(VkDevice device, VkDeferredOperationKHR operation, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_DestroyDeferredOperationKHR(VkDevice device, VkDeferredOperationKHR operation, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_DestroyDeferredOperationKHR(VkDevice device, VkDeferredOperationKHR operation, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_DestroyDeferredOperationKHR(VkDevice device, VkDeferredOperationKHR operation, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_DestroyDeferredOperationKHR(VkDevice device, VkDeferredOperationKHR operation, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR uint32_t VKAPI_CALL radv_GetDeferredOperationMaxConcurrencyKHR(VkDevice device, VkDeferredOperationKHR operation) ATTR_WEAK;
  VKAPI_ATTR uint32_t VKAPI_CALL sqtt_GetDeferredOperationMaxConcurrencyKHR(VkDevice device, VkDeferredOperationKHR operation) ATTR_WEAK;
  VKAPI_ATTR uint32_t VKAPI_CALL rra_GetDeferredOperationMaxConcurrencyKHR(VkDevice device, VkDeferredOperationKHR operation) ATTR_WEAK;
  VKAPI_ATTR uint32_t VKAPI_CALL rmv_GetDeferredOperationMaxConcurrencyKHR(VkDevice device, VkDeferredOperationKHR operation) ATTR_WEAK;
  VKAPI_ATTR uint32_t VKAPI_CALL metro_exodus_GetDeferredOperationMaxConcurrencyKHR(VkDevice device, VkDeferredOperationKHR operation) ATTR_WEAK;
  VKAPI_ATTR uint32_t VKAPI_CALL rage2_GetDeferredOperationMaxConcurrencyKHR(VkDevice device, VkDeferredOperationKHR operation) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_GetDeferredOperationResultKHR(VkDevice device, VkDeferredOperationKHR operation) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetDeferredOperationResultKHR(VkDevice device, VkDeferredOperationKHR operation) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetDeferredOperationResultKHR(VkDevice device, VkDeferredOperationKHR operation) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetDeferredOperationResultKHR(VkDevice device, VkDeferredOperationKHR operation) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetDeferredOperationResultKHR(VkDevice device, VkDeferredOperationKHR operation) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetDeferredOperationResultKHR(VkDevice device, VkDeferredOperationKHR operation) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_DeferredOperationJoinKHR(VkDevice device, VkDeferredOperationKHR operation) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_DeferredOperationJoinKHR(VkDevice device, VkDeferredOperationKHR operation) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_DeferredOperationJoinKHR(VkDevice device, VkDeferredOperationKHR operation) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_DeferredOperationJoinKHR(VkDevice device, VkDeferredOperationKHR operation) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_DeferredOperationJoinKHR(VkDevice device, VkDeferredOperationKHR operation) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_DeferredOperationJoinKHR(VkDevice device, VkDeferredOperationKHR operation) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetCullMode(VkCommandBuffer commandBuffer, VkCullModeFlags cullMode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetCullMode(VkCommandBuffer commandBuffer, VkCullModeFlags cullMode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetCullMode(VkCommandBuffer commandBuffer, VkCullModeFlags cullMode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetCullMode(VkCommandBuffer commandBuffer, VkCullModeFlags cullMode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetCullMode(VkCommandBuffer commandBuffer, VkCullModeFlags cullMode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetCullMode(VkCommandBuffer commandBuffer, VkCullModeFlags cullMode) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetCullModeEXT(VkCommandBuffer commandBuffer, VkCullModeFlags cullMode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetCullModeEXT(VkCommandBuffer commandBuffer, VkCullModeFlags cullMode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetCullModeEXT(VkCommandBuffer commandBuffer, VkCullModeFlags cullMode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetCullModeEXT(VkCommandBuffer commandBuffer, VkCullModeFlags cullMode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetCullModeEXT(VkCommandBuffer commandBuffer, VkCullModeFlags cullMode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetCullModeEXT(VkCommandBuffer commandBuffer, VkCullModeFlags cullMode) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetFrontFace(VkCommandBuffer commandBuffer, VkFrontFace frontFace) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetFrontFace(VkCommandBuffer commandBuffer, VkFrontFace frontFace) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetFrontFace(VkCommandBuffer commandBuffer, VkFrontFace frontFace) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetFrontFace(VkCommandBuffer commandBuffer, VkFrontFace frontFace) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetFrontFace(VkCommandBuffer commandBuffer, VkFrontFace frontFace) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetFrontFace(VkCommandBuffer commandBuffer, VkFrontFace frontFace) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetFrontFaceEXT(VkCommandBuffer commandBuffer, VkFrontFace frontFace) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetFrontFaceEXT(VkCommandBuffer commandBuffer, VkFrontFace frontFace) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetFrontFaceEXT(VkCommandBuffer commandBuffer, VkFrontFace frontFace) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetFrontFaceEXT(VkCommandBuffer commandBuffer, VkFrontFace frontFace) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetFrontFaceEXT(VkCommandBuffer commandBuffer, VkFrontFace frontFace) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetFrontFaceEXT(VkCommandBuffer commandBuffer, VkFrontFace frontFace) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetPrimitiveTopology(VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetPrimitiveTopology(VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetPrimitiveTopology(VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetPrimitiveTopology(VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetPrimitiveTopology(VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetPrimitiveTopology(VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetPrimitiveTopologyEXT(VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetPrimitiveTopologyEXT(VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetPrimitiveTopologyEXT(VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetPrimitiveTopologyEXT(VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetPrimitiveTopologyEXT(VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetPrimitiveTopologyEXT(VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetViewportWithCount(VkCommandBuffer commandBuffer, uint32_t viewportCount, const VkViewport* pViewports) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetViewportWithCount(VkCommandBuffer commandBuffer, uint32_t viewportCount, const VkViewport* pViewports) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetViewportWithCount(VkCommandBuffer commandBuffer, uint32_t viewportCount, const VkViewport* pViewports) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetViewportWithCount(VkCommandBuffer commandBuffer, uint32_t viewportCount, const VkViewport* pViewports) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetViewportWithCount(VkCommandBuffer commandBuffer, uint32_t viewportCount, const VkViewport* pViewports) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetViewportWithCount(VkCommandBuffer commandBuffer, uint32_t viewportCount, const VkViewport* pViewports) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetViewportWithCountEXT(VkCommandBuffer commandBuffer, uint32_t viewportCount, const VkViewport* pViewports) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetViewportWithCountEXT(VkCommandBuffer commandBuffer, uint32_t viewportCount, const VkViewport* pViewports) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetViewportWithCountEXT(VkCommandBuffer commandBuffer, uint32_t viewportCount, const VkViewport* pViewports) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetViewportWithCountEXT(VkCommandBuffer commandBuffer, uint32_t viewportCount, const VkViewport* pViewports) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetViewportWithCountEXT(VkCommandBuffer commandBuffer, uint32_t viewportCount, const VkViewport* pViewports) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetViewportWithCountEXT(VkCommandBuffer commandBuffer, uint32_t viewportCount, const VkViewport* pViewports) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetScissorWithCount(VkCommandBuffer commandBuffer, uint32_t scissorCount, const VkRect2D* pScissors) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetScissorWithCount(VkCommandBuffer commandBuffer, uint32_t scissorCount, const VkRect2D* pScissors) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetScissorWithCount(VkCommandBuffer commandBuffer, uint32_t scissorCount, const VkRect2D* pScissors) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetScissorWithCount(VkCommandBuffer commandBuffer, uint32_t scissorCount, const VkRect2D* pScissors) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetScissorWithCount(VkCommandBuffer commandBuffer, uint32_t scissorCount, const VkRect2D* pScissors) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetScissorWithCount(VkCommandBuffer commandBuffer, uint32_t scissorCount, const VkRect2D* pScissors) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetScissorWithCountEXT(VkCommandBuffer commandBuffer, uint32_t scissorCount, const VkRect2D* pScissors) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetScissorWithCountEXT(VkCommandBuffer commandBuffer, uint32_t scissorCount, const VkRect2D* pScissors) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetScissorWithCountEXT(VkCommandBuffer commandBuffer, uint32_t scissorCount, const VkRect2D* pScissors) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetScissorWithCountEXT(VkCommandBuffer commandBuffer, uint32_t scissorCount, const VkRect2D* pScissors) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetScissorWithCountEXT(VkCommandBuffer commandBuffer, uint32_t scissorCount, const VkRect2D* pScissors) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetScissorWithCountEXT(VkCommandBuffer commandBuffer, uint32_t scissorCount, const VkRect2D* pScissors) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdBindVertexBuffers2(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes, const VkDeviceSize* pStrides) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdBindVertexBuffers2(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes, const VkDeviceSize* pStrides) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdBindVertexBuffers2(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes, const VkDeviceSize* pStrides) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdBindVertexBuffers2(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes, const VkDeviceSize* pStrides) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdBindVertexBuffers2(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes, const VkDeviceSize* pStrides) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdBindVertexBuffers2(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes, const VkDeviceSize* pStrides) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdBindVertexBuffers2EXT(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes, const VkDeviceSize* pStrides) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdBindVertexBuffers2EXT(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes, const VkDeviceSize* pStrides) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdBindVertexBuffers2EXT(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes, const VkDeviceSize* pStrides) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdBindVertexBuffers2EXT(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes, const VkDeviceSize* pStrides) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdBindVertexBuffers2EXT(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes, const VkDeviceSize* pStrides) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdBindVertexBuffers2EXT(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes, const VkDeviceSize* pStrides) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetDepthTestEnable(VkCommandBuffer commandBuffer, VkBool32 depthTestEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetDepthTestEnable(VkCommandBuffer commandBuffer, VkBool32 depthTestEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetDepthTestEnable(VkCommandBuffer commandBuffer, VkBool32 depthTestEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetDepthTestEnable(VkCommandBuffer commandBuffer, VkBool32 depthTestEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetDepthTestEnable(VkCommandBuffer commandBuffer, VkBool32 depthTestEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetDepthTestEnable(VkCommandBuffer commandBuffer, VkBool32 depthTestEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetDepthTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthTestEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetDepthTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthTestEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetDepthTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthTestEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetDepthTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthTestEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetDepthTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthTestEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetDepthTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthTestEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetDepthWriteEnable(VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetDepthWriteEnable(VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetDepthWriteEnable(VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetDepthWriteEnable(VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetDepthWriteEnable(VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetDepthWriteEnable(VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetDepthWriteEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetDepthWriteEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetDepthWriteEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetDepthWriteEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetDepthWriteEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetDepthWriteEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetDepthCompareOp(VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetDepthCompareOp(VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetDepthCompareOp(VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetDepthCompareOp(VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetDepthCompareOp(VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetDepthCompareOp(VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetDepthCompareOpEXT(VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetDepthCompareOpEXT(VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetDepthCompareOpEXT(VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetDepthCompareOpEXT(VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetDepthCompareOpEXT(VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetDepthCompareOpEXT(VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetDepthBoundsTestEnable(VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetDepthBoundsTestEnable(VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetDepthBoundsTestEnable(VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetDepthBoundsTestEnable(VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetDepthBoundsTestEnable(VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetDepthBoundsTestEnable(VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetDepthBoundsTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetDepthBoundsTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetDepthBoundsTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetDepthBoundsTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetDepthBoundsTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetDepthBoundsTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetStencilTestEnable(VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetStencilTestEnable(VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetStencilTestEnable(VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetStencilTestEnable(VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetStencilTestEnable(VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetStencilTestEnable(VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetStencilTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetStencilTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetStencilTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetStencilTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetStencilTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetStencilTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetStencilOp(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetStencilOp(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetStencilOp(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetStencilOp(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetStencilOp(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetStencilOp(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetStencilOpEXT(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetStencilOpEXT(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetStencilOpEXT(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetStencilOpEXT(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetStencilOpEXT(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetStencilOpEXT(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetPatchControlPointsEXT(VkCommandBuffer commandBuffer, uint32_t patchControlPoints) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetPatchControlPointsEXT(VkCommandBuffer commandBuffer, uint32_t patchControlPoints) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetPatchControlPointsEXT(VkCommandBuffer commandBuffer, uint32_t patchControlPoints) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetPatchControlPointsEXT(VkCommandBuffer commandBuffer, uint32_t patchControlPoints) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetPatchControlPointsEXT(VkCommandBuffer commandBuffer, uint32_t patchControlPoints) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetPatchControlPointsEXT(VkCommandBuffer commandBuffer, uint32_t patchControlPoints) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetRasterizerDiscardEnable(VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetRasterizerDiscardEnable(VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetRasterizerDiscardEnable(VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetRasterizerDiscardEnable(VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetRasterizerDiscardEnable(VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetRasterizerDiscardEnable(VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetRasterizerDiscardEnableEXT(VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetRasterizerDiscardEnableEXT(VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetRasterizerDiscardEnableEXT(VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetRasterizerDiscardEnableEXT(VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetRasterizerDiscardEnableEXT(VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetRasterizerDiscardEnableEXT(VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetDepthBiasEnable(VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetDepthBiasEnable(VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetDepthBiasEnable(VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetDepthBiasEnable(VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetDepthBiasEnable(VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetDepthBiasEnable(VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetDepthBiasEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetDepthBiasEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetDepthBiasEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetDepthBiasEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetDepthBiasEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetDepthBiasEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetLogicOpEXT(VkCommandBuffer commandBuffer, VkLogicOp logicOp) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetLogicOpEXT(VkCommandBuffer commandBuffer, VkLogicOp logicOp) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetLogicOpEXT(VkCommandBuffer commandBuffer, VkLogicOp logicOp) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetLogicOpEXT(VkCommandBuffer commandBuffer, VkLogicOp logicOp) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetLogicOpEXT(VkCommandBuffer commandBuffer, VkLogicOp logicOp) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetLogicOpEXT(VkCommandBuffer commandBuffer, VkLogicOp logicOp) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetPrimitiveRestartEnable(VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetPrimitiveRestartEnable(VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetPrimitiveRestartEnable(VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetPrimitiveRestartEnable(VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetPrimitiveRestartEnable(VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetPrimitiveRestartEnable(VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetPrimitiveRestartEnableEXT(VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetPrimitiveRestartEnableEXT(VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetPrimitiveRestartEnableEXT(VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetPrimitiveRestartEnableEXT(VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetPrimitiveRestartEnableEXT(VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetPrimitiveRestartEnableEXT(VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetTessellationDomainOriginEXT(VkCommandBuffer commandBuffer, VkTessellationDomainOrigin domainOrigin) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetTessellationDomainOriginEXT(VkCommandBuffer commandBuffer, VkTessellationDomainOrigin domainOrigin) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetTessellationDomainOriginEXT(VkCommandBuffer commandBuffer, VkTessellationDomainOrigin domainOrigin) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetTessellationDomainOriginEXT(VkCommandBuffer commandBuffer, VkTessellationDomainOrigin domainOrigin) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetTessellationDomainOriginEXT(VkCommandBuffer commandBuffer, VkTessellationDomainOrigin domainOrigin) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetTessellationDomainOriginEXT(VkCommandBuffer commandBuffer, VkTessellationDomainOrigin domainOrigin) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetDepthClampEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthClampEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetDepthClampEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthClampEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetDepthClampEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthClampEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetDepthClampEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthClampEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetDepthClampEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthClampEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetDepthClampEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthClampEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetPolygonModeEXT(VkCommandBuffer commandBuffer, VkPolygonMode polygonMode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetPolygonModeEXT(VkCommandBuffer commandBuffer, VkPolygonMode polygonMode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetPolygonModeEXT(VkCommandBuffer commandBuffer, VkPolygonMode polygonMode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetPolygonModeEXT(VkCommandBuffer commandBuffer, VkPolygonMode polygonMode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetPolygonModeEXT(VkCommandBuffer commandBuffer, VkPolygonMode polygonMode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetPolygonModeEXT(VkCommandBuffer commandBuffer, VkPolygonMode polygonMode) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetRasterizationSamplesEXT(VkCommandBuffer commandBuffer, VkSampleCountFlagBits  rasterizationSamples) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetRasterizationSamplesEXT(VkCommandBuffer commandBuffer, VkSampleCountFlagBits  rasterizationSamples) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetRasterizationSamplesEXT(VkCommandBuffer commandBuffer, VkSampleCountFlagBits  rasterizationSamples) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetRasterizationSamplesEXT(VkCommandBuffer commandBuffer, VkSampleCountFlagBits  rasterizationSamples) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetRasterizationSamplesEXT(VkCommandBuffer commandBuffer, VkSampleCountFlagBits  rasterizationSamples) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetRasterizationSamplesEXT(VkCommandBuffer commandBuffer, VkSampleCountFlagBits  rasterizationSamples) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetSampleMaskEXT(VkCommandBuffer commandBuffer, VkSampleCountFlagBits  samples, const VkSampleMask*    pSampleMask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetSampleMaskEXT(VkCommandBuffer commandBuffer, VkSampleCountFlagBits  samples, const VkSampleMask*    pSampleMask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetSampleMaskEXT(VkCommandBuffer commandBuffer, VkSampleCountFlagBits  samples, const VkSampleMask*    pSampleMask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetSampleMaskEXT(VkCommandBuffer commandBuffer, VkSampleCountFlagBits  samples, const VkSampleMask*    pSampleMask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetSampleMaskEXT(VkCommandBuffer commandBuffer, VkSampleCountFlagBits  samples, const VkSampleMask*    pSampleMask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetSampleMaskEXT(VkCommandBuffer commandBuffer, VkSampleCountFlagBits  samples, const VkSampleMask*    pSampleMask) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetAlphaToCoverageEnableEXT(VkCommandBuffer commandBuffer, VkBool32 alphaToCoverageEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetAlphaToCoverageEnableEXT(VkCommandBuffer commandBuffer, VkBool32 alphaToCoverageEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetAlphaToCoverageEnableEXT(VkCommandBuffer commandBuffer, VkBool32 alphaToCoverageEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetAlphaToCoverageEnableEXT(VkCommandBuffer commandBuffer, VkBool32 alphaToCoverageEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetAlphaToCoverageEnableEXT(VkCommandBuffer commandBuffer, VkBool32 alphaToCoverageEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetAlphaToCoverageEnableEXT(VkCommandBuffer commandBuffer, VkBool32 alphaToCoverageEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetAlphaToOneEnableEXT(VkCommandBuffer commandBuffer, VkBool32 alphaToOneEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetAlphaToOneEnableEXT(VkCommandBuffer commandBuffer, VkBool32 alphaToOneEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetAlphaToOneEnableEXT(VkCommandBuffer commandBuffer, VkBool32 alphaToOneEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetAlphaToOneEnableEXT(VkCommandBuffer commandBuffer, VkBool32 alphaToOneEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetAlphaToOneEnableEXT(VkCommandBuffer commandBuffer, VkBool32 alphaToOneEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetAlphaToOneEnableEXT(VkCommandBuffer commandBuffer, VkBool32 alphaToOneEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetLogicOpEnableEXT(VkCommandBuffer commandBuffer, VkBool32 logicOpEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetLogicOpEnableEXT(VkCommandBuffer commandBuffer, VkBool32 logicOpEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetLogicOpEnableEXT(VkCommandBuffer commandBuffer, VkBool32 logicOpEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetLogicOpEnableEXT(VkCommandBuffer commandBuffer, VkBool32 logicOpEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetLogicOpEnableEXT(VkCommandBuffer commandBuffer, VkBool32 logicOpEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetLogicOpEnableEXT(VkCommandBuffer commandBuffer, VkBool32 logicOpEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetColorBlendEnableEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkBool32* pColorBlendEnables) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetColorBlendEnableEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkBool32* pColorBlendEnables) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetColorBlendEnableEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkBool32* pColorBlendEnables) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetColorBlendEnableEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkBool32* pColorBlendEnables) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetColorBlendEnableEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkBool32* pColorBlendEnables) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetColorBlendEnableEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkBool32* pColorBlendEnables) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetColorBlendEquationEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorBlendEquationEXT* pColorBlendEquations) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetColorBlendEquationEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorBlendEquationEXT* pColorBlendEquations) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetColorBlendEquationEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorBlendEquationEXT* pColorBlendEquations) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetColorBlendEquationEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorBlendEquationEXT* pColorBlendEquations) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetColorBlendEquationEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorBlendEquationEXT* pColorBlendEquations) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetColorBlendEquationEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorBlendEquationEXT* pColorBlendEquations) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetColorWriteMaskEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorComponentFlags* pColorWriteMasks) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetColorWriteMaskEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorComponentFlags* pColorWriteMasks) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetColorWriteMaskEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorComponentFlags* pColorWriteMasks) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetColorWriteMaskEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorComponentFlags* pColorWriteMasks) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetColorWriteMaskEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorComponentFlags* pColorWriteMasks) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetColorWriteMaskEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorComponentFlags* pColorWriteMasks) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetRasterizationStreamEXT(VkCommandBuffer commandBuffer, uint32_t rasterizationStream) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetRasterizationStreamEXT(VkCommandBuffer commandBuffer, uint32_t rasterizationStream) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetRasterizationStreamEXT(VkCommandBuffer commandBuffer, uint32_t rasterizationStream) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetRasterizationStreamEXT(VkCommandBuffer commandBuffer, uint32_t rasterizationStream) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetRasterizationStreamEXT(VkCommandBuffer commandBuffer, uint32_t rasterizationStream) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetRasterizationStreamEXT(VkCommandBuffer commandBuffer, uint32_t rasterizationStream) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetConservativeRasterizationModeEXT(VkCommandBuffer commandBuffer, VkConservativeRasterizationModeEXT conservativeRasterizationMode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetConservativeRasterizationModeEXT(VkCommandBuffer commandBuffer, VkConservativeRasterizationModeEXT conservativeRasterizationMode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetConservativeRasterizationModeEXT(VkCommandBuffer commandBuffer, VkConservativeRasterizationModeEXT conservativeRasterizationMode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetConservativeRasterizationModeEXT(VkCommandBuffer commandBuffer, VkConservativeRasterizationModeEXT conservativeRasterizationMode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetConservativeRasterizationModeEXT(VkCommandBuffer commandBuffer, VkConservativeRasterizationModeEXT conservativeRasterizationMode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetConservativeRasterizationModeEXT(VkCommandBuffer commandBuffer, VkConservativeRasterizationModeEXT conservativeRasterizationMode) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetExtraPrimitiveOverestimationSizeEXT(VkCommandBuffer commandBuffer, float extraPrimitiveOverestimationSize) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetExtraPrimitiveOverestimationSizeEXT(VkCommandBuffer commandBuffer, float extraPrimitiveOverestimationSize) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetExtraPrimitiveOverestimationSizeEXT(VkCommandBuffer commandBuffer, float extraPrimitiveOverestimationSize) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetExtraPrimitiveOverestimationSizeEXT(VkCommandBuffer commandBuffer, float extraPrimitiveOverestimationSize) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetExtraPrimitiveOverestimationSizeEXT(VkCommandBuffer commandBuffer, float extraPrimitiveOverestimationSize) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetExtraPrimitiveOverestimationSizeEXT(VkCommandBuffer commandBuffer, float extraPrimitiveOverestimationSize) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetDepthClipEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthClipEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetDepthClipEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthClipEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetDepthClipEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthClipEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetDepthClipEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthClipEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetDepthClipEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthClipEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetDepthClipEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthClipEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetSampleLocationsEnableEXT(VkCommandBuffer commandBuffer, VkBool32 sampleLocationsEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetSampleLocationsEnableEXT(VkCommandBuffer commandBuffer, VkBool32 sampleLocationsEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetSampleLocationsEnableEXT(VkCommandBuffer commandBuffer, VkBool32 sampleLocationsEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetSampleLocationsEnableEXT(VkCommandBuffer commandBuffer, VkBool32 sampleLocationsEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetSampleLocationsEnableEXT(VkCommandBuffer commandBuffer, VkBool32 sampleLocationsEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetSampleLocationsEnableEXT(VkCommandBuffer commandBuffer, VkBool32 sampleLocationsEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetColorBlendAdvancedEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorBlendAdvancedEXT* pColorBlendAdvanced) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetColorBlendAdvancedEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorBlendAdvancedEXT* pColorBlendAdvanced) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetColorBlendAdvancedEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorBlendAdvancedEXT* pColorBlendAdvanced) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetColorBlendAdvancedEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorBlendAdvancedEXT* pColorBlendAdvanced) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetColorBlendAdvancedEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorBlendAdvancedEXT* pColorBlendAdvanced) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetColorBlendAdvancedEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorBlendAdvancedEXT* pColorBlendAdvanced) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetProvokingVertexModeEXT(VkCommandBuffer commandBuffer, VkProvokingVertexModeEXT provokingVertexMode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetProvokingVertexModeEXT(VkCommandBuffer commandBuffer, VkProvokingVertexModeEXT provokingVertexMode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetProvokingVertexModeEXT(VkCommandBuffer commandBuffer, VkProvokingVertexModeEXT provokingVertexMode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetProvokingVertexModeEXT(VkCommandBuffer commandBuffer, VkProvokingVertexModeEXT provokingVertexMode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetProvokingVertexModeEXT(VkCommandBuffer commandBuffer, VkProvokingVertexModeEXT provokingVertexMode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetProvokingVertexModeEXT(VkCommandBuffer commandBuffer, VkProvokingVertexModeEXT provokingVertexMode) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetLineRasterizationModeEXT(VkCommandBuffer commandBuffer, VkLineRasterizationModeEXT lineRasterizationMode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetLineRasterizationModeEXT(VkCommandBuffer commandBuffer, VkLineRasterizationModeEXT lineRasterizationMode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetLineRasterizationModeEXT(VkCommandBuffer commandBuffer, VkLineRasterizationModeEXT lineRasterizationMode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetLineRasterizationModeEXT(VkCommandBuffer commandBuffer, VkLineRasterizationModeEXT lineRasterizationMode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetLineRasterizationModeEXT(VkCommandBuffer commandBuffer, VkLineRasterizationModeEXT lineRasterizationMode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetLineRasterizationModeEXT(VkCommandBuffer commandBuffer, VkLineRasterizationModeEXT lineRasterizationMode) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetLineStippleEnableEXT(VkCommandBuffer commandBuffer, VkBool32 stippledLineEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetLineStippleEnableEXT(VkCommandBuffer commandBuffer, VkBool32 stippledLineEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetLineStippleEnableEXT(VkCommandBuffer commandBuffer, VkBool32 stippledLineEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetLineStippleEnableEXT(VkCommandBuffer commandBuffer, VkBool32 stippledLineEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetLineStippleEnableEXT(VkCommandBuffer commandBuffer, VkBool32 stippledLineEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetLineStippleEnableEXT(VkCommandBuffer commandBuffer, VkBool32 stippledLineEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetDepthClipNegativeOneToOneEXT(VkCommandBuffer commandBuffer, VkBool32 negativeOneToOne) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetDepthClipNegativeOneToOneEXT(VkCommandBuffer commandBuffer, VkBool32 negativeOneToOne) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetDepthClipNegativeOneToOneEXT(VkCommandBuffer commandBuffer, VkBool32 negativeOneToOne) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetDepthClipNegativeOneToOneEXT(VkCommandBuffer commandBuffer, VkBool32 negativeOneToOne) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetDepthClipNegativeOneToOneEXT(VkCommandBuffer commandBuffer, VkBool32 negativeOneToOne) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetDepthClipNegativeOneToOneEXT(VkCommandBuffer commandBuffer, VkBool32 negativeOneToOne) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetViewportWScalingEnableNV(VkCommandBuffer commandBuffer, VkBool32 viewportWScalingEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetViewportWScalingEnableNV(VkCommandBuffer commandBuffer, VkBool32 viewportWScalingEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetViewportWScalingEnableNV(VkCommandBuffer commandBuffer, VkBool32 viewportWScalingEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetViewportWScalingEnableNV(VkCommandBuffer commandBuffer, VkBool32 viewportWScalingEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetViewportWScalingEnableNV(VkCommandBuffer commandBuffer, VkBool32 viewportWScalingEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetViewportWScalingEnableNV(VkCommandBuffer commandBuffer, VkBool32 viewportWScalingEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetViewportSwizzleNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewportSwizzleNV* pViewportSwizzles) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetViewportSwizzleNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewportSwizzleNV* pViewportSwizzles) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetViewportSwizzleNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewportSwizzleNV* pViewportSwizzles) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetViewportSwizzleNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewportSwizzleNV* pViewportSwizzles) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetViewportSwizzleNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewportSwizzleNV* pViewportSwizzles) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetViewportSwizzleNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewportSwizzleNV* pViewportSwizzles) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetCoverageToColorEnableNV(VkCommandBuffer commandBuffer, VkBool32 coverageToColorEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetCoverageToColorEnableNV(VkCommandBuffer commandBuffer, VkBool32 coverageToColorEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetCoverageToColorEnableNV(VkCommandBuffer commandBuffer, VkBool32 coverageToColorEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetCoverageToColorEnableNV(VkCommandBuffer commandBuffer, VkBool32 coverageToColorEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetCoverageToColorEnableNV(VkCommandBuffer commandBuffer, VkBool32 coverageToColorEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetCoverageToColorEnableNV(VkCommandBuffer commandBuffer, VkBool32 coverageToColorEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetCoverageToColorLocationNV(VkCommandBuffer commandBuffer, uint32_t coverageToColorLocation) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetCoverageToColorLocationNV(VkCommandBuffer commandBuffer, uint32_t coverageToColorLocation) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetCoverageToColorLocationNV(VkCommandBuffer commandBuffer, uint32_t coverageToColorLocation) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetCoverageToColorLocationNV(VkCommandBuffer commandBuffer, uint32_t coverageToColorLocation) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetCoverageToColorLocationNV(VkCommandBuffer commandBuffer, uint32_t coverageToColorLocation) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetCoverageToColorLocationNV(VkCommandBuffer commandBuffer, uint32_t coverageToColorLocation) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetCoverageModulationModeNV(VkCommandBuffer commandBuffer, VkCoverageModulationModeNV coverageModulationMode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetCoverageModulationModeNV(VkCommandBuffer commandBuffer, VkCoverageModulationModeNV coverageModulationMode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetCoverageModulationModeNV(VkCommandBuffer commandBuffer, VkCoverageModulationModeNV coverageModulationMode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetCoverageModulationModeNV(VkCommandBuffer commandBuffer, VkCoverageModulationModeNV coverageModulationMode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetCoverageModulationModeNV(VkCommandBuffer commandBuffer, VkCoverageModulationModeNV coverageModulationMode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetCoverageModulationModeNV(VkCommandBuffer commandBuffer, VkCoverageModulationModeNV coverageModulationMode) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetCoverageModulationTableEnableNV(VkCommandBuffer commandBuffer, VkBool32 coverageModulationTableEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetCoverageModulationTableEnableNV(VkCommandBuffer commandBuffer, VkBool32 coverageModulationTableEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetCoverageModulationTableEnableNV(VkCommandBuffer commandBuffer, VkBool32 coverageModulationTableEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetCoverageModulationTableEnableNV(VkCommandBuffer commandBuffer, VkBool32 coverageModulationTableEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetCoverageModulationTableEnableNV(VkCommandBuffer commandBuffer, VkBool32 coverageModulationTableEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetCoverageModulationTableEnableNV(VkCommandBuffer commandBuffer, VkBool32 coverageModulationTableEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetCoverageModulationTableNV(VkCommandBuffer commandBuffer, uint32_t coverageModulationTableCount, const float* pCoverageModulationTable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetCoverageModulationTableNV(VkCommandBuffer commandBuffer, uint32_t coverageModulationTableCount, const float* pCoverageModulationTable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetCoverageModulationTableNV(VkCommandBuffer commandBuffer, uint32_t coverageModulationTableCount, const float* pCoverageModulationTable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetCoverageModulationTableNV(VkCommandBuffer commandBuffer, uint32_t coverageModulationTableCount, const float* pCoverageModulationTable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetCoverageModulationTableNV(VkCommandBuffer commandBuffer, uint32_t coverageModulationTableCount, const float* pCoverageModulationTable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetCoverageModulationTableNV(VkCommandBuffer commandBuffer, uint32_t coverageModulationTableCount, const float* pCoverageModulationTable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetShadingRateImageEnableNV(VkCommandBuffer commandBuffer, VkBool32 shadingRateImageEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetShadingRateImageEnableNV(VkCommandBuffer commandBuffer, VkBool32 shadingRateImageEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetShadingRateImageEnableNV(VkCommandBuffer commandBuffer, VkBool32 shadingRateImageEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetShadingRateImageEnableNV(VkCommandBuffer commandBuffer, VkBool32 shadingRateImageEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetShadingRateImageEnableNV(VkCommandBuffer commandBuffer, VkBool32 shadingRateImageEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetShadingRateImageEnableNV(VkCommandBuffer commandBuffer, VkBool32 shadingRateImageEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetCoverageReductionModeNV(VkCommandBuffer commandBuffer, VkCoverageReductionModeNV coverageReductionMode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetCoverageReductionModeNV(VkCommandBuffer commandBuffer, VkCoverageReductionModeNV coverageReductionMode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetCoverageReductionModeNV(VkCommandBuffer commandBuffer, VkCoverageReductionModeNV coverageReductionMode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetCoverageReductionModeNV(VkCommandBuffer commandBuffer, VkCoverageReductionModeNV coverageReductionMode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetCoverageReductionModeNV(VkCommandBuffer commandBuffer, VkCoverageReductionModeNV coverageReductionMode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetCoverageReductionModeNV(VkCommandBuffer commandBuffer, VkCoverageReductionModeNV coverageReductionMode) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetRepresentativeFragmentTestEnableNV(VkCommandBuffer commandBuffer, VkBool32 representativeFragmentTestEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetRepresentativeFragmentTestEnableNV(VkCommandBuffer commandBuffer, VkBool32 representativeFragmentTestEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetRepresentativeFragmentTestEnableNV(VkCommandBuffer commandBuffer, VkBool32 representativeFragmentTestEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetRepresentativeFragmentTestEnableNV(VkCommandBuffer commandBuffer, VkBool32 representativeFragmentTestEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetRepresentativeFragmentTestEnableNV(VkCommandBuffer commandBuffer, VkBool32 representativeFragmentTestEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetRepresentativeFragmentTestEnableNV(VkCommandBuffer commandBuffer, VkBool32 representativeFragmentTestEnable) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_CreatePrivateDataSlot(VkDevice device, const VkPrivateDataSlotCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPrivateDataSlot* pPrivateDataSlot) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_CreatePrivateDataSlot(VkDevice device, const VkPrivateDataSlotCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPrivateDataSlot* pPrivateDataSlot) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_CreatePrivateDataSlot(VkDevice device, const VkPrivateDataSlotCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPrivateDataSlot* pPrivateDataSlot) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_CreatePrivateDataSlot(VkDevice device, const VkPrivateDataSlotCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPrivateDataSlot* pPrivateDataSlot) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_CreatePrivateDataSlot(VkDevice device, const VkPrivateDataSlotCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPrivateDataSlot* pPrivateDataSlot) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_CreatePrivateDataSlot(VkDevice device, const VkPrivateDataSlotCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPrivateDataSlot* pPrivateDataSlot) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_CreatePrivateDataSlotEXT(VkDevice device, const VkPrivateDataSlotCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPrivateDataSlot* pPrivateDataSlot) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_CreatePrivateDataSlotEXT(VkDevice device, const VkPrivateDataSlotCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPrivateDataSlot* pPrivateDataSlot) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_CreatePrivateDataSlotEXT(VkDevice device, const VkPrivateDataSlotCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPrivateDataSlot* pPrivateDataSlot) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_CreatePrivateDataSlotEXT(VkDevice device, const VkPrivateDataSlotCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPrivateDataSlot* pPrivateDataSlot) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_CreatePrivateDataSlotEXT(VkDevice device, const VkPrivateDataSlotCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPrivateDataSlot* pPrivateDataSlot) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_CreatePrivateDataSlotEXT(VkDevice device, const VkPrivateDataSlotCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPrivateDataSlot* pPrivateDataSlot) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_DestroyPrivateDataSlot(VkDevice device, VkPrivateDataSlot privateDataSlot, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_DestroyPrivateDataSlot(VkDevice device, VkPrivateDataSlot privateDataSlot, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_DestroyPrivateDataSlot(VkDevice device, VkPrivateDataSlot privateDataSlot, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_DestroyPrivateDataSlot(VkDevice device, VkPrivateDataSlot privateDataSlot, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_DestroyPrivateDataSlot(VkDevice device, VkPrivateDataSlot privateDataSlot, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_DestroyPrivateDataSlot(VkDevice device, VkPrivateDataSlot privateDataSlot, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_DestroyPrivateDataSlotEXT(VkDevice device, VkPrivateDataSlot privateDataSlot, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_DestroyPrivateDataSlotEXT(VkDevice device, VkPrivateDataSlot privateDataSlot, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_DestroyPrivateDataSlotEXT(VkDevice device, VkPrivateDataSlot privateDataSlot, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_DestroyPrivateDataSlotEXT(VkDevice device, VkPrivateDataSlot privateDataSlot, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_DestroyPrivateDataSlotEXT(VkDevice device, VkPrivateDataSlot privateDataSlot, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_DestroyPrivateDataSlotEXT(VkDevice device, VkPrivateDataSlot privateDataSlot, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_SetPrivateData(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t data) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_SetPrivateData(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t data) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_SetPrivateData(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t data) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_SetPrivateData(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t data) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_SetPrivateData(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t data) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_SetPrivateData(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t data) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_SetPrivateDataEXT(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t data) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_SetPrivateDataEXT(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t data) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_SetPrivateDataEXT(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t data) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_SetPrivateDataEXT(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t data) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_SetPrivateDataEXT(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t data) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_SetPrivateDataEXT(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t data) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_GetPrivateData(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t* pData) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_GetPrivateData(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t* pData) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_GetPrivateData(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t* pData) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_GetPrivateData(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t* pData) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_GetPrivateData(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t* pData) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_GetPrivateData(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t* pData) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_GetPrivateDataEXT(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t* pData) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_GetPrivateDataEXT(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t* pData) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_GetPrivateDataEXT(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t* pData) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_GetPrivateDataEXT(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t* pData) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_GetPrivateDataEXT(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t* pData) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_GetPrivateDataEXT(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t* pData) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdCopyBuffer2(VkCommandBuffer commandBuffer, const VkCopyBufferInfo2* pCopyBufferInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdCopyBuffer2(VkCommandBuffer commandBuffer, const VkCopyBufferInfo2* pCopyBufferInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdCopyBuffer2(VkCommandBuffer commandBuffer, const VkCopyBufferInfo2* pCopyBufferInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdCopyBuffer2(VkCommandBuffer commandBuffer, const VkCopyBufferInfo2* pCopyBufferInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdCopyBuffer2(VkCommandBuffer commandBuffer, const VkCopyBufferInfo2* pCopyBufferInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdCopyBuffer2(VkCommandBuffer commandBuffer, const VkCopyBufferInfo2* pCopyBufferInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdCopyBuffer2KHR(VkCommandBuffer commandBuffer, const VkCopyBufferInfo2* pCopyBufferInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdCopyBuffer2KHR(VkCommandBuffer commandBuffer, const VkCopyBufferInfo2* pCopyBufferInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdCopyBuffer2KHR(VkCommandBuffer commandBuffer, const VkCopyBufferInfo2* pCopyBufferInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdCopyBuffer2KHR(VkCommandBuffer commandBuffer, const VkCopyBufferInfo2* pCopyBufferInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdCopyBuffer2KHR(VkCommandBuffer commandBuffer, const VkCopyBufferInfo2* pCopyBufferInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdCopyBuffer2KHR(VkCommandBuffer commandBuffer, const VkCopyBufferInfo2* pCopyBufferInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdCopyImage2(VkCommandBuffer commandBuffer, const VkCopyImageInfo2* pCopyImageInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdCopyImage2(VkCommandBuffer commandBuffer, const VkCopyImageInfo2* pCopyImageInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdCopyImage2(VkCommandBuffer commandBuffer, const VkCopyImageInfo2* pCopyImageInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdCopyImage2(VkCommandBuffer commandBuffer, const VkCopyImageInfo2* pCopyImageInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdCopyImage2(VkCommandBuffer commandBuffer, const VkCopyImageInfo2* pCopyImageInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdCopyImage2(VkCommandBuffer commandBuffer, const VkCopyImageInfo2* pCopyImageInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdCopyImage2KHR(VkCommandBuffer commandBuffer, const VkCopyImageInfo2* pCopyImageInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdCopyImage2KHR(VkCommandBuffer commandBuffer, const VkCopyImageInfo2* pCopyImageInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdCopyImage2KHR(VkCommandBuffer commandBuffer, const VkCopyImageInfo2* pCopyImageInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdCopyImage2KHR(VkCommandBuffer commandBuffer, const VkCopyImageInfo2* pCopyImageInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdCopyImage2KHR(VkCommandBuffer commandBuffer, const VkCopyImageInfo2* pCopyImageInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdCopyImage2KHR(VkCommandBuffer commandBuffer, const VkCopyImageInfo2* pCopyImageInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdBlitImage2(VkCommandBuffer commandBuffer, const VkBlitImageInfo2* pBlitImageInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdBlitImage2(VkCommandBuffer commandBuffer, const VkBlitImageInfo2* pBlitImageInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdBlitImage2(VkCommandBuffer commandBuffer, const VkBlitImageInfo2* pBlitImageInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdBlitImage2(VkCommandBuffer commandBuffer, const VkBlitImageInfo2* pBlitImageInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdBlitImage2(VkCommandBuffer commandBuffer, const VkBlitImageInfo2* pBlitImageInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdBlitImage2(VkCommandBuffer commandBuffer, const VkBlitImageInfo2* pBlitImageInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdBlitImage2KHR(VkCommandBuffer commandBuffer, const VkBlitImageInfo2* pBlitImageInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdBlitImage2KHR(VkCommandBuffer commandBuffer, const VkBlitImageInfo2* pBlitImageInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdBlitImage2KHR(VkCommandBuffer commandBuffer, const VkBlitImageInfo2* pBlitImageInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdBlitImage2KHR(VkCommandBuffer commandBuffer, const VkBlitImageInfo2* pBlitImageInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdBlitImage2KHR(VkCommandBuffer commandBuffer, const VkBlitImageInfo2* pBlitImageInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdBlitImage2KHR(VkCommandBuffer commandBuffer, const VkBlitImageInfo2* pBlitImageInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdCopyBufferToImage2(VkCommandBuffer commandBuffer, const VkCopyBufferToImageInfo2* pCopyBufferToImageInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdCopyBufferToImage2(VkCommandBuffer commandBuffer, const VkCopyBufferToImageInfo2* pCopyBufferToImageInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdCopyBufferToImage2(VkCommandBuffer commandBuffer, const VkCopyBufferToImageInfo2* pCopyBufferToImageInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdCopyBufferToImage2(VkCommandBuffer commandBuffer, const VkCopyBufferToImageInfo2* pCopyBufferToImageInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdCopyBufferToImage2(VkCommandBuffer commandBuffer, const VkCopyBufferToImageInfo2* pCopyBufferToImageInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdCopyBufferToImage2(VkCommandBuffer commandBuffer, const VkCopyBufferToImageInfo2* pCopyBufferToImageInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdCopyBufferToImage2KHR(VkCommandBuffer commandBuffer, const VkCopyBufferToImageInfo2* pCopyBufferToImageInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdCopyBufferToImage2KHR(VkCommandBuffer commandBuffer, const VkCopyBufferToImageInfo2* pCopyBufferToImageInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdCopyBufferToImage2KHR(VkCommandBuffer commandBuffer, const VkCopyBufferToImageInfo2* pCopyBufferToImageInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdCopyBufferToImage2KHR(VkCommandBuffer commandBuffer, const VkCopyBufferToImageInfo2* pCopyBufferToImageInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdCopyBufferToImage2KHR(VkCommandBuffer commandBuffer, const VkCopyBufferToImageInfo2* pCopyBufferToImageInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdCopyBufferToImage2KHR(VkCommandBuffer commandBuffer, const VkCopyBufferToImageInfo2* pCopyBufferToImageInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdCopyImageToBuffer2(VkCommandBuffer commandBuffer, const VkCopyImageToBufferInfo2* pCopyImageToBufferInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdCopyImageToBuffer2(VkCommandBuffer commandBuffer, const VkCopyImageToBufferInfo2* pCopyImageToBufferInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdCopyImageToBuffer2(VkCommandBuffer commandBuffer, const VkCopyImageToBufferInfo2* pCopyImageToBufferInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdCopyImageToBuffer2(VkCommandBuffer commandBuffer, const VkCopyImageToBufferInfo2* pCopyImageToBufferInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdCopyImageToBuffer2(VkCommandBuffer commandBuffer, const VkCopyImageToBufferInfo2* pCopyImageToBufferInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdCopyImageToBuffer2(VkCommandBuffer commandBuffer, const VkCopyImageToBufferInfo2* pCopyImageToBufferInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdCopyImageToBuffer2KHR(VkCommandBuffer commandBuffer, const VkCopyImageToBufferInfo2* pCopyImageToBufferInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdCopyImageToBuffer2KHR(VkCommandBuffer commandBuffer, const VkCopyImageToBufferInfo2* pCopyImageToBufferInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdCopyImageToBuffer2KHR(VkCommandBuffer commandBuffer, const VkCopyImageToBufferInfo2* pCopyImageToBufferInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdCopyImageToBuffer2KHR(VkCommandBuffer commandBuffer, const VkCopyImageToBufferInfo2* pCopyImageToBufferInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdCopyImageToBuffer2KHR(VkCommandBuffer commandBuffer, const VkCopyImageToBufferInfo2* pCopyImageToBufferInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdCopyImageToBuffer2KHR(VkCommandBuffer commandBuffer, const VkCopyImageToBufferInfo2* pCopyImageToBufferInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdResolveImage2(VkCommandBuffer commandBuffer, const VkResolveImageInfo2* pResolveImageInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdResolveImage2(VkCommandBuffer commandBuffer, const VkResolveImageInfo2* pResolveImageInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdResolveImage2(VkCommandBuffer commandBuffer, const VkResolveImageInfo2* pResolveImageInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdResolveImage2(VkCommandBuffer commandBuffer, const VkResolveImageInfo2* pResolveImageInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdResolveImage2(VkCommandBuffer commandBuffer, const VkResolveImageInfo2* pResolveImageInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdResolveImage2(VkCommandBuffer commandBuffer, const VkResolveImageInfo2* pResolveImageInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdResolveImage2KHR(VkCommandBuffer commandBuffer, const VkResolveImageInfo2* pResolveImageInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdResolveImage2KHR(VkCommandBuffer commandBuffer, const VkResolveImageInfo2* pResolveImageInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdResolveImage2KHR(VkCommandBuffer commandBuffer, const VkResolveImageInfo2* pResolveImageInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdResolveImage2KHR(VkCommandBuffer commandBuffer, const VkResolveImageInfo2* pResolveImageInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdResolveImage2KHR(VkCommandBuffer commandBuffer, const VkResolveImageInfo2* pResolveImageInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdResolveImage2KHR(VkCommandBuffer commandBuffer, const VkResolveImageInfo2* pResolveImageInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetFragmentShadingRateKHR(VkCommandBuffer           commandBuffer, const VkExtent2D*                           pFragmentSize, const VkFragmentShadingRateCombinerOpKHR    combinerOps[2]) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetFragmentShadingRateKHR(VkCommandBuffer           commandBuffer, const VkExtent2D*                           pFragmentSize, const VkFragmentShadingRateCombinerOpKHR    combinerOps[2]) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetFragmentShadingRateKHR(VkCommandBuffer           commandBuffer, const VkExtent2D*                           pFragmentSize, const VkFragmentShadingRateCombinerOpKHR    combinerOps[2]) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetFragmentShadingRateKHR(VkCommandBuffer           commandBuffer, const VkExtent2D*                           pFragmentSize, const VkFragmentShadingRateCombinerOpKHR    combinerOps[2]) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetFragmentShadingRateKHR(VkCommandBuffer           commandBuffer, const VkExtent2D*                           pFragmentSize, const VkFragmentShadingRateCombinerOpKHR    combinerOps[2]) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetFragmentShadingRateKHR(VkCommandBuffer           commandBuffer, const VkExtent2D*                           pFragmentSize, const VkFragmentShadingRateCombinerOpKHR    combinerOps[2]) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetFragmentShadingRateEnumNV(VkCommandBuffer           commandBuffer, VkFragmentShadingRateNV                     shadingRate, const VkFragmentShadingRateCombinerOpKHR    combinerOps[2]) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetFragmentShadingRateEnumNV(VkCommandBuffer           commandBuffer, VkFragmentShadingRateNV                     shadingRate, const VkFragmentShadingRateCombinerOpKHR    combinerOps[2]) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetFragmentShadingRateEnumNV(VkCommandBuffer           commandBuffer, VkFragmentShadingRateNV                     shadingRate, const VkFragmentShadingRateCombinerOpKHR    combinerOps[2]) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetFragmentShadingRateEnumNV(VkCommandBuffer           commandBuffer, VkFragmentShadingRateNV                     shadingRate, const VkFragmentShadingRateCombinerOpKHR    combinerOps[2]) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetFragmentShadingRateEnumNV(VkCommandBuffer           commandBuffer, VkFragmentShadingRateNV                     shadingRate, const VkFragmentShadingRateCombinerOpKHR    combinerOps[2]) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetFragmentShadingRateEnumNV(VkCommandBuffer           commandBuffer, VkFragmentShadingRateNV                     shadingRate, const VkFragmentShadingRateCombinerOpKHR    combinerOps[2]) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_GetAccelerationStructureBuildSizesKHR(VkDevice                                            device, VkAccelerationStructureBuildTypeKHR                 buildType, const VkAccelerationStructureBuildGeometryInfoKHR*  pBuildInfo, const uint32_t*  pMaxPrimitiveCounts, VkAccelerationStructureBuildSizesInfoKHR*           pSizeInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_GetAccelerationStructureBuildSizesKHR(VkDevice                                            device, VkAccelerationStructureBuildTypeKHR                 buildType, const VkAccelerationStructureBuildGeometryInfoKHR*  pBuildInfo, const uint32_t*  pMaxPrimitiveCounts, VkAccelerationStructureBuildSizesInfoKHR*           pSizeInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_GetAccelerationStructureBuildSizesKHR(VkDevice                                            device, VkAccelerationStructureBuildTypeKHR                 buildType, const VkAccelerationStructureBuildGeometryInfoKHR*  pBuildInfo, const uint32_t*  pMaxPrimitiveCounts, VkAccelerationStructureBuildSizesInfoKHR*           pSizeInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_GetAccelerationStructureBuildSizesKHR(VkDevice                                            device, VkAccelerationStructureBuildTypeKHR                 buildType, const VkAccelerationStructureBuildGeometryInfoKHR*  pBuildInfo, const uint32_t*  pMaxPrimitiveCounts, VkAccelerationStructureBuildSizesInfoKHR*           pSizeInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_GetAccelerationStructureBuildSizesKHR(VkDevice                                            device, VkAccelerationStructureBuildTypeKHR                 buildType, const VkAccelerationStructureBuildGeometryInfoKHR*  pBuildInfo, const uint32_t*  pMaxPrimitiveCounts, VkAccelerationStructureBuildSizesInfoKHR*           pSizeInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_GetAccelerationStructureBuildSizesKHR(VkDevice                                            device, VkAccelerationStructureBuildTypeKHR                 buildType, const VkAccelerationStructureBuildGeometryInfoKHR*  pBuildInfo, const uint32_t*  pMaxPrimitiveCounts, VkAccelerationStructureBuildSizesInfoKHR*           pSizeInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetVertexInputEXT(VkCommandBuffer commandBuffer, uint32_t vertexBindingDescriptionCount, const VkVertexInputBindingDescription2EXT* pVertexBindingDescriptions, uint32_t vertexAttributeDescriptionCount, const VkVertexInputAttributeDescription2EXT* pVertexAttributeDescriptions) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetVertexInputEXT(VkCommandBuffer commandBuffer, uint32_t vertexBindingDescriptionCount, const VkVertexInputBindingDescription2EXT* pVertexBindingDescriptions, uint32_t vertexAttributeDescriptionCount, const VkVertexInputAttributeDescription2EXT* pVertexAttributeDescriptions) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetVertexInputEXT(VkCommandBuffer commandBuffer, uint32_t vertexBindingDescriptionCount, const VkVertexInputBindingDescription2EXT* pVertexBindingDescriptions, uint32_t vertexAttributeDescriptionCount, const VkVertexInputAttributeDescription2EXT* pVertexAttributeDescriptions) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetVertexInputEXT(VkCommandBuffer commandBuffer, uint32_t vertexBindingDescriptionCount, const VkVertexInputBindingDescription2EXT* pVertexBindingDescriptions, uint32_t vertexAttributeDescriptionCount, const VkVertexInputAttributeDescription2EXT* pVertexAttributeDescriptions) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetVertexInputEXT(VkCommandBuffer commandBuffer, uint32_t vertexBindingDescriptionCount, const VkVertexInputBindingDescription2EXT* pVertexBindingDescriptions, uint32_t vertexAttributeDescriptionCount, const VkVertexInputAttributeDescription2EXT* pVertexAttributeDescriptions) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetVertexInputEXT(VkCommandBuffer commandBuffer, uint32_t vertexBindingDescriptionCount, const VkVertexInputBindingDescription2EXT* pVertexBindingDescriptions, uint32_t vertexAttributeDescriptionCount, const VkVertexInputAttributeDescription2EXT* pVertexAttributeDescriptions) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetColorWriteEnableEXT(VkCommandBuffer       commandBuffer, uint32_t                                attachmentCount, const VkBool32*   pColorWriteEnables) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetColorWriteEnableEXT(VkCommandBuffer       commandBuffer, uint32_t                                attachmentCount, const VkBool32*   pColorWriteEnables) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetColorWriteEnableEXT(VkCommandBuffer       commandBuffer, uint32_t                                attachmentCount, const VkBool32*   pColorWriteEnables) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetColorWriteEnableEXT(VkCommandBuffer       commandBuffer, uint32_t                                attachmentCount, const VkBool32*   pColorWriteEnables) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetColorWriteEnableEXT(VkCommandBuffer       commandBuffer, uint32_t                                attachmentCount, const VkBool32*   pColorWriteEnables) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetColorWriteEnableEXT(VkCommandBuffer       commandBuffer, uint32_t                                attachmentCount, const VkBool32*   pColorWriteEnables) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetEvent2(VkCommandBuffer                   commandBuffer, VkEvent                                             event, const VkDependencyInfo*                             pDependencyInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetEvent2(VkCommandBuffer                   commandBuffer, VkEvent                                             event, const VkDependencyInfo*                             pDependencyInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetEvent2(VkCommandBuffer                   commandBuffer, VkEvent                                             event, const VkDependencyInfo*                             pDependencyInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetEvent2(VkCommandBuffer                   commandBuffer, VkEvent                                             event, const VkDependencyInfo*                             pDependencyInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetEvent2(VkCommandBuffer                   commandBuffer, VkEvent                                             event, const VkDependencyInfo*                             pDependencyInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetEvent2(VkCommandBuffer                   commandBuffer, VkEvent                                             event, const VkDependencyInfo*                             pDependencyInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetEvent2KHR(VkCommandBuffer                   commandBuffer, VkEvent                                             event, const VkDependencyInfo*                             pDependencyInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetEvent2KHR(VkCommandBuffer                   commandBuffer, VkEvent                                             event, const VkDependencyInfo*                             pDependencyInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetEvent2KHR(VkCommandBuffer                   commandBuffer, VkEvent                                             event, const VkDependencyInfo*                             pDependencyInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetEvent2KHR(VkCommandBuffer                   commandBuffer, VkEvent                                             event, const VkDependencyInfo*                             pDependencyInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetEvent2KHR(VkCommandBuffer                   commandBuffer, VkEvent                                             event, const VkDependencyInfo*                             pDependencyInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetEvent2KHR(VkCommandBuffer                   commandBuffer, VkEvent                                             event, const VkDependencyInfo*                             pDependencyInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdResetEvent2(VkCommandBuffer                   commandBuffer, VkEvent                                             event, VkPipelineStageFlags2               stageMask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdResetEvent2(VkCommandBuffer                   commandBuffer, VkEvent                                             event, VkPipelineStageFlags2               stageMask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdResetEvent2(VkCommandBuffer                   commandBuffer, VkEvent                                             event, VkPipelineStageFlags2               stageMask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdResetEvent2(VkCommandBuffer                   commandBuffer, VkEvent                                             event, VkPipelineStageFlags2               stageMask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdResetEvent2(VkCommandBuffer                   commandBuffer, VkEvent                                             event, VkPipelineStageFlags2               stageMask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdResetEvent2(VkCommandBuffer                   commandBuffer, VkEvent                                             event, VkPipelineStageFlags2               stageMask) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdResetEvent2KHR(VkCommandBuffer                   commandBuffer, VkEvent                                             event, VkPipelineStageFlags2               stageMask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdResetEvent2KHR(VkCommandBuffer                   commandBuffer, VkEvent                                             event, VkPipelineStageFlags2               stageMask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdResetEvent2KHR(VkCommandBuffer                   commandBuffer, VkEvent                                             event, VkPipelineStageFlags2               stageMask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdResetEvent2KHR(VkCommandBuffer                   commandBuffer, VkEvent                                             event, VkPipelineStageFlags2               stageMask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdResetEvent2KHR(VkCommandBuffer                   commandBuffer, VkEvent                                             event, VkPipelineStageFlags2               stageMask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdResetEvent2KHR(VkCommandBuffer                   commandBuffer, VkEvent                                             event, VkPipelineStageFlags2               stageMask) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdWaitEvents2(VkCommandBuffer                   commandBuffer, uint32_t                                            eventCount, const VkEvent*                     pEvents, const VkDependencyInfo*            pDependencyInfos) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdWaitEvents2(VkCommandBuffer                   commandBuffer, uint32_t                                            eventCount, const VkEvent*                     pEvents, const VkDependencyInfo*            pDependencyInfos) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdWaitEvents2(VkCommandBuffer                   commandBuffer, uint32_t                                            eventCount, const VkEvent*                     pEvents, const VkDependencyInfo*            pDependencyInfos) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdWaitEvents2(VkCommandBuffer                   commandBuffer, uint32_t                                            eventCount, const VkEvent*                     pEvents, const VkDependencyInfo*            pDependencyInfos) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdWaitEvents2(VkCommandBuffer                   commandBuffer, uint32_t                                            eventCount, const VkEvent*                     pEvents, const VkDependencyInfo*            pDependencyInfos) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdWaitEvents2(VkCommandBuffer                   commandBuffer, uint32_t                                            eventCount, const VkEvent*                     pEvents, const VkDependencyInfo*            pDependencyInfos) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdWaitEvents2KHR(VkCommandBuffer                   commandBuffer, uint32_t                                            eventCount, const VkEvent*                     pEvents, const VkDependencyInfo*            pDependencyInfos) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdWaitEvents2KHR(VkCommandBuffer                   commandBuffer, uint32_t                                            eventCount, const VkEvent*                     pEvents, const VkDependencyInfo*            pDependencyInfos) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdWaitEvents2KHR(VkCommandBuffer                   commandBuffer, uint32_t                                            eventCount, const VkEvent*                     pEvents, const VkDependencyInfo*            pDependencyInfos) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdWaitEvents2KHR(VkCommandBuffer                   commandBuffer, uint32_t                                            eventCount, const VkEvent*                     pEvents, const VkDependencyInfo*            pDependencyInfos) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdWaitEvents2KHR(VkCommandBuffer                   commandBuffer, uint32_t                                            eventCount, const VkEvent*                     pEvents, const VkDependencyInfo*            pDependencyInfos) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdWaitEvents2KHR(VkCommandBuffer                   commandBuffer, uint32_t                                            eventCount, const VkEvent*                     pEvents, const VkDependencyInfo*            pDependencyInfos) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdPipelineBarrier2(VkCommandBuffer                   commandBuffer, const VkDependencyInfo*                             pDependencyInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdPipelineBarrier2(VkCommandBuffer                   commandBuffer, const VkDependencyInfo*                             pDependencyInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdPipelineBarrier2(VkCommandBuffer                   commandBuffer, const VkDependencyInfo*                             pDependencyInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdPipelineBarrier2(VkCommandBuffer                   commandBuffer, const VkDependencyInfo*                             pDependencyInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdPipelineBarrier2(VkCommandBuffer                   commandBuffer, const VkDependencyInfo*                             pDependencyInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdPipelineBarrier2(VkCommandBuffer                   commandBuffer, const VkDependencyInfo*                             pDependencyInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdPipelineBarrier2KHR(VkCommandBuffer                   commandBuffer, const VkDependencyInfo*                             pDependencyInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdPipelineBarrier2KHR(VkCommandBuffer                   commandBuffer, const VkDependencyInfo*                             pDependencyInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdPipelineBarrier2KHR(VkCommandBuffer                   commandBuffer, const VkDependencyInfo*                             pDependencyInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdPipelineBarrier2KHR(VkCommandBuffer                   commandBuffer, const VkDependencyInfo*                             pDependencyInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdPipelineBarrier2KHR(VkCommandBuffer                   commandBuffer, const VkDependencyInfo*                             pDependencyInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdPipelineBarrier2KHR(VkCommandBuffer                   commandBuffer, const VkDependencyInfo*                             pDependencyInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_QueueSubmit2(VkQueue                           queue, uint32_t                            submitCount, const VkSubmitInfo2*              pSubmits, VkFence           fence) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_QueueSubmit2(VkQueue                           queue, uint32_t                            submitCount, const VkSubmitInfo2*              pSubmits, VkFence           fence) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_QueueSubmit2(VkQueue                           queue, uint32_t                            submitCount, const VkSubmitInfo2*              pSubmits, VkFence           fence) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_QueueSubmit2(VkQueue                           queue, uint32_t                            submitCount, const VkSubmitInfo2*              pSubmits, VkFence           fence) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_QueueSubmit2(VkQueue                           queue, uint32_t                            submitCount, const VkSubmitInfo2*              pSubmits, VkFence           fence) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_QueueSubmit2(VkQueue                           queue, uint32_t                            submitCount, const VkSubmitInfo2*              pSubmits, VkFence           fence) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_QueueSubmit2KHR(VkQueue                           queue, uint32_t                            submitCount, const VkSubmitInfo2*              pSubmits, VkFence           fence) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_QueueSubmit2KHR(VkQueue                           queue, uint32_t                            submitCount, const VkSubmitInfo2*              pSubmits, VkFence           fence) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_QueueSubmit2KHR(VkQueue                           queue, uint32_t                            submitCount, const VkSubmitInfo2*              pSubmits, VkFence           fence) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_QueueSubmit2KHR(VkQueue                           queue, uint32_t                            submitCount, const VkSubmitInfo2*              pSubmits, VkFence           fence) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_QueueSubmit2KHR(VkQueue                           queue, uint32_t                            submitCount, const VkSubmitInfo2*              pSubmits, VkFence           fence) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_QueueSubmit2KHR(VkQueue                           queue, uint32_t                            submitCount, const VkSubmitInfo2*              pSubmits, VkFence           fence) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdWriteTimestamp2(VkCommandBuffer                   commandBuffer, VkPipelineStageFlags2               stage, VkQueryPool                                         queryPool, uint32_t                                            query) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdWriteTimestamp2(VkCommandBuffer                   commandBuffer, VkPipelineStageFlags2               stage, VkQueryPool                                         queryPool, uint32_t                                            query) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdWriteTimestamp2(VkCommandBuffer                   commandBuffer, VkPipelineStageFlags2               stage, VkQueryPool                                         queryPool, uint32_t                                            query) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdWriteTimestamp2(VkCommandBuffer                   commandBuffer, VkPipelineStageFlags2               stage, VkQueryPool                                         queryPool, uint32_t                                            query) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdWriteTimestamp2(VkCommandBuffer                   commandBuffer, VkPipelineStageFlags2               stage, VkQueryPool                                         queryPool, uint32_t                                            query) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdWriteTimestamp2(VkCommandBuffer                   commandBuffer, VkPipelineStageFlags2               stage, VkQueryPool                                         queryPool, uint32_t                                            query) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdWriteTimestamp2KHR(VkCommandBuffer                   commandBuffer, VkPipelineStageFlags2               stage, VkQueryPool                                         queryPool, uint32_t                                            query) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdWriteTimestamp2KHR(VkCommandBuffer                   commandBuffer, VkPipelineStageFlags2               stage, VkQueryPool                                         queryPool, uint32_t                                            query) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdWriteTimestamp2KHR(VkCommandBuffer                   commandBuffer, VkPipelineStageFlags2               stage, VkQueryPool                                         queryPool, uint32_t                                            query) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdWriteTimestamp2KHR(VkCommandBuffer                   commandBuffer, VkPipelineStageFlags2               stage, VkQueryPool                                         queryPool, uint32_t                                            query) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdWriteTimestamp2KHR(VkCommandBuffer                   commandBuffer, VkPipelineStageFlags2               stage, VkQueryPool                                         queryPool, uint32_t                                            query) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdWriteTimestamp2KHR(VkCommandBuffer                   commandBuffer, VkPipelineStageFlags2               stage, VkQueryPool                                         queryPool, uint32_t                                            query) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdWriteBufferMarker2AMD(VkCommandBuffer                   commandBuffer, VkPipelineStageFlags2               stage, VkBuffer                                            dstBuffer, VkDeviceSize                                        dstOffset, uint32_t                                            marker) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdWriteBufferMarker2AMD(VkCommandBuffer                   commandBuffer, VkPipelineStageFlags2               stage, VkBuffer                                            dstBuffer, VkDeviceSize                                        dstOffset, uint32_t                                            marker) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdWriteBufferMarker2AMD(VkCommandBuffer                   commandBuffer, VkPipelineStageFlags2               stage, VkBuffer                                            dstBuffer, VkDeviceSize                                        dstOffset, uint32_t                                            marker) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdWriteBufferMarker2AMD(VkCommandBuffer                   commandBuffer, VkPipelineStageFlags2               stage, VkBuffer                                            dstBuffer, VkDeviceSize                                        dstOffset, uint32_t                                            marker) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdWriteBufferMarker2AMD(VkCommandBuffer                   commandBuffer, VkPipelineStageFlags2               stage, VkBuffer                                            dstBuffer, VkDeviceSize                                        dstOffset, uint32_t                                            marker) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdWriteBufferMarker2AMD(VkCommandBuffer                   commandBuffer, VkPipelineStageFlags2               stage, VkBuffer                                            dstBuffer, VkDeviceSize                                        dstOffset, uint32_t                                            marker) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_GetQueueCheckpointData2NV(VkQueue queue, uint32_t* pCheckpointDataCount, VkCheckpointData2NV* pCheckpointData) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_GetQueueCheckpointData2NV(VkQueue queue, uint32_t* pCheckpointDataCount, VkCheckpointData2NV* pCheckpointData) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_GetQueueCheckpointData2NV(VkQueue queue, uint32_t* pCheckpointDataCount, VkCheckpointData2NV* pCheckpointData) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_GetQueueCheckpointData2NV(VkQueue queue, uint32_t* pCheckpointDataCount, VkCheckpointData2NV* pCheckpointData) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_GetQueueCheckpointData2NV(VkQueue queue, uint32_t* pCheckpointDataCount, VkCheckpointData2NV* pCheckpointData) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_GetQueueCheckpointData2NV(VkQueue queue, uint32_t* pCheckpointDataCount, VkCheckpointData2NV* pCheckpointData) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateVideoSessionKHR(VkDevice device, const VkVideoSessionCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkVideoSessionKHR* pVideoSession) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_CreateVideoSessionKHR(VkDevice device, const VkVideoSessionCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkVideoSessionKHR* pVideoSession) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_CreateVideoSessionKHR(VkDevice device, const VkVideoSessionCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkVideoSessionKHR* pVideoSession) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_CreateVideoSessionKHR(VkDevice device, const VkVideoSessionCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkVideoSessionKHR* pVideoSession) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_CreateVideoSessionKHR(VkDevice device, const VkVideoSessionCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkVideoSessionKHR* pVideoSession) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_CreateVideoSessionKHR(VkDevice device, const VkVideoSessionCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkVideoSessionKHR* pVideoSession) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_DestroyVideoSessionKHR(VkDevice device, VkVideoSessionKHR videoSession, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_DestroyVideoSessionKHR(VkDevice device, VkVideoSessionKHR videoSession, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_DestroyVideoSessionKHR(VkDevice device, VkVideoSessionKHR videoSession, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_DestroyVideoSessionKHR(VkDevice device, VkVideoSessionKHR videoSession, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_DestroyVideoSessionKHR(VkDevice device, VkVideoSessionKHR videoSession, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_DestroyVideoSessionKHR(VkDevice device, VkVideoSessionKHR videoSession, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateVideoSessionParametersKHR(VkDevice device, const VkVideoSessionParametersCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkVideoSessionParametersKHR* pVideoSessionParameters) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_CreateVideoSessionParametersKHR(VkDevice device, const VkVideoSessionParametersCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkVideoSessionParametersKHR* pVideoSessionParameters) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_CreateVideoSessionParametersKHR(VkDevice device, const VkVideoSessionParametersCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkVideoSessionParametersKHR* pVideoSessionParameters) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_CreateVideoSessionParametersKHR(VkDevice device, const VkVideoSessionParametersCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkVideoSessionParametersKHR* pVideoSessionParameters) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_CreateVideoSessionParametersKHR(VkDevice device, const VkVideoSessionParametersCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkVideoSessionParametersKHR* pVideoSessionParameters) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_CreateVideoSessionParametersKHR(VkDevice device, const VkVideoSessionParametersCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkVideoSessionParametersKHR* pVideoSessionParameters) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_UpdateVideoSessionParametersKHR(VkDevice device, VkVideoSessionParametersKHR videoSessionParameters, const VkVideoSessionParametersUpdateInfoKHR* pUpdateInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_UpdateVideoSessionParametersKHR(VkDevice device, VkVideoSessionParametersKHR videoSessionParameters, const VkVideoSessionParametersUpdateInfoKHR* pUpdateInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_UpdateVideoSessionParametersKHR(VkDevice device, VkVideoSessionParametersKHR videoSessionParameters, const VkVideoSessionParametersUpdateInfoKHR* pUpdateInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_UpdateVideoSessionParametersKHR(VkDevice device, VkVideoSessionParametersKHR videoSessionParameters, const VkVideoSessionParametersUpdateInfoKHR* pUpdateInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_UpdateVideoSessionParametersKHR(VkDevice device, VkVideoSessionParametersKHR videoSessionParameters, const VkVideoSessionParametersUpdateInfoKHR* pUpdateInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_UpdateVideoSessionParametersKHR(VkDevice device, VkVideoSessionParametersKHR videoSessionParameters, const VkVideoSessionParametersUpdateInfoKHR* pUpdateInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_DestroyVideoSessionParametersKHR(VkDevice device, VkVideoSessionParametersKHR videoSessionParameters, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_DestroyVideoSessionParametersKHR(VkDevice device, VkVideoSessionParametersKHR videoSessionParameters, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_DestroyVideoSessionParametersKHR(VkDevice device, VkVideoSessionParametersKHR videoSessionParameters, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_DestroyVideoSessionParametersKHR(VkDevice device, VkVideoSessionParametersKHR videoSessionParameters, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_DestroyVideoSessionParametersKHR(VkDevice device, VkVideoSessionParametersKHR videoSessionParameters, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_DestroyVideoSessionParametersKHR(VkDevice device, VkVideoSessionParametersKHR videoSessionParameters, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_GetVideoSessionMemoryRequirementsKHR(VkDevice device, VkVideoSessionKHR videoSession, uint32_t* pMemoryRequirementsCount, VkVideoSessionMemoryRequirementsKHR* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetVideoSessionMemoryRequirementsKHR(VkDevice device, VkVideoSessionKHR videoSession, uint32_t* pMemoryRequirementsCount, VkVideoSessionMemoryRequirementsKHR* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetVideoSessionMemoryRequirementsKHR(VkDevice device, VkVideoSessionKHR videoSession, uint32_t* pMemoryRequirementsCount, VkVideoSessionMemoryRequirementsKHR* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetVideoSessionMemoryRequirementsKHR(VkDevice device, VkVideoSessionKHR videoSession, uint32_t* pMemoryRequirementsCount, VkVideoSessionMemoryRequirementsKHR* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetVideoSessionMemoryRequirementsKHR(VkDevice device, VkVideoSessionKHR videoSession, uint32_t* pMemoryRequirementsCount, VkVideoSessionMemoryRequirementsKHR* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetVideoSessionMemoryRequirementsKHR(VkDevice device, VkVideoSessionKHR videoSession, uint32_t* pMemoryRequirementsCount, VkVideoSessionMemoryRequirementsKHR* pMemoryRequirements) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_BindVideoSessionMemoryKHR(VkDevice device, VkVideoSessionKHR videoSession, uint32_t bindSessionMemoryInfoCount, const VkBindVideoSessionMemoryInfoKHR* pBindSessionMemoryInfos) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_BindVideoSessionMemoryKHR(VkDevice device, VkVideoSessionKHR videoSession, uint32_t bindSessionMemoryInfoCount, const VkBindVideoSessionMemoryInfoKHR* pBindSessionMemoryInfos) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_BindVideoSessionMemoryKHR(VkDevice device, VkVideoSessionKHR videoSession, uint32_t bindSessionMemoryInfoCount, const VkBindVideoSessionMemoryInfoKHR* pBindSessionMemoryInfos) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_BindVideoSessionMemoryKHR(VkDevice device, VkVideoSessionKHR videoSession, uint32_t bindSessionMemoryInfoCount, const VkBindVideoSessionMemoryInfoKHR* pBindSessionMemoryInfos) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_BindVideoSessionMemoryKHR(VkDevice device, VkVideoSessionKHR videoSession, uint32_t bindSessionMemoryInfoCount, const VkBindVideoSessionMemoryInfoKHR* pBindSessionMemoryInfos) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_BindVideoSessionMemoryKHR(VkDevice device, VkVideoSessionKHR videoSession, uint32_t bindSessionMemoryInfoCount, const VkBindVideoSessionMemoryInfoKHR* pBindSessionMemoryInfos) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdDecodeVideoKHR(VkCommandBuffer commandBuffer, const VkVideoDecodeInfoKHR* pDecodeInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdDecodeVideoKHR(VkCommandBuffer commandBuffer, const VkVideoDecodeInfoKHR* pDecodeInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdDecodeVideoKHR(VkCommandBuffer commandBuffer, const VkVideoDecodeInfoKHR* pDecodeInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdDecodeVideoKHR(VkCommandBuffer commandBuffer, const VkVideoDecodeInfoKHR* pDecodeInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdDecodeVideoKHR(VkCommandBuffer commandBuffer, const VkVideoDecodeInfoKHR* pDecodeInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdDecodeVideoKHR(VkCommandBuffer commandBuffer, const VkVideoDecodeInfoKHR* pDecodeInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdBeginVideoCodingKHR(VkCommandBuffer commandBuffer, const VkVideoBeginCodingInfoKHR* pBeginInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdBeginVideoCodingKHR(VkCommandBuffer commandBuffer, const VkVideoBeginCodingInfoKHR* pBeginInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdBeginVideoCodingKHR(VkCommandBuffer commandBuffer, const VkVideoBeginCodingInfoKHR* pBeginInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdBeginVideoCodingKHR(VkCommandBuffer commandBuffer, const VkVideoBeginCodingInfoKHR* pBeginInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdBeginVideoCodingKHR(VkCommandBuffer commandBuffer, const VkVideoBeginCodingInfoKHR* pBeginInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdBeginVideoCodingKHR(VkCommandBuffer commandBuffer, const VkVideoBeginCodingInfoKHR* pBeginInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdControlVideoCodingKHR(VkCommandBuffer commandBuffer, const VkVideoCodingControlInfoKHR* pCodingControlInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdControlVideoCodingKHR(VkCommandBuffer commandBuffer, const VkVideoCodingControlInfoKHR* pCodingControlInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdControlVideoCodingKHR(VkCommandBuffer commandBuffer, const VkVideoCodingControlInfoKHR* pCodingControlInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdControlVideoCodingKHR(VkCommandBuffer commandBuffer, const VkVideoCodingControlInfoKHR* pCodingControlInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdControlVideoCodingKHR(VkCommandBuffer commandBuffer, const VkVideoCodingControlInfoKHR* pCodingControlInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdControlVideoCodingKHR(VkCommandBuffer commandBuffer, const VkVideoCodingControlInfoKHR* pCodingControlInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdEndVideoCodingKHR(VkCommandBuffer commandBuffer, const VkVideoEndCodingInfoKHR* pEndCodingInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdEndVideoCodingKHR(VkCommandBuffer commandBuffer, const VkVideoEndCodingInfoKHR* pEndCodingInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdEndVideoCodingKHR(VkCommandBuffer commandBuffer, const VkVideoEndCodingInfoKHR* pEndCodingInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdEndVideoCodingKHR(VkCommandBuffer commandBuffer, const VkVideoEndCodingInfoKHR* pEndCodingInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdEndVideoCodingKHR(VkCommandBuffer commandBuffer, const VkVideoEndCodingInfoKHR* pEndCodingInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdEndVideoCodingKHR(VkCommandBuffer commandBuffer, const VkVideoEndCodingInfoKHR* pEndCodingInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdDecompressMemoryNV(VkCommandBuffer commandBuffer, uint32_t decompressRegionCount, const VkDecompressMemoryRegionNV* pDecompressMemoryRegions) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdDecompressMemoryNV(VkCommandBuffer commandBuffer, uint32_t decompressRegionCount, const VkDecompressMemoryRegionNV* pDecompressMemoryRegions) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdDecompressMemoryNV(VkCommandBuffer commandBuffer, uint32_t decompressRegionCount, const VkDecompressMemoryRegionNV* pDecompressMemoryRegions) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdDecompressMemoryNV(VkCommandBuffer commandBuffer, uint32_t decompressRegionCount, const VkDecompressMemoryRegionNV* pDecompressMemoryRegions) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdDecompressMemoryNV(VkCommandBuffer commandBuffer, uint32_t decompressRegionCount, const VkDecompressMemoryRegionNV* pDecompressMemoryRegions) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdDecompressMemoryNV(VkCommandBuffer commandBuffer, uint32_t decompressRegionCount, const VkDecompressMemoryRegionNV* pDecompressMemoryRegions) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdDecompressMemoryIndirectCountNV(VkCommandBuffer commandBuffer, VkDeviceAddress indirectCommandsAddress, VkDeviceAddress indirectCommandsCountAddress, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdDecompressMemoryIndirectCountNV(VkCommandBuffer commandBuffer, VkDeviceAddress indirectCommandsAddress, VkDeviceAddress indirectCommandsCountAddress, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdDecompressMemoryIndirectCountNV(VkCommandBuffer commandBuffer, VkDeviceAddress indirectCommandsAddress, VkDeviceAddress indirectCommandsCountAddress, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdDecompressMemoryIndirectCountNV(VkCommandBuffer commandBuffer, VkDeviceAddress indirectCommandsAddress, VkDeviceAddress indirectCommandsCountAddress, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdDecompressMemoryIndirectCountNV(VkCommandBuffer commandBuffer, VkDeviceAddress indirectCommandsAddress, VkDeviceAddress indirectCommandsCountAddress, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdDecompressMemoryIndirectCountNV(VkCommandBuffer commandBuffer, VkDeviceAddress indirectCommandsAddress, VkDeviceAddress indirectCommandsCountAddress, uint32_t stride) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateCuModuleNVX(VkDevice device, const VkCuModuleCreateInfoNVX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCuModuleNVX* pModule) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_CreateCuModuleNVX(VkDevice device, const VkCuModuleCreateInfoNVX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCuModuleNVX* pModule) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_CreateCuModuleNVX(VkDevice device, const VkCuModuleCreateInfoNVX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCuModuleNVX* pModule) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_CreateCuModuleNVX(VkDevice device, const VkCuModuleCreateInfoNVX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCuModuleNVX* pModule) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_CreateCuModuleNVX(VkDevice device, const VkCuModuleCreateInfoNVX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCuModuleNVX* pModule) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_CreateCuModuleNVX(VkDevice device, const VkCuModuleCreateInfoNVX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCuModuleNVX* pModule) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateCuFunctionNVX(VkDevice device, const VkCuFunctionCreateInfoNVX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCuFunctionNVX* pFunction) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_CreateCuFunctionNVX(VkDevice device, const VkCuFunctionCreateInfoNVX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCuFunctionNVX* pFunction) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_CreateCuFunctionNVX(VkDevice device, const VkCuFunctionCreateInfoNVX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCuFunctionNVX* pFunction) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_CreateCuFunctionNVX(VkDevice device, const VkCuFunctionCreateInfoNVX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCuFunctionNVX* pFunction) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_CreateCuFunctionNVX(VkDevice device, const VkCuFunctionCreateInfoNVX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCuFunctionNVX* pFunction) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_CreateCuFunctionNVX(VkDevice device, const VkCuFunctionCreateInfoNVX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCuFunctionNVX* pFunction) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_DestroyCuModuleNVX(VkDevice device, VkCuModuleNVX module, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_DestroyCuModuleNVX(VkDevice device, VkCuModuleNVX module, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_DestroyCuModuleNVX(VkDevice device, VkCuModuleNVX module, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_DestroyCuModuleNVX(VkDevice device, VkCuModuleNVX module, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_DestroyCuModuleNVX(VkDevice device, VkCuModuleNVX module, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_DestroyCuModuleNVX(VkDevice device, VkCuModuleNVX module, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_DestroyCuFunctionNVX(VkDevice device, VkCuFunctionNVX function, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_DestroyCuFunctionNVX(VkDevice device, VkCuFunctionNVX function, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_DestroyCuFunctionNVX(VkDevice device, VkCuFunctionNVX function, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_DestroyCuFunctionNVX(VkDevice device, VkCuFunctionNVX function, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_DestroyCuFunctionNVX(VkDevice device, VkCuFunctionNVX function, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_DestroyCuFunctionNVX(VkDevice device, VkCuFunctionNVX function, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdCuLaunchKernelNVX(VkCommandBuffer commandBuffer, const VkCuLaunchInfoNVX* pLaunchInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdCuLaunchKernelNVX(VkCommandBuffer commandBuffer, const VkCuLaunchInfoNVX* pLaunchInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdCuLaunchKernelNVX(VkCommandBuffer commandBuffer, const VkCuLaunchInfoNVX* pLaunchInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdCuLaunchKernelNVX(VkCommandBuffer commandBuffer, const VkCuLaunchInfoNVX* pLaunchInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdCuLaunchKernelNVX(VkCommandBuffer commandBuffer, const VkCuLaunchInfoNVX* pLaunchInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdCuLaunchKernelNVX(VkCommandBuffer commandBuffer, const VkCuLaunchInfoNVX* pLaunchInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_GetDescriptorSetLayoutSizeEXT(VkDevice device, VkDescriptorSetLayout layout, VkDeviceSize* pLayoutSizeInBytes) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_GetDescriptorSetLayoutSizeEXT(VkDevice device, VkDescriptorSetLayout layout, VkDeviceSize* pLayoutSizeInBytes) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_GetDescriptorSetLayoutSizeEXT(VkDevice device, VkDescriptorSetLayout layout, VkDeviceSize* pLayoutSizeInBytes) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_GetDescriptorSetLayoutSizeEXT(VkDevice device, VkDescriptorSetLayout layout, VkDeviceSize* pLayoutSizeInBytes) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_GetDescriptorSetLayoutSizeEXT(VkDevice device, VkDescriptorSetLayout layout, VkDeviceSize* pLayoutSizeInBytes) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_GetDescriptorSetLayoutSizeEXT(VkDevice device, VkDescriptorSetLayout layout, VkDeviceSize* pLayoutSizeInBytes) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_GetDescriptorSetLayoutBindingOffsetEXT(VkDevice device, VkDescriptorSetLayout layout, uint32_t binding, VkDeviceSize* pOffset) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_GetDescriptorSetLayoutBindingOffsetEXT(VkDevice device, VkDescriptorSetLayout layout, uint32_t binding, VkDeviceSize* pOffset) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_GetDescriptorSetLayoutBindingOffsetEXT(VkDevice device, VkDescriptorSetLayout layout, uint32_t binding, VkDeviceSize* pOffset) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_GetDescriptorSetLayoutBindingOffsetEXT(VkDevice device, VkDescriptorSetLayout layout, uint32_t binding, VkDeviceSize* pOffset) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_GetDescriptorSetLayoutBindingOffsetEXT(VkDevice device, VkDescriptorSetLayout layout, uint32_t binding, VkDeviceSize* pOffset) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_GetDescriptorSetLayoutBindingOffsetEXT(VkDevice device, VkDescriptorSetLayout layout, uint32_t binding, VkDeviceSize* pOffset) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_GetDescriptorEXT(VkDevice device, const VkDescriptorGetInfoEXT* pDescriptorInfo, size_t dataSize, void* pDescriptor) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_GetDescriptorEXT(VkDevice device, const VkDescriptorGetInfoEXT* pDescriptorInfo, size_t dataSize, void* pDescriptor) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_GetDescriptorEXT(VkDevice device, const VkDescriptorGetInfoEXT* pDescriptorInfo, size_t dataSize, void* pDescriptor) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_GetDescriptorEXT(VkDevice device, const VkDescriptorGetInfoEXT* pDescriptorInfo, size_t dataSize, void* pDescriptor) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_GetDescriptorEXT(VkDevice device, const VkDescriptorGetInfoEXT* pDescriptorInfo, size_t dataSize, void* pDescriptor) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_GetDescriptorEXT(VkDevice device, const VkDescriptorGetInfoEXT* pDescriptorInfo, size_t dataSize, void* pDescriptor) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdBindDescriptorBuffersEXT(VkCommandBuffer commandBuffer, uint32_t bufferCount, const VkDescriptorBufferBindingInfoEXT* pBindingInfos) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdBindDescriptorBuffersEXT(VkCommandBuffer commandBuffer, uint32_t bufferCount, const VkDescriptorBufferBindingInfoEXT* pBindingInfos) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdBindDescriptorBuffersEXT(VkCommandBuffer commandBuffer, uint32_t bufferCount, const VkDescriptorBufferBindingInfoEXT* pBindingInfos) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdBindDescriptorBuffersEXT(VkCommandBuffer commandBuffer, uint32_t bufferCount, const VkDescriptorBufferBindingInfoEXT* pBindingInfos) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdBindDescriptorBuffersEXT(VkCommandBuffer commandBuffer, uint32_t bufferCount, const VkDescriptorBufferBindingInfoEXT* pBindingInfos) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdBindDescriptorBuffersEXT(VkCommandBuffer commandBuffer, uint32_t bufferCount, const VkDescriptorBufferBindingInfoEXT* pBindingInfos) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdSetDescriptorBufferOffsetsEXT(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t setCount, const uint32_t* pBufferIndices, const VkDeviceSize* pOffsets) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdSetDescriptorBufferOffsetsEXT(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t setCount, const uint32_t* pBufferIndices, const VkDeviceSize* pOffsets) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdSetDescriptorBufferOffsetsEXT(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t setCount, const uint32_t* pBufferIndices, const VkDeviceSize* pOffsets) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdSetDescriptorBufferOffsetsEXT(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t setCount, const uint32_t* pBufferIndices, const VkDeviceSize* pOffsets) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdSetDescriptorBufferOffsetsEXT(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t setCount, const uint32_t* pBufferIndices, const VkDeviceSize* pOffsets) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdSetDescriptorBufferOffsetsEXT(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t setCount, const uint32_t* pBufferIndices, const VkDeviceSize* pOffsets) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdBindDescriptorBufferEmbeddedSamplersEXT(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdBindDescriptorBufferEmbeddedSamplersEXT(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdBindDescriptorBufferEmbeddedSamplersEXT(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdBindDescriptorBufferEmbeddedSamplersEXT(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdBindDescriptorBufferEmbeddedSamplersEXT(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdBindDescriptorBufferEmbeddedSamplersEXT(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_GetBufferOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkBufferCaptureDescriptorDataInfoEXT* pInfo, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetBufferOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkBufferCaptureDescriptorDataInfoEXT* pInfo, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetBufferOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkBufferCaptureDescriptorDataInfoEXT* pInfo, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetBufferOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkBufferCaptureDescriptorDataInfoEXT* pInfo, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetBufferOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkBufferCaptureDescriptorDataInfoEXT* pInfo, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetBufferOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkBufferCaptureDescriptorDataInfoEXT* pInfo, void* pData) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_GetImageOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkImageCaptureDescriptorDataInfoEXT* pInfo, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetImageOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkImageCaptureDescriptorDataInfoEXT* pInfo, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetImageOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkImageCaptureDescriptorDataInfoEXT* pInfo, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetImageOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkImageCaptureDescriptorDataInfoEXT* pInfo, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetImageOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkImageCaptureDescriptorDataInfoEXT* pInfo, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetImageOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkImageCaptureDescriptorDataInfoEXT* pInfo, void* pData) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_GetImageViewOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkImageViewCaptureDescriptorDataInfoEXT* pInfo, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetImageViewOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkImageViewCaptureDescriptorDataInfoEXT* pInfo, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetImageViewOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkImageViewCaptureDescriptorDataInfoEXT* pInfo, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetImageViewOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkImageViewCaptureDescriptorDataInfoEXT* pInfo, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetImageViewOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkImageViewCaptureDescriptorDataInfoEXT* pInfo, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetImageViewOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkImageViewCaptureDescriptorDataInfoEXT* pInfo, void* pData) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_GetSamplerOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkSamplerCaptureDescriptorDataInfoEXT* pInfo, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetSamplerOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkSamplerCaptureDescriptorDataInfoEXT* pInfo, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetSamplerOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkSamplerCaptureDescriptorDataInfoEXT* pInfo, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetSamplerOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkSamplerCaptureDescriptorDataInfoEXT* pInfo, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetSamplerOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkSamplerCaptureDescriptorDataInfoEXT* pInfo, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetSamplerOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkSamplerCaptureDescriptorDataInfoEXT* pInfo, void* pData) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_GetAccelerationStructureOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkAccelerationStructureCaptureDescriptorDataInfoEXT* pInfo, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetAccelerationStructureOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkAccelerationStructureCaptureDescriptorDataInfoEXT* pInfo, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetAccelerationStructureOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkAccelerationStructureCaptureDescriptorDataInfoEXT* pInfo, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetAccelerationStructureOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkAccelerationStructureCaptureDescriptorDataInfoEXT* pInfo, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetAccelerationStructureOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkAccelerationStructureCaptureDescriptorDataInfoEXT* pInfo, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetAccelerationStructureOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkAccelerationStructureCaptureDescriptorDataInfoEXT* pInfo, void* pData) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_SetDeviceMemoryPriorityEXT(VkDevice       device, VkDeviceMemory memory, float          priority) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_SetDeviceMemoryPriorityEXT(VkDevice       device, VkDeviceMemory memory, float          priority) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_SetDeviceMemoryPriorityEXT(VkDevice       device, VkDeviceMemory memory, float          priority) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_SetDeviceMemoryPriorityEXT(VkDevice       device, VkDeviceMemory memory, float          priority) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_SetDeviceMemoryPriorityEXT(VkDevice       device, VkDeviceMemory memory, float          priority) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_SetDeviceMemoryPriorityEXT(VkDevice       device, VkDeviceMemory memory, float          priority) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_WaitForPresentKHR(VkDevice device, VkSwapchainKHR swapchain, uint64_t presentId, uint64_t timeout) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_WaitForPresentKHR(VkDevice device, VkSwapchainKHR swapchain, uint64_t presentId, uint64_t timeout) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_WaitForPresentKHR(VkDevice device, VkSwapchainKHR swapchain, uint64_t presentId, uint64_t timeout) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_WaitForPresentKHR(VkDevice device, VkSwapchainKHR swapchain, uint64_t presentId, uint64_t timeout) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_WaitForPresentKHR(VkDevice device, VkSwapchainKHR swapchain, uint64_t presentId, uint64_t timeout) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_WaitForPresentKHR(VkDevice device, VkSwapchainKHR swapchain, uint64_t presentId, uint64_t timeout) ATTR_WEAK;


#ifdef VK_USE_PLATFORM_FUCHSIA
  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateBufferCollectionFUCHSIA(VkDevice device, const VkBufferCollectionCreateInfoFUCHSIA* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferCollectionFUCHSIA* pCollection) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_CreateBufferCollectionFUCHSIA(VkDevice device, const VkBufferCollectionCreateInfoFUCHSIA* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferCollectionFUCHSIA* pCollection) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_CreateBufferCollectionFUCHSIA(VkDevice device, const VkBufferCollectionCreateInfoFUCHSIA* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferCollectionFUCHSIA* pCollection) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_CreateBufferCollectionFUCHSIA(VkDevice device, const VkBufferCollectionCreateInfoFUCHSIA* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferCollectionFUCHSIA* pCollection) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_CreateBufferCollectionFUCHSIA(VkDevice device, const VkBufferCollectionCreateInfoFUCHSIA* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferCollectionFUCHSIA* pCollection) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_CreateBufferCollectionFUCHSIA(VkDevice device, const VkBufferCollectionCreateInfoFUCHSIA* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferCollectionFUCHSIA* pCollection) ATTR_WEAK;


#endif // VK_USE_PLATFORM_FUCHSIA
#ifdef VK_USE_PLATFORM_FUCHSIA
  VKAPI_ATTR VkResult VKAPI_CALL radv_SetBufferCollectionBufferConstraintsFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, const VkBufferConstraintsInfoFUCHSIA* pBufferConstraintsInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_SetBufferCollectionBufferConstraintsFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, const VkBufferConstraintsInfoFUCHSIA* pBufferConstraintsInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_SetBufferCollectionBufferConstraintsFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, const VkBufferConstraintsInfoFUCHSIA* pBufferConstraintsInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_SetBufferCollectionBufferConstraintsFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, const VkBufferConstraintsInfoFUCHSIA* pBufferConstraintsInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_SetBufferCollectionBufferConstraintsFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, const VkBufferConstraintsInfoFUCHSIA* pBufferConstraintsInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_SetBufferCollectionBufferConstraintsFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, const VkBufferConstraintsInfoFUCHSIA* pBufferConstraintsInfo) ATTR_WEAK;


#endif // VK_USE_PLATFORM_FUCHSIA
#ifdef VK_USE_PLATFORM_FUCHSIA
  VKAPI_ATTR VkResult VKAPI_CALL radv_SetBufferCollectionImageConstraintsFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, const VkImageConstraintsInfoFUCHSIA* pImageConstraintsInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_SetBufferCollectionImageConstraintsFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, const VkImageConstraintsInfoFUCHSIA* pImageConstraintsInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_SetBufferCollectionImageConstraintsFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, const VkImageConstraintsInfoFUCHSIA* pImageConstraintsInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_SetBufferCollectionImageConstraintsFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, const VkImageConstraintsInfoFUCHSIA* pImageConstraintsInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_SetBufferCollectionImageConstraintsFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, const VkImageConstraintsInfoFUCHSIA* pImageConstraintsInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_SetBufferCollectionImageConstraintsFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, const VkImageConstraintsInfoFUCHSIA* pImageConstraintsInfo) ATTR_WEAK;


#endif // VK_USE_PLATFORM_FUCHSIA
#ifdef VK_USE_PLATFORM_FUCHSIA
  VKAPI_ATTR void VKAPI_CALL radv_DestroyBufferCollectionFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_DestroyBufferCollectionFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_DestroyBufferCollectionFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_DestroyBufferCollectionFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_DestroyBufferCollectionFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_DestroyBufferCollectionFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


#endif // VK_USE_PLATFORM_FUCHSIA
#ifdef VK_USE_PLATFORM_FUCHSIA
  VKAPI_ATTR VkResult VKAPI_CALL radv_GetBufferCollectionPropertiesFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, VkBufferCollectionPropertiesFUCHSIA* pProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetBufferCollectionPropertiesFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, VkBufferCollectionPropertiesFUCHSIA* pProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetBufferCollectionPropertiesFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, VkBufferCollectionPropertiesFUCHSIA* pProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetBufferCollectionPropertiesFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, VkBufferCollectionPropertiesFUCHSIA* pProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetBufferCollectionPropertiesFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, VkBufferCollectionPropertiesFUCHSIA* pProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetBufferCollectionPropertiesFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, VkBufferCollectionPropertiesFUCHSIA* pProperties) ATTR_WEAK;


#endif // VK_USE_PLATFORM_FUCHSIA
  VKAPI_ATTR void VKAPI_CALL radv_CmdBeginRendering(VkCommandBuffer                   commandBuffer, const VkRenderingInfo*                              pRenderingInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdBeginRendering(VkCommandBuffer                   commandBuffer, const VkRenderingInfo*                              pRenderingInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdBeginRendering(VkCommandBuffer                   commandBuffer, const VkRenderingInfo*                              pRenderingInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdBeginRendering(VkCommandBuffer                   commandBuffer, const VkRenderingInfo*                              pRenderingInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdBeginRendering(VkCommandBuffer                   commandBuffer, const VkRenderingInfo*                              pRenderingInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdBeginRendering(VkCommandBuffer                   commandBuffer, const VkRenderingInfo*                              pRenderingInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdBeginRenderingKHR(VkCommandBuffer                   commandBuffer, const VkRenderingInfo*                              pRenderingInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdBeginRenderingKHR(VkCommandBuffer                   commandBuffer, const VkRenderingInfo*                              pRenderingInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdBeginRenderingKHR(VkCommandBuffer                   commandBuffer, const VkRenderingInfo*                              pRenderingInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdBeginRenderingKHR(VkCommandBuffer                   commandBuffer, const VkRenderingInfo*                              pRenderingInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdBeginRenderingKHR(VkCommandBuffer                   commandBuffer, const VkRenderingInfo*                              pRenderingInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdBeginRenderingKHR(VkCommandBuffer                   commandBuffer, const VkRenderingInfo*                              pRenderingInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdEndRendering(VkCommandBuffer                   commandBuffer) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdEndRendering(VkCommandBuffer                   commandBuffer) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdEndRendering(VkCommandBuffer                   commandBuffer) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdEndRendering(VkCommandBuffer                   commandBuffer) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdEndRendering(VkCommandBuffer                   commandBuffer) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdEndRendering(VkCommandBuffer                   commandBuffer) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdEndRenderingKHR(VkCommandBuffer                   commandBuffer) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdEndRenderingKHR(VkCommandBuffer                   commandBuffer) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdEndRenderingKHR(VkCommandBuffer                   commandBuffer) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdEndRenderingKHR(VkCommandBuffer                   commandBuffer) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdEndRenderingKHR(VkCommandBuffer                   commandBuffer) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdEndRenderingKHR(VkCommandBuffer                   commandBuffer) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_GetDescriptorSetLayoutHostMappingInfoVALVE(VkDevice device, const VkDescriptorSetBindingReferenceVALVE* pBindingReference, VkDescriptorSetLayoutHostMappingInfoVALVE* pHostMapping) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_GetDescriptorSetLayoutHostMappingInfoVALVE(VkDevice device, const VkDescriptorSetBindingReferenceVALVE* pBindingReference, VkDescriptorSetLayoutHostMappingInfoVALVE* pHostMapping) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_GetDescriptorSetLayoutHostMappingInfoVALVE(VkDevice device, const VkDescriptorSetBindingReferenceVALVE* pBindingReference, VkDescriptorSetLayoutHostMappingInfoVALVE* pHostMapping) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_GetDescriptorSetLayoutHostMappingInfoVALVE(VkDevice device, const VkDescriptorSetBindingReferenceVALVE* pBindingReference, VkDescriptorSetLayoutHostMappingInfoVALVE* pHostMapping) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_GetDescriptorSetLayoutHostMappingInfoVALVE(VkDevice device, const VkDescriptorSetBindingReferenceVALVE* pBindingReference, VkDescriptorSetLayoutHostMappingInfoVALVE* pHostMapping) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_GetDescriptorSetLayoutHostMappingInfoVALVE(VkDevice device, const VkDescriptorSetBindingReferenceVALVE* pBindingReference, VkDescriptorSetLayoutHostMappingInfoVALVE* pHostMapping) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_GetDescriptorSetHostMappingVALVE(VkDevice device, VkDescriptorSet descriptorSet, void** ppData) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_GetDescriptorSetHostMappingVALVE(VkDevice device, VkDescriptorSet descriptorSet, void** ppData) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_GetDescriptorSetHostMappingVALVE(VkDevice device, VkDescriptorSet descriptorSet, void** ppData) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_GetDescriptorSetHostMappingVALVE(VkDevice device, VkDescriptorSet descriptorSet, void** ppData) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_GetDescriptorSetHostMappingVALVE(VkDevice device, VkDescriptorSet descriptorSet, void** ppData) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_GetDescriptorSetHostMappingVALVE(VkDevice device, VkDescriptorSet descriptorSet, void** ppData) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateMicromapEXT(VkDevice                                           device, const VkMicromapCreateInfoEXT*        pCreateInfo, const VkAllocationCallbacks*       pAllocator, VkMicromapEXT*                        pMicromap) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_CreateMicromapEXT(VkDevice                                           device, const VkMicromapCreateInfoEXT*        pCreateInfo, const VkAllocationCallbacks*       pAllocator, VkMicromapEXT*                        pMicromap) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_CreateMicromapEXT(VkDevice                                           device, const VkMicromapCreateInfoEXT*        pCreateInfo, const VkAllocationCallbacks*       pAllocator, VkMicromapEXT*                        pMicromap) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_CreateMicromapEXT(VkDevice                                           device, const VkMicromapCreateInfoEXT*        pCreateInfo, const VkAllocationCallbacks*       pAllocator, VkMicromapEXT*                        pMicromap) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_CreateMicromapEXT(VkDevice                                           device, const VkMicromapCreateInfoEXT*        pCreateInfo, const VkAllocationCallbacks*       pAllocator, VkMicromapEXT*                        pMicromap) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_CreateMicromapEXT(VkDevice                                           device, const VkMicromapCreateInfoEXT*        pCreateInfo, const VkAllocationCallbacks*       pAllocator, VkMicromapEXT*                        pMicromap) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdBuildMicromapsEXT(VkCommandBuffer                                    commandBuffer, uint32_t infoCount, const VkMicromapBuildInfoEXT* pInfos) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdBuildMicromapsEXT(VkCommandBuffer                                    commandBuffer, uint32_t infoCount, const VkMicromapBuildInfoEXT* pInfos) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdBuildMicromapsEXT(VkCommandBuffer                                    commandBuffer, uint32_t infoCount, const VkMicromapBuildInfoEXT* pInfos) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdBuildMicromapsEXT(VkCommandBuffer                                    commandBuffer, uint32_t infoCount, const VkMicromapBuildInfoEXT* pInfos) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdBuildMicromapsEXT(VkCommandBuffer                                    commandBuffer, uint32_t infoCount, const VkMicromapBuildInfoEXT* pInfos) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdBuildMicromapsEXT(VkCommandBuffer                                    commandBuffer, uint32_t infoCount, const VkMicromapBuildInfoEXT* pInfos) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_BuildMicromapsEXT(VkDevice                                           device, VkDeferredOperationKHR deferredOperation, uint32_t infoCount, const VkMicromapBuildInfoEXT* pInfos) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_BuildMicromapsEXT(VkDevice                                           device, VkDeferredOperationKHR deferredOperation, uint32_t infoCount, const VkMicromapBuildInfoEXT* pInfos) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_BuildMicromapsEXT(VkDevice                                           device, VkDeferredOperationKHR deferredOperation, uint32_t infoCount, const VkMicromapBuildInfoEXT* pInfos) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_BuildMicromapsEXT(VkDevice                                           device, VkDeferredOperationKHR deferredOperation, uint32_t infoCount, const VkMicromapBuildInfoEXT* pInfos) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_BuildMicromapsEXT(VkDevice                                           device, VkDeferredOperationKHR deferredOperation, uint32_t infoCount, const VkMicromapBuildInfoEXT* pInfos) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_BuildMicromapsEXT(VkDevice                                           device, VkDeferredOperationKHR deferredOperation, uint32_t infoCount, const VkMicromapBuildInfoEXT* pInfos) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_DestroyMicromapEXT(VkDevice device, VkMicromapEXT micromap, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_DestroyMicromapEXT(VkDevice device, VkMicromapEXT micromap, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_DestroyMicromapEXT(VkDevice device, VkMicromapEXT micromap, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_DestroyMicromapEXT(VkDevice device, VkMicromapEXT micromap, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_DestroyMicromapEXT(VkDevice device, VkMicromapEXT micromap, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_DestroyMicromapEXT(VkDevice device, VkMicromapEXT micromap, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdCopyMicromapEXT(VkCommandBuffer commandBuffer, const VkCopyMicromapInfoEXT* pInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdCopyMicromapEXT(VkCommandBuffer commandBuffer, const VkCopyMicromapInfoEXT* pInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdCopyMicromapEXT(VkCommandBuffer commandBuffer, const VkCopyMicromapInfoEXT* pInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdCopyMicromapEXT(VkCommandBuffer commandBuffer, const VkCopyMicromapInfoEXT* pInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdCopyMicromapEXT(VkCommandBuffer commandBuffer, const VkCopyMicromapInfoEXT* pInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdCopyMicromapEXT(VkCommandBuffer commandBuffer, const VkCopyMicromapInfoEXT* pInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_CopyMicromapEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMicromapInfoEXT* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_CopyMicromapEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMicromapInfoEXT* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_CopyMicromapEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMicromapInfoEXT* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_CopyMicromapEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMicromapInfoEXT* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_CopyMicromapEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMicromapInfoEXT* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_CopyMicromapEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMicromapInfoEXT* pInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdCopyMicromapToMemoryEXT(VkCommandBuffer commandBuffer, const VkCopyMicromapToMemoryInfoEXT* pInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdCopyMicromapToMemoryEXT(VkCommandBuffer commandBuffer, const VkCopyMicromapToMemoryInfoEXT* pInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdCopyMicromapToMemoryEXT(VkCommandBuffer commandBuffer, const VkCopyMicromapToMemoryInfoEXT* pInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdCopyMicromapToMemoryEXT(VkCommandBuffer commandBuffer, const VkCopyMicromapToMemoryInfoEXT* pInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdCopyMicromapToMemoryEXT(VkCommandBuffer commandBuffer, const VkCopyMicromapToMemoryInfoEXT* pInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdCopyMicromapToMemoryEXT(VkCommandBuffer commandBuffer, const VkCopyMicromapToMemoryInfoEXT* pInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_CopyMicromapToMemoryEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMicromapToMemoryInfoEXT* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_CopyMicromapToMemoryEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMicromapToMemoryInfoEXT* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_CopyMicromapToMemoryEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMicromapToMemoryInfoEXT* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_CopyMicromapToMemoryEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMicromapToMemoryInfoEXT* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_CopyMicromapToMemoryEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMicromapToMemoryInfoEXT* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_CopyMicromapToMemoryEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMicromapToMemoryInfoEXT* pInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdCopyMemoryToMicromapEXT(VkCommandBuffer commandBuffer, const VkCopyMemoryToMicromapInfoEXT* pInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdCopyMemoryToMicromapEXT(VkCommandBuffer commandBuffer, const VkCopyMemoryToMicromapInfoEXT* pInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdCopyMemoryToMicromapEXT(VkCommandBuffer commandBuffer, const VkCopyMemoryToMicromapInfoEXT* pInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdCopyMemoryToMicromapEXT(VkCommandBuffer commandBuffer, const VkCopyMemoryToMicromapInfoEXT* pInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdCopyMemoryToMicromapEXT(VkCommandBuffer commandBuffer, const VkCopyMemoryToMicromapInfoEXT* pInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdCopyMemoryToMicromapEXT(VkCommandBuffer commandBuffer, const VkCopyMemoryToMicromapInfoEXT* pInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_CopyMemoryToMicromapEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMemoryToMicromapInfoEXT* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_CopyMemoryToMicromapEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMemoryToMicromapInfoEXT* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_CopyMemoryToMicromapEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMemoryToMicromapInfoEXT* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_CopyMemoryToMicromapEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMemoryToMicromapInfoEXT* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_CopyMemoryToMicromapEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMemoryToMicromapInfoEXT* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_CopyMemoryToMicromapEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMemoryToMicromapInfoEXT* pInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdWriteMicromapsPropertiesEXT(VkCommandBuffer commandBuffer, uint32_t micromapCount, const VkMicromapEXT* pMicromaps, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdWriteMicromapsPropertiesEXT(VkCommandBuffer commandBuffer, uint32_t micromapCount, const VkMicromapEXT* pMicromaps, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdWriteMicromapsPropertiesEXT(VkCommandBuffer commandBuffer, uint32_t micromapCount, const VkMicromapEXT* pMicromaps, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdWriteMicromapsPropertiesEXT(VkCommandBuffer commandBuffer, uint32_t micromapCount, const VkMicromapEXT* pMicromaps, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdWriteMicromapsPropertiesEXT(VkCommandBuffer commandBuffer, uint32_t micromapCount, const VkMicromapEXT* pMicromaps, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdWriteMicromapsPropertiesEXT(VkCommandBuffer commandBuffer, uint32_t micromapCount, const VkMicromapEXT* pMicromaps, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_WriteMicromapsPropertiesEXT(VkDevice device, uint32_t micromapCount, const VkMicromapEXT* pMicromaps, VkQueryType  queryType, size_t       dataSize, void* pData, size_t stride) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_WriteMicromapsPropertiesEXT(VkDevice device, uint32_t micromapCount, const VkMicromapEXT* pMicromaps, VkQueryType  queryType, size_t       dataSize, void* pData, size_t stride) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_WriteMicromapsPropertiesEXT(VkDevice device, uint32_t micromapCount, const VkMicromapEXT* pMicromaps, VkQueryType  queryType, size_t       dataSize, void* pData, size_t stride) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_WriteMicromapsPropertiesEXT(VkDevice device, uint32_t micromapCount, const VkMicromapEXT* pMicromaps, VkQueryType  queryType, size_t       dataSize, void* pData, size_t stride) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_WriteMicromapsPropertiesEXT(VkDevice device, uint32_t micromapCount, const VkMicromapEXT* pMicromaps, VkQueryType  queryType, size_t       dataSize, void* pData, size_t stride) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_WriteMicromapsPropertiesEXT(VkDevice device, uint32_t micromapCount, const VkMicromapEXT* pMicromaps, VkQueryType  queryType, size_t       dataSize, void* pData, size_t stride) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_GetDeviceMicromapCompatibilityEXT(VkDevice device, const VkMicromapVersionInfoEXT* pVersionInfo, VkAccelerationStructureCompatibilityKHR* pCompatibility) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_GetDeviceMicromapCompatibilityEXT(VkDevice device, const VkMicromapVersionInfoEXT* pVersionInfo, VkAccelerationStructureCompatibilityKHR* pCompatibility) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_GetDeviceMicromapCompatibilityEXT(VkDevice device, const VkMicromapVersionInfoEXT* pVersionInfo, VkAccelerationStructureCompatibilityKHR* pCompatibility) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_GetDeviceMicromapCompatibilityEXT(VkDevice device, const VkMicromapVersionInfoEXT* pVersionInfo, VkAccelerationStructureCompatibilityKHR* pCompatibility) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_GetDeviceMicromapCompatibilityEXT(VkDevice device, const VkMicromapVersionInfoEXT* pVersionInfo, VkAccelerationStructureCompatibilityKHR* pCompatibility) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_GetDeviceMicromapCompatibilityEXT(VkDevice device, const VkMicromapVersionInfoEXT* pVersionInfo, VkAccelerationStructureCompatibilityKHR* pCompatibility) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_GetMicromapBuildSizesEXT(VkDevice                                            device, VkAccelerationStructureBuildTypeKHR                 buildType, const VkMicromapBuildInfoEXT*  pBuildInfo, VkMicromapBuildSizesInfoEXT*           pSizeInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_GetMicromapBuildSizesEXT(VkDevice                                            device, VkAccelerationStructureBuildTypeKHR                 buildType, const VkMicromapBuildInfoEXT*  pBuildInfo, VkMicromapBuildSizesInfoEXT*           pSizeInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_GetMicromapBuildSizesEXT(VkDevice                                            device, VkAccelerationStructureBuildTypeKHR                 buildType, const VkMicromapBuildInfoEXT*  pBuildInfo, VkMicromapBuildSizesInfoEXT*           pSizeInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_GetMicromapBuildSizesEXT(VkDevice                                            device, VkAccelerationStructureBuildTypeKHR                 buildType, const VkMicromapBuildInfoEXT*  pBuildInfo, VkMicromapBuildSizesInfoEXT*           pSizeInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_GetMicromapBuildSizesEXT(VkDevice                                            device, VkAccelerationStructureBuildTypeKHR                 buildType, const VkMicromapBuildInfoEXT*  pBuildInfo, VkMicromapBuildSizesInfoEXT*           pSizeInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_GetMicromapBuildSizesEXT(VkDevice                                            device, VkAccelerationStructureBuildTypeKHR                 buildType, const VkMicromapBuildInfoEXT*  pBuildInfo, VkMicromapBuildSizesInfoEXT*           pSizeInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_GetShaderModuleIdentifierEXT(VkDevice device, VkShaderModule shaderModule, VkShaderModuleIdentifierEXT* pIdentifier) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_GetShaderModuleIdentifierEXT(VkDevice device, VkShaderModule shaderModule, VkShaderModuleIdentifierEXT* pIdentifier) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_GetShaderModuleIdentifierEXT(VkDevice device, VkShaderModule shaderModule, VkShaderModuleIdentifierEXT* pIdentifier) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_GetShaderModuleIdentifierEXT(VkDevice device, VkShaderModule shaderModule, VkShaderModuleIdentifierEXT* pIdentifier) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_GetShaderModuleIdentifierEXT(VkDevice device, VkShaderModule shaderModule, VkShaderModuleIdentifierEXT* pIdentifier) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_GetShaderModuleIdentifierEXT(VkDevice device, VkShaderModule shaderModule, VkShaderModuleIdentifierEXT* pIdentifier) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_GetShaderModuleCreateInfoIdentifierEXT(VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, VkShaderModuleIdentifierEXT* pIdentifier) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_GetShaderModuleCreateInfoIdentifierEXT(VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, VkShaderModuleIdentifierEXT* pIdentifier) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_GetShaderModuleCreateInfoIdentifierEXT(VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, VkShaderModuleIdentifierEXT* pIdentifier) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_GetShaderModuleCreateInfoIdentifierEXT(VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, VkShaderModuleIdentifierEXT* pIdentifier) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_GetShaderModuleCreateInfoIdentifierEXT(VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, VkShaderModuleIdentifierEXT* pIdentifier) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_GetShaderModuleCreateInfoIdentifierEXT(VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, VkShaderModuleIdentifierEXT* pIdentifier) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_GetImageSubresourceLayout2EXT(VkDevice device, VkImage image, const VkImageSubresource2EXT* pSubresource, VkSubresourceLayout2EXT* pLayout) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_GetImageSubresourceLayout2EXT(VkDevice device, VkImage image, const VkImageSubresource2EXT* pSubresource, VkSubresourceLayout2EXT* pLayout) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_GetImageSubresourceLayout2EXT(VkDevice device, VkImage image, const VkImageSubresource2EXT* pSubresource, VkSubresourceLayout2EXT* pLayout) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_GetImageSubresourceLayout2EXT(VkDevice device, VkImage image, const VkImageSubresource2EXT* pSubresource, VkSubresourceLayout2EXT* pLayout) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_GetImageSubresourceLayout2EXT(VkDevice device, VkImage image, const VkImageSubresource2EXT* pSubresource, VkSubresourceLayout2EXT* pLayout) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_GetImageSubresourceLayout2EXT(VkDevice device, VkImage image, const VkImageSubresource2EXT* pSubresource, VkSubresourceLayout2EXT* pLayout) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_GetPipelinePropertiesEXT(VkDevice device, const VkPipelineInfoEXT* pPipelineInfo, VkBaseOutStructure* pPipelineProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetPipelinePropertiesEXT(VkDevice device, const VkPipelineInfoEXT* pPipelineInfo, VkBaseOutStructure* pPipelineProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetPipelinePropertiesEXT(VkDevice device, const VkPipelineInfoEXT* pPipelineInfo, VkBaseOutStructure* pPipelineProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetPipelinePropertiesEXT(VkDevice device, const VkPipelineInfoEXT* pPipelineInfo, VkBaseOutStructure* pPipelineProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetPipelinePropertiesEXT(VkDevice device, const VkPipelineInfoEXT* pPipelineInfo, VkBaseOutStructure* pPipelineProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetPipelinePropertiesEXT(VkDevice device, const VkPipelineInfoEXT* pPipelineInfo, VkBaseOutStructure* pPipelineProperties) ATTR_WEAK;


#ifdef VK_USE_PLATFORM_METAL_EXT
  VKAPI_ATTR void VKAPI_CALL radv_ExportMetalObjectsEXT(VkDevice device, VkExportMetalObjectsInfoEXT* pMetalObjectsInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_ExportMetalObjectsEXT(VkDevice device, VkExportMetalObjectsInfoEXT* pMetalObjectsInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_ExportMetalObjectsEXT(VkDevice device, VkExportMetalObjectsInfoEXT* pMetalObjectsInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_ExportMetalObjectsEXT(VkDevice device, VkExportMetalObjectsInfoEXT* pMetalObjectsInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_ExportMetalObjectsEXT(VkDevice device, VkExportMetalObjectsInfoEXT* pMetalObjectsInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_ExportMetalObjectsEXT(VkDevice device, VkExportMetalObjectsInfoEXT* pMetalObjectsInfo) ATTR_WEAK;


#endif // VK_USE_PLATFORM_METAL_EXT
  VKAPI_ATTR VkResult VKAPI_CALL radv_GetFramebufferTilePropertiesQCOM(VkDevice device, VkFramebuffer framebuffer, uint32_t* pPropertiesCount, VkTilePropertiesQCOM* pProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetFramebufferTilePropertiesQCOM(VkDevice device, VkFramebuffer framebuffer, uint32_t* pPropertiesCount, VkTilePropertiesQCOM* pProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetFramebufferTilePropertiesQCOM(VkDevice device, VkFramebuffer framebuffer, uint32_t* pPropertiesCount, VkTilePropertiesQCOM* pProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetFramebufferTilePropertiesQCOM(VkDevice device, VkFramebuffer framebuffer, uint32_t* pPropertiesCount, VkTilePropertiesQCOM* pProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetFramebufferTilePropertiesQCOM(VkDevice device, VkFramebuffer framebuffer, uint32_t* pPropertiesCount, VkTilePropertiesQCOM* pProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetFramebufferTilePropertiesQCOM(VkDevice device, VkFramebuffer framebuffer, uint32_t* pPropertiesCount, VkTilePropertiesQCOM* pProperties) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_GetDynamicRenderingTilePropertiesQCOM(VkDevice device, const VkRenderingInfo* pRenderingInfo, VkTilePropertiesQCOM* pProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetDynamicRenderingTilePropertiesQCOM(VkDevice device, const VkRenderingInfo* pRenderingInfo, VkTilePropertiesQCOM* pProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetDynamicRenderingTilePropertiesQCOM(VkDevice device, const VkRenderingInfo* pRenderingInfo, VkTilePropertiesQCOM* pProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetDynamicRenderingTilePropertiesQCOM(VkDevice device, const VkRenderingInfo* pRenderingInfo, VkTilePropertiesQCOM* pProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetDynamicRenderingTilePropertiesQCOM(VkDevice device, const VkRenderingInfo* pRenderingInfo, VkTilePropertiesQCOM* pProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetDynamicRenderingTilePropertiesQCOM(VkDevice device, const VkRenderingInfo* pRenderingInfo, VkTilePropertiesQCOM* pProperties) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateOpticalFlowSessionNV(VkDevice device, const VkOpticalFlowSessionCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkOpticalFlowSessionNV* pSession) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_CreateOpticalFlowSessionNV(VkDevice device, const VkOpticalFlowSessionCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkOpticalFlowSessionNV* pSession) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_CreateOpticalFlowSessionNV(VkDevice device, const VkOpticalFlowSessionCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkOpticalFlowSessionNV* pSession) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_CreateOpticalFlowSessionNV(VkDevice device, const VkOpticalFlowSessionCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkOpticalFlowSessionNV* pSession) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_CreateOpticalFlowSessionNV(VkDevice device, const VkOpticalFlowSessionCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkOpticalFlowSessionNV* pSession) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_CreateOpticalFlowSessionNV(VkDevice device, const VkOpticalFlowSessionCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkOpticalFlowSessionNV* pSession) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_DestroyOpticalFlowSessionNV(VkDevice device, VkOpticalFlowSessionNV session, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_DestroyOpticalFlowSessionNV(VkDevice device, VkOpticalFlowSessionNV session, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_DestroyOpticalFlowSessionNV(VkDevice device, VkOpticalFlowSessionNV session, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_DestroyOpticalFlowSessionNV(VkDevice device, VkOpticalFlowSessionNV session, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_DestroyOpticalFlowSessionNV(VkDevice device, VkOpticalFlowSessionNV session, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_DestroyOpticalFlowSessionNV(VkDevice device, VkOpticalFlowSessionNV session, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_BindOpticalFlowSessionImageNV(VkDevice device, VkOpticalFlowSessionNV session, VkOpticalFlowSessionBindingPointNV bindingPoint, VkImageView view, VkImageLayout layout) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_BindOpticalFlowSessionImageNV(VkDevice device, VkOpticalFlowSessionNV session, VkOpticalFlowSessionBindingPointNV bindingPoint, VkImageView view, VkImageLayout layout) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_BindOpticalFlowSessionImageNV(VkDevice device, VkOpticalFlowSessionNV session, VkOpticalFlowSessionBindingPointNV bindingPoint, VkImageView view, VkImageLayout layout) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_BindOpticalFlowSessionImageNV(VkDevice device, VkOpticalFlowSessionNV session, VkOpticalFlowSessionBindingPointNV bindingPoint, VkImageView view, VkImageLayout layout) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_BindOpticalFlowSessionImageNV(VkDevice device, VkOpticalFlowSessionNV session, VkOpticalFlowSessionBindingPointNV bindingPoint, VkImageView view, VkImageLayout layout) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_BindOpticalFlowSessionImageNV(VkDevice device, VkOpticalFlowSessionNV session, VkOpticalFlowSessionBindingPointNV bindingPoint, VkImageView view, VkImageLayout layout) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdOpticalFlowExecuteNV(VkCommandBuffer commandBuffer, VkOpticalFlowSessionNV session, const VkOpticalFlowExecuteInfoNV* pExecuteInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdOpticalFlowExecuteNV(VkCommandBuffer commandBuffer, VkOpticalFlowSessionNV session, const VkOpticalFlowExecuteInfoNV* pExecuteInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdOpticalFlowExecuteNV(VkCommandBuffer commandBuffer, VkOpticalFlowSessionNV session, const VkOpticalFlowExecuteInfoNV* pExecuteInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdOpticalFlowExecuteNV(VkCommandBuffer commandBuffer, VkOpticalFlowSessionNV session, const VkOpticalFlowExecuteInfoNV* pExecuteInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdOpticalFlowExecuteNV(VkCommandBuffer commandBuffer, VkOpticalFlowSessionNV session, const VkOpticalFlowExecuteInfoNV* pExecuteInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdOpticalFlowExecuteNV(VkCommandBuffer commandBuffer, VkOpticalFlowSessionNV session, const VkOpticalFlowExecuteInfoNV* pExecuteInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_GetDeviceFaultInfoEXT(VkDevice device, VkDeviceFaultCountsEXT* pFaultCounts, VkDeviceFaultInfoEXT* pFaultInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetDeviceFaultInfoEXT(VkDevice device, VkDeviceFaultCountsEXT* pFaultCounts, VkDeviceFaultInfoEXT* pFaultInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetDeviceFaultInfoEXT(VkDevice device, VkDeviceFaultCountsEXT* pFaultCounts, VkDeviceFaultInfoEXT* pFaultInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetDeviceFaultInfoEXT(VkDevice device, VkDeviceFaultCountsEXT* pFaultCounts, VkDeviceFaultInfoEXT* pFaultInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetDeviceFaultInfoEXT(VkDevice device, VkDeviceFaultCountsEXT* pFaultCounts, VkDeviceFaultInfoEXT* pFaultInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetDeviceFaultInfoEXT(VkDevice device, VkDeviceFaultCountsEXT* pFaultCounts, VkDeviceFaultInfoEXT* pFaultInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_ReleaseSwapchainImagesEXT(VkDevice device, const VkReleaseSwapchainImagesInfoEXT* pReleaseInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_ReleaseSwapchainImagesEXT(VkDevice device, const VkReleaseSwapchainImagesInfoEXT* pReleaseInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_ReleaseSwapchainImagesEXT(VkDevice device, const VkReleaseSwapchainImagesInfoEXT* pReleaseInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_ReleaseSwapchainImagesEXT(VkDevice device, const VkReleaseSwapchainImagesInfoEXT* pReleaseInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_ReleaseSwapchainImagesEXT(VkDevice device, const VkReleaseSwapchainImagesInfoEXT* pReleaseInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_ReleaseSwapchainImagesEXT(VkDevice device, const VkReleaseSwapchainImagesInfoEXT* pReleaseInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_MapMemory2KHR(VkDevice device, const VkMemoryMapInfoKHR* pMemoryMapInfo, void** ppData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_MapMemory2KHR(VkDevice device, const VkMemoryMapInfoKHR* pMemoryMapInfo, void** ppData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_MapMemory2KHR(VkDevice device, const VkMemoryMapInfoKHR* pMemoryMapInfo, void** ppData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_MapMemory2KHR(VkDevice device, const VkMemoryMapInfoKHR* pMemoryMapInfo, void** ppData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_MapMemory2KHR(VkDevice device, const VkMemoryMapInfoKHR* pMemoryMapInfo, void** ppData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_MapMemory2KHR(VkDevice device, const VkMemoryMapInfoKHR* pMemoryMapInfo, void** ppData) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_UnmapMemory2KHR(VkDevice device, const VkMemoryUnmapInfoKHR* pMemoryUnmapInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_UnmapMemory2KHR(VkDevice device, const VkMemoryUnmapInfoKHR* pMemoryUnmapInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_UnmapMemory2KHR(VkDevice device, const VkMemoryUnmapInfoKHR* pMemoryUnmapInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_UnmapMemory2KHR(VkDevice device, const VkMemoryUnmapInfoKHR* pMemoryUnmapInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_UnmapMemory2KHR(VkDevice device, const VkMemoryUnmapInfoKHR* pMemoryUnmapInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_UnmapMemory2KHR(VkDevice device, const VkMemoryUnmapInfoKHR* pMemoryUnmapInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_CreateShadersEXT(VkDevice device, uint32_t createInfoCount, const VkShaderCreateInfoEXT* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkShaderEXT* pShaders) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_CreateShadersEXT(VkDevice device, uint32_t createInfoCount, const VkShaderCreateInfoEXT* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkShaderEXT* pShaders) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_CreateShadersEXT(VkDevice device, uint32_t createInfoCount, const VkShaderCreateInfoEXT* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkShaderEXT* pShaders) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_CreateShadersEXT(VkDevice device, uint32_t createInfoCount, const VkShaderCreateInfoEXT* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkShaderEXT* pShaders) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_CreateShadersEXT(VkDevice device, uint32_t createInfoCount, const VkShaderCreateInfoEXT* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkShaderEXT* pShaders) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_CreateShadersEXT(VkDevice device, uint32_t createInfoCount, const VkShaderCreateInfoEXT* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkShaderEXT* pShaders) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_DestroyShaderEXT(VkDevice device, VkShaderEXT shader, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_DestroyShaderEXT(VkDevice device, VkShaderEXT shader, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_DestroyShaderEXT(VkDevice device, VkShaderEXT shader, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_DestroyShaderEXT(VkDevice device, VkShaderEXT shader, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_DestroyShaderEXT(VkDevice device, VkShaderEXT shader, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_DestroyShaderEXT(VkDevice device, VkShaderEXT shader, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL radv_GetShaderBinaryDataEXT(VkDevice device, VkShaderEXT shader, size_t* pDataSize, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL sqtt_GetShaderBinaryDataEXT(VkDevice device, VkShaderEXT shader, size_t* pDataSize, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rra_GetShaderBinaryDataEXT(VkDevice device, VkShaderEXT shader, size_t* pDataSize, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rmv_GetShaderBinaryDataEXT(VkDevice device, VkShaderEXT shader, size_t* pDataSize, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL metro_exodus_GetShaderBinaryDataEXT(VkDevice device, VkShaderEXT shader, size_t* pDataSize, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL rage2_GetShaderBinaryDataEXT(VkDevice device, VkShaderEXT shader, size_t* pDataSize, void* pData) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL radv_CmdBindShadersEXT(VkCommandBuffer commandBuffer, uint32_t stageCount, const VkShaderStageFlagBits* pStages, const VkShaderEXT* pShaders) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL sqtt_CmdBindShadersEXT(VkCommandBuffer commandBuffer, uint32_t stageCount, const VkShaderStageFlagBits* pStages, const VkShaderEXT* pShaders) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rra_CmdBindShadersEXT(VkCommandBuffer commandBuffer, uint32_t stageCount, const VkShaderStageFlagBits* pStages, const VkShaderEXT* pShaders) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rmv_CmdBindShadersEXT(VkCommandBuffer commandBuffer, uint32_t stageCount, const VkShaderStageFlagBits* pStages, const VkShaderEXT* pShaders) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL metro_exodus_CmdBindShadersEXT(VkCommandBuffer commandBuffer, uint32_t stageCount, const VkShaderStageFlagBits* pStages, const VkShaderEXT* pShaders) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL rage2_CmdBindShadersEXT(VkCommandBuffer commandBuffer, uint32_t stageCount, const VkShaderStageFlagBits* pStages, const VkShaderEXT* pShaders) ATTR_WEAK;



#ifdef __cplusplus
}
#endif

#endif /* RADV_ENTRYPOINTS_H */
