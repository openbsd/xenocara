/*
 * Copyright © 2015 Intel Corporation
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

/* This file generated from radv_entrypoints_gen.py, don't edit directly. */

#include "radv_private.h"

#include "util/macros.h"

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
    "vkCreateDebugReportCallbackEXT\0"
    "vkCreateDisplayPlaneSurfaceKHR\0"
    "vkCreateInstance\0"
    "vkCreateWaylandSurfaceKHR\0"
    "vkCreateXcbSurfaceKHR\0"
    "vkCreateXlibSurfaceKHR\0"
    "vkDebugReportMessageEXT\0"
    "vkDestroyDebugReportCallbackEXT\0"
    "vkDestroyInstance\0"
    "vkDestroySurfaceKHR\0"
    "vkEnumerateInstanceExtensionProperties\0"
    "vkEnumerateInstanceLayerProperties\0"
    "vkEnumerateInstanceVersion\0"
    "vkEnumeratePhysicalDeviceGroups\0"
    "vkEnumeratePhysicalDeviceGroupsKHR\0"
    "vkEnumeratePhysicalDevices\0"
    "vkGetInstanceProcAddr\0"
;

static const struct string_map_entry instance_string_map_entries[] = {
    { 0, 0x987ef56, 12 }, /* vkCreateDebugReportCallbackEXT */
    { 31, 0x7ac4dacb, 7 }, /* vkCreateDisplayPlaneSurfaceKHR */
    { 62, 0x38a581a6, 0 }, /* vkCreateInstance */
    { 79, 0x2b2a4b79, 9 }, /* vkCreateWaylandSurfaceKHR */
    { 105, 0xc5e5b106, 11 }, /* vkCreateXcbSurfaceKHR */
    { 127, 0xa693bc66, 10 }, /* vkCreateXlibSurfaceKHR */
    { 150, 0xa4e75334, 14 }, /* vkDebugReportMessageEXT */
    { 174, 0x43d4c4e2, 13 }, /* vkDestroyDebugReportCallbackEXT */
    { 206, 0x9bd21af2, 1 }, /* vkDestroyInstance */
    { 224, 0xf204ce7d, 8 }, /* vkDestroySurfaceKHR */
    { 244, 0xeb27627e, 6 }, /* vkEnumerateInstanceExtensionProperties */
    { 283, 0x81f69d8, 5 }, /* vkEnumerateInstanceLayerProperties */
    { 318, 0xd0481e5c, 4 }, /* vkEnumerateInstanceVersion */
    { 345, 0x270514f0, 15 }, /* vkEnumeratePhysicalDeviceGroups */
    { 377, 0x549ce595, 16 }, /* vkEnumeratePhysicalDeviceGroupsKHR */
    { 412, 0x5787c327, 2 }, /* vkEnumeratePhysicalDevices */
    { 439, 0x3d2ae9ad, 3 }, /* vkGetInstanceProcAddr */
};

/* Hash table stats:
 * size 17 entries
 * collisions entries:
 *     0      15
 *     1      0
 *     2      1
 *     3      1
 *     4      0
 *     5      0
 *     6      0
 *     7      0
 *     8      0
 *     9+     0
 */

#define none 0xffff
static const uint16_t instance_string_map[32] = {
    none,
    none,
    0x0007,
    none,
    none,
    none,
    0x0002,
    0x000f,
    none,
    none,
    none,
    0x0001,
    0x0004,
    0x0010,
    none,
    none,
    0x000d,
    none,
    0x0008,
    none,
    0x0006,
    0x000e,
    0x0000,
    none,
    0x000b,
    0x0003,
    none,
    none,
    0x000c,
    0x0009,
    0x000a,
    0x0005,
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
        i = instance_string_map[h & 31];
        if (i == none)
           return -1;
        e = &instance_string_map_entries[i];
        if (e->hash == hash && strcmp(str, instance_strings + e->name) == 0)
            return e->num;
        h += prime_step;
    }

    return -1;
}

static const char *
instance_entry_name(int num)
{
   for (int i = 0; i < ARRAY_SIZE(instance_string_map_entries); i++) {
      if (instance_string_map_entries[i].num == num)
         return &instance_strings[instance_string_map_entries[i].name];
   }
   return NULL;
}


static const char physical_device_strings[] =
    "vkAcquireXlibDisplayEXT\0"
    "vkCreateDevice\0"
    "vkCreateDisplayModeKHR\0"
    "vkEnumerateDeviceExtensionProperties\0"
    "vkEnumerateDeviceLayerProperties\0"
    "vkGetDisplayModeProperties2KHR\0"
    "vkGetDisplayModePropertiesKHR\0"
    "vkGetDisplayPlaneCapabilities2KHR\0"
    "vkGetDisplayPlaneCapabilitiesKHR\0"
    "vkGetDisplayPlaneSupportedDisplaysKHR\0"
    "vkGetPhysicalDeviceCalibrateableTimeDomainsEXT\0"
    "vkGetPhysicalDeviceDisplayPlaneProperties2KHR\0"
    "vkGetPhysicalDeviceDisplayPlanePropertiesKHR\0"
    "vkGetPhysicalDeviceDisplayProperties2KHR\0"
    "vkGetPhysicalDeviceDisplayPropertiesKHR\0"
    "vkGetPhysicalDeviceExternalBufferProperties\0"
    "vkGetPhysicalDeviceExternalBufferPropertiesKHR\0"
    "vkGetPhysicalDeviceExternalFenceProperties\0"
    "vkGetPhysicalDeviceExternalFencePropertiesKHR\0"
    "vkGetPhysicalDeviceExternalSemaphoreProperties\0"
    "vkGetPhysicalDeviceExternalSemaphorePropertiesKHR\0"
    "vkGetPhysicalDeviceFeatures\0"
    "vkGetPhysicalDeviceFeatures2\0"
    "vkGetPhysicalDeviceFeatures2KHR\0"
    "vkGetPhysicalDeviceFormatProperties\0"
    "vkGetPhysicalDeviceFormatProperties2\0"
    "vkGetPhysicalDeviceFormatProperties2KHR\0"
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
    "vkGetPhysicalDeviceQueueFamilyProperties\0"
    "vkGetPhysicalDeviceQueueFamilyProperties2\0"
    "vkGetPhysicalDeviceQueueFamilyProperties2KHR\0"
    "vkGetPhysicalDeviceSparseImageFormatProperties\0"
    "vkGetPhysicalDeviceSparseImageFormatProperties2\0"
    "vkGetPhysicalDeviceSparseImageFormatProperties2KHR\0"
    "vkGetPhysicalDeviceSurfaceCapabilities2EXT\0"
    "vkGetPhysicalDeviceSurfaceCapabilities2KHR\0"
    "vkGetPhysicalDeviceSurfaceCapabilitiesKHR\0"
    "vkGetPhysicalDeviceSurfaceFormats2KHR\0"
    "vkGetPhysicalDeviceSurfaceFormatsKHR\0"
    "vkGetPhysicalDeviceSurfacePresentModesKHR\0"
    "vkGetPhysicalDeviceSurfaceSupportKHR\0"
    "vkGetPhysicalDeviceWaylandPresentationSupportKHR\0"
    "vkGetPhysicalDeviceXcbPresentationSupportKHR\0"
    "vkGetPhysicalDeviceXlibPresentationSupportKHR\0"
    "vkGetRandROutputDisplayEXT\0"
    "vkReleaseDisplayEXT\0"
;

static const struct string_map_entry physical_device_string_map_entries[] = {
    { 0, 0x60df100d, 44 }, /* vkAcquireXlibDisplayEXT */
    { 24, 0x85ed23f, 6 }, /* vkCreateDevice */
    { 39, 0xcc0bde41, 14 }, /* vkCreateDisplayModeKHR */
    { 62, 0x5fd13eed, 8 }, /* vkEnumerateDeviceExtensionProperties */
    { 99, 0x2f8566e7, 7 }, /* vkEnumerateDeviceLayerProperties */
    { 132, 0x3e613e42, 53 }, /* vkGetDisplayModeProperties2KHR */
    { 163, 0x36b8a8de, 13 }, /* vkGetDisplayModePropertiesKHR */
    { 193, 0xff1655a4, 54 }, /* vkGetDisplayPlaneCapabilities2KHR */
    { 227, 0x4b60d48c, 15 }, /* vkGetDisplayPlaneCapabilitiesKHR */
    { 260, 0xabef4889, 12 }, /* vkGetDisplayPlaneSupportedDisplaysKHR */
    { 298, 0xea07da1a, 55 }, /* vkGetPhysicalDeviceCalibrateableTimeDomainsEXT */
    { 345, 0xb7bc4386, 52 }, /* vkGetPhysicalDeviceDisplayPlaneProperties2KHR */
    { 391, 0xb9b8ddba, 11 }, /* vkGetPhysicalDeviceDisplayPlanePropertiesKHR */
    { 436, 0x540c0372, 51 }, /* vkGetPhysicalDeviceDisplayProperties2KHR */
    { 477, 0xfa0cd2e, 10 }, /* vkGetPhysicalDeviceDisplayPropertiesKHR */
    { 517, 0x944476dc, 37 }, /* vkGetPhysicalDeviceExternalBufferProperties */
    { 561, 0xee68b389, 38 }, /* vkGetPhysicalDeviceExternalBufferPropertiesKHR */
    { 608, 0x3bc965eb, 41 }, /* vkGetPhysicalDeviceExternalFenceProperties */
    { 651, 0x99b35492, 42 }, /* vkGetPhysicalDeviceExternalFencePropertiesKHR */
    { 697, 0xcf251b0e, 39 }, /* vkGetPhysicalDeviceExternalSemaphoreProperties */
    { 744, 0x984c3fa7, 40 }, /* vkGetPhysicalDeviceExternalSemaphorePropertiesKHR */
    { 794, 0x113e2f33, 3 }, /* vkGetPhysicalDeviceFeatures */
    { 822, 0x63c068a7, 23 }, /* vkGetPhysicalDeviceFeatures2 */
    { 851, 0x6a9a3636, 24 }, /* vkGetPhysicalDeviceFeatures2KHR */
    { 883, 0x3e54b398, 4 }, /* vkGetPhysicalDeviceFormatProperties */
    { 919, 0xca3bb9da, 27 }, /* vkGetPhysicalDeviceFormatProperties2 */
    { 956, 0x9099cbbb, 28 }, /* vkGetPhysicalDeviceFormatProperties2KHR */
    { 996, 0xdd36a867, 5 }, /* vkGetPhysicalDeviceImageFormatProperties */
    { 1037, 0x35d260d3, 29 }, /* vkGetPhysicalDeviceImageFormatProperties2 */
    { 1079, 0x102ff7ea, 30 }, /* vkGetPhysicalDeviceImageFormatProperties2KHR */
    { 1124, 0xa90da4da, 2 }, /* vkGetPhysicalDeviceMemoryProperties */
    { 1160, 0xcb4cc208, 33 }, /* vkGetPhysicalDeviceMemoryProperties2 */
    { 1197, 0xc8c3da3d, 34 }, /* vkGetPhysicalDeviceMemoryProperties2KHR */
    { 1237, 0x219aa0b9, 48 }, /* vkGetPhysicalDeviceMultisamplePropertiesEXT */
    { 1281, 0x100341b4, 47 }, /* vkGetPhysicalDevicePresentRectanglesKHR */
    { 1321, 0x52fe22c9, 0 }, /* vkGetPhysicalDeviceProperties */
    { 1351, 0x6c4d8ee1, 25 }, /* vkGetPhysicalDeviceProperties2 */
    { 1382, 0xcd15838c, 26 }, /* vkGetPhysicalDeviceProperties2KHR */
    { 1416, 0x4e5fc88a, 1 }, /* vkGetPhysicalDeviceQueueFamilyProperties */
    { 1457, 0xcad374d8, 31 }, /* vkGetPhysicalDeviceQueueFamilyProperties2 */
    { 1499, 0x5ceb2bed, 32 }, /* vkGetPhysicalDeviceQueueFamilyProperties2KHR */
    { 1544, 0x272ef8ef, 9 }, /* vkGetPhysicalDeviceSparseImageFormatProperties */
    { 1591, 0xebddba0b, 35 }, /* vkGetPhysicalDeviceSparseImageFormatProperties2 */
    { 1639, 0x8746ed72, 36 }, /* vkGetPhysicalDeviceSparseImageFormatProperties2KHR */
    { 1690, 0x5a5fba04, 46 }, /* vkGetPhysicalDeviceSurfaceCapabilities2EXT */
    { 1733, 0x9497e378, 49 }, /* vkGetPhysicalDeviceSurfaceCapabilities2KHR */
    { 1776, 0x77890558, 17 }, /* vkGetPhysicalDeviceSurfaceCapabilitiesKHR */
    { 1818, 0xd00b7188, 50 }, /* vkGetPhysicalDeviceSurfaceFormats2KHR */
    { 1856, 0xe32227c8, 18 }, /* vkGetPhysicalDeviceSurfaceFormatsKHR */
    { 1893, 0x31c3cbd1, 19 }, /* vkGetPhysicalDeviceSurfacePresentModesKHR */
    { 1935, 0x1a687885, 16 }, /* vkGetPhysicalDeviceSurfaceSupportKHR */
    { 1972, 0x84e085ac, 20 }, /* vkGetPhysicalDeviceWaylandPresentationSupportKHR */
    { 2021, 0x41782cb9, 22 }, /* vkGetPhysicalDeviceXcbPresentationSupportKHR */
    { 2066, 0x34a063ab, 21 }, /* vkGetPhysicalDeviceXlibPresentationSupportKHR */
    { 2112, 0xb87cdd6c, 45 }, /* vkGetRandROutputDisplayEXT */
    { 2139, 0x4207f4f1, 43 }, /* vkReleaseDisplayEXT */
};

/* Hash table stats:
 * size 56 entries
 * collisions entries:
 *     0      44
 *     1      7
 *     2      5
 *     3      0
 *     4      0
 *     5      0
 *     6      0
 *     7      0
 *     8      0
 *     9+     0
 */

