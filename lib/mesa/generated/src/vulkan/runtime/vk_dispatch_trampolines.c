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
/* This file generated from vk_dispatch_trampolines_gen.py, don't edit directly. */

#include "vk_device.h"
#include "vk_dispatch_trampolines.h"
#include "vk_object.h"
#include "vk_physical_device.h"

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
            #ifdef VK_USE_PLATFORM_SCREEN_QNX
static VKAPI_ATTR VkBool32 VKAPI_CALL
vk_tramp_GetPhysicalDeviceScreenPresentationSupportQNX(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, struct _screen_window* window)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    return vk_physical_device->dispatch_table.GetPhysicalDeviceScreenPresentationSupportQNX(physicalDevice, queueFamilyIndex, window);
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
vk_tramp_GetPhysicalDeviceToolProperties(VkPhysicalDevice physicalDevice, uint32_t* pToolCount, VkPhysicalDeviceToolProperties* pToolProperties)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    return vk_physical_device->dispatch_table.GetPhysicalDeviceToolProperties(physicalDevice, pToolCount, pToolProperties);
}
                                                                                                                                                                                                                                                                                                                                                                                                        static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetPhysicalDeviceFragmentShadingRatesKHR(VkPhysicalDevice physicalDevice, uint32_t* pFragmentShadingRateCount, VkPhysicalDeviceFragmentShadingRateKHR* pFragmentShadingRates)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    return vk_physical_device->dispatch_table.GetPhysicalDeviceFragmentShadingRatesKHR(physicalDevice, pFragmentShadingRateCount, pFragmentShadingRates);
}
                                                                                        static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetPhysicalDeviceVideoCapabilitiesKHR(VkPhysicalDevice physicalDevice, const VkVideoProfileInfoKHR* pVideoProfile, VkVideoCapabilitiesKHR* pCapabilities)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    return vk_physical_device->dispatch_table.GetPhysicalDeviceVideoCapabilitiesKHR(physicalDevice, pVideoProfile, pCapabilities);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetPhysicalDeviceVideoFormatPropertiesKHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceVideoFormatInfoKHR* pVideoFormatInfo, uint32_t* pVideoFormatPropertyCount, VkVideoFormatPropertiesKHR* pVideoFormatProperties)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    return vk_physical_device->dispatch_table.GetPhysicalDeviceVideoFormatPropertiesKHR(physicalDevice, pVideoFormatInfo, pVideoFormatPropertyCount, pVideoFormatProperties);
}
                                                                                                                        static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_AcquireDrmDisplayEXT(VkPhysicalDevice physicalDevice, int32_t drmFd, VkDisplayKHR display)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    return vk_physical_device->dispatch_table.AcquireDrmDisplayEXT(physicalDevice, drmFd, display);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetDrmDisplayEXT(VkPhysicalDevice physicalDevice, int32_t drmFd, uint32_t connectorId, VkDisplayKHR* display)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    return vk_physical_device->dispatch_table.GetDrmDisplayEXT(physicalDevice, drmFd, connectorId, display);
}
                                                                                                                                        static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetPhysicalDeviceOpticalFlowImageFormatsNV(VkPhysicalDevice physicalDevice, const VkOpticalFlowImageFormatInfoNV* pOpticalFlowImageFormatInfo, uint32_t* pFormatCount, VkOpticalFlowImageFormatPropertiesNV* pImageFormatProperties)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    return vk_physical_device->dispatch_table.GetPhysicalDeviceOpticalFlowImageFormatsNV(physicalDevice, pOpticalFlowImageFormatInfo, pFormatCount, pImageFormatProperties);
}
                                                            static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetPhysicalDeviceCooperativeMatrixPropertiesKHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkCooperativeMatrixPropertiesKHR* pProperties)
{
    
    VK_FROM_HANDLE(vk_physical_device, vk_physical_device, physicalDevice);
    return vk_physical_device->dispatch_table.GetPhysicalDeviceCooperativeMatrixPropertiesKHR(physicalDevice, pPropertyCount, pProperties);
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
            #ifdef VK_USE_PLATFORM_SCREEN_QNX
    .GetPhysicalDeviceScreenPresentationSupportQNX = vk_tramp_GetPhysicalDeviceScreenPresentationSupportQNX,
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
                                                                .GetPhysicalDeviceToolProperties = vk_tramp_GetPhysicalDeviceToolProperties,
                                                                                                                                                                                                                                                                                                                                                                                                            .GetPhysicalDeviceFragmentShadingRatesKHR = vk_tramp_GetPhysicalDeviceFragmentShadingRatesKHR,
                                                                                            .GetPhysicalDeviceVideoCapabilitiesKHR = vk_tramp_GetPhysicalDeviceVideoCapabilitiesKHR,
    .GetPhysicalDeviceVideoFormatPropertiesKHR = vk_tramp_GetPhysicalDeviceVideoFormatPropertiesKHR,
                                                                                                                            .AcquireDrmDisplayEXT = vk_tramp_AcquireDrmDisplayEXT,
    .GetDrmDisplayEXT = vk_tramp_GetDrmDisplayEXT,
                                                                                                                                            .GetPhysicalDeviceOpticalFlowImageFormatsNV = vk_tramp_GetPhysicalDeviceOpticalFlowImageFormatsNV,
                                                                .GetPhysicalDeviceCooperativeMatrixPropertiesKHR = vk_tramp_GetPhysicalDeviceCooperativeMatrixPropertiesKHR,
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
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI(VkDevice device, VkRenderPass renderpass, VkExtent2D* pMaxWorkgroupSize)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI(device, renderpass, pMaxWorkgroupSize);
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
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_GetRenderingAreaGranularityKHR(VkDevice device, const VkRenderingAreaInfoKHR* pRenderingAreaInfo, VkExtent2D* pGranularity)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.GetRenderingAreaGranularityKHR(device, pRenderingAreaInfo, pGranularity);
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
vk_tramp_CmdSetAttachmentFeedbackLoopEnableEXT(VkCommandBuffer commandBuffer, VkImageAspectFlags aspectMask)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetAttachmentFeedbackLoopEnableEXT(commandBuffer, aspectMask);
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
vk_tramp_CmdDrawMultiEXT(VkCommandBuffer commandBuffer, uint32_t drawCount, const VkMultiDrawInfoEXT* pVertexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdDrawMultiEXT(commandBuffer, drawCount, pVertexInfo, instanceCount, firstInstance, stride);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdDrawMultiIndexedEXT(VkCommandBuffer commandBuffer, uint32_t drawCount, const VkMultiDrawIndexedInfoEXT* pIndexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride, const int32_t* pVertexOffset)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdDrawMultiIndexedEXT(commandBuffer, drawCount, pIndexInfo, instanceCount, firstInstance, stride, pVertexOffset);
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
vk_tramp_CmdSubpassShadingHUAWEI(VkCommandBuffer commandBuffer)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSubpassShadingHUAWEI(commandBuffer);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdDrawClusterHUAWEI(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdDrawClusterHUAWEI(commandBuffer, groupCountX, groupCountY, groupCountZ);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdDrawClusterIndirectHUAWEI(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdDrawClusterIndirectHUAWEI(commandBuffer, buffer, offset);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdUpdatePipelineIndirectBufferNV(VkCommandBuffer commandBuffer, VkPipelineBindPoint           pipelineBindPoint, VkPipeline                    pipeline)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdUpdatePipelineIndirectBufferNV(commandBuffer, pipelineBindPoint, pipeline);
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
vk_tramp_CmdCopyMemoryIndirectNV(VkCommandBuffer commandBuffer, VkDeviceAddress copyBufferAddress, uint32_t copyCount, uint32_t stride)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdCopyMemoryIndirectNV(commandBuffer, copyBufferAddress, copyCount, stride);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdCopyMemoryToImageIndirectNV(VkCommandBuffer commandBuffer, VkDeviceAddress copyBufferAddress, uint32_t copyCount, uint32_t stride, VkImage dstImage, VkImageLayout dstImageLayout, const VkImageSubresourceLayers* pImageSubresources)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdCopyMemoryToImageIndirectNV(commandBuffer, copyBufferAddress, copyCount, stride, dstImage, dstImageLayout, pImageSubresources);
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
#ifdef VK_USE_PLATFORM_FUCHSIA
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetMemoryZirconHandleFUCHSIA(VkDevice device, const VkMemoryGetZirconHandleInfoFUCHSIA* pGetZirconHandleInfo, zx_handle_t* pZirconHandle)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetMemoryZirconHandleFUCHSIA(device, pGetZirconHandleInfo, pZirconHandle);
}
#endif
#ifdef VK_USE_PLATFORM_FUCHSIA
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetMemoryZirconHandlePropertiesFUCHSIA(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, zx_handle_t zirconHandle, VkMemoryZirconHandlePropertiesFUCHSIA* pMemoryZirconHandleProperties)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetMemoryZirconHandlePropertiesFUCHSIA(device, handleType, zirconHandle, pMemoryZirconHandleProperties);
}
#endif
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetMemoryRemoteAddressNV(VkDevice device, const VkMemoryGetRemoteAddressInfoNV* pMemoryGetRemoteAddressInfo, VkRemoteAddressNV* pAddress)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetMemoryRemoteAddressNV(device, pMemoryGetRemoteAddressInfo, pAddress);
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
#ifdef VK_USE_PLATFORM_FUCHSIA
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetSemaphoreZirconHandleFUCHSIA(VkDevice device, const VkSemaphoreGetZirconHandleInfoFUCHSIA* pGetZirconHandleInfo, zx_handle_t* pZirconHandle)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetSemaphoreZirconHandleFUCHSIA(device, pGetZirconHandleInfo, pZirconHandle);
}
#endif
#ifdef VK_USE_PLATFORM_FUCHSIA
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_ImportSemaphoreZirconHandleFUCHSIA(VkDevice device, const VkImportSemaphoreZirconHandleInfoFUCHSIA* pImportSemaphoreZirconHandleInfo)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.ImportSemaphoreZirconHandleFUCHSIA(device, pImportSemaphoreZirconHandleInfo);
}
#endif
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
vk_tramp_CmdSetDiscardRectangleEnableEXT(VkCommandBuffer commandBuffer, VkBool32 discardRectangleEnable)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetDiscardRectangleEnableEXT(commandBuffer, discardRectangleEnable);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetDiscardRectangleModeEXT(VkCommandBuffer commandBuffer, VkDiscardRectangleModeEXT discardRectangleMode)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetDiscardRectangleModeEXT(commandBuffer, discardRectangleMode);
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
    static VKAPI_ATTR void VKAPI_CALL
