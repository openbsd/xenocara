
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


#ifndef VK_CMD_ENQUEUE_ENTRYPOINTS_H
#define VK_CMD_ENQUEUE_ENTRYPOINTS_H

#ifdef __cplusplus
extern "C" {
#endif

/* clang wants function declarations in the header to have weak attribute */
#if !defined(_MSC_VER) && !defined(__MINGW32__)
#define ATTR_WEAK __attribute__ ((weak))
#else
#define ATTR_WEAK
#endif

extern const struct vk_instance_entrypoint_table vk_cmd_enqueue_instance_entrypoints;
extern const struct vk_instance_entrypoint_table vk_cmd_enqueue_unless_primary_instance_entrypoints;

extern const struct vk_physical_device_entrypoint_table vk_cmd_enqueue_physical_device_entrypoints;
extern const struct vk_physical_device_entrypoint_table vk_cmd_enqueue_unless_primary_physical_device_entrypoints;

extern const struct vk_device_entrypoint_table vk_cmd_enqueue_device_entrypoints;
extern const struct vk_device_entrypoint_table vk_cmd_enqueue_unless_primary_device_entrypoints;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateInstance(const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateInstance(const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_DestroyInstance(VkInstance instance, const VkAllocationCallbacks* pAllocator);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_DestroyInstance(VkInstance instance, const VkAllocationCallbacks* pAllocator);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_EnumeratePhysicalDevices(VkInstance instance, uint32_t* pPhysicalDeviceCount, VkPhysicalDevice* pPhysicalDevices);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_EnumeratePhysicalDevices(VkInstance instance, uint32_t* pPhysicalDeviceCount, VkPhysicalDevice* pPhysicalDevices);
  VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vk_cmd_enqueue_GetInstanceProcAddr(VkInstance instance, const char* pName);
  VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vk_cmd_enqueue_unless_primary_GetInstanceProcAddr(VkInstance instance, const char* pName);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_EnumerateInstanceVersion(uint32_t* pApiVersion);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_EnumerateInstanceVersion(uint32_t* pApiVersion);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_EnumerateInstanceLayerProperties(uint32_t* pPropertyCount, VkLayerProperties* pProperties);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_EnumerateInstanceLayerProperties(uint32_t* pPropertyCount, VkLayerProperties* pProperties);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_EnumerateInstanceExtensionProperties(const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_EnumerateInstanceExtensionProperties(const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties);
#ifdef VK_USE_PLATFORM_ANDROID_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateAndroidSurfaceKHR(VkInstance instance, const VkAndroidSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateAndroidSurfaceKHR(VkInstance instance, const VkAndroidSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_ANDROID_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateDisplayPlaneSurfaceKHR(VkInstance instance, const VkDisplaySurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateDisplayPlaneSurfaceKHR(VkInstance instance, const VkDisplaySurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_DestroySurfaceKHR(VkInstance instance, VkSurfaceKHR surface, const VkAllocationCallbacks* pAllocator);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_DestroySurfaceKHR(VkInstance instance, VkSurfaceKHR surface, const VkAllocationCallbacks* pAllocator);
#ifdef VK_USE_PLATFORM_VI_NN
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateViSurfaceNN(VkInstance instance, const VkViSurfaceCreateInfoNN* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateViSurfaceNN(VkInstance instance, const VkViSurfaceCreateInfoNN* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_VI_NN
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateWaylandSurfaceKHR(VkInstance instance, const VkWaylandSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateWaylandSurfaceKHR(VkInstance instance, const VkWaylandSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_WAYLAND_KHR
#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateWin32SurfaceKHR(VkInstance instance, const VkWin32SurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateWin32SurfaceKHR(VkInstance instance, const VkWin32SurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_WIN32_KHR
#ifdef VK_USE_PLATFORM_XLIB_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateXlibSurfaceKHR(VkInstance instance, const VkXlibSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateXlibSurfaceKHR(VkInstance instance, const VkXlibSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_XLIB_KHR
#ifdef VK_USE_PLATFORM_XCB_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateXcbSurfaceKHR(VkInstance instance, const VkXcbSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateXcbSurfaceKHR(VkInstance instance, const VkXcbSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_XCB_KHR
#ifdef VK_USE_PLATFORM_DIRECTFB_EXT
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateDirectFBSurfaceEXT(VkInstance instance, const VkDirectFBSurfaceCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateDirectFBSurfaceEXT(VkInstance instance, const VkDirectFBSurfaceCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_DIRECTFB_EXT
#ifdef VK_USE_PLATFORM_FUCHSIA
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateImagePipeSurfaceFUCHSIA(VkInstance instance, const VkImagePipeSurfaceCreateInfoFUCHSIA* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateImagePipeSurfaceFUCHSIA(VkInstance instance, const VkImagePipeSurfaceCreateInfoFUCHSIA* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_FUCHSIA
#ifdef VK_USE_PLATFORM_GGP
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateStreamDescriptorSurfaceGGP(VkInstance instance, const VkStreamDescriptorSurfaceCreateInfoGGP* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateStreamDescriptorSurfaceGGP(VkInstance instance, const VkStreamDescriptorSurfaceCreateInfoGGP* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_GGP
#ifdef VK_USE_PLATFORM_SCREEN_QNX
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateScreenSurfaceQNX(VkInstance instance, const VkScreenSurfaceCreateInfoQNX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateScreenSurfaceQNX(VkInstance instance, const VkScreenSurfaceCreateInfoQNX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_SCREEN_QNX
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_DebugReportMessageEXT(VkInstance instance, VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_DebugReportMessageEXT(VkInstance instance, VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_EnumeratePhysicalDeviceGroups(VkInstance instance, uint32_t* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_EnumeratePhysicalDeviceGroups(VkInstance instance, uint32_t* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_EnumeratePhysicalDeviceGroupsKHR(VkInstance instance, uint32_t* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_EnumeratePhysicalDeviceGroupsKHR(VkInstance instance, uint32_t* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties);
#ifdef VK_USE_PLATFORM_IOS_MVK
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateIOSSurfaceMVK(VkInstance instance, const VkIOSSurfaceCreateInfoMVK* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateIOSSurfaceMVK(VkInstance instance, const VkIOSSurfaceCreateInfoMVK* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_IOS_MVK
#ifdef VK_USE_PLATFORM_MACOS_MVK
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateMacOSSurfaceMVK(VkInstance instance, const VkMacOSSurfaceCreateInfoMVK* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateMacOSSurfaceMVK(VkInstance instance, const VkMacOSSurfaceCreateInfoMVK* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_MACOS_MVK
#ifdef VK_USE_PLATFORM_METAL_EXT
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateMetalSurfaceEXT(VkInstance instance, const VkMetalSurfaceCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateMetalSurfaceEXT(VkInstance instance, const VkMetalSurfaceCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_METAL_EXT
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pMessenger);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pMessenger);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT messenger, const VkAllocationCallbacks* pAllocator);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT messenger, const VkAllocationCallbacks* pAllocator);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_SubmitDebugUtilsMessageEXT(VkInstance instance, VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_SubmitDebugUtilsMessageEXT(VkInstance instance, VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateHeadlessSurfaceEXT(VkInstance instance, const VkHeadlessSurfaceCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateHeadlessSurfaceEXT(VkInstance instance, const VkHeadlessSurfaceCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);

  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties* pProperties);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties* pProperties);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceQueueFamilyProperties(VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties* pQueueFamilyProperties);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceQueueFamilyProperties(VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties* pQueueFamilyProperties);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceMemoryProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties* pMemoryProperties);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceMemoryProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties* pMemoryProperties);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceFeatures(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures* pFeatures);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceFeatures(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures* pFeatures);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties* pFormatProperties);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties* pFormatProperties);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceImageFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkImageFormatProperties* pImageFormatProperties);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceImageFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkImageFormatProperties* pImageFormatProperties);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateDevice(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDevice* pDevice);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateDevice(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDevice* pDevice);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_EnumerateDeviceLayerProperties(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkLayerProperties* pProperties);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_EnumerateDeviceLayerProperties(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkLayerProperties* pProperties);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_EnumerateDeviceExtensionProperties(VkPhysicalDevice physicalDevice, const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_EnumerateDeviceExtensionProperties(VkPhysicalDevice physicalDevice, const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceSparseImageFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkSampleCountFlagBits samples, VkImageUsageFlags usage, VkImageTiling tiling, uint32_t* pPropertyCount, VkSparseImageFormatProperties* pProperties);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceSparseImageFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkSampleCountFlagBits samples, VkImageUsageFlags usage, VkImageTiling tiling, uint32_t* pPropertyCount, VkSparseImageFormatProperties* pProperties);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceDisplayPropertiesKHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPropertiesKHR* pProperties);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceDisplayPropertiesKHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPropertiesKHR* pProperties);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceDisplayPlanePropertiesKHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPlanePropertiesKHR* pProperties);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceDisplayPlanePropertiesKHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPlanePropertiesKHR* pProperties);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetDisplayPlaneSupportedDisplaysKHR(VkPhysicalDevice physicalDevice, uint32_t planeIndex, uint32_t* pDisplayCount, VkDisplayKHR* pDisplays);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetDisplayPlaneSupportedDisplaysKHR(VkPhysicalDevice physicalDevice, uint32_t planeIndex, uint32_t* pDisplayCount, VkDisplayKHR* pDisplays);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetDisplayModePropertiesKHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModePropertiesKHR* pProperties);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetDisplayModePropertiesKHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModePropertiesKHR* pProperties);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateDisplayModeKHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, const VkDisplayModeCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDisplayModeKHR* pMode);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateDisplayModeKHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, const VkDisplayModeCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDisplayModeKHR* pMode);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetDisplayPlaneCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkDisplayModeKHR mode, uint32_t planeIndex, VkDisplayPlaneCapabilitiesKHR* pCapabilities);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetDisplayPlaneCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkDisplayModeKHR mode, uint32_t planeIndex, VkDisplayPlaneCapabilitiesKHR* pCapabilities);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceSurfaceSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceSurfaceSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceSurfaceCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR* pSurfaceCapabilities);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceSurfaceCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR* pSurfaceCapabilities);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceSurfaceFormatsKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pSurfaceFormatCount, VkSurfaceFormatKHR* pSurfaceFormats);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceSurfaceFormatsKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pSurfaceFormatCount, VkSurfaceFormatKHR* pSurfaceFormats);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceSurfacePresentModesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceSurfacePresentModesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes);
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
  VKAPI_ATTR VkBool32 VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceWaylandPresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, struct wl_display* display);
  VKAPI_ATTR VkBool32 VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceWaylandPresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, struct wl_display* display);
#endif // VK_USE_PLATFORM_WAYLAND_KHR
#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkBool32 VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceWin32PresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex);
  VKAPI_ATTR VkBool32 VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceWin32PresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex);
#endif // VK_USE_PLATFORM_WIN32_KHR
#ifdef VK_USE_PLATFORM_XLIB_KHR
  VKAPI_ATTR VkBool32 VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceXlibPresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, Display* dpy, VisualID visualID);
  VKAPI_ATTR VkBool32 VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceXlibPresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, Display* dpy, VisualID visualID);
#endif // VK_USE_PLATFORM_XLIB_KHR
#ifdef VK_USE_PLATFORM_XCB_KHR
  VKAPI_ATTR VkBool32 VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceXcbPresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, xcb_connection_t* connection, xcb_visualid_t visual_id);
  VKAPI_ATTR VkBool32 VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceXcbPresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, xcb_connection_t* connection, xcb_visualid_t visual_id);
#endif // VK_USE_PLATFORM_XCB_KHR
#ifdef VK_USE_PLATFORM_DIRECTFB_EXT
  VKAPI_ATTR VkBool32 VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceDirectFBPresentationSupportEXT(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, IDirectFB* dfb);
  VKAPI_ATTR VkBool32 VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceDirectFBPresentationSupportEXT(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, IDirectFB* dfb);
#endif // VK_USE_PLATFORM_DIRECTFB_EXT
#ifdef VK_USE_PLATFORM_SCREEN_QNX
  VKAPI_ATTR VkBool32 VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceScreenPresentationSupportQNX(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, struct _screen_window* window);
  VKAPI_ATTR VkBool32 VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceScreenPresentationSupportQNX(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, struct _screen_window* window);