#define none 0xffff
static const uint16_t physical_device_string_map[128] = {
    0x001e,
    none,
    none,
    none,
    0x002c,
    0x002b,
    0x000b,
    none,
    0x001f,
    0x0009,
    0x0026,
    0x002a,
    0x0008,
    0x0000,
    0x0013,
    none,
    none,
    none,
    0x0012,
    0x0028,
    none,
    none,
    none,
    none,
    0x0018,
    none,
    0x000a,
    0x002f,
    0x0010,
    none,
    none,
    0x0025,
    none,
    none,
    none,
    none,
    0x0007,
    none,
    none,
    0x0014,
    none,
    none,
    none,
    0x0032,
    0x0033,
    none,
    0x000e,
    none,
    none,
    none,
    none,
    0x0015,
    0x0022,
    none,
    0x0017,
    none,
    none,
    0x0021,
    0x000c,
    0x001a,
    none,
    0x0020,
    0x0035,
    0x0001,
    none,
    0x0002,
    0x0005,
    none,
    none,
    none,
    none,
    none,
    0x0030,
    0x0023,
    none,
    none,
    0x0034,
    0x0016,
    none,
    none,
    none,
    0x0031,
    none,
    0x001c,
    none,
    none,
    none,
    none,
    0x0027,
    none,
    0x0019,
    none,
    0x000f,
    none,
    0x0006,
    none,
    none,
    0x0024,
    none,
    none,
    none,
    none,
    none,
    0x0004,
    none,
    none,
    0x001d,
    0x0011,
    0x0036,
    0x0003,
    none,
    0x0029,
    none,
    0x0037,
    0x000d,
    none,
    none,
    none,
    none,
    none,
    0x002d,
    none,
    0x001b,
    none,
    none,
    none,
    0x002e,
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

static const char *
physical_device_entry_name(int num)
{
   for (int i = 0; i < ARRAY_SIZE(physical_device_string_map_entries); i++) {
      if (physical_device_string_map_entries[i].num == num)
         return &physical_device_strings[physical_device_string_map_entries[i].name];
   }
   return NULL;
}


static const char device_strings[] =
    "vkAcquireImageANDROID\0"
    "vkAcquireNextImage2KHR\0"
    "vkAcquireNextImageKHR\0"
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
    "vkCmdBeginConditionalRenderingEXT\0"
    "vkCmdBeginQuery\0"
    "vkCmdBeginQueryIndexedEXT\0"
    "vkCmdBeginRenderPass\0"
    "vkCmdBeginRenderPass2\0"
    "vkCmdBeginRenderPass2KHR\0"
    "vkCmdBeginTransformFeedbackEXT\0"
    "vkCmdBindDescriptorSets\0"
    "vkCmdBindIndexBuffer\0"
    "vkCmdBindPipeline\0"
    "vkCmdBindTransformFeedbackBuffersEXT\0"
    "vkCmdBindVertexBuffers\0"
    "vkCmdBlitImage\0"
    "vkCmdClearAttachments\0"
    "vkCmdClearColorImage\0"
    "vkCmdClearDepthStencilImage\0"
    "vkCmdCopyBuffer\0"
    "vkCmdCopyBufferToImage\0"
    "vkCmdCopyImage\0"
    "vkCmdCopyImageToBuffer\0"
    "vkCmdCopyQueryPoolResults\0"
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
    "vkCmdEndConditionalRenderingEXT\0"
    "vkCmdEndQuery\0"
    "vkCmdEndQueryIndexedEXT\0"
    "vkCmdEndRenderPass\0"
    "vkCmdEndRenderPass2\0"
    "vkCmdEndRenderPass2KHR\0"
    "vkCmdEndTransformFeedbackEXT\0"
    "vkCmdExecuteCommands\0"
    "vkCmdFillBuffer\0"
    "vkCmdNextSubpass\0"
    "vkCmdNextSubpass2\0"
    "vkCmdNextSubpass2KHR\0"
    "vkCmdPipelineBarrier\0"
    "vkCmdPushConstants\0"
    "vkCmdPushDescriptorSetKHR\0"
    "vkCmdPushDescriptorSetWithTemplateKHR\0"
    "vkCmdResetEvent\0"
    "vkCmdResetQueryPool\0"
    "vkCmdResolveImage\0"
    "vkCmdSetBlendConstants\0"
    "vkCmdSetDepthBias\0"
    "vkCmdSetDepthBounds\0"
    "vkCmdSetDeviceMask\0"
    "vkCmdSetDeviceMaskKHR\0"
    "vkCmdSetDiscardRectangleEXT\0"
    "vkCmdSetEvent\0"
    "vkCmdSetLineStippleEXT\0"
    "vkCmdSetLineWidth\0"
    "vkCmdSetSampleLocationsEXT\0"
    "vkCmdSetScissor\0"
    "vkCmdSetStencilCompareMask\0"
    "vkCmdSetStencilReference\0"
    "vkCmdSetStencilWriteMask\0"
    "vkCmdSetViewport\0"
    "vkCmdUpdateBuffer\0"
    "vkCmdWaitEvents\0"
    "vkCmdWriteAccelerationStructuresPropertiesNV\0"
    "vkCmdWriteBufferMarkerAMD\0"
    "vkCmdWriteTimestamp\0"
    "vkCreateBuffer\0"
    "vkCreateBufferView\0"
    "vkCreateCommandPool\0"
    "vkCreateComputePipelines\0"
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
    "vkCreatePipelineCache\0"
    "vkCreatePipelineLayout\0"
    "vkCreateQueryPool\0"
    "vkCreateRenderPass\0"
    "vkCreateRenderPass2\0"
    "vkCreateRenderPass2KHR\0"
    "vkCreateSampler\0"
    "vkCreateSamplerYcbcrConversion\0"
    "vkCreateSamplerYcbcrConversionKHR\0"
    "vkCreateSemaphore\0"
    "vkCreateShaderModule\0"
    "vkCreateSwapchainKHR\0"
    "vkDestroyAccelerationStructureNV\0"
    "vkDestroyBuffer\0"
    "vkDestroyBufferView\0"
    "vkDestroyCommandPool\0"
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
    "vkDestroyPipeline\0"
    "vkDestroyPipelineCache\0"
    "vkDestroyPipelineLayout\0"
    "vkDestroyQueryPool\0"
    "vkDestroyRenderPass\0"
    "vkDestroySampler\0"
    "vkDestroySamplerYcbcrConversion\0"
    "vkDestroySamplerYcbcrConversionKHR\0"
    "vkDestroySemaphore\0"
    "vkDestroyShaderModule\0"
    "vkDestroySwapchainKHR\0"
    "vkDeviceWaitIdle\0"
    "vkDisplayPowerControlEXT\0"
    "vkEndCommandBuffer\0"
    "vkFlushMappedMemoryRanges\0"
    "vkFreeCommandBuffers\0"
    "vkFreeDescriptorSets\0"
    "vkFreeMemory\0"
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
    "vkGetDescriptorSetLayoutSupport\0"
    "vkGetDescriptorSetLayoutSupportKHR\0"
    "vkGetDeviceGroupPeerMemoryFeatures\0"
    "vkGetDeviceGroupPeerMemoryFeaturesKHR\0"
    "vkGetDeviceGroupPresentCapabilitiesKHR\0"
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
    "vkGetImageMemoryRequirements\0"
    "vkGetImageMemoryRequirements2\0"
    "vkGetImageMemoryRequirements2KHR\0"
    "vkGetImageSparseMemoryRequirements\0"
    "vkGetImageSparseMemoryRequirements2\0"
    "vkGetImageSparseMemoryRequirements2KHR\0"
    "vkGetImageSubresourceLayout\0"
    "vkGetMemoryAndroidHardwareBufferANDROID\0"
    "vkGetMemoryFdKHR\0"
    "vkGetMemoryFdPropertiesKHR\0"
    "vkGetMemoryHostPointerPropertiesEXT\0"
    "vkGetPipelineCacheData\0"
    "vkGetPipelineExecutableInternalRepresentationsKHR\0"
    "vkGetPipelineExecutablePropertiesKHR\0"
    "vkGetPipelineExecutableStatisticsKHR\0"
    "vkGetQueryPoolResults\0"
    "vkGetRayTracingShaderGroupHandlesNV\0"
    "vkGetRenderAreaGranularity\0"
    "vkGetSemaphoreCounterValue\0"
    "vkGetSemaphoreCounterValueKHR\0"
    "vkGetSemaphoreFdKHR\0"
    "vkGetShaderInfoAMD\0"
    "vkGetSwapchainCounterEXT\0"
    "vkGetSwapchainGrallocUsage2ANDROID\0"
    "vkGetSwapchainGrallocUsageANDROID\0"
    "vkGetSwapchainImagesKHR\0"
    "vkImportFenceFdKHR\0"
    "vkImportSemaphoreFdKHR\0"
    "vkInvalidateMappedMemoryRanges\0"
    "vkMapMemory\0"
    "vkMergePipelineCaches\0"
    "vkQueueBindSparse\0"
    "vkQueuePresentKHR\0"
    "vkQueueSignalReleaseImageANDROID\0"
    "vkQueueSubmit\0"
    "vkQueueWaitIdle\0"
    "vkRegisterDeviceEventEXT\0"
    "vkRegisterDisplayEventEXT\0"
    "vkResetCommandBuffer\0"
    "vkResetCommandPool\0"
    "vkResetDescriptorPool\0"
    "vkResetEvent\0"
    "vkResetFences\0"
    "vkResetQueryPool\0"
    "vkResetQueryPoolEXT\0"
    "vkSetEvent\0"
    "vkSignalSemaphore\0"
    "vkSignalSemaphoreKHR\0"
    "vkTrimCommandPool\0"
    "vkTrimCommandPoolKHR\0"
    "vkUnmapMemory\0"
    "vkUpdateDescriptorSetWithTemplate\0"
    "vkUpdateDescriptorSetWithTemplateKHR\0"
    "vkUpdateDescriptorSets\0"
    "vkWaitForFences\0"
    "vkWaitSemaphores\0"
    "vkWaitSemaphoresKHR\0"
;

static const struct string_map_entry device_string_map_entries[] = {
    { 0, 0x6bf780dd, 180 }, /* vkAcquireImageANDROID */
    { 22, 0x82860572, 153 }, /* vkAcquireNextImage2KHR */
    { 45, 0xc3fedb2e, 128 }, /* vkAcquireNextImageKHR */
    { 67, 0x8c0c811a, 74 }, /* vkAllocateCommandBuffers */
    { 92, 0x4c449d3a, 63 }, /* vkAllocateDescriptorSets */
    { 117, 0x522b85d3, 6 }, /* vkAllocateMemory */
    { 134, 0xc54f7327, 76 }, /* vkBeginCommandBuffer */
    { 155, 0x3ec4e21a, 215 }, /* vkBindAccelerationStructureMemoryNV */
    { 191, 0x6bcbdcb, 14 }, /* vkBindBufferMemory */
    { 210, 0xc27aaf4f, 145 }, /* vkBindBufferMemory2 */
    { 230, 0x6878d3ce, 146 }, /* vkBindBufferMemory2KHR */
    { 253, 0x5caaae4a, 16 }, /* vkBindImageMemory */
    { 271, 0xa9097118, 147 }, /* vkBindImageMemory2 */
    { 290, 0xf18729ad, 148 }, /* vkBindImageMemory2KHR */
    { 312, 0xe561c19f, 115 }, /* vkCmdBeginConditionalRenderingEXT */
    { 346, 0xf5064ea4, 113 }, /* vkCmdBeginQuery */
    { 362, 0x73251a2c, 211 }, /* vkCmdBeginQueryIndexedEXT */
    { 388, 0xcb7a58e3, 121 }, /* vkCmdBeginRenderPass */
    { 409, 0x9c876577, 188 }, /* vkCmdBeginRenderPass2 */
    { 431, 0x8b6b4de6, 189 }, /* vkCmdBeginRenderPass2KHR */
    { 456, 0xb217c94, 209 }, /* vkCmdBeginTransformFeedbackEXT */
    { 487, 0x28c7a5da, 89 }, /* vkCmdBindDescriptorSets */
    { 511, 0x4c22d870, 90 }, /* vkCmdBindIndexBuffer */
    { 532, 0x3af9fd84, 79 }, /* vkCmdBindPipeline */
    { 550, 0x98fdb5cd, 208 }, /* vkCmdBindTransformFeedbackBuffersEXT */
    { 587, 0xa9c83f1d, 91 }, /* vkCmdBindVertexBuffers */
    { 610, 0x331ebf89, 100 }, /* vkCmdBlitImage */
    { 625, 0x93cb5cb8, 107 }, /* vkCmdClearAttachments */
    { 647, 0xb4bc8d08, 105 }, /* vkCmdClearColorImage */
    { 668, 0x4f88e4ba, 106 }, /* vkCmdClearDepthStencilImage */
    { 696, 0xc939a0da, 98 }, /* vkCmdCopyBuffer */
    { 712, 0x929847e, 101 }, /* vkCmdCopyBufferToImage */
    { 735, 0x278effa9, 99 }, /* vkCmdCopyImage */
    { 750, 0x68cddbac, 102 }, /* vkCmdCopyImageToBuffer */
    { 773, 0xdee8c6d4, 119 }, /* vkCmdCopyQueryPoolResults */
    { 799, 0xbd58e867, 96 }, /* vkCmdDispatch */
    { 813, 0xfb767220, 154 }, /* vkCmdDispatchBase */
    { 831, 0x402403e5, 155 }, /* vkCmdDispatchBaseKHR */
    { 852, 0xd6353005, 97 }, /* vkCmdDispatchIndirect */
    { 874, 0x9912c1a1, 92 }, /* vkCmdDraw */
    { 884, 0xbe5a8058, 93 }, /* vkCmdDrawIndexed */
    { 901, 0x94e7ed36, 95 }, /* vkCmdDrawIndexedIndirect */
    { 926, 0xb4acef41, 205 }, /* vkCmdDrawIndexedIndirectCount */
    { 956, 0xc86e9287, 207 }, /* vkCmdDrawIndexedIndirectCountAMD */
    { 989, 0xda9e8a2c, 206 }, /* vkCmdDrawIndexedIndirectCountKHR */
    { 1022, 0xe9ac41bf, 94 }, /* vkCmdDrawIndirect */
    { 1040, 0x80c3b089, 213 }, /* vkCmdDrawIndirectByteCountEXT */
    { 1070, 0x40079990, 202 }, /* vkCmdDrawIndirectCount */
    { 1093, 0xe5ad0a50, 204 }, /* vkCmdDrawIndirectCountAMD */
    { 1119, 0xf7dd01f5, 203 }, /* vkCmdDrawIndirectCountKHR */
    { 1145, 0x18c8217d, 116 }, /* vkCmdEndConditionalRenderingEXT */
    { 1177, 0xd556fd22, 114 }, /* vkCmdEndQuery */
    { 1191, 0xd5c2f48a, 212 }, /* vkCmdEndQueryIndexedEXT */
    { 1215, 0xdcdb0235, 123 }, /* vkCmdEndRenderPass */
    { 1234, 0x1cbf9115, 192 }, /* vkCmdEndRenderPass2 */
    { 1254, 0x57eebe78, 193 }, /* vkCmdEndRenderPass2KHR */
    { 1277, 0xf008d706, 210 }, /* vkCmdEndTransformFeedbackEXT */
    { 1306, 0x9eaabe40, 124 }, /* vkCmdExecuteCommands */
    { 1327, 0x5bdd2ae0, 104 }, /* vkCmdFillBuffer */
    { 1343, 0x2eeec2f9, 122 }, /* vkCmdNextSubpass */
    { 1360, 0xd4fc131, 190 }, /* vkCmdNextSubpass2 */
    { 1378, 0x25b621bc, 191 }, /* vkCmdNextSubpass2KHR */
    { 1399, 0x97fccfe8, 112 }, /* vkCmdPipelineBarrier */
    { 1420, 0xb1c6b468, 120 }, /* vkCmdPushConstants */
    { 1439, 0xf17232a1, 130 }, /* vkCmdPushDescriptorSetKHR */
    { 1465, 0x3d528981, 162 }, /* vkCmdPushDescriptorSetWithTemplateKHR */
    { 1503, 0x4fccce28, 110 }, /* vkCmdResetEvent */
    { 1519, 0x2f614082, 117 }, /* vkCmdResetQueryPool */
    { 1539, 0x671bb594, 108 }, /* vkCmdResolveImage */
    { 1557, 0x1c989dfb, 84 }, /* vkCmdSetBlendConstants */
    { 1580, 0x30f14d07, 83 }, /* vkCmdSetDepthBias */
    { 1598, 0x7b3a8a63, 85 }, /* vkCmdSetDepthBounds */
    { 1618, 0xaecdae87, 149 }, /* vkCmdSetDeviceMask */
    { 1637, 0xfbb79356, 150 }, /* vkCmdSetDeviceMaskKHR */
    { 1659, 0x64df188b, 163 }, /* vkCmdSetDiscardRectangleEXT */
    { 1687, 0xe257f075, 109 }, /* vkCmdSetEvent */
    { 1701, 0xbdaa62f9, 228 }, /* vkCmdSetLineStippleEXT */
    { 1724, 0x32282165, 82 }, /* vkCmdSetLineWidth */
    { 1742, 0xa9e2c72, 164 }, /* vkCmdSetSampleLocationsEXT */
    { 1769, 0x48f28c7f, 81 }, /* vkCmdSetScissor */
    { 1785, 0xa8f534e2, 86 }, /* vkCmdSetStencilCompareMask */
    { 1812, 0x83e2b024, 88 }, /* vkCmdSetStencilReference */
    { 1837, 0xe7c4b134, 87 }, /* vkCmdSetStencilWriteMask */
    { 1862, 0x53d6c2b, 80 }, /* vkCmdSetViewport */
    { 1879, 0xd2986b5e, 103 }, /* vkCmdUpdateBuffer */
    { 1897, 0x3b9346b3, 111 }, /* vkCmdWaitEvents */
    { 1913, 0xd2925ead, 216 }, /* vkCmdWriteAccelerationStructuresPropertiesNV */
    { 1958, 0x447be82c, 185 }, /* vkCmdWriteBufferMarkerAMD */
    { 1984, 0xec4d324c, 118 }, /* vkCmdWriteTimestamp */
    { 2004, 0x7d4282b9, 36 }, /* vkCreateBuffer */
    { 2019, 0x925bd256, 38 }, /* vkCreateBufferView */
    { 2038, 0x820fe476, 71 }, /* vkCreateCommandPool */
    { 2058, 0xf70c85eb, 52 }, /* vkCreateComputePipelines */
    { 2083, 0xfb95a8a4, 60 }, /* vkCreateDescriptorPool */
    { 2106, 0x3c14cc74, 58 }, /* vkCreateDescriptorSetLayout */
    { 2134, 0xad3ce733, 156 }, /* vkCreateDescriptorUpdateTemplate */
    { 2167, 0x5189488a, 157 }, /* vkCreateDescriptorUpdateTemplateKHR */
    { 2203, 0xe7188731, 26 }, /* vkCreateEvent */
    { 2217, 0x958af968, 19 }, /* vkCreateFence */
    { 2231, 0x887a38c4, 66 }, /* vkCreateFramebuffer */
    { 2251, 0x4b59f96d, 51 }, /* vkCreateGraphicsPipelines */
    { 2277, 0x652128c2, 40 }, /* vkCreateImage */
    { 2291, 0xdce077ff, 43 }, /* vkCreateImageView */
    { 2309, 0xcbf6489f, 47 }, /* vkCreatePipelineCache */
    { 2331, 0x451ef1ed, 54 }, /* vkCreatePipelineLayout */
    { 2354, 0x5edcd92b, 31 }, /* vkCreateQueryPool */
    { 2372, 0x109a9c18, 68 }, /* vkCreateRenderPass */
    { 2391, 0x46b16d5a, 186 }, /* vkCreateRenderPass2 */
    { 2411, 0xfa16043b, 187 }, /* vkCreateRenderPass2KHR */
    { 2434, 0x13cf03f, 56 }, /* vkCreateSampler */
    { 2450, 0xe6a58c26, 171 }, /* vkCreateSamplerYcbcrConversion */
    { 2481, 0x7482104f, 172 }, /* vkCreateSamplerYcbcrConversionKHR */
    { 2515, 0xf2065e5b, 24 }, /* vkCreateSemaphore */
    { 2533, 0xa0d3cea2, 45 }, /* vkCreateShaderModule */
    { 2554, 0xcdefcaa8, 125 }, /* vkCreateSwapchainKHR */
    { 2575, 0x693f9d26, 214 }, /* vkDestroyAccelerationStructureNV */
    { 2608, 0x94a07a45, 37 }, /* vkDestroyBuffer */
    { 2624, 0x98b27962, 39 }, /* vkDestroyBufferView */
    { 2644, 0xd5d83a0a, 72 }, /* vkDestroyCommandPool */
    { 2665, 0x47bdaf30, 61 }, /* vkDestroyDescriptorPool */
    { 2689, 0xa4227b08, 59 }, /* vkDestroyDescriptorSetLayout */
    { 2718, 0xbb2cbe7f, 158 }, /* vkDestroyDescriptorUpdateTemplate */
    { 2752, 0xaa83901e, 159 }, /* vkDestroyDescriptorUpdateTemplateKHR */
    { 2789, 0x1fbcc9cb, 1 }, /* vkDestroyDevice */
    { 2805, 0x4df27c05, 27 }, /* vkDestroyEvent */
    { 2820, 0xfc64ee3c, 20 }, /* vkDestroyFence */
    { 2835, 0xdc428e58, 67 }, /* vkDestroyFramebuffer */
    { 2856, 0xcbfb1d96, 41 }, /* vkDestroyImage */
    { 2871, 0xb5853953, 44 }, /* vkDestroyImageView */
    { 2890, 0x6aac68af, 53 }, /* vkDestroyPipeline */
    { 2908, 0x4112a673, 48 }, /* vkDestroyPipelineCache */
    { 2931, 0x9146f879, 55 }, /* vkDestroyPipelineLayout */
    { 2955, 0x37819a7f, 32 }, /* vkDestroyQueryPool */
    { 2974, 0x16f14324, 69 }, /* vkDestroyRenderPass */
    { 2994, 0x3b645153, 57 }, /* vkDestroySampler */
    { 3011, 0x20f261b2, 173 }, /* vkDestroySamplerYcbcrConversion */
    { 3043, 0xaaa623a3, 174 }, /* vkDestroySamplerYcbcrConversionKHR */
    { 3078, 0xcaab1faf, 25 }, /* vkDestroySemaphore */
    { 3097, 0x2d77af6e, 46 }, /* vkDestroyShaderModule */
    { 3119, 0x5a93ab74, 126 }, /* vkDestroySwapchainKHR */
    { 3141, 0xd46c5f24, 5 }, /* vkDeviceWaitIdle */
    { 3158, 0xdbb064, 139 }, /* vkDisplayPowerControlEXT */
    { 3183, 0xaffb5725, 77 }, /* vkEndCommandBuffer */
    { 3202, 0xff52f051, 10 }, /* vkFlushMappedMemoryRanges */
    { 3228, 0xb9db2b91, 75 }, /* vkFreeCommandBuffers */
    { 3249, 0x7a1347b1, 64 }, /* vkFreeDescriptorSets */
    { 3270, 0x8f6f838a, 7 }, /* vkFreeMemory */
    { 3283, 0xb891b5e, 200 }, /* vkGetAndroidHardwareBufferPropertiesANDROID */
    { 3327, 0x7022f0cd, 220 }, /* vkGetBufferDeviceAddress */
    { 3352, 0x3703280c, 222 }, /* vkGetBufferDeviceAddressEXT */
    { 3380, 0x713b5180, 221 }, /* vkGetBufferDeviceAddressKHR */
    { 3408, 0xab98422a, 13 }, /* vkGetBufferMemoryRequirements */
    { 3438, 0xd1fd0638, 165 }, /* vkGetBufferMemoryRequirements2 */
    { 3469, 0x78dbe98d, 166 }, /* vkGetBufferMemoryRequirements2KHR */
    { 3503, 0x2a5545a0, 218 }, /* vkGetBufferOpaqueCaptureAddress */
    { 3535, 0xddac1c65, 219 }, /* vkGetBufferOpaqueCaptureAddressKHR */
    { 3570, 0xcf3070fe, 183 }, /* vkGetCalibratedTimestampsEXT */
    { 3599, 0xfeac9573, 176 }, /* vkGetDescriptorSetLayoutSupport */
    { 3631, 0xd7e44a, 177 }, /* vkGetDescriptorSetLayoutSupportKHR */
    { 3666, 0x2e218c10, 143 }, /* vkGetDeviceGroupPeerMemoryFeatures */
    { 3701, 0xa3809375, 144 }, /* vkGetDeviceGroupPeerMemoryFeaturesKHR */
    { 3739, 0xf72c87d4, 151 }, /* vkGetDeviceGroupPresentCapabilitiesKHR */
    { 3778, 0x6b9448c3, 152 }, /* vkGetDeviceGroupSurfacePresentModesKHR */
    { 3817, 0x46e38db5, 12 }, /* vkGetDeviceMemoryCommitment */
    { 3845, 0x9a0fe777, 223 }, /* vkGetDeviceMemoryOpaqueCaptureAddress */
    { 3883, 0x49339be6, 224 }, /* vkGetDeviceMemoryOpaqueCaptureAddressKHR */
    { 3924, 0xba013486, 0 }, /* vkGetDeviceProcAddr */
    { 3944, 0xcc920d9a, 2 }, /* vkGetDeviceQueue */
    { 3961, 0xb11a6348, 175 }, /* vkGetDeviceQueue2 */
    { 3979, 0x96d834b, 28 }, /* vkGetEventStatus */
    { 3996, 0x69a5d6af, 137 }, /* vkGetFenceFdKHR */
    { 4012, 0x5f391892, 22 }, /* vkGetFenceStatus */
    { 4029, 0x916f1e63, 15 }, /* vkGetImageMemoryRequirements */
    { 4058, 0x56e213f7, 167 }, /* vkGetImageMemoryRequirements2 */
    { 4088, 0x8de28366, 168 }, /* vkGetImageMemoryRequirements2KHR */
    { 4121, 0x15855f5b, 17 }, /* vkGetImageSparseMemoryRequirements */
    { 4156, 0xbd4e3d3f, 169 }, /* vkGetImageSparseMemoryRequirements2 */
    { 4192, 0x3df40f5e, 170 }, /* vkGetImageSparseMemoryRequirements2KHR */
    { 4231, 0x9163b686, 42 }, /* vkGetImageSubresourceLayout */
    { 4259, 0x71220e82, 201 }, /* vkGetMemoryAndroidHardwareBufferANDROID */
    { 4299, 0x503c14c5, 133 }, /* vkGetMemoryFdKHR */
    { 4316, 0xb028a792, 134 }, /* vkGetMemoryFdPropertiesKHR */
    { 4343, 0x7030ee5b, 184 }, /* vkGetMemoryHostPointerPropertiesEXT */
    { 4379, 0x2092a349, 49 }, /* vkGetPipelineCacheData */
    { 4402, 0x8b20fc09, 227 }, /* vkGetPipelineExecutableInternalRepresentationsKHR */
    { 4452, 0x748dd8cd, 225 }, /* vkGetPipelineExecutablePropertiesKHR */
    { 4489, 0x5c4d6435, 226 }, /* vkGetPipelineExecutableStatisticsKHR */
    { 4526, 0xbf3f2cb3, 33 }, /* vkGetQueryPoolResults */
    { 4548, 0x3b54d93a, 217 }, /* vkGetRayTracingShaderGroupHandlesNV */
    { 4584, 0xa9820d22, 70 }, /* vkGetRenderAreaGranularity */
    { 4611, 0xd05a61a0, 194 }, /* vkGetSemaphoreCounterValue */
    { 4638, 0xf3c26065, 195 }, /* vkGetSemaphoreCounterValueKHR */
    { 4668, 0x3e0e9884, 135 }, /* vkGetSemaphoreFdKHR */
    { 4688, 0x5330743c, 182 }, /* vkGetShaderInfoAMD */
    { 4707, 0xa4aeb5a, 142 }, /* vkGetSwapchainCounterEXT */
    { 4732, 0x219d929, 179 }, /* vkGetSwapchainGrallocUsage2ANDROID */
    { 4767, 0x4979c9a3, 178 }, /* vkGetSwapchainGrallocUsageANDROID */
    { 4801, 0x57695f28, 127 }, /* vkGetSwapchainImagesKHR */
    { 4825, 0x51df0390, 138 }, /* vkImportFenceFdKHR */
    { 4844, 0x36337c05, 136 }, /* vkImportSemaphoreFdKHR */
    { 4867, 0x1e115cca, 11 }, /* vkInvalidateMappedMemoryRanges */
    { 4898, 0xcb977bd8, 8 }, /* vkMapMemory */
    { 4910, 0xc3499606, 50 }, /* vkMergePipelineCaches */
    { 4932, 0xc3628a09, 18 }, /* vkQueueBindSparse */
    { 4950, 0xfc5fb6ce, 129 }, /* vkQueuePresentKHR */
    { 4968, 0xa0313eef, 181 }, /* vkQueueSignalReleaseImageANDROID */
    { 5001, 0xfa4713ec, 3 }, /* vkQueueSubmit */
    { 5015, 0x6f8fc2a5, 4 }, /* vkQueueWaitIdle */
    { 5031, 0x26cc78f5, 140 }, /* vkRegisterDeviceEventEXT */
    { 5056, 0x4a0bd849, 141 }, /* vkRegisterDisplayEventEXT */
    { 5082, 0x847dc731, 78 }, /* vkResetCommandBuffer */
    { 5103, 0x6da9f7fd, 73 }, /* vkResetCommandPool */
    { 5122, 0x9bd85f5, 62 }, /* vkResetDescriptorPool */
    { 5144, 0x6d373ba8, 30 }, /* vkResetEvent */
    { 5157, 0x684781dc, 21 }, /* vkResetFences */
    { 5171, 0x4e671e02, 34 }, /* vkResetQueryPool */
    { 5188, 0xe6701e5f, 35 }, /* vkResetQueryPoolEXT */
    { 5208, 0x592ae5f5, 29 }, /* vkSetEvent */
    { 5219, 0xcd347297, 198 }, /* vkSignalSemaphore */
    { 5237, 0x8fef55c6, 199 }, /* vkSignalSemaphoreKHR */
    { 5258, 0xfef2fb38, 131 }, /* vkTrimCommandPool */
    { 5276, 0x51177c8d, 132 }, /* vkTrimCommandPoolKHR */
    { 5297, 0x1a1a0e2f, 9 }, /* vkUnmapMemory */
    { 5311, 0x5349c9d, 160 }, /* vkUpdateDescriptorSetWithTemplate */
    { 5345, 0x214ad230, 161 }, /* vkUpdateDescriptorSetWithTemplateKHR */
    { 5382, 0xbfd090ae, 65 }, /* vkUpdateDescriptorSets */
    { 5405, 0x19d64c81, 23 }, /* vkWaitForFences */
    { 5421, 0x74368ad9, 196 }, /* vkWaitSemaphores */
    { 5438, 0x2bc77454, 197 }, /* vkWaitSemaphoresKHR */
};

/* Hash table stats:
 * size 229 entries
 * collisions entries:
 *     0      182
 *     1      28
 *     2      9
 *     3      8
 *     4      1
 *     5      0
 *     6      1
 *     7      0
 *     8      0
 *     9+     0
 */

#define none 0xffff
static const uint16_t device_string_map[512] = {
    none,
    none,
    0x00d7,
    none,
    none,
    0x0026,
    0x00ca,
    none,
    0x00d4,
    0x00b8,
    0x0076,
    none,
    0x0095,
    none,
    none,
    none,
    0x009f,
    none,
    none,
    none,
    none,
    none,
    none,
    none,
    0x006a,
    none,
    0x0007,
    0x00d9,
    0x00cb,
    none,
    0x007a,
    none,
    0x0024,
    none,
    none,
    none,
    0x0051,
    none,
    0x006e,
    none,
    0x0042,
    none,
    0x0097,
    0x0053,
    0x0010,
    none,
    none,
    0x00de,
    0x00e0,
    none,
    none,
    none,
    none,
    0x0035,
    none,
    none,
    0x0098,
    none,
    none,
    0x006c,
    0x007d,
    none,
    0x007c,
    0x002c,
    0x0039,
    none,
    none,
    none,
    none,
    0x0074,
    none,
    none,
    0x00ba,
    0x00d1,
    0x000b,
    none,
    0x0058,
    none,
    none,
    0x006f,
    0x0030,
    0x008f,
    0x0057,
    none,
    0x00e4,
    none,
    0x005a,
    none,
    0x0028,
    none,
    none,
    0x0070,
    none,
    0x009e,
    none,
    0x00d8,
    none,
    none,
    0x00c1,
    0x0047,
    0x008d,
    0x006d,
    none,
    0x0023,
    0x003f,
    none,
    none,
    0x007e,
    none,
    none,
    0x00b6,
    none,
    0x0016,
    none,
    0x004e,
    0x0082,
    0x005e,
    0x004b,
    0x005b,
    0x00c7,
    0x0037,
    0x0083,
    none,
    none,
    none,
    none,
    0x001f,
    0x004f,
    none,
    0x00e2,
    0x0043,
    none,
    0x00c0,
    none,
    0x00a6,
    0x002b,
    none,
    0x002e,
    0x0034,
    0x004a,
    none,
    0x00dd,
    none,
    none,
    none,
    none,
    0x0079,
    none,
    0x0014,
    0x00b3,
    none,
    0x00da,
    none,
    0x00b2,
    0x0048,
    none,
    0x00ac,
    0x0060,
    0x009b,
    0x0067,
    none,
    0x0040,
    0x0071,
    none,
    0x000f,
    0x0084,
    none,
    none,
    0x0072,
    none,
    none,
    none,
    none,
    0x0056,
    0x00e1,
    0x0081,
    0x00df,
    0x00bf,
    none,
    0x0055,
    none,
    none,
    none,
    0x005d,
    0x001b,
    0x0059,
    0x001d,
    none,
    none,
    none,
    none,
    none,
    none,
    none,
    0x0065,
    0x00a2,
    0x0063,
    0x00b4,
    0x00bb,
    none,
    none,
    none,
    0x00c8,
    0x00ab,
    none,
    0x0094,
    0x00cc,
    none,
    none,
    none,
    none,
    none,
    0x0022,
    0x00aa,
    none,
    none,
    none,
    0x00e3,
    0x001e,
    none,
    none,
    0x0000,
    0x00cf,
    none,
    0x003a,
    none,
    0x0050,
    0x0011,
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
    0x00cd,
    none,
    none,
    none,
    0x00b9,
    none,
    0x00d0,
    none,
    none,
    none,
    0x003b,
    none,
    none,
    none,
    none,
    0x009c,
    none,
    none,
    none,
    none,
    none,
    none,
    none,
    0x0038,
    0x0046,
    0x001c,
    none,
    none,
    none,
    0x004c,
    none,
    none,
    none,
    none,
    none,
    none,
    none,
    none,
    0x0036,
    none,
    none,
    0x000c,
    none,
    0x0003,
    0x0078,
    none,
    0x0019,
    none,
    none,
    none,
    none,
    0x0033,
    none,
    0x0085,
    0x008e,
    0x0073,
    0x0006,
    0x00c5,
    0x00c3,
    none,
    0x0069,
    none,
    none,
    0x0002,
    none,
    0x0077,
    0x003c,
    none,
    0x005f,
    0x0052,
    0x00bd,
    0x0029,
    0x008c,
    0x00dc,
    none,
    0x0004,
    none,
    none,
    none,
    none,
    0x00b0,
    none,
    0x002a,
    none,
    none,
    0x0061,
    none,
    none,
    none,
    0x00a8,
    0x00b7,
    none,
    0x00a9,
    none,
    0x00bc,
    none,
    0x0009,
    none,
    none,
    none,
    0x0080,
    none,
    none,
    0x0049,
    0x00d2,
    none,
    none,
    0x006b,
    0x00af,
    none,
    none,
    0x0054,
    none,
    none,
    none,
    0x0075,
    none,
    none,
    0x004d,
    0x0086,
    none,
    0x0062,
    none,
    none,
    none,
    none,
    0x0064,
    0x008a,
    none,
    none,
    0x0093,
    0x0001,
    0x009d,
    0x008b,
    0x00a0,
    none,
    0x0012,
    none,
    0x00ae,
    none,
    none,
    none,
    0x0032,
    none,
    none,
    0x0096,
    0x0041,
    none,
    none,
    0x0017,
    none,
    none,
    none,
    none,
    0x001a,
    0x0092,
    none,
    none,
    0x0099,
    none,
    none,
    0x002f,
    0x0090,
    0x00b5,
    0x00c2,
    0x0044,
    none,
    0x007f,
    0x00b1,
    none,
    none,
    0x00a7,
    none,
    none,
    0x00a4,
    none,
    0x000e,
    0x009a,
    0x0027,
    none,
    0x0088,
    none,
    none,
    none,
    none,
    0x00d5,
    0x0020,
    none,
    none,
    0x0021,
    0x000d,
    none,
    0x0089,
    none,
    0x0091,
    0x0087,
    0x00be,
    none,
    0x00a3,
    0x00c4,
    none,
    none,
    none,
    none,
    none,
    0x003d,
    none,
    none,
    0x002d,
    none,
    none,
    none,
    none,
    none,
    none,
    0x00db,
    none,
    none,
    0x00c6,
    none,
    0x0008,
    none,
    0x0018,
    0x000a,
    none,
    none,
    none,
    none,
    0x0005,
    0x00a1,
    none,
    none,
    none,
    0x00c9,
    none,
    0x0015,
    none,
    0x00d6,
    none,
    0x007b,
    none,
    none,
    none,
    none,
    none,
    none,
    0x0025,
    0x0013,
    none,
    0x003e,
    none,
    none,
    0x005c,
    0x00ce,
    0x0068,
    none,
    none,
    none,
    none,
    none,
    none,
    none,
    0x0031,
    none,
    0x00ad,
    none,
    0x00a5,
    none,
    0x0045,
    none,
    0x00d3,
    none,
    0x0066,
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

static const char *
device_entry_name(int num)
{
   for (int i = 0; i < ARRAY_SIZE(device_string_map_entries); i++) {
      if (device_string_map_entries[i].num == num)
         return &device_strings[device_string_map_entries[i].name];
   }
   return NULL;
}


/* Weak aliases for all potential implementations. These will resolve to
 * NULL if they're not defined, which lets the resolve_entrypoint() function
 * either pick the correct entry point.
 */

  VkResult radv_CreateInstance(const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance) __attribute__ ((weak));
  void radv_DestroyInstance(VkInstance instance, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
  VkResult radv_EnumeratePhysicalDevices(VkInstance instance, uint32_t* pPhysicalDeviceCount, VkPhysicalDevice* pPhysicalDevices) __attribute__ ((weak));
  PFN_vkVoidFunction radv_GetInstanceProcAddr(VkInstance instance, const char* pName) __attribute__ ((weak));
  VkResult radv_EnumerateInstanceVersion(uint32_t* pApiVersion) __attribute__ ((weak));
  VkResult radv_EnumerateInstanceLayerProperties(uint32_t* pPropertyCount, VkLayerProperties* pProperties) __attribute__ ((weak));
  VkResult radv_EnumerateInstanceExtensionProperties(const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties) __attribute__ ((weak));
  VkResult radv_CreateDisplayPlaneSurfaceKHR(VkInstance instance, const VkDisplaySurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) __attribute__ ((weak));
  void radv_DestroySurfaceKHR(VkInstance instance, VkSurfaceKHR surface, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
  VkResult radv_CreateWaylandSurfaceKHR(VkInstance instance, const VkWaylandSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_WAYLAND_KHR
#ifdef VK_USE_PLATFORM_XLIB_KHR
  VkResult radv_CreateXlibSurfaceKHR(VkInstance instance, const VkXlibSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_XLIB_KHR
#ifdef VK_USE_PLATFORM_XCB_KHR
  VkResult radv_CreateXcbSurfaceKHR(VkInstance instance, const VkXcbSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_XCB_KHR
  VkResult radv_CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback) __attribute__ ((weak));
  void radv_DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
  void radv_DebugReportMessageEXT(VkInstance instance, VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage) __attribute__ ((weak));
  VkResult radv_EnumeratePhysicalDeviceGroups(VkInstance instance, uint32_t* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties) __attribute__ ((weak));
    
const struct radv_instance_dispatch_table radv_instance_dispatch_table = {
  .vkCreateInstance = radv_CreateInstance,
  .vkDestroyInstance = radv_DestroyInstance,
  .vkEnumeratePhysicalDevices = radv_EnumeratePhysicalDevices,
  .vkGetInstanceProcAddr = radv_GetInstanceProcAddr,
  .vkEnumerateInstanceVersion = radv_EnumerateInstanceVersion,
  .vkEnumerateInstanceLayerProperties = radv_EnumerateInstanceLayerProperties,
  .vkEnumerateInstanceExtensionProperties = radv_EnumerateInstanceExtensionProperties,
  .vkCreateDisplayPlaneSurfaceKHR = radv_CreateDisplayPlaneSurfaceKHR,
  .vkDestroySurfaceKHR = radv_DestroySurfaceKHR,
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
  .vkCreateWaylandSurfaceKHR = radv_CreateWaylandSurfaceKHR,
#endif // VK_USE_PLATFORM_WAYLAND_KHR
#ifdef VK_USE_PLATFORM_XLIB_KHR
  .vkCreateXlibSurfaceKHR = radv_CreateXlibSurfaceKHR,
#endif // VK_USE_PLATFORM_XLIB_KHR
#ifdef VK_USE_PLATFORM_XCB_KHR
  .vkCreateXcbSurfaceKHR = radv_CreateXcbSurfaceKHR,
#endif // VK_USE_PLATFORM_XCB_KHR
  .vkCreateDebugReportCallbackEXT = radv_CreateDebugReportCallbackEXT,
  .vkDestroyDebugReportCallbackEXT = radv_DestroyDebugReportCallbackEXT,
  .vkDebugReportMessageEXT = radv_DebugReportMessageEXT,
  .vkEnumeratePhysicalDeviceGroups = radv_EnumeratePhysicalDeviceGroups,
  .vkEnumeratePhysicalDeviceGroupsKHR = radv_EnumeratePhysicalDeviceGroups,
};

  void radv_GetPhysicalDeviceProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties* pProperties) __attribute__ ((weak));
  void radv_GetPhysicalDeviceQueueFamilyProperties(VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties* pQueueFamilyProperties) __attribute__ ((weak));
  void radv_GetPhysicalDeviceMemoryProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties* pMemoryProperties) __attribute__ ((weak));
  void radv_GetPhysicalDeviceFeatures(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures* pFeatures) __attribute__ ((weak));
  void radv_GetPhysicalDeviceFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties* pFormatProperties) __attribute__ ((weak));
  VkResult radv_GetPhysicalDeviceImageFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkImageFormatProperties* pImageFormatProperties) __attribute__ ((weak));
  VkResult radv_CreateDevice(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDevice* pDevice) __attribute__ ((weak));
  VkResult radv_EnumerateDeviceLayerProperties(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkLayerProperties* pProperties) __attribute__ ((weak));
  VkResult radv_EnumerateDeviceExtensionProperties(VkPhysicalDevice physicalDevice, const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties) __attribute__ ((weak));
  void radv_GetPhysicalDeviceSparseImageFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkSampleCountFlagBits samples, VkImageUsageFlags usage, VkImageTiling tiling, uint32_t* pPropertyCount, VkSparseImageFormatProperties* pProperties) __attribute__ ((weak));
  VkResult radv_GetPhysicalDeviceDisplayPropertiesKHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPropertiesKHR* pProperties) __attribute__ ((weak));
  VkResult radv_GetPhysicalDeviceDisplayPlanePropertiesKHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPlanePropertiesKHR* pProperties) __attribute__ ((weak));
  VkResult radv_GetDisplayPlaneSupportedDisplaysKHR(VkPhysicalDevice physicalDevice, uint32_t planeIndex, uint32_t* pDisplayCount, VkDisplayKHR* pDisplays) __attribute__ ((weak));
  VkResult radv_GetDisplayModePropertiesKHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModePropertiesKHR* pProperties) __attribute__ ((weak));
  VkResult radv_CreateDisplayModeKHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, const VkDisplayModeCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDisplayModeKHR* pMode) __attribute__ ((weak));
  VkResult radv_GetDisplayPlaneCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkDisplayModeKHR mode, uint32_t planeIndex, VkDisplayPlaneCapabilitiesKHR* pCapabilities) __attribute__ ((weak));
  VkResult radv_GetPhysicalDeviceSurfaceSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported) __attribute__ ((weak));
  VkResult radv_GetPhysicalDeviceSurfaceCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR* pSurfaceCapabilities) __attribute__ ((weak));
  VkResult radv_GetPhysicalDeviceSurfaceFormatsKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pSurfaceFormatCount, VkSurfaceFormatKHR* pSurfaceFormats) __attribute__ ((weak));
  VkResult radv_GetPhysicalDeviceSurfacePresentModesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes) __attribute__ ((weak));
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
  VkBool32 radv_GetPhysicalDeviceWaylandPresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, struct wl_display* display) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_WAYLAND_KHR