vk_tramp_GetDeviceBufferMemoryRequirements(VkDevice device, const VkDeviceBufferMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.GetDeviceBufferMemoryRequirements(device, pInfo, pMemoryRequirements);
}
    static VKAPI_ATTR void VKAPI_CALL
vk_tramp_GetDeviceImageMemoryRequirements(VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.GetDeviceImageMemoryRequirements(device, pInfo, pMemoryRequirements);
}
    static VKAPI_ATTR void VKAPI_CALL
vk_tramp_GetDeviceImageSparseMemoryRequirements(VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.GetDeviceImageSparseMemoryRequirements(device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
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
vk_tramp_CmdSetExclusiveScissorEnableNV(VkCommandBuffer commandBuffer, uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, const VkBool32* pExclusiveScissorEnables)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetExclusiveScissorEnableNV(commandBuffer, firstExclusiveScissor, exclusiveScissorCount, pExclusiveScissorEnables);
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
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdDrawMeshTasksEXT(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdDrawMeshTasksEXT(commandBuffer, groupCountX, groupCountY, groupCountZ);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdDrawMeshTasksIndirectEXT(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdDrawMeshTasksIndirectEXT(commandBuffer, buffer, offset, drawCount, stride);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdDrawMeshTasksIndirectCountEXT(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdDrawMeshTasksIndirectCountEXT(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
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
vk_tramp_CmdBindInvocationMaskHUAWEI(VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdBindInvocationMaskHUAWEI(commandBuffer, imageView, imageLayout);
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
vk_tramp_CmdTraceRaysIndirect2KHR(VkCommandBuffer commandBuffer, VkDeviceAddress indirectDeviceAddress)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdTraceRaysIndirect2KHR(commandBuffer, indirectDeviceAddress);
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
vk_tramp_GetPipelineIndirectMemoryRequirementsNV(VkDevice device, const VkComputePipelineCreateInfo* pCreateInfo, VkMemoryRequirements2* pMemoryRequirements)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.GetPipelineIndirectMemoryRequirementsNV(device, pCreateInfo, pMemoryRequirements);
}
static VKAPI_ATTR VkDeviceAddress VKAPI_CALL
vk_tramp_GetPipelineIndirectDeviceAddressNV(VkDevice device, const VkPipelineIndirectDeviceAddressInfoNV* pInfo)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetPipelineIndirectDeviceAddressNV(device, pInfo);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetCullMode(VkCommandBuffer commandBuffer, VkCullModeFlags cullMode)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetCullMode(commandBuffer, cullMode);
}
    static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetFrontFace(VkCommandBuffer commandBuffer, VkFrontFace frontFace)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetFrontFace(commandBuffer, frontFace);
}
    static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetPrimitiveTopology(VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetPrimitiveTopology(commandBuffer, primitiveTopology);
}
    static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetViewportWithCount(VkCommandBuffer commandBuffer, uint32_t viewportCount, const VkViewport* pViewports)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetViewportWithCount(commandBuffer, viewportCount, pViewports);
}
    static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetScissorWithCount(VkCommandBuffer commandBuffer, uint32_t scissorCount, const VkRect2D* pScissors)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetScissorWithCount(commandBuffer, scissorCount, pScissors);
}
    static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdBindIndexBuffer2KHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkDeviceSize size, VkIndexType indexType)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdBindIndexBuffer2KHR(commandBuffer, buffer, offset, size, indexType);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdBindVertexBuffers2(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes, const VkDeviceSize* pStrides)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdBindVertexBuffers2(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes, pStrides);
}
    static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetDepthTestEnable(VkCommandBuffer commandBuffer, VkBool32 depthTestEnable)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetDepthTestEnable(commandBuffer, depthTestEnable);
}
    static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetDepthWriteEnable(VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetDepthWriteEnable(commandBuffer, depthWriteEnable);
}
    static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetDepthCompareOp(VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetDepthCompareOp(commandBuffer, depthCompareOp);
}
    static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetDepthBoundsTestEnable(VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetDepthBoundsTestEnable(commandBuffer, depthBoundsTestEnable);
}
    static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetStencilTestEnable(VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetStencilTestEnable(commandBuffer, stencilTestEnable);
}
    static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetStencilOp(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetStencilOp(commandBuffer, faceMask, failOp, passOp, depthFailOp, compareOp);
}
    static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetPatchControlPointsEXT(VkCommandBuffer commandBuffer, uint32_t patchControlPoints)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetPatchControlPointsEXT(commandBuffer, patchControlPoints);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetRasterizerDiscardEnable(VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetRasterizerDiscardEnable(commandBuffer, rasterizerDiscardEnable);
}
    static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetDepthBiasEnable(VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetDepthBiasEnable(commandBuffer, depthBiasEnable);
}
    static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetLogicOpEXT(VkCommandBuffer commandBuffer, VkLogicOp logicOp)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetLogicOpEXT(commandBuffer, logicOp);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetPrimitiveRestartEnable(VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetPrimitiveRestartEnable(commandBuffer, primitiveRestartEnable);
}
    static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetTessellationDomainOriginEXT(VkCommandBuffer commandBuffer, VkTessellationDomainOrigin domainOrigin)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetTessellationDomainOriginEXT(commandBuffer, domainOrigin);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetDepthClampEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthClampEnable)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetDepthClampEnableEXT(commandBuffer, depthClampEnable);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetPolygonModeEXT(VkCommandBuffer commandBuffer, VkPolygonMode polygonMode)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetPolygonModeEXT(commandBuffer, polygonMode);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetRasterizationSamplesEXT(VkCommandBuffer commandBuffer, VkSampleCountFlagBits  rasterizationSamples)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetRasterizationSamplesEXT(commandBuffer, rasterizationSamples);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetSampleMaskEXT(VkCommandBuffer commandBuffer, VkSampleCountFlagBits  samples, const VkSampleMask*    pSampleMask)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetSampleMaskEXT(commandBuffer, samples, pSampleMask);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetAlphaToCoverageEnableEXT(VkCommandBuffer commandBuffer, VkBool32 alphaToCoverageEnable)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetAlphaToCoverageEnableEXT(commandBuffer, alphaToCoverageEnable);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetAlphaToOneEnableEXT(VkCommandBuffer commandBuffer, VkBool32 alphaToOneEnable)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetAlphaToOneEnableEXT(commandBuffer, alphaToOneEnable);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetLogicOpEnableEXT(VkCommandBuffer commandBuffer, VkBool32 logicOpEnable)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetLogicOpEnableEXT(commandBuffer, logicOpEnable);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetColorBlendEnableEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkBool32* pColorBlendEnables)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetColorBlendEnableEXT(commandBuffer, firstAttachment, attachmentCount, pColorBlendEnables);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetColorBlendEquationEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorBlendEquationEXT* pColorBlendEquations)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetColorBlendEquationEXT(commandBuffer, firstAttachment, attachmentCount, pColorBlendEquations);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetColorWriteMaskEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorComponentFlags* pColorWriteMasks)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetColorWriteMaskEXT(commandBuffer, firstAttachment, attachmentCount, pColorWriteMasks);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetRasterizationStreamEXT(VkCommandBuffer commandBuffer, uint32_t rasterizationStream)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetRasterizationStreamEXT(commandBuffer, rasterizationStream);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetConservativeRasterizationModeEXT(VkCommandBuffer commandBuffer, VkConservativeRasterizationModeEXT conservativeRasterizationMode)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetConservativeRasterizationModeEXT(commandBuffer, conservativeRasterizationMode);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetExtraPrimitiveOverestimationSizeEXT(VkCommandBuffer commandBuffer, float extraPrimitiveOverestimationSize)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetExtraPrimitiveOverestimationSizeEXT(commandBuffer, extraPrimitiveOverestimationSize);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetDepthClipEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthClipEnable)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetDepthClipEnableEXT(commandBuffer, depthClipEnable);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetSampleLocationsEnableEXT(VkCommandBuffer commandBuffer, VkBool32 sampleLocationsEnable)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetSampleLocationsEnableEXT(commandBuffer, sampleLocationsEnable);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetColorBlendAdvancedEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorBlendAdvancedEXT* pColorBlendAdvanced)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetColorBlendAdvancedEXT(commandBuffer, firstAttachment, attachmentCount, pColorBlendAdvanced);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetProvokingVertexModeEXT(VkCommandBuffer commandBuffer, VkProvokingVertexModeEXT provokingVertexMode)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetProvokingVertexModeEXT(commandBuffer, provokingVertexMode);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetLineRasterizationModeEXT(VkCommandBuffer commandBuffer, VkLineRasterizationModeEXT lineRasterizationMode)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetLineRasterizationModeEXT(commandBuffer, lineRasterizationMode);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetLineStippleEnableEXT(VkCommandBuffer commandBuffer, VkBool32 stippledLineEnable)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetLineStippleEnableEXT(commandBuffer, stippledLineEnable);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetDepthClipNegativeOneToOneEXT(VkCommandBuffer commandBuffer, VkBool32 negativeOneToOne)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetDepthClipNegativeOneToOneEXT(commandBuffer, negativeOneToOne);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetViewportWScalingEnableNV(VkCommandBuffer commandBuffer, VkBool32 viewportWScalingEnable)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetViewportWScalingEnableNV(commandBuffer, viewportWScalingEnable);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetViewportSwizzleNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewportSwizzleNV* pViewportSwizzles)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetViewportSwizzleNV(commandBuffer, firstViewport, viewportCount, pViewportSwizzles);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetCoverageToColorEnableNV(VkCommandBuffer commandBuffer, VkBool32 coverageToColorEnable)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetCoverageToColorEnableNV(commandBuffer, coverageToColorEnable);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetCoverageToColorLocationNV(VkCommandBuffer commandBuffer, uint32_t coverageToColorLocation)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetCoverageToColorLocationNV(commandBuffer, coverageToColorLocation);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetCoverageModulationModeNV(VkCommandBuffer commandBuffer, VkCoverageModulationModeNV coverageModulationMode)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetCoverageModulationModeNV(commandBuffer, coverageModulationMode);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetCoverageModulationTableEnableNV(VkCommandBuffer commandBuffer, VkBool32 coverageModulationTableEnable)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetCoverageModulationTableEnableNV(commandBuffer, coverageModulationTableEnable);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetCoverageModulationTableNV(VkCommandBuffer commandBuffer, uint32_t coverageModulationTableCount, const float* pCoverageModulationTable)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetCoverageModulationTableNV(commandBuffer, coverageModulationTableCount, pCoverageModulationTable);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetShadingRateImageEnableNV(VkCommandBuffer commandBuffer, VkBool32 shadingRateImageEnable)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetShadingRateImageEnableNV(commandBuffer, shadingRateImageEnable);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetCoverageReductionModeNV(VkCommandBuffer commandBuffer, VkCoverageReductionModeNV coverageReductionMode)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetCoverageReductionModeNV(commandBuffer, coverageReductionMode);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetRepresentativeFragmentTestEnableNV(VkCommandBuffer commandBuffer, VkBool32 representativeFragmentTestEnable)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetRepresentativeFragmentTestEnableNV(commandBuffer, representativeFragmentTestEnable);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_CreatePrivateDataSlot(VkDevice device, const VkPrivateDataSlotCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPrivateDataSlot* pPrivateDataSlot)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.CreatePrivateDataSlot(device, pCreateInfo, pAllocator, pPrivateDataSlot);
}
    static VKAPI_ATTR void VKAPI_CALL
