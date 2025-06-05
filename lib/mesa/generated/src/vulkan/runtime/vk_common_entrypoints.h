
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


#ifndef VK_COMMON_ENTRYPOINTS_H
#define VK_COMMON_ENTRYPOINTS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Entrypoint symbols are optional, and resolves to NULL if undefined.
 * On Unix, this semantics is achieved through weak symbols.
 * Note that we only declare the symbols as weak when it needs to be optional;
 * otherwise, the symbol is declared as a regular symbol.
 * This is to workaround a MinGW limitation: on MinGW, the definition for a
 * weak symbol must be regular, or the linker will end up resolving to one of
 * the fallback symbols with the absolute value of 0.
 * On MSVC, weak symbols are not well supported, so we use the functionally
 * equivalent /alternatename.
 */
#if !defined(_MSC_VER) && defined(VK_ENTRY_USE_WEAK)
#define VK_ENTRY_WEAK __attribute__ ((weak))
#else
#define VK_ENTRY_WEAK
#endif

/* On Unix, we explicitly declare the symbols as hidden, as -fvisibility=hidden
 * only applies to definitions, not declarations.
 * Windows uses hidden visibility by default (requiring dllexport for public
 * symbols), so we don't need to deal with visibility there.
 */
#ifndef _WIN32
#define VK_ENTRY_HIDDEN __attribute__ ((visibility("hidden")))
#else
#define VK_ENTRY_HIDDEN
#endif

extern const struct vk_instance_entrypoint_table vk_common_instance_entrypoints;

extern const struct vk_physical_device_entrypoint_table vk_common_physical_device_entrypoints;