#ifdef VK_USE_PLATFORM_XLIB_KHR
  VkBool32 radv_GetPhysicalDeviceXlibPresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, Display* dpy, VisualID visualID) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_XLIB_KHR
#ifdef VK_USE_PLATFORM_XCB_KHR
  VkBool32 radv_GetPhysicalDeviceXcbPresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, xcb_connection_t* connection, xcb_visualid_t visual_id) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_XCB_KHR
  void radv_GetPhysicalDeviceFeatures2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures) __attribute__ ((weak));
      void radv_GetPhysicalDeviceProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties) __attribute__ ((weak));
      void radv_GetPhysicalDeviceFormatProperties2(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2* pFormatProperties) __attribute__ ((weak));
      VkResult radv_GetPhysicalDeviceImageFormatProperties2(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties) __attribute__ ((weak));
      void radv_GetPhysicalDeviceQueueFamilyProperties2(VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties2* pQueueFamilyProperties) __attribute__ ((weak));
      void radv_GetPhysicalDeviceMemoryProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties) __attribute__ ((weak));
      void radv_GetPhysicalDeviceSparseImageFormatProperties2(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo, uint32_t* pPropertyCount, VkSparseImageFormatProperties2* pProperties) __attribute__ ((weak));
      void radv_GetPhysicalDeviceExternalBufferProperties(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties) __attribute__ ((weak));
      void radv_GetPhysicalDeviceExternalSemaphoreProperties(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties) __attribute__ ((weak));
      void radv_GetPhysicalDeviceExternalFenceProperties(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties) __attribute__ ((weak));
      VkResult radv_ReleaseDisplayEXT(VkPhysicalDevice physicalDevice, VkDisplayKHR display) __attribute__ ((weak));
#ifdef VK_USE_PLATFORM_XLIB_XRANDR_EXT
  VkResult radv_AcquireXlibDisplayEXT(VkPhysicalDevice physicalDevice, Display* dpy, VkDisplayKHR display) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_XLIB_XRANDR_EXT
#ifdef VK_USE_PLATFORM_XLIB_XRANDR_EXT
  VkResult radv_GetRandROutputDisplayEXT(VkPhysicalDevice physicalDevice, Display* dpy, RROutput rrOutput, VkDisplayKHR* pDisplay) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_XLIB_XRANDR_EXT
  VkResult radv_GetPhysicalDeviceSurfaceCapabilities2EXT(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilities2EXT* pSurfaceCapabilities) __attribute__ ((weak));
  VkResult radv_GetPhysicalDevicePresentRectanglesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pRectCount, VkRect2D* pRects) __attribute__ ((weak));
  void radv_GetPhysicalDeviceMultisamplePropertiesEXT(VkPhysicalDevice physicalDevice, VkSampleCountFlagBits samples, VkMultisamplePropertiesEXT* pMultisampleProperties) __attribute__ ((weak));
  VkResult radv_GetPhysicalDeviceSurfaceCapabilities2KHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkSurfaceCapabilities2KHR* pSurfaceCapabilities) __attribute__ ((weak));
  VkResult radv_GetPhysicalDeviceSurfaceFormats2KHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pSurfaceFormatCount, VkSurfaceFormat2KHR* pSurfaceFormats) __attribute__ ((weak));
  VkResult radv_GetPhysicalDeviceDisplayProperties2KHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayProperties2KHR* pProperties) __attribute__ ((weak));
  VkResult radv_GetPhysicalDeviceDisplayPlaneProperties2KHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPlaneProperties2KHR* pProperties) __attribute__ ((weak));
  VkResult radv_GetDisplayModeProperties2KHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModeProperties2KHR* pProperties) __attribute__ ((weak));
  VkResult radv_GetDisplayPlaneCapabilities2KHR(VkPhysicalDevice physicalDevice, const VkDisplayPlaneInfo2KHR* pDisplayPlaneInfo, VkDisplayPlaneCapabilities2KHR* pCapabilities) __attribute__ ((weak));
  VkResult radv_GetPhysicalDeviceCalibrateableTimeDomainsEXT(VkPhysicalDevice physicalDevice, uint32_t* pTimeDomainCount, VkTimeDomainEXT* pTimeDomains) __attribute__ ((weak));