vk_tramp_DestroyPrivateDataSlot(VkDevice device, VkPrivateDataSlot privateDataSlot, const VkAllocationCallbacks* pAllocator)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.DestroyPrivateDataSlot(device, privateDataSlot, pAllocator);
}
    static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_SetPrivateData(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t data)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.SetPrivateData(device, objectType, objectHandle, privateDataSlot, data);
}
    static VKAPI_ATTR void VKAPI_CALL
vk_tramp_GetPrivateData(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t* pData)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.GetPrivateData(device, objectType, objectHandle, privateDataSlot, pData);
}
    static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdCopyBuffer2(VkCommandBuffer commandBuffer, const VkCopyBufferInfo2* pCopyBufferInfo)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdCopyBuffer2(commandBuffer, pCopyBufferInfo);
}
    static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdCopyImage2(VkCommandBuffer commandBuffer, const VkCopyImageInfo2* pCopyImageInfo)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdCopyImage2(commandBuffer, pCopyImageInfo);
}
    static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdBlitImage2(VkCommandBuffer commandBuffer, const VkBlitImageInfo2* pBlitImageInfo)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdBlitImage2(commandBuffer, pBlitImageInfo);
}
    static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdCopyBufferToImage2(VkCommandBuffer commandBuffer, const VkCopyBufferToImageInfo2* pCopyBufferToImageInfo)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdCopyBufferToImage2(commandBuffer, pCopyBufferToImageInfo);
}
    static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdCopyImageToBuffer2(VkCommandBuffer commandBuffer, const VkCopyImageToBufferInfo2* pCopyImageToBufferInfo)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdCopyImageToBuffer2(commandBuffer, pCopyImageToBufferInfo);
}
    static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdResolveImage2(VkCommandBuffer commandBuffer, const VkResolveImageInfo2* pResolveImageInfo)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdResolveImage2(commandBuffer, pResolveImageInfo);
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
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetVertexInputEXT(VkCommandBuffer commandBuffer, uint32_t vertexBindingDescriptionCount, const VkVertexInputBindingDescription2EXT* pVertexBindingDescriptions, uint32_t vertexAttributeDescriptionCount, const VkVertexInputAttributeDescription2EXT* pVertexAttributeDescriptions)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetVertexInputEXT(commandBuffer, vertexBindingDescriptionCount, pVertexBindingDescriptions, vertexAttributeDescriptionCount, pVertexAttributeDescriptions);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetColorWriteEnableEXT(VkCommandBuffer       commandBuffer, uint32_t                                attachmentCount, const VkBool32*   pColorWriteEnables)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetColorWriteEnableEXT(commandBuffer, attachmentCount, pColorWriteEnables);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetEvent2(VkCommandBuffer                   commandBuffer, VkEvent                                             event, const VkDependencyInfo*                             pDependencyInfo)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetEvent2(commandBuffer, event, pDependencyInfo);
}
    static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdResetEvent2(VkCommandBuffer                   commandBuffer, VkEvent                                             event, VkPipelineStageFlags2               stageMask)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdResetEvent2(commandBuffer, event, stageMask);
}
    static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdWaitEvents2(VkCommandBuffer                   commandBuffer, uint32_t                                            eventCount, const VkEvent*                     pEvents, const VkDependencyInfo*            pDependencyInfos)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdWaitEvents2(commandBuffer, eventCount, pEvents, pDependencyInfos);
}
    static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdPipelineBarrier2(VkCommandBuffer                   commandBuffer, const VkDependencyInfo*                             pDependencyInfo)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdPipelineBarrier2(commandBuffer, pDependencyInfo);
}
    static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_QueueSubmit2(VkQueue                           queue, uint32_t                            submitCount, const VkSubmitInfo2*              pSubmits, VkFence           fence)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)queue;
    return vk_object->device->dispatch_table.QueueSubmit2(queue, submitCount, pSubmits, fence);
}
    static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdWriteTimestamp2(VkCommandBuffer                   commandBuffer, VkPipelineStageFlags2               stage, VkQueryPool                                         queryPool, uint32_t                                            query)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdWriteTimestamp2(commandBuffer, stage, queryPool, query);
}
    static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdWriteBufferMarker2AMD(VkCommandBuffer                   commandBuffer, VkPipelineStageFlags2               stage, VkBuffer                                            dstBuffer, VkDeviceSize                                        dstOffset, uint32_t                                            marker)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdWriteBufferMarker2AMD(commandBuffer, stage, dstBuffer, dstOffset, marker);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_GetQueueCheckpointData2NV(VkQueue queue, uint32_t* pCheckpointDataCount, VkCheckpointData2NV* pCheckpointData)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)queue;
    vk_object->device->dispatch_table.GetQueueCheckpointData2NV(queue, pCheckpointDataCount, pCheckpointData);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_CopyMemoryToImageEXT(VkDevice device, const VkCopyMemoryToImageInfoEXT* pCopyMemoryToImageInfo)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.CopyMemoryToImageEXT(device, pCopyMemoryToImageInfo);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_CopyImageToMemoryEXT(VkDevice device, const VkCopyImageToMemoryInfoEXT* pCopyImageToMemoryInfo)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.CopyImageToMemoryEXT(device, pCopyImageToMemoryInfo);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_CopyImageToImageEXT(VkDevice device, const VkCopyImageToImageInfoEXT* pCopyImageToImageInfo)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.CopyImageToImageEXT(device, pCopyImageToImageInfo);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_TransitionImageLayoutEXT(VkDevice device, uint32_t transitionCount, const VkHostImageLayoutTransitionInfoEXT* pTransitions)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.TransitionImageLayoutEXT(device, transitionCount, pTransitions);
}
        static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_CreateVideoSessionKHR(VkDevice device, const VkVideoSessionCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkVideoSessionKHR* pVideoSession)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.CreateVideoSessionKHR(device, pCreateInfo, pAllocator, pVideoSession);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_DestroyVideoSessionKHR(VkDevice device, VkVideoSessionKHR videoSession, const VkAllocationCallbacks* pAllocator)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.DestroyVideoSessionKHR(device, videoSession, pAllocator);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_CreateVideoSessionParametersKHR(VkDevice device, const VkVideoSessionParametersCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkVideoSessionParametersKHR* pVideoSessionParameters)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.CreateVideoSessionParametersKHR(device, pCreateInfo, pAllocator, pVideoSessionParameters);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_UpdateVideoSessionParametersKHR(VkDevice device, VkVideoSessionParametersKHR videoSessionParameters, const VkVideoSessionParametersUpdateInfoKHR* pUpdateInfo)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.UpdateVideoSessionParametersKHR(device, videoSessionParameters, pUpdateInfo);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_DestroyVideoSessionParametersKHR(VkDevice device, VkVideoSessionParametersKHR videoSessionParameters, const VkAllocationCallbacks* pAllocator)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.DestroyVideoSessionParametersKHR(device, videoSessionParameters, pAllocator);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetVideoSessionMemoryRequirementsKHR(VkDevice device, VkVideoSessionKHR videoSession, uint32_t* pMemoryRequirementsCount, VkVideoSessionMemoryRequirementsKHR* pMemoryRequirements)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetVideoSessionMemoryRequirementsKHR(device, videoSession, pMemoryRequirementsCount, pMemoryRequirements);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_BindVideoSessionMemoryKHR(VkDevice device, VkVideoSessionKHR videoSession, uint32_t bindSessionMemoryInfoCount, const VkBindVideoSessionMemoryInfoKHR* pBindSessionMemoryInfos)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.BindVideoSessionMemoryKHR(device, videoSession, bindSessionMemoryInfoCount, pBindSessionMemoryInfos);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdDecodeVideoKHR(VkCommandBuffer commandBuffer, const VkVideoDecodeInfoKHR* pDecodeInfo)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdDecodeVideoKHR(commandBuffer, pDecodeInfo);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdBeginVideoCodingKHR(VkCommandBuffer commandBuffer, const VkVideoBeginCodingInfoKHR* pBeginInfo)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdBeginVideoCodingKHR(commandBuffer, pBeginInfo);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdControlVideoCodingKHR(VkCommandBuffer commandBuffer, const VkVideoCodingControlInfoKHR* pCodingControlInfo)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdControlVideoCodingKHR(commandBuffer, pCodingControlInfo);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdEndVideoCodingKHR(VkCommandBuffer commandBuffer, const VkVideoEndCodingInfoKHR* pEndCodingInfo)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdEndVideoCodingKHR(commandBuffer, pEndCodingInfo);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdDecompressMemoryNV(VkCommandBuffer commandBuffer, uint32_t decompressRegionCount, const VkDecompressMemoryRegionNV* pDecompressMemoryRegions)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdDecompressMemoryNV(commandBuffer, decompressRegionCount, pDecompressMemoryRegions);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdDecompressMemoryIndirectCountNV(VkCommandBuffer commandBuffer, VkDeviceAddress indirectCommandsAddress, VkDeviceAddress indirectCommandsCountAddress, uint32_t stride)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdDecompressMemoryIndirectCountNV(commandBuffer, indirectCommandsAddress, indirectCommandsCountAddress, stride);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_CreateCuModuleNVX(VkDevice device, const VkCuModuleCreateInfoNVX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCuModuleNVX* pModule)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.CreateCuModuleNVX(device, pCreateInfo, pAllocator, pModule);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_CreateCuFunctionNVX(VkDevice device, const VkCuFunctionCreateInfoNVX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCuFunctionNVX* pFunction)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.CreateCuFunctionNVX(device, pCreateInfo, pAllocator, pFunction);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_DestroyCuModuleNVX(VkDevice device, VkCuModuleNVX module, const VkAllocationCallbacks* pAllocator)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.DestroyCuModuleNVX(device, module, pAllocator);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_DestroyCuFunctionNVX(VkDevice device, VkCuFunctionNVX function, const VkAllocationCallbacks* pAllocator)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.DestroyCuFunctionNVX(device, function, pAllocator);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdCuLaunchKernelNVX(VkCommandBuffer commandBuffer, const VkCuLaunchInfoNVX* pLaunchInfo)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdCuLaunchKernelNVX(commandBuffer, pLaunchInfo);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_GetDescriptorSetLayoutSizeEXT(VkDevice device, VkDescriptorSetLayout layout, VkDeviceSize* pLayoutSizeInBytes)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.GetDescriptorSetLayoutSizeEXT(device, layout, pLayoutSizeInBytes);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_GetDescriptorSetLayoutBindingOffsetEXT(VkDevice device, VkDescriptorSetLayout layout, uint32_t binding, VkDeviceSize* pOffset)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.GetDescriptorSetLayoutBindingOffsetEXT(device, layout, binding, pOffset);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_GetDescriptorEXT(VkDevice device, const VkDescriptorGetInfoEXT* pDescriptorInfo, size_t dataSize, void* pDescriptor)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.GetDescriptorEXT(device, pDescriptorInfo, dataSize, pDescriptor);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdBindDescriptorBuffersEXT(VkCommandBuffer commandBuffer, uint32_t bufferCount, const VkDescriptorBufferBindingInfoEXT* pBindingInfos)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdBindDescriptorBuffersEXT(commandBuffer, bufferCount, pBindingInfos);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetDescriptorBufferOffsetsEXT(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t setCount, const uint32_t* pBufferIndices, const VkDeviceSize* pOffsets)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetDescriptorBufferOffsetsEXT(commandBuffer, pipelineBindPoint, layout, firstSet, setCount, pBufferIndices, pOffsets);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdBindDescriptorBufferEmbeddedSamplersEXT(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdBindDescriptorBufferEmbeddedSamplersEXT(commandBuffer, pipelineBindPoint, layout, set);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetBufferOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkBufferCaptureDescriptorDataInfoEXT* pInfo, void* pData)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetBufferOpaqueCaptureDescriptorDataEXT(device, pInfo, pData);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetImageOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkImageCaptureDescriptorDataInfoEXT* pInfo, void* pData)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetImageOpaqueCaptureDescriptorDataEXT(device, pInfo, pData);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetImageViewOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkImageViewCaptureDescriptorDataInfoEXT* pInfo, void* pData)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetImageViewOpaqueCaptureDescriptorDataEXT(device, pInfo, pData);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetSamplerOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkSamplerCaptureDescriptorDataInfoEXT* pInfo, void* pData)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetSamplerOpaqueCaptureDescriptorDataEXT(device, pInfo, pData);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetAccelerationStructureOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkAccelerationStructureCaptureDescriptorDataInfoEXT* pInfo, void* pData)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetAccelerationStructureOpaqueCaptureDescriptorDataEXT(device, pInfo, pData);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_SetDeviceMemoryPriorityEXT(VkDevice       device, VkDeviceMemory memory, float          priority)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.SetDeviceMemoryPriorityEXT(device, memory, priority);
}
        static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_WaitForPresentKHR(VkDevice device, VkSwapchainKHR swapchain, uint64_t presentId, uint64_t timeout)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.WaitForPresentKHR(device, swapchain, presentId, timeout);
}
#ifdef VK_USE_PLATFORM_FUCHSIA
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_CreateBufferCollectionFUCHSIA(VkDevice device, const VkBufferCollectionCreateInfoFUCHSIA* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferCollectionFUCHSIA* pCollection)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.CreateBufferCollectionFUCHSIA(device, pCreateInfo, pAllocator, pCollection);
}
#endif
#ifdef VK_USE_PLATFORM_FUCHSIA
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_SetBufferCollectionBufferConstraintsFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, const VkBufferConstraintsInfoFUCHSIA* pBufferConstraintsInfo)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.SetBufferCollectionBufferConstraintsFUCHSIA(device, collection, pBufferConstraintsInfo);
}
#endif
#ifdef VK_USE_PLATFORM_FUCHSIA
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_SetBufferCollectionImageConstraintsFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, const VkImageConstraintsInfoFUCHSIA* pImageConstraintsInfo)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.SetBufferCollectionImageConstraintsFUCHSIA(device, collection, pImageConstraintsInfo);
}
#endif
#ifdef VK_USE_PLATFORM_FUCHSIA
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_DestroyBufferCollectionFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, const VkAllocationCallbacks* pAllocator)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.DestroyBufferCollectionFUCHSIA(device, collection, pAllocator);
}
#endif
#ifdef VK_USE_PLATFORM_FUCHSIA
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetBufferCollectionPropertiesFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, VkBufferCollectionPropertiesFUCHSIA* pProperties)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetBufferCollectionPropertiesFUCHSIA(device, collection, pProperties);
}
#endif
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdBeginRendering(VkCommandBuffer                   commandBuffer, const VkRenderingInfo*                              pRenderingInfo)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdBeginRendering(commandBuffer, pRenderingInfo);
}
    static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdEndRendering(VkCommandBuffer                   commandBuffer)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdEndRendering(commandBuffer);
}
    static VKAPI_ATTR void VKAPI_CALL