#endif // VK_USE_PLATFORM_SCREEN_QNX
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceExternalImageFormatPropertiesNV(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkExternalMemoryHandleTypeFlagsNV externalHandleType, VkExternalImageFormatPropertiesNV* pExternalImageFormatProperties);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceExternalImageFormatPropertiesNV(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkExternalMemoryHandleTypeFlagsNV externalHandleType, VkExternalImageFormatPropertiesNV* pExternalImageFormatProperties);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceFeatures2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceFeatures2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceFeatures2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceFeatures2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceProperties2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceProperties2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceFormatProperties2(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2* pFormatProperties);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceFormatProperties2(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2* pFormatProperties);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceFormatProperties2KHR(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2* pFormatProperties);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceFormatProperties2KHR(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2* pFormatProperties);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceImageFormatProperties2(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceImageFormatProperties2(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceImageFormatProperties2KHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceImageFormatProperties2KHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceQueueFamilyProperties2(VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties2* pQueueFamilyProperties);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceQueueFamilyProperties2(VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties2* pQueueFamilyProperties);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceQueueFamilyProperties2KHR(VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties2* pQueueFamilyProperties);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceQueueFamilyProperties2KHR(VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties2* pQueueFamilyProperties);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceMemoryProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceMemoryProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceMemoryProperties2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceMemoryProperties2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceSparseImageFormatProperties2(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo, uint32_t* pPropertyCount, VkSparseImageFormatProperties2* pProperties);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceSparseImageFormatProperties2(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo, uint32_t* pPropertyCount, VkSparseImageFormatProperties2* pProperties);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceSparseImageFormatProperties2KHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo, uint32_t* pPropertyCount, VkSparseImageFormatProperties2* pProperties);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceSparseImageFormatProperties2KHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo, uint32_t* pPropertyCount, VkSparseImageFormatProperties2* pProperties);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceExternalBufferProperties(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceExternalBufferProperties(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceExternalBufferPropertiesKHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceExternalBufferPropertiesKHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceExternalSemaphoreProperties(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceExternalSemaphoreProperties(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceExternalSemaphorePropertiesKHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceExternalSemaphorePropertiesKHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceExternalFenceProperties(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceExternalFenceProperties(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceExternalFencePropertiesKHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceExternalFencePropertiesKHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_ReleaseDisplayEXT(VkPhysicalDevice physicalDevice, VkDisplayKHR display);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_ReleaseDisplayEXT(VkPhysicalDevice physicalDevice, VkDisplayKHR display);
#ifdef VK_USE_PLATFORM_XLIB_XRANDR_EXT
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_AcquireXlibDisplayEXT(VkPhysicalDevice physicalDevice, Display* dpy, VkDisplayKHR display);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_AcquireXlibDisplayEXT(VkPhysicalDevice physicalDevice, Display* dpy, VkDisplayKHR display);
#endif // VK_USE_PLATFORM_XLIB_XRANDR_EXT
#ifdef VK_USE_PLATFORM_XLIB_XRANDR_EXT
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetRandROutputDisplayEXT(VkPhysicalDevice physicalDevice, Display* dpy, RROutput rrOutput, VkDisplayKHR* pDisplay);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetRandROutputDisplayEXT(VkPhysicalDevice physicalDevice, Display* dpy, RROutput rrOutput, VkDisplayKHR* pDisplay);
#endif // VK_USE_PLATFORM_XLIB_XRANDR_EXT
#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_AcquireWinrtDisplayNV(VkPhysicalDevice physicalDevice, VkDisplayKHR display);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_AcquireWinrtDisplayNV(VkPhysicalDevice physicalDevice, VkDisplayKHR display);
#endif // VK_USE_PLATFORM_WIN32_KHR
#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetWinrtDisplayNV(VkPhysicalDevice physicalDevice, uint32_t deviceRelativeId, VkDisplayKHR* pDisplay);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetWinrtDisplayNV(VkPhysicalDevice physicalDevice, uint32_t deviceRelativeId, VkDisplayKHR* pDisplay);
#endif // VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceSurfaceCapabilities2EXT(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilities2EXT* pSurfaceCapabilities);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceSurfaceCapabilities2EXT(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilities2EXT* pSurfaceCapabilities);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetPhysicalDevicePresentRectanglesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pRectCount, VkRect2D* pRects);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDevicePresentRectanglesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pRectCount, VkRect2D* pRects);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceMultisamplePropertiesEXT(VkPhysicalDevice physicalDevice, VkSampleCountFlagBits samples, VkMultisamplePropertiesEXT* pMultisampleProperties);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceMultisamplePropertiesEXT(VkPhysicalDevice physicalDevice, VkSampleCountFlagBits samples, VkMultisamplePropertiesEXT* pMultisampleProperties);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceSurfaceCapabilities2KHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkSurfaceCapabilities2KHR* pSurfaceCapabilities);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceSurfaceCapabilities2KHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkSurfaceCapabilities2KHR* pSurfaceCapabilities);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceSurfaceFormats2KHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pSurfaceFormatCount, VkSurfaceFormat2KHR* pSurfaceFormats);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceSurfaceFormats2KHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pSurfaceFormatCount, VkSurfaceFormat2KHR* pSurfaceFormats);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceDisplayProperties2KHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayProperties2KHR* pProperties);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceDisplayProperties2KHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayProperties2KHR* pProperties);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceDisplayPlaneProperties2KHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPlaneProperties2KHR* pProperties);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceDisplayPlaneProperties2KHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPlaneProperties2KHR* pProperties);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetDisplayModeProperties2KHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModeProperties2KHR* pProperties);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetDisplayModeProperties2KHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModeProperties2KHR* pProperties);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetDisplayPlaneCapabilities2KHR(VkPhysicalDevice physicalDevice, const VkDisplayPlaneInfo2KHR* pDisplayPlaneInfo, VkDisplayPlaneCapabilities2KHR* pCapabilities);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetDisplayPlaneCapabilities2KHR(VkPhysicalDevice physicalDevice, const VkDisplayPlaneInfo2KHR* pDisplayPlaneInfo, VkDisplayPlaneCapabilities2KHR* pCapabilities);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceCalibrateableTimeDomainsEXT(VkPhysicalDevice physicalDevice, uint32_t* pTimeDomainCount, VkTimeDomainEXT* pTimeDomains);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceCalibrateableTimeDomainsEXT(VkPhysicalDevice physicalDevice, uint32_t* pTimeDomainCount, VkTimeDomainEXT* pTimeDomains);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceCooperativeMatrixPropertiesNV(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkCooperativeMatrixPropertiesNV* pProperties);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceCooperativeMatrixPropertiesNV(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkCooperativeMatrixPropertiesNV* pProperties);
#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceSurfacePresentModes2EXT(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceSurfacePresentModes2EXT(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes);
#endif // VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_EnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, uint32_t* pCounterCount, VkPerformanceCounterKHR* pCounters, VkPerformanceCounterDescriptionKHR* pCounterDescriptions);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_EnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, uint32_t* pCounterCount, VkPerformanceCounterKHR* pCounters, VkPerformanceCounterDescriptionKHR* pCounterDescriptions);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR(VkPhysicalDevice physicalDevice, const VkQueryPoolPerformanceCreateInfoKHR* pPerformanceQueryCreateInfo, uint32_t* pNumPasses);
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR(VkPhysicalDevice physicalDevice, const VkQueryPoolPerformanceCreateInfoKHR* pPerformanceQueryCreateInfo, uint32_t* pNumPasses);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV(VkPhysicalDevice physicalDevice, uint32_t* pCombinationCount, VkFramebufferMixedSamplesCombinationNV* pCombinations);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV(VkPhysicalDevice physicalDevice, uint32_t* pCombinationCount, VkFramebufferMixedSamplesCombinationNV* pCombinations);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceToolProperties(VkPhysicalDevice physicalDevice, uint32_t* pToolCount, VkPhysicalDeviceToolProperties* pToolProperties);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceToolProperties(VkPhysicalDevice physicalDevice, uint32_t* pToolCount, VkPhysicalDeviceToolProperties* pToolProperties);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceToolPropertiesEXT(VkPhysicalDevice physicalDevice, uint32_t* pToolCount, VkPhysicalDeviceToolProperties* pToolProperties);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceToolPropertiesEXT(VkPhysicalDevice physicalDevice, uint32_t* pToolCount, VkPhysicalDeviceToolProperties* pToolProperties);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceFragmentShadingRatesKHR(VkPhysicalDevice physicalDevice, uint32_t* pFragmentShadingRateCount, VkPhysicalDeviceFragmentShadingRateKHR* pFragmentShadingRates);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceFragmentShadingRatesKHR(VkPhysicalDevice physicalDevice, uint32_t* pFragmentShadingRateCount, VkPhysicalDeviceFragmentShadingRateKHR* pFragmentShadingRates);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceVideoCapabilitiesKHR(VkPhysicalDevice physicalDevice, const VkVideoProfileInfoKHR* pVideoProfile, VkVideoCapabilitiesKHR* pCapabilities);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceVideoCapabilitiesKHR(VkPhysicalDevice physicalDevice, const VkVideoProfileInfoKHR* pVideoProfile, VkVideoCapabilitiesKHR* pCapabilities);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceVideoFormatPropertiesKHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceVideoFormatInfoKHR* pVideoFormatInfo, uint32_t* pVideoFormatPropertyCount, VkVideoFormatPropertiesKHR* pVideoFormatProperties);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceVideoFormatPropertiesKHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceVideoFormatInfoKHR* pVideoFormatInfo, uint32_t* pVideoFormatPropertyCount, VkVideoFormatPropertiesKHR* pVideoFormatProperties);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_AcquireDrmDisplayEXT(VkPhysicalDevice physicalDevice, int32_t drmFd, VkDisplayKHR display);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_AcquireDrmDisplayEXT(VkPhysicalDevice physicalDevice, int32_t drmFd, VkDisplayKHR display);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetDrmDisplayEXT(VkPhysicalDevice physicalDevice, int32_t drmFd, uint32_t connectorId, VkDisplayKHR* display);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetDrmDisplayEXT(VkPhysicalDevice physicalDevice, int32_t drmFd, uint32_t connectorId, VkDisplayKHR* display);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceOpticalFlowImageFormatsNV(VkPhysicalDevice physicalDevice, const VkOpticalFlowImageFormatInfoNV* pOpticalFlowImageFormatInfo, uint32_t* pFormatCount, VkOpticalFlowImageFormatPropertiesNV* pImageFormatProperties);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceOpticalFlowImageFormatsNV(VkPhysicalDevice physicalDevice, const VkOpticalFlowImageFormatInfoNV* pOpticalFlowImageFormatInfo, uint32_t* pFormatCount, VkOpticalFlowImageFormatPropertiesNV* pImageFormatProperties);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetPhysicalDeviceCooperativeMatrixPropertiesKHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkCooperativeMatrixPropertiesKHR* pProperties);
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPhysicalDeviceCooperativeMatrixPropertiesKHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkCooperativeMatrixPropertiesKHR* pProperties);

  VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vk_cmd_enqueue_GetDeviceProcAddr(VkDevice device, const char* pName) ATTR_WEAK;
  VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vk_cmd_enqueue_unless_primary_GetDeviceProcAddr(VkDevice device, const char* pName) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_DestroyDevice(VkDevice device, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_DestroyDevice(VkDevice device, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetDeviceQueue(VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetDeviceQueue(VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_QueueSubmit(VkQueue queue, uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_QueueSubmit(VkQueue queue, uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_QueueWaitIdle(VkQueue queue) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_QueueWaitIdle(VkQueue queue) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_DeviceWaitIdle(VkDevice device) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_DeviceWaitIdle(VkDevice device) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_AllocateMemory(VkDevice device, const VkMemoryAllocateInfo* pAllocateInfo, const VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_AllocateMemory(VkDevice device, const VkMemoryAllocateInfo* pAllocateInfo, const VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_FreeMemory(VkDevice device, VkDeviceMemory memory, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_FreeMemory(VkDevice device, VkDeviceMemory memory, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_MapMemory(VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_MapMemory(VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_UnmapMemory(VkDevice device, VkDeviceMemory memory) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_UnmapMemory(VkDevice device, VkDeviceMemory memory) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_FlushMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_FlushMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_InvalidateMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_InvalidateMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetDeviceMemoryCommitment(VkDevice device, VkDeviceMemory memory, VkDeviceSize* pCommittedMemoryInBytes) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetDeviceMemoryCommitment(VkDevice device, VkDeviceMemory memory, VkDeviceSize* pCommittedMemoryInBytes) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetBufferMemoryRequirements(VkDevice device, VkBuffer buffer, VkMemoryRequirements* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetBufferMemoryRequirements(VkDevice device, VkBuffer buffer, VkMemoryRequirements* pMemoryRequirements) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_BindBufferMemory(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_BindBufferMemory(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetImageMemoryRequirements(VkDevice device, VkImage image, VkMemoryRequirements* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetImageMemoryRequirements(VkDevice device, VkImage image, VkMemoryRequirements* pMemoryRequirements) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_BindImageMemory(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_BindImageMemory(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetImageSparseMemoryRequirements(VkDevice device, VkImage image, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements* pSparseMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetImageSparseMemoryRequirements(VkDevice device, VkImage image, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements* pSparseMemoryRequirements) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_QueueBindSparse(VkQueue queue, uint32_t bindInfoCount, const VkBindSparseInfo* pBindInfo, VkFence fence) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_QueueBindSparse(VkQueue queue, uint32_t bindInfoCount, const VkBindSparseInfo* pBindInfo, VkFence fence) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateFence(VkDevice device, const VkFenceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateFence(VkDevice device, const VkFenceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_DestroyFence(VkDevice device, VkFence fence, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_DestroyFence(VkDevice device, VkFence fence, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_ResetFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_ResetFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetFenceStatus(VkDevice device, VkFence fence) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetFenceStatus(VkDevice device, VkFence fence) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_WaitForFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences, VkBool32 waitAll, uint64_t timeout) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_WaitForFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences, VkBool32 waitAll, uint64_t timeout) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateSemaphore(VkDevice device, const VkSemaphoreCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateSemaphore(VkDevice device, const VkSemaphoreCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_DestroySemaphore(VkDevice device, VkSemaphore semaphore, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_DestroySemaphore(VkDevice device, VkSemaphore semaphore, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateEvent(VkDevice device, const VkEventCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkEvent* pEvent) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateEvent(VkDevice device, const VkEventCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkEvent* pEvent) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_DestroyEvent(VkDevice device, VkEvent event, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_DestroyEvent(VkDevice device, VkEvent event, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetEventStatus(VkDevice device, VkEvent event) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetEventStatus(VkDevice device, VkEvent event) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_SetEvent(VkDevice device, VkEvent event) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_SetEvent(VkDevice device, VkEvent event) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_ResetEvent(VkDevice device, VkEvent event) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_ResetEvent(VkDevice device, VkEvent event) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateQueryPool(VkDevice device, const VkQueryPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkQueryPool* pQueryPool) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateQueryPool(VkDevice device, const VkQueryPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkQueryPool* pQueryPool) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_DestroyQueryPool(VkDevice device, VkQueryPool queryPool, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_DestroyQueryPool(VkDevice device, VkQueryPool queryPool, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetQueryPoolResults(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, VkDeviceSize stride, VkQueryResultFlags flags) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetQueryPoolResults(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, VkDeviceSize stride, VkQueryResultFlags flags) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_ResetQueryPool(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_ResetQueryPool(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_ResetQueryPoolEXT(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_ResetQueryPoolEXT(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateBuffer(VkDevice device, const VkBufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateBuffer(VkDevice device, const VkBufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_DestroyBuffer(VkDevice device, VkBuffer buffer, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_DestroyBuffer(VkDevice device, VkBuffer buffer, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateBufferView(VkDevice device, const VkBufferViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferView* pView) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateBufferView(VkDevice device, const VkBufferViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferView* pView) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_DestroyBufferView(VkDevice device, VkBufferView bufferView, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_DestroyBufferView(VkDevice device, VkBufferView bufferView, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateImage(VkDevice device, const VkImageCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImage* pImage) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateImage(VkDevice device, const VkImageCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImage* pImage) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_DestroyImage(VkDevice device, VkImage image, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_DestroyImage(VkDevice device, VkImage image, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetImageSubresourceLayout(VkDevice device, VkImage image, const VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetImageSubresourceLayout(VkDevice device, VkImage image, const VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateImageView(VkDevice device, const VkImageViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImageView* pView) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateImageView(VkDevice device, const VkImageViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImageView* pView) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_DestroyImageView(VkDevice device, VkImageView imageView, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_DestroyImageView(VkDevice device, VkImageView imageView, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateShaderModule(VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateShaderModule(VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_DestroyShaderModule(VkDevice device, VkShaderModule shaderModule, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_DestroyShaderModule(VkDevice device, VkShaderModule shaderModule, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreatePipelineCache(VkDevice device, const VkPipelineCacheCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineCache* pPipelineCache) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreatePipelineCache(VkDevice device, const VkPipelineCacheCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineCache* pPipelineCache) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_DestroyPipelineCache(VkDevice device, VkPipelineCache pipelineCache, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_DestroyPipelineCache(VkDevice device, VkPipelineCache pipelineCache, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetPipelineCacheData(VkDevice device, VkPipelineCache pipelineCache, size_t* pDataSize, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPipelineCacheData(VkDevice device, VkPipelineCache pipelineCache, size_t* pDataSize, void* pData) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_MergePipelineCaches(VkDevice device, VkPipelineCache dstCache, uint32_t srcCacheCount, const VkPipelineCache* pSrcCaches) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_MergePipelineCaches(VkDevice device, VkPipelineCache dstCache, uint32_t srcCacheCount, const VkPipelineCache* pSrcCaches) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateGraphicsPipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkGraphicsPipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateGraphicsPipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkGraphicsPipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateComputePipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkComputePipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateComputePipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkComputePipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI(VkDevice device, VkRenderPass renderpass, VkExtent2D* pMaxWorkgroupSize) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI(VkDevice device, VkRenderPass renderpass, VkExtent2D* pMaxWorkgroupSize) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_DestroyPipeline(VkDevice device, VkPipeline pipeline, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_DestroyPipeline(VkDevice device, VkPipeline pipeline, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreatePipelineLayout(VkDevice device, const VkPipelineLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreatePipelineLayout(VkDevice device, const VkPipelineLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_DestroyPipelineLayout(VkDevice device, VkPipelineLayout pipelineLayout, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_DestroyPipelineLayout(VkDevice device, VkPipelineLayout pipelineLayout, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateSampler(VkDevice device, const VkSamplerCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSampler* pSampler) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateSampler(VkDevice device, const VkSamplerCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSampler* pSampler) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_DestroySampler(VkDevice device, VkSampler sampler, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_DestroySampler(VkDevice device, VkSampler sampler, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateDescriptorSetLayout(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorSetLayout* pSetLayout) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateDescriptorSetLayout(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorSetLayout* pSetLayout) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_DestroyDescriptorSetLayout(VkDevice device, VkDescriptorSetLayout descriptorSetLayout, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_DestroyDescriptorSetLayout(VkDevice device, VkDescriptorSetLayout descriptorSetLayout, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateDescriptorPool(VkDevice device, const VkDescriptorPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorPool* pDescriptorPool) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateDescriptorPool(VkDevice device, const VkDescriptorPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorPool* pDescriptorPool) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_DestroyDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_DestroyDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_ResetDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags flags) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_ResetDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags flags) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_AllocateDescriptorSets(VkDevice device, const VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_AllocateDescriptorSets(VkDevice device, const VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_FreeDescriptorSets(VkDevice device, VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_FreeDescriptorSets(VkDevice device, VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_UpdateDescriptorSets(VkDevice device, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, const VkCopyDescriptorSet* pDescriptorCopies) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_UpdateDescriptorSets(VkDevice device, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, const VkCopyDescriptorSet* pDescriptorCopies) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateFramebuffer(VkDevice device, const VkFramebufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFramebuffer* pFramebuffer) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateFramebuffer(VkDevice device, const VkFramebufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFramebuffer* pFramebuffer) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_DestroyFramebuffer(VkDevice device, VkFramebuffer framebuffer, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_DestroyFramebuffer(VkDevice device, VkFramebuffer framebuffer, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateRenderPass(VkDevice device, const VkRenderPassCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateRenderPass(VkDevice device, const VkRenderPassCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_DestroyRenderPass(VkDevice device, VkRenderPass renderPass, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_DestroyRenderPass(VkDevice device, VkRenderPass renderPass, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetRenderAreaGranularity(VkDevice device, VkRenderPass renderPass, VkExtent2D* pGranularity) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetRenderAreaGranularity(VkDevice device, VkRenderPass renderPass, VkExtent2D* pGranularity) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetRenderingAreaGranularityKHR(VkDevice device, const VkRenderingAreaInfoKHR* pRenderingAreaInfo, VkExtent2D* pGranularity) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetRenderingAreaGranularityKHR(VkDevice device, const VkRenderingAreaInfoKHR* pRenderingAreaInfo, VkExtent2D* pGranularity) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateCommandPool(VkDevice device, const VkCommandPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateCommandPool(VkDevice device, const VkCommandPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_DestroyCommandPool(VkDevice device, VkCommandPool commandPool, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_DestroyCommandPool(VkDevice device, VkCommandPool commandPool, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_ResetCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolResetFlags flags) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_ResetCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolResetFlags flags) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_AllocateCommandBuffers(VkDevice device, const VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_AllocateCommandBuffers(VkDevice device, const VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_FreeCommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_FreeCommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_BeginCommandBuffer(VkCommandBuffer commandBuffer, const VkCommandBufferBeginInfo* pBeginInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_BeginCommandBuffer(VkCommandBuffer commandBuffer, const VkCommandBufferBeginInfo* pBeginInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_EndCommandBuffer(VkCommandBuffer commandBuffer) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_EndCommandBuffer(VkCommandBuffer commandBuffer) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_ResetCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_ResetCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdBindPipeline(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdBindPipeline(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetAttachmentFeedbackLoopEnableEXT(VkCommandBuffer commandBuffer, VkImageAspectFlags aspectMask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetAttachmentFeedbackLoopEnableEXT(VkCommandBuffer commandBuffer, VkImageAspectFlags aspectMask) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetViewport(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pViewports) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetViewport(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pViewports) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetScissor(VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pScissors) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetScissor(VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pScissors) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetLineWidth(VkCommandBuffer commandBuffer, float lineWidth) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetLineWidth(VkCommandBuffer commandBuffer, float lineWidth) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetDepthBias(VkCommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetDepthBias(VkCommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetBlendConstants(VkCommandBuffer commandBuffer, const float blendConstants[4]) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetBlendConstants(VkCommandBuffer commandBuffer, const float blendConstants[4]) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetDepthBounds(VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetDepthBounds(VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetStencilCompareMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetStencilCompareMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetStencilWriteMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetStencilWriteMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetStencilReference(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetStencilReference(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdBindDescriptorSets(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdBindDescriptorSets(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdBindIndexBuffer(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdBindIndexBuffer(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdBindVertexBuffers(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdBindVertexBuffers(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdDraw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdDraw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdDrawIndexed(VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdDrawIndexed(VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdDrawMultiEXT(VkCommandBuffer commandBuffer, uint32_t drawCount, const VkMultiDrawInfoEXT* pVertexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdDrawMultiEXT(VkCommandBuffer commandBuffer, uint32_t drawCount, const VkMultiDrawInfoEXT* pVertexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdDrawMultiIndexedEXT(VkCommandBuffer commandBuffer, uint32_t drawCount, const VkMultiDrawIndexedInfoEXT* pIndexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride, const int32_t* pVertexOffset) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdDrawMultiIndexedEXT(VkCommandBuffer commandBuffer, uint32_t drawCount, const VkMultiDrawIndexedInfoEXT* pIndexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride, const int32_t* pVertexOffset) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdDrawIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdDrawIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdDrawIndexedIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdDrawIndexedIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdDispatch(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdDispatch(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdDispatchIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdDispatchIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSubpassShadingHUAWEI(VkCommandBuffer commandBuffer) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSubpassShadingHUAWEI(VkCommandBuffer commandBuffer) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdDrawClusterHUAWEI(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdDrawClusterHUAWEI(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdDrawClusterIndirectHUAWEI(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdDrawClusterIndirectHUAWEI(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdUpdatePipelineIndirectBufferNV(VkCommandBuffer commandBuffer, VkPipelineBindPoint           pipelineBindPoint, VkPipeline                    pipeline) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdUpdatePipelineIndirectBufferNV(VkCommandBuffer commandBuffer, VkPipelineBindPoint           pipelineBindPoint, VkPipeline                    pipeline) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdCopyBuffer(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdCopyBuffer(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdCopyImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdCopyImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdBlitImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdBlitImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdCopyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdCopyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdCopyImageToBuffer(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdCopyImageToBuffer(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdCopyMemoryIndirectNV(VkCommandBuffer commandBuffer, VkDeviceAddress copyBufferAddress, uint32_t copyCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdCopyMemoryIndirectNV(VkCommandBuffer commandBuffer, VkDeviceAddress copyBufferAddress, uint32_t copyCount, uint32_t stride) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdCopyMemoryToImageIndirectNV(VkCommandBuffer commandBuffer, VkDeviceAddress copyBufferAddress, uint32_t copyCount, uint32_t stride, VkImage dstImage, VkImageLayout dstImageLayout, const VkImageSubresourceLayers* pImageSubresources) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdCopyMemoryToImageIndirectNV(VkCommandBuffer commandBuffer, VkDeviceAddress copyBufferAddress, uint32_t copyCount, uint32_t stride, VkImage dstImage, VkImageLayout dstImageLayout, const VkImageSubresourceLayers* pImageSubresources) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdUpdateBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdUpdateBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdFillBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdFillBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdClearColorImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdClearColorImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdClearDepthStencilImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdClearDepthStencilImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdClearAttachments(VkCommandBuffer commandBuffer, uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdClearAttachments(VkCommandBuffer commandBuffer, uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdResolveImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageResolve* pRegions) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdResolveImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageResolve* pRegions) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdResetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdResetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdWaitEvents(VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdWaitEvents(VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdPipelineBarrier(VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdPipelineBarrier(VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdBeginQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdBeginQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdEndQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdEndQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdBeginConditionalRenderingEXT(VkCommandBuffer commandBuffer, const VkConditionalRenderingBeginInfoEXT* pConditionalRenderingBegin) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdBeginConditionalRenderingEXT(VkCommandBuffer commandBuffer, const VkConditionalRenderingBeginInfoEXT* pConditionalRenderingBegin) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdEndConditionalRenderingEXT(VkCommandBuffer commandBuffer) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdEndConditionalRenderingEXT(VkCommandBuffer commandBuffer) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdResetQueryPool(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdResetQueryPool(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdWriteTimestamp(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdWriteTimestamp(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdCopyQueryPoolResults(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdCopyQueryPoolResults(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdPushConstants(VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdPushConstants(VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdBeginRenderPass(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdBeginRenderPass(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdNextSubpass(VkCommandBuffer commandBuffer, VkSubpassContents contents) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdNextSubpass(VkCommandBuffer commandBuffer, VkSubpassContents contents) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdEndRenderPass(VkCommandBuffer commandBuffer) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdEndRenderPass(VkCommandBuffer commandBuffer) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdExecuteCommands(VkCommandBuffer commandBuffer, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdExecuteCommands(VkCommandBuffer commandBuffer, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateSharedSwapchainsKHR(VkDevice device, uint32_t swapchainCount, const VkSwapchainCreateInfoKHR* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchains) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateSharedSwapchainsKHR(VkDevice device, uint32_t swapchainCount, const VkSwapchainCreateInfoKHR* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchains) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateSwapchainKHR(VkDevice device, const VkSwapchainCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateSwapchainKHR(VkDevice device, const VkSwapchainCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_DestroySwapchainKHR(VkDevice device, VkSwapchainKHR swapchain, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_DestroySwapchainKHR(VkDevice device, VkSwapchainKHR swapchain, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetSwapchainImagesKHR(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetSwapchainImagesKHR(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_AcquireNextImageKHR(VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_AcquireNextImageKHR(VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_QueuePresentKHR(VkQueue queue, const VkPresentInfoKHR* pPresentInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_QueuePresentKHR(VkQueue queue, const VkPresentInfoKHR* pPresentInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_DebugMarkerSetObjectNameEXT(VkDevice device, const VkDebugMarkerObjectNameInfoEXT* pNameInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_DebugMarkerSetObjectNameEXT(VkDevice device, const VkDebugMarkerObjectNameInfoEXT* pNameInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_DebugMarkerSetObjectTagEXT(VkDevice device, const VkDebugMarkerObjectTagInfoEXT* pTagInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_DebugMarkerSetObjectTagEXT(VkDevice device, const VkDebugMarkerObjectTagInfoEXT* pTagInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdDebugMarkerBeginEXT(VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdDebugMarkerBeginEXT(VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdDebugMarkerEndEXT(VkCommandBuffer commandBuffer) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdDebugMarkerEndEXT(VkCommandBuffer commandBuffer) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdDebugMarkerInsertEXT(VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdDebugMarkerInsertEXT(VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo) ATTR_WEAK;


#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetMemoryWin32HandleNV(VkDevice device, VkDeviceMemory memory, VkExternalMemoryHandleTypeFlagsNV handleType, HANDLE* pHandle) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetMemoryWin32HandleNV(VkDevice device, VkDeviceMemory memory, VkExternalMemoryHandleTypeFlagsNV handleType, HANDLE* pHandle) ATTR_WEAK;


#endif // VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdExecuteGeneratedCommandsNV(VkCommandBuffer commandBuffer, VkBool32 isPreprocessed, const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdExecuteGeneratedCommandsNV(VkCommandBuffer commandBuffer, VkBool32 isPreprocessed, const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdPreprocessGeneratedCommandsNV(VkCommandBuffer commandBuffer, const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdPreprocessGeneratedCommandsNV(VkCommandBuffer commandBuffer, const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdBindPipelineShaderGroupNV(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline, uint32_t groupIndex) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdBindPipelineShaderGroupNV(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline, uint32_t groupIndex) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetGeneratedCommandsMemoryRequirementsNV(VkDevice device, const VkGeneratedCommandsMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetGeneratedCommandsMemoryRequirementsNV(VkDevice device, const VkGeneratedCommandsMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateIndirectCommandsLayoutNV(VkDevice device, const VkIndirectCommandsLayoutCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkIndirectCommandsLayoutNV* pIndirectCommandsLayout) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateIndirectCommandsLayoutNV(VkDevice device, const VkIndirectCommandsLayoutCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkIndirectCommandsLayoutNV* pIndirectCommandsLayout) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_DestroyIndirectCommandsLayoutNV(VkDevice device, VkIndirectCommandsLayoutNV indirectCommandsLayout, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_DestroyIndirectCommandsLayoutNV(VkDevice device, VkIndirectCommandsLayoutNV indirectCommandsLayout, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdPushDescriptorSetKHR(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdPushDescriptorSetKHR(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_TrimCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_TrimCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_TrimCommandPoolKHR(VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_TrimCommandPoolKHR(VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags) ATTR_WEAK;


#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetMemoryWin32HandleKHR(VkDevice device, const VkMemoryGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetMemoryWin32HandleKHR(VkDevice device, const VkMemoryGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle) ATTR_WEAK;


#endif // VK_USE_PLATFORM_WIN32_KHR
#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetMemoryWin32HandlePropertiesKHR(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, HANDLE handle, VkMemoryWin32HandlePropertiesKHR* pMemoryWin32HandleProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetMemoryWin32HandlePropertiesKHR(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, HANDLE handle, VkMemoryWin32HandlePropertiesKHR* pMemoryWin32HandleProperties) ATTR_WEAK;


#endif // VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetMemoryFdKHR(VkDevice device, const VkMemoryGetFdInfoKHR* pGetFdInfo, int* pFd) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetMemoryFdKHR(VkDevice device, const VkMemoryGetFdInfoKHR* pGetFdInfo, int* pFd) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetMemoryFdPropertiesKHR(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, int fd, VkMemoryFdPropertiesKHR* pMemoryFdProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetMemoryFdPropertiesKHR(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, int fd, VkMemoryFdPropertiesKHR* pMemoryFdProperties) ATTR_WEAK;


#ifdef VK_USE_PLATFORM_FUCHSIA
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetMemoryZirconHandleFUCHSIA(VkDevice device, const VkMemoryGetZirconHandleInfoFUCHSIA* pGetZirconHandleInfo, zx_handle_t* pZirconHandle) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetMemoryZirconHandleFUCHSIA(VkDevice device, const VkMemoryGetZirconHandleInfoFUCHSIA* pGetZirconHandleInfo, zx_handle_t* pZirconHandle) ATTR_WEAK;


#endif // VK_USE_PLATFORM_FUCHSIA
#ifdef VK_USE_PLATFORM_FUCHSIA
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetMemoryZirconHandlePropertiesFUCHSIA(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, zx_handle_t zirconHandle, VkMemoryZirconHandlePropertiesFUCHSIA* pMemoryZirconHandleProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetMemoryZirconHandlePropertiesFUCHSIA(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, zx_handle_t zirconHandle, VkMemoryZirconHandlePropertiesFUCHSIA* pMemoryZirconHandleProperties) ATTR_WEAK;


#endif // VK_USE_PLATFORM_FUCHSIA
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetMemoryRemoteAddressNV(VkDevice device, const VkMemoryGetRemoteAddressInfoNV* pMemoryGetRemoteAddressInfo, VkRemoteAddressNV* pAddress) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetMemoryRemoteAddressNV(VkDevice device, const VkMemoryGetRemoteAddressInfoNV* pMemoryGetRemoteAddressInfo, VkRemoteAddressNV* pAddress) ATTR_WEAK;


#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetSemaphoreWin32HandleKHR(VkDevice device, const VkSemaphoreGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetSemaphoreWin32HandleKHR(VkDevice device, const VkSemaphoreGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle) ATTR_WEAK;


#endif // VK_USE_PLATFORM_WIN32_KHR
#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_ImportSemaphoreWin32HandleKHR(VkDevice device, const VkImportSemaphoreWin32HandleInfoKHR* pImportSemaphoreWin32HandleInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_ImportSemaphoreWin32HandleKHR(VkDevice device, const VkImportSemaphoreWin32HandleInfoKHR* pImportSemaphoreWin32HandleInfo) ATTR_WEAK;


#endif // VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetSemaphoreFdKHR(VkDevice device, const VkSemaphoreGetFdInfoKHR* pGetFdInfo, int* pFd) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetSemaphoreFdKHR(VkDevice device, const VkSemaphoreGetFdInfoKHR* pGetFdInfo, int* pFd) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_ImportSemaphoreFdKHR(VkDevice device, const VkImportSemaphoreFdInfoKHR* pImportSemaphoreFdInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_ImportSemaphoreFdKHR(VkDevice device, const VkImportSemaphoreFdInfoKHR* pImportSemaphoreFdInfo) ATTR_WEAK;


#ifdef VK_USE_PLATFORM_FUCHSIA
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetSemaphoreZirconHandleFUCHSIA(VkDevice device, const VkSemaphoreGetZirconHandleInfoFUCHSIA* pGetZirconHandleInfo, zx_handle_t* pZirconHandle) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetSemaphoreZirconHandleFUCHSIA(VkDevice device, const VkSemaphoreGetZirconHandleInfoFUCHSIA* pGetZirconHandleInfo, zx_handle_t* pZirconHandle) ATTR_WEAK;


#endif // VK_USE_PLATFORM_FUCHSIA
#ifdef VK_USE_PLATFORM_FUCHSIA
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_ImportSemaphoreZirconHandleFUCHSIA(VkDevice device, const VkImportSemaphoreZirconHandleInfoFUCHSIA* pImportSemaphoreZirconHandleInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_ImportSemaphoreZirconHandleFUCHSIA(VkDevice device, const VkImportSemaphoreZirconHandleInfoFUCHSIA* pImportSemaphoreZirconHandleInfo) ATTR_WEAK;


#endif // VK_USE_PLATFORM_FUCHSIA
#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetFenceWin32HandleKHR(VkDevice device, const VkFenceGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetFenceWin32HandleKHR(VkDevice device, const VkFenceGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle) ATTR_WEAK;


#endif // VK_USE_PLATFORM_WIN32_KHR
#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_ImportFenceWin32HandleKHR(VkDevice device, const VkImportFenceWin32HandleInfoKHR* pImportFenceWin32HandleInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_ImportFenceWin32HandleKHR(VkDevice device, const VkImportFenceWin32HandleInfoKHR* pImportFenceWin32HandleInfo) ATTR_WEAK;


#endif // VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetFenceFdKHR(VkDevice device, const VkFenceGetFdInfoKHR* pGetFdInfo, int* pFd) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetFenceFdKHR(VkDevice device, const VkFenceGetFdInfoKHR* pGetFdInfo, int* pFd) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_ImportFenceFdKHR(VkDevice device, const VkImportFenceFdInfoKHR* pImportFenceFdInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_ImportFenceFdKHR(VkDevice device, const VkImportFenceFdInfoKHR* pImportFenceFdInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_DisplayPowerControlEXT(VkDevice device, VkDisplayKHR display, const VkDisplayPowerInfoEXT* pDisplayPowerInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_DisplayPowerControlEXT(VkDevice device, VkDisplayKHR display, const VkDisplayPowerInfoEXT* pDisplayPowerInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_RegisterDeviceEventEXT(VkDevice device, const VkDeviceEventInfoEXT* pDeviceEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_RegisterDeviceEventEXT(VkDevice device, const VkDeviceEventInfoEXT* pDeviceEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_RegisterDisplayEventEXT(VkDevice device, VkDisplayKHR display, const VkDisplayEventInfoEXT* pDisplayEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_RegisterDisplayEventEXT(VkDevice device, VkDisplayKHR display, const VkDisplayEventInfoEXT* pDisplayEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetSwapchainCounterEXT(VkDevice device, VkSwapchainKHR swapchain, VkSurfaceCounterFlagBitsEXT counter, uint64_t* pCounterValue) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetSwapchainCounterEXT(VkDevice device, VkSwapchainKHR swapchain, VkSurfaceCounterFlagBitsEXT counter, uint64_t* pCounterValue) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetDeviceGroupPeerMemoryFeatures(VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetDeviceGroupPeerMemoryFeatures(VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetDeviceGroupPeerMemoryFeaturesKHR(VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetDeviceGroupPeerMemoryFeaturesKHR(VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_BindBufferMemory2(VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_BindBufferMemory2(VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_BindBufferMemory2KHR(VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_BindBufferMemory2KHR(VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_BindImageMemory2(VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_BindImageMemory2(VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_BindImageMemory2KHR(VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_BindImageMemory2KHR(VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetDeviceMask(VkCommandBuffer commandBuffer, uint32_t deviceMask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetDeviceMask(VkCommandBuffer commandBuffer, uint32_t deviceMask) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetDeviceMaskKHR(VkCommandBuffer commandBuffer, uint32_t deviceMask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetDeviceMaskKHR(VkCommandBuffer commandBuffer, uint32_t deviceMask) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetDeviceGroupPresentCapabilitiesKHR(VkDevice device, VkDeviceGroupPresentCapabilitiesKHR* pDeviceGroupPresentCapabilities) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetDeviceGroupPresentCapabilitiesKHR(VkDevice device, VkDeviceGroupPresentCapabilitiesKHR* pDeviceGroupPresentCapabilities) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetDeviceGroupSurfacePresentModesKHR(VkDevice device, VkSurfaceKHR surface, VkDeviceGroupPresentModeFlagsKHR* pModes) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetDeviceGroupSurfacePresentModesKHR(VkDevice device, VkSurfaceKHR surface, VkDeviceGroupPresentModeFlagsKHR* pModes) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_AcquireNextImage2KHR(VkDevice device, const VkAcquireNextImageInfoKHR* pAcquireInfo, uint32_t* pImageIndex) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_AcquireNextImage2KHR(VkDevice device, const VkAcquireNextImageInfoKHR* pAcquireInfo, uint32_t* pImageIndex) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdDispatchBase(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdDispatchBase(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdDispatchBaseKHR(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdDispatchBaseKHR(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateDescriptorUpdateTemplate(VkDevice device, const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateDescriptorUpdateTemplate(VkDevice device, const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateDescriptorUpdateTemplateKHR(VkDevice device, const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateDescriptorUpdateTemplateKHR(VkDevice device, const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_DestroyDescriptorUpdateTemplate(VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_DestroyDescriptorUpdateTemplate(VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_DestroyDescriptorUpdateTemplateKHR(VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_DestroyDescriptorUpdateTemplateKHR(VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_UpdateDescriptorSetWithTemplate(VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_UpdateDescriptorSetWithTemplate(VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_UpdateDescriptorSetWithTemplateKHR(VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_UpdateDescriptorSetWithTemplateKHR(VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdPushDescriptorSetWithTemplateKHR(VkCommandBuffer commandBuffer, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, const void* pData) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdPushDescriptorSetWithTemplateKHR(VkCommandBuffer commandBuffer, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, const void* pData) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_SetHdrMetadataEXT(VkDevice device, uint32_t swapchainCount, const VkSwapchainKHR* pSwapchains, const VkHdrMetadataEXT* pMetadata) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_SetHdrMetadataEXT(VkDevice device, uint32_t swapchainCount, const VkSwapchainKHR* pSwapchains, const VkHdrMetadataEXT* pMetadata) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetSwapchainStatusKHR(VkDevice device, VkSwapchainKHR swapchain) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetSwapchainStatusKHR(VkDevice device, VkSwapchainKHR swapchain) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetRefreshCycleDurationGOOGLE(VkDevice device, VkSwapchainKHR swapchain, VkRefreshCycleDurationGOOGLE* pDisplayTimingProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetRefreshCycleDurationGOOGLE(VkDevice device, VkSwapchainKHR swapchain, VkRefreshCycleDurationGOOGLE* pDisplayTimingProperties) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetPastPresentationTimingGOOGLE(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pPresentationTimingCount, VkPastPresentationTimingGOOGLE* pPresentationTimings) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPastPresentationTimingGOOGLE(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pPresentationTimingCount, VkPastPresentationTimingGOOGLE* pPresentationTimings) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetViewportWScalingNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewportWScalingNV* pViewportWScalings) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetViewportWScalingNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewportWScalingNV* pViewportWScalings) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetDiscardRectangleEXT(VkCommandBuffer commandBuffer, uint32_t firstDiscardRectangle, uint32_t discardRectangleCount, const VkRect2D* pDiscardRectangles) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetDiscardRectangleEXT(VkCommandBuffer commandBuffer, uint32_t firstDiscardRectangle, uint32_t discardRectangleCount, const VkRect2D* pDiscardRectangles) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetDiscardRectangleEnableEXT(VkCommandBuffer commandBuffer, VkBool32 discardRectangleEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetDiscardRectangleEnableEXT(VkCommandBuffer commandBuffer, VkBool32 discardRectangleEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetDiscardRectangleModeEXT(VkCommandBuffer commandBuffer, VkDiscardRectangleModeEXT discardRectangleMode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetDiscardRectangleModeEXT(VkCommandBuffer commandBuffer, VkDiscardRectangleModeEXT discardRectangleMode) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetSampleLocationsEXT(VkCommandBuffer commandBuffer, const VkSampleLocationsInfoEXT* pSampleLocationsInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetSampleLocationsEXT(VkCommandBuffer commandBuffer, const VkSampleLocationsInfoEXT* pSampleLocationsInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetBufferMemoryRequirements2(VkDevice device, const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetBufferMemoryRequirements2(VkDevice device, const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetBufferMemoryRequirements2KHR(VkDevice device, const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetBufferMemoryRequirements2KHR(VkDevice device, const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetImageMemoryRequirements2(VkDevice device, const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetImageMemoryRequirements2(VkDevice device, const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetImageMemoryRequirements2KHR(VkDevice device, const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetImageMemoryRequirements2KHR(VkDevice device, const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetImageSparseMemoryRequirements2(VkDevice device, const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetImageSparseMemoryRequirements2(VkDevice device, const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetImageSparseMemoryRequirements2KHR(VkDevice device, const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetImageSparseMemoryRequirements2KHR(VkDevice device, const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetDeviceBufferMemoryRequirements(VkDevice device, const VkDeviceBufferMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetDeviceBufferMemoryRequirements(VkDevice device, const VkDeviceBufferMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetDeviceBufferMemoryRequirementsKHR(VkDevice device, const VkDeviceBufferMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetDeviceBufferMemoryRequirementsKHR(VkDevice device, const VkDeviceBufferMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetDeviceImageMemoryRequirements(VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetDeviceImageMemoryRequirements(VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetDeviceImageMemoryRequirementsKHR(VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetDeviceImageMemoryRequirementsKHR(VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetDeviceImageSparseMemoryRequirements(VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetDeviceImageSparseMemoryRequirements(VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetDeviceImageSparseMemoryRequirementsKHR(VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetDeviceImageSparseMemoryRequirementsKHR(VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateSamplerYcbcrConversion(VkDevice device, const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateSamplerYcbcrConversion(VkDevice device, const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateSamplerYcbcrConversionKHR(VkDevice device, const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateSamplerYcbcrConversionKHR(VkDevice device, const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_DestroySamplerYcbcrConversion(VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_DestroySamplerYcbcrConversion(VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_DestroySamplerYcbcrConversionKHR(VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_DestroySamplerYcbcrConversionKHR(VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetDeviceQueue2(VkDevice device, const VkDeviceQueueInfo2* pQueueInfo, VkQueue* pQueue) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetDeviceQueue2(VkDevice device, const VkDeviceQueueInfo2* pQueueInfo, VkQueue* pQueue) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateValidationCacheEXT(VkDevice device, const VkValidationCacheCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkValidationCacheEXT* pValidationCache) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateValidationCacheEXT(VkDevice device, const VkValidationCacheCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkValidationCacheEXT* pValidationCache) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_DestroyValidationCacheEXT(VkDevice device, VkValidationCacheEXT validationCache, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_DestroyValidationCacheEXT(VkDevice device, VkValidationCacheEXT validationCache, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetValidationCacheDataEXT(VkDevice device, VkValidationCacheEXT validationCache, size_t* pDataSize, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetValidationCacheDataEXT(VkDevice device, VkValidationCacheEXT validationCache, size_t* pDataSize, void* pData) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_MergeValidationCachesEXT(VkDevice device, VkValidationCacheEXT dstCache, uint32_t srcCacheCount, const VkValidationCacheEXT* pSrcCaches) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_MergeValidationCachesEXT(VkDevice device, VkValidationCacheEXT dstCache, uint32_t srcCacheCount, const VkValidationCacheEXT* pSrcCaches) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetDescriptorSetLayoutSupport(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetDescriptorSetLayoutSupport(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetDescriptorSetLayoutSupportKHR(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetDescriptorSetLayoutSupportKHR(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport) ATTR_WEAK;


#ifdef VK_USE_PLATFORM_ANDROID_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetSwapchainGrallocUsageANDROID(VkDevice device, VkFormat format, VkImageUsageFlags imageUsage, int* grallocUsage) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetSwapchainGrallocUsageANDROID(VkDevice device, VkFormat format, VkImageUsageFlags imageUsage, int* grallocUsage) ATTR_WEAK;


#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetSwapchainGrallocUsage2ANDROID(VkDevice device, VkFormat format, VkImageUsageFlags imageUsage, VkSwapchainImageUsageFlagsANDROID swapchainImageUsage, uint64_t* grallocConsumerUsage, uint64_t* grallocProducerUsage) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetSwapchainGrallocUsage2ANDROID(VkDevice device, VkFormat format, VkImageUsageFlags imageUsage, VkSwapchainImageUsageFlagsANDROID swapchainImageUsage, uint64_t* grallocConsumerUsage, uint64_t* grallocProducerUsage) ATTR_WEAK;


#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_AcquireImageANDROID(VkDevice device, VkImage image, int nativeFenceFd, VkSemaphore semaphore, VkFence fence) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_AcquireImageANDROID(VkDevice device, VkImage image, int nativeFenceFd, VkSemaphore semaphore, VkFence fence) ATTR_WEAK;


#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_QueueSignalReleaseImageANDROID(VkQueue queue, uint32_t waitSemaphoreCount, const VkSemaphore* pWaitSemaphores, VkImage image, int* pNativeFenceFd) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_QueueSignalReleaseImageANDROID(VkQueue queue, uint32_t waitSemaphoreCount, const VkSemaphore* pWaitSemaphores, VkImage image, int* pNativeFenceFd) ATTR_WEAK;


#endif // VK_USE_PLATFORM_ANDROID_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetShaderInfoAMD(VkDevice device, VkPipeline pipeline, VkShaderStageFlagBits shaderStage, VkShaderInfoTypeAMD infoType, size_t* pInfoSize, void* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetShaderInfoAMD(VkDevice device, VkPipeline pipeline, VkShaderStageFlagBits shaderStage, VkShaderInfoTypeAMD infoType, size_t* pInfoSize, void* pInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_SetLocalDimmingAMD(VkDevice device, VkSwapchainKHR swapChain, VkBool32 localDimmingEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_SetLocalDimmingAMD(VkDevice device, VkSwapchainKHR swapChain, VkBool32 localDimmingEnable) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetCalibratedTimestampsEXT(VkDevice device, uint32_t timestampCount, const VkCalibratedTimestampInfoEXT* pTimestampInfos, uint64_t* pTimestamps, uint64_t* pMaxDeviation) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetCalibratedTimestampsEXT(VkDevice device, uint32_t timestampCount, const VkCalibratedTimestampInfoEXT* pTimestampInfos, uint64_t* pTimestamps, uint64_t* pMaxDeviation) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_SetDebugUtilsObjectNameEXT(VkDevice device, const VkDebugUtilsObjectNameInfoEXT* pNameInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_SetDebugUtilsObjectNameEXT(VkDevice device, const VkDebugUtilsObjectNameInfoEXT* pNameInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_SetDebugUtilsObjectTagEXT(VkDevice device, const VkDebugUtilsObjectTagInfoEXT* pTagInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_SetDebugUtilsObjectTagEXT(VkDevice device, const VkDebugUtilsObjectTagInfoEXT* pTagInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_QueueBeginDebugUtilsLabelEXT(VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_QueueBeginDebugUtilsLabelEXT(VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_QueueEndDebugUtilsLabelEXT(VkQueue queue) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_QueueEndDebugUtilsLabelEXT(VkQueue queue) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_QueueInsertDebugUtilsLabelEXT(VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_QueueInsertDebugUtilsLabelEXT(VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdBeginDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdBeginDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdEndDebugUtilsLabelEXT(VkCommandBuffer commandBuffer) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdEndDebugUtilsLabelEXT(VkCommandBuffer commandBuffer) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdInsertDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdInsertDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetMemoryHostPointerPropertiesEXT(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, const void* pHostPointer, VkMemoryHostPointerPropertiesEXT* pMemoryHostPointerProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetMemoryHostPointerPropertiesEXT(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, const void* pHostPointer, VkMemoryHostPointerPropertiesEXT* pMemoryHostPointerProperties) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdWriteBufferMarkerAMD(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdWriteBufferMarkerAMD(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateRenderPass2(VkDevice device, const VkRenderPassCreateInfo2* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateRenderPass2(VkDevice device, const VkRenderPassCreateInfo2* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateRenderPass2KHR(VkDevice device, const VkRenderPassCreateInfo2* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateRenderPass2KHR(VkDevice device, const VkRenderPassCreateInfo2* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdBeginRenderPass2(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo*      pRenderPassBegin, const VkSubpassBeginInfo*      pSubpassBeginInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdBeginRenderPass2(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo*      pRenderPassBegin, const VkSubpassBeginInfo*      pSubpassBeginInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdBeginRenderPass2KHR(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo*      pRenderPassBegin, const VkSubpassBeginInfo*      pSubpassBeginInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdBeginRenderPass2KHR(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo*      pRenderPassBegin, const VkSubpassBeginInfo*      pSubpassBeginInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdNextSubpass2(VkCommandBuffer commandBuffer, const VkSubpassBeginInfo*      pSubpassBeginInfo, const VkSubpassEndInfo*        pSubpassEndInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdNextSubpass2(VkCommandBuffer commandBuffer, const VkSubpassBeginInfo*      pSubpassBeginInfo, const VkSubpassEndInfo*        pSubpassEndInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdNextSubpass2KHR(VkCommandBuffer commandBuffer, const VkSubpassBeginInfo*      pSubpassBeginInfo, const VkSubpassEndInfo*        pSubpassEndInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdNextSubpass2KHR(VkCommandBuffer commandBuffer, const VkSubpassBeginInfo*      pSubpassBeginInfo, const VkSubpassEndInfo*        pSubpassEndInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdEndRenderPass2(VkCommandBuffer commandBuffer, const VkSubpassEndInfo*        pSubpassEndInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdEndRenderPass2(VkCommandBuffer commandBuffer, const VkSubpassEndInfo*        pSubpassEndInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdEndRenderPass2KHR(VkCommandBuffer commandBuffer, const VkSubpassEndInfo*        pSubpassEndInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdEndRenderPass2KHR(VkCommandBuffer commandBuffer, const VkSubpassEndInfo*        pSubpassEndInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetSemaphoreCounterValue(VkDevice device, VkSemaphore semaphore, uint64_t* pValue) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetSemaphoreCounterValue(VkDevice device, VkSemaphore semaphore, uint64_t* pValue) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetSemaphoreCounterValueKHR(VkDevice device, VkSemaphore semaphore, uint64_t* pValue) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetSemaphoreCounterValueKHR(VkDevice device, VkSemaphore semaphore, uint64_t* pValue) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_WaitSemaphores(VkDevice device, const VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_WaitSemaphores(VkDevice device, const VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_WaitSemaphoresKHR(VkDevice device, const VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_WaitSemaphoresKHR(VkDevice device, const VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_SignalSemaphore(VkDevice device, const VkSemaphoreSignalInfo* pSignalInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_SignalSemaphore(VkDevice device, const VkSemaphoreSignalInfo* pSignalInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_SignalSemaphoreKHR(VkDevice device, const VkSemaphoreSignalInfo* pSignalInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_SignalSemaphoreKHR(VkDevice device, const VkSemaphoreSignalInfo* pSignalInfo) ATTR_WEAK;


#ifdef VK_USE_PLATFORM_ANDROID_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetAndroidHardwareBufferPropertiesANDROID(VkDevice device, const struct AHardwareBuffer* buffer, VkAndroidHardwareBufferPropertiesANDROID* pProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetAndroidHardwareBufferPropertiesANDROID(VkDevice device, const struct AHardwareBuffer* buffer, VkAndroidHardwareBufferPropertiesANDROID* pProperties) ATTR_WEAK;


#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetMemoryAndroidHardwareBufferANDROID(VkDevice device, const VkMemoryGetAndroidHardwareBufferInfoANDROID* pInfo, struct AHardwareBuffer** pBuffer) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetMemoryAndroidHardwareBufferANDROID(VkDevice device, const VkMemoryGetAndroidHardwareBufferInfoANDROID* pInfo, struct AHardwareBuffer** pBuffer) ATTR_WEAK;


#endif // VK_USE_PLATFORM_ANDROID_KHR
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdDrawIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdDrawIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdDrawIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdDrawIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdDrawIndirectCountAMD(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdDrawIndirectCountAMD(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdDrawIndexedIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdDrawIndexedIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdDrawIndexedIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdDrawIndexedIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdDrawIndexedIndirectCountAMD(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdDrawIndexedIndirectCountAMD(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetCheckpointNV(VkCommandBuffer commandBuffer, const void* pCheckpointMarker) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetCheckpointNV(VkCommandBuffer commandBuffer, const void* pCheckpointMarker) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetQueueCheckpointDataNV(VkQueue queue, uint32_t* pCheckpointDataCount, VkCheckpointDataNV* pCheckpointData) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetQueueCheckpointDataNV(VkQueue queue, uint32_t* pCheckpointDataCount, VkCheckpointDataNV* pCheckpointData) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdBindTransformFeedbackBuffersEXT(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdBindTransformFeedbackBuffersEXT(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdBeginTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdBeginTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdEndTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdEndTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdBeginQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags, uint32_t index) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdBeginQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags, uint32_t index) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdEndQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, uint32_t index) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdEndQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, uint32_t index) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdDrawIndirectByteCountEXT(VkCommandBuffer commandBuffer, uint32_t instanceCount, uint32_t firstInstance, VkBuffer counterBuffer, VkDeviceSize counterBufferOffset, uint32_t counterOffset, uint32_t vertexStride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdDrawIndirectByteCountEXT(VkCommandBuffer commandBuffer, uint32_t instanceCount, uint32_t firstInstance, VkBuffer counterBuffer, VkDeviceSize counterBufferOffset, uint32_t counterOffset, uint32_t vertexStride) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetExclusiveScissorNV(VkCommandBuffer commandBuffer, uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, const VkRect2D* pExclusiveScissors) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetExclusiveScissorNV(VkCommandBuffer commandBuffer, uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, const VkRect2D* pExclusiveScissors) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetExclusiveScissorEnableNV(VkCommandBuffer commandBuffer, uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, const VkBool32* pExclusiveScissorEnables) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetExclusiveScissorEnableNV(VkCommandBuffer commandBuffer, uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, const VkBool32* pExclusiveScissorEnables) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdBindShadingRateImageNV(VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdBindShadingRateImageNV(VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetViewportShadingRatePaletteNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkShadingRatePaletteNV* pShadingRatePalettes) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetViewportShadingRatePaletteNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkShadingRatePaletteNV* pShadingRatePalettes) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetCoarseSampleOrderNV(VkCommandBuffer commandBuffer, VkCoarseSampleOrderTypeNV sampleOrderType, uint32_t customSampleOrderCount, const VkCoarseSampleOrderCustomNV* pCustomSampleOrders) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetCoarseSampleOrderNV(VkCommandBuffer commandBuffer, VkCoarseSampleOrderTypeNV sampleOrderType, uint32_t customSampleOrderCount, const VkCoarseSampleOrderCustomNV* pCustomSampleOrders) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdDrawMeshTasksNV(VkCommandBuffer commandBuffer, uint32_t taskCount, uint32_t firstTask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdDrawMeshTasksNV(VkCommandBuffer commandBuffer, uint32_t taskCount, uint32_t firstTask) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdDrawMeshTasksIndirectNV(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdDrawMeshTasksIndirectNV(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdDrawMeshTasksIndirectCountNV(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdDrawMeshTasksIndirectCountNV(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdDrawMeshTasksEXT(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdDrawMeshTasksEXT(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdDrawMeshTasksIndirectEXT(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdDrawMeshTasksIndirectEXT(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdDrawMeshTasksIndirectCountEXT(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdDrawMeshTasksIndirectCountEXT(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CompileDeferredNV(VkDevice device, VkPipeline pipeline, uint32_t shader) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CompileDeferredNV(VkDevice device, VkPipeline pipeline, uint32_t shader) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateAccelerationStructureNV(VkDevice device, const VkAccelerationStructureCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkAccelerationStructureNV* pAccelerationStructure) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateAccelerationStructureNV(VkDevice device, const VkAccelerationStructureCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkAccelerationStructureNV* pAccelerationStructure) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdBindInvocationMaskHUAWEI(VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdBindInvocationMaskHUAWEI(VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_DestroyAccelerationStructureKHR(VkDevice device, VkAccelerationStructureKHR accelerationStructure, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_DestroyAccelerationStructureKHR(VkDevice device, VkAccelerationStructureKHR accelerationStructure, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_DestroyAccelerationStructureNV(VkDevice device, VkAccelerationStructureNV accelerationStructure, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_DestroyAccelerationStructureNV(VkDevice device, VkAccelerationStructureNV accelerationStructure, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetAccelerationStructureMemoryRequirementsNV(VkDevice device, const VkAccelerationStructureMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2KHR* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetAccelerationStructureMemoryRequirementsNV(VkDevice device, const VkAccelerationStructureMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2KHR* pMemoryRequirements) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_BindAccelerationStructureMemoryNV(VkDevice device, uint32_t bindInfoCount, const VkBindAccelerationStructureMemoryInfoNV* pBindInfos) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_BindAccelerationStructureMemoryNV(VkDevice device, uint32_t bindInfoCount, const VkBindAccelerationStructureMemoryInfoNV* pBindInfos) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdCopyAccelerationStructureNV(VkCommandBuffer commandBuffer, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkCopyAccelerationStructureModeKHR mode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdCopyAccelerationStructureNV(VkCommandBuffer commandBuffer, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkCopyAccelerationStructureModeKHR mode) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdCopyAccelerationStructureKHR(VkCommandBuffer commandBuffer, const VkCopyAccelerationStructureInfoKHR* pInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdCopyAccelerationStructureKHR(VkCommandBuffer commandBuffer, const VkCopyAccelerationStructureInfoKHR* pInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CopyAccelerationStructureKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyAccelerationStructureInfoKHR* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CopyAccelerationStructureKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyAccelerationStructureInfoKHR* pInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdCopyAccelerationStructureToMemoryKHR(VkCommandBuffer commandBuffer, const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdCopyAccelerationStructureToMemoryKHR(VkCommandBuffer commandBuffer, const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CopyAccelerationStructureToMemoryKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CopyAccelerationStructureToMemoryKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdCopyMemoryToAccelerationStructureKHR(VkCommandBuffer commandBuffer, const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdCopyMemoryToAccelerationStructureKHR(VkCommandBuffer commandBuffer, const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CopyMemoryToAccelerationStructureKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CopyMemoryToAccelerationStructureKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdWriteAccelerationStructuresPropertiesKHR(VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, const VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdWriteAccelerationStructuresPropertiesKHR(VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, const VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdWriteAccelerationStructuresPropertiesNV(VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, const VkAccelerationStructureNV* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdWriteAccelerationStructuresPropertiesNV(VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, const VkAccelerationStructureNV* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdBuildAccelerationStructureNV(VkCommandBuffer commandBuffer, const VkAccelerationStructureInfoNV* pInfo, VkBuffer instanceData, VkDeviceSize instanceOffset, VkBool32 update, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkBuffer scratch, VkDeviceSize scratchOffset) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdBuildAccelerationStructureNV(VkCommandBuffer commandBuffer, const VkAccelerationStructureInfoNV* pInfo, VkBuffer instanceData, VkDeviceSize instanceOffset, VkBool32 update, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkBuffer scratch, VkDeviceSize scratchOffset) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_WriteAccelerationStructuresPropertiesKHR(VkDevice device, uint32_t accelerationStructureCount, const VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType  queryType, size_t       dataSize, void* pData, size_t stride) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_WriteAccelerationStructuresPropertiesKHR(VkDevice device, uint32_t accelerationStructureCount, const VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType  queryType, size_t       dataSize, void* pData, size_t stride) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdTraceRaysKHR(VkCommandBuffer commandBuffer, const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, uint32_t width, uint32_t height, uint32_t depth) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdTraceRaysKHR(VkCommandBuffer commandBuffer, const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, uint32_t width, uint32_t height, uint32_t depth) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdTraceRaysNV(VkCommandBuffer commandBuffer, VkBuffer raygenShaderBindingTableBuffer, VkDeviceSize raygenShaderBindingOffset, VkBuffer missShaderBindingTableBuffer, VkDeviceSize missShaderBindingOffset, VkDeviceSize missShaderBindingStride, VkBuffer hitShaderBindingTableBuffer, VkDeviceSize hitShaderBindingOffset, VkDeviceSize hitShaderBindingStride, VkBuffer callableShaderBindingTableBuffer, VkDeviceSize callableShaderBindingOffset, VkDeviceSize callableShaderBindingStride, uint32_t width, uint32_t height, uint32_t depth) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdTraceRaysNV(VkCommandBuffer commandBuffer, VkBuffer raygenShaderBindingTableBuffer, VkDeviceSize raygenShaderBindingOffset, VkBuffer missShaderBindingTableBuffer, VkDeviceSize missShaderBindingOffset, VkDeviceSize missShaderBindingStride, VkBuffer hitShaderBindingTableBuffer, VkDeviceSize hitShaderBindingOffset, VkDeviceSize hitShaderBindingStride, VkBuffer callableShaderBindingTableBuffer, VkDeviceSize callableShaderBindingOffset, VkDeviceSize callableShaderBindingStride, uint32_t width, uint32_t height, uint32_t depth) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetRayTracingShaderGroupHandlesKHR(VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetRayTracingShaderGroupHandlesKHR(VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetRayTracingShaderGroupHandlesNV(VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetRayTracingShaderGroupHandlesNV(VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetRayTracingCaptureReplayShaderGroupHandlesKHR(VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetRayTracingCaptureReplayShaderGroupHandlesKHR(VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetAccelerationStructureHandleNV(VkDevice device, VkAccelerationStructureNV accelerationStructure, size_t dataSize, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetAccelerationStructureHandleNV(VkDevice device, VkAccelerationStructureNV accelerationStructure, size_t dataSize, void* pData) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateRayTracingPipelinesNV(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkRayTracingPipelineCreateInfoNV* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateRayTracingPipelinesNV(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkRayTracingPipelineCreateInfoNV* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateRayTracingPipelinesKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkRayTracingPipelineCreateInfoKHR* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateRayTracingPipelinesKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkRayTracingPipelineCreateInfoKHR* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdTraceRaysIndirectKHR(VkCommandBuffer commandBuffer, const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, VkDeviceAddress indirectDeviceAddress) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdTraceRaysIndirectKHR(VkCommandBuffer commandBuffer, const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, VkDeviceAddress indirectDeviceAddress) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdTraceRaysIndirect2KHR(VkCommandBuffer commandBuffer, VkDeviceAddress indirectDeviceAddress) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdTraceRaysIndirect2KHR(VkCommandBuffer commandBuffer, VkDeviceAddress indirectDeviceAddress) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetDeviceAccelerationStructureCompatibilityKHR(VkDevice device, const VkAccelerationStructureVersionInfoKHR* pVersionInfo, VkAccelerationStructureCompatibilityKHR* pCompatibility) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetDeviceAccelerationStructureCompatibilityKHR(VkDevice device, const VkAccelerationStructureVersionInfoKHR* pVersionInfo, VkAccelerationStructureCompatibilityKHR* pCompatibility) ATTR_WEAK;


  VKAPI_ATTR VkDeviceSize VKAPI_CALL vk_cmd_enqueue_GetRayTracingShaderGroupStackSizeKHR(VkDevice device, VkPipeline pipeline, uint32_t group, VkShaderGroupShaderKHR groupShader) ATTR_WEAK;
  VKAPI_ATTR VkDeviceSize VKAPI_CALL vk_cmd_enqueue_unless_primary_GetRayTracingShaderGroupStackSizeKHR(VkDevice device, VkPipeline pipeline, uint32_t group, VkShaderGroupShaderKHR groupShader) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetRayTracingPipelineStackSizeKHR(VkCommandBuffer commandBuffer, uint32_t pipelineStackSize) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetRayTracingPipelineStackSizeKHR(VkCommandBuffer commandBuffer, uint32_t pipelineStackSize) ATTR_WEAK;


  VKAPI_ATTR uint32_t VKAPI_CALL vk_cmd_enqueue_GetImageViewHandleNVX(VkDevice device, const VkImageViewHandleInfoNVX* pInfo) ATTR_WEAK;
  VKAPI_ATTR uint32_t VKAPI_CALL vk_cmd_enqueue_unless_primary_GetImageViewHandleNVX(VkDevice device, const VkImageViewHandleInfoNVX* pInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetImageViewAddressNVX(VkDevice device, VkImageView imageView, VkImageViewAddressPropertiesNVX* pProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetImageViewAddressNVX(VkDevice device, VkImageView imageView, VkImageViewAddressPropertiesNVX* pProperties) ATTR_WEAK;


#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetDeviceGroupSurfacePresentModes2EXT(VkDevice device, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkDeviceGroupPresentModeFlagsKHR* pModes) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetDeviceGroupSurfacePresentModes2EXT(VkDevice device, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkDeviceGroupPresentModeFlagsKHR* pModes) ATTR_WEAK;


#endif // VK_USE_PLATFORM_WIN32_KHR
#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_AcquireFullScreenExclusiveModeEXT(VkDevice device, VkSwapchainKHR swapchain) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_AcquireFullScreenExclusiveModeEXT(VkDevice device, VkSwapchainKHR swapchain) ATTR_WEAK;


#endif // VK_USE_PLATFORM_WIN32_KHR
#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_ReleaseFullScreenExclusiveModeEXT(VkDevice device, VkSwapchainKHR swapchain) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_ReleaseFullScreenExclusiveModeEXT(VkDevice device, VkSwapchainKHR swapchain) ATTR_WEAK;


#endif // VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_AcquireProfilingLockKHR(VkDevice device, const VkAcquireProfilingLockInfoKHR* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_AcquireProfilingLockKHR(VkDevice device, const VkAcquireProfilingLockInfoKHR* pInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_ReleaseProfilingLockKHR(VkDevice device) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_ReleaseProfilingLockKHR(VkDevice device) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetImageDrmFormatModifierPropertiesEXT(VkDevice device, VkImage image, VkImageDrmFormatModifierPropertiesEXT* pProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetImageDrmFormatModifierPropertiesEXT(VkDevice device, VkImage image, VkImageDrmFormatModifierPropertiesEXT* pProperties) ATTR_WEAK;


  VKAPI_ATTR uint64_t VKAPI_CALL vk_cmd_enqueue_GetBufferOpaqueCaptureAddress(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) ATTR_WEAK;
  VKAPI_ATTR uint64_t VKAPI_CALL vk_cmd_enqueue_unless_primary_GetBufferOpaqueCaptureAddress(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) ATTR_WEAK;


  VKAPI_ATTR uint64_t VKAPI_CALL vk_cmd_enqueue_GetBufferOpaqueCaptureAddressKHR(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) ATTR_WEAK;
  VKAPI_ATTR uint64_t VKAPI_CALL vk_cmd_enqueue_unless_primary_GetBufferOpaqueCaptureAddressKHR(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) ATTR_WEAK;


  VKAPI_ATTR VkDeviceAddress VKAPI_CALL vk_cmd_enqueue_GetBufferDeviceAddress(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkDeviceAddress VKAPI_CALL vk_cmd_enqueue_unless_primary_GetBufferDeviceAddress(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) ATTR_WEAK;


  VKAPI_ATTR VkDeviceAddress VKAPI_CALL vk_cmd_enqueue_GetBufferDeviceAddressKHR(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkDeviceAddress VKAPI_CALL vk_cmd_enqueue_unless_primary_GetBufferDeviceAddressKHR(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) ATTR_WEAK;


  VKAPI_ATTR VkDeviceAddress VKAPI_CALL vk_cmd_enqueue_GetBufferDeviceAddressEXT(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkDeviceAddress VKAPI_CALL vk_cmd_enqueue_unless_primary_GetBufferDeviceAddressEXT(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_InitializePerformanceApiINTEL(VkDevice device, const VkInitializePerformanceApiInfoINTEL* pInitializeInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_InitializePerformanceApiINTEL(VkDevice device, const VkInitializePerformanceApiInfoINTEL* pInitializeInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_UninitializePerformanceApiINTEL(VkDevice device) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_UninitializePerformanceApiINTEL(VkDevice device) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CmdSetPerformanceMarkerINTEL(VkCommandBuffer commandBuffer, const VkPerformanceMarkerInfoINTEL* pMarkerInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetPerformanceMarkerINTEL(VkCommandBuffer commandBuffer, const VkPerformanceMarkerInfoINTEL* pMarkerInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CmdSetPerformanceStreamMarkerINTEL(VkCommandBuffer commandBuffer, const VkPerformanceStreamMarkerInfoINTEL* pMarkerInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetPerformanceStreamMarkerINTEL(VkCommandBuffer commandBuffer, const VkPerformanceStreamMarkerInfoINTEL* pMarkerInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CmdSetPerformanceOverrideINTEL(VkCommandBuffer commandBuffer, const VkPerformanceOverrideInfoINTEL* pOverrideInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetPerformanceOverrideINTEL(VkCommandBuffer commandBuffer, const VkPerformanceOverrideInfoINTEL* pOverrideInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_AcquirePerformanceConfigurationINTEL(VkDevice device, const VkPerformanceConfigurationAcquireInfoINTEL* pAcquireInfo, VkPerformanceConfigurationINTEL* pConfiguration) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_AcquirePerformanceConfigurationINTEL(VkDevice device, const VkPerformanceConfigurationAcquireInfoINTEL* pAcquireInfo, VkPerformanceConfigurationINTEL* pConfiguration) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_ReleasePerformanceConfigurationINTEL(VkDevice device, VkPerformanceConfigurationINTEL configuration) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_ReleasePerformanceConfigurationINTEL(VkDevice device, VkPerformanceConfigurationINTEL configuration) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_QueueSetPerformanceConfigurationINTEL(VkQueue queue, VkPerformanceConfigurationINTEL configuration) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_QueueSetPerformanceConfigurationINTEL(VkQueue queue, VkPerformanceConfigurationINTEL configuration) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetPerformanceParameterINTEL(VkDevice device, VkPerformanceParameterTypeINTEL parameter, VkPerformanceValueINTEL* pValue) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPerformanceParameterINTEL(VkDevice device, VkPerformanceParameterTypeINTEL parameter, VkPerformanceValueINTEL* pValue) ATTR_WEAK;


  VKAPI_ATTR uint64_t VKAPI_CALL vk_cmd_enqueue_GetDeviceMemoryOpaqueCaptureAddress(VkDevice device, const VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo) ATTR_WEAK;
  VKAPI_ATTR uint64_t VKAPI_CALL vk_cmd_enqueue_unless_primary_GetDeviceMemoryOpaqueCaptureAddress(VkDevice device, const VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo) ATTR_WEAK;


  VKAPI_ATTR uint64_t VKAPI_CALL vk_cmd_enqueue_GetDeviceMemoryOpaqueCaptureAddressKHR(VkDevice device, const VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo) ATTR_WEAK;
  VKAPI_ATTR uint64_t VKAPI_CALL vk_cmd_enqueue_unless_primary_GetDeviceMemoryOpaqueCaptureAddressKHR(VkDevice device, const VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetPipelineExecutablePropertiesKHR(VkDevice                        device, const VkPipelineInfoKHR*        pPipelineInfo, uint32_t* pExecutableCount, VkPipelineExecutablePropertiesKHR* pProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPipelineExecutablePropertiesKHR(VkDevice                        device, const VkPipelineInfoKHR*        pPipelineInfo, uint32_t* pExecutableCount, VkPipelineExecutablePropertiesKHR* pProperties) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetPipelineExecutableStatisticsKHR(VkDevice                        device, const VkPipelineExecutableInfoKHR*  pExecutableInfo, uint32_t* pStatisticCount, VkPipelineExecutableStatisticKHR* pStatistics) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPipelineExecutableStatisticsKHR(VkDevice                        device, const VkPipelineExecutableInfoKHR*  pExecutableInfo, uint32_t* pStatisticCount, VkPipelineExecutableStatisticKHR* pStatistics) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetPipelineExecutableInternalRepresentationsKHR(VkDevice                        device, const VkPipelineExecutableInfoKHR*  pExecutableInfo, uint32_t* pInternalRepresentationCount, VkPipelineExecutableInternalRepresentationKHR* pInternalRepresentations) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPipelineExecutableInternalRepresentationsKHR(VkDevice                        device, const VkPipelineExecutableInfoKHR*  pExecutableInfo, uint32_t* pInternalRepresentationCount, VkPipelineExecutableInternalRepresentationKHR* pInternalRepresentations) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetLineStippleEXT(VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetLineStippleEXT(VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateAccelerationStructureKHR(VkDevice                                           device, const VkAccelerationStructureCreateInfoKHR*        pCreateInfo, const VkAllocationCallbacks*       pAllocator, VkAccelerationStructureKHR*                        pAccelerationStructure) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateAccelerationStructureKHR(VkDevice                                           device, const VkAccelerationStructureCreateInfoKHR*        pCreateInfo, const VkAllocationCallbacks*       pAllocator, VkAccelerationStructureKHR*                        pAccelerationStructure) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdBuildAccelerationStructuresKHR(VkCommandBuffer                                    commandBuffer, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdBuildAccelerationStructuresKHR(VkCommandBuffer                                    commandBuffer, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdBuildAccelerationStructuresIndirectKHR(VkCommandBuffer                  commandBuffer, uint32_t                                           infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkDeviceAddress*             pIndirectDeviceAddresses, const uint32_t*                    pIndirectStrides, const uint32_t* const*             ppMaxPrimitiveCounts) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdBuildAccelerationStructuresIndirectKHR(VkCommandBuffer                  commandBuffer, uint32_t                                           infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkDeviceAddress*             pIndirectDeviceAddresses, const uint32_t*                    pIndirectStrides, const uint32_t* const*             ppMaxPrimitiveCounts) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_BuildAccelerationStructuresKHR(VkDevice                                           device, VkDeferredOperationKHR deferredOperation, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_BuildAccelerationStructuresKHR(VkDevice                                           device, VkDeferredOperationKHR deferredOperation, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos) ATTR_WEAK;


  VKAPI_ATTR VkDeviceAddress VKAPI_CALL vk_cmd_enqueue_GetAccelerationStructureDeviceAddressKHR(VkDevice device, const VkAccelerationStructureDeviceAddressInfoKHR* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkDeviceAddress VKAPI_CALL vk_cmd_enqueue_unless_primary_GetAccelerationStructureDeviceAddressKHR(VkDevice device, const VkAccelerationStructureDeviceAddressInfoKHR* pInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateDeferredOperationKHR(VkDevice device, const VkAllocationCallbacks* pAllocator, VkDeferredOperationKHR* pDeferredOperation) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateDeferredOperationKHR(VkDevice device, const VkAllocationCallbacks* pAllocator, VkDeferredOperationKHR* pDeferredOperation) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_DestroyDeferredOperationKHR(VkDevice device, VkDeferredOperationKHR operation, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_DestroyDeferredOperationKHR(VkDevice device, VkDeferredOperationKHR operation, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR uint32_t VKAPI_CALL vk_cmd_enqueue_GetDeferredOperationMaxConcurrencyKHR(VkDevice device, VkDeferredOperationKHR operation) ATTR_WEAK;
  VKAPI_ATTR uint32_t VKAPI_CALL vk_cmd_enqueue_unless_primary_GetDeferredOperationMaxConcurrencyKHR(VkDevice device, VkDeferredOperationKHR operation) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetDeferredOperationResultKHR(VkDevice device, VkDeferredOperationKHR operation) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetDeferredOperationResultKHR(VkDevice device, VkDeferredOperationKHR operation) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_DeferredOperationJoinKHR(VkDevice device, VkDeferredOperationKHR operation) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_DeferredOperationJoinKHR(VkDevice device, VkDeferredOperationKHR operation) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetPipelineIndirectMemoryRequirementsNV(VkDevice device, const VkComputePipelineCreateInfo* pCreateInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPipelineIndirectMemoryRequirementsNV(VkDevice device, const VkComputePipelineCreateInfo* pCreateInfo, VkMemoryRequirements2* pMemoryRequirements) ATTR_WEAK;


  VKAPI_ATTR VkDeviceAddress VKAPI_CALL vk_cmd_enqueue_GetPipelineIndirectDeviceAddressNV(VkDevice device, const VkPipelineIndirectDeviceAddressInfoNV* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkDeviceAddress VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPipelineIndirectDeviceAddressNV(VkDevice device, const VkPipelineIndirectDeviceAddressInfoNV* pInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetCullMode(VkCommandBuffer commandBuffer, VkCullModeFlags cullMode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetCullMode(VkCommandBuffer commandBuffer, VkCullModeFlags cullMode) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetCullModeEXT(VkCommandBuffer commandBuffer, VkCullModeFlags cullMode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetCullModeEXT(VkCommandBuffer commandBuffer, VkCullModeFlags cullMode) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetFrontFace(VkCommandBuffer commandBuffer, VkFrontFace frontFace) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetFrontFace(VkCommandBuffer commandBuffer, VkFrontFace frontFace) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetFrontFaceEXT(VkCommandBuffer commandBuffer, VkFrontFace frontFace) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetFrontFaceEXT(VkCommandBuffer commandBuffer, VkFrontFace frontFace) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetPrimitiveTopology(VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetPrimitiveTopology(VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetPrimitiveTopologyEXT(VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetPrimitiveTopologyEXT(VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetViewportWithCount(VkCommandBuffer commandBuffer, uint32_t viewportCount, const VkViewport* pViewports) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetViewportWithCount(VkCommandBuffer commandBuffer, uint32_t viewportCount, const VkViewport* pViewports) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetViewportWithCountEXT(VkCommandBuffer commandBuffer, uint32_t viewportCount, const VkViewport* pViewports) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetViewportWithCountEXT(VkCommandBuffer commandBuffer, uint32_t viewportCount, const VkViewport* pViewports) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetScissorWithCount(VkCommandBuffer commandBuffer, uint32_t scissorCount, const VkRect2D* pScissors) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetScissorWithCount(VkCommandBuffer commandBuffer, uint32_t scissorCount, const VkRect2D* pScissors) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetScissorWithCountEXT(VkCommandBuffer commandBuffer, uint32_t scissorCount, const VkRect2D* pScissors) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetScissorWithCountEXT(VkCommandBuffer commandBuffer, uint32_t scissorCount, const VkRect2D* pScissors) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdBindIndexBuffer2KHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkDeviceSize size, VkIndexType indexType) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdBindIndexBuffer2KHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkDeviceSize size, VkIndexType indexType) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdBindVertexBuffers2(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes, const VkDeviceSize* pStrides) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdBindVertexBuffers2(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes, const VkDeviceSize* pStrides) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdBindVertexBuffers2EXT(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes, const VkDeviceSize* pStrides) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdBindVertexBuffers2EXT(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes, const VkDeviceSize* pStrides) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetDepthTestEnable(VkCommandBuffer commandBuffer, VkBool32 depthTestEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetDepthTestEnable(VkCommandBuffer commandBuffer, VkBool32 depthTestEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetDepthTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthTestEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetDepthTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthTestEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetDepthWriteEnable(VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetDepthWriteEnable(VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetDepthWriteEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetDepthWriteEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetDepthCompareOp(VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetDepthCompareOp(VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetDepthCompareOpEXT(VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetDepthCompareOpEXT(VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetDepthBoundsTestEnable(VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetDepthBoundsTestEnable(VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetDepthBoundsTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetDepthBoundsTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetStencilTestEnable(VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetStencilTestEnable(VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetStencilTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetStencilTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetStencilOp(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetStencilOp(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetStencilOpEXT(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetStencilOpEXT(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetPatchControlPointsEXT(VkCommandBuffer commandBuffer, uint32_t patchControlPoints) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetPatchControlPointsEXT(VkCommandBuffer commandBuffer, uint32_t patchControlPoints) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetRasterizerDiscardEnable(VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetRasterizerDiscardEnable(VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetRasterizerDiscardEnableEXT(VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetRasterizerDiscardEnableEXT(VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetDepthBiasEnable(VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetDepthBiasEnable(VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetDepthBiasEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetDepthBiasEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetLogicOpEXT(VkCommandBuffer commandBuffer, VkLogicOp logicOp) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetLogicOpEXT(VkCommandBuffer commandBuffer, VkLogicOp logicOp) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetPrimitiveRestartEnable(VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetPrimitiveRestartEnable(VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetPrimitiveRestartEnableEXT(VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetPrimitiveRestartEnableEXT(VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetTessellationDomainOriginEXT(VkCommandBuffer commandBuffer, VkTessellationDomainOrigin domainOrigin) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetTessellationDomainOriginEXT(VkCommandBuffer commandBuffer, VkTessellationDomainOrigin domainOrigin) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetDepthClampEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthClampEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetDepthClampEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthClampEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetPolygonModeEXT(VkCommandBuffer commandBuffer, VkPolygonMode polygonMode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetPolygonModeEXT(VkCommandBuffer commandBuffer, VkPolygonMode polygonMode) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetRasterizationSamplesEXT(VkCommandBuffer commandBuffer, VkSampleCountFlagBits  rasterizationSamples) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetRasterizationSamplesEXT(VkCommandBuffer commandBuffer, VkSampleCountFlagBits  rasterizationSamples) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetSampleMaskEXT(VkCommandBuffer commandBuffer, VkSampleCountFlagBits  samples, const VkSampleMask*    pSampleMask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetSampleMaskEXT(VkCommandBuffer commandBuffer, VkSampleCountFlagBits  samples, const VkSampleMask*    pSampleMask) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetAlphaToCoverageEnableEXT(VkCommandBuffer commandBuffer, VkBool32 alphaToCoverageEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetAlphaToCoverageEnableEXT(VkCommandBuffer commandBuffer, VkBool32 alphaToCoverageEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetAlphaToOneEnableEXT(VkCommandBuffer commandBuffer, VkBool32 alphaToOneEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetAlphaToOneEnableEXT(VkCommandBuffer commandBuffer, VkBool32 alphaToOneEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetLogicOpEnableEXT(VkCommandBuffer commandBuffer, VkBool32 logicOpEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetLogicOpEnableEXT(VkCommandBuffer commandBuffer, VkBool32 logicOpEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetColorBlendEnableEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkBool32* pColorBlendEnables) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetColorBlendEnableEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkBool32* pColorBlendEnables) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetColorBlendEquationEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorBlendEquationEXT* pColorBlendEquations) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetColorBlendEquationEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorBlendEquationEXT* pColorBlendEquations) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetColorWriteMaskEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorComponentFlags* pColorWriteMasks) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetColorWriteMaskEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorComponentFlags* pColorWriteMasks) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetRasterizationStreamEXT(VkCommandBuffer commandBuffer, uint32_t rasterizationStream) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetRasterizationStreamEXT(VkCommandBuffer commandBuffer, uint32_t rasterizationStream) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetConservativeRasterizationModeEXT(VkCommandBuffer commandBuffer, VkConservativeRasterizationModeEXT conservativeRasterizationMode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetConservativeRasterizationModeEXT(VkCommandBuffer commandBuffer, VkConservativeRasterizationModeEXT conservativeRasterizationMode) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetExtraPrimitiveOverestimationSizeEXT(VkCommandBuffer commandBuffer, float extraPrimitiveOverestimationSize) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetExtraPrimitiveOverestimationSizeEXT(VkCommandBuffer commandBuffer, float extraPrimitiveOverestimationSize) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetDepthClipEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthClipEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetDepthClipEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthClipEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetSampleLocationsEnableEXT(VkCommandBuffer commandBuffer, VkBool32 sampleLocationsEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetSampleLocationsEnableEXT(VkCommandBuffer commandBuffer, VkBool32 sampleLocationsEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetColorBlendAdvancedEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorBlendAdvancedEXT* pColorBlendAdvanced) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetColorBlendAdvancedEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorBlendAdvancedEXT* pColorBlendAdvanced) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetProvokingVertexModeEXT(VkCommandBuffer commandBuffer, VkProvokingVertexModeEXT provokingVertexMode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetProvokingVertexModeEXT(VkCommandBuffer commandBuffer, VkProvokingVertexModeEXT provokingVertexMode) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetLineRasterizationModeEXT(VkCommandBuffer commandBuffer, VkLineRasterizationModeEXT lineRasterizationMode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetLineRasterizationModeEXT(VkCommandBuffer commandBuffer, VkLineRasterizationModeEXT lineRasterizationMode) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetLineStippleEnableEXT(VkCommandBuffer commandBuffer, VkBool32 stippledLineEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetLineStippleEnableEXT(VkCommandBuffer commandBuffer, VkBool32 stippledLineEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetDepthClipNegativeOneToOneEXT(VkCommandBuffer commandBuffer, VkBool32 negativeOneToOne) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetDepthClipNegativeOneToOneEXT(VkCommandBuffer commandBuffer, VkBool32 negativeOneToOne) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetViewportWScalingEnableNV(VkCommandBuffer commandBuffer, VkBool32 viewportWScalingEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetViewportWScalingEnableNV(VkCommandBuffer commandBuffer, VkBool32 viewportWScalingEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetViewportSwizzleNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewportSwizzleNV* pViewportSwizzles) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetViewportSwizzleNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewportSwizzleNV* pViewportSwizzles) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetCoverageToColorEnableNV(VkCommandBuffer commandBuffer, VkBool32 coverageToColorEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetCoverageToColorEnableNV(VkCommandBuffer commandBuffer, VkBool32 coverageToColorEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetCoverageToColorLocationNV(VkCommandBuffer commandBuffer, uint32_t coverageToColorLocation) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetCoverageToColorLocationNV(VkCommandBuffer commandBuffer, uint32_t coverageToColorLocation) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetCoverageModulationModeNV(VkCommandBuffer commandBuffer, VkCoverageModulationModeNV coverageModulationMode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetCoverageModulationModeNV(VkCommandBuffer commandBuffer, VkCoverageModulationModeNV coverageModulationMode) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetCoverageModulationTableEnableNV(VkCommandBuffer commandBuffer, VkBool32 coverageModulationTableEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetCoverageModulationTableEnableNV(VkCommandBuffer commandBuffer, VkBool32 coverageModulationTableEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetCoverageModulationTableNV(VkCommandBuffer commandBuffer, uint32_t coverageModulationTableCount, const float* pCoverageModulationTable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetCoverageModulationTableNV(VkCommandBuffer commandBuffer, uint32_t coverageModulationTableCount, const float* pCoverageModulationTable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetShadingRateImageEnableNV(VkCommandBuffer commandBuffer, VkBool32 shadingRateImageEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetShadingRateImageEnableNV(VkCommandBuffer commandBuffer, VkBool32 shadingRateImageEnable) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetCoverageReductionModeNV(VkCommandBuffer commandBuffer, VkCoverageReductionModeNV coverageReductionMode) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetCoverageReductionModeNV(VkCommandBuffer commandBuffer, VkCoverageReductionModeNV coverageReductionMode) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetRepresentativeFragmentTestEnableNV(VkCommandBuffer commandBuffer, VkBool32 representativeFragmentTestEnable) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetRepresentativeFragmentTestEnableNV(VkCommandBuffer commandBuffer, VkBool32 representativeFragmentTestEnable) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreatePrivateDataSlot(VkDevice device, const VkPrivateDataSlotCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPrivateDataSlot* pPrivateDataSlot) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreatePrivateDataSlot(VkDevice device, const VkPrivateDataSlotCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPrivateDataSlot* pPrivateDataSlot) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreatePrivateDataSlotEXT(VkDevice device, const VkPrivateDataSlotCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPrivateDataSlot* pPrivateDataSlot) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreatePrivateDataSlotEXT(VkDevice device, const VkPrivateDataSlotCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPrivateDataSlot* pPrivateDataSlot) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_DestroyPrivateDataSlot(VkDevice device, VkPrivateDataSlot privateDataSlot, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_DestroyPrivateDataSlot(VkDevice device, VkPrivateDataSlot privateDataSlot, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_DestroyPrivateDataSlotEXT(VkDevice device, VkPrivateDataSlot privateDataSlot, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_DestroyPrivateDataSlotEXT(VkDevice device, VkPrivateDataSlot privateDataSlot, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_SetPrivateData(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t data) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_SetPrivateData(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t data) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_SetPrivateDataEXT(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t data) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_SetPrivateDataEXT(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t data) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetPrivateData(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t* pData) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPrivateData(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t* pData) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetPrivateDataEXT(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t* pData) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPrivateDataEXT(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t* pData) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdCopyBuffer2(VkCommandBuffer commandBuffer, const VkCopyBufferInfo2* pCopyBufferInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdCopyBuffer2(VkCommandBuffer commandBuffer, const VkCopyBufferInfo2* pCopyBufferInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdCopyBuffer2KHR(VkCommandBuffer commandBuffer, const VkCopyBufferInfo2* pCopyBufferInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdCopyBuffer2KHR(VkCommandBuffer commandBuffer, const VkCopyBufferInfo2* pCopyBufferInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdCopyImage2(VkCommandBuffer commandBuffer, const VkCopyImageInfo2* pCopyImageInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdCopyImage2(VkCommandBuffer commandBuffer, const VkCopyImageInfo2* pCopyImageInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdCopyImage2KHR(VkCommandBuffer commandBuffer, const VkCopyImageInfo2* pCopyImageInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdCopyImage2KHR(VkCommandBuffer commandBuffer, const VkCopyImageInfo2* pCopyImageInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdBlitImage2(VkCommandBuffer commandBuffer, const VkBlitImageInfo2* pBlitImageInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdBlitImage2(VkCommandBuffer commandBuffer, const VkBlitImageInfo2* pBlitImageInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdBlitImage2KHR(VkCommandBuffer commandBuffer, const VkBlitImageInfo2* pBlitImageInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdBlitImage2KHR(VkCommandBuffer commandBuffer, const VkBlitImageInfo2* pBlitImageInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdCopyBufferToImage2(VkCommandBuffer commandBuffer, const VkCopyBufferToImageInfo2* pCopyBufferToImageInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdCopyBufferToImage2(VkCommandBuffer commandBuffer, const VkCopyBufferToImageInfo2* pCopyBufferToImageInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdCopyBufferToImage2KHR(VkCommandBuffer commandBuffer, const VkCopyBufferToImageInfo2* pCopyBufferToImageInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdCopyBufferToImage2KHR(VkCommandBuffer commandBuffer, const VkCopyBufferToImageInfo2* pCopyBufferToImageInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdCopyImageToBuffer2(VkCommandBuffer commandBuffer, const VkCopyImageToBufferInfo2* pCopyImageToBufferInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdCopyImageToBuffer2(VkCommandBuffer commandBuffer, const VkCopyImageToBufferInfo2* pCopyImageToBufferInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdCopyImageToBuffer2KHR(VkCommandBuffer commandBuffer, const VkCopyImageToBufferInfo2* pCopyImageToBufferInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdCopyImageToBuffer2KHR(VkCommandBuffer commandBuffer, const VkCopyImageToBufferInfo2* pCopyImageToBufferInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdResolveImage2(VkCommandBuffer commandBuffer, const VkResolveImageInfo2* pResolveImageInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdResolveImage2(VkCommandBuffer commandBuffer, const VkResolveImageInfo2* pResolveImageInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdResolveImage2KHR(VkCommandBuffer commandBuffer, const VkResolveImageInfo2* pResolveImageInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdResolveImage2KHR(VkCommandBuffer commandBuffer, const VkResolveImageInfo2* pResolveImageInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetFragmentShadingRateKHR(VkCommandBuffer           commandBuffer, const VkExtent2D*                           pFragmentSize, const VkFragmentShadingRateCombinerOpKHR    combinerOps[2]) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetFragmentShadingRateKHR(VkCommandBuffer           commandBuffer, const VkExtent2D*                           pFragmentSize, const VkFragmentShadingRateCombinerOpKHR    combinerOps[2]) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetFragmentShadingRateEnumNV(VkCommandBuffer           commandBuffer, VkFragmentShadingRateNV                     shadingRate, const VkFragmentShadingRateCombinerOpKHR    combinerOps[2]) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetFragmentShadingRateEnumNV(VkCommandBuffer           commandBuffer, VkFragmentShadingRateNV                     shadingRate, const VkFragmentShadingRateCombinerOpKHR    combinerOps[2]) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetAccelerationStructureBuildSizesKHR(VkDevice                                            device, VkAccelerationStructureBuildTypeKHR                 buildType, const VkAccelerationStructureBuildGeometryInfoKHR*  pBuildInfo, const uint32_t*  pMaxPrimitiveCounts, VkAccelerationStructureBuildSizesInfoKHR*           pSizeInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetAccelerationStructureBuildSizesKHR(VkDevice                                            device, VkAccelerationStructureBuildTypeKHR                 buildType, const VkAccelerationStructureBuildGeometryInfoKHR*  pBuildInfo, const uint32_t*  pMaxPrimitiveCounts, VkAccelerationStructureBuildSizesInfoKHR*           pSizeInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetVertexInputEXT(VkCommandBuffer commandBuffer, uint32_t vertexBindingDescriptionCount, const VkVertexInputBindingDescription2EXT* pVertexBindingDescriptions, uint32_t vertexAttributeDescriptionCount, const VkVertexInputAttributeDescription2EXT* pVertexAttributeDescriptions) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetVertexInputEXT(VkCommandBuffer commandBuffer, uint32_t vertexBindingDescriptionCount, const VkVertexInputBindingDescription2EXT* pVertexBindingDescriptions, uint32_t vertexAttributeDescriptionCount, const VkVertexInputAttributeDescription2EXT* pVertexAttributeDescriptions) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetColorWriteEnableEXT(VkCommandBuffer       commandBuffer, uint32_t                                attachmentCount, const VkBool32*   pColorWriteEnables) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetColorWriteEnableEXT(VkCommandBuffer       commandBuffer, uint32_t                                attachmentCount, const VkBool32*   pColorWriteEnables) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetEvent2(VkCommandBuffer                   commandBuffer, VkEvent                                             event, const VkDependencyInfo*                             pDependencyInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetEvent2(VkCommandBuffer                   commandBuffer, VkEvent                                             event, const VkDependencyInfo*                             pDependencyInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetEvent2KHR(VkCommandBuffer                   commandBuffer, VkEvent                                             event, const VkDependencyInfo*                             pDependencyInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetEvent2KHR(VkCommandBuffer                   commandBuffer, VkEvent                                             event, const VkDependencyInfo*                             pDependencyInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdResetEvent2(VkCommandBuffer                   commandBuffer, VkEvent                                             event, VkPipelineStageFlags2               stageMask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdResetEvent2(VkCommandBuffer                   commandBuffer, VkEvent                                             event, VkPipelineStageFlags2               stageMask) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdResetEvent2KHR(VkCommandBuffer                   commandBuffer, VkEvent                                             event, VkPipelineStageFlags2               stageMask) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdResetEvent2KHR(VkCommandBuffer                   commandBuffer, VkEvent                                             event, VkPipelineStageFlags2               stageMask) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdWaitEvents2(VkCommandBuffer                   commandBuffer, uint32_t                                            eventCount, const VkEvent*                     pEvents, const VkDependencyInfo*            pDependencyInfos) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdWaitEvents2(VkCommandBuffer                   commandBuffer, uint32_t                                            eventCount, const VkEvent*                     pEvents, const VkDependencyInfo*            pDependencyInfos) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdWaitEvents2KHR(VkCommandBuffer                   commandBuffer, uint32_t                                            eventCount, const VkEvent*                     pEvents, const VkDependencyInfo*            pDependencyInfos) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdWaitEvents2KHR(VkCommandBuffer                   commandBuffer, uint32_t                                            eventCount, const VkEvent*                     pEvents, const VkDependencyInfo*            pDependencyInfos) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdPipelineBarrier2(VkCommandBuffer                   commandBuffer, const VkDependencyInfo*                             pDependencyInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdPipelineBarrier2(VkCommandBuffer                   commandBuffer, const VkDependencyInfo*                             pDependencyInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdPipelineBarrier2KHR(VkCommandBuffer                   commandBuffer, const VkDependencyInfo*                             pDependencyInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdPipelineBarrier2KHR(VkCommandBuffer                   commandBuffer, const VkDependencyInfo*                             pDependencyInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_QueueSubmit2(VkQueue                           queue, uint32_t                            submitCount, const VkSubmitInfo2*              pSubmits, VkFence           fence) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_QueueSubmit2(VkQueue                           queue, uint32_t                            submitCount, const VkSubmitInfo2*              pSubmits, VkFence           fence) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_QueueSubmit2KHR(VkQueue                           queue, uint32_t                            submitCount, const VkSubmitInfo2*              pSubmits, VkFence           fence) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_QueueSubmit2KHR(VkQueue                           queue, uint32_t                            submitCount, const VkSubmitInfo2*              pSubmits, VkFence           fence) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdWriteTimestamp2(VkCommandBuffer                   commandBuffer, VkPipelineStageFlags2               stage, VkQueryPool                                         queryPool, uint32_t                                            query) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdWriteTimestamp2(VkCommandBuffer                   commandBuffer, VkPipelineStageFlags2               stage, VkQueryPool                                         queryPool, uint32_t                                            query) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdWriteTimestamp2KHR(VkCommandBuffer                   commandBuffer, VkPipelineStageFlags2               stage, VkQueryPool                                         queryPool, uint32_t                                            query) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdWriteTimestamp2KHR(VkCommandBuffer                   commandBuffer, VkPipelineStageFlags2               stage, VkQueryPool                                         queryPool, uint32_t                                            query) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdWriteBufferMarker2AMD(VkCommandBuffer                   commandBuffer, VkPipelineStageFlags2               stage, VkBuffer                                            dstBuffer, VkDeviceSize                                        dstOffset, uint32_t                                            marker) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdWriteBufferMarker2AMD(VkCommandBuffer                   commandBuffer, VkPipelineStageFlags2               stage, VkBuffer                                            dstBuffer, VkDeviceSize                                        dstOffset, uint32_t                                            marker) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetQueueCheckpointData2NV(VkQueue queue, uint32_t* pCheckpointDataCount, VkCheckpointData2NV* pCheckpointData) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetQueueCheckpointData2NV(VkQueue queue, uint32_t* pCheckpointDataCount, VkCheckpointData2NV* pCheckpointData) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CopyMemoryToImageEXT(VkDevice device, const VkCopyMemoryToImageInfoEXT* pCopyMemoryToImageInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CopyMemoryToImageEXT(VkDevice device, const VkCopyMemoryToImageInfoEXT* pCopyMemoryToImageInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CopyImageToMemoryEXT(VkDevice device, const VkCopyImageToMemoryInfoEXT* pCopyImageToMemoryInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CopyImageToMemoryEXT(VkDevice device, const VkCopyImageToMemoryInfoEXT* pCopyImageToMemoryInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CopyImageToImageEXT(VkDevice device, const VkCopyImageToImageInfoEXT* pCopyImageToImageInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CopyImageToImageEXT(VkDevice device, const VkCopyImageToImageInfoEXT* pCopyImageToImageInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_TransitionImageLayoutEXT(VkDevice device, uint32_t transitionCount, const VkHostImageLayoutTransitionInfoEXT* pTransitions) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_TransitionImageLayoutEXT(VkDevice device, uint32_t transitionCount, const VkHostImageLayoutTransitionInfoEXT* pTransitions) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateVideoSessionKHR(VkDevice device, const VkVideoSessionCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkVideoSessionKHR* pVideoSession) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateVideoSessionKHR(VkDevice device, const VkVideoSessionCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkVideoSessionKHR* pVideoSession) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_DestroyVideoSessionKHR(VkDevice device, VkVideoSessionKHR videoSession, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_DestroyVideoSessionKHR(VkDevice device, VkVideoSessionKHR videoSession, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateVideoSessionParametersKHR(VkDevice device, const VkVideoSessionParametersCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkVideoSessionParametersKHR* pVideoSessionParameters) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateVideoSessionParametersKHR(VkDevice device, const VkVideoSessionParametersCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkVideoSessionParametersKHR* pVideoSessionParameters) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_UpdateVideoSessionParametersKHR(VkDevice device, VkVideoSessionParametersKHR videoSessionParameters, const VkVideoSessionParametersUpdateInfoKHR* pUpdateInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_UpdateVideoSessionParametersKHR(VkDevice device, VkVideoSessionParametersKHR videoSessionParameters, const VkVideoSessionParametersUpdateInfoKHR* pUpdateInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_DestroyVideoSessionParametersKHR(VkDevice device, VkVideoSessionParametersKHR videoSessionParameters, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_DestroyVideoSessionParametersKHR(VkDevice device, VkVideoSessionParametersKHR videoSessionParameters, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetVideoSessionMemoryRequirementsKHR(VkDevice device, VkVideoSessionKHR videoSession, uint32_t* pMemoryRequirementsCount, VkVideoSessionMemoryRequirementsKHR* pMemoryRequirements) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetVideoSessionMemoryRequirementsKHR(VkDevice device, VkVideoSessionKHR videoSession, uint32_t* pMemoryRequirementsCount, VkVideoSessionMemoryRequirementsKHR* pMemoryRequirements) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_BindVideoSessionMemoryKHR(VkDevice device, VkVideoSessionKHR videoSession, uint32_t bindSessionMemoryInfoCount, const VkBindVideoSessionMemoryInfoKHR* pBindSessionMemoryInfos) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_BindVideoSessionMemoryKHR(VkDevice device, VkVideoSessionKHR videoSession, uint32_t bindSessionMemoryInfoCount, const VkBindVideoSessionMemoryInfoKHR* pBindSessionMemoryInfos) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdDecodeVideoKHR(VkCommandBuffer commandBuffer, const VkVideoDecodeInfoKHR* pDecodeInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdDecodeVideoKHR(VkCommandBuffer commandBuffer, const VkVideoDecodeInfoKHR* pDecodeInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdBeginVideoCodingKHR(VkCommandBuffer commandBuffer, const VkVideoBeginCodingInfoKHR* pBeginInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdBeginVideoCodingKHR(VkCommandBuffer commandBuffer, const VkVideoBeginCodingInfoKHR* pBeginInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdControlVideoCodingKHR(VkCommandBuffer commandBuffer, const VkVideoCodingControlInfoKHR* pCodingControlInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdControlVideoCodingKHR(VkCommandBuffer commandBuffer, const VkVideoCodingControlInfoKHR* pCodingControlInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdEndVideoCodingKHR(VkCommandBuffer commandBuffer, const VkVideoEndCodingInfoKHR* pEndCodingInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdEndVideoCodingKHR(VkCommandBuffer commandBuffer, const VkVideoEndCodingInfoKHR* pEndCodingInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdDecompressMemoryNV(VkCommandBuffer commandBuffer, uint32_t decompressRegionCount, const VkDecompressMemoryRegionNV* pDecompressMemoryRegions) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdDecompressMemoryNV(VkCommandBuffer commandBuffer, uint32_t decompressRegionCount, const VkDecompressMemoryRegionNV* pDecompressMemoryRegions) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdDecompressMemoryIndirectCountNV(VkCommandBuffer commandBuffer, VkDeviceAddress indirectCommandsAddress, VkDeviceAddress indirectCommandsCountAddress, uint32_t stride) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdDecompressMemoryIndirectCountNV(VkCommandBuffer commandBuffer, VkDeviceAddress indirectCommandsAddress, VkDeviceAddress indirectCommandsCountAddress, uint32_t stride) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateCuModuleNVX(VkDevice device, const VkCuModuleCreateInfoNVX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCuModuleNVX* pModule) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateCuModuleNVX(VkDevice device, const VkCuModuleCreateInfoNVX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCuModuleNVX* pModule) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateCuFunctionNVX(VkDevice device, const VkCuFunctionCreateInfoNVX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCuFunctionNVX* pFunction) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateCuFunctionNVX(VkDevice device, const VkCuFunctionCreateInfoNVX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCuFunctionNVX* pFunction) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_DestroyCuModuleNVX(VkDevice device, VkCuModuleNVX module, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_DestroyCuModuleNVX(VkDevice device, VkCuModuleNVX module, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_DestroyCuFunctionNVX(VkDevice device, VkCuFunctionNVX function, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_DestroyCuFunctionNVX(VkDevice device, VkCuFunctionNVX function, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdCuLaunchKernelNVX(VkCommandBuffer commandBuffer, const VkCuLaunchInfoNVX* pLaunchInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdCuLaunchKernelNVX(VkCommandBuffer commandBuffer, const VkCuLaunchInfoNVX* pLaunchInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetDescriptorSetLayoutSizeEXT(VkDevice device, VkDescriptorSetLayout layout, VkDeviceSize* pLayoutSizeInBytes) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetDescriptorSetLayoutSizeEXT(VkDevice device, VkDescriptorSetLayout layout, VkDeviceSize* pLayoutSizeInBytes) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetDescriptorSetLayoutBindingOffsetEXT(VkDevice device, VkDescriptorSetLayout layout, uint32_t binding, VkDeviceSize* pOffset) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetDescriptorSetLayoutBindingOffsetEXT(VkDevice device, VkDescriptorSetLayout layout, uint32_t binding, VkDeviceSize* pOffset) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetDescriptorEXT(VkDevice device, const VkDescriptorGetInfoEXT* pDescriptorInfo, size_t dataSize, void* pDescriptor) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetDescriptorEXT(VkDevice device, const VkDescriptorGetInfoEXT* pDescriptorInfo, size_t dataSize, void* pDescriptor) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdBindDescriptorBuffersEXT(VkCommandBuffer commandBuffer, uint32_t bufferCount, const VkDescriptorBufferBindingInfoEXT* pBindingInfos) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdBindDescriptorBuffersEXT(VkCommandBuffer commandBuffer, uint32_t bufferCount, const VkDescriptorBufferBindingInfoEXT* pBindingInfos) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetDescriptorBufferOffsetsEXT(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t setCount, const uint32_t* pBufferIndices, const VkDeviceSize* pOffsets) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetDescriptorBufferOffsetsEXT(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t setCount, const uint32_t* pBufferIndices, const VkDeviceSize* pOffsets) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdBindDescriptorBufferEmbeddedSamplersEXT(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdBindDescriptorBufferEmbeddedSamplersEXT(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetBufferOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkBufferCaptureDescriptorDataInfoEXT* pInfo, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetBufferOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkBufferCaptureDescriptorDataInfoEXT* pInfo, void* pData) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetImageOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkImageCaptureDescriptorDataInfoEXT* pInfo, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetImageOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkImageCaptureDescriptorDataInfoEXT* pInfo, void* pData) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetImageViewOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkImageViewCaptureDescriptorDataInfoEXT* pInfo, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetImageViewOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkImageViewCaptureDescriptorDataInfoEXT* pInfo, void* pData) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetSamplerOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkSamplerCaptureDescriptorDataInfoEXT* pInfo, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetSamplerOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkSamplerCaptureDescriptorDataInfoEXT* pInfo, void* pData) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetAccelerationStructureOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkAccelerationStructureCaptureDescriptorDataInfoEXT* pInfo, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetAccelerationStructureOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkAccelerationStructureCaptureDescriptorDataInfoEXT* pInfo, void* pData) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_SetDeviceMemoryPriorityEXT(VkDevice       device, VkDeviceMemory memory, float          priority) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_SetDeviceMemoryPriorityEXT(VkDevice       device, VkDeviceMemory memory, float          priority) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_WaitForPresentKHR(VkDevice device, VkSwapchainKHR swapchain, uint64_t presentId, uint64_t timeout) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_WaitForPresentKHR(VkDevice device, VkSwapchainKHR swapchain, uint64_t presentId, uint64_t timeout) ATTR_WEAK;


#ifdef VK_USE_PLATFORM_FUCHSIA
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateBufferCollectionFUCHSIA(VkDevice device, const VkBufferCollectionCreateInfoFUCHSIA* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferCollectionFUCHSIA* pCollection) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateBufferCollectionFUCHSIA(VkDevice device, const VkBufferCollectionCreateInfoFUCHSIA* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferCollectionFUCHSIA* pCollection) ATTR_WEAK;


#endif // VK_USE_PLATFORM_FUCHSIA
#ifdef VK_USE_PLATFORM_FUCHSIA
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_SetBufferCollectionBufferConstraintsFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, const VkBufferConstraintsInfoFUCHSIA* pBufferConstraintsInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_SetBufferCollectionBufferConstraintsFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, const VkBufferConstraintsInfoFUCHSIA* pBufferConstraintsInfo) ATTR_WEAK;


#endif // VK_USE_PLATFORM_FUCHSIA
#ifdef VK_USE_PLATFORM_FUCHSIA
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_SetBufferCollectionImageConstraintsFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, const VkImageConstraintsInfoFUCHSIA* pImageConstraintsInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_SetBufferCollectionImageConstraintsFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, const VkImageConstraintsInfoFUCHSIA* pImageConstraintsInfo) ATTR_WEAK;


#endif // VK_USE_PLATFORM_FUCHSIA
#ifdef VK_USE_PLATFORM_FUCHSIA
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_DestroyBufferCollectionFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_DestroyBufferCollectionFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


#endif // VK_USE_PLATFORM_FUCHSIA
#ifdef VK_USE_PLATFORM_FUCHSIA
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetBufferCollectionPropertiesFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, VkBufferCollectionPropertiesFUCHSIA* pProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetBufferCollectionPropertiesFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, VkBufferCollectionPropertiesFUCHSIA* pProperties) ATTR_WEAK;


#endif // VK_USE_PLATFORM_FUCHSIA
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdBeginRendering(VkCommandBuffer                   commandBuffer, const VkRenderingInfo*                              pRenderingInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdBeginRendering(VkCommandBuffer                   commandBuffer, const VkRenderingInfo*                              pRenderingInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdBeginRenderingKHR(VkCommandBuffer                   commandBuffer, const VkRenderingInfo*                              pRenderingInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdBeginRenderingKHR(VkCommandBuffer                   commandBuffer, const VkRenderingInfo*                              pRenderingInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdEndRendering(VkCommandBuffer                   commandBuffer) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdEndRendering(VkCommandBuffer                   commandBuffer) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdEndRenderingKHR(VkCommandBuffer                   commandBuffer) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdEndRenderingKHR(VkCommandBuffer                   commandBuffer) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetDescriptorSetLayoutHostMappingInfoVALVE(VkDevice device, const VkDescriptorSetBindingReferenceVALVE* pBindingReference, VkDescriptorSetLayoutHostMappingInfoVALVE* pHostMapping) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetDescriptorSetLayoutHostMappingInfoVALVE(VkDevice device, const VkDescriptorSetBindingReferenceVALVE* pBindingReference, VkDescriptorSetLayoutHostMappingInfoVALVE* pHostMapping) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetDescriptorSetHostMappingVALVE(VkDevice device, VkDescriptorSet descriptorSet, void** ppData) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetDescriptorSetHostMappingVALVE(VkDevice device, VkDescriptorSet descriptorSet, void** ppData) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateMicromapEXT(VkDevice                                           device, const VkMicromapCreateInfoEXT*        pCreateInfo, const VkAllocationCallbacks*       pAllocator, VkMicromapEXT*                        pMicromap) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateMicromapEXT(VkDevice                                           device, const VkMicromapCreateInfoEXT*        pCreateInfo, const VkAllocationCallbacks*       pAllocator, VkMicromapEXT*                        pMicromap) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdBuildMicromapsEXT(VkCommandBuffer                                    commandBuffer, uint32_t infoCount, const VkMicromapBuildInfoEXT* pInfos) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdBuildMicromapsEXT(VkCommandBuffer                                    commandBuffer, uint32_t infoCount, const VkMicromapBuildInfoEXT* pInfos) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_BuildMicromapsEXT(VkDevice                                           device, VkDeferredOperationKHR deferredOperation, uint32_t infoCount, const VkMicromapBuildInfoEXT* pInfos) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_BuildMicromapsEXT(VkDevice                                           device, VkDeferredOperationKHR deferredOperation, uint32_t infoCount, const VkMicromapBuildInfoEXT* pInfos) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_DestroyMicromapEXT(VkDevice device, VkMicromapEXT micromap, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_DestroyMicromapEXT(VkDevice device, VkMicromapEXT micromap, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdCopyMicromapEXT(VkCommandBuffer commandBuffer, const VkCopyMicromapInfoEXT* pInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdCopyMicromapEXT(VkCommandBuffer commandBuffer, const VkCopyMicromapInfoEXT* pInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CopyMicromapEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMicromapInfoEXT* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CopyMicromapEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMicromapInfoEXT* pInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdCopyMicromapToMemoryEXT(VkCommandBuffer commandBuffer, const VkCopyMicromapToMemoryInfoEXT* pInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdCopyMicromapToMemoryEXT(VkCommandBuffer commandBuffer, const VkCopyMicromapToMemoryInfoEXT* pInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CopyMicromapToMemoryEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMicromapToMemoryInfoEXT* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CopyMicromapToMemoryEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMicromapToMemoryInfoEXT* pInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdCopyMemoryToMicromapEXT(VkCommandBuffer commandBuffer, const VkCopyMemoryToMicromapInfoEXT* pInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdCopyMemoryToMicromapEXT(VkCommandBuffer commandBuffer, const VkCopyMemoryToMicromapInfoEXT* pInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CopyMemoryToMicromapEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMemoryToMicromapInfoEXT* pInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CopyMemoryToMicromapEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMemoryToMicromapInfoEXT* pInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdWriteMicromapsPropertiesEXT(VkCommandBuffer commandBuffer, uint32_t micromapCount, const VkMicromapEXT* pMicromaps, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdWriteMicromapsPropertiesEXT(VkCommandBuffer commandBuffer, uint32_t micromapCount, const VkMicromapEXT* pMicromaps, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_WriteMicromapsPropertiesEXT(VkDevice device, uint32_t micromapCount, const VkMicromapEXT* pMicromaps, VkQueryType  queryType, size_t       dataSize, void* pData, size_t stride) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_WriteMicromapsPropertiesEXT(VkDevice device, uint32_t micromapCount, const VkMicromapEXT* pMicromaps, VkQueryType  queryType, size_t       dataSize, void* pData, size_t stride) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetDeviceMicromapCompatibilityEXT(VkDevice device, const VkMicromapVersionInfoEXT* pVersionInfo, VkAccelerationStructureCompatibilityKHR* pCompatibility) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetDeviceMicromapCompatibilityEXT(VkDevice device, const VkMicromapVersionInfoEXT* pVersionInfo, VkAccelerationStructureCompatibilityKHR* pCompatibility) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetMicromapBuildSizesEXT(VkDevice                                            device, VkAccelerationStructureBuildTypeKHR                 buildType, const VkMicromapBuildInfoEXT*  pBuildInfo, VkMicromapBuildSizesInfoEXT*           pSizeInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetMicromapBuildSizesEXT(VkDevice                                            device, VkAccelerationStructureBuildTypeKHR                 buildType, const VkMicromapBuildInfoEXT*  pBuildInfo, VkMicromapBuildSizesInfoEXT*           pSizeInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetShaderModuleIdentifierEXT(VkDevice device, VkShaderModule shaderModule, VkShaderModuleIdentifierEXT* pIdentifier) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetShaderModuleIdentifierEXT(VkDevice device, VkShaderModule shaderModule, VkShaderModuleIdentifierEXT* pIdentifier) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetShaderModuleCreateInfoIdentifierEXT(VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, VkShaderModuleIdentifierEXT* pIdentifier) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetShaderModuleCreateInfoIdentifierEXT(VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, VkShaderModuleIdentifierEXT* pIdentifier) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetImageSubresourceLayout2KHR(VkDevice device, VkImage image, const VkImageSubresource2KHR* pSubresource, VkSubresourceLayout2KHR* pLayout) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetImageSubresourceLayout2KHR(VkDevice device, VkImage image, const VkImageSubresource2KHR* pSubresource, VkSubresourceLayout2KHR* pLayout) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetImageSubresourceLayout2EXT(VkDevice device, VkImage image, const VkImageSubresource2KHR* pSubresource, VkSubresourceLayout2KHR* pLayout) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetImageSubresourceLayout2EXT(VkDevice device, VkImage image, const VkImageSubresource2KHR* pSubresource, VkSubresourceLayout2KHR* pLayout) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetPipelinePropertiesEXT(VkDevice device, const VkPipelineInfoEXT* pPipelineInfo, VkBaseOutStructure* pPipelineProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetPipelinePropertiesEXT(VkDevice device, const VkPipelineInfoEXT* pPipelineInfo, VkBaseOutStructure* pPipelineProperties) ATTR_WEAK;


#ifdef VK_USE_PLATFORM_METAL_EXT
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_ExportMetalObjectsEXT(VkDevice device, VkExportMetalObjectsInfoEXT* pMetalObjectsInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_ExportMetalObjectsEXT(VkDevice device, VkExportMetalObjectsInfoEXT* pMetalObjectsInfo) ATTR_WEAK;


#endif // VK_USE_PLATFORM_METAL_EXT
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetFramebufferTilePropertiesQCOM(VkDevice device, VkFramebuffer framebuffer, uint32_t* pPropertiesCount, VkTilePropertiesQCOM* pProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetFramebufferTilePropertiesQCOM(VkDevice device, VkFramebuffer framebuffer, uint32_t* pPropertiesCount, VkTilePropertiesQCOM* pProperties) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetDynamicRenderingTilePropertiesQCOM(VkDevice device, const VkRenderingInfo* pRenderingInfo, VkTilePropertiesQCOM* pProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetDynamicRenderingTilePropertiesQCOM(VkDevice device, const VkRenderingInfo* pRenderingInfo, VkTilePropertiesQCOM* pProperties) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateOpticalFlowSessionNV(VkDevice device, const VkOpticalFlowSessionCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkOpticalFlowSessionNV* pSession) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateOpticalFlowSessionNV(VkDevice device, const VkOpticalFlowSessionCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkOpticalFlowSessionNV* pSession) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_DestroyOpticalFlowSessionNV(VkDevice device, VkOpticalFlowSessionNV session, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_DestroyOpticalFlowSessionNV(VkDevice device, VkOpticalFlowSessionNV session, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_BindOpticalFlowSessionImageNV(VkDevice device, VkOpticalFlowSessionNV session, VkOpticalFlowSessionBindingPointNV bindingPoint, VkImageView view, VkImageLayout layout) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_BindOpticalFlowSessionImageNV(VkDevice device, VkOpticalFlowSessionNV session, VkOpticalFlowSessionBindingPointNV bindingPoint, VkImageView view, VkImageLayout layout) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdOpticalFlowExecuteNV(VkCommandBuffer commandBuffer, VkOpticalFlowSessionNV session, const VkOpticalFlowExecuteInfoNV* pExecuteInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdOpticalFlowExecuteNV(VkCommandBuffer commandBuffer, VkOpticalFlowSessionNV session, const VkOpticalFlowExecuteInfoNV* pExecuteInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetDeviceFaultInfoEXT(VkDevice device, VkDeviceFaultCountsEXT* pFaultCounts, VkDeviceFaultInfoEXT* pFaultInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetDeviceFaultInfoEXT(VkDevice device, VkDeviceFaultCountsEXT* pFaultCounts, VkDeviceFaultInfoEXT* pFaultInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdSetDepthBias2EXT(VkCommandBuffer commandBuffer, const VkDepthBiasInfoEXT*         pDepthBiasInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdSetDepthBias2EXT(VkCommandBuffer commandBuffer, const VkDepthBiasInfoEXT*         pDepthBiasInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_ReleaseSwapchainImagesEXT(VkDevice device, const VkReleaseSwapchainImagesInfoEXT* pReleaseInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_ReleaseSwapchainImagesEXT(VkDevice device, const VkReleaseSwapchainImagesInfoEXT* pReleaseInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetDeviceImageSubresourceLayoutKHR(VkDevice device, const VkDeviceImageSubresourceInfoKHR* pInfo, VkSubresourceLayout2KHR* pLayout) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetDeviceImageSubresourceLayoutKHR(VkDevice device, const VkDeviceImageSubresourceInfoKHR* pInfo, VkSubresourceLayout2KHR* pLayout) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_MapMemory2KHR(VkDevice device, const VkMemoryMapInfoKHR* pMemoryMapInfo, void** ppData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_MapMemory2KHR(VkDevice device, const VkMemoryMapInfoKHR* pMemoryMapInfo, void** ppData) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_UnmapMemory2KHR(VkDevice device, const VkMemoryUnmapInfoKHR* pMemoryUnmapInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_UnmapMemory2KHR(VkDevice device, const VkMemoryUnmapInfoKHR* pMemoryUnmapInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_CreateShadersEXT(VkDevice device, uint32_t createInfoCount, const VkShaderCreateInfoEXT* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkShaderEXT* pShaders) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_CreateShadersEXT(VkDevice device, uint32_t createInfoCount, const VkShaderCreateInfoEXT* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkShaderEXT* pShaders) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_DestroyShaderEXT(VkDevice device, VkShaderEXT shader, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_DestroyShaderEXT(VkDevice device, VkShaderEXT shader, const VkAllocationCallbacks* pAllocator) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetShaderBinaryDataEXT(VkDevice device, VkShaderEXT shader, size_t* pDataSize, void* pData) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetShaderBinaryDataEXT(VkDevice device, VkShaderEXT shader, size_t* pDataSize, void* pData) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_CmdBindShadersEXT(VkCommandBuffer commandBuffer, uint32_t stageCount, const VkShaderStageFlagBits* pStages, const VkShaderEXT* pShaders) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_CmdBindShadersEXT(VkCommandBuffer commandBuffer, uint32_t stageCount, const VkShaderStageFlagBits* pStages, const VkShaderEXT* pShaders) ATTR_WEAK;


#ifdef VK_USE_PLATFORM_SCREEN_QNX
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_GetScreenBufferPropertiesQNX(VkDevice device, const struct _screen_buffer* buffer, VkScreenBufferPropertiesQNX* pProperties) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_GetScreenBufferPropertiesQNX(VkDevice device, const struct _screen_buffer* buffer, VkScreenBufferPropertiesQNX* pProperties) ATTR_WEAK;


#endif // VK_USE_PLATFORM_SCREEN_QNX
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_SetLatencySleepModeNV(VkDevice device, VkSwapchainKHR swapchain, const VkLatencySleepModeInfoNV* pSleepModeInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_SetLatencySleepModeNV(VkDevice device, VkSwapchainKHR swapchain, const VkLatencySleepModeInfoNV* pSleepModeInfo) ATTR_WEAK;


  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_LatencySleepNV(VkDevice device, VkSwapchainKHR swapchain, const VkLatencySleepInfoNV* pSleepInfo) ATTR_WEAK;
  VKAPI_ATTR VkResult VKAPI_CALL vk_cmd_enqueue_unless_primary_LatencySleepNV(VkDevice device, VkSwapchainKHR swapchain, const VkLatencySleepInfoNV* pSleepInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_SetLatencyMarkerNV(VkDevice device, VkSwapchainKHR swapchain, const VkSetLatencyMarkerInfoNV* pLatencyMarkerInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_SetLatencyMarkerNV(VkDevice device, VkSwapchainKHR swapchain, const VkSetLatencyMarkerInfoNV* pLatencyMarkerInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_GetLatencyTimingsNV(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pTimingCount, VkGetLatencyMarkerInfoNV* pLatencyMarkerInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_GetLatencyTimingsNV(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pTimingCount, VkGetLatencyMarkerInfoNV* pLatencyMarkerInfo) ATTR_WEAK;


  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_QueueNotifyOutOfBandNV(VkQueue queue, const VkOutOfBandQueueTypeInfoNV* pQueueTypeInfo) ATTR_WEAK;
  VKAPI_ATTR void VKAPI_CALL vk_cmd_enqueue_unless_primary_QueueNotifyOutOfBandNV(VkQueue queue, const VkOutOfBandQueueTypeInfoNV* pQueueTypeInfo) ATTR_WEAK;



#ifdef __cplusplus
}
#endif

#endif /* VK_CMD_ENQUEUE_ENTRYPOINTS_H */