const struct radv_physical_device_dispatch_table radv_physical_device_dispatch_table = {
  .vkGetPhysicalDeviceProperties = radv_GetPhysicalDeviceProperties,
  .vkGetPhysicalDeviceQueueFamilyProperties = radv_GetPhysicalDeviceQueueFamilyProperties,
  .vkGetPhysicalDeviceMemoryProperties = radv_GetPhysicalDeviceMemoryProperties,
  .vkGetPhysicalDeviceFeatures = radv_GetPhysicalDeviceFeatures,
  .vkGetPhysicalDeviceFormatProperties = radv_GetPhysicalDeviceFormatProperties,
  .vkGetPhysicalDeviceImageFormatProperties = radv_GetPhysicalDeviceImageFormatProperties,
  .vkCreateDevice = radv_CreateDevice,
  .vkEnumerateDeviceLayerProperties = radv_EnumerateDeviceLayerProperties,
  .vkEnumerateDeviceExtensionProperties = radv_EnumerateDeviceExtensionProperties,
  .vkGetPhysicalDeviceSparseImageFormatProperties = radv_GetPhysicalDeviceSparseImageFormatProperties,
  .vkGetPhysicalDeviceDisplayPropertiesKHR = radv_GetPhysicalDeviceDisplayPropertiesKHR,
  .vkGetPhysicalDeviceDisplayPlanePropertiesKHR = radv_GetPhysicalDeviceDisplayPlanePropertiesKHR,
  .vkGetDisplayPlaneSupportedDisplaysKHR = radv_GetDisplayPlaneSupportedDisplaysKHR,
  .vkGetDisplayModePropertiesKHR = radv_GetDisplayModePropertiesKHR,
  .vkCreateDisplayModeKHR = radv_CreateDisplayModeKHR,
  .vkGetDisplayPlaneCapabilitiesKHR = radv_GetDisplayPlaneCapabilitiesKHR,
  .vkGetPhysicalDeviceSurfaceSupportKHR = radv_GetPhysicalDeviceSurfaceSupportKHR,
  .vkGetPhysicalDeviceSurfaceCapabilitiesKHR = radv_GetPhysicalDeviceSurfaceCapabilitiesKHR,
  .vkGetPhysicalDeviceSurfaceFormatsKHR = radv_GetPhysicalDeviceSurfaceFormatsKHR,
  .vkGetPhysicalDeviceSurfacePresentModesKHR = radv_GetPhysicalDeviceSurfacePresentModesKHR,
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
  .vkGetPhysicalDeviceWaylandPresentationSupportKHR = radv_GetPhysicalDeviceWaylandPresentationSupportKHR,
#endif // VK_USE_PLATFORM_WAYLAND_KHR
#ifdef VK_USE_PLATFORM_XLIB_KHR
  .vkGetPhysicalDeviceXlibPresentationSupportKHR = radv_GetPhysicalDeviceXlibPresentationSupportKHR,
#endif // VK_USE_PLATFORM_XLIB_KHR
#ifdef VK_USE_PLATFORM_XCB_KHR
  .vkGetPhysicalDeviceXcbPresentationSupportKHR = radv_GetPhysicalDeviceXcbPresentationSupportKHR,
#endif // VK_USE_PLATFORM_XCB_KHR
  .vkGetPhysicalDeviceFeatures2 = radv_GetPhysicalDeviceFeatures2,
  .vkGetPhysicalDeviceFeatures2KHR = radv_GetPhysicalDeviceFeatures2,
  .vkGetPhysicalDeviceProperties2 = radv_GetPhysicalDeviceProperties2,
  .vkGetPhysicalDeviceProperties2KHR = radv_GetPhysicalDeviceProperties2,
  .vkGetPhysicalDeviceFormatProperties2 = radv_GetPhysicalDeviceFormatProperties2,
  .vkGetPhysicalDeviceFormatProperties2KHR = radv_GetPhysicalDeviceFormatProperties2,
  .vkGetPhysicalDeviceImageFormatProperties2 = radv_GetPhysicalDeviceImageFormatProperties2,
  .vkGetPhysicalDeviceImageFormatProperties2KHR = radv_GetPhysicalDeviceImageFormatProperties2,
  .vkGetPhysicalDeviceQueueFamilyProperties2 = radv_GetPhysicalDeviceQueueFamilyProperties2,
  .vkGetPhysicalDeviceQueueFamilyProperties2KHR = radv_GetPhysicalDeviceQueueFamilyProperties2,
  .vkGetPhysicalDeviceMemoryProperties2 = radv_GetPhysicalDeviceMemoryProperties2,
  .vkGetPhysicalDeviceMemoryProperties2KHR = radv_GetPhysicalDeviceMemoryProperties2,
  .vkGetPhysicalDeviceSparseImageFormatProperties2 = radv_GetPhysicalDeviceSparseImageFormatProperties2,
  .vkGetPhysicalDeviceSparseImageFormatProperties2KHR = radv_GetPhysicalDeviceSparseImageFormatProperties2,
  .vkGetPhysicalDeviceExternalBufferProperties = radv_GetPhysicalDeviceExternalBufferProperties,
  .vkGetPhysicalDeviceExternalBufferPropertiesKHR = radv_GetPhysicalDeviceExternalBufferProperties,
  .vkGetPhysicalDeviceExternalSemaphoreProperties = radv_GetPhysicalDeviceExternalSemaphoreProperties,
  .vkGetPhysicalDeviceExternalSemaphorePropertiesKHR = radv_GetPhysicalDeviceExternalSemaphoreProperties,
  .vkGetPhysicalDeviceExternalFenceProperties = radv_GetPhysicalDeviceExternalFenceProperties,
  .vkGetPhysicalDeviceExternalFencePropertiesKHR = radv_GetPhysicalDeviceExternalFenceProperties,
  .vkReleaseDisplayEXT = radv_ReleaseDisplayEXT,
#ifdef VK_USE_PLATFORM_XLIB_XRANDR_EXT
  .vkAcquireXlibDisplayEXT = radv_AcquireXlibDisplayEXT,
#endif // VK_USE_PLATFORM_XLIB_XRANDR_EXT
#ifdef VK_USE_PLATFORM_XLIB_XRANDR_EXT
  .vkGetRandROutputDisplayEXT = radv_GetRandROutputDisplayEXT,
#endif // VK_USE_PLATFORM_XLIB_XRANDR_EXT
  .vkGetPhysicalDeviceSurfaceCapabilities2EXT = radv_GetPhysicalDeviceSurfaceCapabilities2EXT,
  .vkGetPhysicalDevicePresentRectanglesKHR = radv_GetPhysicalDevicePresentRectanglesKHR,
  .vkGetPhysicalDeviceMultisamplePropertiesEXT = radv_GetPhysicalDeviceMultisamplePropertiesEXT,
  .vkGetPhysicalDeviceSurfaceCapabilities2KHR = radv_GetPhysicalDeviceSurfaceCapabilities2KHR,
  .vkGetPhysicalDeviceSurfaceFormats2KHR = radv_GetPhysicalDeviceSurfaceFormats2KHR,
  .vkGetPhysicalDeviceDisplayProperties2KHR = radv_GetPhysicalDeviceDisplayProperties2KHR,
  .vkGetPhysicalDeviceDisplayPlaneProperties2KHR = radv_GetPhysicalDeviceDisplayPlaneProperties2KHR,
  .vkGetDisplayModeProperties2KHR = radv_GetDisplayModeProperties2KHR,
  .vkGetDisplayPlaneCapabilities2KHR = radv_GetDisplayPlaneCapabilities2KHR,
  .vkGetPhysicalDeviceCalibrateableTimeDomainsEXT = radv_GetPhysicalDeviceCalibrateableTimeDomainsEXT,
};


      PFN_vkVoidFunction __attribute__ ((weak))
      radv_GetDeviceProcAddr(VkDevice device, const char* pName)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkGetDeviceProcAddr(device, pName);
      }
      void __attribute__ ((weak))
      radv_DestroyDevice(VkDevice device, const VkAllocationCallbacks* pAllocator)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkDestroyDevice(device, pAllocator);
      }
      void __attribute__ ((weak))
      radv_GetDeviceQueue(VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkGetDeviceQueue(device, queueFamilyIndex, queueIndex, pQueue);
      }
      VkResult __attribute__ ((weak))
      radv_QueueSubmit(VkQueue queue, uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence)
      {
          RADV_FROM_HANDLE(radv_queue, radv_queue, queue);
          return radv_queue->device->dispatch.vkQueueSubmit(queue, submitCount, pSubmits, fence);
      }
      VkResult __attribute__ ((weak))
      radv_QueueWaitIdle(VkQueue queue)
      {
          RADV_FROM_HANDLE(radv_queue, radv_queue, queue);
          return radv_queue->device->dispatch.vkQueueWaitIdle(queue);
      }
      VkResult __attribute__ ((weak))
      radv_DeviceWaitIdle(VkDevice device)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkDeviceWaitIdle(device);
      }
      VkResult __attribute__ ((weak))
      radv_AllocateMemory(VkDevice device, const VkMemoryAllocateInfo* pAllocateInfo, const VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkAllocateMemory(device, pAllocateInfo, pAllocator, pMemory);
      }
      void __attribute__ ((weak))
      radv_FreeMemory(VkDevice device, VkDeviceMemory memory, const VkAllocationCallbacks* pAllocator)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkFreeMemory(device, memory, pAllocator);
      }
      VkResult __attribute__ ((weak))
      radv_MapMemory(VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkMapMemory(device, memory, offset, size, flags, ppData);
      }
      void __attribute__ ((weak))
      radv_UnmapMemory(VkDevice device, VkDeviceMemory memory)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkUnmapMemory(device, memory);
      }
      VkResult __attribute__ ((weak))
      radv_FlushMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkFlushMappedMemoryRanges(device, memoryRangeCount, pMemoryRanges);
      }
      VkResult __attribute__ ((weak))
      radv_InvalidateMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkInvalidateMappedMemoryRanges(device, memoryRangeCount, pMemoryRanges);
      }
      void __attribute__ ((weak))
      radv_GetDeviceMemoryCommitment(VkDevice device, VkDeviceMemory memory, VkDeviceSize* pCommittedMemoryInBytes)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkGetDeviceMemoryCommitment(device, memory, pCommittedMemoryInBytes);
      }
      void __attribute__ ((weak))
      radv_GetBufferMemoryRequirements(VkDevice device, VkBuffer buffer, VkMemoryRequirements* pMemoryRequirements)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkGetBufferMemoryRequirements(device, buffer, pMemoryRequirements);
      }
      VkResult __attribute__ ((weak))
      radv_BindBufferMemory(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkBindBufferMemory(device, buffer, memory, memoryOffset);
      }
      void __attribute__ ((weak))
      radv_GetImageMemoryRequirements(VkDevice device, VkImage image, VkMemoryRequirements* pMemoryRequirements)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkGetImageMemoryRequirements(device, image, pMemoryRequirements);
      }
      VkResult __attribute__ ((weak))
      radv_BindImageMemory(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkBindImageMemory(device, image, memory, memoryOffset);
      }
      void __attribute__ ((weak))
      radv_GetImageSparseMemoryRequirements(VkDevice device, VkImage image, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements* pSparseMemoryRequirements)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkGetImageSparseMemoryRequirements(device, image, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
      }
      VkResult __attribute__ ((weak))
      radv_QueueBindSparse(VkQueue queue, uint32_t bindInfoCount, const VkBindSparseInfo* pBindInfo, VkFence fence)
      {
          RADV_FROM_HANDLE(radv_queue, radv_queue, queue);
          return radv_queue->device->dispatch.vkQueueBindSparse(queue, bindInfoCount, pBindInfo, fence);
      }
      VkResult __attribute__ ((weak))
      radv_CreateFence(VkDevice device, const VkFenceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkCreateFence(device, pCreateInfo, pAllocator, pFence);
      }
      void __attribute__ ((weak))
      radv_DestroyFence(VkDevice device, VkFence fence, const VkAllocationCallbacks* pAllocator)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkDestroyFence(device, fence, pAllocator);
      }
      VkResult __attribute__ ((weak))
      radv_ResetFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkResetFences(device, fenceCount, pFences);
      }
      VkResult __attribute__ ((weak))
      radv_GetFenceStatus(VkDevice device, VkFence fence)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkGetFenceStatus(device, fence);
      }
      VkResult __attribute__ ((weak))
      radv_WaitForFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences, VkBool32 waitAll, uint64_t timeout)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkWaitForFences(device, fenceCount, pFences, waitAll, timeout);
      }
      VkResult __attribute__ ((weak))
      radv_CreateSemaphore(VkDevice device, const VkSemaphoreCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkCreateSemaphore(device, pCreateInfo, pAllocator, pSemaphore);
      }
      void __attribute__ ((weak))
      radv_DestroySemaphore(VkDevice device, VkSemaphore semaphore, const VkAllocationCallbacks* pAllocator)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkDestroySemaphore(device, semaphore, pAllocator);
      }
      VkResult __attribute__ ((weak))
      radv_CreateEvent(VkDevice device, const VkEventCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkEvent* pEvent)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkCreateEvent(device, pCreateInfo, pAllocator, pEvent);
      }
      void __attribute__ ((weak))
      radv_DestroyEvent(VkDevice device, VkEvent event, const VkAllocationCallbacks* pAllocator)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkDestroyEvent(device, event, pAllocator);
      }
      VkResult __attribute__ ((weak))
      radv_GetEventStatus(VkDevice device, VkEvent event)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkGetEventStatus(device, event);
      }
      VkResult __attribute__ ((weak))
      radv_SetEvent(VkDevice device, VkEvent event)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkSetEvent(device, event);
      }
      VkResult __attribute__ ((weak))
      radv_ResetEvent(VkDevice device, VkEvent event)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkResetEvent(device, event);
      }
      VkResult __attribute__ ((weak))
      radv_CreateQueryPool(VkDevice device, const VkQueryPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkQueryPool* pQueryPool)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkCreateQueryPool(device, pCreateInfo, pAllocator, pQueryPool);
      }
      void __attribute__ ((weak))
      radv_DestroyQueryPool(VkDevice device, VkQueryPool queryPool, const VkAllocationCallbacks* pAllocator)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkDestroyQueryPool(device, queryPool, pAllocator);
      }
      VkResult __attribute__ ((weak))
      radv_GetQueryPoolResults(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, VkDeviceSize stride, VkQueryResultFlags flags)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkGetQueryPoolResults(device, queryPool, firstQuery, queryCount, dataSize, pData, stride, flags);
      }
      void __attribute__ ((weak))
      radv_ResetQueryPool(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkResetQueryPool(device, queryPool, firstQuery, queryCount);
      }
            VkResult __attribute__ ((weak))
      radv_CreateBuffer(VkDevice device, const VkBufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkCreateBuffer(device, pCreateInfo, pAllocator, pBuffer);
      }
      void __attribute__ ((weak))
      radv_DestroyBuffer(VkDevice device, VkBuffer buffer, const VkAllocationCallbacks* pAllocator)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkDestroyBuffer(device, buffer, pAllocator);
      }
      VkResult __attribute__ ((weak))
      radv_CreateBufferView(VkDevice device, const VkBufferViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferView* pView)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkCreateBufferView(device, pCreateInfo, pAllocator, pView);
      }
      void __attribute__ ((weak))
      radv_DestroyBufferView(VkDevice device, VkBufferView bufferView, const VkAllocationCallbacks* pAllocator)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkDestroyBufferView(device, bufferView, pAllocator);
      }
      VkResult __attribute__ ((weak))
      radv_CreateImage(VkDevice device, const VkImageCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImage* pImage)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkCreateImage(device, pCreateInfo, pAllocator, pImage);
      }
      void __attribute__ ((weak))
      radv_DestroyImage(VkDevice device, VkImage image, const VkAllocationCallbacks* pAllocator)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkDestroyImage(device, image, pAllocator);
      }
      void __attribute__ ((weak))
      radv_GetImageSubresourceLayout(VkDevice device, VkImage image, const VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkGetImageSubresourceLayout(device, image, pSubresource, pLayout);
      }
      VkResult __attribute__ ((weak))
      radv_CreateImageView(VkDevice device, const VkImageViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImageView* pView)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkCreateImageView(device, pCreateInfo, pAllocator, pView);
      }
      void __attribute__ ((weak))
      radv_DestroyImageView(VkDevice device, VkImageView imageView, const VkAllocationCallbacks* pAllocator)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkDestroyImageView(device, imageView, pAllocator);
      }
      VkResult __attribute__ ((weak))
      radv_CreateShaderModule(VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkCreateShaderModule(device, pCreateInfo, pAllocator, pShaderModule);
      }
      void __attribute__ ((weak))
      radv_DestroyShaderModule(VkDevice device, VkShaderModule shaderModule, const VkAllocationCallbacks* pAllocator)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkDestroyShaderModule(device, shaderModule, pAllocator);
      }
      VkResult __attribute__ ((weak))
      radv_CreatePipelineCache(VkDevice device, const VkPipelineCacheCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineCache* pPipelineCache)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkCreatePipelineCache(device, pCreateInfo, pAllocator, pPipelineCache);
      }
      void __attribute__ ((weak))
      radv_DestroyPipelineCache(VkDevice device, VkPipelineCache pipelineCache, const VkAllocationCallbacks* pAllocator)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkDestroyPipelineCache(device, pipelineCache, pAllocator);
      }
      VkResult __attribute__ ((weak))
      radv_GetPipelineCacheData(VkDevice device, VkPipelineCache pipelineCache, size_t* pDataSize, void* pData)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkGetPipelineCacheData(device, pipelineCache, pDataSize, pData);
      }
      VkResult __attribute__ ((weak))
      radv_MergePipelineCaches(VkDevice device, VkPipelineCache dstCache, uint32_t srcCacheCount, const VkPipelineCache* pSrcCaches)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkMergePipelineCaches(device, dstCache, srcCacheCount, pSrcCaches);
      }
      VkResult __attribute__ ((weak))
      radv_CreateGraphicsPipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkGraphicsPipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkCreateGraphicsPipelines(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
      }
      VkResult __attribute__ ((weak))
      radv_CreateComputePipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkComputePipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkCreateComputePipelines(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
      }
      void __attribute__ ((weak))
      radv_DestroyPipeline(VkDevice device, VkPipeline pipeline, const VkAllocationCallbacks* pAllocator)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkDestroyPipeline(device, pipeline, pAllocator);
      }
      VkResult __attribute__ ((weak))
      radv_CreatePipelineLayout(VkDevice device, const VkPipelineLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkCreatePipelineLayout(device, pCreateInfo, pAllocator, pPipelineLayout);
      }
      void __attribute__ ((weak))
      radv_DestroyPipelineLayout(VkDevice device, VkPipelineLayout pipelineLayout, const VkAllocationCallbacks* pAllocator)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkDestroyPipelineLayout(device, pipelineLayout, pAllocator);
      }
      VkResult __attribute__ ((weak))
      radv_CreateSampler(VkDevice device, const VkSamplerCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSampler* pSampler)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkCreateSampler(device, pCreateInfo, pAllocator, pSampler);
      }
      void __attribute__ ((weak))
      radv_DestroySampler(VkDevice device, VkSampler sampler, const VkAllocationCallbacks* pAllocator)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkDestroySampler(device, sampler, pAllocator);
      }
      VkResult __attribute__ ((weak))
      radv_CreateDescriptorSetLayout(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorSetLayout* pSetLayout)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkCreateDescriptorSetLayout(device, pCreateInfo, pAllocator, pSetLayout);
      }
      void __attribute__ ((weak))
      radv_DestroyDescriptorSetLayout(VkDevice device, VkDescriptorSetLayout descriptorSetLayout, const VkAllocationCallbacks* pAllocator)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkDestroyDescriptorSetLayout(device, descriptorSetLayout, pAllocator);
      }
      VkResult __attribute__ ((weak))
      radv_CreateDescriptorPool(VkDevice device, const VkDescriptorPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorPool* pDescriptorPool)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkCreateDescriptorPool(device, pCreateInfo, pAllocator, pDescriptorPool);
      }
      void __attribute__ ((weak))
      radv_DestroyDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, const VkAllocationCallbacks* pAllocator)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkDestroyDescriptorPool(device, descriptorPool, pAllocator);
      }
      VkResult __attribute__ ((weak))
      radv_ResetDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags flags)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkResetDescriptorPool(device, descriptorPool, flags);
      }
      VkResult __attribute__ ((weak))
      radv_AllocateDescriptorSets(VkDevice device, const VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkAllocateDescriptorSets(device, pAllocateInfo, pDescriptorSets);
      }
      VkResult __attribute__ ((weak))
      radv_FreeDescriptorSets(VkDevice device, VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkFreeDescriptorSets(device, descriptorPool, descriptorSetCount, pDescriptorSets);
      }
      void __attribute__ ((weak))
      radv_UpdateDescriptorSets(VkDevice device, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, const VkCopyDescriptorSet* pDescriptorCopies)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkUpdateDescriptorSets(device, descriptorWriteCount, pDescriptorWrites, descriptorCopyCount, pDescriptorCopies);
      }
      VkResult __attribute__ ((weak))
      radv_CreateFramebuffer(VkDevice device, const VkFramebufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFramebuffer* pFramebuffer)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkCreateFramebuffer(device, pCreateInfo, pAllocator, pFramebuffer);
      }
      void __attribute__ ((weak))
      radv_DestroyFramebuffer(VkDevice device, VkFramebuffer framebuffer, const VkAllocationCallbacks* pAllocator)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkDestroyFramebuffer(device, framebuffer, pAllocator);
      }
      VkResult __attribute__ ((weak))
      radv_CreateRenderPass(VkDevice device, const VkRenderPassCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkCreateRenderPass(device, pCreateInfo, pAllocator, pRenderPass);
      }
      void __attribute__ ((weak))
      radv_DestroyRenderPass(VkDevice device, VkRenderPass renderPass, const VkAllocationCallbacks* pAllocator)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkDestroyRenderPass(device, renderPass, pAllocator);
      }
      void __attribute__ ((weak))
      radv_GetRenderAreaGranularity(VkDevice device, VkRenderPass renderPass, VkExtent2D* pGranularity)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkGetRenderAreaGranularity(device, renderPass, pGranularity);
      }
      VkResult __attribute__ ((weak))
      radv_CreateCommandPool(VkDevice device, const VkCommandPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkCreateCommandPool(device, pCreateInfo, pAllocator, pCommandPool);
      }
      void __attribute__ ((weak))
      radv_DestroyCommandPool(VkDevice device, VkCommandPool commandPool, const VkAllocationCallbacks* pAllocator)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkDestroyCommandPool(device, commandPool, pAllocator);
      }
      VkResult __attribute__ ((weak))
      radv_ResetCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolResetFlags flags)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkResetCommandPool(device, commandPool, flags);
      }
      VkResult __attribute__ ((weak))
      radv_AllocateCommandBuffers(VkDevice device, const VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkAllocateCommandBuffers(device, pAllocateInfo, pCommandBuffers);
      }
      void __attribute__ ((weak))
      radv_FreeCommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkFreeCommandBuffers(device, commandPool, commandBufferCount, pCommandBuffers);
      }
      VkResult __attribute__ ((weak))
      radv_BeginCommandBuffer(VkCommandBuffer commandBuffer, const VkCommandBufferBeginInfo* pBeginInfo)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkBeginCommandBuffer(commandBuffer, pBeginInfo);
      }
      VkResult __attribute__ ((weak))
      radv_EndCommandBuffer(VkCommandBuffer commandBuffer)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkEndCommandBuffer(commandBuffer);
      }
      VkResult __attribute__ ((weak))
      radv_ResetCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkResetCommandBuffer(commandBuffer, flags);
      }
      void __attribute__ ((weak))
      radv_CmdBindPipeline(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdBindPipeline(commandBuffer, pipelineBindPoint, pipeline);
      }
      void __attribute__ ((weak))
      radv_CmdSetViewport(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pViewports)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdSetViewport(commandBuffer, firstViewport, viewportCount, pViewports);
      }
      void __attribute__ ((weak))
      radv_CmdSetScissor(VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pScissors)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdSetScissor(commandBuffer, firstScissor, scissorCount, pScissors);
      }
      void __attribute__ ((weak))
      radv_CmdSetLineWidth(VkCommandBuffer commandBuffer, float lineWidth)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdSetLineWidth(commandBuffer, lineWidth);
      }
      void __attribute__ ((weak))
      radv_CmdSetDepthBias(VkCommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdSetDepthBias(commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
      }
      void __attribute__ ((weak))
      radv_CmdSetBlendConstants(VkCommandBuffer commandBuffer, const float blendConstants[4])
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdSetBlendConstants(commandBuffer, blendConstants);
      }
      void __attribute__ ((weak))
      radv_CmdSetDepthBounds(VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdSetDepthBounds(commandBuffer, minDepthBounds, maxDepthBounds);
      }
      void __attribute__ ((weak))
      radv_CmdSetStencilCompareMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdSetStencilCompareMask(commandBuffer, faceMask, compareMask);
      }
      void __attribute__ ((weak))
      radv_CmdSetStencilWriteMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdSetStencilWriteMask(commandBuffer, faceMask, writeMask);
      }
      void __attribute__ ((weak))
      radv_CmdSetStencilReference(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdSetStencilReference(commandBuffer, faceMask, reference);
      }
      void __attribute__ ((weak))
      radv_CmdBindDescriptorSets(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdBindDescriptorSets(commandBuffer, pipelineBindPoint, layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);
      }
      void __attribute__ ((weak))
      radv_CmdBindIndexBuffer(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdBindIndexBuffer(commandBuffer, buffer, offset, indexType);
      }
      void __attribute__ ((weak))
      radv_CmdBindVertexBuffers(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdBindVertexBuffers(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets);
      }
      void __attribute__ ((weak))
      radv_CmdDraw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdDraw(commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
      }
      void __attribute__ ((weak))
      radv_CmdDrawIndexed(VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdDrawIndexed(commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
      }
      void __attribute__ ((weak))
      radv_CmdDrawIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdDrawIndirect(commandBuffer, buffer, offset, drawCount, stride);
      }
      void __attribute__ ((weak))
      radv_CmdDrawIndexedIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdDrawIndexedIndirect(commandBuffer, buffer, offset, drawCount, stride);
      }
      void __attribute__ ((weak))
      radv_CmdDispatch(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdDispatch(commandBuffer, groupCountX, groupCountY, groupCountZ);
      }
      void __attribute__ ((weak))
      radv_CmdDispatchIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdDispatchIndirect(commandBuffer, buffer, offset);
      }
      void __attribute__ ((weak))
      radv_CmdCopyBuffer(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, regionCount, pRegions);
      }
      void __attribute__ ((weak))
      radv_CmdCopyImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdCopyImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
      }
      void __attribute__ ((weak))
      radv_CmdBlitImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdBlitImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions, filter);
      }
      void __attribute__ ((weak))
      radv_CmdCopyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdCopyBufferToImage(commandBuffer, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions);
      }
      void __attribute__ ((weak))
      radv_CmdCopyImageToBuffer(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdCopyImageToBuffer(commandBuffer, srcImage, srcImageLayout, dstBuffer, regionCount, pRegions);
      }
      void __attribute__ ((weak))
      radv_CmdUpdateBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdUpdateBuffer(commandBuffer, dstBuffer, dstOffset, dataSize, pData);
      }
      void __attribute__ ((weak))
      radv_CmdFillBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdFillBuffer(commandBuffer, dstBuffer, dstOffset, size, data);
      }
      void __attribute__ ((weak))
      radv_CmdClearColorImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount, const VkImageSubresourceRange* pRanges)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdClearColorImage(commandBuffer, image, imageLayout, pColor, rangeCount, pRanges);
      }
      void __attribute__ ((weak))
      radv_CmdClearDepthStencilImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const VkImageSubresourceRange* pRanges)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdClearDepthStencilImage(commandBuffer, image, imageLayout, pDepthStencil, rangeCount, pRanges);
      }
      void __attribute__ ((weak))
      radv_CmdClearAttachments(VkCommandBuffer commandBuffer, uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdClearAttachments(commandBuffer, attachmentCount, pAttachments, rectCount, pRects);
      }
      void __attribute__ ((weak))
      radv_CmdResolveImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageResolve* pRegions)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdResolveImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
      }
      void __attribute__ ((weak))
      radv_CmdSetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdSetEvent(commandBuffer, event, stageMask);
      }
      void __attribute__ ((weak))
      radv_CmdResetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdResetEvent(commandBuffer, event, stageMask);
      }
      void __attribute__ ((weak))
      radv_CmdWaitEvents(VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdWaitEvents(commandBuffer, eventCount, pEvents, srcStageMask, dstStageMask, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
      }
      void __attribute__ ((weak))
      radv_CmdPipelineBarrier(VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdPipelineBarrier(commandBuffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
      }
      void __attribute__ ((weak))
      radv_CmdBeginQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdBeginQuery(commandBuffer, queryPool, query, flags);
      }
      void __attribute__ ((weak))
      radv_CmdEndQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdEndQuery(commandBuffer, queryPool, query);
      }
      void __attribute__ ((weak))
      radv_CmdBeginConditionalRenderingEXT(VkCommandBuffer commandBuffer, const VkConditionalRenderingBeginInfoEXT* pConditionalRenderingBegin)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdBeginConditionalRenderingEXT(commandBuffer, pConditionalRenderingBegin);
      }
      void __attribute__ ((weak))
      radv_CmdEndConditionalRenderingEXT(VkCommandBuffer commandBuffer)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdEndConditionalRenderingEXT(commandBuffer);
      }
      void __attribute__ ((weak))
      radv_CmdResetQueryPool(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdResetQueryPool(commandBuffer, queryPool, firstQuery, queryCount);
      }
      void __attribute__ ((weak))
      radv_CmdWriteTimestamp(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdWriteTimestamp(commandBuffer, pipelineStage, queryPool, query);
      }
      void __attribute__ ((weak))
      radv_CmdCopyQueryPoolResults(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdCopyQueryPoolResults(commandBuffer, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, flags);
      }
      void __attribute__ ((weak))
      radv_CmdPushConstants(VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdPushConstants(commandBuffer, layout, stageFlags, offset, size, pValues);
      }
      void __attribute__ ((weak))
      radv_CmdBeginRenderPass(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdBeginRenderPass(commandBuffer, pRenderPassBegin, contents);
      }
      void __attribute__ ((weak))
      radv_CmdNextSubpass(VkCommandBuffer commandBuffer, VkSubpassContents contents)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdNextSubpass(commandBuffer, contents);
      }
      void __attribute__ ((weak))
      radv_CmdEndRenderPass(VkCommandBuffer commandBuffer)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdEndRenderPass(commandBuffer);
      }
      void __attribute__ ((weak))
      radv_CmdExecuteCommands(VkCommandBuffer commandBuffer, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdExecuteCommands(commandBuffer, commandBufferCount, pCommandBuffers);
      }
      VkResult __attribute__ ((weak))
      radv_CreateSwapchainKHR(VkDevice device, const VkSwapchainCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkCreateSwapchainKHR(device, pCreateInfo, pAllocator, pSwapchain);
      }
      void __attribute__ ((weak))
      radv_DestroySwapchainKHR(VkDevice device, VkSwapchainKHR swapchain, const VkAllocationCallbacks* pAllocator)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkDestroySwapchainKHR(device, swapchain, pAllocator);
      }
      VkResult __attribute__ ((weak))
      radv_GetSwapchainImagesKHR(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkGetSwapchainImagesKHR(device, swapchain, pSwapchainImageCount, pSwapchainImages);
      }
      VkResult __attribute__ ((weak))
      radv_AcquireNextImageKHR(VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkAcquireNextImageKHR(device, swapchain, timeout, semaphore, fence, pImageIndex);
      }
      VkResult __attribute__ ((weak))
      radv_QueuePresentKHR(VkQueue queue, const VkPresentInfoKHR* pPresentInfo)
      {
          RADV_FROM_HANDLE(radv_queue, radv_queue, queue);
          return radv_queue->device->dispatch.vkQueuePresentKHR(queue, pPresentInfo);
      }
      void __attribute__ ((weak))
      radv_CmdPushDescriptorSetKHR(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdPushDescriptorSetKHR(commandBuffer, pipelineBindPoint, layout, set, descriptorWriteCount, pDescriptorWrites);
      }
      void __attribute__ ((weak))
      radv_TrimCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkTrimCommandPool(device, commandPool, flags);
      }
            VkResult __attribute__ ((weak))
      radv_GetMemoryFdKHR(VkDevice device, const VkMemoryGetFdInfoKHR* pGetFdInfo, int* pFd)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkGetMemoryFdKHR(device, pGetFdInfo, pFd);
      }
      VkResult __attribute__ ((weak))
      radv_GetMemoryFdPropertiesKHR(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, int fd, VkMemoryFdPropertiesKHR* pMemoryFdProperties)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkGetMemoryFdPropertiesKHR(device, handleType, fd, pMemoryFdProperties);
      }
      VkResult __attribute__ ((weak))
      radv_GetSemaphoreFdKHR(VkDevice device, const VkSemaphoreGetFdInfoKHR* pGetFdInfo, int* pFd)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkGetSemaphoreFdKHR(device, pGetFdInfo, pFd);
      }
      VkResult __attribute__ ((weak))
      radv_ImportSemaphoreFdKHR(VkDevice device, const VkImportSemaphoreFdInfoKHR* pImportSemaphoreFdInfo)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkImportSemaphoreFdKHR(device, pImportSemaphoreFdInfo);
      }
      VkResult __attribute__ ((weak))
      radv_GetFenceFdKHR(VkDevice device, const VkFenceGetFdInfoKHR* pGetFdInfo, int* pFd)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkGetFenceFdKHR(device, pGetFdInfo, pFd);
      }
      VkResult __attribute__ ((weak))
      radv_ImportFenceFdKHR(VkDevice device, const VkImportFenceFdInfoKHR* pImportFenceFdInfo)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkImportFenceFdKHR(device, pImportFenceFdInfo);
      }
      VkResult __attribute__ ((weak))
      radv_DisplayPowerControlEXT(VkDevice device, VkDisplayKHR display, const VkDisplayPowerInfoEXT* pDisplayPowerInfo)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkDisplayPowerControlEXT(device, display, pDisplayPowerInfo);
      }
      VkResult __attribute__ ((weak))
      radv_RegisterDeviceEventEXT(VkDevice device, const VkDeviceEventInfoEXT* pDeviceEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkRegisterDeviceEventEXT(device, pDeviceEventInfo, pAllocator, pFence);
      }
      VkResult __attribute__ ((weak))
      radv_RegisterDisplayEventEXT(VkDevice device, VkDisplayKHR display, const VkDisplayEventInfoEXT* pDisplayEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkRegisterDisplayEventEXT(device, display, pDisplayEventInfo, pAllocator, pFence);
      }
      VkResult __attribute__ ((weak))
      radv_GetSwapchainCounterEXT(VkDevice device, VkSwapchainKHR swapchain, VkSurfaceCounterFlagBitsEXT counter, uint64_t* pCounterValue)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkGetSwapchainCounterEXT(device, swapchain, counter, pCounterValue);
      }
      void __attribute__ ((weak))
      radv_GetDeviceGroupPeerMemoryFeatures(VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkGetDeviceGroupPeerMemoryFeatures(device, heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures);
      }
            VkResult __attribute__ ((weak))
      radv_BindBufferMemory2(VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkBindBufferMemory2(device, bindInfoCount, pBindInfos);
      }
            VkResult __attribute__ ((weak))
      radv_BindImageMemory2(VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkBindImageMemory2(device, bindInfoCount, pBindInfos);
      }
            void __attribute__ ((weak))
      radv_CmdSetDeviceMask(VkCommandBuffer commandBuffer, uint32_t deviceMask)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdSetDeviceMask(commandBuffer, deviceMask);
      }
            VkResult __attribute__ ((weak))
      radv_GetDeviceGroupPresentCapabilitiesKHR(VkDevice device, VkDeviceGroupPresentCapabilitiesKHR* pDeviceGroupPresentCapabilities)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkGetDeviceGroupPresentCapabilitiesKHR(device, pDeviceGroupPresentCapabilities);
      }
      VkResult __attribute__ ((weak))
      radv_GetDeviceGroupSurfacePresentModesKHR(VkDevice device, VkSurfaceKHR surface, VkDeviceGroupPresentModeFlagsKHR* pModes)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkGetDeviceGroupSurfacePresentModesKHR(device, surface, pModes);
      }
      VkResult __attribute__ ((weak))
      radv_AcquireNextImage2KHR(VkDevice device, const VkAcquireNextImageInfoKHR* pAcquireInfo, uint32_t* pImageIndex)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkAcquireNextImage2KHR(device, pAcquireInfo, pImageIndex);
      }
      void __attribute__ ((weak))
      radv_CmdDispatchBase(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdDispatchBase(commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
      }
            VkResult __attribute__ ((weak))
      radv_CreateDescriptorUpdateTemplate(VkDevice device, const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkCreateDescriptorUpdateTemplate(device, pCreateInfo, pAllocator, pDescriptorUpdateTemplate);
      }
            void __attribute__ ((weak))
      radv_DestroyDescriptorUpdateTemplate(VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkDestroyDescriptorUpdateTemplate(device, descriptorUpdateTemplate, pAllocator);
      }
            void __attribute__ ((weak))
      radv_UpdateDescriptorSetWithTemplate(VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkUpdateDescriptorSetWithTemplate(device, descriptorSet, descriptorUpdateTemplate, pData);
      }
            void __attribute__ ((weak))
      radv_CmdPushDescriptorSetWithTemplateKHR(VkCommandBuffer commandBuffer, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, const void* pData)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdPushDescriptorSetWithTemplateKHR(commandBuffer, descriptorUpdateTemplate, layout, set, pData);
      }
      void __attribute__ ((weak))
      radv_CmdSetDiscardRectangleEXT(VkCommandBuffer commandBuffer, uint32_t firstDiscardRectangle, uint32_t discardRectangleCount, const VkRect2D* pDiscardRectangles)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdSetDiscardRectangleEXT(commandBuffer, firstDiscardRectangle, discardRectangleCount, pDiscardRectangles);
      }
      void __attribute__ ((weak))
      radv_CmdSetSampleLocationsEXT(VkCommandBuffer commandBuffer, const VkSampleLocationsInfoEXT* pSampleLocationsInfo)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdSetSampleLocationsEXT(commandBuffer, pSampleLocationsInfo);
      }
      void __attribute__ ((weak))
      radv_GetBufferMemoryRequirements2(VkDevice device, const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkGetBufferMemoryRequirements2(device, pInfo, pMemoryRequirements);
      }
            void __attribute__ ((weak))
      radv_GetImageMemoryRequirements2(VkDevice device, const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkGetImageMemoryRequirements2(device, pInfo, pMemoryRequirements);
      }
            void __attribute__ ((weak))
      radv_GetImageSparseMemoryRequirements2(VkDevice device, const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkGetImageSparseMemoryRequirements2(device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
      }
            VkResult __attribute__ ((weak))
      radv_CreateSamplerYcbcrConversion(VkDevice device, const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkCreateSamplerYcbcrConversion(device, pCreateInfo, pAllocator, pYcbcrConversion);
      }
            void __attribute__ ((weak))
      radv_DestroySamplerYcbcrConversion(VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkDestroySamplerYcbcrConversion(device, ycbcrConversion, pAllocator);
      }
            void __attribute__ ((weak))
      radv_GetDeviceQueue2(VkDevice device, const VkDeviceQueueInfo2* pQueueInfo, VkQueue* pQueue)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkGetDeviceQueue2(device, pQueueInfo, pQueue);
      }
      void __attribute__ ((weak))
      radv_GetDescriptorSetLayoutSupport(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkGetDescriptorSetLayoutSupport(device, pCreateInfo, pSupport);
      }
      #ifdef VK_USE_PLATFORM_ANDROID_KHR
      VkResult __attribute__ ((weak))
      radv_GetSwapchainGrallocUsageANDROID(VkDevice device, VkFormat format, VkImageUsageFlags imageUsage, int* grallocUsage)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkGetSwapchainGrallocUsageANDROID(device, format, imageUsage, grallocUsage);
      }
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
      VkResult __attribute__ ((weak))
      radv_GetSwapchainGrallocUsage2ANDROID(VkDevice device, VkFormat format, VkImageUsageFlags imageUsage, VkSwapchainImageUsageFlagsANDROID swapchainImageUsage, uint64_t* grallocConsumerUsage, uint64_t* grallocProducerUsage)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkGetSwapchainGrallocUsage2ANDROID(device, format, imageUsage, swapchainImageUsage, grallocConsumerUsage, grallocProducerUsage);
      }
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
      VkResult __attribute__ ((weak))
      radv_AcquireImageANDROID(VkDevice device, VkImage image, int nativeFenceFd, VkSemaphore semaphore, VkFence fence)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkAcquireImageANDROID(device, image, nativeFenceFd, semaphore, fence);
      }
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
      VkResult __attribute__ ((weak))
      radv_QueueSignalReleaseImageANDROID(VkQueue queue, uint32_t waitSemaphoreCount, const VkSemaphore* pWaitSemaphores, VkImage image, int* pNativeFenceFd)
      {
          RADV_FROM_HANDLE(radv_queue, radv_queue, queue);
          return radv_queue->device->dispatch.vkQueueSignalReleaseImageANDROID(queue, waitSemaphoreCount, pWaitSemaphores, image, pNativeFenceFd);
      }