vk_tramp_GetDescriptorSetLayoutHostMappingInfoVALVE(VkDevice device, const VkDescriptorSetBindingReferenceVALVE* pBindingReference, VkDescriptorSetLayoutHostMappingInfoVALVE* pHostMapping)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.GetDescriptorSetLayoutHostMappingInfoVALVE(device, pBindingReference, pHostMapping);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_GetDescriptorSetHostMappingVALVE(VkDevice device, VkDescriptorSet descriptorSet, void** ppData)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.GetDescriptorSetHostMappingVALVE(device, descriptorSet, ppData);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_CreateMicromapEXT(VkDevice                                           device, const VkMicromapCreateInfoEXT*        pCreateInfo, const VkAllocationCallbacks*       pAllocator, VkMicromapEXT*                        pMicromap)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.CreateMicromapEXT(device, pCreateInfo, pAllocator, pMicromap);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdBuildMicromapsEXT(VkCommandBuffer                                    commandBuffer, uint32_t infoCount, const VkMicromapBuildInfoEXT* pInfos)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdBuildMicromapsEXT(commandBuffer, infoCount, pInfos);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_BuildMicromapsEXT(VkDevice                                           device, VkDeferredOperationKHR deferredOperation, uint32_t infoCount, const VkMicromapBuildInfoEXT* pInfos)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.BuildMicromapsEXT(device, deferredOperation, infoCount, pInfos);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_DestroyMicromapEXT(VkDevice device, VkMicromapEXT micromap, const VkAllocationCallbacks* pAllocator)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.DestroyMicromapEXT(device, micromap, pAllocator);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdCopyMicromapEXT(VkCommandBuffer commandBuffer, const VkCopyMicromapInfoEXT* pInfo)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdCopyMicromapEXT(commandBuffer, pInfo);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_CopyMicromapEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMicromapInfoEXT* pInfo)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.CopyMicromapEXT(device, deferredOperation, pInfo);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdCopyMicromapToMemoryEXT(VkCommandBuffer commandBuffer, const VkCopyMicromapToMemoryInfoEXT* pInfo)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdCopyMicromapToMemoryEXT(commandBuffer, pInfo);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_CopyMicromapToMemoryEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMicromapToMemoryInfoEXT* pInfo)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.CopyMicromapToMemoryEXT(device, deferredOperation, pInfo);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdCopyMemoryToMicromapEXT(VkCommandBuffer commandBuffer, const VkCopyMemoryToMicromapInfoEXT* pInfo)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdCopyMemoryToMicromapEXT(commandBuffer, pInfo);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_CopyMemoryToMicromapEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMemoryToMicromapInfoEXT* pInfo)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.CopyMemoryToMicromapEXT(device, deferredOperation, pInfo);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdWriteMicromapsPropertiesEXT(VkCommandBuffer commandBuffer, uint32_t micromapCount, const VkMicromapEXT* pMicromaps, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdWriteMicromapsPropertiesEXT(commandBuffer, micromapCount, pMicromaps, queryType, queryPool, firstQuery);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_WriteMicromapsPropertiesEXT(VkDevice device, uint32_t micromapCount, const VkMicromapEXT* pMicromaps, VkQueryType  queryType, size_t       dataSize, void* pData, size_t stride)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.WriteMicromapsPropertiesEXT(device, micromapCount, pMicromaps, queryType, dataSize, pData, stride);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_GetDeviceMicromapCompatibilityEXT(VkDevice device, const VkMicromapVersionInfoEXT* pVersionInfo, VkAccelerationStructureCompatibilityKHR* pCompatibility)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.GetDeviceMicromapCompatibilityEXT(device, pVersionInfo, pCompatibility);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_GetMicromapBuildSizesEXT(VkDevice                                            device, VkAccelerationStructureBuildTypeKHR                 buildType, const VkMicromapBuildInfoEXT*  pBuildInfo, VkMicromapBuildSizesInfoEXT*           pSizeInfo)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.GetMicromapBuildSizesEXT(device, buildType, pBuildInfo, pSizeInfo);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_GetShaderModuleIdentifierEXT(VkDevice device, VkShaderModule shaderModule, VkShaderModuleIdentifierEXT* pIdentifier)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.GetShaderModuleIdentifierEXT(device, shaderModule, pIdentifier);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_GetShaderModuleCreateInfoIdentifierEXT(VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, VkShaderModuleIdentifierEXT* pIdentifier)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.GetShaderModuleCreateInfoIdentifierEXT(device, pCreateInfo, pIdentifier);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_GetImageSubresourceLayout2KHR(VkDevice device, VkImage image, const VkImageSubresource2KHR* pSubresource, VkSubresourceLayout2KHR* pLayout)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.GetImageSubresourceLayout2KHR(device, image, pSubresource, pLayout);
}
    static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetPipelinePropertiesEXT(VkDevice device, const VkPipelineInfoEXT* pPipelineInfo, VkBaseOutStructure* pPipelineProperties)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetPipelinePropertiesEXT(device, pPipelineInfo, pPipelineProperties);
}
#ifdef VK_USE_PLATFORM_METAL_EXT
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_ExportMetalObjectsEXT(VkDevice device, VkExportMetalObjectsInfoEXT* pMetalObjectsInfo)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.ExportMetalObjectsEXT(device, pMetalObjectsInfo);
}
#endif
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetFramebufferTilePropertiesQCOM(VkDevice device, VkFramebuffer framebuffer, uint32_t* pPropertiesCount, VkTilePropertiesQCOM* pProperties)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetFramebufferTilePropertiesQCOM(device, framebuffer, pPropertiesCount, pProperties);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetDynamicRenderingTilePropertiesQCOM(VkDevice device, const VkRenderingInfo* pRenderingInfo, VkTilePropertiesQCOM* pProperties)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetDynamicRenderingTilePropertiesQCOM(device, pRenderingInfo, pProperties);
}
    static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_CreateOpticalFlowSessionNV(VkDevice device, const VkOpticalFlowSessionCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkOpticalFlowSessionNV* pSession)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.CreateOpticalFlowSessionNV(device, pCreateInfo, pAllocator, pSession);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_DestroyOpticalFlowSessionNV(VkDevice device, VkOpticalFlowSessionNV session, const VkAllocationCallbacks* pAllocator)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.DestroyOpticalFlowSessionNV(device, session, pAllocator);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_BindOpticalFlowSessionImageNV(VkDevice device, VkOpticalFlowSessionNV session, VkOpticalFlowSessionBindingPointNV bindingPoint, VkImageView view, VkImageLayout layout)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.BindOpticalFlowSessionImageNV(device, session, bindingPoint, view, layout);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdOpticalFlowExecuteNV(VkCommandBuffer commandBuffer, VkOpticalFlowSessionNV session, const VkOpticalFlowExecuteInfoNV* pExecuteInfo)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdOpticalFlowExecuteNV(commandBuffer, session, pExecuteInfo);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetDeviceFaultInfoEXT(VkDevice device, VkDeviceFaultCountsEXT* pFaultCounts, VkDeviceFaultInfoEXT* pFaultInfo)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetDeviceFaultInfoEXT(device, pFaultCounts, pFaultInfo);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdSetDepthBias2EXT(VkCommandBuffer commandBuffer, const VkDepthBiasInfoEXT*         pDepthBiasInfo)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdSetDepthBias2EXT(commandBuffer, pDepthBiasInfo);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_ReleaseSwapchainImagesEXT(VkDevice device, const VkReleaseSwapchainImagesInfoEXT* pReleaseInfo)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.ReleaseSwapchainImagesEXT(device, pReleaseInfo);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_GetDeviceImageSubresourceLayoutKHR(VkDevice device, const VkDeviceImageSubresourceInfoKHR* pInfo, VkSubresourceLayout2KHR* pLayout)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.GetDeviceImageSubresourceLayoutKHR(device, pInfo, pLayout);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_MapMemory2KHR(VkDevice device, const VkMemoryMapInfoKHR* pMemoryMapInfo, void** ppData)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.MapMemory2KHR(device, pMemoryMapInfo, ppData);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_UnmapMemory2KHR(VkDevice device, const VkMemoryUnmapInfoKHR* pMemoryUnmapInfo)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.UnmapMemory2KHR(device, pMemoryUnmapInfo);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_CreateShadersEXT(VkDevice device, uint32_t createInfoCount, const VkShaderCreateInfoEXT* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkShaderEXT* pShaders)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.CreateShadersEXT(device, createInfoCount, pCreateInfos, pAllocator, pShaders);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_DestroyShaderEXT(VkDevice device, VkShaderEXT shader, const VkAllocationCallbacks* pAllocator)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.DestroyShaderEXT(device, shader, pAllocator);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetShaderBinaryDataEXT(VkDevice device, VkShaderEXT shader, size_t* pDataSize, void* pData)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetShaderBinaryDataEXT(device, shader, pDataSize, pData);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_CmdBindShadersEXT(VkCommandBuffer commandBuffer, uint32_t stageCount, const VkShaderStageFlagBits* pStages, const VkShaderEXT* pShaders)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)commandBuffer;
    vk_object->device->dispatch_table.CmdBindShadersEXT(commandBuffer, stageCount, pStages, pShaders);
}
#ifdef VK_USE_PLATFORM_SCREEN_QNX
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_GetScreenBufferPropertiesQNX(VkDevice device, const struct _screen_buffer* buffer, VkScreenBufferPropertiesQNX* pProperties)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.GetScreenBufferPropertiesQNX(device, buffer, pProperties);
}
#endif
    static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_SetLatencySleepModeNV(VkDevice device, VkSwapchainKHR swapchain, const VkLatencySleepModeInfoNV* pSleepModeInfo)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.SetLatencySleepModeNV(device, swapchain, pSleepModeInfo);
}
static VKAPI_ATTR VkResult VKAPI_CALL
vk_tramp_LatencySleepNV(VkDevice device, VkSwapchainKHR swapchain, const VkLatencySleepInfoNV* pSleepInfo)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    return vk_device->dispatch_table.LatencySleepNV(device, swapchain, pSleepInfo);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_SetLatencyMarkerNV(VkDevice device, VkSwapchainKHR swapchain, const VkSetLatencyMarkerInfoNV* pLatencyMarkerInfo)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.SetLatencyMarkerNV(device, swapchain, pLatencyMarkerInfo);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_GetLatencyTimingsNV(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pTimingCount, VkGetLatencyMarkerInfoNV* pLatencyMarkerInfo)
{
    VK_FROM_HANDLE(vk_device, vk_device, device);
    vk_device->dispatch_table.GetLatencyTimingsNV(device, swapchain, pTimingCount, pLatencyMarkerInfo);
}
static VKAPI_ATTR void VKAPI_CALL
vk_tramp_QueueNotifyOutOfBandNV(VkQueue queue, const VkOutOfBandQueueTypeInfoNV* pQueueTypeInfo)
{
    struct vk_object_base *vk_object = (struct vk_object_base *)queue;
    vk_object->device->dispatch_table.QueueNotifyOutOfBandNV(queue, pQueueTypeInfo);
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
    .GetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI = vk_tramp_GetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI,
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
    .GetRenderingAreaGranularityKHR = vk_tramp_GetRenderingAreaGranularityKHR,
    .CreateCommandPool = vk_tramp_CreateCommandPool,
    .DestroyCommandPool = vk_tramp_DestroyCommandPool,
    .ResetCommandPool = vk_tramp_ResetCommandPool,
    .AllocateCommandBuffers = vk_tramp_AllocateCommandBuffers,
    .FreeCommandBuffers = vk_tramp_FreeCommandBuffers,
    .BeginCommandBuffer = vk_tramp_BeginCommandBuffer,
    .EndCommandBuffer = vk_tramp_EndCommandBuffer,
    .ResetCommandBuffer = vk_tramp_ResetCommandBuffer,
    .CmdBindPipeline = vk_tramp_CmdBindPipeline,
    .CmdSetAttachmentFeedbackLoopEnableEXT = vk_tramp_CmdSetAttachmentFeedbackLoopEnableEXT,
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
    .CmdDrawMultiEXT = vk_tramp_CmdDrawMultiEXT,
    .CmdDrawMultiIndexedEXT = vk_tramp_CmdDrawMultiIndexedEXT,
    .CmdDrawIndirect = vk_tramp_CmdDrawIndirect,
    .CmdDrawIndexedIndirect = vk_tramp_CmdDrawIndexedIndirect,
    .CmdDispatch = vk_tramp_CmdDispatch,
    .CmdDispatchIndirect = vk_tramp_CmdDispatchIndirect,
    .CmdSubpassShadingHUAWEI = vk_tramp_CmdSubpassShadingHUAWEI,
    .CmdDrawClusterHUAWEI = vk_tramp_CmdDrawClusterHUAWEI,
    .CmdDrawClusterIndirectHUAWEI = vk_tramp_CmdDrawClusterIndirectHUAWEI,
    .CmdUpdatePipelineIndirectBufferNV = vk_tramp_CmdUpdatePipelineIndirectBufferNV,
    .CmdCopyBuffer = vk_tramp_CmdCopyBuffer,
    .CmdCopyImage = vk_tramp_CmdCopyImage,
    .CmdBlitImage = vk_tramp_CmdBlitImage,
    .CmdCopyBufferToImage = vk_tramp_CmdCopyBufferToImage,
    .CmdCopyImageToBuffer = vk_tramp_CmdCopyImageToBuffer,
    .CmdCopyMemoryIndirectNV = vk_tramp_CmdCopyMemoryIndirectNV,
    .CmdCopyMemoryToImageIndirectNV = vk_tramp_CmdCopyMemoryToImageIndirectNV,
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
#ifdef VK_USE_PLATFORM_FUCHSIA
    .GetMemoryZirconHandleFUCHSIA = vk_tramp_GetMemoryZirconHandleFUCHSIA,
#endif
#ifdef VK_USE_PLATFORM_FUCHSIA
    .GetMemoryZirconHandlePropertiesFUCHSIA = vk_tramp_GetMemoryZirconHandlePropertiesFUCHSIA,
#endif
    .GetMemoryRemoteAddressNV = vk_tramp_GetMemoryRemoteAddressNV,
        #ifdef VK_USE_PLATFORM_WIN32_KHR
    .GetSemaphoreWin32HandleKHR = vk_tramp_GetSemaphoreWin32HandleKHR,
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
    .ImportSemaphoreWin32HandleKHR = vk_tramp_ImportSemaphoreWin32HandleKHR,
#endif
    .GetSemaphoreFdKHR = vk_tramp_GetSemaphoreFdKHR,
    .ImportSemaphoreFdKHR = vk_tramp_ImportSemaphoreFdKHR,
#ifdef VK_USE_PLATFORM_FUCHSIA
    .GetSemaphoreZirconHandleFUCHSIA = vk_tramp_GetSemaphoreZirconHandleFUCHSIA,
#endif
#ifdef VK_USE_PLATFORM_FUCHSIA
    .ImportSemaphoreZirconHandleFUCHSIA = vk_tramp_ImportSemaphoreZirconHandleFUCHSIA,
#endif
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
    .CmdSetDiscardRectangleEnableEXT = vk_tramp_CmdSetDiscardRectangleEnableEXT,
    .CmdSetDiscardRectangleModeEXT = vk_tramp_CmdSetDiscardRectangleModeEXT,
    .CmdSetSampleLocationsEXT = vk_tramp_CmdSetSampleLocationsEXT,
                                .GetBufferMemoryRequirements2 = vk_tramp_GetBufferMemoryRequirements2,
        .GetImageMemoryRequirements2 = vk_tramp_GetImageMemoryRequirements2,
        .GetImageSparseMemoryRequirements2 = vk_tramp_GetImageSparseMemoryRequirements2,
        .GetDeviceBufferMemoryRequirements = vk_tramp_GetDeviceBufferMemoryRequirements,
        .GetDeviceImageMemoryRequirements = vk_tramp_GetDeviceImageMemoryRequirements,
        .GetDeviceImageSparseMemoryRequirements = vk_tramp_GetDeviceImageSparseMemoryRequirements,
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
    .CmdSetExclusiveScissorEnableNV = vk_tramp_CmdSetExclusiveScissorEnableNV,
    .CmdBindShadingRateImageNV = vk_tramp_CmdBindShadingRateImageNV,
    .CmdSetViewportShadingRatePaletteNV = vk_tramp_CmdSetViewportShadingRatePaletteNV,
    .CmdSetCoarseSampleOrderNV = vk_tramp_CmdSetCoarseSampleOrderNV,
    .CmdDrawMeshTasksNV = vk_tramp_CmdDrawMeshTasksNV,
    .CmdDrawMeshTasksIndirectNV = vk_tramp_CmdDrawMeshTasksIndirectNV,
    .CmdDrawMeshTasksIndirectCountNV = vk_tramp_CmdDrawMeshTasksIndirectCountNV,
    .CmdDrawMeshTasksEXT = vk_tramp_CmdDrawMeshTasksEXT,
    .CmdDrawMeshTasksIndirectEXT = vk_tramp_CmdDrawMeshTasksIndirectEXT,
    .CmdDrawMeshTasksIndirectCountEXT = vk_tramp_CmdDrawMeshTasksIndirectCountEXT,
    .CompileDeferredNV = vk_tramp_CompileDeferredNV,
    .CreateAccelerationStructureNV = vk_tramp_CreateAccelerationStructureNV,
    .CmdBindInvocationMaskHUAWEI = vk_tramp_CmdBindInvocationMaskHUAWEI,
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
    .CmdTraceRaysIndirect2KHR = vk_tramp_CmdTraceRaysIndirect2KHR,
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
    .GetPipelineIndirectMemoryRequirementsNV = vk_tramp_GetPipelineIndirectMemoryRequirementsNV,
    .GetPipelineIndirectDeviceAddressNV = vk_tramp_GetPipelineIndirectDeviceAddressNV,
    .CmdSetCullMode = vk_tramp_CmdSetCullMode,
        .CmdSetFrontFace = vk_tramp_CmdSetFrontFace,
        .CmdSetPrimitiveTopology = vk_tramp_CmdSetPrimitiveTopology,
        .CmdSetViewportWithCount = vk_tramp_CmdSetViewportWithCount,
        .CmdSetScissorWithCount = vk_tramp_CmdSetScissorWithCount,
        .CmdBindIndexBuffer2KHR = vk_tramp_CmdBindIndexBuffer2KHR,
    .CmdBindVertexBuffers2 = vk_tramp_CmdBindVertexBuffers2,
        .CmdSetDepthTestEnable = vk_tramp_CmdSetDepthTestEnable,
        .CmdSetDepthWriteEnable = vk_tramp_CmdSetDepthWriteEnable,
        .CmdSetDepthCompareOp = vk_tramp_CmdSetDepthCompareOp,
        .CmdSetDepthBoundsTestEnable = vk_tramp_CmdSetDepthBoundsTestEnable,
        .CmdSetStencilTestEnable = vk_tramp_CmdSetStencilTestEnable,
        .CmdSetStencilOp = vk_tramp_CmdSetStencilOp,
        .CmdSetPatchControlPointsEXT = vk_tramp_CmdSetPatchControlPointsEXT,
    .CmdSetRasterizerDiscardEnable = vk_tramp_CmdSetRasterizerDiscardEnable,
        .CmdSetDepthBiasEnable = vk_tramp_CmdSetDepthBiasEnable,
        .CmdSetLogicOpEXT = vk_tramp_CmdSetLogicOpEXT,
    .CmdSetPrimitiveRestartEnable = vk_tramp_CmdSetPrimitiveRestartEnable,
        .CmdSetTessellationDomainOriginEXT = vk_tramp_CmdSetTessellationDomainOriginEXT,
    .CmdSetDepthClampEnableEXT = vk_tramp_CmdSetDepthClampEnableEXT,
    .CmdSetPolygonModeEXT = vk_tramp_CmdSetPolygonModeEXT,
    .CmdSetRasterizationSamplesEXT = vk_tramp_CmdSetRasterizationSamplesEXT,
    .CmdSetSampleMaskEXT = vk_tramp_CmdSetSampleMaskEXT,
    .CmdSetAlphaToCoverageEnableEXT = vk_tramp_CmdSetAlphaToCoverageEnableEXT,
    .CmdSetAlphaToOneEnableEXT = vk_tramp_CmdSetAlphaToOneEnableEXT,
    .CmdSetLogicOpEnableEXT = vk_tramp_CmdSetLogicOpEnableEXT,
    .CmdSetColorBlendEnableEXT = vk_tramp_CmdSetColorBlendEnableEXT,
    .CmdSetColorBlendEquationEXT = vk_tramp_CmdSetColorBlendEquationEXT,
    .CmdSetColorWriteMaskEXT = vk_tramp_CmdSetColorWriteMaskEXT,
    .CmdSetRasterizationStreamEXT = vk_tramp_CmdSetRasterizationStreamEXT,
    .CmdSetConservativeRasterizationModeEXT = vk_tramp_CmdSetConservativeRasterizationModeEXT,
    .CmdSetExtraPrimitiveOverestimationSizeEXT = vk_tramp_CmdSetExtraPrimitiveOverestimationSizeEXT,
    .CmdSetDepthClipEnableEXT = vk_tramp_CmdSetDepthClipEnableEXT,
    .CmdSetSampleLocationsEnableEXT = vk_tramp_CmdSetSampleLocationsEnableEXT,
    .CmdSetColorBlendAdvancedEXT = vk_tramp_CmdSetColorBlendAdvancedEXT,
    .CmdSetProvokingVertexModeEXT = vk_tramp_CmdSetProvokingVertexModeEXT,
    .CmdSetLineRasterizationModeEXT = vk_tramp_CmdSetLineRasterizationModeEXT,
    .CmdSetLineStippleEnableEXT = vk_tramp_CmdSetLineStippleEnableEXT,
    .CmdSetDepthClipNegativeOneToOneEXT = vk_tramp_CmdSetDepthClipNegativeOneToOneEXT,
    .CmdSetViewportWScalingEnableNV = vk_tramp_CmdSetViewportWScalingEnableNV,
    .CmdSetViewportSwizzleNV = vk_tramp_CmdSetViewportSwizzleNV,
    .CmdSetCoverageToColorEnableNV = vk_tramp_CmdSetCoverageToColorEnableNV,
    .CmdSetCoverageToColorLocationNV = vk_tramp_CmdSetCoverageToColorLocationNV,
    .CmdSetCoverageModulationModeNV = vk_tramp_CmdSetCoverageModulationModeNV,
    .CmdSetCoverageModulationTableEnableNV = vk_tramp_CmdSetCoverageModulationTableEnableNV,
    .CmdSetCoverageModulationTableNV = vk_tramp_CmdSetCoverageModulationTableNV,
    .CmdSetShadingRateImageEnableNV = vk_tramp_CmdSetShadingRateImageEnableNV,
    .CmdSetCoverageReductionModeNV = vk_tramp_CmdSetCoverageReductionModeNV,
    .CmdSetRepresentativeFragmentTestEnableNV = vk_tramp_CmdSetRepresentativeFragmentTestEnableNV,
    .CreatePrivateDataSlot = vk_tramp_CreatePrivateDataSlot,
        .DestroyPrivateDataSlot = vk_tramp_DestroyPrivateDataSlot,
        .SetPrivateData = vk_tramp_SetPrivateData,
        .GetPrivateData = vk_tramp_GetPrivateData,
        .CmdCopyBuffer2 = vk_tramp_CmdCopyBuffer2,
        .CmdCopyImage2 = vk_tramp_CmdCopyImage2,
        .CmdBlitImage2 = vk_tramp_CmdBlitImage2,
        .CmdCopyBufferToImage2 = vk_tramp_CmdCopyBufferToImage2,
        .CmdCopyImageToBuffer2 = vk_tramp_CmdCopyImageToBuffer2,
        .CmdResolveImage2 = vk_tramp_CmdResolveImage2,
        .CmdSetFragmentShadingRateKHR = vk_tramp_CmdSetFragmentShadingRateKHR,
        .CmdSetFragmentShadingRateEnumNV = vk_tramp_CmdSetFragmentShadingRateEnumNV,
    .GetAccelerationStructureBuildSizesKHR = vk_tramp_GetAccelerationStructureBuildSizesKHR,
    .CmdSetVertexInputEXT = vk_tramp_CmdSetVertexInputEXT,
    .CmdSetColorWriteEnableEXT = vk_tramp_CmdSetColorWriteEnableEXT,
    .CmdSetEvent2 = vk_tramp_CmdSetEvent2,
        .CmdResetEvent2 = vk_tramp_CmdResetEvent2,
        .CmdWaitEvents2 = vk_tramp_CmdWaitEvents2,
        .CmdPipelineBarrier2 = vk_tramp_CmdPipelineBarrier2,
        .QueueSubmit2 = vk_tramp_QueueSubmit2,
        .CmdWriteTimestamp2 = vk_tramp_CmdWriteTimestamp2,
        .CmdWriteBufferMarker2AMD = vk_tramp_CmdWriteBufferMarker2AMD,
    .GetQueueCheckpointData2NV = vk_tramp_GetQueueCheckpointData2NV,
    .CopyMemoryToImageEXT = vk_tramp_CopyMemoryToImageEXT,
    .CopyImageToMemoryEXT = vk_tramp_CopyImageToMemoryEXT,
    .CopyImageToImageEXT = vk_tramp_CopyImageToImageEXT,
    .TransitionImageLayoutEXT = vk_tramp_TransitionImageLayoutEXT,
            .CreateVideoSessionKHR = vk_tramp_CreateVideoSessionKHR,
    .DestroyVideoSessionKHR = vk_tramp_DestroyVideoSessionKHR,
    .CreateVideoSessionParametersKHR = vk_tramp_CreateVideoSessionParametersKHR,
    .UpdateVideoSessionParametersKHR = vk_tramp_UpdateVideoSessionParametersKHR,
    .DestroyVideoSessionParametersKHR = vk_tramp_DestroyVideoSessionParametersKHR,
    .GetVideoSessionMemoryRequirementsKHR = vk_tramp_GetVideoSessionMemoryRequirementsKHR,
    .BindVideoSessionMemoryKHR = vk_tramp_BindVideoSessionMemoryKHR,
    .CmdDecodeVideoKHR = vk_tramp_CmdDecodeVideoKHR,
    .CmdBeginVideoCodingKHR = vk_tramp_CmdBeginVideoCodingKHR,
    .CmdControlVideoCodingKHR = vk_tramp_CmdControlVideoCodingKHR,
    .CmdEndVideoCodingKHR = vk_tramp_CmdEndVideoCodingKHR,
    .CmdDecompressMemoryNV = vk_tramp_CmdDecompressMemoryNV,
    .CmdDecompressMemoryIndirectCountNV = vk_tramp_CmdDecompressMemoryIndirectCountNV,
    .CreateCuModuleNVX = vk_tramp_CreateCuModuleNVX,
    .CreateCuFunctionNVX = vk_tramp_CreateCuFunctionNVX,
    .DestroyCuModuleNVX = vk_tramp_DestroyCuModuleNVX,
    .DestroyCuFunctionNVX = vk_tramp_DestroyCuFunctionNVX,
    .CmdCuLaunchKernelNVX = vk_tramp_CmdCuLaunchKernelNVX,
    .GetDescriptorSetLayoutSizeEXT = vk_tramp_GetDescriptorSetLayoutSizeEXT,
    .GetDescriptorSetLayoutBindingOffsetEXT = vk_tramp_GetDescriptorSetLayoutBindingOffsetEXT,
    .GetDescriptorEXT = vk_tramp_GetDescriptorEXT,
    .CmdBindDescriptorBuffersEXT = vk_tramp_CmdBindDescriptorBuffersEXT,
    .CmdSetDescriptorBufferOffsetsEXT = vk_tramp_CmdSetDescriptorBufferOffsetsEXT,
    .CmdBindDescriptorBufferEmbeddedSamplersEXT = vk_tramp_CmdBindDescriptorBufferEmbeddedSamplersEXT,
    .GetBufferOpaqueCaptureDescriptorDataEXT = vk_tramp_GetBufferOpaqueCaptureDescriptorDataEXT,
    .GetImageOpaqueCaptureDescriptorDataEXT = vk_tramp_GetImageOpaqueCaptureDescriptorDataEXT,
    .GetImageViewOpaqueCaptureDescriptorDataEXT = vk_tramp_GetImageViewOpaqueCaptureDescriptorDataEXT,
    .GetSamplerOpaqueCaptureDescriptorDataEXT = vk_tramp_GetSamplerOpaqueCaptureDescriptorDataEXT,
    .GetAccelerationStructureOpaqueCaptureDescriptorDataEXT = vk_tramp_GetAccelerationStructureOpaqueCaptureDescriptorDataEXT,
    .SetDeviceMemoryPriorityEXT = vk_tramp_SetDeviceMemoryPriorityEXT,
            .WaitForPresentKHR = vk_tramp_WaitForPresentKHR,
#ifdef VK_USE_PLATFORM_FUCHSIA
    .CreateBufferCollectionFUCHSIA = vk_tramp_CreateBufferCollectionFUCHSIA,
#endif
#ifdef VK_USE_PLATFORM_FUCHSIA
    .SetBufferCollectionBufferConstraintsFUCHSIA = vk_tramp_SetBufferCollectionBufferConstraintsFUCHSIA,
#endif
#ifdef VK_USE_PLATFORM_FUCHSIA
    .SetBufferCollectionImageConstraintsFUCHSIA = vk_tramp_SetBufferCollectionImageConstraintsFUCHSIA,
#endif
#ifdef VK_USE_PLATFORM_FUCHSIA
    .DestroyBufferCollectionFUCHSIA = vk_tramp_DestroyBufferCollectionFUCHSIA,
#endif
#ifdef VK_USE_PLATFORM_FUCHSIA
    .GetBufferCollectionPropertiesFUCHSIA = vk_tramp_GetBufferCollectionPropertiesFUCHSIA,
#endif
    .CmdBeginRendering = vk_tramp_CmdBeginRendering,
        .CmdEndRendering = vk_tramp_CmdEndRendering,
        .GetDescriptorSetLayoutHostMappingInfoVALVE = vk_tramp_GetDescriptorSetLayoutHostMappingInfoVALVE,
    .GetDescriptorSetHostMappingVALVE = vk_tramp_GetDescriptorSetHostMappingVALVE,
    .CreateMicromapEXT = vk_tramp_CreateMicromapEXT,
    .CmdBuildMicromapsEXT = vk_tramp_CmdBuildMicromapsEXT,
    .BuildMicromapsEXT = vk_tramp_BuildMicromapsEXT,
    .DestroyMicromapEXT = vk_tramp_DestroyMicromapEXT,
    .CmdCopyMicromapEXT = vk_tramp_CmdCopyMicromapEXT,
    .CopyMicromapEXT = vk_tramp_CopyMicromapEXT,
    .CmdCopyMicromapToMemoryEXT = vk_tramp_CmdCopyMicromapToMemoryEXT,
    .CopyMicromapToMemoryEXT = vk_tramp_CopyMicromapToMemoryEXT,
    .CmdCopyMemoryToMicromapEXT = vk_tramp_CmdCopyMemoryToMicromapEXT,
    .CopyMemoryToMicromapEXT = vk_tramp_CopyMemoryToMicromapEXT,
    .CmdWriteMicromapsPropertiesEXT = vk_tramp_CmdWriteMicromapsPropertiesEXT,
    .WriteMicromapsPropertiesEXT = vk_tramp_WriteMicromapsPropertiesEXT,
    .GetDeviceMicromapCompatibilityEXT = vk_tramp_GetDeviceMicromapCompatibilityEXT,
    .GetMicromapBuildSizesEXT = vk_tramp_GetMicromapBuildSizesEXT,
    .GetShaderModuleIdentifierEXT = vk_tramp_GetShaderModuleIdentifierEXT,
    .GetShaderModuleCreateInfoIdentifierEXT = vk_tramp_GetShaderModuleCreateInfoIdentifierEXT,
    .GetImageSubresourceLayout2KHR = vk_tramp_GetImageSubresourceLayout2KHR,
        .GetPipelinePropertiesEXT = vk_tramp_GetPipelinePropertiesEXT,
#ifdef VK_USE_PLATFORM_METAL_EXT
    .ExportMetalObjectsEXT = vk_tramp_ExportMetalObjectsEXT,
#endif
    .GetFramebufferTilePropertiesQCOM = vk_tramp_GetFramebufferTilePropertiesQCOM,
    .GetDynamicRenderingTilePropertiesQCOM = vk_tramp_GetDynamicRenderingTilePropertiesQCOM,
        .CreateOpticalFlowSessionNV = vk_tramp_CreateOpticalFlowSessionNV,
    .DestroyOpticalFlowSessionNV = vk_tramp_DestroyOpticalFlowSessionNV,
    .BindOpticalFlowSessionImageNV = vk_tramp_BindOpticalFlowSessionImageNV,
    .CmdOpticalFlowExecuteNV = vk_tramp_CmdOpticalFlowExecuteNV,
    .GetDeviceFaultInfoEXT = vk_tramp_GetDeviceFaultInfoEXT,
    .CmdSetDepthBias2EXT = vk_tramp_CmdSetDepthBias2EXT,
    .ReleaseSwapchainImagesEXT = vk_tramp_ReleaseSwapchainImagesEXT,
    .GetDeviceImageSubresourceLayoutKHR = vk_tramp_GetDeviceImageSubresourceLayoutKHR,
    .MapMemory2KHR = vk_tramp_MapMemory2KHR,
    .UnmapMemory2KHR = vk_tramp_UnmapMemory2KHR,
    .CreateShadersEXT = vk_tramp_CreateShadersEXT,
    .DestroyShaderEXT = vk_tramp_DestroyShaderEXT,
    .GetShaderBinaryDataEXT = vk_tramp_GetShaderBinaryDataEXT,
    .CmdBindShadersEXT = vk_tramp_CmdBindShadersEXT,
#ifdef VK_USE_PLATFORM_SCREEN_QNX
    .GetScreenBufferPropertiesQNX = vk_tramp_GetScreenBufferPropertiesQNX,
#endif
        .SetLatencySleepModeNV = vk_tramp_SetLatencySleepModeNV,
    .LatencySleepNV = vk_tramp_LatencySleepNV,
    .SetLatencyMarkerNV = vk_tramp_SetLatencyMarkerNV,
    .GetLatencyTimingsNV = vk_tramp_GetLatencyTimingsNV,
    .QueueNotifyOutOfBandNV = vk_tramp_QueueNotifyOutOfBandNV,
};