extern const struct vk_device_entrypoint_table vk_common_device_entrypoints;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateInstance(const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR void VKAPI_CALL vk_common_DestroyInstance(VkInstance instance, const VkAllocationCallbacks* pAllocator) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_EnumeratePhysicalDevices(VkInstance instance, uint32_t* pPhysicalDeviceCount, VkPhysicalDevice* pPhysicalDevices) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vk_common_GetInstanceProcAddr(VkInstance instance, const char* pName) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_EnumerateInstanceVersion(uint32_t* pApiVersion) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_EnumerateInstanceLayerProperties(uint32_t* pPropertyCount, VkLayerProperties* pProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_EnumerateInstanceExtensionProperties(const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
#ifdef VK_USE_PLATFORM_ANDROID_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateAndroidSurfaceKHR(VkInstance instance, const VkAndroidSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
#endif // VK_USE_PLATFORM_ANDROID_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateDisplayPlaneSurfaceKHR(VkInstance instance, const VkDisplaySurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR void VKAPI_CALL vk_common_DestroySurfaceKHR(VkInstance instance, VkSurfaceKHR surface, const VkAllocationCallbacks* pAllocator) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
#ifdef VK_USE_PLATFORM_VI_NN
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateViSurfaceNN(VkInstance instance, const VkViSurfaceCreateInfoNN* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
#endif // VK_USE_PLATFORM_VI_NN
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateWaylandSurfaceKHR(VkInstance instance, const VkWaylandSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
#endif // VK_USE_PLATFORM_WAYLAND_KHR
#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateWin32SurfaceKHR(VkInstance instance, const VkWin32SurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
#endif // VK_USE_PLATFORM_WIN32_KHR
#ifdef VK_USE_PLATFORM_XLIB_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateXlibSurfaceKHR(VkInstance instance, const VkXlibSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
#endif // VK_USE_PLATFORM_XLIB_KHR
#ifdef VK_USE_PLATFORM_XCB_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateXcbSurfaceKHR(VkInstance instance, const VkXcbSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
#endif // VK_USE_PLATFORM_XCB_KHR
#ifdef VK_USE_PLATFORM_DIRECTFB_EXT
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateDirectFBSurfaceEXT(VkInstance instance, const VkDirectFBSurfaceCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
#endif // VK_USE_PLATFORM_DIRECTFB_EXT
#ifdef VK_USE_PLATFORM_FUCHSIA
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateImagePipeSurfaceFUCHSIA(VkInstance instance, const VkImagePipeSurfaceCreateInfoFUCHSIA* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
#endif // VK_USE_PLATFORM_FUCHSIA
#ifdef VK_USE_PLATFORM_GGP
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateStreamDescriptorSurfaceGGP(VkInstance instance, const VkStreamDescriptorSurfaceCreateInfoGGP* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
#endif // VK_USE_PLATFORM_GGP
#ifdef VK_USE_PLATFORM_SCREEN_QNX
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateScreenSurfaceQNX(VkInstance instance, const VkScreenSurfaceCreateInfoQNX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
#endif // VK_USE_PLATFORM_SCREEN_QNX
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR void VKAPI_CALL vk_common_DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR void VKAPI_CALL vk_common_DebugReportMessageEXT(VkInstance instance, VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_EnumeratePhysicalDeviceGroups(VkInstance instance, uint32_t* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_EnumeratePhysicalDeviceGroupsKHR(VkInstance instance, uint32_t* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
#ifdef VK_USE_PLATFORM_IOS_MVK
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateIOSSurfaceMVK(VkInstance instance, const VkIOSSurfaceCreateInfoMVK* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
#endif // VK_USE_PLATFORM_IOS_MVK
#ifdef VK_USE_PLATFORM_MACOS_MVK
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateMacOSSurfaceMVK(VkInstance instance, const VkMacOSSurfaceCreateInfoMVK* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
#endif // VK_USE_PLATFORM_MACOS_MVK
#ifdef VK_USE_PLATFORM_METAL_EXT
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateMetalSurfaceEXT(VkInstance instance, const VkMetalSurfaceCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
#endif // VK_USE_PLATFORM_METAL_EXT
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pMessenger) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR void VKAPI_CALL vk_common_DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT messenger, const VkAllocationCallbacks* pAllocator) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR void VKAPI_CALL vk_common_SubmitDebugUtilsMessageEXT(VkInstance instance, VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateHeadlessSurfaceEXT(VkInstance instance, const VkHeadlessSurfaceCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;

  VKAPI_ATTR void VKAPI_CALL vk_common_GetPhysicalDeviceProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties* pProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR void VKAPI_CALL vk_common_GetPhysicalDeviceQueueFamilyProperties(VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties* pQueueFamilyProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR void VKAPI_CALL vk_common_GetPhysicalDeviceMemoryProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties* pMemoryProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR void VKAPI_CALL vk_common_GetPhysicalDeviceFeatures(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures* pFeatures) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR void VKAPI_CALL vk_common_GetPhysicalDeviceFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties* pFormatProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetPhysicalDeviceImageFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkImageFormatProperties* pImageFormatProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateDevice(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDevice* pDevice) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_EnumerateDeviceLayerProperties(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkLayerProperties* pProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_EnumerateDeviceExtensionProperties(VkPhysicalDevice physicalDevice, const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR void VKAPI_CALL vk_common_GetPhysicalDeviceSparseImageFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkSampleCountFlagBits samples, VkImageUsageFlags usage, VkImageTiling tiling, uint32_t* pPropertyCount, VkSparseImageFormatProperties* pProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetPhysicalDeviceDisplayPropertiesKHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPropertiesKHR* pProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetPhysicalDeviceDisplayPlanePropertiesKHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPlanePropertiesKHR* pProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetDisplayPlaneSupportedDisplaysKHR(VkPhysicalDevice physicalDevice, uint32_t planeIndex, uint32_t* pDisplayCount, VkDisplayKHR* pDisplays) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetDisplayModePropertiesKHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModePropertiesKHR* pProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateDisplayModeKHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, const VkDisplayModeCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDisplayModeKHR* pMode) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetDisplayPlaneCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkDisplayModeKHR mode, uint32_t planeIndex, VkDisplayPlaneCapabilitiesKHR* pCapabilities) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetPhysicalDeviceSurfaceSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetPhysicalDeviceSurfaceCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR* pSurfaceCapabilities) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetPhysicalDeviceSurfaceFormatsKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pSurfaceFormatCount, VkSurfaceFormatKHR* pSurfaceFormats) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetPhysicalDeviceSurfacePresentModesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
  VKAPI_ATTR VkBool32 VKAPI_CALL vk_common_GetPhysicalDeviceWaylandPresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, struct wl_display* display) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
#endif // VK_USE_PLATFORM_WAYLAND_KHR
#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkBool32 VKAPI_CALL vk_common_GetPhysicalDeviceWin32PresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
#endif // VK_USE_PLATFORM_WIN32_KHR
#ifdef VK_USE_PLATFORM_XLIB_KHR
  VKAPI_ATTR VkBool32 VKAPI_CALL vk_common_GetPhysicalDeviceXlibPresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, Display* dpy, VisualID visualID) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
#endif // VK_USE_PLATFORM_XLIB_KHR
#ifdef VK_USE_PLATFORM_XCB_KHR
  VKAPI_ATTR VkBool32 VKAPI_CALL vk_common_GetPhysicalDeviceXcbPresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, xcb_connection_t* connection, xcb_visualid_t visual_id) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
#endif // VK_USE_PLATFORM_XCB_KHR
#ifdef VK_USE_PLATFORM_DIRECTFB_EXT
  VKAPI_ATTR VkBool32 VKAPI_CALL vk_common_GetPhysicalDeviceDirectFBPresentationSupportEXT(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, IDirectFB* dfb) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
#endif // VK_USE_PLATFORM_DIRECTFB_EXT
#ifdef VK_USE_PLATFORM_SCREEN_QNX
  VKAPI_ATTR VkBool32 VKAPI_CALL vk_common_GetPhysicalDeviceScreenPresentationSupportQNX(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, struct _screen_window* window) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
#endif // VK_USE_PLATFORM_SCREEN_QNX
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetPhysicalDeviceExternalImageFormatPropertiesNV(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkExternalMemoryHandleTypeFlagsNV externalHandleType, VkExternalImageFormatPropertiesNV* pExternalImageFormatProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR void VKAPI_CALL vk_common_GetPhysicalDeviceFeatures2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR void VKAPI_CALL vk_common_GetPhysicalDeviceFeatures2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR void VKAPI_CALL vk_common_GetPhysicalDeviceProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR void VKAPI_CALL vk_common_GetPhysicalDeviceProperties2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR void VKAPI_CALL vk_common_GetPhysicalDeviceFormatProperties2(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2* pFormatProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR void VKAPI_CALL vk_common_GetPhysicalDeviceFormatProperties2KHR(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2* pFormatProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetPhysicalDeviceImageFormatProperties2(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetPhysicalDeviceImageFormatProperties2KHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR void VKAPI_CALL vk_common_GetPhysicalDeviceQueueFamilyProperties2(VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties2* pQueueFamilyProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR void VKAPI_CALL vk_common_GetPhysicalDeviceQueueFamilyProperties2KHR(VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties2* pQueueFamilyProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR void VKAPI_CALL vk_common_GetPhysicalDeviceMemoryProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR void VKAPI_CALL vk_common_GetPhysicalDeviceMemoryProperties2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR void VKAPI_CALL vk_common_GetPhysicalDeviceSparseImageFormatProperties2(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo, uint32_t* pPropertyCount, VkSparseImageFormatProperties2* pProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR void VKAPI_CALL vk_common_GetPhysicalDeviceSparseImageFormatProperties2KHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo, uint32_t* pPropertyCount, VkSparseImageFormatProperties2* pProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR void VKAPI_CALL vk_common_GetPhysicalDeviceExternalBufferProperties(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR void VKAPI_CALL vk_common_GetPhysicalDeviceExternalBufferPropertiesKHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR void VKAPI_CALL vk_common_GetPhysicalDeviceExternalSemaphoreProperties(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR void VKAPI_CALL vk_common_GetPhysicalDeviceExternalSemaphorePropertiesKHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR void VKAPI_CALL vk_common_GetPhysicalDeviceExternalFenceProperties(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR void VKAPI_CALL vk_common_GetPhysicalDeviceExternalFencePropertiesKHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_ReleaseDisplayEXT(VkPhysicalDevice physicalDevice, VkDisplayKHR display) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
#ifdef VK_USE_PLATFORM_XLIB_XRANDR_EXT
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_AcquireXlibDisplayEXT(VkPhysicalDevice physicalDevice, Display* dpy, VkDisplayKHR display) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
#endif // VK_USE_PLATFORM_XLIB_XRANDR_EXT
#ifdef VK_USE_PLATFORM_XLIB_XRANDR_EXT
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetRandROutputDisplayEXT(VkPhysicalDevice physicalDevice, Display* dpy, RROutput rrOutput, VkDisplayKHR* pDisplay) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
#endif // VK_USE_PLATFORM_XLIB_XRANDR_EXT
#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_AcquireWinrtDisplayNV(VkPhysicalDevice physicalDevice, VkDisplayKHR display) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
#endif // VK_USE_PLATFORM_WIN32_KHR
#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetWinrtDisplayNV(VkPhysicalDevice physicalDevice, uint32_t deviceRelativeId, VkDisplayKHR* pDisplay) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
#endif // VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetPhysicalDeviceSurfaceCapabilities2EXT(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilities2EXT* pSurfaceCapabilities) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetPhysicalDevicePresentRectanglesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pRectCount, VkRect2D* pRects) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR void VKAPI_CALL vk_common_GetPhysicalDeviceMultisamplePropertiesEXT(VkPhysicalDevice physicalDevice, VkSampleCountFlagBits samples, VkMultisamplePropertiesEXT* pMultisampleProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetPhysicalDeviceSurfaceCapabilities2KHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkSurfaceCapabilities2KHR* pSurfaceCapabilities) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetPhysicalDeviceSurfaceFormats2KHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pSurfaceFormatCount, VkSurfaceFormat2KHR* pSurfaceFormats) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetPhysicalDeviceDisplayProperties2KHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayProperties2KHR* pProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetPhysicalDeviceDisplayPlaneProperties2KHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPlaneProperties2KHR* pProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetDisplayModeProperties2KHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModeProperties2KHR* pProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetDisplayPlaneCapabilities2KHR(VkPhysicalDevice physicalDevice, const VkDisplayPlaneInfo2KHR* pDisplayPlaneInfo, VkDisplayPlaneCapabilities2KHR* pCapabilities) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetPhysicalDeviceCalibrateableTimeDomainsKHR(VkPhysicalDevice physicalDevice, uint32_t* pTimeDomainCount, VkTimeDomainKHR* pTimeDomains) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetPhysicalDeviceCalibrateableTimeDomainsEXT(VkPhysicalDevice physicalDevice, uint32_t* pTimeDomainCount, VkTimeDomainKHR* pTimeDomains) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetPhysicalDeviceCooperativeMatrixPropertiesNV(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkCooperativeMatrixPropertiesNV* pProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetPhysicalDeviceSurfacePresentModes2EXT(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
#endif // VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_EnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, uint32_t* pCounterCount, VkPerformanceCounterKHR* pCounters, VkPerformanceCounterDescriptionKHR* pCounterDescriptions) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR void VKAPI_CALL vk_common_GetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR(VkPhysicalDevice physicalDevice, const VkQueryPoolPerformanceCreateInfoKHR* pPerformanceQueryCreateInfo, uint32_t* pNumPasses) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV(VkPhysicalDevice physicalDevice, uint32_t* pCombinationCount, VkFramebufferMixedSamplesCombinationNV* pCombinations) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetPhysicalDeviceToolProperties(VkPhysicalDevice physicalDevice, uint32_t* pToolCount, VkPhysicalDeviceToolProperties* pToolProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetPhysicalDeviceToolPropertiesEXT(VkPhysicalDevice physicalDevice, uint32_t* pToolCount, VkPhysicalDeviceToolProperties* pToolProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetPhysicalDeviceFragmentShadingRatesKHR(VkPhysicalDevice physicalDevice, uint32_t* pFragmentShadingRateCount, VkPhysicalDeviceFragmentShadingRateKHR* pFragmentShadingRates) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetPhysicalDeviceVideoCapabilitiesKHR(VkPhysicalDevice physicalDevice, const VkVideoProfileInfoKHR* pVideoProfile, VkVideoCapabilitiesKHR* pCapabilities) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetPhysicalDeviceVideoFormatPropertiesKHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceVideoFormatInfoKHR* pVideoFormatInfo, uint32_t* pVideoFormatPropertyCount, VkVideoFormatPropertiesKHR* pVideoFormatProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceVideoEncodeQualityLevelInfoKHR* pQualityLevelInfo, VkVideoEncodeQualityLevelPropertiesKHR* pQualityLevelProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_AcquireDrmDisplayEXT(VkPhysicalDevice physicalDevice, int32_t drmFd, VkDisplayKHR display) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetDrmDisplayEXT(VkPhysicalDevice physicalDevice, int32_t drmFd, uint32_t connectorId, VkDisplayKHR* display) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetPhysicalDeviceOpticalFlowImageFormatsNV(VkPhysicalDevice physicalDevice, const VkOpticalFlowImageFormatInfoNV* pOpticalFlowImageFormatInfo, uint32_t* pFormatCount, VkOpticalFlowImageFormatPropertiesNV* pImageFormatProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetPhysicalDeviceCooperativeMatrixPropertiesKHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkCooperativeMatrixPropertiesKHR* pProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetPhysicalDeviceCooperativeMatrixFlexibleDimensionsPropertiesNV(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkCooperativeMatrixFlexibleDimensionsPropertiesNV* pProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;

  VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vk_common_GetDeviceProcAddr(VkDevice device, const char* pName) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_DestroyDevice(VkDevice device, const VkAllocationCallbacks* pAllocator) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_GetDeviceQueue(VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_QueueSubmit(VkQueue queue, uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_QueueWaitIdle(VkQueue queue) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_DeviceWaitIdle(VkDevice device) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_AllocateMemory(VkDevice device, const VkMemoryAllocateInfo* pAllocateInfo, const VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_FreeMemory(VkDevice device, VkDeviceMemory memory, const VkAllocationCallbacks* pAllocator) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_MapMemory(VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_UnmapMemory(VkDevice device, VkDeviceMemory memory) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_FlushMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_InvalidateMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_GetDeviceMemoryCommitment(VkDevice device, VkDeviceMemory memory, VkDeviceSize* pCommittedMemoryInBytes) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_GetBufferMemoryRequirements(VkDevice device, VkBuffer buffer, VkMemoryRequirements* pMemoryRequirements) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_BindBufferMemory(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_GetImageMemoryRequirements(VkDevice device, VkImage image, VkMemoryRequirements* pMemoryRequirements) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_BindImageMemory(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_GetImageSparseMemoryRequirements(VkDevice device, VkImage image, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements* pSparseMemoryRequirements) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_QueueBindSparse(VkQueue queue, uint32_t bindInfoCount, const VkBindSparseInfo* pBindInfo, VkFence fence) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateFence(VkDevice device, const VkFenceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_DestroyFence(VkDevice device, VkFence fence, const VkAllocationCallbacks* pAllocator) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_ResetFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetFenceStatus(VkDevice device, VkFence fence) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_WaitForFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences, VkBool32 waitAll, uint64_t timeout) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateSemaphore(VkDevice device, const VkSemaphoreCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_DestroySemaphore(VkDevice device, VkSemaphore semaphore, const VkAllocationCallbacks* pAllocator) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateEvent(VkDevice device, const VkEventCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkEvent* pEvent) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_DestroyEvent(VkDevice device, VkEvent event, const VkAllocationCallbacks* pAllocator) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetEventStatus(VkDevice device, VkEvent event) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_SetEvent(VkDevice device, VkEvent event) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_ResetEvent(VkDevice device, VkEvent event) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateQueryPool(VkDevice device, const VkQueryPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkQueryPool* pQueryPool) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_DestroyQueryPool(VkDevice device, VkQueryPool queryPool, const VkAllocationCallbacks* pAllocator) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetQueryPoolResults(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, VkDeviceSize stride, VkQueryResultFlags flags) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_ResetQueryPool(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_ResetQueryPoolEXT(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateBuffer(VkDevice device, const VkBufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_DestroyBuffer(VkDevice device, VkBuffer buffer, const VkAllocationCallbacks* pAllocator) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateBufferView(VkDevice device, const VkBufferViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferView* pView) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_DestroyBufferView(VkDevice device, VkBufferView bufferView, const VkAllocationCallbacks* pAllocator) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateImage(VkDevice device, const VkImageCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImage* pImage) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_DestroyImage(VkDevice device, VkImage image, const VkAllocationCallbacks* pAllocator) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_GetImageSubresourceLayout(VkDevice device, VkImage image, const VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateImageView(VkDevice device, const VkImageViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImageView* pView) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_DestroyImageView(VkDevice device, VkImageView imageView, const VkAllocationCallbacks* pAllocator) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateShaderModule(VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_DestroyShaderModule(VkDevice device, VkShaderModule shaderModule, const VkAllocationCallbacks* pAllocator) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreatePipelineCache(VkDevice device, const VkPipelineCacheCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineCache* pPipelineCache) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_DestroyPipelineCache(VkDevice device, VkPipelineCache pipelineCache, const VkAllocationCallbacks* pAllocator) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetPipelineCacheData(VkDevice device, VkPipelineCache pipelineCache, size_t* pDataSize, void* pData) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_MergePipelineCaches(VkDevice device, VkPipelineCache dstCache, uint32_t srcCacheCount, const VkPipelineCache* pSrcCaches) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreatePipelineBinariesKHR(VkDevice device, const VkPipelineBinaryCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineBinaryHandlesInfoKHR* pBinaries) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_DestroyPipelineBinaryKHR(VkDevice device, VkPipelineBinaryKHR pipelineBinary, const VkAllocationCallbacks* pAllocator) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetPipelineKeyKHR(VkDevice device, const VkPipelineCreateInfoKHR* pPipelineCreateInfo, VkPipelineBinaryKeyKHR* pPipelineKey) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetPipelineBinaryDataKHR(VkDevice device, const VkPipelineBinaryDataInfoKHR* pInfo, VkPipelineBinaryKeyKHR* pPipelineBinaryKey, size_t* pPipelineBinaryDataSize, void* pPipelineBinaryData) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_ReleaseCapturedPipelineDataKHR(VkDevice device, const VkReleaseCapturedPipelineDataInfoKHR* pInfo, const VkAllocationCallbacks* pAllocator) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateGraphicsPipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkGraphicsPipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateComputePipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkComputePipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI(VkDevice device, VkRenderPass renderpass, VkExtent2D* pMaxWorkgroupSize) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_DestroyPipeline(VkDevice device, VkPipeline pipeline, const VkAllocationCallbacks* pAllocator) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreatePipelineLayout(VkDevice device, const VkPipelineLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_DestroyPipelineLayout(VkDevice device, VkPipelineLayout pipelineLayout, const VkAllocationCallbacks* pAllocator) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateSampler(VkDevice device, const VkSamplerCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSampler* pSampler) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_DestroySampler(VkDevice device, VkSampler sampler, const VkAllocationCallbacks* pAllocator) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateDescriptorSetLayout(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorSetLayout* pSetLayout) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_DestroyDescriptorSetLayout(VkDevice device, VkDescriptorSetLayout descriptorSetLayout, const VkAllocationCallbacks* pAllocator) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateDescriptorPool(VkDevice device, const VkDescriptorPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorPool* pDescriptorPool) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_DestroyDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, const VkAllocationCallbacks* pAllocator) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_ResetDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags flags) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_AllocateDescriptorSets(VkDevice device, const VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_FreeDescriptorSets(VkDevice device, VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_UpdateDescriptorSets(VkDevice device, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, const VkCopyDescriptorSet* pDescriptorCopies) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateFramebuffer(VkDevice device, const VkFramebufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFramebuffer* pFramebuffer) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_DestroyFramebuffer(VkDevice device, VkFramebuffer framebuffer, const VkAllocationCallbacks* pAllocator) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateRenderPass(VkDevice device, const VkRenderPassCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_DestroyRenderPass(VkDevice device, VkRenderPass renderPass, const VkAllocationCallbacks* pAllocator) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_GetRenderAreaGranularity(VkDevice device, VkRenderPass renderPass, VkExtent2D* pGranularity) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_GetRenderingAreaGranularity(VkDevice device, const VkRenderingAreaInfo* pRenderingAreaInfo, VkExtent2D* pGranularity) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_GetRenderingAreaGranularityKHR(VkDevice device, const VkRenderingAreaInfo* pRenderingAreaInfo, VkExtent2D* pGranularity) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateCommandPool(VkDevice device, const VkCommandPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_DestroyCommandPool(VkDevice device, VkCommandPool commandPool, const VkAllocationCallbacks* pAllocator) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_ResetCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolResetFlags flags) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_AllocateCommandBuffers(VkDevice device, const VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_FreeCommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_BeginCommandBuffer(VkCommandBuffer commandBuffer, const VkCommandBufferBeginInfo* pBeginInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_EndCommandBuffer(VkCommandBuffer commandBuffer) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_ResetCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdBindPipeline(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetAttachmentFeedbackLoopEnableEXT(VkCommandBuffer commandBuffer, VkImageAspectFlags aspectMask) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetViewport(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pViewports) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetScissor(VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pScissors) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetLineWidth(VkCommandBuffer commandBuffer, float lineWidth) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetDepthBias(VkCommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetBlendConstants(VkCommandBuffer commandBuffer, const float blendConstants[4]) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetDepthBounds(VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetStencilCompareMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetStencilWriteMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetStencilReference(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdBindDescriptorSets(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdBindIndexBuffer(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdBindVertexBuffers(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdDraw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdDrawIndexed(VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdDrawMultiEXT(VkCommandBuffer commandBuffer, uint32_t drawCount, const VkMultiDrawInfoEXT* pVertexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdDrawMultiIndexedEXT(VkCommandBuffer commandBuffer, uint32_t drawCount, const VkMultiDrawIndexedInfoEXT* pIndexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride, const int32_t* pVertexOffset) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdDrawIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdDrawIndexedIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdDispatch(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdDispatchIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSubpassShadingHUAWEI(VkCommandBuffer commandBuffer) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdDrawClusterHUAWEI(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdDrawClusterIndirectHUAWEI(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdUpdatePipelineIndirectBufferNV(VkCommandBuffer commandBuffer, VkPipelineBindPoint           pipelineBindPoint, VkPipeline                    pipeline) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdCopyBuffer(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdCopyImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdBlitImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdCopyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdCopyImageToBuffer(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdCopyMemoryIndirectNV(VkCommandBuffer commandBuffer, VkDeviceAddress copyBufferAddress, uint32_t copyCount, uint32_t stride) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdCopyMemoryToImageIndirectNV(VkCommandBuffer commandBuffer, VkDeviceAddress copyBufferAddress, uint32_t copyCount, uint32_t stride, VkImage dstImage, VkImageLayout dstImageLayout, const VkImageSubresourceLayers* pImageSubresources) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdUpdateBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdFillBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdClearColorImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdClearDepthStencilImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdClearAttachments(VkCommandBuffer commandBuffer, uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdResolveImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageResolve* pRegions) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdResetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdWaitEvents(VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdPipelineBarrier(VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdBeginQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdEndQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdBeginConditionalRenderingEXT(VkCommandBuffer commandBuffer, const VkConditionalRenderingBeginInfoEXT* pConditionalRenderingBegin) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdEndConditionalRenderingEXT(VkCommandBuffer commandBuffer) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdResetQueryPool(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdWriteTimestamp(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdCopyQueryPoolResults(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdPushConstants(VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdBeginRenderPass(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdNextSubpass(VkCommandBuffer commandBuffer, VkSubpassContents contents) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdEndRenderPass(VkCommandBuffer commandBuffer) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdExecuteCommands(VkCommandBuffer commandBuffer, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateSharedSwapchainsKHR(VkDevice device, uint32_t swapchainCount, const VkSwapchainCreateInfoKHR* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchains) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateSwapchainKHR(VkDevice device, const VkSwapchainCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_DestroySwapchainKHR(VkDevice device, VkSwapchainKHR swapchain, const VkAllocationCallbacks* pAllocator) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetSwapchainImagesKHR(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_AcquireNextImageKHR(VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_QueuePresentKHR(VkQueue queue, const VkPresentInfoKHR* pPresentInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_DebugMarkerSetObjectNameEXT(VkDevice device, const VkDebugMarkerObjectNameInfoEXT* pNameInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_DebugMarkerSetObjectTagEXT(VkDevice device, const VkDebugMarkerObjectTagInfoEXT* pTagInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdDebugMarkerBeginEXT(VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdDebugMarkerEndEXT(VkCommandBuffer commandBuffer) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdDebugMarkerInsertEXT(VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetMemoryWin32HandleNV(VkDevice device, VkDeviceMemory memory, VkExternalMemoryHandleTypeFlagsNV handleType, HANDLE* pHandle) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


#endif // VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR void VKAPI_CALL vk_common_CmdExecuteGeneratedCommandsNV(VkCommandBuffer commandBuffer, VkBool32 isPreprocessed, const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdPreprocessGeneratedCommandsNV(VkCommandBuffer commandBuffer, const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdBindPipelineShaderGroupNV(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline, uint32_t groupIndex) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_GetGeneratedCommandsMemoryRequirementsNV(VkDevice device, const VkGeneratedCommandsMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2* pMemoryRequirements) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateIndirectCommandsLayoutNV(VkDevice device, const VkIndirectCommandsLayoutCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkIndirectCommandsLayoutNV* pIndirectCommandsLayout) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_DestroyIndirectCommandsLayoutNV(VkDevice device, VkIndirectCommandsLayoutNV indirectCommandsLayout, const VkAllocationCallbacks* pAllocator) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdExecuteGeneratedCommandsEXT(VkCommandBuffer commandBuffer, VkBool32 isPreprocessed, const VkGeneratedCommandsInfoEXT* pGeneratedCommandsInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdPreprocessGeneratedCommandsEXT(VkCommandBuffer commandBuffer, const VkGeneratedCommandsInfoEXT* pGeneratedCommandsInfo, VkCommandBuffer stateCommandBuffer) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_GetGeneratedCommandsMemoryRequirementsEXT(VkDevice device, const VkGeneratedCommandsMemoryRequirementsInfoEXT* pInfo, VkMemoryRequirements2* pMemoryRequirements) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateIndirectCommandsLayoutEXT(VkDevice device, const VkIndirectCommandsLayoutCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkIndirectCommandsLayoutEXT* pIndirectCommandsLayout) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_DestroyIndirectCommandsLayoutEXT(VkDevice device, VkIndirectCommandsLayoutEXT indirectCommandsLayout, const VkAllocationCallbacks* pAllocator) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateIndirectExecutionSetEXT(VkDevice device, const VkIndirectExecutionSetCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkIndirectExecutionSetEXT* pIndirectExecutionSet) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_DestroyIndirectExecutionSetEXT(VkDevice device, VkIndirectExecutionSetEXT indirectExecutionSet, const VkAllocationCallbacks* pAllocator) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_UpdateIndirectExecutionSetPipelineEXT(VkDevice device, VkIndirectExecutionSetEXT indirectExecutionSet, uint32_t executionSetWriteCount, const VkWriteIndirectExecutionSetPipelineEXT* pExecutionSetWrites) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_UpdateIndirectExecutionSetShaderEXT(VkDevice device, VkIndirectExecutionSetEXT indirectExecutionSet, uint32_t executionSetWriteCount, const VkWriteIndirectExecutionSetShaderEXT* pExecutionSetWrites) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdPushDescriptorSet(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdPushDescriptorSetKHR(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_TrimCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_TrimCommandPoolKHR(VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetMemoryWin32HandleKHR(VkDevice device, const VkMemoryGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


#endif // VK_USE_PLATFORM_WIN32_KHR
#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetMemoryWin32HandlePropertiesKHR(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, HANDLE handle, VkMemoryWin32HandlePropertiesKHR* pMemoryWin32HandleProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


#endif // VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetMemoryFdKHR(VkDevice device, const VkMemoryGetFdInfoKHR* pGetFdInfo, int* pFd) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetMemoryFdPropertiesKHR(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, int fd, VkMemoryFdPropertiesKHR* pMemoryFdProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


#ifdef VK_USE_PLATFORM_FUCHSIA
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetMemoryZirconHandleFUCHSIA(VkDevice device, const VkMemoryGetZirconHandleInfoFUCHSIA* pGetZirconHandleInfo, zx_handle_t* pZirconHandle) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


#endif // VK_USE_PLATFORM_FUCHSIA
#ifdef VK_USE_PLATFORM_FUCHSIA
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetMemoryZirconHandlePropertiesFUCHSIA(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, zx_handle_t zirconHandle, VkMemoryZirconHandlePropertiesFUCHSIA* pMemoryZirconHandleProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


#endif // VK_USE_PLATFORM_FUCHSIA
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetMemoryRemoteAddressNV(VkDevice device, const VkMemoryGetRemoteAddressInfoNV* pMemoryGetRemoteAddressInfo, VkRemoteAddressNV* pAddress) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetSemaphoreWin32HandleKHR(VkDevice device, const VkSemaphoreGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


#endif // VK_USE_PLATFORM_WIN32_KHR
#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_ImportSemaphoreWin32HandleKHR(VkDevice device, const VkImportSemaphoreWin32HandleInfoKHR* pImportSemaphoreWin32HandleInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


#endif // VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetSemaphoreFdKHR(VkDevice device, const VkSemaphoreGetFdInfoKHR* pGetFdInfo, int* pFd) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_ImportSemaphoreFdKHR(VkDevice device, const VkImportSemaphoreFdInfoKHR* pImportSemaphoreFdInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


#ifdef VK_USE_PLATFORM_FUCHSIA
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetSemaphoreZirconHandleFUCHSIA(VkDevice device, const VkSemaphoreGetZirconHandleInfoFUCHSIA* pGetZirconHandleInfo, zx_handle_t* pZirconHandle) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


#endif // VK_USE_PLATFORM_FUCHSIA
#ifdef VK_USE_PLATFORM_FUCHSIA
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_ImportSemaphoreZirconHandleFUCHSIA(VkDevice device, const VkImportSemaphoreZirconHandleInfoFUCHSIA* pImportSemaphoreZirconHandleInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


#endif // VK_USE_PLATFORM_FUCHSIA
#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetFenceWin32HandleKHR(VkDevice device, const VkFenceGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


#endif // VK_USE_PLATFORM_WIN32_KHR
#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_ImportFenceWin32HandleKHR(VkDevice device, const VkImportFenceWin32HandleInfoKHR* pImportFenceWin32HandleInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


#endif // VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetFenceFdKHR(VkDevice device, const VkFenceGetFdInfoKHR* pGetFdInfo, int* pFd) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_ImportFenceFdKHR(VkDevice device, const VkImportFenceFdInfoKHR* pImportFenceFdInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_DisplayPowerControlEXT(VkDevice device, VkDisplayKHR display, const VkDisplayPowerInfoEXT* pDisplayPowerInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_RegisterDeviceEventEXT(VkDevice device, const VkDeviceEventInfoEXT* pDeviceEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_RegisterDisplayEventEXT(VkDevice device, VkDisplayKHR display, const VkDisplayEventInfoEXT* pDisplayEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetSwapchainCounterEXT(VkDevice device, VkSwapchainKHR swapchain, VkSurfaceCounterFlagBitsEXT counter, uint64_t* pCounterValue) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_GetDeviceGroupPeerMemoryFeatures(VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_GetDeviceGroupPeerMemoryFeaturesKHR(VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_BindBufferMemory2(VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_BindBufferMemory2KHR(VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_BindImageMemory2(VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_BindImageMemory2KHR(VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetDeviceMask(VkCommandBuffer commandBuffer, uint32_t deviceMask) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetDeviceMaskKHR(VkCommandBuffer commandBuffer, uint32_t deviceMask) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetDeviceGroupPresentCapabilitiesKHR(VkDevice device, VkDeviceGroupPresentCapabilitiesKHR* pDeviceGroupPresentCapabilities) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetDeviceGroupSurfacePresentModesKHR(VkDevice device, VkSurfaceKHR surface, VkDeviceGroupPresentModeFlagsKHR* pModes) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_AcquireNextImage2KHR(VkDevice device, const VkAcquireNextImageInfoKHR* pAcquireInfo, uint32_t* pImageIndex) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdDispatchBase(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdDispatchBaseKHR(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateDescriptorUpdateTemplate(VkDevice device, const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateDescriptorUpdateTemplateKHR(VkDevice device, const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_DestroyDescriptorUpdateTemplate(VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_DestroyDescriptorUpdateTemplateKHR(VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_UpdateDescriptorSetWithTemplate(VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_UpdateDescriptorSetWithTemplateKHR(VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdPushDescriptorSetWithTemplate(VkCommandBuffer commandBuffer, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, const void* pData) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdPushDescriptorSetWithTemplateKHR(VkCommandBuffer commandBuffer, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, const void* pData) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_SetHdrMetadataEXT(VkDevice device, uint32_t swapchainCount, const VkSwapchainKHR* pSwapchains, const VkHdrMetadataEXT* pMetadata) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetSwapchainStatusKHR(VkDevice device, VkSwapchainKHR swapchain) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetRefreshCycleDurationGOOGLE(VkDevice device, VkSwapchainKHR swapchain, VkRefreshCycleDurationGOOGLE* pDisplayTimingProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetPastPresentationTimingGOOGLE(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pPresentationTimingCount, VkPastPresentationTimingGOOGLE* pPresentationTimings) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetViewportWScalingNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewportWScalingNV* pViewportWScalings) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetDiscardRectangleEXT(VkCommandBuffer commandBuffer, uint32_t firstDiscardRectangle, uint32_t discardRectangleCount, const VkRect2D* pDiscardRectangles) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetDiscardRectangleEnableEXT(VkCommandBuffer commandBuffer, VkBool32 discardRectangleEnable) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetDiscardRectangleModeEXT(VkCommandBuffer commandBuffer, VkDiscardRectangleModeEXT discardRectangleMode) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetSampleLocationsEXT(VkCommandBuffer commandBuffer, const VkSampleLocationsInfoEXT* pSampleLocationsInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_GetBufferMemoryRequirements2(VkDevice device, const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_GetBufferMemoryRequirements2KHR(VkDevice device, const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_GetImageMemoryRequirements2(VkDevice device, const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_GetImageMemoryRequirements2KHR(VkDevice device, const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_GetImageSparseMemoryRequirements2(VkDevice device, const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_GetImageSparseMemoryRequirements2KHR(VkDevice device, const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_GetDeviceBufferMemoryRequirements(VkDevice device, const VkDeviceBufferMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_GetDeviceBufferMemoryRequirementsKHR(VkDevice device, const VkDeviceBufferMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_GetDeviceImageMemoryRequirements(VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_GetDeviceImageMemoryRequirementsKHR(VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_GetDeviceImageSparseMemoryRequirements(VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_GetDeviceImageSparseMemoryRequirementsKHR(VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateSamplerYcbcrConversion(VkDevice device, const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateSamplerYcbcrConversionKHR(VkDevice device, const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_DestroySamplerYcbcrConversion(VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_DestroySamplerYcbcrConversionKHR(VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_GetDeviceQueue2(VkDevice device, const VkDeviceQueueInfo2* pQueueInfo, VkQueue* pQueue) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateValidationCacheEXT(VkDevice device, const VkValidationCacheCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkValidationCacheEXT* pValidationCache) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_DestroyValidationCacheEXT(VkDevice device, VkValidationCacheEXT validationCache, const VkAllocationCallbacks* pAllocator) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetValidationCacheDataEXT(VkDevice device, VkValidationCacheEXT validationCache, size_t* pDataSize, void* pData) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_MergeValidationCachesEXT(VkDevice device, VkValidationCacheEXT dstCache, uint32_t srcCacheCount, const VkValidationCacheEXT* pSrcCaches) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_GetDescriptorSetLayoutSupport(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_GetDescriptorSetLayoutSupportKHR(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


#ifdef VK_USE_PLATFORM_ANDROID_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetSwapchainGrallocUsageANDROID(VkDevice device, VkFormat format, VkImageUsageFlags imageUsage, int* grallocUsage) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetSwapchainGrallocUsage2ANDROID(VkDevice device, VkFormat format, VkImageUsageFlags imageUsage, VkSwapchainImageUsageFlagsANDROID swapchainImageUsage, uint64_t* grallocConsumerUsage, uint64_t* grallocProducerUsage) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_AcquireImageANDROID(VkDevice device, VkImage image, int nativeFenceFd, VkSemaphore semaphore, VkFence fence) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_QueueSignalReleaseImageANDROID(VkQueue queue, uint32_t waitSemaphoreCount, const VkSemaphore* pWaitSemaphores, VkImage image, int* pNativeFenceFd) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


#endif // VK_USE_PLATFORM_ANDROID_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetShaderInfoAMD(VkDevice device, VkPipeline pipeline, VkShaderStageFlagBits shaderStage, VkShaderInfoTypeAMD infoType, size_t* pInfoSize, void* pInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_SetLocalDimmingAMD(VkDevice device, VkSwapchainKHR swapChain, VkBool32 localDimmingEnable) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetCalibratedTimestampsKHR(VkDevice device, uint32_t timestampCount, const VkCalibratedTimestampInfoKHR* pTimestampInfos, uint64_t* pTimestamps, uint64_t* pMaxDeviation) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetCalibratedTimestampsEXT(VkDevice device, uint32_t timestampCount, const VkCalibratedTimestampInfoKHR* pTimestampInfos, uint64_t* pTimestamps, uint64_t* pMaxDeviation) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_SetDebugUtilsObjectNameEXT(VkDevice device, const VkDebugUtilsObjectNameInfoEXT* pNameInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_SetDebugUtilsObjectTagEXT(VkDevice device, const VkDebugUtilsObjectTagInfoEXT* pTagInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_QueueBeginDebugUtilsLabelEXT(VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_QueueEndDebugUtilsLabelEXT(VkQueue queue) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_QueueInsertDebugUtilsLabelEXT(VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdBeginDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdEndDebugUtilsLabelEXT(VkCommandBuffer commandBuffer) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdInsertDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetMemoryHostPointerPropertiesEXT(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, const void* pHostPointer, VkMemoryHostPointerPropertiesEXT* pMemoryHostPointerProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdWriteBufferMarkerAMD(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateRenderPass2(VkDevice device, const VkRenderPassCreateInfo2* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateRenderPass2KHR(VkDevice device, const VkRenderPassCreateInfo2* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdBeginRenderPass2(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo*      pRenderPassBegin, const VkSubpassBeginInfo*      pSubpassBeginInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdBeginRenderPass2KHR(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo*      pRenderPassBegin, const VkSubpassBeginInfo*      pSubpassBeginInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdNextSubpass2(VkCommandBuffer commandBuffer, const VkSubpassBeginInfo*      pSubpassBeginInfo, const VkSubpassEndInfo*        pSubpassEndInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdNextSubpass2KHR(VkCommandBuffer commandBuffer, const VkSubpassBeginInfo*      pSubpassBeginInfo, const VkSubpassEndInfo*        pSubpassEndInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdEndRenderPass2(VkCommandBuffer commandBuffer, const VkSubpassEndInfo*        pSubpassEndInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdEndRenderPass2KHR(VkCommandBuffer commandBuffer, const VkSubpassEndInfo*        pSubpassEndInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetSemaphoreCounterValue(VkDevice device, VkSemaphore semaphore, uint64_t* pValue) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetSemaphoreCounterValueKHR(VkDevice device, VkSemaphore semaphore, uint64_t* pValue) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_WaitSemaphores(VkDevice device, const VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_WaitSemaphoresKHR(VkDevice device, const VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_SignalSemaphore(VkDevice device, const VkSemaphoreSignalInfo* pSignalInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_SignalSemaphoreKHR(VkDevice device, const VkSemaphoreSignalInfo* pSignalInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


#ifdef VK_USE_PLATFORM_ANDROID_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetAndroidHardwareBufferPropertiesANDROID(VkDevice device, const struct AHardwareBuffer* buffer, VkAndroidHardwareBufferPropertiesANDROID* pProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetMemoryAndroidHardwareBufferANDROID(VkDevice device, const VkMemoryGetAndroidHardwareBufferInfoANDROID* pInfo, struct AHardwareBuffer** pBuffer) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


#endif // VK_USE_PLATFORM_ANDROID_KHR
  VKAPI_ATTR void VKAPI_CALL vk_common_CmdDrawIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdDrawIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdDrawIndirectCountAMD(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdDrawIndexedIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdDrawIndexedIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdDrawIndexedIndirectCountAMD(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetCheckpointNV(VkCommandBuffer commandBuffer, const void* pCheckpointMarker) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_GetQueueCheckpointDataNV(VkQueue queue, uint32_t* pCheckpointDataCount, VkCheckpointDataNV* pCheckpointData) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdBindTransformFeedbackBuffersEXT(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdBeginTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdEndTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdBeginQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags, uint32_t index) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdEndQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, uint32_t index) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdDrawIndirectByteCountEXT(VkCommandBuffer commandBuffer, uint32_t instanceCount, uint32_t firstInstance, VkBuffer counterBuffer, VkDeviceSize counterBufferOffset, uint32_t counterOffset, uint32_t vertexStride) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetExclusiveScissorNV(VkCommandBuffer commandBuffer, uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, const VkRect2D* pExclusiveScissors) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetExclusiveScissorEnableNV(VkCommandBuffer commandBuffer, uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, const VkBool32* pExclusiveScissorEnables) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdBindShadingRateImageNV(VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetViewportShadingRatePaletteNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkShadingRatePaletteNV* pShadingRatePalettes) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetCoarseSampleOrderNV(VkCommandBuffer commandBuffer, VkCoarseSampleOrderTypeNV sampleOrderType, uint32_t customSampleOrderCount, const VkCoarseSampleOrderCustomNV* pCustomSampleOrders) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdDrawMeshTasksNV(VkCommandBuffer commandBuffer, uint32_t taskCount, uint32_t firstTask) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdDrawMeshTasksIndirectNV(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdDrawMeshTasksIndirectCountNV(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdDrawMeshTasksEXT(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdDrawMeshTasksIndirectEXT(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdDrawMeshTasksIndirectCountEXT(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CompileDeferredNV(VkDevice device, VkPipeline pipeline, uint32_t shader) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateAccelerationStructureNV(VkDevice device, const VkAccelerationStructureCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkAccelerationStructureNV* pAccelerationStructure) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdBindInvocationMaskHUAWEI(VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_DestroyAccelerationStructureKHR(VkDevice device, VkAccelerationStructureKHR accelerationStructure, const VkAllocationCallbacks* pAllocator) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_DestroyAccelerationStructureNV(VkDevice device, VkAccelerationStructureNV accelerationStructure, const VkAllocationCallbacks* pAllocator) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_GetAccelerationStructureMemoryRequirementsNV(VkDevice device, const VkAccelerationStructureMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2KHR* pMemoryRequirements) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_BindAccelerationStructureMemoryNV(VkDevice device, uint32_t bindInfoCount, const VkBindAccelerationStructureMemoryInfoNV* pBindInfos) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdCopyAccelerationStructureNV(VkCommandBuffer commandBuffer, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkCopyAccelerationStructureModeKHR mode) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdCopyAccelerationStructureKHR(VkCommandBuffer commandBuffer, const VkCopyAccelerationStructureInfoKHR* pInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CopyAccelerationStructureKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyAccelerationStructureInfoKHR* pInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdCopyAccelerationStructureToMemoryKHR(VkCommandBuffer commandBuffer, const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CopyAccelerationStructureToMemoryKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdCopyMemoryToAccelerationStructureKHR(VkCommandBuffer commandBuffer, const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CopyMemoryToAccelerationStructureKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdWriteAccelerationStructuresPropertiesKHR(VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, const VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdWriteAccelerationStructuresPropertiesNV(VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, const VkAccelerationStructureNV* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdBuildAccelerationStructureNV(VkCommandBuffer commandBuffer, const VkAccelerationStructureInfoNV* pInfo, VkBuffer instanceData, VkDeviceSize instanceOffset, VkBool32 update, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkBuffer scratch, VkDeviceSize scratchOffset) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_WriteAccelerationStructuresPropertiesKHR(VkDevice device, uint32_t accelerationStructureCount, const VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType  queryType, size_t       dataSize, void* pData, size_t stride) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdTraceRaysKHR(VkCommandBuffer commandBuffer, const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, uint32_t width, uint32_t height, uint32_t depth) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdTraceRaysNV(VkCommandBuffer commandBuffer, VkBuffer raygenShaderBindingTableBuffer, VkDeviceSize raygenShaderBindingOffset, VkBuffer missShaderBindingTableBuffer, VkDeviceSize missShaderBindingOffset, VkDeviceSize missShaderBindingStride, VkBuffer hitShaderBindingTableBuffer, VkDeviceSize hitShaderBindingOffset, VkDeviceSize hitShaderBindingStride, VkBuffer callableShaderBindingTableBuffer, VkDeviceSize callableShaderBindingOffset, VkDeviceSize callableShaderBindingStride, uint32_t width, uint32_t height, uint32_t depth) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetRayTracingShaderGroupHandlesKHR(VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetRayTracingShaderGroupHandlesNV(VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetRayTracingCaptureReplayShaderGroupHandlesKHR(VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetAccelerationStructureHandleNV(VkDevice device, VkAccelerationStructureNV accelerationStructure, size_t dataSize, void* pData) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateRayTracingPipelinesNV(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkRayTracingPipelineCreateInfoNV* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateRayTracingPipelinesKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkRayTracingPipelineCreateInfoKHR* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdTraceRaysIndirectKHR(VkCommandBuffer commandBuffer, const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, VkDeviceAddress indirectDeviceAddress) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdTraceRaysIndirect2KHR(VkCommandBuffer commandBuffer, VkDeviceAddress indirectDeviceAddress) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_GetDeviceAccelerationStructureCompatibilityKHR(VkDevice device, const VkAccelerationStructureVersionInfoKHR* pVersionInfo, VkAccelerationStructureCompatibilityKHR* pCompatibility) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkDeviceSize VKAPI_CALL vk_common_GetRayTracingShaderGroupStackSizeKHR(VkDevice device, VkPipeline pipeline, uint32_t group, VkShaderGroupShaderKHR groupShader) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetRayTracingPipelineStackSizeKHR(VkCommandBuffer commandBuffer, uint32_t pipelineStackSize) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR uint32_t VKAPI_CALL vk_common_GetImageViewHandleNVX(VkDevice device, const VkImageViewHandleInfoNVX* pInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR uint64_t VKAPI_CALL vk_common_GetImageViewHandle64NVX(VkDevice device, const VkImageViewHandleInfoNVX* pInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetImageViewAddressNVX(VkDevice device, VkImageView imageView, VkImageViewAddressPropertiesNVX* pProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetDeviceGroupSurfacePresentModes2EXT(VkDevice device, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkDeviceGroupPresentModeFlagsKHR* pModes) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


#endif // VK_USE_PLATFORM_WIN32_KHR
#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_AcquireFullScreenExclusiveModeEXT(VkDevice device, VkSwapchainKHR swapchain) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


#endif // VK_USE_PLATFORM_WIN32_KHR
#ifdef VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_ReleaseFullScreenExclusiveModeEXT(VkDevice device, VkSwapchainKHR swapchain) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


#endif // VK_USE_PLATFORM_WIN32_KHR
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_AcquireProfilingLockKHR(VkDevice device, const VkAcquireProfilingLockInfoKHR* pInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_ReleaseProfilingLockKHR(VkDevice device) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetImageDrmFormatModifierPropertiesEXT(VkDevice device, VkImage image, VkImageDrmFormatModifierPropertiesEXT* pProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR uint64_t VKAPI_CALL vk_common_GetBufferOpaqueCaptureAddress(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR uint64_t VKAPI_CALL vk_common_GetBufferOpaqueCaptureAddressKHR(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkDeviceAddress VKAPI_CALL vk_common_GetBufferDeviceAddress(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkDeviceAddress VKAPI_CALL vk_common_GetBufferDeviceAddressKHR(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkDeviceAddress VKAPI_CALL vk_common_GetBufferDeviceAddressEXT(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_InitializePerformanceApiINTEL(VkDevice device, const VkInitializePerformanceApiInfoINTEL* pInitializeInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_UninitializePerformanceApiINTEL(VkDevice device) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CmdSetPerformanceMarkerINTEL(VkCommandBuffer commandBuffer, const VkPerformanceMarkerInfoINTEL* pMarkerInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CmdSetPerformanceStreamMarkerINTEL(VkCommandBuffer commandBuffer, const VkPerformanceStreamMarkerInfoINTEL* pMarkerInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CmdSetPerformanceOverrideINTEL(VkCommandBuffer commandBuffer, const VkPerformanceOverrideInfoINTEL* pOverrideInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_AcquirePerformanceConfigurationINTEL(VkDevice device, const VkPerformanceConfigurationAcquireInfoINTEL* pAcquireInfo, VkPerformanceConfigurationINTEL* pConfiguration) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_ReleasePerformanceConfigurationINTEL(VkDevice device, VkPerformanceConfigurationINTEL configuration) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_QueueSetPerformanceConfigurationINTEL(VkQueue queue, VkPerformanceConfigurationINTEL configuration) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetPerformanceParameterINTEL(VkDevice device, VkPerformanceParameterTypeINTEL parameter, VkPerformanceValueINTEL* pValue) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR uint64_t VKAPI_CALL vk_common_GetDeviceMemoryOpaqueCaptureAddress(VkDevice device, const VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR uint64_t VKAPI_CALL vk_common_GetDeviceMemoryOpaqueCaptureAddressKHR(VkDevice device, const VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetPipelineExecutablePropertiesKHR(VkDevice                        device, const VkPipelineInfoKHR*        pPipelineInfo, uint32_t* pExecutableCount, VkPipelineExecutablePropertiesKHR* pProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetPipelineExecutableStatisticsKHR(VkDevice                        device, const VkPipelineExecutableInfoKHR*  pExecutableInfo, uint32_t* pStatisticCount, VkPipelineExecutableStatisticKHR* pStatistics) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetPipelineExecutableInternalRepresentationsKHR(VkDevice                        device, const VkPipelineExecutableInfoKHR*  pExecutableInfo, uint32_t* pInternalRepresentationCount, VkPipelineExecutableInternalRepresentationKHR* pInternalRepresentations) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetLineStipple(VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetLineStippleKHR(VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetLineStippleEXT(VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateAccelerationStructureKHR(VkDevice                                           device, const VkAccelerationStructureCreateInfoKHR*        pCreateInfo, const VkAllocationCallbacks*       pAllocator, VkAccelerationStructureKHR*                        pAccelerationStructure) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdBuildAccelerationStructuresKHR(VkCommandBuffer                                    commandBuffer, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdBuildAccelerationStructuresIndirectKHR(VkCommandBuffer                  commandBuffer, uint32_t                                           infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkDeviceAddress*             pIndirectDeviceAddresses, const uint32_t*                    pIndirectStrides, const uint32_t* const*             ppMaxPrimitiveCounts) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_BuildAccelerationStructuresKHR(VkDevice                                           device, VkDeferredOperationKHR deferredOperation, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkDeviceAddress VKAPI_CALL vk_common_GetAccelerationStructureDeviceAddressKHR(VkDevice device, const VkAccelerationStructureDeviceAddressInfoKHR* pInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateDeferredOperationKHR(VkDevice device, const VkAllocationCallbacks* pAllocator, VkDeferredOperationKHR* pDeferredOperation) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_DestroyDeferredOperationKHR(VkDevice device, VkDeferredOperationKHR operation, const VkAllocationCallbacks* pAllocator) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR uint32_t VKAPI_CALL vk_common_GetDeferredOperationMaxConcurrencyKHR(VkDevice device, VkDeferredOperationKHR operation) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetDeferredOperationResultKHR(VkDevice device, VkDeferredOperationKHR operation) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_DeferredOperationJoinKHR(VkDevice device, VkDeferredOperationKHR operation) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_GetPipelineIndirectMemoryRequirementsNV(VkDevice device, const VkComputePipelineCreateInfo* pCreateInfo, VkMemoryRequirements2* pMemoryRequirements) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkDeviceAddress VKAPI_CALL vk_common_GetPipelineIndirectDeviceAddressNV(VkDevice device, const VkPipelineIndirectDeviceAddressInfoNV* pInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_AntiLagUpdateAMD(VkDevice device, const VkAntiLagDataAMD* pData) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetCullMode(VkCommandBuffer commandBuffer, VkCullModeFlags cullMode) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetCullModeEXT(VkCommandBuffer commandBuffer, VkCullModeFlags cullMode) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetFrontFace(VkCommandBuffer commandBuffer, VkFrontFace frontFace) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetFrontFaceEXT(VkCommandBuffer commandBuffer, VkFrontFace frontFace) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetPrimitiveTopology(VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetPrimitiveTopologyEXT(VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetViewportWithCount(VkCommandBuffer commandBuffer, uint32_t viewportCount, const VkViewport* pViewports) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetViewportWithCountEXT(VkCommandBuffer commandBuffer, uint32_t viewportCount, const VkViewport* pViewports) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetScissorWithCount(VkCommandBuffer commandBuffer, uint32_t scissorCount, const VkRect2D* pScissors) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetScissorWithCountEXT(VkCommandBuffer commandBuffer, uint32_t scissorCount, const VkRect2D* pScissors) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdBindIndexBuffer2(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkDeviceSize size, VkIndexType indexType) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdBindIndexBuffer2KHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkDeviceSize size, VkIndexType indexType) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdBindVertexBuffers2(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes, const VkDeviceSize* pStrides) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdBindVertexBuffers2EXT(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes, const VkDeviceSize* pStrides) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetDepthTestEnable(VkCommandBuffer commandBuffer, VkBool32 depthTestEnable) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetDepthTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthTestEnable) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetDepthWriteEnable(VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetDepthWriteEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetDepthCompareOp(VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetDepthCompareOpEXT(VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetDepthBoundsTestEnable(VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetDepthBoundsTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetStencilTestEnable(VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetStencilTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetStencilOp(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetStencilOpEXT(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetPatchControlPointsEXT(VkCommandBuffer commandBuffer, uint32_t patchControlPoints) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetRasterizerDiscardEnable(VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetRasterizerDiscardEnableEXT(VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetDepthBiasEnable(VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetDepthBiasEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetLogicOpEXT(VkCommandBuffer commandBuffer, VkLogicOp logicOp) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetPrimitiveRestartEnable(VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetPrimitiveRestartEnableEXT(VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetTessellationDomainOriginEXT(VkCommandBuffer commandBuffer, VkTessellationDomainOrigin domainOrigin) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetDepthClampEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthClampEnable) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetPolygonModeEXT(VkCommandBuffer commandBuffer, VkPolygonMode polygonMode) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetRasterizationSamplesEXT(VkCommandBuffer commandBuffer, VkSampleCountFlagBits  rasterizationSamples) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetSampleMaskEXT(VkCommandBuffer commandBuffer, VkSampleCountFlagBits  samples, const VkSampleMask*    pSampleMask) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetAlphaToCoverageEnableEXT(VkCommandBuffer commandBuffer, VkBool32 alphaToCoverageEnable) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetAlphaToOneEnableEXT(VkCommandBuffer commandBuffer, VkBool32 alphaToOneEnable) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetLogicOpEnableEXT(VkCommandBuffer commandBuffer, VkBool32 logicOpEnable) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetColorBlendEnableEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkBool32* pColorBlendEnables) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetColorBlendEquationEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorBlendEquationEXT* pColorBlendEquations) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetColorWriteMaskEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorComponentFlags* pColorWriteMasks) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetRasterizationStreamEXT(VkCommandBuffer commandBuffer, uint32_t rasterizationStream) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetConservativeRasterizationModeEXT(VkCommandBuffer commandBuffer, VkConservativeRasterizationModeEXT conservativeRasterizationMode) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetExtraPrimitiveOverestimationSizeEXT(VkCommandBuffer commandBuffer, float extraPrimitiveOverestimationSize) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetDepthClipEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthClipEnable) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetSampleLocationsEnableEXT(VkCommandBuffer commandBuffer, VkBool32 sampleLocationsEnable) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetColorBlendAdvancedEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorBlendAdvancedEXT* pColorBlendAdvanced) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetProvokingVertexModeEXT(VkCommandBuffer commandBuffer, VkProvokingVertexModeEXT provokingVertexMode) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetLineRasterizationModeEXT(VkCommandBuffer commandBuffer, VkLineRasterizationModeEXT lineRasterizationMode) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetLineStippleEnableEXT(VkCommandBuffer commandBuffer, VkBool32 stippledLineEnable) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetDepthClipNegativeOneToOneEXT(VkCommandBuffer commandBuffer, VkBool32 negativeOneToOne) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetViewportWScalingEnableNV(VkCommandBuffer commandBuffer, VkBool32 viewportWScalingEnable) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetViewportSwizzleNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewportSwizzleNV* pViewportSwizzles) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetCoverageToColorEnableNV(VkCommandBuffer commandBuffer, VkBool32 coverageToColorEnable) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetCoverageToColorLocationNV(VkCommandBuffer commandBuffer, uint32_t coverageToColorLocation) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetCoverageModulationModeNV(VkCommandBuffer commandBuffer, VkCoverageModulationModeNV coverageModulationMode) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetCoverageModulationTableEnableNV(VkCommandBuffer commandBuffer, VkBool32 coverageModulationTableEnable) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetCoverageModulationTableNV(VkCommandBuffer commandBuffer, uint32_t coverageModulationTableCount, const float* pCoverageModulationTable) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetShadingRateImageEnableNV(VkCommandBuffer commandBuffer, VkBool32 shadingRateImageEnable) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetCoverageReductionModeNV(VkCommandBuffer commandBuffer, VkCoverageReductionModeNV coverageReductionMode) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetRepresentativeFragmentTestEnableNV(VkCommandBuffer commandBuffer, VkBool32 representativeFragmentTestEnable) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreatePrivateDataSlot(VkDevice device, const VkPrivateDataSlotCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPrivateDataSlot* pPrivateDataSlot) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreatePrivateDataSlotEXT(VkDevice device, const VkPrivateDataSlotCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPrivateDataSlot* pPrivateDataSlot) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_DestroyPrivateDataSlot(VkDevice device, VkPrivateDataSlot privateDataSlot, const VkAllocationCallbacks* pAllocator) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_DestroyPrivateDataSlotEXT(VkDevice device, VkPrivateDataSlot privateDataSlot, const VkAllocationCallbacks* pAllocator) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_SetPrivateData(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t data) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_SetPrivateDataEXT(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t data) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_GetPrivateData(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t* pData) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_GetPrivateDataEXT(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t* pData) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdCopyBuffer2(VkCommandBuffer commandBuffer, const VkCopyBufferInfo2* pCopyBufferInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdCopyBuffer2KHR(VkCommandBuffer commandBuffer, const VkCopyBufferInfo2* pCopyBufferInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdCopyImage2(VkCommandBuffer commandBuffer, const VkCopyImageInfo2* pCopyImageInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdCopyImage2KHR(VkCommandBuffer commandBuffer, const VkCopyImageInfo2* pCopyImageInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdBlitImage2(VkCommandBuffer commandBuffer, const VkBlitImageInfo2* pBlitImageInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdBlitImage2KHR(VkCommandBuffer commandBuffer, const VkBlitImageInfo2* pBlitImageInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdCopyBufferToImage2(VkCommandBuffer commandBuffer, const VkCopyBufferToImageInfo2* pCopyBufferToImageInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdCopyBufferToImage2KHR(VkCommandBuffer commandBuffer, const VkCopyBufferToImageInfo2* pCopyBufferToImageInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdCopyImageToBuffer2(VkCommandBuffer commandBuffer, const VkCopyImageToBufferInfo2* pCopyImageToBufferInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdCopyImageToBuffer2KHR(VkCommandBuffer commandBuffer, const VkCopyImageToBufferInfo2* pCopyImageToBufferInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdResolveImage2(VkCommandBuffer commandBuffer, const VkResolveImageInfo2* pResolveImageInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdResolveImage2KHR(VkCommandBuffer commandBuffer, const VkResolveImageInfo2* pResolveImageInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetFragmentShadingRateKHR(VkCommandBuffer           commandBuffer, const VkExtent2D*                           pFragmentSize, const VkFragmentShadingRateCombinerOpKHR    combinerOps[2]) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetFragmentShadingRateEnumNV(VkCommandBuffer           commandBuffer, VkFragmentShadingRateNV                     shadingRate, const VkFragmentShadingRateCombinerOpKHR    combinerOps[2]) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_GetAccelerationStructureBuildSizesKHR(VkDevice                                            device, VkAccelerationStructureBuildTypeKHR                 buildType, const VkAccelerationStructureBuildGeometryInfoKHR*  pBuildInfo, const uint32_t*  pMaxPrimitiveCounts, VkAccelerationStructureBuildSizesInfoKHR*           pSizeInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetVertexInputEXT(VkCommandBuffer commandBuffer, uint32_t vertexBindingDescriptionCount, const VkVertexInputBindingDescription2EXT* pVertexBindingDescriptions, uint32_t vertexAttributeDescriptionCount, const VkVertexInputAttributeDescription2EXT* pVertexAttributeDescriptions) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetColorWriteEnableEXT(VkCommandBuffer       commandBuffer, uint32_t                                attachmentCount, const VkBool32*   pColorWriteEnables) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetEvent2(VkCommandBuffer                   commandBuffer, VkEvent                                             event, const VkDependencyInfo*                             pDependencyInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetEvent2KHR(VkCommandBuffer                   commandBuffer, VkEvent                                             event, const VkDependencyInfo*                             pDependencyInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdResetEvent2(VkCommandBuffer                   commandBuffer, VkEvent                                             event, VkPipelineStageFlags2               stageMask) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdResetEvent2KHR(VkCommandBuffer                   commandBuffer, VkEvent                                             event, VkPipelineStageFlags2               stageMask) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdWaitEvents2(VkCommandBuffer                   commandBuffer, uint32_t                                            eventCount, const VkEvent*                     pEvents, const VkDependencyInfo*            pDependencyInfos) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdWaitEvents2KHR(VkCommandBuffer                   commandBuffer, uint32_t                                            eventCount, const VkEvent*                     pEvents, const VkDependencyInfo*            pDependencyInfos) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdPipelineBarrier2(VkCommandBuffer                   commandBuffer, const VkDependencyInfo*                             pDependencyInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdPipelineBarrier2KHR(VkCommandBuffer                   commandBuffer, const VkDependencyInfo*                             pDependencyInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_QueueSubmit2(VkQueue                           queue, uint32_t                            submitCount, const VkSubmitInfo2*              pSubmits, VkFence           fence) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_QueueSubmit2KHR(VkQueue                           queue, uint32_t                            submitCount, const VkSubmitInfo2*              pSubmits, VkFence           fence) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdWriteTimestamp2(VkCommandBuffer                   commandBuffer, VkPipelineStageFlags2               stage, VkQueryPool                                         queryPool, uint32_t                                            query) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdWriteTimestamp2KHR(VkCommandBuffer                   commandBuffer, VkPipelineStageFlags2               stage, VkQueryPool                                         queryPool, uint32_t                                            query) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdWriteBufferMarker2AMD(VkCommandBuffer                   commandBuffer, VkPipelineStageFlags2               stage, VkBuffer                                            dstBuffer, VkDeviceSize                                        dstOffset, uint32_t                                            marker) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_GetQueueCheckpointData2NV(VkQueue queue, uint32_t* pCheckpointDataCount, VkCheckpointData2NV* pCheckpointData) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CopyMemoryToImage(VkDevice device, const VkCopyMemoryToImageInfo*    pCopyMemoryToImageInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CopyMemoryToImageEXT(VkDevice device, const VkCopyMemoryToImageInfo*    pCopyMemoryToImageInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CopyImageToMemory(VkDevice device, const VkCopyImageToMemoryInfo*    pCopyImageToMemoryInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CopyImageToMemoryEXT(VkDevice device, const VkCopyImageToMemoryInfo*    pCopyImageToMemoryInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CopyImageToImage(VkDevice device, const VkCopyImageToImageInfo*    pCopyImageToImageInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CopyImageToImageEXT(VkDevice device, const VkCopyImageToImageInfo*    pCopyImageToImageInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_TransitionImageLayout(VkDevice device, uint32_t transitionCount, const VkHostImageLayoutTransitionInfo*    pTransitions) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_TransitionImageLayoutEXT(VkDevice device, uint32_t transitionCount, const VkHostImageLayoutTransitionInfo*    pTransitions) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateVideoSessionKHR(VkDevice device, const VkVideoSessionCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkVideoSessionKHR* pVideoSession) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_DestroyVideoSessionKHR(VkDevice device, VkVideoSessionKHR videoSession, const VkAllocationCallbacks* pAllocator) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateVideoSessionParametersKHR(VkDevice device, const VkVideoSessionParametersCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkVideoSessionParametersKHR* pVideoSessionParameters) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_UpdateVideoSessionParametersKHR(VkDevice device, VkVideoSessionParametersKHR videoSessionParameters, const VkVideoSessionParametersUpdateInfoKHR* pUpdateInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetEncodedVideoSessionParametersKHR(VkDevice device, const VkVideoEncodeSessionParametersGetInfoKHR* pVideoSessionParametersInfo, VkVideoEncodeSessionParametersFeedbackInfoKHR* pFeedbackInfo, size_t* pDataSize, void* pData) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_DestroyVideoSessionParametersKHR(VkDevice device, VkVideoSessionParametersKHR videoSessionParameters, const VkAllocationCallbacks* pAllocator) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetVideoSessionMemoryRequirementsKHR(VkDevice device, VkVideoSessionKHR videoSession, uint32_t* pMemoryRequirementsCount, VkVideoSessionMemoryRequirementsKHR* pMemoryRequirements) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_BindVideoSessionMemoryKHR(VkDevice device, VkVideoSessionKHR videoSession, uint32_t bindSessionMemoryInfoCount, const VkBindVideoSessionMemoryInfoKHR* pBindSessionMemoryInfos) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdDecodeVideoKHR(VkCommandBuffer commandBuffer, const VkVideoDecodeInfoKHR* pDecodeInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdBeginVideoCodingKHR(VkCommandBuffer commandBuffer, const VkVideoBeginCodingInfoKHR* pBeginInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdControlVideoCodingKHR(VkCommandBuffer commandBuffer, const VkVideoCodingControlInfoKHR* pCodingControlInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdEndVideoCodingKHR(VkCommandBuffer commandBuffer, const VkVideoEndCodingInfoKHR* pEndCodingInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdEncodeVideoKHR(VkCommandBuffer commandBuffer, const VkVideoEncodeInfoKHR* pEncodeInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdDecompressMemoryNV(VkCommandBuffer commandBuffer, uint32_t decompressRegionCount, const VkDecompressMemoryRegionNV* pDecompressMemoryRegions) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdDecompressMemoryIndirectCountNV(VkCommandBuffer commandBuffer, VkDeviceAddress indirectCommandsAddress, VkDeviceAddress indirectCommandsCountAddress, uint32_t stride) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateCuModuleNVX(VkDevice device, const VkCuModuleCreateInfoNVX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCuModuleNVX* pModule) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateCuFunctionNVX(VkDevice device, const VkCuFunctionCreateInfoNVX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCuFunctionNVX* pFunction) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_DestroyCuModuleNVX(VkDevice device, VkCuModuleNVX module, const VkAllocationCallbacks* pAllocator) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_DestroyCuFunctionNVX(VkDevice device, VkCuFunctionNVX function, const VkAllocationCallbacks* pAllocator) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdCuLaunchKernelNVX(VkCommandBuffer commandBuffer, const VkCuLaunchInfoNVX* pLaunchInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_GetDescriptorSetLayoutSizeEXT(VkDevice device, VkDescriptorSetLayout layout, VkDeviceSize* pLayoutSizeInBytes) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_GetDescriptorSetLayoutBindingOffsetEXT(VkDevice device, VkDescriptorSetLayout layout, uint32_t binding, VkDeviceSize* pOffset) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_GetDescriptorEXT(VkDevice device, const VkDescriptorGetInfoEXT* pDescriptorInfo, size_t dataSize, void* pDescriptor) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdBindDescriptorBuffersEXT(VkCommandBuffer commandBuffer, uint32_t bufferCount, const VkDescriptorBufferBindingInfoEXT* pBindingInfos) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetDescriptorBufferOffsetsEXT(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t setCount, const uint32_t* pBufferIndices, const VkDeviceSize* pOffsets) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdBindDescriptorBufferEmbeddedSamplersEXT(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetBufferOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkBufferCaptureDescriptorDataInfoEXT* pInfo, void* pData) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetImageOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkImageCaptureDescriptorDataInfoEXT* pInfo, void* pData) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetImageViewOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkImageViewCaptureDescriptorDataInfoEXT* pInfo, void* pData) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetSamplerOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkSamplerCaptureDescriptorDataInfoEXT* pInfo, void* pData) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetAccelerationStructureOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkAccelerationStructureCaptureDescriptorDataInfoEXT* pInfo, void* pData) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_SetDeviceMemoryPriorityEXT(VkDevice       device, VkDeviceMemory memory, float          priority) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_WaitForPresentKHR(VkDevice device, VkSwapchainKHR swapchain, uint64_t presentId, uint64_t timeout) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


#ifdef VK_USE_PLATFORM_FUCHSIA
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateBufferCollectionFUCHSIA(VkDevice device, const VkBufferCollectionCreateInfoFUCHSIA* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferCollectionFUCHSIA* pCollection) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


#endif // VK_USE_PLATFORM_FUCHSIA
#ifdef VK_USE_PLATFORM_FUCHSIA
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_SetBufferCollectionBufferConstraintsFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, const VkBufferConstraintsInfoFUCHSIA* pBufferConstraintsInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


#endif // VK_USE_PLATFORM_FUCHSIA
#ifdef VK_USE_PLATFORM_FUCHSIA
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_SetBufferCollectionImageConstraintsFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, const VkImageConstraintsInfoFUCHSIA* pImageConstraintsInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


#endif // VK_USE_PLATFORM_FUCHSIA
#ifdef VK_USE_PLATFORM_FUCHSIA
  VKAPI_ATTR void VKAPI_CALL vk_common_DestroyBufferCollectionFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, const VkAllocationCallbacks* pAllocator) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


#endif // VK_USE_PLATFORM_FUCHSIA
#ifdef VK_USE_PLATFORM_FUCHSIA
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetBufferCollectionPropertiesFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, VkBufferCollectionPropertiesFUCHSIA* pProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


#endif // VK_USE_PLATFORM_FUCHSIA
  VKAPI_ATTR void VKAPI_CALL vk_common_CmdBeginRendering(VkCommandBuffer                   commandBuffer, const VkRenderingInfo*                              pRenderingInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdBeginRenderingKHR(VkCommandBuffer                   commandBuffer, const VkRenderingInfo*                              pRenderingInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdEndRendering(VkCommandBuffer                   commandBuffer) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdEndRenderingKHR(VkCommandBuffer                   commandBuffer) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_GetDescriptorSetLayoutHostMappingInfoVALVE(VkDevice device, const VkDescriptorSetBindingReferenceVALVE* pBindingReference, VkDescriptorSetLayoutHostMappingInfoVALVE* pHostMapping) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_GetDescriptorSetHostMappingVALVE(VkDevice device, VkDescriptorSet descriptorSet, void** ppData) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateMicromapEXT(VkDevice                                           device, const VkMicromapCreateInfoEXT*        pCreateInfo, const VkAllocationCallbacks*       pAllocator, VkMicromapEXT*                        pMicromap) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdBuildMicromapsEXT(VkCommandBuffer                                    commandBuffer, uint32_t infoCount, const VkMicromapBuildInfoEXT* pInfos) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_BuildMicromapsEXT(VkDevice                                           device, VkDeferredOperationKHR deferredOperation, uint32_t infoCount, const VkMicromapBuildInfoEXT* pInfos) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_DestroyMicromapEXT(VkDevice device, VkMicromapEXT micromap, const VkAllocationCallbacks* pAllocator) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdCopyMicromapEXT(VkCommandBuffer commandBuffer, const VkCopyMicromapInfoEXT* pInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CopyMicromapEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMicromapInfoEXT* pInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdCopyMicromapToMemoryEXT(VkCommandBuffer commandBuffer, const VkCopyMicromapToMemoryInfoEXT* pInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CopyMicromapToMemoryEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMicromapToMemoryInfoEXT* pInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdCopyMemoryToMicromapEXT(VkCommandBuffer commandBuffer, const VkCopyMemoryToMicromapInfoEXT* pInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CopyMemoryToMicromapEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMemoryToMicromapInfoEXT* pInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdWriteMicromapsPropertiesEXT(VkCommandBuffer commandBuffer, uint32_t micromapCount, const VkMicromapEXT* pMicromaps, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_WriteMicromapsPropertiesEXT(VkDevice device, uint32_t micromapCount, const VkMicromapEXT* pMicromaps, VkQueryType  queryType, size_t       dataSize, void* pData, size_t stride) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_GetDeviceMicromapCompatibilityEXT(VkDevice device, const VkMicromapVersionInfoEXT* pVersionInfo, VkAccelerationStructureCompatibilityKHR* pCompatibility) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_GetMicromapBuildSizesEXT(VkDevice                                            device, VkAccelerationStructureBuildTypeKHR                 buildType, const VkMicromapBuildInfoEXT*  pBuildInfo, VkMicromapBuildSizesInfoEXT*           pSizeInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_GetShaderModuleIdentifierEXT(VkDevice device, VkShaderModule shaderModule, VkShaderModuleIdentifierEXT* pIdentifier) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_GetShaderModuleCreateInfoIdentifierEXT(VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, VkShaderModuleIdentifierEXT* pIdentifier) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_GetImageSubresourceLayout2(VkDevice device, VkImage image, const VkImageSubresource2* pSubresource, VkSubresourceLayout2* pLayout) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_GetImageSubresourceLayout2KHR(VkDevice device, VkImage image, const VkImageSubresource2* pSubresource, VkSubresourceLayout2* pLayout) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_GetImageSubresourceLayout2EXT(VkDevice device, VkImage image, const VkImageSubresource2* pSubresource, VkSubresourceLayout2* pLayout) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetPipelinePropertiesEXT(VkDevice device, const VkPipelineInfoEXT* pPipelineInfo, VkBaseOutStructure* pPipelineProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


#ifdef VK_USE_PLATFORM_METAL_EXT
  VKAPI_ATTR void VKAPI_CALL vk_common_ExportMetalObjectsEXT(VkDevice device, VkExportMetalObjectsInfoEXT* pMetalObjectsInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


#endif // VK_USE_PLATFORM_METAL_EXT
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetFramebufferTilePropertiesQCOM(VkDevice device, VkFramebuffer framebuffer, uint32_t* pPropertiesCount, VkTilePropertiesQCOM* pProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetDynamicRenderingTilePropertiesQCOM(VkDevice device, const VkRenderingInfo* pRenderingInfo, VkTilePropertiesQCOM* pProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateOpticalFlowSessionNV(VkDevice device, const VkOpticalFlowSessionCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkOpticalFlowSessionNV* pSession) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_DestroyOpticalFlowSessionNV(VkDevice device, VkOpticalFlowSessionNV session, const VkAllocationCallbacks* pAllocator) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_BindOpticalFlowSessionImageNV(VkDevice device, VkOpticalFlowSessionNV session, VkOpticalFlowSessionBindingPointNV bindingPoint, VkImageView view, VkImageLayout layout) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdOpticalFlowExecuteNV(VkCommandBuffer commandBuffer, VkOpticalFlowSessionNV session, const VkOpticalFlowExecuteInfoNV* pExecuteInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetDeviceFaultInfoEXT(VkDevice device, VkDeviceFaultCountsEXT* pFaultCounts, VkDeviceFaultInfoEXT* pFaultInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetDepthBias2EXT(VkCommandBuffer commandBuffer, const VkDepthBiasInfoEXT*         pDepthBiasInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_ReleaseSwapchainImagesEXT(VkDevice device, const VkReleaseSwapchainImagesInfoEXT* pReleaseInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_GetDeviceImageSubresourceLayout(VkDevice device, const VkDeviceImageSubresourceInfo* pInfo, VkSubresourceLayout2* pLayout) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_GetDeviceImageSubresourceLayoutKHR(VkDevice device, const VkDeviceImageSubresourceInfo* pInfo, VkSubresourceLayout2* pLayout) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_MapMemory2(VkDevice device, const VkMemoryMapInfo* pMemoryMapInfo, void** ppData) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_MapMemory2KHR(VkDevice device, const VkMemoryMapInfo* pMemoryMapInfo, void** ppData) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_UnmapMemory2(VkDevice device, const VkMemoryUnmapInfo* pMemoryUnmapInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_UnmapMemory2KHR(VkDevice device, const VkMemoryUnmapInfo* pMemoryUnmapInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_CreateShadersEXT(VkDevice device, uint32_t createInfoCount, const VkShaderCreateInfoEXT* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkShaderEXT* pShaders) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_DestroyShaderEXT(VkDevice device, VkShaderEXT shader, const VkAllocationCallbacks* pAllocator) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetShaderBinaryDataEXT(VkDevice device, VkShaderEXT shader, size_t* pDataSize, void* pData) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdBindShadersEXT(VkCommandBuffer commandBuffer, uint32_t stageCount, const VkShaderStageFlagBits* pStages, const VkShaderEXT* pShaders) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


#ifdef VK_USE_PLATFORM_SCREEN_QNX
  VKAPI_ATTR VkResult VKAPI_CALL vk_common_GetScreenBufferPropertiesQNX(VkDevice device, const struct _screen_buffer* buffer, VkScreenBufferPropertiesQNX* pProperties) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


#endif // VK_USE_PLATFORM_SCREEN_QNX
  VKAPI_ATTR void VKAPI_CALL vk_common_CmdBindDescriptorSets2(VkCommandBuffer commandBuffer, const VkBindDescriptorSetsInfo*   pBindDescriptorSetsInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdBindDescriptorSets2KHR(VkCommandBuffer commandBuffer, const VkBindDescriptorSetsInfo*   pBindDescriptorSetsInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdPushConstants2(VkCommandBuffer commandBuffer, const VkPushConstantsInfo*        pPushConstantsInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdPushConstants2KHR(VkCommandBuffer commandBuffer, const VkPushConstantsInfo*        pPushConstantsInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdPushDescriptorSet2(VkCommandBuffer commandBuffer, const VkPushDescriptorSetInfo*    pPushDescriptorSetInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdPushDescriptorSet2KHR(VkCommandBuffer commandBuffer, const VkPushDescriptorSetInfo*    pPushDescriptorSetInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdPushDescriptorSetWithTemplate2(VkCommandBuffer commandBuffer, const VkPushDescriptorSetWithTemplateInfo* pPushDescriptorSetWithTemplateInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdPushDescriptorSetWithTemplate2KHR(VkCommandBuffer commandBuffer, const VkPushDescriptorSetWithTemplateInfo* pPushDescriptorSetWithTemplateInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetDescriptorBufferOffsets2EXT(VkCommandBuffer commandBuffer, const VkSetDescriptorBufferOffsetsInfoEXT* pSetDescriptorBufferOffsetsInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdBindDescriptorBufferEmbeddedSamplers2EXT(VkCommandBuffer commandBuffer, const VkBindDescriptorBufferEmbeddedSamplersInfoEXT* pBindDescriptorBufferEmbeddedSamplersInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_SetLatencySleepModeNV(VkDevice device, VkSwapchainKHR swapchain, const VkLatencySleepModeInfoNV* pSleepModeInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR VkResult VKAPI_CALL vk_common_LatencySleepNV(VkDevice device, VkSwapchainKHR swapchain, const VkLatencySleepInfoNV* pSleepInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_SetLatencyMarkerNV(VkDevice device, VkSwapchainKHR swapchain, const VkSetLatencyMarkerInfoNV* pLatencyMarkerInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_GetLatencyTimingsNV(VkDevice device, VkSwapchainKHR swapchain, VkGetLatencyMarkerInfoNV* pLatencyMarkerInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_QueueNotifyOutOfBandNV(VkQueue queue, const VkOutOfBandQueueTypeInfoNV* pQueueTypeInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetRenderingAttachmentLocations(VkCommandBuffer commandBuffer, const VkRenderingAttachmentLocationInfo* pLocationInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetRenderingAttachmentLocationsKHR(VkCommandBuffer commandBuffer, const VkRenderingAttachmentLocationInfo* pLocationInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetRenderingInputAttachmentIndices(VkCommandBuffer commandBuffer, const VkRenderingInputAttachmentIndexInfo* pInputAttachmentIndexInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetRenderingInputAttachmentIndicesKHR(VkCommandBuffer commandBuffer, const VkRenderingInputAttachmentIndexInfo* pInputAttachmentIndexInfo) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;


  VKAPI_ATTR void VKAPI_CALL vk_common_CmdSetDepthClampRangeEXT(VkCommandBuffer commandBuffer, VkDepthClampModeEXT depthClampMode, const VkDepthClampRangeEXT* pDepthClampRange) VK_ENTRY_WEAK VK_ENTRY_HIDDEN;



#ifdef __cplusplus
}
#endif

#endif /* VK_COMMON_ENTRYPOINTS_H */