#endif // VK_USE_PLATFORM_ANDROID_KHR
      VkResult __attribute__ ((weak))
      radv_GetShaderInfoAMD(VkDevice device, VkPipeline pipeline, VkShaderStageFlagBits shaderStage, VkShaderInfoTypeAMD infoType, size_t* pInfoSize, void* pInfo)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkGetShaderInfoAMD(device, pipeline, shaderStage, infoType, pInfoSize, pInfo);
      }
      VkResult __attribute__ ((weak))
      radv_GetCalibratedTimestampsEXT(VkDevice device, uint32_t timestampCount, const VkCalibratedTimestampInfoEXT* pTimestampInfos, uint64_t* pTimestamps, uint64_t* pMaxDeviation)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkGetCalibratedTimestampsEXT(device, timestampCount, pTimestampInfos, pTimestamps, pMaxDeviation);
      }
      VkResult __attribute__ ((weak))
      radv_GetMemoryHostPointerPropertiesEXT(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, const void* pHostPointer, VkMemoryHostPointerPropertiesEXT* pMemoryHostPointerProperties)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkGetMemoryHostPointerPropertiesEXT(device, handleType, pHostPointer, pMemoryHostPointerProperties);
      }
      void __attribute__ ((weak))
      radv_CmdWriteBufferMarkerAMD(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdWriteBufferMarkerAMD(commandBuffer, pipelineStage, dstBuffer, dstOffset, marker);
      }
      VkResult __attribute__ ((weak))
      radv_CreateRenderPass2(VkDevice device, const VkRenderPassCreateInfo2* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkCreateRenderPass2(device, pCreateInfo, pAllocator, pRenderPass);
      }
            void __attribute__ ((weak))
      radv_CmdBeginRenderPass2(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo*      pRenderPassBegin, const VkSubpassBeginInfo*      pSubpassBeginInfo)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdBeginRenderPass2(commandBuffer, pRenderPassBegin, pSubpassBeginInfo);
      }
            void __attribute__ ((weak))
      radv_CmdNextSubpass2(VkCommandBuffer commandBuffer, const VkSubpassBeginInfo*      pSubpassBeginInfo, const VkSubpassEndInfo*        pSubpassEndInfo)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdNextSubpass2(commandBuffer, pSubpassBeginInfo, pSubpassEndInfo);
      }
            void __attribute__ ((weak))
      radv_CmdEndRenderPass2(VkCommandBuffer commandBuffer, const VkSubpassEndInfo*        pSubpassEndInfo)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdEndRenderPass2(commandBuffer, pSubpassEndInfo);
      }
            VkResult __attribute__ ((weak))
      radv_GetSemaphoreCounterValue(VkDevice device, VkSemaphore semaphore, uint64_t* pValue)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkGetSemaphoreCounterValue(device, semaphore, pValue);
      }
            VkResult __attribute__ ((weak))
      radv_WaitSemaphores(VkDevice device, const VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkWaitSemaphores(device, pWaitInfo, timeout);
      }
            VkResult __attribute__ ((weak))
      radv_SignalSemaphore(VkDevice device, const VkSemaphoreSignalInfo* pSignalInfo)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkSignalSemaphore(device, pSignalInfo);
      }
      #ifdef VK_USE_PLATFORM_ANDROID_KHR
      VkResult __attribute__ ((weak))
      radv_GetAndroidHardwareBufferPropertiesANDROID(VkDevice device, const struct AHardwareBuffer* buffer, VkAndroidHardwareBufferPropertiesANDROID* pProperties)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkGetAndroidHardwareBufferPropertiesANDROID(device, buffer, pProperties);
      }
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
      VkResult __attribute__ ((weak))
      radv_GetMemoryAndroidHardwareBufferANDROID(VkDevice device, const VkMemoryGetAndroidHardwareBufferInfoANDROID* pInfo, struct AHardwareBuffer** pBuffer)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkGetMemoryAndroidHardwareBufferANDROID(device, pInfo, pBuffer);
      }
#endif // VK_USE_PLATFORM_ANDROID_KHR
      void __attribute__ ((weak))
      radv_CmdDrawIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdDrawIndirectCount(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
      }
                  void __attribute__ ((weak))
      radv_CmdDrawIndexedIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdDrawIndexedIndirectCount(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
      }
                  void __attribute__ ((weak))
      radv_CmdBindTransformFeedbackBuffersEXT(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdBindTransformFeedbackBuffersEXT(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes);
      }
      void __attribute__ ((weak))
      radv_CmdBeginTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdBeginTransformFeedbackEXT(commandBuffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
      }
      void __attribute__ ((weak))
      radv_CmdEndTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdEndTransformFeedbackEXT(commandBuffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
      }
      void __attribute__ ((weak))
      radv_CmdBeginQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags, uint32_t index)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdBeginQueryIndexedEXT(commandBuffer, queryPool, query, flags, index);
      }
      void __attribute__ ((weak))
      radv_CmdEndQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, uint32_t index)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdEndQueryIndexedEXT(commandBuffer, queryPool, query, index);
      }
      void __attribute__ ((weak))
      radv_CmdDrawIndirectByteCountEXT(VkCommandBuffer commandBuffer, uint32_t instanceCount, uint32_t firstInstance, VkBuffer counterBuffer, VkDeviceSize counterBufferOffset, uint32_t counterOffset, uint32_t vertexStride)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdDrawIndirectByteCountEXT(commandBuffer, instanceCount, firstInstance, counterBuffer, counterBufferOffset, counterOffset, vertexStride);
      }
#ifdef VK_ENABLE_BETA_EXTENSIONS
      void __attribute__ ((weak))
      radv_DestroyAccelerationStructureNV(VkDevice device, VkAccelerationStructureKHR accelerationStructure, const VkAllocationCallbacks* pAllocator)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkDestroyAccelerationStructureNV(device, accelerationStructure, pAllocator);
      }
#endif // VK_ENABLE_BETA_EXTENSIONS
#ifdef VK_ENABLE_BETA_EXTENSIONS
      VkResult __attribute__ ((weak))
      radv_BindAccelerationStructureMemoryNV(VkDevice device, uint32_t bindInfoCount, const VkBindAccelerationStructureMemoryInfoKHR* pBindInfos)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkBindAccelerationStructureMemoryNV(device, bindInfoCount, pBindInfos);
      }
#endif // VK_ENABLE_BETA_EXTENSIONS
#ifdef VK_ENABLE_BETA_EXTENSIONS
      void __attribute__ ((weak))
      radv_CmdWriteAccelerationStructuresPropertiesNV(VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, const VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdWriteAccelerationStructuresPropertiesNV(commandBuffer, accelerationStructureCount, pAccelerationStructures, queryType, queryPool, firstQuery);
      }
#endif // VK_ENABLE_BETA_EXTENSIONS
#ifdef VK_ENABLE_BETA_EXTENSIONS
      VkResult __attribute__ ((weak))
      radv_GetRayTracingShaderGroupHandlesNV(VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkGetRayTracingShaderGroupHandlesNV(device, pipeline, firstGroup, groupCount, dataSize, pData);
      }
#endif // VK_ENABLE_BETA_EXTENSIONS
      uint64_t __attribute__ ((weak))
      radv_GetBufferOpaqueCaptureAddress(VkDevice device, const VkBufferDeviceAddressInfo* pInfo)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkGetBufferOpaqueCaptureAddress(device, pInfo);
      }
            VkDeviceAddress __attribute__ ((weak))
      radv_GetBufferDeviceAddress(VkDevice device, const VkBufferDeviceAddressInfo* pInfo)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkGetBufferDeviceAddress(device, pInfo);
      }
                  uint64_t __attribute__ ((weak))
      radv_GetDeviceMemoryOpaqueCaptureAddress(VkDevice device, const VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkGetDeviceMemoryOpaqueCaptureAddress(device, pInfo);
      }
            VkResult __attribute__ ((weak))
      radv_GetPipelineExecutablePropertiesKHR(VkDevice                        device, const VkPipelineInfoKHR*        pPipelineInfo, uint32_t* pExecutableCount, VkPipelineExecutablePropertiesKHR* pProperties)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkGetPipelineExecutablePropertiesKHR(device, pPipelineInfo, pExecutableCount, pProperties);
      }
      VkResult __attribute__ ((weak))
      radv_GetPipelineExecutableStatisticsKHR(VkDevice                        device, const VkPipelineExecutableInfoKHR*  pExecutableInfo, uint32_t* pStatisticCount, VkPipelineExecutableStatisticKHR* pStatistics)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkGetPipelineExecutableStatisticsKHR(device, pExecutableInfo, pStatisticCount, pStatistics);
      }
      VkResult __attribute__ ((weak))
      radv_GetPipelineExecutableInternalRepresentationsKHR(VkDevice                        device, const VkPipelineExecutableInfoKHR*  pExecutableInfo, uint32_t* pInternalRepresentationCount, VkPipelineExecutableInternalRepresentationKHR* pInternalRepresentations)
      {
          RADV_FROM_HANDLE(radv_device, radv_device, device);
          return radv_device->dispatch.vkGetPipelineExecutableInternalRepresentationsKHR(device, pExecutableInfo, pInternalRepresentationCount, pInternalRepresentations);
      }
      void __attribute__ ((weak))
      radv_CmdSetLineStippleEXT(VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern)
      {
          RADV_FROM_HANDLE(radv_cmd_buffer, radv_cmd_buffer, commandBuffer);
          return radv_cmd_buffer->device->dispatch.vkCmdSetLineStippleEXT(commandBuffer, lineStippleFactor, lineStipplePattern);
      }

  const struct radv_device_dispatch_table radv_device_dispatch_table = {
    .vkGetDeviceProcAddr = radv_GetDeviceProcAddr,
    .vkDestroyDevice = radv_DestroyDevice,
    .vkGetDeviceQueue = radv_GetDeviceQueue,
    .vkQueueSubmit = radv_QueueSubmit,
    .vkQueueWaitIdle = radv_QueueWaitIdle,
    .vkDeviceWaitIdle = radv_DeviceWaitIdle,
    .vkAllocateMemory = radv_AllocateMemory,
    .vkFreeMemory = radv_FreeMemory,
    .vkMapMemory = radv_MapMemory,
    .vkUnmapMemory = radv_UnmapMemory,
    .vkFlushMappedMemoryRanges = radv_FlushMappedMemoryRanges,
    .vkInvalidateMappedMemoryRanges = radv_InvalidateMappedMemoryRanges,
    .vkGetDeviceMemoryCommitment = radv_GetDeviceMemoryCommitment,
    .vkGetBufferMemoryRequirements = radv_GetBufferMemoryRequirements,
    .vkBindBufferMemory = radv_BindBufferMemory,
    .vkGetImageMemoryRequirements = radv_GetImageMemoryRequirements,
    .vkBindImageMemory = radv_BindImageMemory,
    .vkGetImageSparseMemoryRequirements = radv_GetImageSparseMemoryRequirements,
    .vkQueueBindSparse = radv_QueueBindSparse,
    .vkCreateFence = radv_CreateFence,
    .vkDestroyFence = radv_DestroyFence,
    .vkResetFences = radv_ResetFences,
    .vkGetFenceStatus = radv_GetFenceStatus,
    .vkWaitForFences = radv_WaitForFences,
    .vkCreateSemaphore = radv_CreateSemaphore,
    .vkDestroySemaphore = radv_DestroySemaphore,
    .vkCreateEvent = radv_CreateEvent,
    .vkDestroyEvent = radv_DestroyEvent,
    .vkGetEventStatus = radv_GetEventStatus,
    .vkSetEvent = radv_SetEvent,
    .vkResetEvent = radv_ResetEvent,
    .vkCreateQueryPool = radv_CreateQueryPool,
    .vkDestroyQueryPool = radv_DestroyQueryPool,
    .vkGetQueryPoolResults = radv_GetQueryPoolResults,
    .vkResetQueryPool = radv_ResetQueryPool,
    .vkResetQueryPoolEXT = radv_ResetQueryPool,
    .vkCreateBuffer = radv_CreateBuffer,
    .vkDestroyBuffer = radv_DestroyBuffer,
    .vkCreateBufferView = radv_CreateBufferView,
    .vkDestroyBufferView = radv_DestroyBufferView,
    .vkCreateImage = radv_CreateImage,
    .vkDestroyImage = radv_DestroyImage,
    .vkGetImageSubresourceLayout = radv_GetImageSubresourceLayout,
    .vkCreateImageView = radv_CreateImageView,
    .vkDestroyImageView = radv_DestroyImageView,
    .vkCreateShaderModule = radv_CreateShaderModule,
    .vkDestroyShaderModule = radv_DestroyShaderModule,
    .vkCreatePipelineCache = radv_CreatePipelineCache,
    .vkDestroyPipelineCache = radv_DestroyPipelineCache,
    .vkGetPipelineCacheData = radv_GetPipelineCacheData,
    .vkMergePipelineCaches = radv_MergePipelineCaches,
    .vkCreateGraphicsPipelines = radv_CreateGraphicsPipelines,
    .vkCreateComputePipelines = radv_CreateComputePipelines,
    .vkDestroyPipeline = radv_DestroyPipeline,
    .vkCreatePipelineLayout = radv_CreatePipelineLayout,
    .vkDestroyPipelineLayout = radv_DestroyPipelineLayout,
    .vkCreateSampler = radv_CreateSampler,
    .vkDestroySampler = radv_DestroySampler,
    .vkCreateDescriptorSetLayout = radv_CreateDescriptorSetLayout,
    .vkDestroyDescriptorSetLayout = radv_DestroyDescriptorSetLayout,
    .vkCreateDescriptorPool = radv_CreateDescriptorPool,
    .vkDestroyDescriptorPool = radv_DestroyDescriptorPool,
    .vkResetDescriptorPool = radv_ResetDescriptorPool,
    .vkAllocateDescriptorSets = radv_AllocateDescriptorSets,
    .vkFreeDescriptorSets = radv_FreeDescriptorSets,
    .vkUpdateDescriptorSets = radv_UpdateDescriptorSets,
    .vkCreateFramebuffer = radv_CreateFramebuffer,
    .vkDestroyFramebuffer = radv_DestroyFramebuffer,
    .vkCreateRenderPass = radv_CreateRenderPass,
    .vkDestroyRenderPass = radv_DestroyRenderPass,
    .vkGetRenderAreaGranularity = radv_GetRenderAreaGranularity,
    .vkCreateCommandPool = radv_CreateCommandPool,
    .vkDestroyCommandPool = radv_DestroyCommandPool,
    .vkResetCommandPool = radv_ResetCommandPool,
    .vkAllocateCommandBuffers = radv_AllocateCommandBuffers,
    .vkFreeCommandBuffers = radv_FreeCommandBuffers,
    .vkBeginCommandBuffer = radv_BeginCommandBuffer,
    .vkEndCommandBuffer = radv_EndCommandBuffer,
    .vkResetCommandBuffer = radv_ResetCommandBuffer,
    .vkCmdBindPipeline = radv_CmdBindPipeline,
    .vkCmdSetViewport = radv_CmdSetViewport,
    .vkCmdSetScissor = radv_CmdSetScissor,
    .vkCmdSetLineWidth = radv_CmdSetLineWidth,
    .vkCmdSetDepthBias = radv_CmdSetDepthBias,
    .vkCmdSetBlendConstants = radv_CmdSetBlendConstants,
    .vkCmdSetDepthBounds = radv_CmdSetDepthBounds,
    .vkCmdSetStencilCompareMask = radv_CmdSetStencilCompareMask,
    .vkCmdSetStencilWriteMask = radv_CmdSetStencilWriteMask,
    .vkCmdSetStencilReference = radv_CmdSetStencilReference,
    .vkCmdBindDescriptorSets = radv_CmdBindDescriptorSets,
    .vkCmdBindIndexBuffer = radv_CmdBindIndexBuffer,
    .vkCmdBindVertexBuffers = radv_CmdBindVertexBuffers,
    .vkCmdDraw = radv_CmdDraw,
    .vkCmdDrawIndexed = radv_CmdDrawIndexed,
    .vkCmdDrawIndirect = radv_CmdDrawIndirect,
    .vkCmdDrawIndexedIndirect = radv_CmdDrawIndexedIndirect,
    .vkCmdDispatch = radv_CmdDispatch,
    .vkCmdDispatchIndirect = radv_CmdDispatchIndirect,
    .vkCmdCopyBuffer = radv_CmdCopyBuffer,
    .vkCmdCopyImage = radv_CmdCopyImage,
    .vkCmdBlitImage = radv_CmdBlitImage,
    .vkCmdCopyBufferToImage = radv_CmdCopyBufferToImage,
    .vkCmdCopyImageToBuffer = radv_CmdCopyImageToBuffer,
    .vkCmdUpdateBuffer = radv_CmdUpdateBuffer,
    .vkCmdFillBuffer = radv_CmdFillBuffer,
    .vkCmdClearColorImage = radv_CmdClearColorImage,
    .vkCmdClearDepthStencilImage = radv_CmdClearDepthStencilImage,
    .vkCmdClearAttachments = radv_CmdClearAttachments,
    .vkCmdResolveImage = radv_CmdResolveImage,
    .vkCmdSetEvent = radv_CmdSetEvent,
    .vkCmdResetEvent = radv_CmdResetEvent,
    .vkCmdWaitEvents = radv_CmdWaitEvents,
    .vkCmdPipelineBarrier = radv_CmdPipelineBarrier,
    .vkCmdBeginQuery = radv_CmdBeginQuery,
    .vkCmdEndQuery = radv_CmdEndQuery,
    .vkCmdBeginConditionalRenderingEXT = radv_CmdBeginConditionalRenderingEXT,
    .vkCmdEndConditionalRenderingEXT = radv_CmdEndConditionalRenderingEXT,
    .vkCmdResetQueryPool = radv_CmdResetQueryPool,
    .vkCmdWriteTimestamp = radv_CmdWriteTimestamp,
    .vkCmdCopyQueryPoolResults = radv_CmdCopyQueryPoolResults,
    .vkCmdPushConstants = radv_CmdPushConstants,
    .vkCmdBeginRenderPass = radv_CmdBeginRenderPass,
    .vkCmdNextSubpass = radv_CmdNextSubpass,
    .vkCmdEndRenderPass = radv_CmdEndRenderPass,
    .vkCmdExecuteCommands = radv_CmdExecuteCommands,
    .vkCreateSwapchainKHR = radv_CreateSwapchainKHR,
    .vkDestroySwapchainKHR = radv_DestroySwapchainKHR,
    .vkGetSwapchainImagesKHR = radv_GetSwapchainImagesKHR,
    .vkAcquireNextImageKHR = radv_AcquireNextImageKHR,
    .vkQueuePresentKHR = radv_QueuePresentKHR,
    .vkCmdPushDescriptorSetKHR = radv_CmdPushDescriptorSetKHR,
    .vkTrimCommandPool = radv_TrimCommandPool,
    .vkTrimCommandPoolKHR = radv_TrimCommandPool,
    .vkGetMemoryFdKHR = radv_GetMemoryFdKHR,
    .vkGetMemoryFdPropertiesKHR = radv_GetMemoryFdPropertiesKHR,
    .vkGetSemaphoreFdKHR = radv_GetSemaphoreFdKHR,
    .vkImportSemaphoreFdKHR = radv_ImportSemaphoreFdKHR,
    .vkGetFenceFdKHR = radv_GetFenceFdKHR,
    .vkImportFenceFdKHR = radv_ImportFenceFdKHR,
    .vkDisplayPowerControlEXT = radv_DisplayPowerControlEXT,
    .vkRegisterDeviceEventEXT = radv_RegisterDeviceEventEXT,
    .vkRegisterDisplayEventEXT = radv_RegisterDisplayEventEXT,
    .vkGetSwapchainCounterEXT = radv_GetSwapchainCounterEXT,
    .vkGetDeviceGroupPeerMemoryFeatures = radv_GetDeviceGroupPeerMemoryFeatures,
    .vkGetDeviceGroupPeerMemoryFeaturesKHR = radv_GetDeviceGroupPeerMemoryFeatures,
    .vkBindBufferMemory2 = radv_BindBufferMemory2,
    .vkBindBufferMemory2KHR = radv_BindBufferMemory2,
    .vkBindImageMemory2 = radv_BindImageMemory2,
    .vkBindImageMemory2KHR = radv_BindImageMemory2,
    .vkCmdSetDeviceMask = radv_CmdSetDeviceMask,
    .vkCmdSetDeviceMaskKHR = radv_CmdSetDeviceMask,
    .vkGetDeviceGroupPresentCapabilitiesKHR = radv_GetDeviceGroupPresentCapabilitiesKHR,
    .vkGetDeviceGroupSurfacePresentModesKHR = radv_GetDeviceGroupSurfacePresentModesKHR,
    .vkAcquireNextImage2KHR = radv_AcquireNextImage2KHR,
    .vkCmdDispatchBase = radv_CmdDispatchBase,
    .vkCmdDispatchBaseKHR = radv_CmdDispatchBase,
    .vkCreateDescriptorUpdateTemplate = radv_CreateDescriptorUpdateTemplate,
    .vkCreateDescriptorUpdateTemplateKHR = radv_CreateDescriptorUpdateTemplate,
    .vkDestroyDescriptorUpdateTemplate = radv_DestroyDescriptorUpdateTemplate,
    .vkDestroyDescriptorUpdateTemplateKHR = radv_DestroyDescriptorUpdateTemplate,
    .vkUpdateDescriptorSetWithTemplate = radv_UpdateDescriptorSetWithTemplate,
    .vkUpdateDescriptorSetWithTemplateKHR = radv_UpdateDescriptorSetWithTemplate,
    .vkCmdPushDescriptorSetWithTemplateKHR = radv_CmdPushDescriptorSetWithTemplateKHR,
    .vkCmdSetDiscardRectangleEXT = radv_CmdSetDiscardRectangleEXT,
    .vkCmdSetSampleLocationsEXT = radv_CmdSetSampleLocationsEXT,
    .vkGetBufferMemoryRequirements2 = radv_GetBufferMemoryRequirements2,
    .vkGetBufferMemoryRequirements2KHR = radv_GetBufferMemoryRequirements2,
    .vkGetImageMemoryRequirements2 = radv_GetImageMemoryRequirements2,
    .vkGetImageMemoryRequirements2KHR = radv_GetImageMemoryRequirements2,
    .vkGetImageSparseMemoryRequirements2 = radv_GetImageSparseMemoryRequirements2,
    .vkGetImageSparseMemoryRequirements2KHR = radv_GetImageSparseMemoryRequirements2,
    .vkCreateSamplerYcbcrConversion = radv_CreateSamplerYcbcrConversion,
    .vkCreateSamplerYcbcrConversionKHR = radv_CreateSamplerYcbcrConversion,
    .vkDestroySamplerYcbcrConversion = radv_DestroySamplerYcbcrConversion,
    .vkDestroySamplerYcbcrConversionKHR = radv_DestroySamplerYcbcrConversion,
    .vkGetDeviceQueue2 = radv_GetDeviceQueue2,
    .vkGetDescriptorSetLayoutSupport = radv_GetDescriptorSetLayoutSupport,
    .vkGetDescriptorSetLayoutSupportKHR = radv_GetDescriptorSetLayoutSupport,
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .vkGetSwapchainGrallocUsageANDROID = radv_GetSwapchainGrallocUsageANDROID,
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .vkGetSwapchainGrallocUsage2ANDROID = radv_GetSwapchainGrallocUsage2ANDROID,
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .vkAcquireImageANDROID = radv_AcquireImageANDROID,
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .vkQueueSignalReleaseImageANDROID = radv_QueueSignalReleaseImageANDROID,
#endif // VK_USE_PLATFORM_ANDROID_KHR
    .vkGetShaderInfoAMD = radv_GetShaderInfoAMD,
    .vkGetCalibratedTimestampsEXT = radv_GetCalibratedTimestampsEXT,
    .vkGetMemoryHostPointerPropertiesEXT = radv_GetMemoryHostPointerPropertiesEXT,
    .vkCmdWriteBufferMarkerAMD = radv_CmdWriteBufferMarkerAMD,
    .vkCreateRenderPass2 = radv_CreateRenderPass2,
    .vkCreateRenderPass2KHR = radv_CreateRenderPass2,
    .vkCmdBeginRenderPass2 = radv_CmdBeginRenderPass2,
    .vkCmdBeginRenderPass2KHR = radv_CmdBeginRenderPass2,
    .vkCmdNextSubpass2 = radv_CmdNextSubpass2,
    .vkCmdNextSubpass2KHR = radv_CmdNextSubpass2,
    .vkCmdEndRenderPass2 = radv_CmdEndRenderPass2,
    .vkCmdEndRenderPass2KHR = radv_CmdEndRenderPass2,
    .vkGetSemaphoreCounterValue = radv_GetSemaphoreCounterValue,
    .vkGetSemaphoreCounterValueKHR = radv_GetSemaphoreCounterValue,
    .vkWaitSemaphores = radv_WaitSemaphores,
    .vkWaitSemaphoresKHR = radv_WaitSemaphores,
    .vkSignalSemaphore = radv_SignalSemaphore,
    .vkSignalSemaphoreKHR = radv_SignalSemaphore,
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .vkGetAndroidHardwareBufferPropertiesANDROID = radv_GetAndroidHardwareBufferPropertiesANDROID,
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .vkGetMemoryAndroidHardwareBufferANDROID = radv_GetMemoryAndroidHardwareBufferANDROID,
#endif // VK_USE_PLATFORM_ANDROID_KHR
    .vkCmdDrawIndirectCount = radv_CmdDrawIndirectCount,
    .vkCmdDrawIndirectCountKHR = radv_CmdDrawIndirectCount,
    .vkCmdDrawIndirectCountAMD = radv_CmdDrawIndirectCount,
    .vkCmdDrawIndexedIndirectCount = radv_CmdDrawIndexedIndirectCount,
    .vkCmdDrawIndexedIndirectCountKHR = radv_CmdDrawIndexedIndirectCount,
    .vkCmdDrawIndexedIndirectCountAMD = radv_CmdDrawIndexedIndirectCount,
    .vkCmdBindTransformFeedbackBuffersEXT = radv_CmdBindTransformFeedbackBuffersEXT,
    .vkCmdBeginTransformFeedbackEXT = radv_CmdBeginTransformFeedbackEXT,
    .vkCmdEndTransformFeedbackEXT = radv_CmdEndTransformFeedbackEXT,
    .vkCmdBeginQueryIndexedEXT = radv_CmdBeginQueryIndexedEXT,
    .vkCmdEndQueryIndexedEXT = radv_CmdEndQueryIndexedEXT,
    .vkCmdDrawIndirectByteCountEXT = radv_CmdDrawIndirectByteCountEXT,
#ifdef VK_ENABLE_BETA_EXTENSIONS
    .vkDestroyAccelerationStructureNV = radv_DestroyAccelerationStructureNV,
#endif // VK_ENABLE_BETA_EXTENSIONS
#ifdef VK_ENABLE_BETA_EXTENSIONS
    .vkBindAccelerationStructureMemoryNV = radv_BindAccelerationStructureMemoryNV,
#endif // VK_ENABLE_BETA_EXTENSIONS
#ifdef VK_ENABLE_BETA_EXTENSIONS
    .vkCmdWriteAccelerationStructuresPropertiesNV = radv_CmdWriteAccelerationStructuresPropertiesNV,
#endif // VK_ENABLE_BETA_EXTENSIONS
#ifdef VK_ENABLE_BETA_EXTENSIONS
    .vkGetRayTracingShaderGroupHandlesNV = radv_GetRayTracingShaderGroupHandlesNV,
#endif // VK_ENABLE_BETA_EXTENSIONS
    .vkGetBufferOpaqueCaptureAddress = radv_GetBufferOpaqueCaptureAddress,
    .vkGetBufferOpaqueCaptureAddressKHR = radv_GetBufferOpaqueCaptureAddress,
    .vkGetBufferDeviceAddress = radv_GetBufferDeviceAddress,
    .vkGetBufferDeviceAddressKHR = radv_GetBufferDeviceAddress,
    .vkGetBufferDeviceAddressEXT = radv_GetBufferDeviceAddress,
    .vkGetDeviceMemoryOpaqueCaptureAddress = radv_GetDeviceMemoryOpaqueCaptureAddress,
    .vkGetDeviceMemoryOpaqueCaptureAddressKHR = radv_GetDeviceMemoryOpaqueCaptureAddress,
    .vkGetPipelineExecutablePropertiesKHR = radv_GetPipelineExecutablePropertiesKHR,
    .vkGetPipelineExecutableStatisticsKHR = radv_GetPipelineExecutableStatisticsKHR,
    .vkGetPipelineExecutableInternalRepresentationsKHR = radv_GetPipelineExecutableInternalRepresentationsKHR,
    .vkCmdSetLineStippleEXT = radv_CmdSetLineStippleEXT,
  };
      PFN_vkVoidFunction sqtt_GetDeviceProcAddr(VkDevice device, const char* pName) __attribute__ ((weak));
      void sqtt_DestroyDevice(VkDevice device, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      void sqtt_GetDeviceQueue(VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue) __attribute__ ((weak));
      VkResult sqtt_QueueSubmit(VkQueue queue, uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence) __attribute__ ((weak));
      VkResult sqtt_QueueWaitIdle(VkQueue queue) __attribute__ ((weak));
      VkResult sqtt_DeviceWaitIdle(VkDevice device) __attribute__ ((weak));
      VkResult sqtt_AllocateMemory(VkDevice device, const VkMemoryAllocateInfo* pAllocateInfo, const VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory) __attribute__ ((weak));
      void sqtt_FreeMemory(VkDevice device, VkDeviceMemory memory, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult sqtt_MapMemory(VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData) __attribute__ ((weak));
      void sqtt_UnmapMemory(VkDevice device, VkDeviceMemory memory) __attribute__ ((weak));
      VkResult sqtt_FlushMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges) __attribute__ ((weak));
      VkResult sqtt_InvalidateMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges) __attribute__ ((weak));
      void sqtt_GetDeviceMemoryCommitment(VkDevice device, VkDeviceMemory memory, VkDeviceSize* pCommittedMemoryInBytes) __attribute__ ((weak));
      void sqtt_GetBufferMemoryRequirements(VkDevice device, VkBuffer buffer, VkMemoryRequirements* pMemoryRequirements) __attribute__ ((weak));
      VkResult sqtt_BindBufferMemory(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset) __attribute__ ((weak));
      void sqtt_GetImageMemoryRequirements(VkDevice device, VkImage image, VkMemoryRequirements* pMemoryRequirements) __attribute__ ((weak));
      VkResult sqtt_BindImageMemory(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset) __attribute__ ((weak));
      void sqtt_GetImageSparseMemoryRequirements(VkDevice device, VkImage image, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements* pSparseMemoryRequirements) __attribute__ ((weak));
      VkResult sqtt_QueueBindSparse(VkQueue queue, uint32_t bindInfoCount, const VkBindSparseInfo* pBindInfo, VkFence fence) __attribute__ ((weak));
      VkResult sqtt_CreateFence(VkDevice device, const VkFenceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) __attribute__ ((weak));
      void sqtt_DestroyFence(VkDevice device, VkFence fence, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult sqtt_ResetFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences) __attribute__ ((weak));
      VkResult sqtt_GetFenceStatus(VkDevice device, VkFence fence) __attribute__ ((weak));
      VkResult sqtt_WaitForFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences, VkBool32 waitAll, uint64_t timeout) __attribute__ ((weak));
      VkResult sqtt_CreateSemaphore(VkDevice device, const VkSemaphoreCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore) __attribute__ ((weak));
      void sqtt_DestroySemaphore(VkDevice device, VkSemaphore semaphore, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult sqtt_CreateEvent(VkDevice device, const VkEventCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkEvent* pEvent) __attribute__ ((weak));
      void sqtt_DestroyEvent(VkDevice device, VkEvent event, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult sqtt_GetEventStatus(VkDevice device, VkEvent event) __attribute__ ((weak));
      VkResult sqtt_SetEvent(VkDevice device, VkEvent event) __attribute__ ((weak));
      VkResult sqtt_ResetEvent(VkDevice device, VkEvent event) __attribute__ ((weak));
      VkResult sqtt_CreateQueryPool(VkDevice device, const VkQueryPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkQueryPool* pQueryPool) __attribute__ ((weak));
      void sqtt_DestroyQueryPool(VkDevice device, VkQueryPool queryPool, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult sqtt_GetQueryPoolResults(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, VkDeviceSize stride, VkQueryResultFlags flags) __attribute__ ((weak));
      void sqtt_ResetQueryPool(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) __attribute__ ((weak));
            VkResult sqtt_CreateBuffer(VkDevice device, const VkBufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer) __attribute__ ((weak));
      void sqtt_DestroyBuffer(VkDevice device, VkBuffer buffer, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult sqtt_CreateBufferView(VkDevice device, const VkBufferViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferView* pView) __attribute__ ((weak));
      void sqtt_DestroyBufferView(VkDevice device, VkBufferView bufferView, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult sqtt_CreateImage(VkDevice device, const VkImageCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImage* pImage) __attribute__ ((weak));
      void sqtt_DestroyImage(VkDevice device, VkImage image, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      void sqtt_GetImageSubresourceLayout(VkDevice device, VkImage image, const VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout) __attribute__ ((weak));
      VkResult sqtt_CreateImageView(VkDevice device, const VkImageViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImageView* pView) __attribute__ ((weak));
      void sqtt_DestroyImageView(VkDevice device, VkImageView imageView, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult sqtt_CreateShaderModule(VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule) __attribute__ ((weak));
      void sqtt_DestroyShaderModule(VkDevice device, VkShaderModule shaderModule, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult sqtt_CreatePipelineCache(VkDevice device, const VkPipelineCacheCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineCache* pPipelineCache) __attribute__ ((weak));
      void sqtt_DestroyPipelineCache(VkDevice device, VkPipelineCache pipelineCache, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult sqtt_GetPipelineCacheData(VkDevice device, VkPipelineCache pipelineCache, size_t* pDataSize, void* pData) __attribute__ ((weak));
      VkResult sqtt_MergePipelineCaches(VkDevice device, VkPipelineCache dstCache, uint32_t srcCacheCount, const VkPipelineCache* pSrcCaches) __attribute__ ((weak));
      VkResult sqtt_CreateGraphicsPipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkGraphicsPipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) __attribute__ ((weak));
      VkResult sqtt_CreateComputePipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkComputePipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) __attribute__ ((weak));
      void sqtt_DestroyPipeline(VkDevice device, VkPipeline pipeline, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult sqtt_CreatePipelineLayout(VkDevice device, const VkPipelineLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout) __attribute__ ((weak));
      void sqtt_DestroyPipelineLayout(VkDevice device, VkPipelineLayout pipelineLayout, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult sqtt_CreateSampler(VkDevice device, const VkSamplerCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSampler* pSampler) __attribute__ ((weak));
      void sqtt_DestroySampler(VkDevice device, VkSampler sampler, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult sqtt_CreateDescriptorSetLayout(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorSetLayout* pSetLayout) __attribute__ ((weak));
      void sqtt_DestroyDescriptorSetLayout(VkDevice device, VkDescriptorSetLayout descriptorSetLayout, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult sqtt_CreateDescriptorPool(VkDevice device, const VkDescriptorPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorPool* pDescriptorPool) __attribute__ ((weak));
      void sqtt_DestroyDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult sqtt_ResetDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags flags) __attribute__ ((weak));
      VkResult sqtt_AllocateDescriptorSets(VkDevice device, const VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets) __attribute__ ((weak));
      VkResult sqtt_FreeDescriptorSets(VkDevice device, VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets) __attribute__ ((weak));
      void sqtt_UpdateDescriptorSets(VkDevice device, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, const VkCopyDescriptorSet* pDescriptorCopies) __attribute__ ((weak));
      VkResult sqtt_CreateFramebuffer(VkDevice device, const VkFramebufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFramebuffer* pFramebuffer) __attribute__ ((weak));
      void sqtt_DestroyFramebuffer(VkDevice device, VkFramebuffer framebuffer, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult sqtt_CreateRenderPass(VkDevice device, const VkRenderPassCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) __attribute__ ((weak));
      void sqtt_DestroyRenderPass(VkDevice device, VkRenderPass renderPass, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      void sqtt_GetRenderAreaGranularity(VkDevice device, VkRenderPass renderPass, VkExtent2D* pGranularity) __attribute__ ((weak));
      VkResult sqtt_CreateCommandPool(VkDevice device, const VkCommandPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool) __attribute__ ((weak));
      void sqtt_DestroyCommandPool(VkDevice device, VkCommandPool commandPool, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult sqtt_ResetCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolResetFlags flags) __attribute__ ((weak));
      VkResult sqtt_AllocateCommandBuffers(VkDevice device, const VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers) __attribute__ ((weak));
      void sqtt_FreeCommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) __attribute__ ((weak));
      VkResult sqtt_BeginCommandBuffer(VkCommandBuffer commandBuffer, const VkCommandBufferBeginInfo* pBeginInfo) __attribute__ ((weak));
      VkResult sqtt_EndCommandBuffer(VkCommandBuffer commandBuffer) __attribute__ ((weak));
      VkResult sqtt_ResetCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags) __attribute__ ((weak));
      void sqtt_CmdBindPipeline(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline) __attribute__ ((weak));
      void sqtt_CmdSetViewport(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pViewports) __attribute__ ((weak));
      void sqtt_CmdSetScissor(VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pScissors) __attribute__ ((weak));
      void sqtt_CmdSetLineWidth(VkCommandBuffer commandBuffer, float lineWidth) __attribute__ ((weak));
      void sqtt_CmdSetDepthBias(VkCommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor) __attribute__ ((weak));
      void sqtt_CmdSetBlendConstants(VkCommandBuffer commandBuffer, const float blendConstants[4]) __attribute__ ((weak));
      void sqtt_CmdSetDepthBounds(VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds) __attribute__ ((weak));
      void sqtt_CmdSetStencilCompareMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask) __attribute__ ((weak));
      void sqtt_CmdSetStencilWriteMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask) __attribute__ ((weak));
      void sqtt_CmdSetStencilReference(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference) __attribute__ ((weak));
      void sqtt_CmdBindDescriptorSets(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets) __attribute__ ((weak));
      void sqtt_CmdBindIndexBuffer(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType) __attribute__ ((weak));
      void sqtt_CmdBindVertexBuffers(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets) __attribute__ ((weak));
      void sqtt_CmdDraw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) __attribute__ ((weak));
      void sqtt_CmdDrawIndexed(VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) __attribute__ ((weak));
      void sqtt_CmdDrawIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) __attribute__ ((weak));
      void sqtt_CmdDrawIndexedIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) __attribute__ ((weak));
      void sqtt_CmdDispatch(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) __attribute__ ((weak));
      void sqtt_CmdDispatchIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset) __attribute__ ((weak));
      void sqtt_CmdCopyBuffer(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions) __attribute__ ((weak));
      void sqtt_CmdCopyImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions) __attribute__ ((weak));
      void sqtt_CmdBlitImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter) __attribute__ ((weak));
      void sqtt_CmdCopyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions) __attribute__ ((weak));
      void sqtt_CmdCopyImageToBuffer(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions) __attribute__ ((weak));
      void sqtt_CmdUpdateBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData) __attribute__ ((weak));
      void sqtt_CmdFillBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data) __attribute__ ((weak));
      void sqtt_CmdClearColorImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) __attribute__ ((weak));
      void sqtt_CmdClearDepthStencilImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) __attribute__ ((weak));
      void sqtt_CmdClearAttachments(VkCommandBuffer commandBuffer, uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects) __attribute__ ((weak));
      void sqtt_CmdResolveImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageResolve* pRegions) __attribute__ ((weak));
      void sqtt_CmdSetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) __attribute__ ((weak));
      void sqtt_CmdResetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) __attribute__ ((weak));
      void sqtt_CmdWaitEvents(VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) __attribute__ ((weak));
      void sqtt_CmdPipelineBarrier(VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) __attribute__ ((weak));
      void sqtt_CmdBeginQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags) __attribute__ ((weak));
      void sqtt_CmdEndQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query) __attribute__ ((weak));
      void sqtt_CmdBeginConditionalRenderingEXT(VkCommandBuffer commandBuffer, const VkConditionalRenderingBeginInfoEXT* pConditionalRenderingBegin) __attribute__ ((weak));
      void sqtt_CmdEndConditionalRenderingEXT(VkCommandBuffer commandBuffer) __attribute__ ((weak));
      void sqtt_CmdResetQueryPool(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) __attribute__ ((weak));
      void sqtt_CmdWriteTimestamp(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query) __attribute__ ((weak));
      void sqtt_CmdCopyQueryPoolResults(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags) __attribute__ ((weak));
      void sqtt_CmdPushConstants(VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues) __attribute__ ((weak));
      void sqtt_CmdBeginRenderPass(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents) __attribute__ ((weak));
      void sqtt_CmdNextSubpass(VkCommandBuffer commandBuffer, VkSubpassContents contents) __attribute__ ((weak));
      void sqtt_CmdEndRenderPass(VkCommandBuffer commandBuffer) __attribute__ ((weak));
      void sqtt_CmdExecuteCommands(VkCommandBuffer commandBuffer, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) __attribute__ ((weak));
      VkResult sqtt_CreateSwapchainKHR(VkDevice device, const VkSwapchainCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain) __attribute__ ((weak));
      void sqtt_DestroySwapchainKHR(VkDevice device, VkSwapchainKHR swapchain, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult sqtt_GetSwapchainImagesKHR(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages) __attribute__ ((weak));
      VkResult sqtt_AcquireNextImageKHR(VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex) __attribute__ ((weak));
      VkResult sqtt_QueuePresentKHR(VkQueue queue, const VkPresentInfoKHR* pPresentInfo) __attribute__ ((weak));
      void sqtt_CmdPushDescriptorSetKHR(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites) __attribute__ ((weak));
      void sqtt_TrimCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags) __attribute__ ((weak));
            VkResult sqtt_GetMemoryFdKHR(VkDevice device, const VkMemoryGetFdInfoKHR* pGetFdInfo, int* pFd) __attribute__ ((weak));
      VkResult sqtt_GetMemoryFdPropertiesKHR(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, int fd, VkMemoryFdPropertiesKHR* pMemoryFdProperties) __attribute__ ((weak));
      VkResult sqtt_GetSemaphoreFdKHR(VkDevice device, const VkSemaphoreGetFdInfoKHR* pGetFdInfo, int* pFd) __attribute__ ((weak));
      VkResult sqtt_ImportSemaphoreFdKHR(VkDevice device, const VkImportSemaphoreFdInfoKHR* pImportSemaphoreFdInfo) __attribute__ ((weak));
      VkResult sqtt_GetFenceFdKHR(VkDevice device, const VkFenceGetFdInfoKHR* pGetFdInfo, int* pFd) __attribute__ ((weak));
      VkResult sqtt_ImportFenceFdKHR(VkDevice device, const VkImportFenceFdInfoKHR* pImportFenceFdInfo) __attribute__ ((weak));
      VkResult sqtt_DisplayPowerControlEXT(VkDevice device, VkDisplayKHR display, const VkDisplayPowerInfoEXT* pDisplayPowerInfo) __attribute__ ((weak));
      VkResult sqtt_RegisterDeviceEventEXT(VkDevice device, const VkDeviceEventInfoEXT* pDeviceEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) __attribute__ ((weak));
      VkResult sqtt_RegisterDisplayEventEXT(VkDevice device, VkDisplayKHR display, const VkDisplayEventInfoEXT* pDisplayEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) __attribute__ ((weak));
      VkResult sqtt_GetSwapchainCounterEXT(VkDevice device, VkSwapchainKHR swapchain, VkSurfaceCounterFlagBitsEXT counter, uint64_t* pCounterValue) __attribute__ ((weak));
      void sqtt_GetDeviceGroupPeerMemoryFeatures(VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures) __attribute__ ((weak));
            VkResult sqtt_BindBufferMemory2(VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos) __attribute__ ((weak));
            VkResult sqtt_BindImageMemory2(VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos) __attribute__ ((weak));
            void sqtt_CmdSetDeviceMask(VkCommandBuffer commandBuffer, uint32_t deviceMask) __attribute__ ((weak));
            VkResult sqtt_GetDeviceGroupPresentCapabilitiesKHR(VkDevice device, VkDeviceGroupPresentCapabilitiesKHR* pDeviceGroupPresentCapabilities) __attribute__ ((weak));
      VkResult sqtt_GetDeviceGroupSurfacePresentModesKHR(VkDevice device, VkSurfaceKHR surface, VkDeviceGroupPresentModeFlagsKHR* pModes) __attribute__ ((weak));
      VkResult sqtt_AcquireNextImage2KHR(VkDevice device, const VkAcquireNextImageInfoKHR* pAcquireInfo, uint32_t* pImageIndex) __attribute__ ((weak));
      void sqtt_CmdDispatchBase(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) __attribute__ ((weak));
            VkResult sqtt_CreateDescriptorUpdateTemplate(VkDevice device, const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate) __attribute__ ((weak));
            void sqtt_DestroyDescriptorUpdateTemplate(VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
            void sqtt_UpdateDescriptorSetWithTemplate(VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData) __attribute__ ((weak));
            void sqtt_CmdPushDescriptorSetWithTemplateKHR(VkCommandBuffer commandBuffer, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, const void* pData) __attribute__ ((weak));
      void sqtt_CmdSetDiscardRectangleEXT(VkCommandBuffer commandBuffer, uint32_t firstDiscardRectangle, uint32_t discardRectangleCount, const VkRect2D* pDiscardRectangles) __attribute__ ((weak));
      void sqtt_CmdSetSampleLocationsEXT(VkCommandBuffer commandBuffer, const VkSampleLocationsInfoEXT* pSampleLocationsInfo) __attribute__ ((weak));
      void sqtt_GetBufferMemoryRequirements2(VkDevice device, const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) __attribute__ ((weak));
            void sqtt_GetImageMemoryRequirements2(VkDevice device, const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) __attribute__ ((weak));
            void sqtt_GetImageSparseMemoryRequirements2(VkDevice device, const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) __attribute__ ((weak));
            VkResult sqtt_CreateSamplerYcbcrConversion(VkDevice device, const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion) __attribute__ ((weak));
            void sqtt_DestroySamplerYcbcrConversion(VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
            void sqtt_GetDeviceQueue2(VkDevice device, const VkDeviceQueueInfo2* pQueueInfo, VkQueue* pQueue) __attribute__ ((weak));
      void sqtt_GetDescriptorSetLayoutSupport(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport) __attribute__ ((weak));
      #ifdef VK_USE_PLATFORM_ANDROID_KHR
      VkResult sqtt_GetSwapchainGrallocUsageANDROID(VkDevice device, VkFormat format, VkImageUsageFlags imageUsage, int* grallocUsage) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
      VkResult sqtt_GetSwapchainGrallocUsage2ANDROID(VkDevice device, VkFormat format, VkImageUsageFlags imageUsage, VkSwapchainImageUsageFlagsANDROID swapchainImageUsage, uint64_t* grallocConsumerUsage, uint64_t* grallocProducerUsage) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
      VkResult sqtt_AcquireImageANDROID(VkDevice device, VkImage image, int nativeFenceFd, VkSemaphore semaphore, VkFence fence) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
      VkResult sqtt_QueueSignalReleaseImageANDROID(VkQueue queue, uint32_t waitSemaphoreCount, const VkSemaphore* pWaitSemaphores, VkImage image, int* pNativeFenceFd) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_ANDROID_KHR
      VkResult sqtt_GetShaderInfoAMD(VkDevice device, VkPipeline pipeline, VkShaderStageFlagBits shaderStage, VkShaderInfoTypeAMD infoType, size_t* pInfoSize, void* pInfo) __attribute__ ((weak));
      VkResult sqtt_GetCalibratedTimestampsEXT(VkDevice device, uint32_t timestampCount, const VkCalibratedTimestampInfoEXT* pTimestampInfos, uint64_t* pTimestamps, uint64_t* pMaxDeviation) __attribute__ ((weak));
      VkResult sqtt_GetMemoryHostPointerPropertiesEXT(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, const void* pHostPointer, VkMemoryHostPointerPropertiesEXT* pMemoryHostPointerProperties) __attribute__ ((weak));
      void sqtt_CmdWriteBufferMarkerAMD(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker) __attribute__ ((weak));
      VkResult sqtt_CreateRenderPass2(VkDevice device, const VkRenderPassCreateInfo2* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) __attribute__ ((weak));
            void sqtt_CmdBeginRenderPass2(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo*      pRenderPassBegin, const VkSubpassBeginInfo*      pSubpassBeginInfo) __attribute__ ((weak));
            void sqtt_CmdNextSubpass2(VkCommandBuffer commandBuffer, const VkSubpassBeginInfo*      pSubpassBeginInfo, const VkSubpassEndInfo*        pSubpassEndInfo) __attribute__ ((weak));
            void sqtt_CmdEndRenderPass2(VkCommandBuffer commandBuffer, const VkSubpassEndInfo*        pSubpassEndInfo) __attribute__ ((weak));
            VkResult sqtt_GetSemaphoreCounterValue(VkDevice device, VkSemaphore semaphore, uint64_t* pValue) __attribute__ ((weak));
            VkResult sqtt_WaitSemaphores(VkDevice device, const VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout) __attribute__ ((weak));
            VkResult sqtt_SignalSemaphore(VkDevice device, const VkSemaphoreSignalInfo* pSignalInfo) __attribute__ ((weak));
      #ifdef VK_USE_PLATFORM_ANDROID_KHR
      VkResult sqtt_GetAndroidHardwareBufferPropertiesANDROID(VkDevice device, const struct AHardwareBuffer* buffer, VkAndroidHardwareBufferPropertiesANDROID* pProperties) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
      VkResult sqtt_GetMemoryAndroidHardwareBufferANDROID(VkDevice device, const VkMemoryGetAndroidHardwareBufferInfoANDROID* pInfo, struct AHardwareBuffer** pBuffer) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_ANDROID_KHR
      void sqtt_CmdDrawIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) __attribute__ ((weak));
                  void sqtt_CmdDrawIndexedIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) __attribute__ ((weak));
                  void sqtt_CmdBindTransformFeedbackBuffersEXT(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes) __attribute__ ((weak));
      void sqtt_CmdBeginTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets) __attribute__ ((weak));
      void sqtt_CmdEndTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets) __attribute__ ((weak));
      void sqtt_CmdBeginQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags, uint32_t index) __attribute__ ((weak));
      void sqtt_CmdEndQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, uint32_t index) __attribute__ ((weak));
      void sqtt_CmdDrawIndirectByteCountEXT(VkCommandBuffer commandBuffer, uint32_t instanceCount, uint32_t firstInstance, VkBuffer counterBuffer, VkDeviceSize counterBufferOffset, uint32_t counterOffset, uint32_t vertexStride) __attribute__ ((weak));
#ifdef VK_ENABLE_BETA_EXTENSIONS
      void sqtt_DestroyAccelerationStructureNV(VkDevice device, VkAccelerationStructureKHR accelerationStructure, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
#endif // VK_ENABLE_BETA_EXTENSIONS
#ifdef VK_ENABLE_BETA_EXTENSIONS
      VkResult sqtt_BindAccelerationStructureMemoryNV(VkDevice device, uint32_t bindInfoCount, const VkBindAccelerationStructureMemoryInfoKHR* pBindInfos) __attribute__ ((weak));
#endif // VK_ENABLE_BETA_EXTENSIONS
#ifdef VK_ENABLE_BETA_EXTENSIONS
      void sqtt_CmdWriteAccelerationStructuresPropertiesNV(VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, const VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery) __attribute__ ((weak));
#endif // VK_ENABLE_BETA_EXTENSIONS
#ifdef VK_ENABLE_BETA_EXTENSIONS
      VkResult sqtt_GetRayTracingShaderGroupHandlesNV(VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData) __attribute__ ((weak));
#endif // VK_ENABLE_BETA_EXTENSIONS
      uint64_t sqtt_GetBufferOpaqueCaptureAddress(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) __attribute__ ((weak));
            VkDeviceAddress sqtt_GetBufferDeviceAddress(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) __attribute__ ((weak));
                  uint64_t sqtt_GetDeviceMemoryOpaqueCaptureAddress(VkDevice device, const VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo) __attribute__ ((weak));
            VkResult sqtt_GetPipelineExecutablePropertiesKHR(VkDevice                        device, const VkPipelineInfoKHR*        pPipelineInfo, uint32_t* pExecutableCount, VkPipelineExecutablePropertiesKHR* pProperties) __attribute__ ((weak));
      VkResult sqtt_GetPipelineExecutableStatisticsKHR(VkDevice                        device, const VkPipelineExecutableInfoKHR*  pExecutableInfo, uint32_t* pStatisticCount, VkPipelineExecutableStatisticKHR* pStatistics) __attribute__ ((weak));
      VkResult sqtt_GetPipelineExecutableInternalRepresentationsKHR(VkDevice                        device, const VkPipelineExecutableInfoKHR*  pExecutableInfo, uint32_t* pInternalRepresentationCount, VkPipelineExecutableInternalRepresentationKHR* pInternalRepresentations) __attribute__ ((weak));
      void sqtt_CmdSetLineStippleEXT(VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern) __attribute__ ((weak));

  const struct radv_device_dispatch_table sqtt_device_dispatch_table = {
    .vkGetDeviceProcAddr = sqtt_GetDeviceProcAddr,
    .vkDestroyDevice = sqtt_DestroyDevice,
    .vkGetDeviceQueue = sqtt_GetDeviceQueue,
    .vkQueueSubmit = sqtt_QueueSubmit,
    .vkQueueWaitIdle = sqtt_QueueWaitIdle,
    .vkDeviceWaitIdle = sqtt_DeviceWaitIdle,
    .vkAllocateMemory = sqtt_AllocateMemory,
    .vkFreeMemory = sqtt_FreeMemory,
    .vkMapMemory = sqtt_MapMemory,
    .vkUnmapMemory = sqtt_UnmapMemory,
    .vkFlushMappedMemoryRanges = sqtt_FlushMappedMemoryRanges,
    .vkInvalidateMappedMemoryRanges = sqtt_InvalidateMappedMemoryRanges,
    .vkGetDeviceMemoryCommitment = sqtt_GetDeviceMemoryCommitment,
    .vkGetBufferMemoryRequirements = sqtt_GetBufferMemoryRequirements,
    .vkBindBufferMemory = sqtt_BindBufferMemory,
    .vkGetImageMemoryRequirements = sqtt_GetImageMemoryRequirements,
    .vkBindImageMemory = sqtt_BindImageMemory,
    .vkGetImageSparseMemoryRequirements = sqtt_GetImageSparseMemoryRequirements,
    .vkQueueBindSparse = sqtt_QueueBindSparse,
    .vkCreateFence = sqtt_CreateFence,
    .vkDestroyFence = sqtt_DestroyFence,
    .vkResetFences = sqtt_ResetFences,
    .vkGetFenceStatus = sqtt_GetFenceStatus,
    .vkWaitForFences = sqtt_WaitForFences,
    .vkCreateSemaphore = sqtt_CreateSemaphore,
    .vkDestroySemaphore = sqtt_DestroySemaphore,
    .vkCreateEvent = sqtt_CreateEvent,
    .vkDestroyEvent = sqtt_DestroyEvent,
    .vkGetEventStatus = sqtt_GetEventStatus,
    .vkSetEvent = sqtt_SetEvent,
    .vkResetEvent = sqtt_ResetEvent,
    .vkCreateQueryPool = sqtt_CreateQueryPool,
    .vkDestroyQueryPool = sqtt_DestroyQueryPool,
    .vkGetQueryPoolResults = sqtt_GetQueryPoolResults,
    .vkResetQueryPool = sqtt_ResetQueryPool,
    .vkResetQueryPoolEXT = sqtt_ResetQueryPool,
    .vkCreateBuffer = sqtt_CreateBuffer,
    .vkDestroyBuffer = sqtt_DestroyBuffer,
    .vkCreateBufferView = sqtt_CreateBufferView,
    .vkDestroyBufferView = sqtt_DestroyBufferView,
    .vkCreateImage = sqtt_CreateImage,
    .vkDestroyImage = sqtt_DestroyImage,
    .vkGetImageSubresourceLayout = sqtt_GetImageSubresourceLayout,
    .vkCreateImageView = sqtt_CreateImageView,
    .vkDestroyImageView = sqtt_DestroyImageView,
    .vkCreateShaderModule = sqtt_CreateShaderModule,
    .vkDestroyShaderModule = sqtt_DestroyShaderModule,
    .vkCreatePipelineCache = sqtt_CreatePipelineCache,
    .vkDestroyPipelineCache = sqtt_DestroyPipelineCache,
    .vkGetPipelineCacheData = sqtt_GetPipelineCacheData,
    .vkMergePipelineCaches = sqtt_MergePipelineCaches,
    .vkCreateGraphicsPipelines = sqtt_CreateGraphicsPipelines,
    .vkCreateComputePipelines = sqtt_CreateComputePipelines,
    .vkDestroyPipeline = sqtt_DestroyPipeline,
    .vkCreatePipelineLayout = sqtt_CreatePipelineLayout,
    .vkDestroyPipelineLayout = sqtt_DestroyPipelineLayout,
    .vkCreateSampler = sqtt_CreateSampler,
    .vkDestroySampler = sqtt_DestroySampler,
    .vkCreateDescriptorSetLayout = sqtt_CreateDescriptorSetLayout,
    .vkDestroyDescriptorSetLayout = sqtt_DestroyDescriptorSetLayout,
    .vkCreateDescriptorPool = sqtt_CreateDescriptorPool,
    .vkDestroyDescriptorPool = sqtt_DestroyDescriptorPool,
    .vkResetDescriptorPool = sqtt_ResetDescriptorPool,
    .vkAllocateDescriptorSets = sqtt_AllocateDescriptorSets,
    .vkFreeDescriptorSets = sqtt_FreeDescriptorSets,
    .vkUpdateDescriptorSets = sqtt_UpdateDescriptorSets,
    .vkCreateFramebuffer = sqtt_CreateFramebuffer,
    .vkDestroyFramebuffer = sqtt_DestroyFramebuffer,
    .vkCreateRenderPass = sqtt_CreateRenderPass,
    .vkDestroyRenderPass = sqtt_DestroyRenderPass,
    .vkGetRenderAreaGranularity = sqtt_GetRenderAreaGranularity,
    .vkCreateCommandPool = sqtt_CreateCommandPool,
    .vkDestroyCommandPool = sqtt_DestroyCommandPool,
    .vkResetCommandPool = sqtt_ResetCommandPool,
    .vkAllocateCommandBuffers = sqtt_AllocateCommandBuffers,
    .vkFreeCommandBuffers = sqtt_FreeCommandBuffers,
    .vkBeginCommandBuffer = sqtt_BeginCommandBuffer,
    .vkEndCommandBuffer = sqtt_EndCommandBuffer,
    .vkResetCommandBuffer = sqtt_ResetCommandBuffer,
    .vkCmdBindPipeline = sqtt_CmdBindPipeline,
    .vkCmdSetViewport = sqtt_CmdSetViewport,
    .vkCmdSetScissor = sqtt_CmdSetScissor,
    .vkCmdSetLineWidth = sqtt_CmdSetLineWidth,
    .vkCmdSetDepthBias = sqtt_CmdSetDepthBias,
    .vkCmdSetBlendConstants = sqtt_CmdSetBlendConstants,
    .vkCmdSetDepthBounds = sqtt_CmdSetDepthBounds,
    .vkCmdSetStencilCompareMask = sqtt_CmdSetStencilCompareMask,
    .vkCmdSetStencilWriteMask = sqtt_CmdSetStencilWriteMask,
    .vkCmdSetStencilReference = sqtt_CmdSetStencilReference,
    .vkCmdBindDescriptorSets = sqtt_CmdBindDescriptorSets,
    .vkCmdBindIndexBuffer = sqtt_CmdBindIndexBuffer,
    .vkCmdBindVertexBuffers = sqtt_CmdBindVertexBuffers,
    .vkCmdDraw = sqtt_CmdDraw,
    .vkCmdDrawIndexed = sqtt_CmdDrawIndexed,
    .vkCmdDrawIndirect = sqtt_CmdDrawIndirect,
    .vkCmdDrawIndexedIndirect = sqtt_CmdDrawIndexedIndirect,
    .vkCmdDispatch = sqtt_CmdDispatch,
    .vkCmdDispatchIndirect = sqtt_CmdDispatchIndirect,
    .vkCmdCopyBuffer = sqtt_CmdCopyBuffer,
    .vkCmdCopyImage = sqtt_CmdCopyImage,
    .vkCmdBlitImage = sqtt_CmdBlitImage,
    .vkCmdCopyBufferToImage = sqtt_CmdCopyBufferToImage,
    .vkCmdCopyImageToBuffer = sqtt_CmdCopyImageToBuffer,
    .vkCmdUpdateBuffer = sqtt_CmdUpdateBuffer,
    .vkCmdFillBuffer = sqtt_CmdFillBuffer,
    .vkCmdClearColorImage = sqtt_CmdClearColorImage,
    .vkCmdClearDepthStencilImage = sqtt_CmdClearDepthStencilImage,
    .vkCmdClearAttachments = sqtt_CmdClearAttachments,
    .vkCmdResolveImage = sqtt_CmdResolveImage,
    .vkCmdSetEvent = sqtt_CmdSetEvent,
    .vkCmdResetEvent = sqtt_CmdResetEvent,
    .vkCmdWaitEvents = sqtt_CmdWaitEvents,
    .vkCmdPipelineBarrier = sqtt_CmdPipelineBarrier,
    .vkCmdBeginQuery = sqtt_CmdBeginQuery,
    .vkCmdEndQuery = sqtt_CmdEndQuery,
    .vkCmdBeginConditionalRenderingEXT = sqtt_CmdBeginConditionalRenderingEXT,
    .vkCmdEndConditionalRenderingEXT = sqtt_CmdEndConditionalRenderingEXT,
    .vkCmdResetQueryPool = sqtt_CmdResetQueryPool,
    .vkCmdWriteTimestamp = sqtt_CmdWriteTimestamp,
    .vkCmdCopyQueryPoolResults = sqtt_CmdCopyQueryPoolResults,
    .vkCmdPushConstants = sqtt_CmdPushConstants,
    .vkCmdBeginRenderPass = sqtt_CmdBeginRenderPass,
    .vkCmdNextSubpass = sqtt_CmdNextSubpass,
    .vkCmdEndRenderPass = sqtt_CmdEndRenderPass,
    .vkCmdExecuteCommands = sqtt_CmdExecuteCommands,
    .vkCreateSwapchainKHR = sqtt_CreateSwapchainKHR,
    .vkDestroySwapchainKHR = sqtt_DestroySwapchainKHR,
    .vkGetSwapchainImagesKHR = sqtt_GetSwapchainImagesKHR,
    .vkAcquireNextImageKHR = sqtt_AcquireNextImageKHR,
    .vkQueuePresentKHR = sqtt_QueuePresentKHR,
    .vkCmdPushDescriptorSetKHR = sqtt_CmdPushDescriptorSetKHR,
    .vkTrimCommandPool = sqtt_TrimCommandPool,
    .vkTrimCommandPoolKHR = sqtt_TrimCommandPool,
    .vkGetMemoryFdKHR = sqtt_GetMemoryFdKHR,
    .vkGetMemoryFdPropertiesKHR = sqtt_GetMemoryFdPropertiesKHR,
    .vkGetSemaphoreFdKHR = sqtt_GetSemaphoreFdKHR,
    .vkImportSemaphoreFdKHR = sqtt_ImportSemaphoreFdKHR,
    .vkGetFenceFdKHR = sqtt_GetFenceFdKHR,
    .vkImportFenceFdKHR = sqtt_ImportFenceFdKHR,
    .vkDisplayPowerControlEXT = sqtt_DisplayPowerControlEXT,
    .vkRegisterDeviceEventEXT = sqtt_RegisterDeviceEventEXT,
    .vkRegisterDisplayEventEXT = sqtt_RegisterDisplayEventEXT,
    .vkGetSwapchainCounterEXT = sqtt_GetSwapchainCounterEXT,
    .vkGetDeviceGroupPeerMemoryFeatures = sqtt_GetDeviceGroupPeerMemoryFeatures,
    .vkGetDeviceGroupPeerMemoryFeaturesKHR = sqtt_GetDeviceGroupPeerMemoryFeatures,
    .vkBindBufferMemory2 = sqtt_BindBufferMemory2,
    .vkBindBufferMemory2KHR = sqtt_BindBufferMemory2,
    .vkBindImageMemory2 = sqtt_BindImageMemory2,
    .vkBindImageMemory2KHR = sqtt_BindImageMemory2,
    .vkCmdSetDeviceMask = sqtt_CmdSetDeviceMask,
    .vkCmdSetDeviceMaskKHR = sqtt_CmdSetDeviceMask,
    .vkGetDeviceGroupPresentCapabilitiesKHR = sqtt_GetDeviceGroupPresentCapabilitiesKHR,
    .vkGetDeviceGroupSurfacePresentModesKHR = sqtt_GetDeviceGroupSurfacePresentModesKHR,
    .vkAcquireNextImage2KHR = sqtt_AcquireNextImage2KHR,
    .vkCmdDispatchBase = sqtt_CmdDispatchBase,
    .vkCmdDispatchBaseKHR = sqtt_CmdDispatchBase,
    .vkCreateDescriptorUpdateTemplate = sqtt_CreateDescriptorUpdateTemplate,
    .vkCreateDescriptorUpdateTemplateKHR = sqtt_CreateDescriptorUpdateTemplate,
    .vkDestroyDescriptorUpdateTemplate = sqtt_DestroyDescriptorUpdateTemplate,
    .vkDestroyDescriptorUpdateTemplateKHR = sqtt_DestroyDescriptorUpdateTemplate,
    .vkUpdateDescriptorSetWithTemplate = sqtt_UpdateDescriptorSetWithTemplate,
    .vkUpdateDescriptorSetWithTemplateKHR = sqtt_UpdateDescriptorSetWithTemplate,
    .vkCmdPushDescriptorSetWithTemplateKHR = sqtt_CmdPushDescriptorSetWithTemplateKHR,
    .vkCmdSetDiscardRectangleEXT = sqtt_CmdSetDiscardRectangleEXT,
    .vkCmdSetSampleLocationsEXT = sqtt_CmdSetSampleLocationsEXT,
    .vkGetBufferMemoryRequirements2 = sqtt_GetBufferMemoryRequirements2,
    .vkGetBufferMemoryRequirements2KHR = sqtt_GetBufferMemoryRequirements2,
    .vkGetImageMemoryRequirements2 = sqtt_GetImageMemoryRequirements2,
    .vkGetImageMemoryRequirements2KHR = sqtt_GetImageMemoryRequirements2,
    .vkGetImageSparseMemoryRequirements2 = sqtt_GetImageSparseMemoryRequirements2,
    .vkGetImageSparseMemoryRequirements2KHR = sqtt_GetImageSparseMemoryRequirements2,
    .vkCreateSamplerYcbcrConversion = sqtt_CreateSamplerYcbcrConversion,
    .vkCreateSamplerYcbcrConversionKHR = sqtt_CreateSamplerYcbcrConversion,
    .vkDestroySamplerYcbcrConversion = sqtt_DestroySamplerYcbcrConversion,
    .vkDestroySamplerYcbcrConversionKHR = sqtt_DestroySamplerYcbcrConversion,
    .vkGetDeviceQueue2 = sqtt_GetDeviceQueue2,
    .vkGetDescriptorSetLayoutSupport = sqtt_GetDescriptorSetLayoutSupport,
    .vkGetDescriptorSetLayoutSupportKHR = sqtt_GetDescriptorSetLayoutSupport,
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .vkGetSwapchainGrallocUsageANDROID = sqtt_GetSwapchainGrallocUsageANDROID,
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .vkGetSwapchainGrallocUsage2ANDROID = sqtt_GetSwapchainGrallocUsage2ANDROID,
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .vkAcquireImageANDROID = sqtt_AcquireImageANDROID,
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .vkQueueSignalReleaseImageANDROID = sqtt_QueueSignalReleaseImageANDROID,
#endif // VK_USE_PLATFORM_ANDROID_KHR
    .vkGetShaderInfoAMD = sqtt_GetShaderInfoAMD,
    .vkGetCalibratedTimestampsEXT = sqtt_GetCalibratedTimestampsEXT,
    .vkGetMemoryHostPointerPropertiesEXT = sqtt_GetMemoryHostPointerPropertiesEXT,
    .vkCmdWriteBufferMarkerAMD = sqtt_CmdWriteBufferMarkerAMD,
    .vkCreateRenderPass2 = sqtt_CreateRenderPass2,
    .vkCreateRenderPass2KHR = sqtt_CreateRenderPass2,
    .vkCmdBeginRenderPass2 = sqtt_CmdBeginRenderPass2,
    .vkCmdBeginRenderPass2KHR = sqtt_CmdBeginRenderPass2,
    .vkCmdNextSubpass2 = sqtt_CmdNextSubpass2,
    .vkCmdNextSubpass2KHR = sqtt_CmdNextSubpass2,
    .vkCmdEndRenderPass2 = sqtt_CmdEndRenderPass2,
    .vkCmdEndRenderPass2KHR = sqtt_CmdEndRenderPass2,
    .vkGetSemaphoreCounterValue = sqtt_GetSemaphoreCounterValue,
    .vkGetSemaphoreCounterValueKHR = sqtt_GetSemaphoreCounterValue,
    .vkWaitSemaphores = sqtt_WaitSemaphores,
    .vkWaitSemaphoresKHR = sqtt_WaitSemaphores,
    .vkSignalSemaphore = sqtt_SignalSemaphore,
    .vkSignalSemaphoreKHR = sqtt_SignalSemaphore,
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .vkGetAndroidHardwareBufferPropertiesANDROID = sqtt_GetAndroidHardwareBufferPropertiesANDROID,
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .vkGetMemoryAndroidHardwareBufferANDROID = sqtt_GetMemoryAndroidHardwareBufferANDROID,
#endif // VK_USE_PLATFORM_ANDROID_KHR
    .vkCmdDrawIndirectCount = sqtt_CmdDrawIndirectCount,
    .vkCmdDrawIndirectCountKHR = sqtt_CmdDrawIndirectCount,
    .vkCmdDrawIndirectCountAMD = sqtt_CmdDrawIndirectCount,
    .vkCmdDrawIndexedIndirectCount = sqtt_CmdDrawIndexedIndirectCount,
    .vkCmdDrawIndexedIndirectCountKHR = sqtt_CmdDrawIndexedIndirectCount,
    .vkCmdDrawIndexedIndirectCountAMD = sqtt_CmdDrawIndexedIndirectCount,
    .vkCmdBindTransformFeedbackBuffersEXT = sqtt_CmdBindTransformFeedbackBuffersEXT,
    .vkCmdBeginTransformFeedbackEXT = sqtt_CmdBeginTransformFeedbackEXT,
    .vkCmdEndTransformFeedbackEXT = sqtt_CmdEndTransformFeedbackEXT,
    .vkCmdBeginQueryIndexedEXT = sqtt_CmdBeginQueryIndexedEXT,
    .vkCmdEndQueryIndexedEXT = sqtt_CmdEndQueryIndexedEXT,
    .vkCmdDrawIndirectByteCountEXT = sqtt_CmdDrawIndirectByteCountEXT,
#ifdef VK_ENABLE_BETA_EXTENSIONS
    .vkDestroyAccelerationStructureNV = sqtt_DestroyAccelerationStructureNV,
#endif // VK_ENABLE_BETA_EXTENSIONS
#ifdef VK_ENABLE_BETA_EXTENSIONS
    .vkBindAccelerationStructureMemoryNV = sqtt_BindAccelerationStructureMemoryNV,
#endif // VK_ENABLE_BETA_EXTENSIONS
#ifdef VK_ENABLE_BETA_EXTENSIONS
    .vkCmdWriteAccelerationStructuresPropertiesNV = sqtt_CmdWriteAccelerationStructuresPropertiesNV,
#endif // VK_ENABLE_BETA_EXTENSIONS
#ifdef VK_ENABLE_BETA_EXTENSIONS
    .vkGetRayTracingShaderGroupHandlesNV = sqtt_GetRayTracingShaderGroupHandlesNV,
#endif // VK_ENABLE_BETA_EXTENSIONS
    .vkGetBufferOpaqueCaptureAddress = sqtt_GetBufferOpaqueCaptureAddress,
    .vkGetBufferOpaqueCaptureAddressKHR = sqtt_GetBufferOpaqueCaptureAddress,
    .vkGetBufferDeviceAddress = sqtt_GetBufferDeviceAddress,
    .vkGetBufferDeviceAddressKHR = sqtt_GetBufferDeviceAddress,
    .vkGetBufferDeviceAddressEXT = sqtt_GetBufferDeviceAddress,
    .vkGetDeviceMemoryOpaqueCaptureAddress = sqtt_GetDeviceMemoryOpaqueCaptureAddress,
    .vkGetDeviceMemoryOpaqueCaptureAddressKHR = sqtt_GetDeviceMemoryOpaqueCaptureAddress,
    .vkGetPipelineExecutablePropertiesKHR = sqtt_GetPipelineExecutablePropertiesKHR,
    .vkGetPipelineExecutableStatisticsKHR = sqtt_GetPipelineExecutableStatisticsKHR,
    .vkGetPipelineExecutableInternalRepresentationsKHR = sqtt_GetPipelineExecutableInternalRepresentationsKHR,
    .vkCmdSetLineStippleEXT = sqtt_CmdSetLineStippleEXT,
  };


/** Return true if the core version or extension in which the given entrypoint
 * is defined is enabled.
 *
 * If device is NULL, all device extensions are considered enabled.
 */
bool
radv_instance_entrypoint_is_enabled(int index, uint32_t core_version,
                                   const struct radv_instance_extension_table *instance)
{
   switch (index) {
   case 0:
      /* vkCreateInstance */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 1:
      /* vkDestroyInstance */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 2:
      /* vkEnumeratePhysicalDevices */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 3:
      /* vkGetInstanceProcAddr */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 4:
      /* vkEnumerateInstanceVersion */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 5:
      /* vkEnumerateInstanceLayerProperties */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 6:
      /* vkEnumerateInstanceExtensionProperties */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 7:
      /* vkCreateDisplayPlaneSurfaceKHR */
      if (instance->KHR_display) return true;
      return false;
   case 8:
      /* vkDestroySurfaceKHR */
      if (instance->KHR_surface) return true;
      return false;
   case 9:
      /* vkCreateWaylandSurfaceKHR */
      if (instance->KHR_wayland_surface) return true;
      return false;
   case 10:
      /* vkCreateXlibSurfaceKHR */
      if (instance->KHR_xlib_surface) return true;
      return false;
   case 11:
      /* vkCreateXcbSurfaceKHR */
      if (instance->KHR_xcb_surface) return true;
      return false;
   case 12:
      /* vkCreateDebugReportCallbackEXT */
      if (instance->EXT_debug_report) return true;
      return false;
   case 13:
      /* vkDestroyDebugReportCallbackEXT */
      if (instance->EXT_debug_report) return true;
      return false;
   case 14:
      /* vkDebugReportMessageEXT */
      if (instance->EXT_debug_report) return true;
      return false;
   case 15:
      /* vkEnumeratePhysicalDeviceGroups */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 16:
      /* vkEnumeratePhysicalDeviceGroupsKHR */
      if (instance->KHR_device_group_creation) return true;
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
bool
radv_physical_device_entrypoint_is_enabled(int index, uint32_t core_version,
                                          const struct radv_instance_extension_table *instance)
{
   switch (index) {
   case 0:
      /* vkGetPhysicalDeviceProperties */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 1:
      /* vkGetPhysicalDeviceQueueFamilyProperties */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 2:
      /* vkGetPhysicalDeviceMemoryProperties */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 3:
      /* vkGetPhysicalDeviceFeatures */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 4:
      /* vkGetPhysicalDeviceFormatProperties */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 5:
      /* vkGetPhysicalDeviceImageFormatProperties */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 6:
      /* vkCreateDevice */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 7:
      /* vkEnumerateDeviceLayerProperties */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 8:
      /* vkEnumerateDeviceExtensionProperties */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 9:
      /* vkGetPhysicalDeviceSparseImageFormatProperties */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 10:
      /* vkGetPhysicalDeviceDisplayPropertiesKHR */
      if (instance->KHR_display) return true;
      return false;
   case 11:
      /* vkGetPhysicalDeviceDisplayPlanePropertiesKHR */
      if (instance->KHR_display) return true;
      return false;
   case 12:
      /* vkGetDisplayPlaneSupportedDisplaysKHR */
      if (instance->KHR_display) return true;
      return false;
   case 13:
      /* vkGetDisplayModePropertiesKHR */
      if (instance->KHR_display) return true;
      return false;
   case 14:
      /* vkCreateDisplayModeKHR */
      if (instance->KHR_display) return true;
      return false;
   case 15:
      /* vkGetDisplayPlaneCapabilitiesKHR */
      if (instance->KHR_display) return true;
      return false;
   case 16:
      /* vkGetPhysicalDeviceSurfaceSupportKHR */
      if (instance->KHR_surface) return true;
      return false;
   case 17:
      /* vkGetPhysicalDeviceSurfaceCapabilitiesKHR */
      if (instance->KHR_surface) return true;
      return false;
   case 18:
      /* vkGetPhysicalDeviceSurfaceFormatsKHR */
      if (instance->KHR_surface) return true;
      return false;
   case 19:
      /* vkGetPhysicalDeviceSurfacePresentModesKHR */
      if (instance->KHR_surface) return true;
      return false;
   case 20:
      /* vkGetPhysicalDeviceWaylandPresentationSupportKHR */
      if (instance->KHR_wayland_surface) return true;
      return false;
   case 21:
      /* vkGetPhysicalDeviceXlibPresentationSupportKHR */
      if (instance->KHR_xlib_surface) return true;
      return false;
   case 22:
      /* vkGetPhysicalDeviceXcbPresentationSupportKHR */
      if (instance->KHR_xcb_surface) return true;
      return false;
   case 23:
      /* vkGetPhysicalDeviceFeatures2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 24:
      /* vkGetPhysicalDeviceFeatures2KHR */
      if (instance->KHR_get_physical_device_properties2) return true;
      return false;
   case 25:
      /* vkGetPhysicalDeviceProperties2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 26:
      /* vkGetPhysicalDeviceProperties2KHR */
      if (instance->KHR_get_physical_device_properties2) return true;
      return false;
   case 27:
      /* vkGetPhysicalDeviceFormatProperties2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 28:
      /* vkGetPhysicalDeviceFormatProperties2KHR */
      if (instance->KHR_get_physical_device_properties2) return true;
      return false;
   case 29:
      /* vkGetPhysicalDeviceImageFormatProperties2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 30:
      /* vkGetPhysicalDeviceImageFormatProperties2KHR */
      if (instance->KHR_get_physical_device_properties2) return true;
      return false;
   case 31:
      /* vkGetPhysicalDeviceQueueFamilyProperties2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 32:
      /* vkGetPhysicalDeviceQueueFamilyProperties2KHR */
      if (instance->KHR_get_physical_device_properties2) return true;
      return false;
   case 33:
      /* vkGetPhysicalDeviceMemoryProperties2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 34:
      /* vkGetPhysicalDeviceMemoryProperties2KHR */
      if (instance->KHR_get_physical_device_properties2) return true;
      return false;
   case 35:
      /* vkGetPhysicalDeviceSparseImageFormatProperties2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 36:
      /* vkGetPhysicalDeviceSparseImageFormatProperties2KHR */
      if (instance->KHR_get_physical_device_properties2) return true;
      return false;
   case 37:
      /* vkGetPhysicalDeviceExternalBufferProperties */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 38:
      /* vkGetPhysicalDeviceExternalBufferPropertiesKHR */
      if (instance->KHR_external_memory_capabilities) return true;
      return false;
   case 39:
      /* vkGetPhysicalDeviceExternalSemaphoreProperties */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 40:
      /* vkGetPhysicalDeviceExternalSemaphorePropertiesKHR */
      if (instance->KHR_external_semaphore_capabilities) return true;
      return false;
   case 41:
      /* vkGetPhysicalDeviceExternalFenceProperties */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 42:
      /* vkGetPhysicalDeviceExternalFencePropertiesKHR */
      if (instance->KHR_external_fence_capabilities) return true;
      return false;
   case 43:
      /* vkReleaseDisplayEXT */
      if (instance->EXT_direct_mode_display) return true;
      return false;
   case 44:
      /* vkAcquireXlibDisplayEXT */
      if (instance->EXT_acquire_xlib_display) return true;
      return false;
   case 45:
      /* vkGetRandROutputDisplayEXT */
      if (instance->EXT_acquire_xlib_display) return true;
      return false;
   case 46:
      /* vkGetPhysicalDeviceSurfaceCapabilities2EXT */
      if (instance->EXT_display_surface_counter) return true;
      return false;
   case 47:
      /* vkGetPhysicalDevicePresentRectanglesKHR */
      /* All device extensions are considered enabled at the instance level */
      return true;
      /* All device extensions are considered enabled at the instance level */
      return true;
      return false;
   case 48:
      /* vkGetPhysicalDeviceMultisamplePropertiesEXT */
      /* All device extensions are considered enabled at the instance level */
      return true;
      return false;
   case 49:
      /* vkGetPhysicalDeviceSurfaceCapabilities2KHR */
      if (instance->KHR_get_surface_capabilities2) return true;
      return false;
   case 50:
      /* vkGetPhysicalDeviceSurfaceFormats2KHR */
      if (instance->KHR_get_surface_capabilities2) return true;
      return false;
   case 51:
      /* vkGetPhysicalDeviceDisplayProperties2KHR */
      if (instance->KHR_get_display_properties2) return true;
      return false;
   case 52:
      /* vkGetPhysicalDeviceDisplayPlaneProperties2KHR */
      if (instance->KHR_get_display_properties2) return true;
      return false;
   case 53:
      /* vkGetDisplayModeProperties2KHR */
      if (instance->KHR_get_display_properties2) return true;
      return false;
   case 54:
      /* vkGetDisplayPlaneCapabilities2KHR */
      if (instance->KHR_get_display_properties2) return true;
      return false;
   case 55:
      /* vkGetPhysicalDeviceCalibrateableTimeDomainsEXT */
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
bool
radv_device_entrypoint_is_enabled(int index, uint32_t core_version,
                                 const struct radv_instance_extension_table *instance,
                                 const struct radv_device_extension_table *device)
{
   switch (index) {
   case 0:
      /* vkGetDeviceProcAddr */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 1:
      /* vkDestroyDevice */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 2:
      /* vkGetDeviceQueue */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 3:
      /* vkQueueSubmit */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 4:
      /* vkQueueWaitIdle */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 5:
      /* vkDeviceWaitIdle */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 6:
      /* vkAllocateMemory */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 7:
      /* vkFreeMemory */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 8:
      /* vkMapMemory */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 9:
      /* vkUnmapMemory */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 10:
      /* vkFlushMappedMemoryRanges */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 11:
      /* vkInvalidateMappedMemoryRanges */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 12:
      /* vkGetDeviceMemoryCommitment */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 13:
      /* vkGetBufferMemoryRequirements */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 14:
      /* vkBindBufferMemory */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 15:
      /* vkGetImageMemoryRequirements */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 16:
      /* vkBindImageMemory */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 17:
      /* vkGetImageSparseMemoryRequirements */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 18:
      /* vkQueueBindSparse */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 19:
      /* vkCreateFence */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 20:
      /* vkDestroyFence */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 21:
      /* vkResetFences */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 22:
      /* vkGetFenceStatus */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 23:
      /* vkWaitForFences */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 24:
      /* vkCreateSemaphore */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 25:
      /* vkDestroySemaphore */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 26:
      /* vkCreateEvent */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 27:
      /* vkDestroyEvent */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 28:
      /* vkGetEventStatus */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 29:
      /* vkSetEvent */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 30:
      /* vkResetEvent */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 31:
      /* vkCreateQueryPool */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 32:
      /* vkDestroyQueryPool */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 33:
      /* vkGetQueryPoolResults */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 34:
      /* vkResetQueryPool */
      return VK_MAKE_VERSION(1, 2, 0) <= core_version;
   case 35:
      /* vkResetQueryPoolEXT */
      if (!device || device->EXT_host_query_reset) return true;
      return false;
   case 36:
      /* vkCreateBuffer */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 37:
      /* vkDestroyBuffer */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 38:
      /* vkCreateBufferView */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 39:
      /* vkDestroyBufferView */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 40:
      /* vkCreateImage */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 41:
      /* vkDestroyImage */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 42:
      /* vkGetImageSubresourceLayout */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 43:
      /* vkCreateImageView */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 44:
      /* vkDestroyImageView */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 45:
      /* vkCreateShaderModule */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 46:
      /* vkDestroyShaderModule */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 47:
      /* vkCreatePipelineCache */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 48:
      /* vkDestroyPipelineCache */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 49:
      /* vkGetPipelineCacheData */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 50:
      /* vkMergePipelineCaches */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 51:
      /* vkCreateGraphicsPipelines */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 52:
      /* vkCreateComputePipelines */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 53:
      /* vkDestroyPipeline */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 54:
      /* vkCreatePipelineLayout */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 55:
      /* vkDestroyPipelineLayout */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 56:
      /* vkCreateSampler */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 57:
      /* vkDestroySampler */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 58:
      /* vkCreateDescriptorSetLayout */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 59:
      /* vkDestroyDescriptorSetLayout */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 60:
      /* vkCreateDescriptorPool */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 61:
      /* vkDestroyDescriptorPool */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 62:
      /* vkResetDescriptorPool */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 63:
      /* vkAllocateDescriptorSets */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 64:
      /* vkFreeDescriptorSets */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 65:
      /* vkUpdateDescriptorSets */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 66:
      /* vkCreateFramebuffer */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 67:
      /* vkDestroyFramebuffer */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 68:
      /* vkCreateRenderPass */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 69:
      /* vkDestroyRenderPass */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 70:
      /* vkGetRenderAreaGranularity */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 71:
      /* vkCreateCommandPool */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 72:
      /* vkDestroyCommandPool */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 73:
      /* vkResetCommandPool */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 74:
      /* vkAllocateCommandBuffers */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 75:
      /* vkFreeCommandBuffers */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 76:
      /* vkBeginCommandBuffer */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 77:
      /* vkEndCommandBuffer */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 78:
      /* vkResetCommandBuffer */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 79:
      /* vkCmdBindPipeline */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 80:
      /* vkCmdSetViewport */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 81:
      /* vkCmdSetScissor */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 82:
      /* vkCmdSetLineWidth */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 83:
      /* vkCmdSetDepthBias */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 84:
      /* vkCmdSetBlendConstants */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 85:
      /* vkCmdSetDepthBounds */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 86:
      /* vkCmdSetStencilCompareMask */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 87:
      /* vkCmdSetStencilWriteMask */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 88:
      /* vkCmdSetStencilReference */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 89:
      /* vkCmdBindDescriptorSets */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 90:
      /* vkCmdBindIndexBuffer */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 91:
      /* vkCmdBindVertexBuffers */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 92:
      /* vkCmdDraw */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 93:
      /* vkCmdDrawIndexed */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 94:
      /* vkCmdDrawIndirect */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 95:
      /* vkCmdDrawIndexedIndirect */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 96:
      /* vkCmdDispatch */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 97:
      /* vkCmdDispatchIndirect */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 98:
      /* vkCmdCopyBuffer */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 99:
      /* vkCmdCopyImage */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 100:
      /* vkCmdBlitImage */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 101:
      /* vkCmdCopyBufferToImage */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 102:
      /* vkCmdCopyImageToBuffer */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 103:
      /* vkCmdUpdateBuffer */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 104:
      /* vkCmdFillBuffer */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 105:
      /* vkCmdClearColorImage */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 106:
      /* vkCmdClearDepthStencilImage */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 107:
      /* vkCmdClearAttachments */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 108:
      /* vkCmdResolveImage */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 109:
      /* vkCmdSetEvent */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 110:
      /* vkCmdResetEvent */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 111:
      /* vkCmdWaitEvents */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 112:
      /* vkCmdPipelineBarrier */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 113:
      /* vkCmdBeginQuery */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 114:
      /* vkCmdEndQuery */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 115:
      /* vkCmdBeginConditionalRenderingEXT */
      if (!device || device->EXT_conditional_rendering) return true;
      return false;
   case 116:
      /* vkCmdEndConditionalRenderingEXT */
      if (!device || device->EXT_conditional_rendering) return true;
      return false;
   case 117:
      /* vkCmdResetQueryPool */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 118:
      /* vkCmdWriteTimestamp */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 119:
      /* vkCmdCopyQueryPoolResults */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 120:
      /* vkCmdPushConstants */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 121:
      /* vkCmdBeginRenderPass */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 122:
      /* vkCmdNextSubpass */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 123:
      /* vkCmdEndRenderPass */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 124:
      /* vkCmdExecuteCommands */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 125:
      /* vkCreateSwapchainKHR */
      if (!device || device->KHR_swapchain) return true;
      return false;
   case 126:
      /* vkDestroySwapchainKHR */
      if (!device || device->KHR_swapchain) return true;
      return false;
   case 127:
      /* vkGetSwapchainImagesKHR */
      if (!device || device->KHR_swapchain) return true;
      return false;
   case 128:
      /* vkAcquireNextImageKHR */
      if (!device || device->KHR_swapchain) return true;
      return false;
   case 129:
      /* vkQueuePresentKHR */
      if (!device || device->KHR_swapchain) return true;
      return false;
   case 130:
      /* vkCmdPushDescriptorSetKHR */
      if (!device || device->KHR_push_descriptor) return true;
      return false;
   case 131:
      /* vkTrimCommandPool */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 132:
      /* vkTrimCommandPoolKHR */
      if (!device || device->KHR_maintenance1) return true;
      return false;
   case 133:
      /* vkGetMemoryFdKHR */
      if (!device || device->KHR_external_memory_fd) return true;
      return false;
   case 134:
      /* vkGetMemoryFdPropertiesKHR */
      if (!device || device->KHR_external_memory_fd) return true;
      return false;
   case 135:
      /* vkGetSemaphoreFdKHR */
      if (!device || device->KHR_external_semaphore_fd) return true;
      return false;
   case 136:
      /* vkImportSemaphoreFdKHR */
      if (!device || device->KHR_external_semaphore_fd) return true;
      return false;
   case 137:
      /* vkGetFenceFdKHR */
      if (!device || device->KHR_external_fence_fd) return true;
      return false;
   case 138:
      /* vkImportFenceFdKHR */
      if (!device || device->KHR_external_fence_fd) return true;
      return false;
   case 139:
      /* vkDisplayPowerControlEXT */
      if (!device || device->EXT_display_control) return true;
      return false;
   case 140:
      /* vkRegisterDeviceEventEXT */
      if (!device || device->EXT_display_control) return true;
      return false;
   case 141:
      /* vkRegisterDisplayEventEXT */
      if (!device || device->EXT_display_control) return true;
      return false;
   case 142:
      /* vkGetSwapchainCounterEXT */
      if (!device || device->EXT_display_control) return true;
      return false;
   case 143:
      /* vkGetDeviceGroupPeerMemoryFeatures */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 144:
      /* vkGetDeviceGroupPeerMemoryFeaturesKHR */
      if (!device || device->KHR_device_group) return true;
      return false;
   case 145:
      /* vkBindBufferMemory2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 146:
      /* vkBindBufferMemory2KHR */
      if (!device || device->KHR_bind_memory2) return true;
      return false;
   case 147:
      /* vkBindImageMemory2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 148:
      /* vkBindImageMemory2KHR */
      if (!device || device->KHR_bind_memory2) return true;
      return false;
   case 149:
      /* vkCmdSetDeviceMask */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 150:
      /* vkCmdSetDeviceMaskKHR */
      if (!device || device->KHR_device_group) return true;
      return false;
   case 151:
      /* vkGetDeviceGroupPresentCapabilitiesKHR */
      if (!device || device->KHR_swapchain) return true;
      if (!device || device->KHR_device_group) return true;
      return false;
   case 152:
      /* vkGetDeviceGroupSurfacePresentModesKHR */
      if (!device || device->KHR_swapchain) return true;
      if (!device || device->KHR_device_group) return true;
      return false;
   case 153:
      /* vkAcquireNextImage2KHR */
      if (!device || device->KHR_swapchain) return true;
      if (!device || device->KHR_device_group) return true;
      return false;
   case 154:
      /* vkCmdDispatchBase */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 155:
      /* vkCmdDispatchBaseKHR */
      if (!device || device->KHR_device_group) return true;
      return false;
   case 156:
      /* vkCreateDescriptorUpdateTemplate */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 157:
      /* vkCreateDescriptorUpdateTemplateKHR */
      if (!device || device->KHR_descriptor_update_template) return true;
      return false;
   case 158:
      /* vkDestroyDescriptorUpdateTemplate */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 159:
      /* vkDestroyDescriptorUpdateTemplateKHR */
      if (!device || device->KHR_descriptor_update_template) return true;
      return false;
   case 160:
      /* vkUpdateDescriptorSetWithTemplate */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 161:
      /* vkUpdateDescriptorSetWithTemplateKHR */
      if (!device || device->KHR_descriptor_update_template) return true;
      return false;
   case 162:
      /* vkCmdPushDescriptorSetWithTemplateKHR */
      if (!device || device->KHR_push_descriptor) return true;
      if (!device || device->KHR_descriptor_update_template) return true;
      return false;
   case 163:
      /* vkCmdSetDiscardRectangleEXT */
      if (!device || device->EXT_discard_rectangles) return true;
      return false;
   case 164:
      /* vkCmdSetSampleLocationsEXT */
      if (!device || device->EXT_sample_locations) return true;
      return false;
   case 165:
      /* vkGetBufferMemoryRequirements2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 166:
      /* vkGetBufferMemoryRequirements2KHR */
      if (!device || device->KHR_get_memory_requirements2) return true;
      return false;
   case 167:
      /* vkGetImageMemoryRequirements2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 168:
      /* vkGetImageMemoryRequirements2KHR */
      if (!device || device->KHR_get_memory_requirements2) return true;
      return false;
   case 169:
      /* vkGetImageSparseMemoryRequirements2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 170:
      /* vkGetImageSparseMemoryRequirements2KHR */
      if (!device || device->KHR_get_memory_requirements2) return true;
      return false;
   case 171:
      /* vkCreateSamplerYcbcrConversion */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 172:
      /* vkCreateSamplerYcbcrConversionKHR */
      if (!device || device->KHR_sampler_ycbcr_conversion) return true;
      return false;
   case 173:
      /* vkDestroySamplerYcbcrConversion */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 174:
      /* vkDestroySamplerYcbcrConversionKHR */
      if (!device || device->KHR_sampler_ycbcr_conversion) return true;
      return false;
   case 175:
      /* vkGetDeviceQueue2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 176:
      /* vkGetDescriptorSetLayoutSupport */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 177:
      /* vkGetDescriptorSetLayoutSupportKHR */
      if (!device || device->KHR_maintenance3) return true;
      return false;
   case 178:
      /* vkGetSwapchainGrallocUsageANDROID */
      if (!device || device->ANDROID_native_buffer) return true;
      return false;
   case 179:
      /* vkGetSwapchainGrallocUsage2ANDROID */
      if (!device || device->ANDROID_native_buffer) return true;
      return false;
   case 180:
      /* vkAcquireImageANDROID */
      if (!device || device->ANDROID_native_buffer) return true;
      return false;
   case 181:
      /* vkQueueSignalReleaseImageANDROID */
      if (!device || device->ANDROID_native_buffer) return true;
      return false;
   case 182:
      /* vkGetShaderInfoAMD */
      if (!device || device->AMD_shader_info) return true;
      return false;
   case 183:
      /* vkGetCalibratedTimestampsEXT */
      if (!device || device->EXT_calibrated_timestamps) return true;
      return false;
   case 184:
      /* vkGetMemoryHostPointerPropertiesEXT */
      if (!device || device->EXT_external_memory_host) return true;
      return false;
   case 185:
      /* vkCmdWriteBufferMarkerAMD */
      if (!device || device->AMD_buffer_marker) return true;
      return false;
   case 186:
      /* vkCreateRenderPass2 */
      return VK_MAKE_VERSION(1, 2, 0) <= core_version;
   case 187:
      /* vkCreateRenderPass2KHR */
      if (!device || device->KHR_create_renderpass2) return true;
      return false;
   case 188:
      /* vkCmdBeginRenderPass2 */
      return VK_MAKE_VERSION(1, 2, 0) <= core_version;
   case 189:
      /* vkCmdBeginRenderPass2KHR */
      if (!device || device->KHR_create_renderpass2) return true;
      return false;
   case 190:
      /* vkCmdNextSubpass2 */
      return VK_MAKE_VERSION(1, 2, 0) <= core_version;
   case 191:
      /* vkCmdNextSubpass2KHR */
      if (!device || device->KHR_create_renderpass2) return true;
      return false;
   case 192:
      /* vkCmdEndRenderPass2 */
      return VK_MAKE_VERSION(1, 2, 0) <= core_version;
   case 193:
      /* vkCmdEndRenderPass2KHR */
      if (!device || device->KHR_create_renderpass2) return true;
      return false;
   case 194:
      /* vkGetSemaphoreCounterValue */
      return VK_MAKE_VERSION(1, 2, 0) <= core_version;
   case 195:
      /* vkGetSemaphoreCounterValueKHR */
      if (!device || device->KHR_timeline_semaphore) return true;
      return false;
   case 196:
      /* vkWaitSemaphores */
      return VK_MAKE_VERSION(1, 2, 0) <= core_version;
   case 197:
      /* vkWaitSemaphoresKHR */
      if (!device || device->KHR_timeline_semaphore) return true;
      return false;
   case 198:
      /* vkSignalSemaphore */
      return VK_MAKE_VERSION(1, 2, 0) <= core_version;
   case 199:
      /* vkSignalSemaphoreKHR */
      if (!device || device->KHR_timeline_semaphore) return true;
      return false;
   case 200:
      /* vkGetAndroidHardwareBufferPropertiesANDROID */
      if (!device || device->ANDROID_external_memory_android_hardware_buffer) return true;
      return false;
   case 201:
      /* vkGetMemoryAndroidHardwareBufferANDROID */
      if (!device || device->ANDROID_external_memory_android_hardware_buffer) return true;
      return false;
   case 202:
      /* vkCmdDrawIndirectCount */
      return VK_MAKE_VERSION(1, 2, 0) <= core_version;
   case 203:
      /* vkCmdDrawIndirectCountKHR */
      if (!device || device->KHR_draw_indirect_count) return true;
      return false;
   case 204:
      /* vkCmdDrawIndirectCountAMD */
      if (!device || device->AMD_draw_indirect_count) return true;
      return false;
   case 205:
      /* vkCmdDrawIndexedIndirectCount */
      return VK_MAKE_VERSION(1, 2, 0) <= core_version;
   case 206:
      /* vkCmdDrawIndexedIndirectCountKHR */
      if (!device || device->KHR_draw_indirect_count) return true;
      return false;
   case 207:
      /* vkCmdDrawIndexedIndirectCountAMD */
      if (!device || device->AMD_draw_indirect_count) return true;
      return false;
   case 208:
      /* vkCmdBindTransformFeedbackBuffersEXT */
      if (!device || device->EXT_transform_feedback) return true;
      return false;
   case 209:
      /* vkCmdBeginTransformFeedbackEXT */
      if (!device || device->EXT_transform_feedback) return true;
      return false;
   case 210:
      /* vkCmdEndTransformFeedbackEXT */
      if (!device || device->EXT_transform_feedback) return true;
      return false;
   case 211:
      /* vkCmdBeginQueryIndexedEXT */
      if (!device || device->EXT_transform_feedback) return true;
      return false;
   case 212:
      /* vkCmdEndQueryIndexedEXT */
      if (!device || device->EXT_transform_feedback) return true;
      return false;
   case 213:
      /* vkCmdDrawIndirectByteCountEXT */
      if (!device || device->EXT_transform_feedback) return true;
      return false;
   case 214:
      /* vkDestroyAccelerationStructureNV */
      return true;
   case 215:
      /* vkBindAccelerationStructureMemoryNV */
      return true;
   case 216:
      /* vkCmdWriteAccelerationStructuresPropertiesNV */
      return true;
   case 217:
      /* vkGetRayTracingShaderGroupHandlesNV */
      return true;
   case 218:
      /* vkGetBufferOpaqueCaptureAddress */
      return VK_MAKE_VERSION(1, 2, 0) <= core_version;
   case 219:
      /* vkGetBufferOpaqueCaptureAddressKHR */
      if (!device || device->KHR_buffer_device_address) return true;
      return false;
   case 220:
      /* vkGetBufferDeviceAddress */
      return VK_MAKE_VERSION(1, 2, 0) <= core_version;
   case 221:
      /* vkGetBufferDeviceAddressKHR */
      if (!device || device->KHR_buffer_device_address) return true;
      return false;
   case 222:
      /* vkGetBufferDeviceAddressEXT */
      if (!device || device->EXT_buffer_device_address) return true;
      return false;
   case 223:
      /* vkGetDeviceMemoryOpaqueCaptureAddress */
      return VK_MAKE_VERSION(1, 2, 0) <= core_version;
   case 224:
      /* vkGetDeviceMemoryOpaqueCaptureAddressKHR */
      if (!device || device->KHR_buffer_device_address) return true;
      return false;
   case 225:
      /* vkGetPipelineExecutablePropertiesKHR */
      if (!device || device->KHR_pipeline_executable_properties) return true;
      return false;
   case 226:
      /* vkGetPipelineExecutableStatisticsKHR */
      if (!device || device->KHR_pipeline_executable_properties) return true;
      return false;
   case 227:
      /* vkGetPipelineExecutableInternalRepresentationsKHR */
      if (!device || device->KHR_pipeline_executable_properties) return true;
      return false;
   case 228:
      /* vkCmdSetLineStippleEXT */
      if (!device || device->EXT_line_rasterization) return true;
      return false;
   default:
      return false;
   }
}

int
radv_get_instance_entrypoint_index(const char *name)
{
   return instance_string_map_lookup(name);
}

int
radv_get_physical_device_entrypoint_index(const char *name)
{
   return physical_device_string_map_lookup(name);
}

int
radv_get_device_entrypoint_index(const char *name)
{
   return device_string_map_lookup(name);
}

const char *
radv_get_instance_entry_name(int index)
{
   return instance_entry_name(index);
}

const char *
radv_get_physical_device_entry_name(int index)
{
   return physical_device_entry_name(index);
}

const char *
radv_get_device_entry_name(int index)
{
   return device_entry_name(index);
}

static void * __attribute__ ((noinline))
radv_resolve_device_entrypoint(uint32_t index)
{
    return radv_device_dispatch_table.entrypoints[index];
}

void *
radv_lookup_entrypoint(const char *name)
{
   int idx = radv_get_instance_entrypoint_index(name);
   if (idx >= 0)
      return radv_instance_dispatch_table.entrypoints[idx];

   idx = radv_get_physical_device_entrypoint_index(name);
   if (idx >= 0)
      return radv_physical_device_dispatch_table.entrypoints[idx];

   idx = radv_get_device_entrypoint_index(name);
   if (idx >= 0)
      return radv_resolve_device_entrypoint(idx);

   return NULL;
}