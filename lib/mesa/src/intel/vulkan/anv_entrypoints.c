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

/* This file generated from anv_entrypoints_gen.py, don't edit directly. */

#include "anv_private.h"

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
    "vkAcquireXlibDisplayEXT\0"
    "vkCreateDebugReportCallbackEXT\0"
    "vkCreateDevice\0"
    "vkCreateDisplayModeKHR\0"
    "vkCreateDisplayPlaneSurfaceKHR\0"
    "vkCreateInstance\0"
    "vkCreateWaylandSurfaceKHR\0"
    "vkCreateXcbSurfaceKHR\0"
    "vkCreateXlibSurfaceKHR\0"
    "vkDebugReportMessageEXT\0"
    "vkDestroyDebugReportCallbackEXT\0"
    "vkDestroyInstance\0"
    "vkDestroySurfaceKHR\0"
    "vkEnumerateDeviceExtensionProperties\0"
    "vkEnumerateDeviceLayerProperties\0"
    "vkEnumerateInstanceExtensionProperties\0"
    "vkEnumerateInstanceLayerProperties\0"
    "vkEnumerateInstanceVersion\0"
    "vkEnumeratePhysicalDeviceGroups\0"
    "vkEnumeratePhysicalDeviceGroupsKHR\0"
    "vkEnumeratePhysicalDevices\0"
    "vkGetDisplayModeProperties2KHR\0"
    "vkGetDisplayModePropertiesKHR\0"
    "vkGetDisplayPlaneCapabilities2KHR\0"
    "vkGetDisplayPlaneCapabilitiesKHR\0"
    "vkGetDisplayPlaneSupportedDisplaysKHR\0"
    "vkGetInstanceProcAddr\0"
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

static const struct string_map_entry instance_string_map_entries[] = {
    { 0, 0x60df100d, 59 }, /* vkAcquireXlibDisplayEXT */
    { 24, 0x987ef56, 35 }, /* vkCreateDebugReportCallbackEXT */
    { 55, 0x85ed23f, 10 }, /* vkCreateDevice */
    { 70, 0xcc0bde41, 21 }, /* vkCreateDisplayModeKHR */
    { 93, 0x7ac4dacb, 23 }, /* vkCreateDisplayPlaneSurfaceKHR */
    { 124, 0x38a581a6, 0 }, /* vkCreateInstance */
    { 141, 0x2b2a4b79, 29 }, /* vkCreateWaylandSurfaceKHR */
    { 167, 0xc5e5b106, 33 }, /* vkCreateXcbSurfaceKHR */
    { 189, 0xa693bc66, 31 }, /* vkCreateXlibSurfaceKHR */
    { 212, 0xa4e75334, 37 }, /* vkDebugReportMessageEXT */
    { 236, 0x43d4c4e2, 36 }, /* vkDestroyDebugReportCallbackEXT */
    { 268, 0x9bd21af2, 1 }, /* vkDestroyInstance */
    { 286, 0xf204ce7d, 24 }, /* vkDestroySurfaceKHR */
    { 306, 0x5fd13eed, 15 }, /* vkEnumerateDeviceExtensionProperties */
    { 343, 0x2f8566e7, 14 }, /* vkEnumerateDeviceLayerProperties */
    { 376, 0xeb27627e, 13 }, /* vkEnumerateInstanceExtensionProperties */
    { 415, 0x81f69d8, 12 }, /* vkEnumerateInstanceLayerProperties */
    { 450, 0xd0481e5c, 11 }, /* vkEnumerateInstanceVersion */
    { 477, 0x270514f0, 62 }, /* vkEnumeratePhysicalDeviceGroups */
    { 509, 0x549ce595, 63 }, /* vkEnumeratePhysicalDeviceGroupsKHR */
    { 544, 0x5787c327, 2 }, /* vkEnumeratePhysicalDevices */
    { 571, 0x3e613e42, 69 }, /* vkGetDisplayModeProperties2KHR */
    { 602, 0x36b8a8de, 20 }, /* vkGetDisplayModePropertiesKHR */
    { 632, 0xff1655a4, 70 }, /* vkGetDisplayPlaneCapabilities2KHR */
    { 666, 0x4b60d48c, 22 }, /* vkGetDisplayPlaneCapabilitiesKHR */
    { 699, 0xabef4889, 19 }, /* vkGetDisplayPlaneSupportedDisplaysKHR */
    { 737, 0x3d2ae9ad, 3 }, /* vkGetInstanceProcAddr */
    { 759, 0xea07da1a, 71 }, /* vkGetPhysicalDeviceCalibrateableTimeDomainsEXT */
    { 806, 0xb7bc4386, 68 }, /* vkGetPhysicalDeviceDisplayPlaneProperties2KHR */
    { 852, 0xb9b8ddba, 18 }, /* vkGetPhysicalDeviceDisplayPlanePropertiesKHR */
    { 897, 0x540c0372, 67 }, /* vkGetPhysicalDeviceDisplayProperties2KHR */
    { 938, 0xfa0cd2e, 17 }, /* vkGetPhysicalDeviceDisplayPropertiesKHR */
    { 978, 0x944476dc, 52 }, /* vkGetPhysicalDeviceExternalBufferProperties */
    { 1022, 0xee68b389, 53 }, /* vkGetPhysicalDeviceExternalBufferPropertiesKHR */
    { 1069, 0x3bc965eb, 56 }, /* vkGetPhysicalDeviceExternalFenceProperties */
    { 1112, 0x99b35492, 57 }, /* vkGetPhysicalDeviceExternalFencePropertiesKHR */
    { 1158, 0xcf251b0e, 54 }, /* vkGetPhysicalDeviceExternalSemaphoreProperties */
    { 1205, 0x984c3fa7, 55 }, /* vkGetPhysicalDeviceExternalSemaphorePropertiesKHR */
    { 1255, 0x113e2f33, 7 }, /* vkGetPhysicalDeviceFeatures */
    { 1283, 0x63c068a7, 38 }, /* vkGetPhysicalDeviceFeatures2 */
    { 1312, 0x6a9a3636, 39 }, /* vkGetPhysicalDeviceFeatures2KHR */
    { 1344, 0x3e54b398, 8 }, /* vkGetPhysicalDeviceFormatProperties */
    { 1380, 0xca3bb9da, 42 }, /* vkGetPhysicalDeviceFormatProperties2 */
    { 1417, 0x9099cbbb, 43 }, /* vkGetPhysicalDeviceFormatProperties2KHR */
    { 1457, 0xdd36a867, 9 }, /* vkGetPhysicalDeviceImageFormatProperties */
    { 1498, 0x35d260d3, 44 }, /* vkGetPhysicalDeviceImageFormatProperties2 */
    { 1540, 0x102ff7ea, 45 }, /* vkGetPhysicalDeviceImageFormatProperties2KHR */
    { 1585, 0xa90da4da, 6 }, /* vkGetPhysicalDeviceMemoryProperties */
    { 1621, 0xcb4cc208, 48 }, /* vkGetPhysicalDeviceMemoryProperties2 */
    { 1658, 0xc8c3da3d, 49 }, /* vkGetPhysicalDeviceMemoryProperties2KHR */
    { 1698, 0x100341b4, 64 }, /* vkGetPhysicalDevicePresentRectanglesKHR */
    { 1738, 0x52fe22c9, 4 }, /* vkGetPhysicalDeviceProperties */
    { 1768, 0x6c4d8ee1, 40 }, /* vkGetPhysicalDeviceProperties2 */
    { 1799, 0xcd15838c, 41 }, /* vkGetPhysicalDeviceProperties2KHR */
    { 1833, 0x4e5fc88a, 5 }, /* vkGetPhysicalDeviceQueueFamilyProperties */
    { 1874, 0xcad374d8, 46 }, /* vkGetPhysicalDeviceQueueFamilyProperties2 */
    { 1916, 0x5ceb2bed, 47 }, /* vkGetPhysicalDeviceQueueFamilyProperties2KHR */
    { 1961, 0x272ef8ef, 16 }, /* vkGetPhysicalDeviceSparseImageFormatProperties */
    { 2008, 0xebddba0b, 50 }, /* vkGetPhysicalDeviceSparseImageFormatProperties2 */
    { 2056, 0x8746ed72, 51 }, /* vkGetPhysicalDeviceSparseImageFormatProperties2KHR */
    { 2107, 0x5a5fba04, 61 }, /* vkGetPhysicalDeviceSurfaceCapabilities2EXT */
    { 2150, 0x9497e378, 65 }, /* vkGetPhysicalDeviceSurfaceCapabilities2KHR */
    { 2193, 0x77890558, 26 }, /* vkGetPhysicalDeviceSurfaceCapabilitiesKHR */
    { 2235, 0xd00b7188, 66 }, /* vkGetPhysicalDeviceSurfaceFormats2KHR */
    { 2273, 0xe32227c8, 27 }, /* vkGetPhysicalDeviceSurfaceFormatsKHR */
    { 2310, 0x31c3cbd1, 28 }, /* vkGetPhysicalDeviceSurfacePresentModesKHR */
    { 2352, 0x1a687885, 25 }, /* vkGetPhysicalDeviceSurfaceSupportKHR */
    { 2389, 0x84e085ac, 30 }, /* vkGetPhysicalDeviceWaylandPresentationSupportKHR */
    { 2438, 0x41782cb9, 34 }, /* vkGetPhysicalDeviceXcbPresentationSupportKHR */
    { 2483, 0x34a063ab, 32 }, /* vkGetPhysicalDeviceXlibPresentationSupportKHR */
    { 2529, 0xb87cdd6c, 60 }, /* vkGetRandROutputDisplayEXT */
    { 2556, 0x4207f4f1, 58 }, /* vkReleaseDisplayEXT */
};

/* Hash table stats:
 * size 72 entries
 * collisions entries:
 *     0      54
 *     1      9
 *     2      3
 *     3      5
 *     4      0
 *     5      1
 *     6      0
 *     7      0
 *     8      0
 *     9+     0
 */

#define none 0xffff
static const uint16_t instance_string_map[128] = {
    0x002f,
    none,
    0x0039,
    none,
    0x003c,
    0x001e,
    0x0007,
    none,
    0x0030,
    0x0019,
    0x0036,
    0x003a,
    0x0018,
    0x0000,
    0x0024,
    none,
    none,
    0x0037,
    0x0023,
    0x0038,
    none,
    0x0013,
    none,
    none,
    0x0029,
    0x001c,
    0x001b,
    0x003f,
    0x0021,
    none,
    none,
    0x0035,
    none,
    none,
    none,
    none,
    0x0017,
    none,
    0x0005,
    0x0014,
    none,
    none,
    none,
    0x003b,
    0x0043,
    0x001a,
    0x001f,
    none,
    none,
    none,
    none,
    0x0026,
    0x0009,
    none,
    0x0028,
    0x003e,
    none,
    0x0044,
    0x001d,
    0x002b,
    none,
    0x0031,
    0x0042,
    0x0002,
    none,
    0x0003,
    0x0015,
    none,
    none,
    none,
    none,
    0x0032,
    0x0040,
    0x0033,
    none,
    0x0004,
    none,
    0x0025,
    none,
    none,
    none,
    0x0041,
    none,
    0x002d,
    none,
    none,
    0x0001,
    none,
    0x0010,
    none,
    0x002a,
    none,
    0x0011,
    none,
    0x0016,
    none,
    0x0027,
    0x0034,
    0x000a,
    none,
    0x0045,
    none,
    0x0008,
    0x000e,
    none,
    none,
    0x002e,
    0x0022,
    0x0046,
    0x000d,
    none,
    0x0020,
    0x0012,
    0x0047,
    0x000b,
    none,
    none,
    none,
    none,
    none,
    0x003d,
    0x0006,
    0x002c,
    none,
    none,
    0x000c,
    0x000f,
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
        i = instance_string_map[h & 127];
        if (i == none)
           return -1;
        e = &instance_string_map_entries[i];
        if (e->hash == hash && strcmp(str, instance_strings + e->name) == 0)
            return e->num;
        h += prime_step;
    }

    return -1;
}


static const char device_strings[] =
    "vkAcquireImageANDROID\0"
    "vkAcquireNextImage2KHR\0"
    "vkAcquireNextImageKHR\0"
    "vkAllocateCommandBuffers\0"
    "vkAllocateDescriptorSets\0"
    "vkAllocateMemory\0"
    "vkBeginCommandBuffer\0"
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
    "vkCmdDrawIndexedIndirectCountKHR\0"
    "vkCmdDrawIndirect\0"
    "vkCmdDrawIndirectByteCountEXT\0"
    "vkCmdDrawIndirectCountKHR\0"
    "vkCmdEndConditionalRenderingEXT\0"
    "vkCmdEndQuery\0"
    "vkCmdEndQueryIndexedEXT\0"
    "vkCmdEndRenderPass\0"
    "vkCmdEndRenderPass2KHR\0"
    "vkCmdEndTransformFeedbackEXT\0"
    "vkCmdExecuteCommands\0"
    "vkCmdFillBuffer\0"
    "vkCmdNextSubpass\0"
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
    "vkCmdSetEvent\0"
    "vkCmdSetLineStippleEXT\0"
    "vkCmdSetLineWidth\0"
    "vkCmdSetScissor\0"
    "vkCmdSetStencilCompareMask\0"
    "vkCmdSetStencilReference\0"
    "vkCmdSetStencilWriteMask\0"
    "vkCmdSetViewport\0"
    "vkCmdUpdateBuffer\0"
    "vkCmdWaitEvents\0"
    "vkCmdWriteTimestamp\0"
    "vkCreateBuffer\0"
    "vkCreateBufferView\0"
    "vkCreateCommandPool\0"
    "vkCreateComputePipelines\0"
    "vkCreateDescriptorPool\0"
    "vkCreateDescriptorSetLayout\0"
    "vkCreateDescriptorUpdateTemplate\0"
    "vkCreateDescriptorUpdateTemplateKHR\0"
    "vkCreateDmaBufImageINTEL\0"
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
    "vkCreateRenderPass2KHR\0"
    "vkCreateSampler\0"
    "vkCreateSamplerYcbcrConversion\0"
    "vkCreateSamplerYcbcrConversionKHR\0"
    "vkCreateSemaphore\0"
    "vkCreateShaderModule\0"
    "vkCreateSwapchainKHR\0"
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
    "vkGetBufferDeviceAddressEXT\0"
    "vkGetBufferMemoryRequirements\0"
    "vkGetBufferMemoryRequirements2\0"
    "vkGetBufferMemoryRequirements2KHR\0"
    "vkGetCalibratedTimestampsEXT\0"
    "vkGetDescriptorSetLayoutSupport\0"
    "vkGetDescriptorSetLayoutSupportKHR\0"
    "vkGetDeviceGroupPeerMemoryFeatures\0"
    "vkGetDeviceGroupPeerMemoryFeaturesKHR\0"
    "vkGetDeviceGroupPresentCapabilitiesKHR\0"
    "vkGetDeviceGroupSurfacePresentModesKHR\0"
    "vkGetDeviceMemoryCommitment\0"
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
    "vkGetRenderAreaGranularity\0"
    "vkGetSemaphoreFdKHR\0"
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
    "vkResetQueryPoolEXT\0"
    "vkSetEvent\0"
    "vkTrimCommandPool\0"
    "vkTrimCommandPoolKHR\0"
    "vkUnmapMemory\0"
    "vkUpdateDescriptorSetWithTemplate\0"
    "vkUpdateDescriptorSetWithTemplateKHR\0"
    "vkUpdateDescriptorSets\0"
    "vkWaitForFences\0"
;

static const struct string_map_entry device_string_map_entries[] = {
    { 0, 0x6bf780dd, 177 }, /* vkAcquireImageANDROID */
    { 22, 0x82860572, 152 }, /* vkAcquireNextImage2KHR */
    { 45, 0xc3fedb2e, 127 }, /* vkAcquireNextImageKHR */
    { 67, 0x8c0c811a, 73 }, /* vkAllocateCommandBuffers */
    { 92, 0x4c449d3a, 62 }, /* vkAllocateDescriptorSets */
    { 117, 0x522b85d3, 6 }, /* vkAllocateMemory */
    { 134, 0xc54f7327, 75 }, /* vkBeginCommandBuffer */
    { 155, 0x6bcbdcb, 14 }, /* vkBindBufferMemory */
    { 174, 0xc27aaf4f, 144 }, /* vkBindBufferMemory2 */
    { 194, 0x6878d3ce, 145 }, /* vkBindBufferMemory2KHR */
    { 217, 0x5caaae4a, 16 }, /* vkBindImageMemory */
    { 235, 0xa9097118, 146 }, /* vkBindImageMemory2 */
    { 254, 0xf18729ad, 147 }, /* vkBindImageMemory2KHR */
    { 276, 0xe561c19f, 114 }, /* vkCmdBeginConditionalRenderingEXT */
    { 310, 0xf5064ea4, 112 }, /* vkCmdBeginQuery */
    { 326, 0x73251a2c, 192 }, /* vkCmdBeginQueryIndexedEXT */
    { 352, 0xcb7a58e3, 120 }, /* vkCmdBeginRenderPass */
    { 373, 0x8b6b4de6, 182 }, /* vkCmdBeginRenderPass2KHR */
    { 398, 0xb217c94, 190 }, /* vkCmdBeginTransformFeedbackEXT */
    { 429, 0x28c7a5da, 88 }, /* vkCmdBindDescriptorSets */
    { 453, 0x4c22d870, 89 }, /* vkCmdBindIndexBuffer */
    { 474, 0x3af9fd84, 78 }, /* vkCmdBindPipeline */
    { 492, 0x98fdb5cd, 189 }, /* vkCmdBindTransformFeedbackBuffersEXT */
    { 529, 0xa9c83f1d, 90 }, /* vkCmdBindVertexBuffers */
    { 552, 0x331ebf89, 99 }, /* vkCmdBlitImage */
    { 567, 0x93cb5cb8, 106 }, /* vkCmdClearAttachments */
    { 589, 0xb4bc8d08, 104 }, /* vkCmdClearColorImage */
    { 610, 0x4f88e4ba, 105 }, /* vkCmdClearDepthStencilImage */
    { 638, 0xc939a0da, 97 }, /* vkCmdCopyBuffer */
    { 654, 0x929847e, 100 }, /* vkCmdCopyBufferToImage */
    { 677, 0x278effa9, 98 }, /* vkCmdCopyImage */
    { 692, 0x68cddbac, 101 }, /* vkCmdCopyImageToBuffer */
    { 715, 0xdee8c6d4, 118 }, /* vkCmdCopyQueryPoolResults */
    { 741, 0xbd58e867, 95 }, /* vkCmdDispatch */
    { 755, 0xfb767220, 153 }, /* vkCmdDispatchBase */
    { 773, 0x402403e5, 154 }, /* vkCmdDispatchBaseKHR */
    { 794, 0xd6353005, 96 }, /* vkCmdDispatchIndirect */
    { 816, 0x9912c1a1, 91 }, /* vkCmdDraw */
    { 826, 0xbe5a8058, 92 }, /* vkCmdDrawIndexed */
    { 843, 0x94e7ed36, 94 }, /* vkCmdDrawIndexedIndirect */
    { 868, 0xda9e8a2c, 188 }, /* vkCmdDrawIndexedIndirectCountKHR */
    { 901, 0xe9ac41bf, 93 }, /* vkCmdDrawIndirect */
    { 919, 0x80c3b089, 194 }, /* vkCmdDrawIndirectByteCountEXT */
    { 949, 0xf7dd01f5, 187 }, /* vkCmdDrawIndirectCountKHR */
    { 975, 0x18c8217d, 115 }, /* vkCmdEndConditionalRenderingEXT */
    { 1007, 0xd556fd22, 113 }, /* vkCmdEndQuery */
    { 1021, 0xd5c2f48a, 193 }, /* vkCmdEndQueryIndexedEXT */
    { 1045, 0xdcdb0235, 122 }, /* vkCmdEndRenderPass */
    { 1064, 0x57eebe78, 184 }, /* vkCmdEndRenderPass2KHR */
    { 1087, 0xf008d706, 191 }, /* vkCmdEndTransformFeedbackEXT */
    { 1116, 0x9eaabe40, 123 }, /* vkCmdExecuteCommands */
    { 1137, 0x5bdd2ae0, 103 }, /* vkCmdFillBuffer */
    { 1153, 0x2eeec2f9, 121 }, /* vkCmdNextSubpass */
    { 1170, 0x25b621bc, 183 }, /* vkCmdNextSubpass2KHR */
    { 1191, 0x97fccfe8, 111 }, /* vkCmdPipelineBarrier */
    { 1212, 0xb1c6b468, 119 }, /* vkCmdPushConstants */
    { 1231, 0xf17232a1, 129 }, /* vkCmdPushDescriptorSetKHR */
    { 1257, 0x3d528981, 161 }, /* vkCmdPushDescriptorSetWithTemplateKHR */
    { 1295, 0x4fccce28, 109 }, /* vkCmdResetEvent */
    { 1311, 0x2f614082, 116 }, /* vkCmdResetQueryPool */
    { 1331, 0x671bb594, 107 }, /* vkCmdResolveImage */
    { 1349, 0x1c989dfb, 83 }, /* vkCmdSetBlendConstants */
    { 1372, 0x30f14d07, 82 }, /* vkCmdSetDepthBias */
    { 1390, 0x7b3a8a63, 84 }, /* vkCmdSetDepthBounds */
    { 1410, 0xaecdae87, 148 }, /* vkCmdSetDeviceMask */
    { 1429, 0xfbb79356, 149 }, /* vkCmdSetDeviceMaskKHR */
    { 1451, 0xe257f075, 108 }, /* vkCmdSetEvent */
    { 1465, 0xbdaa62f9, 199 }, /* vkCmdSetLineStippleEXT */
    { 1488, 0x32282165, 81 }, /* vkCmdSetLineWidth */
    { 1506, 0x48f28c7f, 80 }, /* vkCmdSetScissor */
    { 1522, 0xa8f534e2, 85 }, /* vkCmdSetStencilCompareMask */
    { 1549, 0x83e2b024, 87 }, /* vkCmdSetStencilReference */
    { 1574, 0xe7c4b134, 86 }, /* vkCmdSetStencilWriteMask */
    { 1599, 0x53d6c2b, 79 }, /* vkCmdSetViewport */
    { 1616, 0xd2986b5e, 102 }, /* vkCmdUpdateBuffer */
    { 1634, 0x3b9346b3, 110 }, /* vkCmdWaitEvents */
    { 1650, 0xec4d324c, 117 }, /* vkCmdWriteTimestamp */
    { 1670, 0x7d4282b9, 35 }, /* vkCreateBuffer */
    { 1685, 0x925bd256, 37 }, /* vkCreateBufferView */
    { 1704, 0x820fe476, 70 }, /* vkCreateCommandPool */
    { 1724, 0xf70c85eb, 51 }, /* vkCreateComputePipelines */
    { 1749, 0xfb95a8a4, 59 }, /* vkCreateDescriptorPool */
    { 1772, 0x3c14cc74, 57 }, /* vkCreateDescriptorSetLayout */
    { 1800, 0xad3ce733, 155 }, /* vkCreateDescriptorUpdateTemplate */
    { 1833, 0x5189488a, 156 }, /* vkCreateDescriptorUpdateTemplateKHR */
    { 1869, 0x6392dfa7, 200 }, /* vkCreateDmaBufImageINTEL */
    { 1894, 0xe7188731, 26 }, /* vkCreateEvent */
    { 1908, 0x958af968, 19 }, /* vkCreateFence */
    { 1922, 0x887a38c4, 65 }, /* vkCreateFramebuffer */
    { 1942, 0x4b59f96d, 50 }, /* vkCreateGraphicsPipelines */
    { 1968, 0x652128c2, 39 }, /* vkCreateImage */
    { 1982, 0xdce077ff, 42 }, /* vkCreateImageView */
    { 2000, 0xcbf6489f, 46 }, /* vkCreatePipelineCache */
    { 2022, 0x451ef1ed, 53 }, /* vkCreatePipelineLayout */
    { 2045, 0x5edcd92b, 31 }, /* vkCreateQueryPool */
    { 2063, 0x109a9c18, 67 }, /* vkCreateRenderPass */
    { 2082, 0xfa16043b, 181 }, /* vkCreateRenderPass2KHR */
    { 2105, 0x13cf03f, 55 }, /* vkCreateSampler */
    { 2121, 0xe6a58c26, 168 }, /* vkCreateSamplerYcbcrConversion */
    { 2152, 0x7482104f, 169 }, /* vkCreateSamplerYcbcrConversionKHR */
    { 2186, 0xf2065e5b, 24 }, /* vkCreateSemaphore */
    { 2204, 0xa0d3cea2, 44 }, /* vkCreateShaderModule */
    { 2225, 0xcdefcaa8, 124 }, /* vkCreateSwapchainKHR */
    { 2246, 0x94a07a45, 36 }, /* vkDestroyBuffer */
    { 2262, 0x98b27962, 38 }, /* vkDestroyBufferView */
    { 2282, 0xd5d83a0a, 71 }, /* vkDestroyCommandPool */
    { 2303, 0x47bdaf30, 60 }, /* vkDestroyDescriptorPool */
    { 2327, 0xa4227b08, 58 }, /* vkDestroyDescriptorSetLayout */
    { 2356, 0xbb2cbe7f, 157 }, /* vkDestroyDescriptorUpdateTemplate */
    { 2390, 0xaa83901e, 158 }, /* vkDestroyDescriptorUpdateTemplateKHR */
    { 2427, 0x1fbcc9cb, 1 }, /* vkDestroyDevice */
    { 2443, 0x4df27c05, 27 }, /* vkDestroyEvent */
    { 2458, 0xfc64ee3c, 20 }, /* vkDestroyFence */
    { 2473, 0xdc428e58, 66 }, /* vkDestroyFramebuffer */
    { 2494, 0xcbfb1d96, 40 }, /* vkDestroyImage */
    { 2509, 0xb5853953, 43 }, /* vkDestroyImageView */
    { 2528, 0x6aac68af, 52 }, /* vkDestroyPipeline */
    { 2546, 0x4112a673, 47 }, /* vkDestroyPipelineCache */
    { 2569, 0x9146f879, 54 }, /* vkDestroyPipelineLayout */
    { 2593, 0x37819a7f, 32 }, /* vkDestroyQueryPool */
    { 2612, 0x16f14324, 68 }, /* vkDestroyRenderPass */
    { 2632, 0x3b645153, 56 }, /* vkDestroySampler */
    { 2649, 0x20f261b2, 170 }, /* vkDestroySamplerYcbcrConversion */
    { 2681, 0xaaa623a3, 171 }, /* vkDestroySamplerYcbcrConversionKHR */
    { 2716, 0xcaab1faf, 25 }, /* vkDestroySemaphore */
    { 2735, 0x2d77af6e, 45 }, /* vkDestroyShaderModule */
    { 2757, 0x5a93ab74, 125 }, /* vkDestroySwapchainKHR */
    { 2779, 0xd46c5f24, 5 }, /* vkDeviceWaitIdle */
    { 2796, 0xdbb064, 138 }, /* vkDisplayPowerControlEXT */
    { 2821, 0xaffb5725, 76 }, /* vkEndCommandBuffer */
    { 2840, 0xff52f051, 10 }, /* vkFlushMappedMemoryRanges */
    { 2866, 0xb9db2b91, 74 }, /* vkFreeCommandBuffers */
    { 2887, 0x7a1347b1, 63 }, /* vkFreeDescriptorSets */
    { 2908, 0x8f6f838a, 7 }, /* vkFreeMemory */
    { 2921, 0xb891b5e, 185 }, /* vkGetAndroidHardwareBufferPropertiesANDROID */
    { 2965, 0x3703280c, 195 }, /* vkGetBufferDeviceAddressEXT */
    { 2993, 0xab98422a, 13 }, /* vkGetBufferMemoryRequirements */
    { 3023, 0xd1fd0638, 162 }, /* vkGetBufferMemoryRequirements2 */
    { 3054, 0x78dbe98d, 163 }, /* vkGetBufferMemoryRequirements2KHR */
    { 3088, 0xcf3070fe, 179 }, /* vkGetCalibratedTimestampsEXT */
    { 3117, 0xfeac9573, 173 }, /* vkGetDescriptorSetLayoutSupport */
    { 3149, 0xd7e44a, 174 }, /* vkGetDescriptorSetLayoutSupportKHR */
    { 3184, 0x2e218c10, 142 }, /* vkGetDeviceGroupPeerMemoryFeatures */
    { 3219, 0xa3809375, 143 }, /* vkGetDeviceGroupPeerMemoryFeaturesKHR */
    { 3257, 0xf72c87d4, 150 }, /* vkGetDeviceGroupPresentCapabilitiesKHR */
    { 3296, 0x6b9448c3, 151 }, /* vkGetDeviceGroupSurfacePresentModesKHR */
    { 3335, 0x46e38db5, 12 }, /* vkGetDeviceMemoryCommitment */
    { 3363, 0xba013486, 0 }, /* vkGetDeviceProcAddr */
    { 3383, 0xcc920d9a, 2 }, /* vkGetDeviceQueue */
    { 3400, 0xb11a6348, 172 }, /* vkGetDeviceQueue2 */
    { 3418, 0x96d834b, 28 }, /* vkGetEventStatus */
    { 3435, 0x69a5d6af, 136 }, /* vkGetFenceFdKHR */
    { 3451, 0x5f391892, 22 }, /* vkGetFenceStatus */
    { 3468, 0x916f1e63, 15 }, /* vkGetImageMemoryRequirements */
    { 3497, 0x56e213f7, 164 }, /* vkGetImageMemoryRequirements2 */
    { 3527, 0x8de28366, 165 }, /* vkGetImageMemoryRequirements2KHR */
    { 3560, 0x15855f5b, 17 }, /* vkGetImageSparseMemoryRequirements */
    { 3595, 0xbd4e3d3f, 166 }, /* vkGetImageSparseMemoryRequirements2 */
    { 3631, 0x3df40f5e, 167 }, /* vkGetImageSparseMemoryRequirements2KHR */
    { 3670, 0x9163b686, 41 }, /* vkGetImageSubresourceLayout */
    { 3698, 0x71220e82, 186 }, /* vkGetMemoryAndroidHardwareBufferANDROID */
    { 3738, 0x503c14c5, 132 }, /* vkGetMemoryFdKHR */
    { 3755, 0xb028a792, 133 }, /* vkGetMemoryFdPropertiesKHR */
    { 3782, 0x7030ee5b, 180 }, /* vkGetMemoryHostPointerPropertiesEXT */
    { 3818, 0x2092a349, 48 }, /* vkGetPipelineCacheData */
    { 3841, 0x8b20fc09, 198 }, /* vkGetPipelineExecutableInternalRepresentationsKHR */
    { 3891, 0x748dd8cd, 196 }, /* vkGetPipelineExecutablePropertiesKHR */
    { 3928, 0x5c4d6435, 197 }, /* vkGetPipelineExecutableStatisticsKHR */
    { 3965, 0xbf3f2cb3, 33 }, /* vkGetQueryPoolResults */
    { 3987, 0xa9820d22, 69 }, /* vkGetRenderAreaGranularity */
    { 4014, 0x3e0e9884, 134 }, /* vkGetSemaphoreFdKHR */
    { 4034, 0xa4aeb5a, 141 }, /* vkGetSwapchainCounterEXT */
    { 4059, 0x219d929, 176 }, /* vkGetSwapchainGrallocUsage2ANDROID */
    { 4094, 0x4979c9a3, 175 }, /* vkGetSwapchainGrallocUsageANDROID */
    { 4128, 0x57695f28, 126 }, /* vkGetSwapchainImagesKHR */
    { 4152, 0x51df0390, 137 }, /* vkImportFenceFdKHR */
    { 4171, 0x36337c05, 135 }, /* vkImportSemaphoreFdKHR */
    { 4194, 0x1e115cca, 11 }, /* vkInvalidateMappedMemoryRanges */
    { 4225, 0xcb977bd8, 8 }, /* vkMapMemory */
    { 4237, 0xc3499606, 49 }, /* vkMergePipelineCaches */
    { 4259, 0xc3628a09, 18 }, /* vkQueueBindSparse */
    { 4277, 0xfc5fb6ce, 128 }, /* vkQueuePresentKHR */
    { 4295, 0xa0313eef, 178 }, /* vkQueueSignalReleaseImageANDROID */
    { 4328, 0xfa4713ec, 3 }, /* vkQueueSubmit */
    { 4342, 0x6f8fc2a5, 4 }, /* vkQueueWaitIdle */
    { 4358, 0x26cc78f5, 139 }, /* vkRegisterDeviceEventEXT */
    { 4383, 0x4a0bd849, 140 }, /* vkRegisterDisplayEventEXT */
    { 4409, 0x847dc731, 77 }, /* vkResetCommandBuffer */
    { 4430, 0x6da9f7fd, 72 }, /* vkResetCommandPool */
    { 4449, 0x9bd85f5, 61 }, /* vkResetDescriptorPool */
    { 4471, 0x6d373ba8, 30 }, /* vkResetEvent */
    { 4484, 0x684781dc, 21 }, /* vkResetFences */
    { 4498, 0xe6701e5f, 34 }, /* vkResetQueryPoolEXT */
    { 4518, 0x592ae5f5, 29 }, /* vkSetEvent */
    { 4529, 0xfef2fb38, 130 }, /* vkTrimCommandPool */
    { 4547, 0x51177c8d, 131 }, /* vkTrimCommandPoolKHR */
    { 4568, 0x1a1a0e2f, 9 }, /* vkUnmapMemory */
    { 4582, 0x5349c9d, 159 }, /* vkUpdateDescriptorSetWithTemplate */
    { 4616, 0x214ad230, 160 }, /* vkUpdateDescriptorSetWithTemplateKHR */
    { 4653, 0xbfd090ae, 64 }, /* vkUpdateDescriptorSets */
    { 4676, 0x19d64c81, 23 }, /* vkWaitForFences */
};

/* Hash table stats:
 * size 201 entries
 * collisions entries:
 *     0      122
 *     1      40
 *     2      9
 *     3      6
 *     4      4
 *     5      6
 *     6      3
 *     7      2
 *     8      0
 *     9+     9
 */

#define none 0xffff
static const uint16_t device_string_map[256] = {
    0x005d,
    none,
    none,
    none,
    0x00a2,
    0x0024,
    0x0031,
    0x003e,
    0x001a,
    0x00a5,
    0x0069,
    none,
    0x0043,
    none,
    0x0097,
    none,
    0x008e,
    0x00b2,
    none,
    none,
    none,
    none,
    none,
    0x00b8,
    0x000b,
    0x009c,
    0x0003,
    0x006b,
    0x00b4,
    0x0017,
    0x006d,
    0x0087,
    0x0022,
    0x0099,
    0x002d,
    0x00b3,
    0x0047,
    0x0081,
    0x0062,
    0x0006,
    0x003a,
    0x00ac,
    0x0088,
    0x0049,
    0x000f,
    none,
    0x0002,
    0x00c4,
    0x006a,
    0x0056,
    none,
    0x0053,
    0x0048,
    0x002f,
    0x0027,
    0x0078,
    0x0089,
    none,
    0x0004,
    0x0060,
    0x0070,
    none,
    0x005e,
    0x0028,
    0x0032,
    0x00b9,
    none,
    0x00c6,
    0x00bb,
    0x0067,
    none,
    none,
    0x0095,
    0x00a4,
    0x000a,
    0x0096,
    0x004c,
    none,
    0x00ae,
    0x0008,
    none,
    0x005f,
    0x0061,
    0x0073,
    0x00bd,
    none,
    0x0041,
    none,
    0x0026,
    none,
    0x00ab,
    0x0064,
    0x00ba,
    0x007f,
    0x004a,
    0x00c0,
    none,
    none,
    0x0063,
    0x003f,
    0x006f,
    0x0044,
    0x0079,
    0x0021,
    0x0037,
    0x004e,
    none,
    0x0071,
    none,
    0x0059,
    0x007d,
    0x00bc,
    0x0014,
    0x0086,
    0x0001,
    0x0075,
    0x0052,
    0x0042,
    0x004f,
    0x0080,
    0x0030,
    0x0076,
    0x00c1,
    0x0057,
    none,
    0x002c,
    0x001d,
    0x0045,
    none,
    0x0039,
    0x003b,
    0x008d,
    0x0015,
    none,
    0x008c,
    0x0040,
    0x0068,
    0x0018,
    0x002e,
    0x009d,
    0x009b,
    0x008a,
    none,
    none,
    0x00af,
    0x0083,
    0x006c,
    none,
    0x0012,
    0x00a0,
    0x0072,
    0x009e,
    none,
    0x0093,
    0x007e,
    0x008f,
    0x002a,
    0x0054,
    0x00a3,
    0x000d,
    0x00c3,
    0x0025,
    0x0065,
    0x007b,
    0x000e,
    0x0077,
    none,
    0x003c,
    0x0066,
    0x001e,
    0x00aa,
    none,
    0x001f,
    0x000c,
    0x00c7,
    0x0074,
    0x0082,
    0x0084,
    0x005c,
    0x004b,
    0x0038,
    0x0092,
    0x00ad,
    0x0051,
    0x0019,
    0x004d,
    0x001b,
    0x00be,
    0x0035,
    0x00c2,
    none,
    0x0029,
    0x0094,
    none,
    0x005a,
    0x0085,
    0x0058,
    0x007a,
    0x00a8,
    none,
    none,
    none,
    0x00b1,
    0x0007,
    none,
    0x0016,
    0x0009,
    none,
    none,
    none,
    0x009f,
    0x0005,
    0x0020,
    0x007c,
    0x0091,
    0x00a6,
    0x00a1,
    none,
    0x0013,
    none,
    0x00bf,
    0x0000,
    0x006e,
    none,
    0x0033,
    0x00b5,
    0x0046,
    0x0010,
    none,
    0x0023,
    0x0011,
    0x0090,
    0x0036,
    0x00b0,
    0x00a7,
    0x0050,
    0x00b7,
    0x001c,
    none,
    0x00b6,
    none,
    0x0098,
    none,
    0x0055,
    0x00c8,
    0x002b,
    none,
    0x009a,
    none,
    0x0034,
    none,
    0x003d,
    0x00c5,
    0x00a9,
    0x008b,
    0x005b,
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
        i = device_string_map[h & 255];
        if (i == none)
           return -1;
        e = &device_string_map_entries[i];
        if (e->hash == hash && strcmp(str, device_strings + e->name) == 0)
            return e->num;
        h += prime_step;
    }

    return -1;
}


/* Weak aliases for all potential implementations. These will resolve to
 * NULL if they're not defined, which lets the resolve_entrypoint() function
 * either pick the correct entry point.
 */

  VkResult anv_CreateInstance(const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance) __attribute__ ((weak));
  void anv_DestroyInstance(VkInstance instance, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
  VkResult anv_EnumeratePhysicalDevices(VkInstance instance, uint32_t* pPhysicalDeviceCount, VkPhysicalDevice* pPhysicalDevices) __attribute__ ((weak));
  PFN_vkVoidFunction anv_GetInstanceProcAddr(VkInstance instance, const char* pName) __attribute__ ((weak));
  void anv_GetPhysicalDeviceProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties* pProperties) __attribute__ ((weak));
  void anv_GetPhysicalDeviceQueueFamilyProperties(VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties* pQueueFamilyProperties) __attribute__ ((weak));
  void anv_GetPhysicalDeviceMemoryProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties* pMemoryProperties) __attribute__ ((weak));
  void anv_GetPhysicalDeviceFeatures(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures* pFeatures) __attribute__ ((weak));
  void anv_GetPhysicalDeviceFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties* pFormatProperties) __attribute__ ((weak));
  VkResult anv_GetPhysicalDeviceImageFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkImageFormatProperties* pImageFormatProperties) __attribute__ ((weak));
  VkResult anv_CreateDevice(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDevice* pDevice) __attribute__ ((weak));
  VkResult anv_EnumerateInstanceVersion(uint32_t* pApiVersion) __attribute__ ((weak));
  VkResult anv_EnumerateInstanceLayerProperties(uint32_t* pPropertyCount, VkLayerProperties* pProperties) __attribute__ ((weak));
  VkResult anv_EnumerateInstanceExtensionProperties(const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties) __attribute__ ((weak));
  VkResult anv_EnumerateDeviceLayerProperties(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkLayerProperties* pProperties) __attribute__ ((weak));
  VkResult anv_EnumerateDeviceExtensionProperties(VkPhysicalDevice physicalDevice, const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties) __attribute__ ((weak));
  void anv_GetPhysicalDeviceSparseImageFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkSampleCountFlagBits samples, VkImageUsageFlags usage, VkImageTiling tiling, uint32_t* pPropertyCount, VkSparseImageFormatProperties* pProperties) __attribute__ ((weak));
  VkResult anv_GetPhysicalDeviceDisplayPropertiesKHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPropertiesKHR* pProperties) __attribute__ ((weak));
  VkResult anv_GetPhysicalDeviceDisplayPlanePropertiesKHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPlanePropertiesKHR* pProperties) __attribute__ ((weak));
  VkResult anv_GetDisplayPlaneSupportedDisplaysKHR(VkPhysicalDevice physicalDevice, uint32_t planeIndex, uint32_t* pDisplayCount, VkDisplayKHR* pDisplays) __attribute__ ((weak));
  VkResult anv_GetDisplayModePropertiesKHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModePropertiesKHR* pProperties) __attribute__ ((weak));
  VkResult anv_CreateDisplayModeKHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, const VkDisplayModeCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDisplayModeKHR* pMode) __attribute__ ((weak));
  VkResult anv_GetDisplayPlaneCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkDisplayModeKHR mode, uint32_t planeIndex, VkDisplayPlaneCapabilitiesKHR* pCapabilities) __attribute__ ((weak));
  VkResult anv_CreateDisplayPlaneSurfaceKHR(VkInstance instance, const VkDisplaySurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) __attribute__ ((weak));
  void anv_DestroySurfaceKHR(VkInstance instance, VkSurfaceKHR surface, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
  VkResult anv_GetPhysicalDeviceSurfaceSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported) __attribute__ ((weak));
  VkResult anv_GetPhysicalDeviceSurfaceCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR* pSurfaceCapabilities) __attribute__ ((weak));
  VkResult anv_GetPhysicalDeviceSurfaceFormatsKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pSurfaceFormatCount, VkSurfaceFormatKHR* pSurfaceFormats) __attribute__ ((weak));
  VkResult anv_GetPhysicalDeviceSurfacePresentModesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes) __attribute__ ((weak));
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
  VkResult anv_CreateWaylandSurfaceKHR(VkInstance instance, const VkWaylandSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_WAYLAND_KHR
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
  VkBool32 anv_GetPhysicalDeviceWaylandPresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, struct wl_display* display) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_WAYLAND_KHR
#ifdef VK_USE_PLATFORM_XLIB_KHR
  VkResult anv_CreateXlibSurfaceKHR(VkInstance instance, const VkXlibSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_XLIB_KHR
#ifdef VK_USE_PLATFORM_XLIB_KHR
  VkBool32 anv_GetPhysicalDeviceXlibPresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, Display* dpy, VisualID visualID) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_XLIB_KHR
#ifdef VK_USE_PLATFORM_XCB_KHR
  VkResult anv_CreateXcbSurfaceKHR(VkInstance instance, const VkXcbSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_XCB_KHR
#ifdef VK_USE_PLATFORM_XCB_KHR
  VkBool32 anv_GetPhysicalDeviceXcbPresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, xcb_connection_t* connection, xcb_visualid_t visual_id) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_XCB_KHR
  VkResult anv_CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback) __attribute__ ((weak));
  void anv_DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
  void anv_DebugReportMessageEXT(VkInstance instance, VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage) __attribute__ ((weak));
  void anv_GetPhysicalDeviceFeatures2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures) __attribute__ ((weak));
      void anv_GetPhysicalDeviceProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties) __attribute__ ((weak));
      void anv_GetPhysicalDeviceFormatProperties2(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2* pFormatProperties) __attribute__ ((weak));
      VkResult anv_GetPhysicalDeviceImageFormatProperties2(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties) __attribute__ ((weak));
      void anv_GetPhysicalDeviceQueueFamilyProperties2(VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties2* pQueueFamilyProperties) __attribute__ ((weak));
      void anv_GetPhysicalDeviceMemoryProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties) __attribute__ ((weak));
      void anv_GetPhysicalDeviceSparseImageFormatProperties2(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo, uint32_t* pPropertyCount, VkSparseImageFormatProperties2* pProperties) __attribute__ ((weak));
      void anv_GetPhysicalDeviceExternalBufferProperties(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties) __attribute__ ((weak));
      void anv_GetPhysicalDeviceExternalSemaphoreProperties(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties) __attribute__ ((weak));
      void anv_GetPhysicalDeviceExternalFenceProperties(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties) __attribute__ ((weak));
      VkResult anv_ReleaseDisplayEXT(VkPhysicalDevice physicalDevice, VkDisplayKHR display) __attribute__ ((weak));
#ifdef VK_USE_PLATFORM_XLIB_XRANDR_EXT
  VkResult anv_AcquireXlibDisplayEXT(VkPhysicalDevice physicalDevice, Display* dpy, VkDisplayKHR display) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_XLIB_XRANDR_EXT
#ifdef VK_USE_PLATFORM_XLIB_XRANDR_EXT
  VkResult anv_GetRandROutputDisplayEXT(VkPhysicalDevice physicalDevice, Display* dpy, RROutput rrOutput, VkDisplayKHR* pDisplay) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_XLIB_XRANDR_EXT
  VkResult anv_GetPhysicalDeviceSurfaceCapabilities2EXT(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilities2EXT* pSurfaceCapabilities) __attribute__ ((weak));
  VkResult anv_EnumeratePhysicalDeviceGroups(VkInstance instance, uint32_t* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties) __attribute__ ((weak));
      VkResult anv_GetPhysicalDevicePresentRectanglesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pRectCount, VkRect2D* pRects) __attribute__ ((weak));
  VkResult anv_GetPhysicalDeviceSurfaceCapabilities2KHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkSurfaceCapabilities2KHR* pSurfaceCapabilities) __attribute__ ((weak));
  VkResult anv_GetPhysicalDeviceSurfaceFormats2KHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pSurfaceFormatCount, VkSurfaceFormat2KHR* pSurfaceFormats) __attribute__ ((weak));
  VkResult anv_GetPhysicalDeviceDisplayProperties2KHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayProperties2KHR* pProperties) __attribute__ ((weak));
  VkResult anv_GetPhysicalDeviceDisplayPlaneProperties2KHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPlaneProperties2KHR* pProperties) __attribute__ ((weak));
  VkResult anv_GetDisplayModeProperties2KHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModeProperties2KHR* pProperties) __attribute__ ((weak));
  VkResult anv_GetDisplayPlaneCapabilities2KHR(VkPhysicalDevice physicalDevice, const VkDisplayPlaneInfo2KHR* pDisplayPlaneInfo, VkDisplayPlaneCapabilities2KHR* pCapabilities) __attribute__ ((weak));
  VkResult anv_GetPhysicalDeviceCalibrateableTimeDomainsEXT(VkPhysicalDevice physicalDevice, uint32_t* pTimeDomainCount, VkTimeDomainEXT* pTimeDomains) __attribute__ ((weak));

const struct anv_instance_dispatch_table anv_instance_dispatch_table = {
  .vkCreateInstance = anv_CreateInstance,
  .vkDestroyInstance = anv_DestroyInstance,
  .vkEnumeratePhysicalDevices = anv_EnumeratePhysicalDevices,
  .vkGetInstanceProcAddr = anv_GetInstanceProcAddr,
  .vkGetPhysicalDeviceProperties = anv_GetPhysicalDeviceProperties,
  .vkGetPhysicalDeviceQueueFamilyProperties = anv_GetPhysicalDeviceQueueFamilyProperties,
  .vkGetPhysicalDeviceMemoryProperties = anv_GetPhysicalDeviceMemoryProperties,
  .vkGetPhysicalDeviceFeatures = anv_GetPhysicalDeviceFeatures,
  .vkGetPhysicalDeviceFormatProperties = anv_GetPhysicalDeviceFormatProperties,
  .vkGetPhysicalDeviceImageFormatProperties = anv_GetPhysicalDeviceImageFormatProperties,
  .vkCreateDevice = anv_CreateDevice,
  .vkEnumerateInstanceVersion = anv_EnumerateInstanceVersion,
  .vkEnumerateInstanceLayerProperties = anv_EnumerateInstanceLayerProperties,
  .vkEnumerateInstanceExtensionProperties = anv_EnumerateInstanceExtensionProperties,
  .vkEnumerateDeviceLayerProperties = anv_EnumerateDeviceLayerProperties,
  .vkEnumerateDeviceExtensionProperties = anv_EnumerateDeviceExtensionProperties,
  .vkGetPhysicalDeviceSparseImageFormatProperties = anv_GetPhysicalDeviceSparseImageFormatProperties,
  .vkGetPhysicalDeviceDisplayPropertiesKHR = anv_GetPhysicalDeviceDisplayPropertiesKHR,
  .vkGetPhysicalDeviceDisplayPlanePropertiesKHR = anv_GetPhysicalDeviceDisplayPlanePropertiesKHR,
  .vkGetDisplayPlaneSupportedDisplaysKHR = anv_GetDisplayPlaneSupportedDisplaysKHR,
  .vkGetDisplayModePropertiesKHR = anv_GetDisplayModePropertiesKHR,
  .vkCreateDisplayModeKHR = anv_CreateDisplayModeKHR,
  .vkGetDisplayPlaneCapabilitiesKHR = anv_GetDisplayPlaneCapabilitiesKHR,
  .vkCreateDisplayPlaneSurfaceKHR = anv_CreateDisplayPlaneSurfaceKHR,
  .vkDestroySurfaceKHR = anv_DestroySurfaceKHR,
  .vkGetPhysicalDeviceSurfaceSupportKHR = anv_GetPhysicalDeviceSurfaceSupportKHR,
  .vkGetPhysicalDeviceSurfaceCapabilitiesKHR = anv_GetPhysicalDeviceSurfaceCapabilitiesKHR,
  .vkGetPhysicalDeviceSurfaceFormatsKHR = anv_GetPhysicalDeviceSurfaceFormatsKHR,
  .vkGetPhysicalDeviceSurfacePresentModesKHR = anv_GetPhysicalDeviceSurfacePresentModesKHR,
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
  .vkCreateWaylandSurfaceKHR = anv_CreateWaylandSurfaceKHR,
#endif // VK_USE_PLATFORM_WAYLAND_KHR
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
  .vkGetPhysicalDeviceWaylandPresentationSupportKHR = anv_GetPhysicalDeviceWaylandPresentationSupportKHR,
#endif // VK_USE_PLATFORM_WAYLAND_KHR
#ifdef VK_USE_PLATFORM_XLIB_KHR
  .vkCreateXlibSurfaceKHR = anv_CreateXlibSurfaceKHR,
#endif // VK_USE_PLATFORM_XLIB_KHR
#ifdef VK_USE_PLATFORM_XLIB_KHR
  .vkGetPhysicalDeviceXlibPresentationSupportKHR = anv_GetPhysicalDeviceXlibPresentationSupportKHR,
#endif // VK_USE_PLATFORM_XLIB_KHR
#ifdef VK_USE_PLATFORM_XCB_KHR
  .vkCreateXcbSurfaceKHR = anv_CreateXcbSurfaceKHR,
#endif // VK_USE_PLATFORM_XCB_KHR
#ifdef VK_USE_PLATFORM_XCB_KHR
  .vkGetPhysicalDeviceXcbPresentationSupportKHR = anv_GetPhysicalDeviceXcbPresentationSupportKHR,
#endif // VK_USE_PLATFORM_XCB_KHR
  .vkCreateDebugReportCallbackEXT = anv_CreateDebugReportCallbackEXT,
  .vkDestroyDebugReportCallbackEXT = anv_DestroyDebugReportCallbackEXT,
  .vkDebugReportMessageEXT = anv_DebugReportMessageEXT,
  .vkGetPhysicalDeviceFeatures2 = anv_GetPhysicalDeviceFeatures2,
  .vkGetPhysicalDeviceFeatures2KHR = anv_GetPhysicalDeviceFeatures2,
  .vkGetPhysicalDeviceProperties2 = anv_GetPhysicalDeviceProperties2,
  .vkGetPhysicalDeviceProperties2KHR = anv_GetPhysicalDeviceProperties2,
  .vkGetPhysicalDeviceFormatProperties2 = anv_GetPhysicalDeviceFormatProperties2,
  .vkGetPhysicalDeviceFormatProperties2KHR = anv_GetPhysicalDeviceFormatProperties2,
  .vkGetPhysicalDeviceImageFormatProperties2 = anv_GetPhysicalDeviceImageFormatProperties2,
  .vkGetPhysicalDeviceImageFormatProperties2KHR = anv_GetPhysicalDeviceImageFormatProperties2,
  .vkGetPhysicalDeviceQueueFamilyProperties2 = anv_GetPhysicalDeviceQueueFamilyProperties2,
  .vkGetPhysicalDeviceQueueFamilyProperties2KHR = anv_GetPhysicalDeviceQueueFamilyProperties2,
  .vkGetPhysicalDeviceMemoryProperties2 = anv_GetPhysicalDeviceMemoryProperties2,
  .vkGetPhysicalDeviceMemoryProperties2KHR = anv_GetPhysicalDeviceMemoryProperties2,
  .vkGetPhysicalDeviceSparseImageFormatProperties2 = anv_GetPhysicalDeviceSparseImageFormatProperties2,
  .vkGetPhysicalDeviceSparseImageFormatProperties2KHR = anv_GetPhysicalDeviceSparseImageFormatProperties2,
  .vkGetPhysicalDeviceExternalBufferProperties = anv_GetPhysicalDeviceExternalBufferProperties,
  .vkGetPhysicalDeviceExternalBufferPropertiesKHR = anv_GetPhysicalDeviceExternalBufferProperties,
  .vkGetPhysicalDeviceExternalSemaphoreProperties = anv_GetPhysicalDeviceExternalSemaphoreProperties,
  .vkGetPhysicalDeviceExternalSemaphorePropertiesKHR = anv_GetPhysicalDeviceExternalSemaphoreProperties,
  .vkGetPhysicalDeviceExternalFenceProperties = anv_GetPhysicalDeviceExternalFenceProperties,
  .vkGetPhysicalDeviceExternalFencePropertiesKHR = anv_GetPhysicalDeviceExternalFenceProperties,
  .vkReleaseDisplayEXT = anv_ReleaseDisplayEXT,
#ifdef VK_USE_PLATFORM_XLIB_XRANDR_EXT
  .vkAcquireXlibDisplayEXT = anv_AcquireXlibDisplayEXT,
#endif // VK_USE_PLATFORM_XLIB_XRANDR_EXT
#ifdef VK_USE_PLATFORM_XLIB_XRANDR_EXT
  .vkGetRandROutputDisplayEXT = anv_GetRandROutputDisplayEXT,
#endif // VK_USE_PLATFORM_XLIB_XRANDR_EXT
  .vkGetPhysicalDeviceSurfaceCapabilities2EXT = anv_GetPhysicalDeviceSurfaceCapabilities2EXT,
  .vkEnumeratePhysicalDeviceGroups = anv_EnumeratePhysicalDeviceGroups,
  .vkEnumeratePhysicalDeviceGroupsKHR = anv_EnumeratePhysicalDeviceGroups,
  .vkGetPhysicalDevicePresentRectanglesKHR = anv_GetPhysicalDevicePresentRectanglesKHR,
  .vkGetPhysicalDeviceSurfaceCapabilities2KHR = anv_GetPhysicalDeviceSurfaceCapabilities2KHR,
  .vkGetPhysicalDeviceSurfaceFormats2KHR = anv_GetPhysicalDeviceSurfaceFormats2KHR,
  .vkGetPhysicalDeviceDisplayProperties2KHR = anv_GetPhysicalDeviceDisplayProperties2KHR,
  .vkGetPhysicalDeviceDisplayPlaneProperties2KHR = anv_GetPhysicalDeviceDisplayPlaneProperties2KHR,
  .vkGetDisplayModeProperties2KHR = anv_GetDisplayModeProperties2KHR,
  .vkGetDisplayPlaneCapabilities2KHR = anv_GetDisplayPlaneCapabilities2KHR,
  .vkGetPhysicalDeviceCalibrateableTimeDomainsEXT = anv_GetPhysicalDeviceCalibrateableTimeDomainsEXT,
};

      PFN_vkVoidFunction __attribute__ ((weak))
      anv_GetDeviceProcAddr(VkDevice device, const char* pName)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkGetDeviceProcAddr(device, pName);
      }
      void __attribute__ ((weak))
      anv_DestroyDevice(VkDevice device, const VkAllocationCallbacks* pAllocator)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkDestroyDevice(device, pAllocator);
      }
      void __attribute__ ((weak))
      anv_GetDeviceQueue(VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkGetDeviceQueue(device, queueFamilyIndex, queueIndex, pQueue);
      }
      VkResult __attribute__ ((weak))
      anv_QueueSubmit(VkQueue queue, uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence)
      {
          ANV_FROM_HANDLE(anv_queue, anv_queue, queue);
          return anv_queue->device->dispatch.vkQueueSubmit(queue, submitCount, pSubmits, fence);
      }
      VkResult __attribute__ ((weak))
      anv_QueueWaitIdle(VkQueue queue)
      {
          ANV_FROM_HANDLE(anv_queue, anv_queue, queue);
          return anv_queue->device->dispatch.vkQueueWaitIdle(queue);
      }
      VkResult __attribute__ ((weak))
      anv_DeviceWaitIdle(VkDevice device)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkDeviceWaitIdle(device);
      }
      VkResult __attribute__ ((weak))
      anv_AllocateMemory(VkDevice device, const VkMemoryAllocateInfo* pAllocateInfo, const VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkAllocateMemory(device, pAllocateInfo, pAllocator, pMemory);
      }
      void __attribute__ ((weak))
      anv_FreeMemory(VkDevice device, VkDeviceMemory memory, const VkAllocationCallbacks* pAllocator)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkFreeMemory(device, memory, pAllocator);
      }
      VkResult __attribute__ ((weak))
      anv_MapMemory(VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkMapMemory(device, memory, offset, size, flags, ppData);
      }
      void __attribute__ ((weak))
      anv_UnmapMemory(VkDevice device, VkDeviceMemory memory)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkUnmapMemory(device, memory);
      }
      VkResult __attribute__ ((weak))
      anv_FlushMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkFlushMappedMemoryRanges(device, memoryRangeCount, pMemoryRanges);
      }
      VkResult __attribute__ ((weak))
      anv_InvalidateMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkInvalidateMappedMemoryRanges(device, memoryRangeCount, pMemoryRanges);
      }
      void __attribute__ ((weak))
      anv_GetDeviceMemoryCommitment(VkDevice device, VkDeviceMemory memory, VkDeviceSize* pCommittedMemoryInBytes)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkGetDeviceMemoryCommitment(device, memory, pCommittedMemoryInBytes);
      }
      void __attribute__ ((weak))
      anv_GetBufferMemoryRequirements(VkDevice device, VkBuffer buffer, VkMemoryRequirements* pMemoryRequirements)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkGetBufferMemoryRequirements(device, buffer, pMemoryRequirements);
      }
      VkResult __attribute__ ((weak))
      anv_BindBufferMemory(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkBindBufferMemory(device, buffer, memory, memoryOffset);
      }
      void __attribute__ ((weak))
      anv_GetImageMemoryRequirements(VkDevice device, VkImage image, VkMemoryRequirements* pMemoryRequirements)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkGetImageMemoryRequirements(device, image, pMemoryRequirements);
      }
      VkResult __attribute__ ((weak))
      anv_BindImageMemory(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkBindImageMemory(device, image, memory, memoryOffset);
      }
      void __attribute__ ((weak))
      anv_GetImageSparseMemoryRequirements(VkDevice device, VkImage image, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements* pSparseMemoryRequirements)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkGetImageSparseMemoryRequirements(device, image, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
      }
      VkResult __attribute__ ((weak))
      anv_QueueBindSparse(VkQueue queue, uint32_t bindInfoCount, const VkBindSparseInfo* pBindInfo, VkFence fence)
      {
          ANV_FROM_HANDLE(anv_queue, anv_queue, queue);
          return anv_queue->device->dispatch.vkQueueBindSparse(queue, bindInfoCount, pBindInfo, fence);
      }
      VkResult __attribute__ ((weak))
      anv_CreateFence(VkDevice device, const VkFenceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkCreateFence(device, pCreateInfo, pAllocator, pFence);
      }
      void __attribute__ ((weak))
      anv_DestroyFence(VkDevice device, VkFence fence, const VkAllocationCallbacks* pAllocator)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkDestroyFence(device, fence, pAllocator);
      }
      VkResult __attribute__ ((weak))
      anv_ResetFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkResetFences(device, fenceCount, pFences);
      }
      VkResult __attribute__ ((weak))
      anv_GetFenceStatus(VkDevice device, VkFence fence)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkGetFenceStatus(device, fence);
      }
      VkResult __attribute__ ((weak))
      anv_WaitForFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences, VkBool32 waitAll, uint64_t timeout)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkWaitForFences(device, fenceCount, pFences, waitAll, timeout);
      }
      VkResult __attribute__ ((weak))
      anv_CreateSemaphore(VkDevice device, const VkSemaphoreCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkCreateSemaphore(device, pCreateInfo, pAllocator, pSemaphore);
      }
      void __attribute__ ((weak))
      anv_DestroySemaphore(VkDevice device, VkSemaphore semaphore, const VkAllocationCallbacks* pAllocator)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkDestroySemaphore(device, semaphore, pAllocator);
      }
      VkResult __attribute__ ((weak))
      anv_CreateEvent(VkDevice device, const VkEventCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkEvent* pEvent)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkCreateEvent(device, pCreateInfo, pAllocator, pEvent);
      }
      void __attribute__ ((weak))
      anv_DestroyEvent(VkDevice device, VkEvent event, const VkAllocationCallbacks* pAllocator)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkDestroyEvent(device, event, pAllocator);
      }
      VkResult __attribute__ ((weak))
      anv_GetEventStatus(VkDevice device, VkEvent event)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkGetEventStatus(device, event);
      }
      VkResult __attribute__ ((weak))
      anv_SetEvent(VkDevice device, VkEvent event)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkSetEvent(device, event);
      }
      VkResult __attribute__ ((weak))
      anv_ResetEvent(VkDevice device, VkEvent event)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkResetEvent(device, event);
      }
      VkResult __attribute__ ((weak))
      anv_CreateQueryPool(VkDevice device, const VkQueryPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkQueryPool* pQueryPool)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkCreateQueryPool(device, pCreateInfo, pAllocator, pQueryPool);
      }
      void __attribute__ ((weak))
      anv_DestroyQueryPool(VkDevice device, VkQueryPool queryPool, const VkAllocationCallbacks* pAllocator)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkDestroyQueryPool(device, queryPool, pAllocator);
      }
      VkResult __attribute__ ((weak))
      anv_GetQueryPoolResults(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, VkDeviceSize stride, VkQueryResultFlags flags)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkGetQueryPoolResults(device, queryPool, firstQuery, queryCount, dataSize, pData, stride, flags);
      }
      void __attribute__ ((weak))
      anv_ResetQueryPoolEXT(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkResetQueryPoolEXT(device, queryPool, firstQuery, queryCount);
      }
      VkResult __attribute__ ((weak))
      anv_CreateBuffer(VkDevice device, const VkBufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkCreateBuffer(device, pCreateInfo, pAllocator, pBuffer);
      }
      void __attribute__ ((weak))
      anv_DestroyBuffer(VkDevice device, VkBuffer buffer, const VkAllocationCallbacks* pAllocator)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkDestroyBuffer(device, buffer, pAllocator);
      }
      VkResult __attribute__ ((weak))
      anv_CreateBufferView(VkDevice device, const VkBufferViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferView* pView)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkCreateBufferView(device, pCreateInfo, pAllocator, pView);
      }
      void __attribute__ ((weak))
      anv_DestroyBufferView(VkDevice device, VkBufferView bufferView, const VkAllocationCallbacks* pAllocator)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkDestroyBufferView(device, bufferView, pAllocator);
      }
      VkResult __attribute__ ((weak))
      anv_CreateImage(VkDevice device, const VkImageCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImage* pImage)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkCreateImage(device, pCreateInfo, pAllocator, pImage);
      }
      void __attribute__ ((weak))
      anv_DestroyImage(VkDevice device, VkImage image, const VkAllocationCallbacks* pAllocator)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkDestroyImage(device, image, pAllocator);
      }
      void __attribute__ ((weak))
      anv_GetImageSubresourceLayout(VkDevice device, VkImage image, const VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkGetImageSubresourceLayout(device, image, pSubresource, pLayout);
      }
      VkResult __attribute__ ((weak))
      anv_CreateImageView(VkDevice device, const VkImageViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImageView* pView)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkCreateImageView(device, pCreateInfo, pAllocator, pView);
      }
      void __attribute__ ((weak))
      anv_DestroyImageView(VkDevice device, VkImageView imageView, const VkAllocationCallbacks* pAllocator)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkDestroyImageView(device, imageView, pAllocator);
      }
      VkResult __attribute__ ((weak))
      anv_CreateShaderModule(VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkCreateShaderModule(device, pCreateInfo, pAllocator, pShaderModule);
      }
      void __attribute__ ((weak))
      anv_DestroyShaderModule(VkDevice device, VkShaderModule shaderModule, const VkAllocationCallbacks* pAllocator)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkDestroyShaderModule(device, shaderModule, pAllocator);
      }
      VkResult __attribute__ ((weak))
      anv_CreatePipelineCache(VkDevice device, const VkPipelineCacheCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineCache* pPipelineCache)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkCreatePipelineCache(device, pCreateInfo, pAllocator, pPipelineCache);
      }
      void __attribute__ ((weak))
      anv_DestroyPipelineCache(VkDevice device, VkPipelineCache pipelineCache, const VkAllocationCallbacks* pAllocator)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkDestroyPipelineCache(device, pipelineCache, pAllocator);
      }
      VkResult __attribute__ ((weak))
      anv_GetPipelineCacheData(VkDevice device, VkPipelineCache pipelineCache, size_t* pDataSize, void* pData)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkGetPipelineCacheData(device, pipelineCache, pDataSize, pData);
      }
      VkResult __attribute__ ((weak))
      anv_MergePipelineCaches(VkDevice device, VkPipelineCache dstCache, uint32_t srcCacheCount, const VkPipelineCache* pSrcCaches)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkMergePipelineCaches(device, dstCache, srcCacheCount, pSrcCaches);
      }
      VkResult __attribute__ ((weak))
      anv_CreateGraphicsPipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkGraphicsPipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkCreateGraphicsPipelines(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
      }
      VkResult __attribute__ ((weak))
      anv_CreateComputePipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkComputePipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkCreateComputePipelines(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
      }
      void __attribute__ ((weak))
      anv_DestroyPipeline(VkDevice device, VkPipeline pipeline, const VkAllocationCallbacks* pAllocator)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkDestroyPipeline(device, pipeline, pAllocator);
      }
      VkResult __attribute__ ((weak))
      anv_CreatePipelineLayout(VkDevice device, const VkPipelineLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkCreatePipelineLayout(device, pCreateInfo, pAllocator, pPipelineLayout);
      }
      void __attribute__ ((weak))
      anv_DestroyPipelineLayout(VkDevice device, VkPipelineLayout pipelineLayout, const VkAllocationCallbacks* pAllocator)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkDestroyPipelineLayout(device, pipelineLayout, pAllocator);
      }
      VkResult __attribute__ ((weak))
      anv_CreateSampler(VkDevice device, const VkSamplerCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSampler* pSampler)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkCreateSampler(device, pCreateInfo, pAllocator, pSampler);
      }
      void __attribute__ ((weak))
      anv_DestroySampler(VkDevice device, VkSampler sampler, const VkAllocationCallbacks* pAllocator)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkDestroySampler(device, sampler, pAllocator);
      }
      VkResult __attribute__ ((weak))
      anv_CreateDescriptorSetLayout(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorSetLayout* pSetLayout)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkCreateDescriptorSetLayout(device, pCreateInfo, pAllocator, pSetLayout);
      }
      void __attribute__ ((weak))
      anv_DestroyDescriptorSetLayout(VkDevice device, VkDescriptorSetLayout descriptorSetLayout, const VkAllocationCallbacks* pAllocator)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkDestroyDescriptorSetLayout(device, descriptorSetLayout, pAllocator);
      }
      VkResult __attribute__ ((weak))
      anv_CreateDescriptorPool(VkDevice device, const VkDescriptorPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorPool* pDescriptorPool)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkCreateDescriptorPool(device, pCreateInfo, pAllocator, pDescriptorPool);
      }
      void __attribute__ ((weak))
      anv_DestroyDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, const VkAllocationCallbacks* pAllocator)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkDestroyDescriptorPool(device, descriptorPool, pAllocator);
      }
      VkResult __attribute__ ((weak))
      anv_ResetDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags flags)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkResetDescriptorPool(device, descriptorPool, flags);
      }
      VkResult __attribute__ ((weak))
      anv_AllocateDescriptorSets(VkDevice device, const VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkAllocateDescriptorSets(device, pAllocateInfo, pDescriptorSets);
      }
      VkResult __attribute__ ((weak))
      anv_FreeDescriptorSets(VkDevice device, VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkFreeDescriptorSets(device, descriptorPool, descriptorSetCount, pDescriptorSets);
      }
      void __attribute__ ((weak))
      anv_UpdateDescriptorSets(VkDevice device, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, const VkCopyDescriptorSet* pDescriptorCopies)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkUpdateDescriptorSets(device, descriptorWriteCount, pDescriptorWrites, descriptorCopyCount, pDescriptorCopies);
      }
      VkResult __attribute__ ((weak))
      anv_CreateFramebuffer(VkDevice device, const VkFramebufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFramebuffer* pFramebuffer)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkCreateFramebuffer(device, pCreateInfo, pAllocator, pFramebuffer);
      }
      void __attribute__ ((weak))
      anv_DestroyFramebuffer(VkDevice device, VkFramebuffer framebuffer, const VkAllocationCallbacks* pAllocator)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkDestroyFramebuffer(device, framebuffer, pAllocator);
      }
      VkResult __attribute__ ((weak))
      anv_CreateRenderPass(VkDevice device, const VkRenderPassCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkCreateRenderPass(device, pCreateInfo, pAllocator, pRenderPass);
      }
      void __attribute__ ((weak))
      anv_DestroyRenderPass(VkDevice device, VkRenderPass renderPass, const VkAllocationCallbacks* pAllocator)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkDestroyRenderPass(device, renderPass, pAllocator);
      }
      void __attribute__ ((weak))
      anv_GetRenderAreaGranularity(VkDevice device, VkRenderPass renderPass, VkExtent2D* pGranularity)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkGetRenderAreaGranularity(device, renderPass, pGranularity);
      }
      VkResult __attribute__ ((weak))
      anv_CreateCommandPool(VkDevice device, const VkCommandPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkCreateCommandPool(device, pCreateInfo, pAllocator, pCommandPool);
      }
      void __attribute__ ((weak))
      anv_DestroyCommandPool(VkDevice device, VkCommandPool commandPool, const VkAllocationCallbacks* pAllocator)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkDestroyCommandPool(device, commandPool, pAllocator);
      }
      VkResult __attribute__ ((weak))
      anv_ResetCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolResetFlags flags)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkResetCommandPool(device, commandPool, flags);
      }
      VkResult __attribute__ ((weak))
      anv_AllocateCommandBuffers(VkDevice device, const VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkAllocateCommandBuffers(device, pAllocateInfo, pCommandBuffers);
      }
      void __attribute__ ((weak))
      anv_FreeCommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkFreeCommandBuffers(device, commandPool, commandBufferCount, pCommandBuffers);
      }
      VkResult __attribute__ ((weak))
      anv_BeginCommandBuffer(VkCommandBuffer commandBuffer, const VkCommandBufferBeginInfo* pBeginInfo)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkBeginCommandBuffer(commandBuffer, pBeginInfo);
      }
      VkResult __attribute__ ((weak))
      anv_EndCommandBuffer(VkCommandBuffer commandBuffer)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkEndCommandBuffer(commandBuffer);
      }
      VkResult __attribute__ ((weak))
      anv_ResetCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkResetCommandBuffer(commandBuffer, flags);
      }
      void __attribute__ ((weak))
      anv_CmdBindPipeline(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdBindPipeline(commandBuffer, pipelineBindPoint, pipeline);
      }
      void __attribute__ ((weak))
      anv_CmdSetViewport(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pViewports)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdSetViewport(commandBuffer, firstViewport, viewportCount, pViewports);
      }
      void __attribute__ ((weak))
      anv_CmdSetScissor(VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pScissors)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdSetScissor(commandBuffer, firstScissor, scissorCount, pScissors);
      }
      void __attribute__ ((weak))
      anv_CmdSetLineWidth(VkCommandBuffer commandBuffer, float lineWidth)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdSetLineWidth(commandBuffer, lineWidth);
      }
      void __attribute__ ((weak))
      anv_CmdSetDepthBias(VkCommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdSetDepthBias(commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
      }
      void __attribute__ ((weak))
      anv_CmdSetBlendConstants(VkCommandBuffer commandBuffer, const float blendConstants[4])
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdSetBlendConstants(commandBuffer, blendConstants);
      }
      void __attribute__ ((weak))
      anv_CmdSetDepthBounds(VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdSetDepthBounds(commandBuffer, minDepthBounds, maxDepthBounds);
      }
      void __attribute__ ((weak))
      anv_CmdSetStencilCompareMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdSetStencilCompareMask(commandBuffer, faceMask, compareMask);
      }
      void __attribute__ ((weak))
      anv_CmdSetStencilWriteMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdSetStencilWriteMask(commandBuffer, faceMask, writeMask);
      }
      void __attribute__ ((weak))
      anv_CmdSetStencilReference(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdSetStencilReference(commandBuffer, faceMask, reference);
      }
      void __attribute__ ((weak))
      anv_CmdBindDescriptorSets(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdBindDescriptorSets(commandBuffer, pipelineBindPoint, layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);
      }
      void __attribute__ ((weak))
      anv_CmdBindIndexBuffer(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdBindIndexBuffer(commandBuffer, buffer, offset, indexType);
      }
      void __attribute__ ((weak))
      anv_CmdBindVertexBuffers(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdBindVertexBuffers(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets);
      }
      void __attribute__ ((weak))
      anv_CmdDraw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdDraw(commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
      }
      void __attribute__ ((weak))
      anv_CmdDrawIndexed(VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdDrawIndexed(commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
      }
      void __attribute__ ((weak))
      anv_CmdDrawIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdDrawIndirect(commandBuffer, buffer, offset, drawCount, stride);
      }
      void __attribute__ ((weak))
      anv_CmdDrawIndexedIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdDrawIndexedIndirect(commandBuffer, buffer, offset, drawCount, stride);
      }
      void __attribute__ ((weak))
      anv_CmdDispatch(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdDispatch(commandBuffer, groupCountX, groupCountY, groupCountZ);
      }
      void __attribute__ ((weak))
      anv_CmdDispatchIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdDispatchIndirect(commandBuffer, buffer, offset);
      }
      void __attribute__ ((weak))
      anv_CmdCopyBuffer(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, regionCount, pRegions);
      }
      void __attribute__ ((weak))
      anv_CmdCopyImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdCopyImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
      }
      void __attribute__ ((weak))
      anv_CmdBlitImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdBlitImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions, filter);
      }
      void __attribute__ ((weak))
      anv_CmdCopyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdCopyBufferToImage(commandBuffer, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions);
      }
      void __attribute__ ((weak))
      anv_CmdCopyImageToBuffer(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdCopyImageToBuffer(commandBuffer, srcImage, srcImageLayout, dstBuffer, regionCount, pRegions);
      }
      void __attribute__ ((weak))
      anv_CmdUpdateBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdUpdateBuffer(commandBuffer, dstBuffer, dstOffset, dataSize, pData);
      }
      void __attribute__ ((weak))
      anv_CmdFillBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdFillBuffer(commandBuffer, dstBuffer, dstOffset, size, data);
      }
      void __attribute__ ((weak))
      anv_CmdClearColorImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount, const VkImageSubresourceRange* pRanges)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdClearColorImage(commandBuffer, image, imageLayout, pColor, rangeCount, pRanges);
      }
      void __attribute__ ((weak))
      anv_CmdClearDepthStencilImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const VkImageSubresourceRange* pRanges)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdClearDepthStencilImage(commandBuffer, image, imageLayout, pDepthStencil, rangeCount, pRanges);
      }
      void __attribute__ ((weak))
      anv_CmdClearAttachments(VkCommandBuffer commandBuffer, uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdClearAttachments(commandBuffer, attachmentCount, pAttachments, rectCount, pRects);
      }
      void __attribute__ ((weak))
      anv_CmdResolveImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageResolve* pRegions)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdResolveImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
      }
      void __attribute__ ((weak))
      anv_CmdSetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdSetEvent(commandBuffer, event, stageMask);
      }
      void __attribute__ ((weak))
      anv_CmdResetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdResetEvent(commandBuffer, event, stageMask);
      }
      void __attribute__ ((weak))
      anv_CmdWaitEvents(VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdWaitEvents(commandBuffer, eventCount, pEvents, srcStageMask, dstStageMask, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
      }
      void __attribute__ ((weak))
      anv_CmdPipelineBarrier(VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdPipelineBarrier(commandBuffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
      }
      void __attribute__ ((weak))
      anv_CmdBeginQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdBeginQuery(commandBuffer, queryPool, query, flags);
      }
      void __attribute__ ((weak))
      anv_CmdEndQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdEndQuery(commandBuffer, queryPool, query);
      }
      void __attribute__ ((weak))
      anv_CmdBeginConditionalRenderingEXT(VkCommandBuffer commandBuffer, const VkConditionalRenderingBeginInfoEXT* pConditionalRenderingBegin)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdBeginConditionalRenderingEXT(commandBuffer, pConditionalRenderingBegin);
      }
      void __attribute__ ((weak))
      anv_CmdEndConditionalRenderingEXT(VkCommandBuffer commandBuffer)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdEndConditionalRenderingEXT(commandBuffer);
      }
      void __attribute__ ((weak))
      anv_CmdResetQueryPool(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdResetQueryPool(commandBuffer, queryPool, firstQuery, queryCount);
      }
      void __attribute__ ((weak))
      anv_CmdWriteTimestamp(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdWriteTimestamp(commandBuffer, pipelineStage, queryPool, query);
      }
      void __attribute__ ((weak))
      anv_CmdCopyQueryPoolResults(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdCopyQueryPoolResults(commandBuffer, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, flags);
      }
      void __attribute__ ((weak))
      anv_CmdPushConstants(VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdPushConstants(commandBuffer, layout, stageFlags, offset, size, pValues);
      }
      void __attribute__ ((weak))
      anv_CmdBeginRenderPass(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdBeginRenderPass(commandBuffer, pRenderPassBegin, contents);
      }
      void __attribute__ ((weak))
      anv_CmdNextSubpass(VkCommandBuffer commandBuffer, VkSubpassContents contents)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdNextSubpass(commandBuffer, contents);
      }
      void __attribute__ ((weak))
      anv_CmdEndRenderPass(VkCommandBuffer commandBuffer)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdEndRenderPass(commandBuffer);
      }
      void __attribute__ ((weak))
      anv_CmdExecuteCommands(VkCommandBuffer commandBuffer, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdExecuteCommands(commandBuffer, commandBufferCount, pCommandBuffers);
      }
      VkResult __attribute__ ((weak))
      anv_CreateSwapchainKHR(VkDevice device, const VkSwapchainCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkCreateSwapchainKHR(device, pCreateInfo, pAllocator, pSwapchain);
      }
      void __attribute__ ((weak))
      anv_DestroySwapchainKHR(VkDevice device, VkSwapchainKHR swapchain, const VkAllocationCallbacks* pAllocator)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkDestroySwapchainKHR(device, swapchain, pAllocator);
      }
      VkResult __attribute__ ((weak))
      anv_GetSwapchainImagesKHR(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkGetSwapchainImagesKHR(device, swapchain, pSwapchainImageCount, pSwapchainImages);
      }
      VkResult __attribute__ ((weak))
      anv_AcquireNextImageKHR(VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkAcquireNextImageKHR(device, swapchain, timeout, semaphore, fence, pImageIndex);
      }
      VkResult __attribute__ ((weak))
      anv_QueuePresentKHR(VkQueue queue, const VkPresentInfoKHR* pPresentInfo)
      {
          ANV_FROM_HANDLE(anv_queue, anv_queue, queue);
          return anv_queue->device->dispatch.vkQueuePresentKHR(queue, pPresentInfo);
      }
      void __attribute__ ((weak))
      anv_CmdPushDescriptorSetKHR(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdPushDescriptorSetKHR(commandBuffer, pipelineBindPoint, layout, set, descriptorWriteCount, pDescriptorWrites);
      }
      void __attribute__ ((weak))
      anv_TrimCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkTrimCommandPool(device, commandPool, flags);
      }
            VkResult __attribute__ ((weak))
      anv_GetMemoryFdKHR(VkDevice device, const VkMemoryGetFdInfoKHR* pGetFdInfo, int* pFd)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkGetMemoryFdKHR(device, pGetFdInfo, pFd);
      }
      VkResult __attribute__ ((weak))
      anv_GetMemoryFdPropertiesKHR(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, int fd, VkMemoryFdPropertiesKHR* pMemoryFdProperties)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkGetMemoryFdPropertiesKHR(device, handleType, fd, pMemoryFdProperties);
      }
      VkResult __attribute__ ((weak))
      anv_GetSemaphoreFdKHR(VkDevice device, const VkSemaphoreGetFdInfoKHR* pGetFdInfo, int* pFd)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkGetSemaphoreFdKHR(device, pGetFdInfo, pFd);
      }
      VkResult __attribute__ ((weak))
      anv_ImportSemaphoreFdKHR(VkDevice device, const VkImportSemaphoreFdInfoKHR* pImportSemaphoreFdInfo)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkImportSemaphoreFdKHR(device, pImportSemaphoreFdInfo);
      }
      VkResult __attribute__ ((weak))
      anv_GetFenceFdKHR(VkDevice device, const VkFenceGetFdInfoKHR* pGetFdInfo, int* pFd)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkGetFenceFdKHR(device, pGetFdInfo, pFd);
      }
      VkResult __attribute__ ((weak))
      anv_ImportFenceFdKHR(VkDevice device, const VkImportFenceFdInfoKHR* pImportFenceFdInfo)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkImportFenceFdKHR(device, pImportFenceFdInfo);
      }
      VkResult __attribute__ ((weak))
      anv_DisplayPowerControlEXT(VkDevice device, VkDisplayKHR display, const VkDisplayPowerInfoEXT* pDisplayPowerInfo)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkDisplayPowerControlEXT(device, display, pDisplayPowerInfo);
      }
      VkResult __attribute__ ((weak))
      anv_RegisterDeviceEventEXT(VkDevice device, const VkDeviceEventInfoEXT* pDeviceEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkRegisterDeviceEventEXT(device, pDeviceEventInfo, pAllocator, pFence);
      }
      VkResult __attribute__ ((weak))
      anv_RegisterDisplayEventEXT(VkDevice device, VkDisplayKHR display, const VkDisplayEventInfoEXT* pDisplayEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkRegisterDisplayEventEXT(device, display, pDisplayEventInfo, pAllocator, pFence);
      }
      VkResult __attribute__ ((weak))
      anv_GetSwapchainCounterEXT(VkDevice device, VkSwapchainKHR swapchain, VkSurfaceCounterFlagBitsEXT counter, uint64_t* pCounterValue)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkGetSwapchainCounterEXT(device, swapchain, counter, pCounterValue);
      }
      void __attribute__ ((weak))
      anv_GetDeviceGroupPeerMemoryFeatures(VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkGetDeviceGroupPeerMemoryFeatures(device, heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures);
      }
            VkResult __attribute__ ((weak))
      anv_BindBufferMemory2(VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkBindBufferMemory2(device, bindInfoCount, pBindInfos);
      }
            VkResult __attribute__ ((weak))
      anv_BindImageMemory2(VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkBindImageMemory2(device, bindInfoCount, pBindInfos);
      }
            void __attribute__ ((weak))
      anv_CmdSetDeviceMask(VkCommandBuffer commandBuffer, uint32_t deviceMask)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdSetDeviceMask(commandBuffer, deviceMask);
      }
            VkResult __attribute__ ((weak))
      anv_GetDeviceGroupPresentCapabilitiesKHR(VkDevice device, VkDeviceGroupPresentCapabilitiesKHR* pDeviceGroupPresentCapabilities)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkGetDeviceGroupPresentCapabilitiesKHR(device, pDeviceGroupPresentCapabilities);
      }
      VkResult __attribute__ ((weak))
      anv_GetDeviceGroupSurfacePresentModesKHR(VkDevice device, VkSurfaceKHR surface, VkDeviceGroupPresentModeFlagsKHR* pModes)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkGetDeviceGroupSurfacePresentModesKHR(device, surface, pModes);
      }
      VkResult __attribute__ ((weak))
      anv_AcquireNextImage2KHR(VkDevice device, const VkAcquireNextImageInfoKHR* pAcquireInfo, uint32_t* pImageIndex)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkAcquireNextImage2KHR(device, pAcquireInfo, pImageIndex);
      }
      void __attribute__ ((weak))
      anv_CmdDispatchBase(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdDispatchBase(commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
      }
            VkResult __attribute__ ((weak))
      anv_CreateDescriptorUpdateTemplate(VkDevice device, const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkCreateDescriptorUpdateTemplate(device, pCreateInfo, pAllocator, pDescriptorUpdateTemplate);
      }
            void __attribute__ ((weak))
      anv_DestroyDescriptorUpdateTemplate(VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkDestroyDescriptorUpdateTemplate(device, descriptorUpdateTemplate, pAllocator);
      }
            void __attribute__ ((weak))
      anv_UpdateDescriptorSetWithTemplate(VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkUpdateDescriptorSetWithTemplate(device, descriptorSet, descriptorUpdateTemplate, pData);
      }
            void __attribute__ ((weak))
      anv_CmdPushDescriptorSetWithTemplateKHR(VkCommandBuffer commandBuffer, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, const void* pData)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdPushDescriptorSetWithTemplateKHR(commandBuffer, descriptorUpdateTemplate, layout, set, pData);
      }
      void __attribute__ ((weak))
      anv_GetBufferMemoryRequirements2(VkDevice device, const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkGetBufferMemoryRequirements2(device, pInfo, pMemoryRequirements);
      }
            void __attribute__ ((weak))
      anv_GetImageMemoryRequirements2(VkDevice device, const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkGetImageMemoryRequirements2(device, pInfo, pMemoryRequirements);
      }
            void __attribute__ ((weak))
      anv_GetImageSparseMemoryRequirements2(VkDevice device, const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkGetImageSparseMemoryRequirements2(device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
      }
            VkResult __attribute__ ((weak))
      anv_CreateSamplerYcbcrConversion(VkDevice device, const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkCreateSamplerYcbcrConversion(device, pCreateInfo, pAllocator, pYcbcrConversion);
      }
            void __attribute__ ((weak))
      anv_DestroySamplerYcbcrConversion(VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkDestroySamplerYcbcrConversion(device, ycbcrConversion, pAllocator);
      }
            void __attribute__ ((weak))
      anv_GetDeviceQueue2(VkDevice device, const VkDeviceQueueInfo2* pQueueInfo, VkQueue* pQueue)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkGetDeviceQueue2(device, pQueueInfo, pQueue);
      }
      void __attribute__ ((weak))
      anv_GetDescriptorSetLayoutSupport(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkGetDescriptorSetLayoutSupport(device, pCreateInfo, pSupport);
      }
      #ifdef VK_USE_PLATFORM_ANDROID_KHR
      VkResult __attribute__ ((weak))
      anv_GetSwapchainGrallocUsageANDROID(VkDevice device, VkFormat format, VkImageUsageFlags imageUsage, int* grallocUsage)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkGetSwapchainGrallocUsageANDROID(device, format, imageUsage, grallocUsage);
      }
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
      VkResult __attribute__ ((weak))
      anv_GetSwapchainGrallocUsage2ANDROID(VkDevice device, VkFormat format, VkImageUsageFlags imageUsage, VkSwapchainImageUsageFlagsANDROID swapchainImageUsage, uint64_t* grallocConsumerUsage, uint64_t* grallocProducerUsage)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkGetSwapchainGrallocUsage2ANDROID(device, format, imageUsage, swapchainImageUsage, grallocConsumerUsage, grallocProducerUsage);
      }
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
      VkResult __attribute__ ((weak))
      anv_AcquireImageANDROID(VkDevice device, VkImage image, int nativeFenceFd, VkSemaphore semaphore, VkFence fence)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkAcquireImageANDROID(device, image, nativeFenceFd, semaphore, fence);
      }
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
      VkResult __attribute__ ((weak))
      anv_QueueSignalReleaseImageANDROID(VkQueue queue, uint32_t waitSemaphoreCount, const VkSemaphore* pWaitSemaphores, VkImage image, int* pNativeFenceFd)
      {
          ANV_FROM_HANDLE(anv_queue, anv_queue, queue);
          return anv_queue->device->dispatch.vkQueueSignalReleaseImageANDROID(queue, waitSemaphoreCount, pWaitSemaphores, image, pNativeFenceFd);
      }
#endif // VK_USE_PLATFORM_ANDROID_KHR
      VkResult __attribute__ ((weak))
      anv_GetCalibratedTimestampsEXT(VkDevice device, uint32_t timestampCount, const VkCalibratedTimestampInfoEXT* pTimestampInfos, uint64_t* pTimestamps, uint64_t* pMaxDeviation)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkGetCalibratedTimestampsEXT(device, timestampCount, pTimestampInfos, pTimestamps, pMaxDeviation);
      }
      VkResult __attribute__ ((weak))
      anv_GetMemoryHostPointerPropertiesEXT(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, const void* pHostPointer, VkMemoryHostPointerPropertiesEXT* pMemoryHostPointerProperties)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkGetMemoryHostPointerPropertiesEXT(device, handleType, pHostPointer, pMemoryHostPointerProperties);
      }
      VkResult __attribute__ ((weak))
      anv_CreateRenderPass2KHR(VkDevice device, const VkRenderPassCreateInfo2KHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkCreateRenderPass2KHR(device, pCreateInfo, pAllocator, pRenderPass);
      }
      void __attribute__ ((weak))
      anv_CmdBeginRenderPass2KHR(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo*      pRenderPassBegin, const VkSubpassBeginInfoKHR*      pSubpassBeginInfo)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdBeginRenderPass2KHR(commandBuffer, pRenderPassBegin, pSubpassBeginInfo);
      }
      void __attribute__ ((weak))
      anv_CmdNextSubpass2KHR(VkCommandBuffer commandBuffer, const VkSubpassBeginInfoKHR*      pSubpassBeginInfo, const VkSubpassEndInfoKHR*        pSubpassEndInfo)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdNextSubpass2KHR(commandBuffer, pSubpassBeginInfo, pSubpassEndInfo);
      }
      void __attribute__ ((weak))
      anv_CmdEndRenderPass2KHR(VkCommandBuffer commandBuffer, const VkSubpassEndInfoKHR*        pSubpassEndInfo)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdEndRenderPass2KHR(commandBuffer, pSubpassEndInfo);
      }
#ifdef VK_USE_PLATFORM_ANDROID_KHR
      VkResult __attribute__ ((weak))
      anv_GetAndroidHardwareBufferPropertiesANDROID(VkDevice device, const struct AHardwareBuffer* buffer, VkAndroidHardwareBufferPropertiesANDROID* pProperties)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkGetAndroidHardwareBufferPropertiesANDROID(device, buffer, pProperties);
      }
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
      VkResult __attribute__ ((weak))
      anv_GetMemoryAndroidHardwareBufferANDROID(VkDevice device, const VkMemoryGetAndroidHardwareBufferInfoANDROID* pInfo, struct AHardwareBuffer** pBuffer)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkGetMemoryAndroidHardwareBufferANDROID(device, pInfo, pBuffer);
      }
#endif // VK_USE_PLATFORM_ANDROID_KHR
      void __attribute__ ((weak))
      anv_CmdDrawIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdDrawIndirectCountKHR(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
      }
      void __attribute__ ((weak))
      anv_CmdDrawIndexedIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdDrawIndexedIndirectCountKHR(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
      }
      void __attribute__ ((weak))
      anv_CmdBindTransformFeedbackBuffersEXT(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdBindTransformFeedbackBuffersEXT(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes);
      }
      void __attribute__ ((weak))
      anv_CmdBeginTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdBeginTransformFeedbackEXT(commandBuffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
      }
      void __attribute__ ((weak))
      anv_CmdEndTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdEndTransformFeedbackEXT(commandBuffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
      }
      void __attribute__ ((weak))
      anv_CmdBeginQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags, uint32_t index)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdBeginQueryIndexedEXT(commandBuffer, queryPool, query, flags, index);
      }
      void __attribute__ ((weak))
      anv_CmdEndQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, uint32_t index)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdEndQueryIndexedEXT(commandBuffer, queryPool, query, index);
      }
      void __attribute__ ((weak))
      anv_CmdDrawIndirectByteCountEXT(VkCommandBuffer commandBuffer, uint32_t instanceCount, uint32_t firstInstance, VkBuffer counterBuffer, VkDeviceSize counterBufferOffset, uint32_t counterOffset, uint32_t vertexStride)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdDrawIndirectByteCountEXT(commandBuffer, instanceCount, firstInstance, counterBuffer, counterBufferOffset, counterOffset, vertexStride);
      }
      VkDeviceAddress __attribute__ ((weak))
      anv_GetBufferDeviceAddressEXT(VkDevice device, const VkBufferDeviceAddressInfoEXT* pInfo)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkGetBufferDeviceAddressEXT(device, pInfo);
      }
      VkResult __attribute__ ((weak))
      anv_GetPipelineExecutablePropertiesKHR(VkDevice                        device, const VkPipelineInfoKHR*        pPipelineInfo, uint32_t* pExecutableCount, VkPipelineExecutablePropertiesKHR* pProperties)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkGetPipelineExecutablePropertiesKHR(device, pPipelineInfo, pExecutableCount, pProperties);
      }
      VkResult __attribute__ ((weak))
      anv_GetPipelineExecutableStatisticsKHR(VkDevice                        device, const VkPipelineExecutableInfoKHR*  pExecutableInfo, uint32_t* pStatisticCount, VkPipelineExecutableStatisticKHR* pStatistics)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkGetPipelineExecutableStatisticsKHR(device, pExecutableInfo, pStatisticCount, pStatistics);
      }
      VkResult __attribute__ ((weak))
      anv_GetPipelineExecutableInternalRepresentationsKHR(VkDevice                        device, const VkPipelineExecutableInfoKHR*  pExecutableInfo, uint32_t* pInternalRepresentationCount, VkPipelineExecutableInternalRepresentationKHR* pInternalRepresentations)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkGetPipelineExecutableInternalRepresentationsKHR(device, pExecutableInfo, pInternalRepresentationCount, pInternalRepresentations);
      }
      void __attribute__ ((weak))
      anv_CmdSetLineStippleEXT(VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern)
      {
          ANV_FROM_HANDLE(anv_cmd_buffer, anv_cmd_buffer, commandBuffer);
          return anv_cmd_buffer->device->dispatch.vkCmdSetLineStippleEXT(commandBuffer, lineStippleFactor, lineStipplePattern);
      }
      VkResult __attribute__ ((weak))
      anv_CreateDmaBufImageINTEL(VkDevice device, const VkDmaBufImageCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMem, VkImage* pImage)
      {
          ANV_FROM_HANDLE(anv_device, anv_device, device);
          return anv_device->dispatch.vkCreateDmaBufImageINTEL(device, pCreateInfo, pAllocator, pMem, pImage);
      }

  const struct anv_device_dispatch_table anv_device_dispatch_table = {
    .vkGetDeviceProcAddr = anv_GetDeviceProcAddr,
    .vkDestroyDevice = anv_DestroyDevice,
    .vkGetDeviceQueue = anv_GetDeviceQueue,
    .vkQueueSubmit = anv_QueueSubmit,
    .vkQueueWaitIdle = anv_QueueWaitIdle,
    .vkDeviceWaitIdle = anv_DeviceWaitIdle,
    .vkAllocateMemory = anv_AllocateMemory,
    .vkFreeMemory = anv_FreeMemory,
    .vkMapMemory = anv_MapMemory,
    .vkUnmapMemory = anv_UnmapMemory,
    .vkFlushMappedMemoryRanges = anv_FlushMappedMemoryRanges,
    .vkInvalidateMappedMemoryRanges = anv_InvalidateMappedMemoryRanges,
    .vkGetDeviceMemoryCommitment = anv_GetDeviceMemoryCommitment,
    .vkGetBufferMemoryRequirements = anv_GetBufferMemoryRequirements,
    .vkBindBufferMemory = anv_BindBufferMemory,
    .vkGetImageMemoryRequirements = anv_GetImageMemoryRequirements,
    .vkBindImageMemory = anv_BindImageMemory,
    .vkGetImageSparseMemoryRequirements = anv_GetImageSparseMemoryRequirements,
    .vkQueueBindSparse = anv_QueueBindSparse,
    .vkCreateFence = anv_CreateFence,
    .vkDestroyFence = anv_DestroyFence,
    .vkResetFences = anv_ResetFences,
    .vkGetFenceStatus = anv_GetFenceStatus,
    .vkWaitForFences = anv_WaitForFences,
    .vkCreateSemaphore = anv_CreateSemaphore,
    .vkDestroySemaphore = anv_DestroySemaphore,
    .vkCreateEvent = anv_CreateEvent,
    .vkDestroyEvent = anv_DestroyEvent,
    .vkGetEventStatus = anv_GetEventStatus,
    .vkSetEvent = anv_SetEvent,
    .vkResetEvent = anv_ResetEvent,
    .vkCreateQueryPool = anv_CreateQueryPool,
    .vkDestroyQueryPool = anv_DestroyQueryPool,
    .vkGetQueryPoolResults = anv_GetQueryPoolResults,
    .vkResetQueryPoolEXT = anv_ResetQueryPoolEXT,
    .vkCreateBuffer = anv_CreateBuffer,
    .vkDestroyBuffer = anv_DestroyBuffer,
    .vkCreateBufferView = anv_CreateBufferView,
    .vkDestroyBufferView = anv_DestroyBufferView,
    .vkCreateImage = anv_CreateImage,
    .vkDestroyImage = anv_DestroyImage,
    .vkGetImageSubresourceLayout = anv_GetImageSubresourceLayout,
    .vkCreateImageView = anv_CreateImageView,
    .vkDestroyImageView = anv_DestroyImageView,
    .vkCreateShaderModule = anv_CreateShaderModule,
    .vkDestroyShaderModule = anv_DestroyShaderModule,
    .vkCreatePipelineCache = anv_CreatePipelineCache,
    .vkDestroyPipelineCache = anv_DestroyPipelineCache,
    .vkGetPipelineCacheData = anv_GetPipelineCacheData,
    .vkMergePipelineCaches = anv_MergePipelineCaches,
    .vkCreateGraphicsPipelines = anv_CreateGraphicsPipelines,
    .vkCreateComputePipelines = anv_CreateComputePipelines,
    .vkDestroyPipeline = anv_DestroyPipeline,
    .vkCreatePipelineLayout = anv_CreatePipelineLayout,
    .vkDestroyPipelineLayout = anv_DestroyPipelineLayout,
    .vkCreateSampler = anv_CreateSampler,
    .vkDestroySampler = anv_DestroySampler,
    .vkCreateDescriptorSetLayout = anv_CreateDescriptorSetLayout,
    .vkDestroyDescriptorSetLayout = anv_DestroyDescriptorSetLayout,
    .vkCreateDescriptorPool = anv_CreateDescriptorPool,
    .vkDestroyDescriptorPool = anv_DestroyDescriptorPool,
    .vkResetDescriptorPool = anv_ResetDescriptorPool,
    .vkAllocateDescriptorSets = anv_AllocateDescriptorSets,
    .vkFreeDescriptorSets = anv_FreeDescriptorSets,
    .vkUpdateDescriptorSets = anv_UpdateDescriptorSets,
    .vkCreateFramebuffer = anv_CreateFramebuffer,
    .vkDestroyFramebuffer = anv_DestroyFramebuffer,
    .vkCreateRenderPass = anv_CreateRenderPass,
    .vkDestroyRenderPass = anv_DestroyRenderPass,
    .vkGetRenderAreaGranularity = anv_GetRenderAreaGranularity,
    .vkCreateCommandPool = anv_CreateCommandPool,
    .vkDestroyCommandPool = anv_DestroyCommandPool,
    .vkResetCommandPool = anv_ResetCommandPool,
    .vkAllocateCommandBuffers = anv_AllocateCommandBuffers,
    .vkFreeCommandBuffers = anv_FreeCommandBuffers,
    .vkBeginCommandBuffer = anv_BeginCommandBuffer,
    .vkEndCommandBuffer = anv_EndCommandBuffer,
    .vkResetCommandBuffer = anv_ResetCommandBuffer,
    .vkCmdBindPipeline = anv_CmdBindPipeline,
    .vkCmdSetViewport = anv_CmdSetViewport,
    .vkCmdSetScissor = anv_CmdSetScissor,
    .vkCmdSetLineWidth = anv_CmdSetLineWidth,
    .vkCmdSetDepthBias = anv_CmdSetDepthBias,
    .vkCmdSetBlendConstants = anv_CmdSetBlendConstants,
    .vkCmdSetDepthBounds = anv_CmdSetDepthBounds,
    .vkCmdSetStencilCompareMask = anv_CmdSetStencilCompareMask,
    .vkCmdSetStencilWriteMask = anv_CmdSetStencilWriteMask,
    .vkCmdSetStencilReference = anv_CmdSetStencilReference,
    .vkCmdBindDescriptorSets = anv_CmdBindDescriptorSets,
    .vkCmdBindIndexBuffer = anv_CmdBindIndexBuffer,
    .vkCmdBindVertexBuffers = anv_CmdBindVertexBuffers,
    .vkCmdDraw = anv_CmdDraw,
    .vkCmdDrawIndexed = anv_CmdDrawIndexed,
    .vkCmdDrawIndirect = anv_CmdDrawIndirect,
    .vkCmdDrawIndexedIndirect = anv_CmdDrawIndexedIndirect,
    .vkCmdDispatch = anv_CmdDispatch,
    .vkCmdDispatchIndirect = anv_CmdDispatchIndirect,
    .vkCmdCopyBuffer = anv_CmdCopyBuffer,
    .vkCmdCopyImage = anv_CmdCopyImage,
    .vkCmdBlitImage = anv_CmdBlitImage,
    .vkCmdCopyBufferToImage = anv_CmdCopyBufferToImage,
    .vkCmdCopyImageToBuffer = anv_CmdCopyImageToBuffer,
    .vkCmdUpdateBuffer = anv_CmdUpdateBuffer,
    .vkCmdFillBuffer = anv_CmdFillBuffer,
    .vkCmdClearColorImage = anv_CmdClearColorImage,
    .vkCmdClearDepthStencilImage = anv_CmdClearDepthStencilImage,
    .vkCmdClearAttachments = anv_CmdClearAttachments,
    .vkCmdResolveImage = anv_CmdResolveImage,
    .vkCmdSetEvent = anv_CmdSetEvent,
    .vkCmdResetEvent = anv_CmdResetEvent,
    .vkCmdWaitEvents = anv_CmdWaitEvents,
    .vkCmdPipelineBarrier = anv_CmdPipelineBarrier,
    .vkCmdBeginQuery = anv_CmdBeginQuery,
    .vkCmdEndQuery = anv_CmdEndQuery,
    .vkCmdBeginConditionalRenderingEXT = anv_CmdBeginConditionalRenderingEXT,
    .vkCmdEndConditionalRenderingEXT = anv_CmdEndConditionalRenderingEXT,
    .vkCmdResetQueryPool = anv_CmdResetQueryPool,
    .vkCmdWriteTimestamp = anv_CmdWriteTimestamp,
    .vkCmdCopyQueryPoolResults = anv_CmdCopyQueryPoolResults,
    .vkCmdPushConstants = anv_CmdPushConstants,
    .vkCmdBeginRenderPass = anv_CmdBeginRenderPass,
    .vkCmdNextSubpass = anv_CmdNextSubpass,
    .vkCmdEndRenderPass = anv_CmdEndRenderPass,
    .vkCmdExecuteCommands = anv_CmdExecuteCommands,
    .vkCreateSwapchainKHR = anv_CreateSwapchainKHR,
    .vkDestroySwapchainKHR = anv_DestroySwapchainKHR,
    .vkGetSwapchainImagesKHR = anv_GetSwapchainImagesKHR,
    .vkAcquireNextImageKHR = anv_AcquireNextImageKHR,
    .vkQueuePresentKHR = anv_QueuePresentKHR,
    .vkCmdPushDescriptorSetKHR = anv_CmdPushDescriptorSetKHR,
    .vkTrimCommandPool = anv_TrimCommandPool,
    .vkTrimCommandPoolKHR = anv_TrimCommandPool,
    .vkGetMemoryFdKHR = anv_GetMemoryFdKHR,
    .vkGetMemoryFdPropertiesKHR = anv_GetMemoryFdPropertiesKHR,
    .vkGetSemaphoreFdKHR = anv_GetSemaphoreFdKHR,
    .vkImportSemaphoreFdKHR = anv_ImportSemaphoreFdKHR,
    .vkGetFenceFdKHR = anv_GetFenceFdKHR,
    .vkImportFenceFdKHR = anv_ImportFenceFdKHR,
    .vkDisplayPowerControlEXT = anv_DisplayPowerControlEXT,
    .vkRegisterDeviceEventEXT = anv_RegisterDeviceEventEXT,
    .vkRegisterDisplayEventEXT = anv_RegisterDisplayEventEXT,
    .vkGetSwapchainCounterEXT = anv_GetSwapchainCounterEXT,
    .vkGetDeviceGroupPeerMemoryFeatures = anv_GetDeviceGroupPeerMemoryFeatures,
    .vkGetDeviceGroupPeerMemoryFeaturesKHR = anv_GetDeviceGroupPeerMemoryFeatures,
    .vkBindBufferMemory2 = anv_BindBufferMemory2,
    .vkBindBufferMemory2KHR = anv_BindBufferMemory2,
    .vkBindImageMemory2 = anv_BindImageMemory2,
    .vkBindImageMemory2KHR = anv_BindImageMemory2,
    .vkCmdSetDeviceMask = anv_CmdSetDeviceMask,
    .vkCmdSetDeviceMaskKHR = anv_CmdSetDeviceMask,
    .vkGetDeviceGroupPresentCapabilitiesKHR = anv_GetDeviceGroupPresentCapabilitiesKHR,
    .vkGetDeviceGroupSurfacePresentModesKHR = anv_GetDeviceGroupSurfacePresentModesKHR,
    .vkAcquireNextImage2KHR = anv_AcquireNextImage2KHR,
    .vkCmdDispatchBase = anv_CmdDispatchBase,
    .vkCmdDispatchBaseKHR = anv_CmdDispatchBase,
    .vkCreateDescriptorUpdateTemplate = anv_CreateDescriptorUpdateTemplate,
    .vkCreateDescriptorUpdateTemplateKHR = anv_CreateDescriptorUpdateTemplate,
    .vkDestroyDescriptorUpdateTemplate = anv_DestroyDescriptorUpdateTemplate,
    .vkDestroyDescriptorUpdateTemplateKHR = anv_DestroyDescriptorUpdateTemplate,
    .vkUpdateDescriptorSetWithTemplate = anv_UpdateDescriptorSetWithTemplate,
    .vkUpdateDescriptorSetWithTemplateKHR = anv_UpdateDescriptorSetWithTemplate,
    .vkCmdPushDescriptorSetWithTemplateKHR = anv_CmdPushDescriptorSetWithTemplateKHR,
    .vkGetBufferMemoryRequirements2 = anv_GetBufferMemoryRequirements2,
    .vkGetBufferMemoryRequirements2KHR = anv_GetBufferMemoryRequirements2,
    .vkGetImageMemoryRequirements2 = anv_GetImageMemoryRequirements2,
    .vkGetImageMemoryRequirements2KHR = anv_GetImageMemoryRequirements2,
    .vkGetImageSparseMemoryRequirements2 = anv_GetImageSparseMemoryRequirements2,
    .vkGetImageSparseMemoryRequirements2KHR = anv_GetImageSparseMemoryRequirements2,
    .vkCreateSamplerYcbcrConversion = anv_CreateSamplerYcbcrConversion,
    .vkCreateSamplerYcbcrConversionKHR = anv_CreateSamplerYcbcrConversion,
    .vkDestroySamplerYcbcrConversion = anv_DestroySamplerYcbcrConversion,
    .vkDestroySamplerYcbcrConversionKHR = anv_DestroySamplerYcbcrConversion,
    .vkGetDeviceQueue2 = anv_GetDeviceQueue2,
    .vkGetDescriptorSetLayoutSupport = anv_GetDescriptorSetLayoutSupport,
    .vkGetDescriptorSetLayoutSupportKHR = anv_GetDescriptorSetLayoutSupport,
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .vkGetSwapchainGrallocUsageANDROID = anv_GetSwapchainGrallocUsageANDROID,
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .vkGetSwapchainGrallocUsage2ANDROID = anv_GetSwapchainGrallocUsage2ANDROID,
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .vkAcquireImageANDROID = anv_AcquireImageANDROID,
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .vkQueueSignalReleaseImageANDROID = anv_QueueSignalReleaseImageANDROID,
#endif // VK_USE_PLATFORM_ANDROID_KHR
    .vkGetCalibratedTimestampsEXT = anv_GetCalibratedTimestampsEXT,
    .vkGetMemoryHostPointerPropertiesEXT = anv_GetMemoryHostPointerPropertiesEXT,
    .vkCreateRenderPass2KHR = anv_CreateRenderPass2KHR,
    .vkCmdBeginRenderPass2KHR = anv_CmdBeginRenderPass2KHR,
    .vkCmdNextSubpass2KHR = anv_CmdNextSubpass2KHR,
    .vkCmdEndRenderPass2KHR = anv_CmdEndRenderPass2KHR,
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .vkGetAndroidHardwareBufferPropertiesANDROID = anv_GetAndroidHardwareBufferPropertiesANDROID,
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .vkGetMemoryAndroidHardwareBufferANDROID = anv_GetMemoryAndroidHardwareBufferANDROID,
#endif // VK_USE_PLATFORM_ANDROID_KHR
    .vkCmdDrawIndirectCountKHR = anv_CmdDrawIndirectCountKHR,
    .vkCmdDrawIndexedIndirectCountKHR = anv_CmdDrawIndexedIndirectCountKHR,
    .vkCmdBindTransformFeedbackBuffersEXT = anv_CmdBindTransformFeedbackBuffersEXT,
    .vkCmdBeginTransformFeedbackEXT = anv_CmdBeginTransformFeedbackEXT,
    .vkCmdEndTransformFeedbackEXT = anv_CmdEndTransformFeedbackEXT,
    .vkCmdBeginQueryIndexedEXT = anv_CmdBeginQueryIndexedEXT,
    .vkCmdEndQueryIndexedEXT = anv_CmdEndQueryIndexedEXT,
    .vkCmdDrawIndirectByteCountEXT = anv_CmdDrawIndirectByteCountEXT,
    .vkGetBufferDeviceAddressEXT = anv_GetBufferDeviceAddressEXT,
    .vkGetPipelineExecutablePropertiesKHR = anv_GetPipelineExecutablePropertiesKHR,
    .vkGetPipelineExecutableStatisticsKHR = anv_GetPipelineExecutableStatisticsKHR,
    .vkGetPipelineExecutableInternalRepresentationsKHR = anv_GetPipelineExecutableInternalRepresentationsKHR,
    .vkCmdSetLineStippleEXT = anv_CmdSetLineStippleEXT,
    .vkCreateDmaBufImageINTEL = anv_CreateDmaBufImageINTEL,
  };
      PFN_vkVoidFunction gen7_GetDeviceProcAddr(VkDevice device, const char* pName) __attribute__ ((weak));
      void gen7_DestroyDevice(VkDevice device, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      void gen7_GetDeviceQueue(VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue) __attribute__ ((weak));
      VkResult gen7_QueueSubmit(VkQueue queue, uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence) __attribute__ ((weak));
      VkResult gen7_QueueWaitIdle(VkQueue queue) __attribute__ ((weak));
      VkResult gen7_DeviceWaitIdle(VkDevice device) __attribute__ ((weak));
      VkResult gen7_AllocateMemory(VkDevice device, const VkMemoryAllocateInfo* pAllocateInfo, const VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory) __attribute__ ((weak));
      void gen7_FreeMemory(VkDevice device, VkDeviceMemory memory, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen7_MapMemory(VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData) __attribute__ ((weak));
      void gen7_UnmapMemory(VkDevice device, VkDeviceMemory memory) __attribute__ ((weak));
      VkResult gen7_FlushMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges) __attribute__ ((weak));
      VkResult gen7_InvalidateMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges) __attribute__ ((weak));
      void gen7_GetDeviceMemoryCommitment(VkDevice device, VkDeviceMemory memory, VkDeviceSize* pCommittedMemoryInBytes) __attribute__ ((weak));
      void gen7_GetBufferMemoryRequirements(VkDevice device, VkBuffer buffer, VkMemoryRequirements* pMemoryRequirements) __attribute__ ((weak));
      VkResult gen7_BindBufferMemory(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset) __attribute__ ((weak));
      void gen7_GetImageMemoryRequirements(VkDevice device, VkImage image, VkMemoryRequirements* pMemoryRequirements) __attribute__ ((weak));
      VkResult gen7_BindImageMemory(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset) __attribute__ ((weak));
      void gen7_GetImageSparseMemoryRequirements(VkDevice device, VkImage image, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements* pSparseMemoryRequirements) __attribute__ ((weak));
      VkResult gen7_QueueBindSparse(VkQueue queue, uint32_t bindInfoCount, const VkBindSparseInfo* pBindInfo, VkFence fence) __attribute__ ((weak));
      VkResult gen7_CreateFence(VkDevice device, const VkFenceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) __attribute__ ((weak));
      void gen7_DestroyFence(VkDevice device, VkFence fence, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen7_ResetFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences) __attribute__ ((weak));
      VkResult gen7_GetFenceStatus(VkDevice device, VkFence fence) __attribute__ ((weak));
      VkResult gen7_WaitForFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences, VkBool32 waitAll, uint64_t timeout) __attribute__ ((weak));
      VkResult gen7_CreateSemaphore(VkDevice device, const VkSemaphoreCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore) __attribute__ ((weak));
      void gen7_DestroySemaphore(VkDevice device, VkSemaphore semaphore, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen7_CreateEvent(VkDevice device, const VkEventCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkEvent* pEvent) __attribute__ ((weak));
      void gen7_DestroyEvent(VkDevice device, VkEvent event, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen7_GetEventStatus(VkDevice device, VkEvent event) __attribute__ ((weak));
      VkResult gen7_SetEvent(VkDevice device, VkEvent event) __attribute__ ((weak));
      VkResult gen7_ResetEvent(VkDevice device, VkEvent event) __attribute__ ((weak));
      VkResult gen7_CreateQueryPool(VkDevice device, const VkQueryPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkQueryPool* pQueryPool) __attribute__ ((weak));
      void gen7_DestroyQueryPool(VkDevice device, VkQueryPool queryPool, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen7_GetQueryPoolResults(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, VkDeviceSize stride, VkQueryResultFlags flags) __attribute__ ((weak));
      void gen7_ResetQueryPoolEXT(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) __attribute__ ((weak));
      VkResult gen7_CreateBuffer(VkDevice device, const VkBufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer) __attribute__ ((weak));
      void gen7_DestroyBuffer(VkDevice device, VkBuffer buffer, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen7_CreateBufferView(VkDevice device, const VkBufferViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferView* pView) __attribute__ ((weak));
      void gen7_DestroyBufferView(VkDevice device, VkBufferView bufferView, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen7_CreateImage(VkDevice device, const VkImageCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImage* pImage) __attribute__ ((weak));
      void gen7_DestroyImage(VkDevice device, VkImage image, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      void gen7_GetImageSubresourceLayout(VkDevice device, VkImage image, const VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout) __attribute__ ((weak));
      VkResult gen7_CreateImageView(VkDevice device, const VkImageViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImageView* pView) __attribute__ ((weak));
      void gen7_DestroyImageView(VkDevice device, VkImageView imageView, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen7_CreateShaderModule(VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule) __attribute__ ((weak));
      void gen7_DestroyShaderModule(VkDevice device, VkShaderModule shaderModule, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen7_CreatePipelineCache(VkDevice device, const VkPipelineCacheCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineCache* pPipelineCache) __attribute__ ((weak));
      void gen7_DestroyPipelineCache(VkDevice device, VkPipelineCache pipelineCache, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen7_GetPipelineCacheData(VkDevice device, VkPipelineCache pipelineCache, size_t* pDataSize, void* pData) __attribute__ ((weak));
      VkResult gen7_MergePipelineCaches(VkDevice device, VkPipelineCache dstCache, uint32_t srcCacheCount, const VkPipelineCache* pSrcCaches) __attribute__ ((weak));
      VkResult gen7_CreateGraphicsPipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkGraphicsPipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) __attribute__ ((weak));
      VkResult gen7_CreateComputePipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkComputePipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) __attribute__ ((weak));
      void gen7_DestroyPipeline(VkDevice device, VkPipeline pipeline, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen7_CreatePipelineLayout(VkDevice device, const VkPipelineLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout) __attribute__ ((weak));
      void gen7_DestroyPipelineLayout(VkDevice device, VkPipelineLayout pipelineLayout, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen7_CreateSampler(VkDevice device, const VkSamplerCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSampler* pSampler) __attribute__ ((weak));
      void gen7_DestroySampler(VkDevice device, VkSampler sampler, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen7_CreateDescriptorSetLayout(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorSetLayout* pSetLayout) __attribute__ ((weak));
      void gen7_DestroyDescriptorSetLayout(VkDevice device, VkDescriptorSetLayout descriptorSetLayout, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen7_CreateDescriptorPool(VkDevice device, const VkDescriptorPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorPool* pDescriptorPool) __attribute__ ((weak));
      void gen7_DestroyDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen7_ResetDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags flags) __attribute__ ((weak));
      VkResult gen7_AllocateDescriptorSets(VkDevice device, const VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets) __attribute__ ((weak));
      VkResult gen7_FreeDescriptorSets(VkDevice device, VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets) __attribute__ ((weak));
      void gen7_UpdateDescriptorSets(VkDevice device, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, const VkCopyDescriptorSet* pDescriptorCopies) __attribute__ ((weak));
      VkResult gen7_CreateFramebuffer(VkDevice device, const VkFramebufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFramebuffer* pFramebuffer) __attribute__ ((weak));
      void gen7_DestroyFramebuffer(VkDevice device, VkFramebuffer framebuffer, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen7_CreateRenderPass(VkDevice device, const VkRenderPassCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) __attribute__ ((weak));
      void gen7_DestroyRenderPass(VkDevice device, VkRenderPass renderPass, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      void gen7_GetRenderAreaGranularity(VkDevice device, VkRenderPass renderPass, VkExtent2D* pGranularity) __attribute__ ((weak));
      VkResult gen7_CreateCommandPool(VkDevice device, const VkCommandPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool) __attribute__ ((weak));
      void gen7_DestroyCommandPool(VkDevice device, VkCommandPool commandPool, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen7_ResetCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolResetFlags flags) __attribute__ ((weak));
      VkResult gen7_AllocateCommandBuffers(VkDevice device, const VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers) __attribute__ ((weak));
      void gen7_FreeCommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) __attribute__ ((weak));
      VkResult gen7_BeginCommandBuffer(VkCommandBuffer commandBuffer, const VkCommandBufferBeginInfo* pBeginInfo) __attribute__ ((weak));
      VkResult gen7_EndCommandBuffer(VkCommandBuffer commandBuffer) __attribute__ ((weak));
      VkResult gen7_ResetCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags) __attribute__ ((weak));
      void gen7_CmdBindPipeline(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline) __attribute__ ((weak));
      void gen7_CmdSetViewport(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pViewports) __attribute__ ((weak));
      void gen7_CmdSetScissor(VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pScissors) __attribute__ ((weak));
      void gen7_CmdSetLineWidth(VkCommandBuffer commandBuffer, float lineWidth) __attribute__ ((weak));
      void gen7_CmdSetDepthBias(VkCommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor) __attribute__ ((weak));
      void gen7_CmdSetBlendConstants(VkCommandBuffer commandBuffer, const float blendConstants[4]) __attribute__ ((weak));
      void gen7_CmdSetDepthBounds(VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds) __attribute__ ((weak));
      void gen7_CmdSetStencilCompareMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask) __attribute__ ((weak));
      void gen7_CmdSetStencilWriteMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask) __attribute__ ((weak));
      void gen7_CmdSetStencilReference(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference) __attribute__ ((weak));
      void gen7_CmdBindDescriptorSets(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets) __attribute__ ((weak));
      void gen7_CmdBindIndexBuffer(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType) __attribute__ ((weak));
      void gen7_CmdBindVertexBuffers(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets) __attribute__ ((weak));
      void gen7_CmdDraw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) __attribute__ ((weak));
      void gen7_CmdDrawIndexed(VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) __attribute__ ((weak));
      void gen7_CmdDrawIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) __attribute__ ((weak));
      void gen7_CmdDrawIndexedIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) __attribute__ ((weak));
      void gen7_CmdDispatch(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) __attribute__ ((weak));
      void gen7_CmdDispatchIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset) __attribute__ ((weak));
      void gen7_CmdCopyBuffer(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions) __attribute__ ((weak));
      void gen7_CmdCopyImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions) __attribute__ ((weak));
      void gen7_CmdBlitImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter) __attribute__ ((weak));
      void gen7_CmdCopyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions) __attribute__ ((weak));
      void gen7_CmdCopyImageToBuffer(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions) __attribute__ ((weak));
      void gen7_CmdUpdateBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData) __attribute__ ((weak));
      void gen7_CmdFillBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data) __attribute__ ((weak));
      void gen7_CmdClearColorImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) __attribute__ ((weak));
      void gen7_CmdClearDepthStencilImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) __attribute__ ((weak));
      void gen7_CmdClearAttachments(VkCommandBuffer commandBuffer, uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects) __attribute__ ((weak));
      void gen7_CmdResolveImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageResolve* pRegions) __attribute__ ((weak));
      void gen7_CmdSetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) __attribute__ ((weak));
      void gen7_CmdResetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) __attribute__ ((weak));
      void gen7_CmdWaitEvents(VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) __attribute__ ((weak));
      void gen7_CmdPipelineBarrier(VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) __attribute__ ((weak));
      void gen7_CmdBeginQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags) __attribute__ ((weak));
      void gen7_CmdEndQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query) __attribute__ ((weak));
      void gen7_CmdBeginConditionalRenderingEXT(VkCommandBuffer commandBuffer, const VkConditionalRenderingBeginInfoEXT* pConditionalRenderingBegin) __attribute__ ((weak));
      void gen7_CmdEndConditionalRenderingEXT(VkCommandBuffer commandBuffer) __attribute__ ((weak));
      void gen7_CmdResetQueryPool(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) __attribute__ ((weak));
      void gen7_CmdWriteTimestamp(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query) __attribute__ ((weak));
      void gen7_CmdCopyQueryPoolResults(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags) __attribute__ ((weak));
      void gen7_CmdPushConstants(VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues) __attribute__ ((weak));
      void gen7_CmdBeginRenderPass(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents) __attribute__ ((weak));
      void gen7_CmdNextSubpass(VkCommandBuffer commandBuffer, VkSubpassContents contents) __attribute__ ((weak));
      void gen7_CmdEndRenderPass(VkCommandBuffer commandBuffer) __attribute__ ((weak));
      void gen7_CmdExecuteCommands(VkCommandBuffer commandBuffer, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) __attribute__ ((weak));
      VkResult gen7_CreateSwapchainKHR(VkDevice device, const VkSwapchainCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain) __attribute__ ((weak));
      void gen7_DestroySwapchainKHR(VkDevice device, VkSwapchainKHR swapchain, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen7_GetSwapchainImagesKHR(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages) __attribute__ ((weak));
      VkResult gen7_AcquireNextImageKHR(VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex) __attribute__ ((weak));
      VkResult gen7_QueuePresentKHR(VkQueue queue, const VkPresentInfoKHR* pPresentInfo) __attribute__ ((weak));
      void gen7_CmdPushDescriptorSetKHR(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites) __attribute__ ((weak));
      void gen7_TrimCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags) __attribute__ ((weak));
            VkResult gen7_GetMemoryFdKHR(VkDevice device, const VkMemoryGetFdInfoKHR* pGetFdInfo, int* pFd) __attribute__ ((weak));
      VkResult gen7_GetMemoryFdPropertiesKHR(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, int fd, VkMemoryFdPropertiesKHR* pMemoryFdProperties) __attribute__ ((weak));
      VkResult gen7_GetSemaphoreFdKHR(VkDevice device, const VkSemaphoreGetFdInfoKHR* pGetFdInfo, int* pFd) __attribute__ ((weak));
      VkResult gen7_ImportSemaphoreFdKHR(VkDevice device, const VkImportSemaphoreFdInfoKHR* pImportSemaphoreFdInfo) __attribute__ ((weak));
      VkResult gen7_GetFenceFdKHR(VkDevice device, const VkFenceGetFdInfoKHR* pGetFdInfo, int* pFd) __attribute__ ((weak));
      VkResult gen7_ImportFenceFdKHR(VkDevice device, const VkImportFenceFdInfoKHR* pImportFenceFdInfo) __attribute__ ((weak));
      VkResult gen7_DisplayPowerControlEXT(VkDevice device, VkDisplayKHR display, const VkDisplayPowerInfoEXT* pDisplayPowerInfo) __attribute__ ((weak));
      VkResult gen7_RegisterDeviceEventEXT(VkDevice device, const VkDeviceEventInfoEXT* pDeviceEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) __attribute__ ((weak));
      VkResult gen7_RegisterDisplayEventEXT(VkDevice device, VkDisplayKHR display, const VkDisplayEventInfoEXT* pDisplayEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) __attribute__ ((weak));
      VkResult gen7_GetSwapchainCounterEXT(VkDevice device, VkSwapchainKHR swapchain, VkSurfaceCounterFlagBitsEXT counter, uint64_t* pCounterValue) __attribute__ ((weak));
      void gen7_GetDeviceGroupPeerMemoryFeatures(VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures) __attribute__ ((weak));
            VkResult gen7_BindBufferMemory2(VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos) __attribute__ ((weak));
            VkResult gen7_BindImageMemory2(VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos) __attribute__ ((weak));
            void gen7_CmdSetDeviceMask(VkCommandBuffer commandBuffer, uint32_t deviceMask) __attribute__ ((weak));
            VkResult gen7_GetDeviceGroupPresentCapabilitiesKHR(VkDevice device, VkDeviceGroupPresentCapabilitiesKHR* pDeviceGroupPresentCapabilities) __attribute__ ((weak));
      VkResult gen7_GetDeviceGroupSurfacePresentModesKHR(VkDevice device, VkSurfaceKHR surface, VkDeviceGroupPresentModeFlagsKHR* pModes) __attribute__ ((weak));
      VkResult gen7_AcquireNextImage2KHR(VkDevice device, const VkAcquireNextImageInfoKHR* pAcquireInfo, uint32_t* pImageIndex) __attribute__ ((weak));
      void gen7_CmdDispatchBase(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) __attribute__ ((weak));
            VkResult gen7_CreateDescriptorUpdateTemplate(VkDevice device, const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate) __attribute__ ((weak));
            void gen7_DestroyDescriptorUpdateTemplate(VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
            void gen7_UpdateDescriptorSetWithTemplate(VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData) __attribute__ ((weak));
            void gen7_CmdPushDescriptorSetWithTemplateKHR(VkCommandBuffer commandBuffer, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, const void* pData) __attribute__ ((weak));
      void gen7_GetBufferMemoryRequirements2(VkDevice device, const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) __attribute__ ((weak));
            void gen7_GetImageMemoryRequirements2(VkDevice device, const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) __attribute__ ((weak));
            void gen7_GetImageSparseMemoryRequirements2(VkDevice device, const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) __attribute__ ((weak));
            VkResult gen7_CreateSamplerYcbcrConversion(VkDevice device, const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion) __attribute__ ((weak));
            void gen7_DestroySamplerYcbcrConversion(VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
            void gen7_GetDeviceQueue2(VkDevice device, const VkDeviceQueueInfo2* pQueueInfo, VkQueue* pQueue) __attribute__ ((weak));
      void gen7_GetDescriptorSetLayoutSupport(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport) __attribute__ ((weak));
      #ifdef VK_USE_PLATFORM_ANDROID_KHR
      VkResult gen7_GetSwapchainGrallocUsageANDROID(VkDevice device, VkFormat format, VkImageUsageFlags imageUsage, int* grallocUsage) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
      VkResult gen7_GetSwapchainGrallocUsage2ANDROID(VkDevice device, VkFormat format, VkImageUsageFlags imageUsage, VkSwapchainImageUsageFlagsANDROID swapchainImageUsage, uint64_t* grallocConsumerUsage, uint64_t* grallocProducerUsage) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
      VkResult gen7_AcquireImageANDROID(VkDevice device, VkImage image, int nativeFenceFd, VkSemaphore semaphore, VkFence fence) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
      VkResult gen7_QueueSignalReleaseImageANDROID(VkQueue queue, uint32_t waitSemaphoreCount, const VkSemaphore* pWaitSemaphores, VkImage image, int* pNativeFenceFd) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_ANDROID_KHR
      VkResult gen7_GetCalibratedTimestampsEXT(VkDevice device, uint32_t timestampCount, const VkCalibratedTimestampInfoEXT* pTimestampInfos, uint64_t* pTimestamps, uint64_t* pMaxDeviation) __attribute__ ((weak));
      VkResult gen7_GetMemoryHostPointerPropertiesEXT(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, const void* pHostPointer, VkMemoryHostPointerPropertiesEXT* pMemoryHostPointerProperties) __attribute__ ((weak));
      VkResult gen7_CreateRenderPass2KHR(VkDevice device, const VkRenderPassCreateInfo2KHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) __attribute__ ((weak));
      void gen7_CmdBeginRenderPass2KHR(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo*      pRenderPassBegin, const VkSubpassBeginInfoKHR*      pSubpassBeginInfo) __attribute__ ((weak));
      void gen7_CmdNextSubpass2KHR(VkCommandBuffer commandBuffer, const VkSubpassBeginInfoKHR*      pSubpassBeginInfo, const VkSubpassEndInfoKHR*        pSubpassEndInfo) __attribute__ ((weak));
      void gen7_CmdEndRenderPass2KHR(VkCommandBuffer commandBuffer, const VkSubpassEndInfoKHR*        pSubpassEndInfo) __attribute__ ((weak));
#ifdef VK_USE_PLATFORM_ANDROID_KHR
      VkResult gen7_GetAndroidHardwareBufferPropertiesANDROID(VkDevice device, const struct AHardwareBuffer* buffer, VkAndroidHardwareBufferPropertiesANDROID* pProperties) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
      VkResult gen7_GetMemoryAndroidHardwareBufferANDROID(VkDevice device, const VkMemoryGetAndroidHardwareBufferInfoANDROID* pInfo, struct AHardwareBuffer** pBuffer) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_ANDROID_KHR
      void gen7_CmdDrawIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) __attribute__ ((weak));
      void gen7_CmdDrawIndexedIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) __attribute__ ((weak));
      void gen7_CmdBindTransformFeedbackBuffersEXT(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes) __attribute__ ((weak));
      void gen7_CmdBeginTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets) __attribute__ ((weak));
      void gen7_CmdEndTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets) __attribute__ ((weak));
      void gen7_CmdBeginQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags, uint32_t index) __attribute__ ((weak));
      void gen7_CmdEndQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, uint32_t index) __attribute__ ((weak));
      void gen7_CmdDrawIndirectByteCountEXT(VkCommandBuffer commandBuffer, uint32_t instanceCount, uint32_t firstInstance, VkBuffer counterBuffer, VkDeviceSize counterBufferOffset, uint32_t counterOffset, uint32_t vertexStride) __attribute__ ((weak));
      VkDeviceAddress gen7_GetBufferDeviceAddressEXT(VkDevice device, const VkBufferDeviceAddressInfoEXT* pInfo) __attribute__ ((weak));
      VkResult gen7_GetPipelineExecutablePropertiesKHR(VkDevice                        device, const VkPipelineInfoKHR*        pPipelineInfo, uint32_t* pExecutableCount, VkPipelineExecutablePropertiesKHR* pProperties) __attribute__ ((weak));
      VkResult gen7_GetPipelineExecutableStatisticsKHR(VkDevice                        device, const VkPipelineExecutableInfoKHR*  pExecutableInfo, uint32_t* pStatisticCount, VkPipelineExecutableStatisticKHR* pStatistics) __attribute__ ((weak));
      VkResult gen7_GetPipelineExecutableInternalRepresentationsKHR(VkDevice                        device, const VkPipelineExecutableInfoKHR*  pExecutableInfo, uint32_t* pInternalRepresentationCount, VkPipelineExecutableInternalRepresentationKHR* pInternalRepresentations) __attribute__ ((weak));
      void gen7_CmdSetLineStippleEXT(VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern) __attribute__ ((weak));
      VkResult gen7_CreateDmaBufImageINTEL(VkDevice device, const VkDmaBufImageCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMem, VkImage* pImage) __attribute__ ((weak));

  const struct anv_device_dispatch_table gen7_device_dispatch_table = {
    .vkGetDeviceProcAddr = gen7_GetDeviceProcAddr,
    .vkDestroyDevice = gen7_DestroyDevice,
    .vkGetDeviceQueue = gen7_GetDeviceQueue,
    .vkQueueSubmit = gen7_QueueSubmit,
    .vkQueueWaitIdle = gen7_QueueWaitIdle,
    .vkDeviceWaitIdle = gen7_DeviceWaitIdle,
    .vkAllocateMemory = gen7_AllocateMemory,
    .vkFreeMemory = gen7_FreeMemory,
    .vkMapMemory = gen7_MapMemory,
    .vkUnmapMemory = gen7_UnmapMemory,
    .vkFlushMappedMemoryRanges = gen7_FlushMappedMemoryRanges,
    .vkInvalidateMappedMemoryRanges = gen7_InvalidateMappedMemoryRanges,
    .vkGetDeviceMemoryCommitment = gen7_GetDeviceMemoryCommitment,
    .vkGetBufferMemoryRequirements = gen7_GetBufferMemoryRequirements,
    .vkBindBufferMemory = gen7_BindBufferMemory,
    .vkGetImageMemoryRequirements = gen7_GetImageMemoryRequirements,
    .vkBindImageMemory = gen7_BindImageMemory,
    .vkGetImageSparseMemoryRequirements = gen7_GetImageSparseMemoryRequirements,
    .vkQueueBindSparse = gen7_QueueBindSparse,
    .vkCreateFence = gen7_CreateFence,
    .vkDestroyFence = gen7_DestroyFence,
    .vkResetFences = gen7_ResetFences,
    .vkGetFenceStatus = gen7_GetFenceStatus,
    .vkWaitForFences = gen7_WaitForFences,
    .vkCreateSemaphore = gen7_CreateSemaphore,
    .vkDestroySemaphore = gen7_DestroySemaphore,
    .vkCreateEvent = gen7_CreateEvent,
    .vkDestroyEvent = gen7_DestroyEvent,
    .vkGetEventStatus = gen7_GetEventStatus,
    .vkSetEvent = gen7_SetEvent,
    .vkResetEvent = gen7_ResetEvent,
    .vkCreateQueryPool = gen7_CreateQueryPool,
    .vkDestroyQueryPool = gen7_DestroyQueryPool,
    .vkGetQueryPoolResults = gen7_GetQueryPoolResults,
    .vkResetQueryPoolEXT = gen7_ResetQueryPoolEXT,
    .vkCreateBuffer = gen7_CreateBuffer,
    .vkDestroyBuffer = gen7_DestroyBuffer,
    .vkCreateBufferView = gen7_CreateBufferView,
    .vkDestroyBufferView = gen7_DestroyBufferView,
    .vkCreateImage = gen7_CreateImage,
    .vkDestroyImage = gen7_DestroyImage,
    .vkGetImageSubresourceLayout = gen7_GetImageSubresourceLayout,
    .vkCreateImageView = gen7_CreateImageView,
    .vkDestroyImageView = gen7_DestroyImageView,
    .vkCreateShaderModule = gen7_CreateShaderModule,
    .vkDestroyShaderModule = gen7_DestroyShaderModule,
    .vkCreatePipelineCache = gen7_CreatePipelineCache,
    .vkDestroyPipelineCache = gen7_DestroyPipelineCache,
    .vkGetPipelineCacheData = gen7_GetPipelineCacheData,
    .vkMergePipelineCaches = gen7_MergePipelineCaches,
    .vkCreateGraphicsPipelines = gen7_CreateGraphicsPipelines,
    .vkCreateComputePipelines = gen7_CreateComputePipelines,
    .vkDestroyPipeline = gen7_DestroyPipeline,
    .vkCreatePipelineLayout = gen7_CreatePipelineLayout,
    .vkDestroyPipelineLayout = gen7_DestroyPipelineLayout,
    .vkCreateSampler = gen7_CreateSampler,
    .vkDestroySampler = gen7_DestroySampler,
    .vkCreateDescriptorSetLayout = gen7_CreateDescriptorSetLayout,
    .vkDestroyDescriptorSetLayout = gen7_DestroyDescriptorSetLayout,
    .vkCreateDescriptorPool = gen7_CreateDescriptorPool,
    .vkDestroyDescriptorPool = gen7_DestroyDescriptorPool,
    .vkResetDescriptorPool = gen7_ResetDescriptorPool,
    .vkAllocateDescriptorSets = gen7_AllocateDescriptorSets,
    .vkFreeDescriptorSets = gen7_FreeDescriptorSets,
    .vkUpdateDescriptorSets = gen7_UpdateDescriptorSets,
    .vkCreateFramebuffer = gen7_CreateFramebuffer,
    .vkDestroyFramebuffer = gen7_DestroyFramebuffer,
    .vkCreateRenderPass = gen7_CreateRenderPass,
    .vkDestroyRenderPass = gen7_DestroyRenderPass,
    .vkGetRenderAreaGranularity = gen7_GetRenderAreaGranularity,
    .vkCreateCommandPool = gen7_CreateCommandPool,
    .vkDestroyCommandPool = gen7_DestroyCommandPool,
    .vkResetCommandPool = gen7_ResetCommandPool,
    .vkAllocateCommandBuffers = gen7_AllocateCommandBuffers,
    .vkFreeCommandBuffers = gen7_FreeCommandBuffers,
    .vkBeginCommandBuffer = gen7_BeginCommandBuffer,
    .vkEndCommandBuffer = gen7_EndCommandBuffer,
    .vkResetCommandBuffer = gen7_ResetCommandBuffer,
    .vkCmdBindPipeline = gen7_CmdBindPipeline,
    .vkCmdSetViewport = gen7_CmdSetViewport,
    .vkCmdSetScissor = gen7_CmdSetScissor,
    .vkCmdSetLineWidth = gen7_CmdSetLineWidth,
    .vkCmdSetDepthBias = gen7_CmdSetDepthBias,
    .vkCmdSetBlendConstants = gen7_CmdSetBlendConstants,
    .vkCmdSetDepthBounds = gen7_CmdSetDepthBounds,
    .vkCmdSetStencilCompareMask = gen7_CmdSetStencilCompareMask,
    .vkCmdSetStencilWriteMask = gen7_CmdSetStencilWriteMask,
    .vkCmdSetStencilReference = gen7_CmdSetStencilReference,
    .vkCmdBindDescriptorSets = gen7_CmdBindDescriptorSets,
    .vkCmdBindIndexBuffer = gen7_CmdBindIndexBuffer,
    .vkCmdBindVertexBuffers = gen7_CmdBindVertexBuffers,
    .vkCmdDraw = gen7_CmdDraw,
    .vkCmdDrawIndexed = gen7_CmdDrawIndexed,
    .vkCmdDrawIndirect = gen7_CmdDrawIndirect,
    .vkCmdDrawIndexedIndirect = gen7_CmdDrawIndexedIndirect,
    .vkCmdDispatch = gen7_CmdDispatch,
    .vkCmdDispatchIndirect = gen7_CmdDispatchIndirect,
    .vkCmdCopyBuffer = gen7_CmdCopyBuffer,
    .vkCmdCopyImage = gen7_CmdCopyImage,
    .vkCmdBlitImage = gen7_CmdBlitImage,
    .vkCmdCopyBufferToImage = gen7_CmdCopyBufferToImage,
    .vkCmdCopyImageToBuffer = gen7_CmdCopyImageToBuffer,
    .vkCmdUpdateBuffer = gen7_CmdUpdateBuffer,
    .vkCmdFillBuffer = gen7_CmdFillBuffer,
    .vkCmdClearColorImage = gen7_CmdClearColorImage,
    .vkCmdClearDepthStencilImage = gen7_CmdClearDepthStencilImage,
    .vkCmdClearAttachments = gen7_CmdClearAttachments,
    .vkCmdResolveImage = gen7_CmdResolveImage,
    .vkCmdSetEvent = gen7_CmdSetEvent,
    .vkCmdResetEvent = gen7_CmdResetEvent,
    .vkCmdWaitEvents = gen7_CmdWaitEvents,
    .vkCmdPipelineBarrier = gen7_CmdPipelineBarrier,
    .vkCmdBeginQuery = gen7_CmdBeginQuery,
    .vkCmdEndQuery = gen7_CmdEndQuery,
    .vkCmdBeginConditionalRenderingEXT = gen7_CmdBeginConditionalRenderingEXT,
    .vkCmdEndConditionalRenderingEXT = gen7_CmdEndConditionalRenderingEXT,
    .vkCmdResetQueryPool = gen7_CmdResetQueryPool,
    .vkCmdWriteTimestamp = gen7_CmdWriteTimestamp,
    .vkCmdCopyQueryPoolResults = gen7_CmdCopyQueryPoolResults,
    .vkCmdPushConstants = gen7_CmdPushConstants,
    .vkCmdBeginRenderPass = gen7_CmdBeginRenderPass,
    .vkCmdNextSubpass = gen7_CmdNextSubpass,
    .vkCmdEndRenderPass = gen7_CmdEndRenderPass,
    .vkCmdExecuteCommands = gen7_CmdExecuteCommands,
    .vkCreateSwapchainKHR = gen7_CreateSwapchainKHR,
    .vkDestroySwapchainKHR = gen7_DestroySwapchainKHR,
    .vkGetSwapchainImagesKHR = gen7_GetSwapchainImagesKHR,
    .vkAcquireNextImageKHR = gen7_AcquireNextImageKHR,
    .vkQueuePresentKHR = gen7_QueuePresentKHR,
    .vkCmdPushDescriptorSetKHR = gen7_CmdPushDescriptorSetKHR,
    .vkTrimCommandPool = gen7_TrimCommandPool,
    .vkTrimCommandPoolKHR = gen7_TrimCommandPool,
    .vkGetMemoryFdKHR = gen7_GetMemoryFdKHR,
    .vkGetMemoryFdPropertiesKHR = gen7_GetMemoryFdPropertiesKHR,
    .vkGetSemaphoreFdKHR = gen7_GetSemaphoreFdKHR,
    .vkImportSemaphoreFdKHR = gen7_ImportSemaphoreFdKHR,
    .vkGetFenceFdKHR = gen7_GetFenceFdKHR,
    .vkImportFenceFdKHR = gen7_ImportFenceFdKHR,
    .vkDisplayPowerControlEXT = gen7_DisplayPowerControlEXT,
    .vkRegisterDeviceEventEXT = gen7_RegisterDeviceEventEXT,
    .vkRegisterDisplayEventEXT = gen7_RegisterDisplayEventEXT,
    .vkGetSwapchainCounterEXT = gen7_GetSwapchainCounterEXT,
    .vkGetDeviceGroupPeerMemoryFeatures = gen7_GetDeviceGroupPeerMemoryFeatures,
    .vkGetDeviceGroupPeerMemoryFeaturesKHR = gen7_GetDeviceGroupPeerMemoryFeatures,
    .vkBindBufferMemory2 = gen7_BindBufferMemory2,
    .vkBindBufferMemory2KHR = gen7_BindBufferMemory2,
    .vkBindImageMemory2 = gen7_BindImageMemory2,
    .vkBindImageMemory2KHR = gen7_BindImageMemory2,
    .vkCmdSetDeviceMask = gen7_CmdSetDeviceMask,
    .vkCmdSetDeviceMaskKHR = gen7_CmdSetDeviceMask,
    .vkGetDeviceGroupPresentCapabilitiesKHR = gen7_GetDeviceGroupPresentCapabilitiesKHR,
    .vkGetDeviceGroupSurfacePresentModesKHR = gen7_GetDeviceGroupSurfacePresentModesKHR,
    .vkAcquireNextImage2KHR = gen7_AcquireNextImage2KHR,
    .vkCmdDispatchBase = gen7_CmdDispatchBase,
    .vkCmdDispatchBaseKHR = gen7_CmdDispatchBase,
    .vkCreateDescriptorUpdateTemplate = gen7_CreateDescriptorUpdateTemplate,
    .vkCreateDescriptorUpdateTemplateKHR = gen7_CreateDescriptorUpdateTemplate,
    .vkDestroyDescriptorUpdateTemplate = gen7_DestroyDescriptorUpdateTemplate,
    .vkDestroyDescriptorUpdateTemplateKHR = gen7_DestroyDescriptorUpdateTemplate,
    .vkUpdateDescriptorSetWithTemplate = gen7_UpdateDescriptorSetWithTemplate,
    .vkUpdateDescriptorSetWithTemplateKHR = gen7_UpdateDescriptorSetWithTemplate,
    .vkCmdPushDescriptorSetWithTemplateKHR = gen7_CmdPushDescriptorSetWithTemplateKHR,
    .vkGetBufferMemoryRequirements2 = gen7_GetBufferMemoryRequirements2,
    .vkGetBufferMemoryRequirements2KHR = gen7_GetBufferMemoryRequirements2,
    .vkGetImageMemoryRequirements2 = gen7_GetImageMemoryRequirements2,
    .vkGetImageMemoryRequirements2KHR = gen7_GetImageMemoryRequirements2,
    .vkGetImageSparseMemoryRequirements2 = gen7_GetImageSparseMemoryRequirements2,
    .vkGetImageSparseMemoryRequirements2KHR = gen7_GetImageSparseMemoryRequirements2,
    .vkCreateSamplerYcbcrConversion = gen7_CreateSamplerYcbcrConversion,
    .vkCreateSamplerYcbcrConversionKHR = gen7_CreateSamplerYcbcrConversion,
    .vkDestroySamplerYcbcrConversion = gen7_DestroySamplerYcbcrConversion,
    .vkDestroySamplerYcbcrConversionKHR = gen7_DestroySamplerYcbcrConversion,
    .vkGetDeviceQueue2 = gen7_GetDeviceQueue2,
    .vkGetDescriptorSetLayoutSupport = gen7_GetDescriptorSetLayoutSupport,
    .vkGetDescriptorSetLayoutSupportKHR = gen7_GetDescriptorSetLayoutSupport,
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .vkGetSwapchainGrallocUsageANDROID = gen7_GetSwapchainGrallocUsageANDROID,
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .vkGetSwapchainGrallocUsage2ANDROID = gen7_GetSwapchainGrallocUsage2ANDROID,
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .vkAcquireImageANDROID = gen7_AcquireImageANDROID,
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .vkQueueSignalReleaseImageANDROID = gen7_QueueSignalReleaseImageANDROID,
#endif // VK_USE_PLATFORM_ANDROID_KHR
    .vkGetCalibratedTimestampsEXT = gen7_GetCalibratedTimestampsEXT,
    .vkGetMemoryHostPointerPropertiesEXT = gen7_GetMemoryHostPointerPropertiesEXT,
    .vkCreateRenderPass2KHR = gen7_CreateRenderPass2KHR,
    .vkCmdBeginRenderPass2KHR = gen7_CmdBeginRenderPass2KHR,
    .vkCmdNextSubpass2KHR = gen7_CmdNextSubpass2KHR,
    .vkCmdEndRenderPass2KHR = gen7_CmdEndRenderPass2KHR,
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .vkGetAndroidHardwareBufferPropertiesANDROID = gen7_GetAndroidHardwareBufferPropertiesANDROID,
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .vkGetMemoryAndroidHardwareBufferANDROID = gen7_GetMemoryAndroidHardwareBufferANDROID,
#endif // VK_USE_PLATFORM_ANDROID_KHR
    .vkCmdDrawIndirectCountKHR = gen7_CmdDrawIndirectCountKHR,
    .vkCmdDrawIndexedIndirectCountKHR = gen7_CmdDrawIndexedIndirectCountKHR,
    .vkCmdBindTransformFeedbackBuffersEXT = gen7_CmdBindTransformFeedbackBuffersEXT,
    .vkCmdBeginTransformFeedbackEXT = gen7_CmdBeginTransformFeedbackEXT,
    .vkCmdEndTransformFeedbackEXT = gen7_CmdEndTransformFeedbackEXT,
    .vkCmdBeginQueryIndexedEXT = gen7_CmdBeginQueryIndexedEXT,
    .vkCmdEndQueryIndexedEXT = gen7_CmdEndQueryIndexedEXT,
    .vkCmdDrawIndirectByteCountEXT = gen7_CmdDrawIndirectByteCountEXT,
    .vkGetBufferDeviceAddressEXT = gen7_GetBufferDeviceAddressEXT,
    .vkGetPipelineExecutablePropertiesKHR = gen7_GetPipelineExecutablePropertiesKHR,
    .vkGetPipelineExecutableStatisticsKHR = gen7_GetPipelineExecutableStatisticsKHR,
    .vkGetPipelineExecutableInternalRepresentationsKHR = gen7_GetPipelineExecutableInternalRepresentationsKHR,
    .vkCmdSetLineStippleEXT = gen7_CmdSetLineStippleEXT,
    .vkCreateDmaBufImageINTEL = gen7_CreateDmaBufImageINTEL,
  };
      PFN_vkVoidFunction gen75_GetDeviceProcAddr(VkDevice device, const char* pName) __attribute__ ((weak));
      void gen75_DestroyDevice(VkDevice device, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      void gen75_GetDeviceQueue(VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue) __attribute__ ((weak));
      VkResult gen75_QueueSubmit(VkQueue queue, uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence) __attribute__ ((weak));
      VkResult gen75_QueueWaitIdle(VkQueue queue) __attribute__ ((weak));
      VkResult gen75_DeviceWaitIdle(VkDevice device) __attribute__ ((weak));
      VkResult gen75_AllocateMemory(VkDevice device, const VkMemoryAllocateInfo* pAllocateInfo, const VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory) __attribute__ ((weak));
      void gen75_FreeMemory(VkDevice device, VkDeviceMemory memory, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen75_MapMemory(VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData) __attribute__ ((weak));
      void gen75_UnmapMemory(VkDevice device, VkDeviceMemory memory) __attribute__ ((weak));
      VkResult gen75_FlushMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges) __attribute__ ((weak));
      VkResult gen75_InvalidateMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges) __attribute__ ((weak));
      void gen75_GetDeviceMemoryCommitment(VkDevice device, VkDeviceMemory memory, VkDeviceSize* pCommittedMemoryInBytes) __attribute__ ((weak));
      void gen75_GetBufferMemoryRequirements(VkDevice device, VkBuffer buffer, VkMemoryRequirements* pMemoryRequirements) __attribute__ ((weak));
      VkResult gen75_BindBufferMemory(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset) __attribute__ ((weak));
      void gen75_GetImageMemoryRequirements(VkDevice device, VkImage image, VkMemoryRequirements* pMemoryRequirements) __attribute__ ((weak));
      VkResult gen75_BindImageMemory(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset) __attribute__ ((weak));
      void gen75_GetImageSparseMemoryRequirements(VkDevice device, VkImage image, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements* pSparseMemoryRequirements) __attribute__ ((weak));
      VkResult gen75_QueueBindSparse(VkQueue queue, uint32_t bindInfoCount, const VkBindSparseInfo* pBindInfo, VkFence fence) __attribute__ ((weak));
      VkResult gen75_CreateFence(VkDevice device, const VkFenceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) __attribute__ ((weak));
      void gen75_DestroyFence(VkDevice device, VkFence fence, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen75_ResetFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences) __attribute__ ((weak));
      VkResult gen75_GetFenceStatus(VkDevice device, VkFence fence) __attribute__ ((weak));
      VkResult gen75_WaitForFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences, VkBool32 waitAll, uint64_t timeout) __attribute__ ((weak));
      VkResult gen75_CreateSemaphore(VkDevice device, const VkSemaphoreCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore) __attribute__ ((weak));
      void gen75_DestroySemaphore(VkDevice device, VkSemaphore semaphore, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen75_CreateEvent(VkDevice device, const VkEventCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkEvent* pEvent) __attribute__ ((weak));
      void gen75_DestroyEvent(VkDevice device, VkEvent event, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen75_GetEventStatus(VkDevice device, VkEvent event) __attribute__ ((weak));
      VkResult gen75_SetEvent(VkDevice device, VkEvent event) __attribute__ ((weak));
      VkResult gen75_ResetEvent(VkDevice device, VkEvent event) __attribute__ ((weak));
      VkResult gen75_CreateQueryPool(VkDevice device, const VkQueryPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkQueryPool* pQueryPool) __attribute__ ((weak));
      void gen75_DestroyQueryPool(VkDevice device, VkQueryPool queryPool, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen75_GetQueryPoolResults(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, VkDeviceSize stride, VkQueryResultFlags flags) __attribute__ ((weak));
      void gen75_ResetQueryPoolEXT(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) __attribute__ ((weak));
      VkResult gen75_CreateBuffer(VkDevice device, const VkBufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer) __attribute__ ((weak));
      void gen75_DestroyBuffer(VkDevice device, VkBuffer buffer, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen75_CreateBufferView(VkDevice device, const VkBufferViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferView* pView) __attribute__ ((weak));
      void gen75_DestroyBufferView(VkDevice device, VkBufferView bufferView, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen75_CreateImage(VkDevice device, const VkImageCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImage* pImage) __attribute__ ((weak));
      void gen75_DestroyImage(VkDevice device, VkImage image, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      void gen75_GetImageSubresourceLayout(VkDevice device, VkImage image, const VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout) __attribute__ ((weak));
      VkResult gen75_CreateImageView(VkDevice device, const VkImageViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImageView* pView) __attribute__ ((weak));
      void gen75_DestroyImageView(VkDevice device, VkImageView imageView, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen75_CreateShaderModule(VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule) __attribute__ ((weak));
      void gen75_DestroyShaderModule(VkDevice device, VkShaderModule shaderModule, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen75_CreatePipelineCache(VkDevice device, const VkPipelineCacheCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineCache* pPipelineCache) __attribute__ ((weak));
      void gen75_DestroyPipelineCache(VkDevice device, VkPipelineCache pipelineCache, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen75_GetPipelineCacheData(VkDevice device, VkPipelineCache pipelineCache, size_t* pDataSize, void* pData) __attribute__ ((weak));
      VkResult gen75_MergePipelineCaches(VkDevice device, VkPipelineCache dstCache, uint32_t srcCacheCount, const VkPipelineCache* pSrcCaches) __attribute__ ((weak));
      VkResult gen75_CreateGraphicsPipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkGraphicsPipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) __attribute__ ((weak));
      VkResult gen75_CreateComputePipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkComputePipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) __attribute__ ((weak));
      void gen75_DestroyPipeline(VkDevice device, VkPipeline pipeline, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen75_CreatePipelineLayout(VkDevice device, const VkPipelineLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout) __attribute__ ((weak));
      void gen75_DestroyPipelineLayout(VkDevice device, VkPipelineLayout pipelineLayout, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen75_CreateSampler(VkDevice device, const VkSamplerCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSampler* pSampler) __attribute__ ((weak));
      void gen75_DestroySampler(VkDevice device, VkSampler sampler, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen75_CreateDescriptorSetLayout(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorSetLayout* pSetLayout) __attribute__ ((weak));
      void gen75_DestroyDescriptorSetLayout(VkDevice device, VkDescriptorSetLayout descriptorSetLayout, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen75_CreateDescriptorPool(VkDevice device, const VkDescriptorPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorPool* pDescriptorPool) __attribute__ ((weak));
      void gen75_DestroyDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen75_ResetDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags flags) __attribute__ ((weak));
      VkResult gen75_AllocateDescriptorSets(VkDevice device, const VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets) __attribute__ ((weak));
      VkResult gen75_FreeDescriptorSets(VkDevice device, VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets) __attribute__ ((weak));
      void gen75_UpdateDescriptorSets(VkDevice device, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, const VkCopyDescriptorSet* pDescriptorCopies) __attribute__ ((weak));
      VkResult gen75_CreateFramebuffer(VkDevice device, const VkFramebufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFramebuffer* pFramebuffer) __attribute__ ((weak));
      void gen75_DestroyFramebuffer(VkDevice device, VkFramebuffer framebuffer, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen75_CreateRenderPass(VkDevice device, const VkRenderPassCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) __attribute__ ((weak));
      void gen75_DestroyRenderPass(VkDevice device, VkRenderPass renderPass, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      void gen75_GetRenderAreaGranularity(VkDevice device, VkRenderPass renderPass, VkExtent2D* pGranularity) __attribute__ ((weak));
      VkResult gen75_CreateCommandPool(VkDevice device, const VkCommandPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool) __attribute__ ((weak));
      void gen75_DestroyCommandPool(VkDevice device, VkCommandPool commandPool, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen75_ResetCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolResetFlags flags) __attribute__ ((weak));
      VkResult gen75_AllocateCommandBuffers(VkDevice device, const VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers) __attribute__ ((weak));
      void gen75_FreeCommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) __attribute__ ((weak));
      VkResult gen75_BeginCommandBuffer(VkCommandBuffer commandBuffer, const VkCommandBufferBeginInfo* pBeginInfo) __attribute__ ((weak));
      VkResult gen75_EndCommandBuffer(VkCommandBuffer commandBuffer) __attribute__ ((weak));
      VkResult gen75_ResetCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags) __attribute__ ((weak));
      void gen75_CmdBindPipeline(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline) __attribute__ ((weak));
      void gen75_CmdSetViewport(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pViewports) __attribute__ ((weak));
      void gen75_CmdSetScissor(VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pScissors) __attribute__ ((weak));
      void gen75_CmdSetLineWidth(VkCommandBuffer commandBuffer, float lineWidth) __attribute__ ((weak));
      void gen75_CmdSetDepthBias(VkCommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor) __attribute__ ((weak));
      void gen75_CmdSetBlendConstants(VkCommandBuffer commandBuffer, const float blendConstants[4]) __attribute__ ((weak));
      void gen75_CmdSetDepthBounds(VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds) __attribute__ ((weak));
      void gen75_CmdSetStencilCompareMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask) __attribute__ ((weak));
      void gen75_CmdSetStencilWriteMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask) __attribute__ ((weak));
      void gen75_CmdSetStencilReference(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference) __attribute__ ((weak));
      void gen75_CmdBindDescriptorSets(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets) __attribute__ ((weak));
      void gen75_CmdBindIndexBuffer(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType) __attribute__ ((weak));
      void gen75_CmdBindVertexBuffers(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets) __attribute__ ((weak));
      void gen75_CmdDraw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) __attribute__ ((weak));
      void gen75_CmdDrawIndexed(VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) __attribute__ ((weak));
      void gen75_CmdDrawIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) __attribute__ ((weak));
      void gen75_CmdDrawIndexedIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) __attribute__ ((weak));
      void gen75_CmdDispatch(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) __attribute__ ((weak));
      void gen75_CmdDispatchIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset) __attribute__ ((weak));
      void gen75_CmdCopyBuffer(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions) __attribute__ ((weak));
      void gen75_CmdCopyImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions) __attribute__ ((weak));
      void gen75_CmdBlitImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter) __attribute__ ((weak));
      void gen75_CmdCopyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions) __attribute__ ((weak));
      void gen75_CmdCopyImageToBuffer(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions) __attribute__ ((weak));
      void gen75_CmdUpdateBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData) __attribute__ ((weak));
      void gen75_CmdFillBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data) __attribute__ ((weak));
      void gen75_CmdClearColorImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) __attribute__ ((weak));
      void gen75_CmdClearDepthStencilImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) __attribute__ ((weak));
      void gen75_CmdClearAttachments(VkCommandBuffer commandBuffer, uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects) __attribute__ ((weak));
      void gen75_CmdResolveImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageResolve* pRegions) __attribute__ ((weak));
      void gen75_CmdSetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) __attribute__ ((weak));
      void gen75_CmdResetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) __attribute__ ((weak));
      void gen75_CmdWaitEvents(VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) __attribute__ ((weak));
      void gen75_CmdPipelineBarrier(VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) __attribute__ ((weak));
      void gen75_CmdBeginQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags) __attribute__ ((weak));
      void gen75_CmdEndQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query) __attribute__ ((weak));
      void gen75_CmdBeginConditionalRenderingEXT(VkCommandBuffer commandBuffer, const VkConditionalRenderingBeginInfoEXT* pConditionalRenderingBegin) __attribute__ ((weak));
      void gen75_CmdEndConditionalRenderingEXT(VkCommandBuffer commandBuffer) __attribute__ ((weak));
      void gen75_CmdResetQueryPool(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) __attribute__ ((weak));
      void gen75_CmdWriteTimestamp(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query) __attribute__ ((weak));
      void gen75_CmdCopyQueryPoolResults(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags) __attribute__ ((weak));
      void gen75_CmdPushConstants(VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues) __attribute__ ((weak));
      void gen75_CmdBeginRenderPass(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents) __attribute__ ((weak));
      void gen75_CmdNextSubpass(VkCommandBuffer commandBuffer, VkSubpassContents contents) __attribute__ ((weak));
      void gen75_CmdEndRenderPass(VkCommandBuffer commandBuffer) __attribute__ ((weak));
      void gen75_CmdExecuteCommands(VkCommandBuffer commandBuffer, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) __attribute__ ((weak));
      VkResult gen75_CreateSwapchainKHR(VkDevice device, const VkSwapchainCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain) __attribute__ ((weak));
      void gen75_DestroySwapchainKHR(VkDevice device, VkSwapchainKHR swapchain, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen75_GetSwapchainImagesKHR(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages) __attribute__ ((weak));
      VkResult gen75_AcquireNextImageKHR(VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex) __attribute__ ((weak));
      VkResult gen75_QueuePresentKHR(VkQueue queue, const VkPresentInfoKHR* pPresentInfo) __attribute__ ((weak));
      void gen75_CmdPushDescriptorSetKHR(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites) __attribute__ ((weak));
      void gen75_TrimCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags) __attribute__ ((weak));
            VkResult gen75_GetMemoryFdKHR(VkDevice device, const VkMemoryGetFdInfoKHR* pGetFdInfo, int* pFd) __attribute__ ((weak));
      VkResult gen75_GetMemoryFdPropertiesKHR(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, int fd, VkMemoryFdPropertiesKHR* pMemoryFdProperties) __attribute__ ((weak));
      VkResult gen75_GetSemaphoreFdKHR(VkDevice device, const VkSemaphoreGetFdInfoKHR* pGetFdInfo, int* pFd) __attribute__ ((weak));
      VkResult gen75_ImportSemaphoreFdKHR(VkDevice device, const VkImportSemaphoreFdInfoKHR* pImportSemaphoreFdInfo) __attribute__ ((weak));
      VkResult gen75_GetFenceFdKHR(VkDevice device, const VkFenceGetFdInfoKHR* pGetFdInfo, int* pFd) __attribute__ ((weak));
      VkResult gen75_ImportFenceFdKHR(VkDevice device, const VkImportFenceFdInfoKHR* pImportFenceFdInfo) __attribute__ ((weak));
      VkResult gen75_DisplayPowerControlEXT(VkDevice device, VkDisplayKHR display, const VkDisplayPowerInfoEXT* pDisplayPowerInfo) __attribute__ ((weak));
      VkResult gen75_RegisterDeviceEventEXT(VkDevice device, const VkDeviceEventInfoEXT* pDeviceEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) __attribute__ ((weak));
      VkResult gen75_RegisterDisplayEventEXT(VkDevice device, VkDisplayKHR display, const VkDisplayEventInfoEXT* pDisplayEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) __attribute__ ((weak));
      VkResult gen75_GetSwapchainCounterEXT(VkDevice device, VkSwapchainKHR swapchain, VkSurfaceCounterFlagBitsEXT counter, uint64_t* pCounterValue) __attribute__ ((weak));
      void gen75_GetDeviceGroupPeerMemoryFeatures(VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures) __attribute__ ((weak));
            VkResult gen75_BindBufferMemory2(VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos) __attribute__ ((weak));
            VkResult gen75_BindImageMemory2(VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos) __attribute__ ((weak));
            void gen75_CmdSetDeviceMask(VkCommandBuffer commandBuffer, uint32_t deviceMask) __attribute__ ((weak));
            VkResult gen75_GetDeviceGroupPresentCapabilitiesKHR(VkDevice device, VkDeviceGroupPresentCapabilitiesKHR* pDeviceGroupPresentCapabilities) __attribute__ ((weak));
      VkResult gen75_GetDeviceGroupSurfacePresentModesKHR(VkDevice device, VkSurfaceKHR surface, VkDeviceGroupPresentModeFlagsKHR* pModes) __attribute__ ((weak));
      VkResult gen75_AcquireNextImage2KHR(VkDevice device, const VkAcquireNextImageInfoKHR* pAcquireInfo, uint32_t* pImageIndex) __attribute__ ((weak));
      void gen75_CmdDispatchBase(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) __attribute__ ((weak));
            VkResult gen75_CreateDescriptorUpdateTemplate(VkDevice device, const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate) __attribute__ ((weak));
            void gen75_DestroyDescriptorUpdateTemplate(VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
            void gen75_UpdateDescriptorSetWithTemplate(VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData) __attribute__ ((weak));
            void gen75_CmdPushDescriptorSetWithTemplateKHR(VkCommandBuffer commandBuffer, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, const void* pData) __attribute__ ((weak));
      void gen75_GetBufferMemoryRequirements2(VkDevice device, const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) __attribute__ ((weak));
            void gen75_GetImageMemoryRequirements2(VkDevice device, const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) __attribute__ ((weak));
            void gen75_GetImageSparseMemoryRequirements2(VkDevice device, const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) __attribute__ ((weak));
            VkResult gen75_CreateSamplerYcbcrConversion(VkDevice device, const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion) __attribute__ ((weak));
            void gen75_DestroySamplerYcbcrConversion(VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
            void gen75_GetDeviceQueue2(VkDevice device, const VkDeviceQueueInfo2* pQueueInfo, VkQueue* pQueue) __attribute__ ((weak));
      void gen75_GetDescriptorSetLayoutSupport(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport) __attribute__ ((weak));
      #ifdef VK_USE_PLATFORM_ANDROID_KHR
      VkResult gen75_GetSwapchainGrallocUsageANDROID(VkDevice device, VkFormat format, VkImageUsageFlags imageUsage, int* grallocUsage) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
      VkResult gen75_GetSwapchainGrallocUsage2ANDROID(VkDevice device, VkFormat format, VkImageUsageFlags imageUsage, VkSwapchainImageUsageFlagsANDROID swapchainImageUsage, uint64_t* grallocConsumerUsage, uint64_t* grallocProducerUsage) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
      VkResult gen75_AcquireImageANDROID(VkDevice device, VkImage image, int nativeFenceFd, VkSemaphore semaphore, VkFence fence) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
      VkResult gen75_QueueSignalReleaseImageANDROID(VkQueue queue, uint32_t waitSemaphoreCount, const VkSemaphore* pWaitSemaphores, VkImage image, int* pNativeFenceFd) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_ANDROID_KHR
      VkResult gen75_GetCalibratedTimestampsEXT(VkDevice device, uint32_t timestampCount, const VkCalibratedTimestampInfoEXT* pTimestampInfos, uint64_t* pTimestamps, uint64_t* pMaxDeviation) __attribute__ ((weak));
      VkResult gen75_GetMemoryHostPointerPropertiesEXT(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, const void* pHostPointer, VkMemoryHostPointerPropertiesEXT* pMemoryHostPointerProperties) __attribute__ ((weak));
      VkResult gen75_CreateRenderPass2KHR(VkDevice device, const VkRenderPassCreateInfo2KHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) __attribute__ ((weak));
      void gen75_CmdBeginRenderPass2KHR(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo*      pRenderPassBegin, const VkSubpassBeginInfoKHR*      pSubpassBeginInfo) __attribute__ ((weak));
      void gen75_CmdNextSubpass2KHR(VkCommandBuffer commandBuffer, const VkSubpassBeginInfoKHR*      pSubpassBeginInfo, const VkSubpassEndInfoKHR*        pSubpassEndInfo) __attribute__ ((weak));
      void gen75_CmdEndRenderPass2KHR(VkCommandBuffer commandBuffer, const VkSubpassEndInfoKHR*        pSubpassEndInfo) __attribute__ ((weak));
#ifdef VK_USE_PLATFORM_ANDROID_KHR
      VkResult gen75_GetAndroidHardwareBufferPropertiesANDROID(VkDevice device, const struct AHardwareBuffer* buffer, VkAndroidHardwareBufferPropertiesANDROID* pProperties) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
      VkResult gen75_GetMemoryAndroidHardwareBufferANDROID(VkDevice device, const VkMemoryGetAndroidHardwareBufferInfoANDROID* pInfo, struct AHardwareBuffer** pBuffer) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_ANDROID_KHR
      void gen75_CmdDrawIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) __attribute__ ((weak));
      void gen75_CmdDrawIndexedIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) __attribute__ ((weak));
      void gen75_CmdBindTransformFeedbackBuffersEXT(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes) __attribute__ ((weak));
      void gen75_CmdBeginTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets) __attribute__ ((weak));
      void gen75_CmdEndTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets) __attribute__ ((weak));
      void gen75_CmdBeginQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags, uint32_t index) __attribute__ ((weak));
      void gen75_CmdEndQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, uint32_t index) __attribute__ ((weak));
      void gen75_CmdDrawIndirectByteCountEXT(VkCommandBuffer commandBuffer, uint32_t instanceCount, uint32_t firstInstance, VkBuffer counterBuffer, VkDeviceSize counterBufferOffset, uint32_t counterOffset, uint32_t vertexStride) __attribute__ ((weak));
      VkDeviceAddress gen75_GetBufferDeviceAddressEXT(VkDevice device, const VkBufferDeviceAddressInfoEXT* pInfo) __attribute__ ((weak));
      VkResult gen75_GetPipelineExecutablePropertiesKHR(VkDevice                        device, const VkPipelineInfoKHR*        pPipelineInfo, uint32_t* pExecutableCount, VkPipelineExecutablePropertiesKHR* pProperties) __attribute__ ((weak));
      VkResult gen75_GetPipelineExecutableStatisticsKHR(VkDevice                        device, const VkPipelineExecutableInfoKHR*  pExecutableInfo, uint32_t* pStatisticCount, VkPipelineExecutableStatisticKHR* pStatistics) __attribute__ ((weak));
      VkResult gen75_GetPipelineExecutableInternalRepresentationsKHR(VkDevice                        device, const VkPipelineExecutableInfoKHR*  pExecutableInfo, uint32_t* pInternalRepresentationCount, VkPipelineExecutableInternalRepresentationKHR* pInternalRepresentations) __attribute__ ((weak));
      void gen75_CmdSetLineStippleEXT(VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern) __attribute__ ((weak));
      VkResult gen75_CreateDmaBufImageINTEL(VkDevice device, const VkDmaBufImageCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMem, VkImage* pImage) __attribute__ ((weak));

  const struct anv_device_dispatch_table gen75_device_dispatch_table = {
    .vkGetDeviceProcAddr = gen75_GetDeviceProcAddr,
    .vkDestroyDevice = gen75_DestroyDevice,
    .vkGetDeviceQueue = gen75_GetDeviceQueue,
    .vkQueueSubmit = gen75_QueueSubmit,
    .vkQueueWaitIdle = gen75_QueueWaitIdle,
    .vkDeviceWaitIdle = gen75_DeviceWaitIdle,
    .vkAllocateMemory = gen75_AllocateMemory,
    .vkFreeMemory = gen75_FreeMemory,
    .vkMapMemory = gen75_MapMemory,
    .vkUnmapMemory = gen75_UnmapMemory,
    .vkFlushMappedMemoryRanges = gen75_FlushMappedMemoryRanges,
    .vkInvalidateMappedMemoryRanges = gen75_InvalidateMappedMemoryRanges,
    .vkGetDeviceMemoryCommitment = gen75_GetDeviceMemoryCommitment,
    .vkGetBufferMemoryRequirements = gen75_GetBufferMemoryRequirements,
    .vkBindBufferMemory = gen75_BindBufferMemory,
    .vkGetImageMemoryRequirements = gen75_GetImageMemoryRequirements,
    .vkBindImageMemory = gen75_BindImageMemory,
    .vkGetImageSparseMemoryRequirements = gen75_GetImageSparseMemoryRequirements,
    .vkQueueBindSparse = gen75_QueueBindSparse,
    .vkCreateFence = gen75_CreateFence,
    .vkDestroyFence = gen75_DestroyFence,
    .vkResetFences = gen75_ResetFences,
    .vkGetFenceStatus = gen75_GetFenceStatus,
    .vkWaitForFences = gen75_WaitForFences,
    .vkCreateSemaphore = gen75_CreateSemaphore,
    .vkDestroySemaphore = gen75_DestroySemaphore,
    .vkCreateEvent = gen75_CreateEvent,
    .vkDestroyEvent = gen75_DestroyEvent,
    .vkGetEventStatus = gen75_GetEventStatus,
    .vkSetEvent = gen75_SetEvent,
    .vkResetEvent = gen75_ResetEvent,
    .vkCreateQueryPool = gen75_CreateQueryPool,
    .vkDestroyQueryPool = gen75_DestroyQueryPool,
    .vkGetQueryPoolResults = gen75_GetQueryPoolResults,
    .vkResetQueryPoolEXT = gen75_ResetQueryPoolEXT,
    .vkCreateBuffer = gen75_CreateBuffer,
    .vkDestroyBuffer = gen75_DestroyBuffer,
    .vkCreateBufferView = gen75_CreateBufferView,
    .vkDestroyBufferView = gen75_DestroyBufferView,
    .vkCreateImage = gen75_CreateImage,
    .vkDestroyImage = gen75_DestroyImage,
    .vkGetImageSubresourceLayout = gen75_GetImageSubresourceLayout,
    .vkCreateImageView = gen75_CreateImageView,
    .vkDestroyImageView = gen75_DestroyImageView,
    .vkCreateShaderModule = gen75_CreateShaderModule,
    .vkDestroyShaderModule = gen75_DestroyShaderModule,
    .vkCreatePipelineCache = gen75_CreatePipelineCache,
    .vkDestroyPipelineCache = gen75_DestroyPipelineCache,
    .vkGetPipelineCacheData = gen75_GetPipelineCacheData,
    .vkMergePipelineCaches = gen75_MergePipelineCaches,
    .vkCreateGraphicsPipelines = gen75_CreateGraphicsPipelines,
    .vkCreateComputePipelines = gen75_CreateComputePipelines,
    .vkDestroyPipeline = gen75_DestroyPipeline,
    .vkCreatePipelineLayout = gen75_CreatePipelineLayout,
    .vkDestroyPipelineLayout = gen75_DestroyPipelineLayout,
    .vkCreateSampler = gen75_CreateSampler,
    .vkDestroySampler = gen75_DestroySampler,
    .vkCreateDescriptorSetLayout = gen75_CreateDescriptorSetLayout,
    .vkDestroyDescriptorSetLayout = gen75_DestroyDescriptorSetLayout,
    .vkCreateDescriptorPool = gen75_CreateDescriptorPool,
    .vkDestroyDescriptorPool = gen75_DestroyDescriptorPool,
    .vkResetDescriptorPool = gen75_ResetDescriptorPool,
    .vkAllocateDescriptorSets = gen75_AllocateDescriptorSets,
    .vkFreeDescriptorSets = gen75_FreeDescriptorSets,
    .vkUpdateDescriptorSets = gen75_UpdateDescriptorSets,
    .vkCreateFramebuffer = gen75_CreateFramebuffer,
    .vkDestroyFramebuffer = gen75_DestroyFramebuffer,
    .vkCreateRenderPass = gen75_CreateRenderPass,
    .vkDestroyRenderPass = gen75_DestroyRenderPass,
    .vkGetRenderAreaGranularity = gen75_GetRenderAreaGranularity,
    .vkCreateCommandPool = gen75_CreateCommandPool,
    .vkDestroyCommandPool = gen75_DestroyCommandPool,
    .vkResetCommandPool = gen75_ResetCommandPool,
    .vkAllocateCommandBuffers = gen75_AllocateCommandBuffers,
    .vkFreeCommandBuffers = gen75_FreeCommandBuffers,
    .vkBeginCommandBuffer = gen75_BeginCommandBuffer,
    .vkEndCommandBuffer = gen75_EndCommandBuffer,
    .vkResetCommandBuffer = gen75_ResetCommandBuffer,
    .vkCmdBindPipeline = gen75_CmdBindPipeline,
    .vkCmdSetViewport = gen75_CmdSetViewport,
    .vkCmdSetScissor = gen75_CmdSetScissor,
    .vkCmdSetLineWidth = gen75_CmdSetLineWidth,
    .vkCmdSetDepthBias = gen75_CmdSetDepthBias,
    .vkCmdSetBlendConstants = gen75_CmdSetBlendConstants,
    .vkCmdSetDepthBounds = gen75_CmdSetDepthBounds,
    .vkCmdSetStencilCompareMask = gen75_CmdSetStencilCompareMask,
    .vkCmdSetStencilWriteMask = gen75_CmdSetStencilWriteMask,
    .vkCmdSetStencilReference = gen75_CmdSetStencilReference,
    .vkCmdBindDescriptorSets = gen75_CmdBindDescriptorSets,
    .vkCmdBindIndexBuffer = gen75_CmdBindIndexBuffer,
    .vkCmdBindVertexBuffers = gen75_CmdBindVertexBuffers,
    .vkCmdDraw = gen75_CmdDraw,
    .vkCmdDrawIndexed = gen75_CmdDrawIndexed,
    .vkCmdDrawIndirect = gen75_CmdDrawIndirect,
    .vkCmdDrawIndexedIndirect = gen75_CmdDrawIndexedIndirect,
    .vkCmdDispatch = gen75_CmdDispatch,
    .vkCmdDispatchIndirect = gen75_CmdDispatchIndirect,
    .vkCmdCopyBuffer = gen75_CmdCopyBuffer,
    .vkCmdCopyImage = gen75_CmdCopyImage,
    .vkCmdBlitImage = gen75_CmdBlitImage,
    .vkCmdCopyBufferToImage = gen75_CmdCopyBufferToImage,
    .vkCmdCopyImageToBuffer = gen75_CmdCopyImageToBuffer,
    .vkCmdUpdateBuffer = gen75_CmdUpdateBuffer,
    .vkCmdFillBuffer = gen75_CmdFillBuffer,
    .vkCmdClearColorImage = gen75_CmdClearColorImage,
    .vkCmdClearDepthStencilImage = gen75_CmdClearDepthStencilImage,
    .vkCmdClearAttachments = gen75_CmdClearAttachments,
    .vkCmdResolveImage = gen75_CmdResolveImage,
    .vkCmdSetEvent = gen75_CmdSetEvent,
    .vkCmdResetEvent = gen75_CmdResetEvent,
    .vkCmdWaitEvents = gen75_CmdWaitEvents,
    .vkCmdPipelineBarrier = gen75_CmdPipelineBarrier,
    .vkCmdBeginQuery = gen75_CmdBeginQuery,
    .vkCmdEndQuery = gen75_CmdEndQuery,
    .vkCmdBeginConditionalRenderingEXT = gen75_CmdBeginConditionalRenderingEXT,
    .vkCmdEndConditionalRenderingEXT = gen75_CmdEndConditionalRenderingEXT,
    .vkCmdResetQueryPool = gen75_CmdResetQueryPool,
    .vkCmdWriteTimestamp = gen75_CmdWriteTimestamp,
    .vkCmdCopyQueryPoolResults = gen75_CmdCopyQueryPoolResults,
    .vkCmdPushConstants = gen75_CmdPushConstants,
    .vkCmdBeginRenderPass = gen75_CmdBeginRenderPass,
    .vkCmdNextSubpass = gen75_CmdNextSubpass,
    .vkCmdEndRenderPass = gen75_CmdEndRenderPass,
    .vkCmdExecuteCommands = gen75_CmdExecuteCommands,
    .vkCreateSwapchainKHR = gen75_CreateSwapchainKHR,
    .vkDestroySwapchainKHR = gen75_DestroySwapchainKHR,
    .vkGetSwapchainImagesKHR = gen75_GetSwapchainImagesKHR,
    .vkAcquireNextImageKHR = gen75_AcquireNextImageKHR,
    .vkQueuePresentKHR = gen75_QueuePresentKHR,
    .vkCmdPushDescriptorSetKHR = gen75_CmdPushDescriptorSetKHR,
    .vkTrimCommandPool = gen75_TrimCommandPool,
    .vkTrimCommandPoolKHR = gen75_TrimCommandPool,
    .vkGetMemoryFdKHR = gen75_GetMemoryFdKHR,
    .vkGetMemoryFdPropertiesKHR = gen75_GetMemoryFdPropertiesKHR,
    .vkGetSemaphoreFdKHR = gen75_GetSemaphoreFdKHR,
    .vkImportSemaphoreFdKHR = gen75_ImportSemaphoreFdKHR,
    .vkGetFenceFdKHR = gen75_GetFenceFdKHR,
    .vkImportFenceFdKHR = gen75_ImportFenceFdKHR,
    .vkDisplayPowerControlEXT = gen75_DisplayPowerControlEXT,
    .vkRegisterDeviceEventEXT = gen75_RegisterDeviceEventEXT,
    .vkRegisterDisplayEventEXT = gen75_RegisterDisplayEventEXT,
    .vkGetSwapchainCounterEXT = gen75_GetSwapchainCounterEXT,
    .vkGetDeviceGroupPeerMemoryFeatures = gen75_GetDeviceGroupPeerMemoryFeatures,
    .vkGetDeviceGroupPeerMemoryFeaturesKHR = gen75_GetDeviceGroupPeerMemoryFeatures,
    .vkBindBufferMemory2 = gen75_BindBufferMemory2,
    .vkBindBufferMemory2KHR = gen75_BindBufferMemory2,
    .vkBindImageMemory2 = gen75_BindImageMemory2,
    .vkBindImageMemory2KHR = gen75_BindImageMemory2,
    .vkCmdSetDeviceMask = gen75_CmdSetDeviceMask,
    .vkCmdSetDeviceMaskKHR = gen75_CmdSetDeviceMask,
    .vkGetDeviceGroupPresentCapabilitiesKHR = gen75_GetDeviceGroupPresentCapabilitiesKHR,
    .vkGetDeviceGroupSurfacePresentModesKHR = gen75_GetDeviceGroupSurfacePresentModesKHR,
    .vkAcquireNextImage2KHR = gen75_AcquireNextImage2KHR,
    .vkCmdDispatchBase = gen75_CmdDispatchBase,
    .vkCmdDispatchBaseKHR = gen75_CmdDispatchBase,
    .vkCreateDescriptorUpdateTemplate = gen75_CreateDescriptorUpdateTemplate,
    .vkCreateDescriptorUpdateTemplateKHR = gen75_CreateDescriptorUpdateTemplate,
    .vkDestroyDescriptorUpdateTemplate = gen75_DestroyDescriptorUpdateTemplate,
    .vkDestroyDescriptorUpdateTemplateKHR = gen75_DestroyDescriptorUpdateTemplate,
    .vkUpdateDescriptorSetWithTemplate = gen75_UpdateDescriptorSetWithTemplate,
    .vkUpdateDescriptorSetWithTemplateKHR = gen75_UpdateDescriptorSetWithTemplate,
    .vkCmdPushDescriptorSetWithTemplateKHR = gen75_CmdPushDescriptorSetWithTemplateKHR,
    .vkGetBufferMemoryRequirements2 = gen75_GetBufferMemoryRequirements2,
    .vkGetBufferMemoryRequirements2KHR = gen75_GetBufferMemoryRequirements2,
    .vkGetImageMemoryRequirements2 = gen75_GetImageMemoryRequirements2,
    .vkGetImageMemoryRequirements2KHR = gen75_GetImageMemoryRequirements2,
    .vkGetImageSparseMemoryRequirements2 = gen75_GetImageSparseMemoryRequirements2,
    .vkGetImageSparseMemoryRequirements2KHR = gen75_GetImageSparseMemoryRequirements2,
    .vkCreateSamplerYcbcrConversion = gen75_CreateSamplerYcbcrConversion,
    .vkCreateSamplerYcbcrConversionKHR = gen75_CreateSamplerYcbcrConversion,
    .vkDestroySamplerYcbcrConversion = gen75_DestroySamplerYcbcrConversion,
    .vkDestroySamplerYcbcrConversionKHR = gen75_DestroySamplerYcbcrConversion,
    .vkGetDeviceQueue2 = gen75_GetDeviceQueue2,
    .vkGetDescriptorSetLayoutSupport = gen75_GetDescriptorSetLayoutSupport,
    .vkGetDescriptorSetLayoutSupportKHR = gen75_GetDescriptorSetLayoutSupport,
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .vkGetSwapchainGrallocUsageANDROID = gen75_GetSwapchainGrallocUsageANDROID,
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .vkGetSwapchainGrallocUsage2ANDROID = gen75_GetSwapchainGrallocUsage2ANDROID,
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .vkAcquireImageANDROID = gen75_AcquireImageANDROID,
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .vkQueueSignalReleaseImageANDROID = gen75_QueueSignalReleaseImageANDROID,
#endif // VK_USE_PLATFORM_ANDROID_KHR
    .vkGetCalibratedTimestampsEXT = gen75_GetCalibratedTimestampsEXT,
    .vkGetMemoryHostPointerPropertiesEXT = gen75_GetMemoryHostPointerPropertiesEXT,
    .vkCreateRenderPass2KHR = gen75_CreateRenderPass2KHR,
    .vkCmdBeginRenderPass2KHR = gen75_CmdBeginRenderPass2KHR,
    .vkCmdNextSubpass2KHR = gen75_CmdNextSubpass2KHR,
    .vkCmdEndRenderPass2KHR = gen75_CmdEndRenderPass2KHR,
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .vkGetAndroidHardwareBufferPropertiesANDROID = gen75_GetAndroidHardwareBufferPropertiesANDROID,
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .vkGetMemoryAndroidHardwareBufferANDROID = gen75_GetMemoryAndroidHardwareBufferANDROID,
#endif // VK_USE_PLATFORM_ANDROID_KHR
    .vkCmdDrawIndirectCountKHR = gen75_CmdDrawIndirectCountKHR,
    .vkCmdDrawIndexedIndirectCountKHR = gen75_CmdDrawIndexedIndirectCountKHR,
    .vkCmdBindTransformFeedbackBuffersEXT = gen75_CmdBindTransformFeedbackBuffersEXT,
    .vkCmdBeginTransformFeedbackEXT = gen75_CmdBeginTransformFeedbackEXT,
    .vkCmdEndTransformFeedbackEXT = gen75_CmdEndTransformFeedbackEXT,
    .vkCmdBeginQueryIndexedEXT = gen75_CmdBeginQueryIndexedEXT,
    .vkCmdEndQueryIndexedEXT = gen75_CmdEndQueryIndexedEXT,
    .vkCmdDrawIndirectByteCountEXT = gen75_CmdDrawIndirectByteCountEXT,
    .vkGetBufferDeviceAddressEXT = gen75_GetBufferDeviceAddressEXT,
    .vkGetPipelineExecutablePropertiesKHR = gen75_GetPipelineExecutablePropertiesKHR,
    .vkGetPipelineExecutableStatisticsKHR = gen75_GetPipelineExecutableStatisticsKHR,
    .vkGetPipelineExecutableInternalRepresentationsKHR = gen75_GetPipelineExecutableInternalRepresentationsKHR,
    .vkCmdSetLineStippleEXT = gen75_CmdSetLineStippleEXT,
    .vkCreateDmaBufImageINTEL = gen75_CreateDmaBufImageINTEL,
  };
      PFN_vkVoidFunction gen8_GetDeviceProcAddr(VkDevice device, const char* pName) __attribute__ ((weak));
      void gen8_DestroyDevice(VkDevice device, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      void gen8_GetDeviceQueue(VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue) __attribute__ ((weak));
      VkResult gen8_QueueSubmit(VkQueue queue, uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence) __attribute__ ((weak));
      VkResult gen8_QueueWaitIdle(VkQueue queue) __attribute__ ((weak));
      VkResult gen8_DeviceWaitIdle(VkDevice device) __attribute__ ((weak));
      VkResult gen8_AllocateMemory(VkDevice device, const VkMemoryAllocateInfo* pAllocateInfo, const VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory) __attribute__ ((weak));
      void gen8_FreeMemory(VkDevice device, VkDeviceMemory memory, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen8_MapMemory(VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData) __attribute__ ((weak));
      void gen8_UnmapMemory(VkDevice device, VkDeviceMemory memory) __attribute__ ((weak));
      VkResult gen8_FlushMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges) __attribute__ ((weak));
      VkResult gen8_InvalidateMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges) __attribute__ ((weak));
      void gen8_GetDeviceMemoryCommitment(VkDevice device, VkDeviceMemory memory, VkDeviceSize* pCommittedMemoryInBytes) __attribute__ ((weak));
      void gen8_GetBufferMemoryRequirements(VkDevice device, VkBuffer buffer, VkMemoryRequirements* pMemoryRequirements) __attribute__ ((weak));
      VkResult gen8_BindBufferMemory(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset) __attribute__ ((weak));
      void gen8_GetImageMemoryRequirements(VkDevice device, VkImage image, VkMemoryRequirements* pMemoryRequirements) __attribute__ ((weak));
      VkResult gen8_BindImageMemory(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset) __attribute__ ((weak));
      void gen8_GetImageSparseMemoryRequirements(VkDevice device, VkImage image, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements* pSparseMemoryRequirements) __attribute__ ((weak));
      VkResult gen8_QueueBindSparse(VkQueue queue, uint32_t bindInfoCount, const VkBindSparseInfo* pBindInfo, VkFence fence) __attribute__ ((weak));
      VkResult gen8_CreateFence(VkDevice device, const VkFenceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) __attribute__ ((weak));
      void gen8_DestroyFence(VkDevice device, VkFence fence, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen8_ResetFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences) __attribute__ ((weak));
      VkResult gen8_GetFenceStatus(VkDevice device, VkFence fence) __attribute__ ((weak));
      VkResult gen8_WaitForFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences, VkBool32 waitAll, uint64_t timeout) __attribute__ ((weak));
      VkResult gen8_CreateSemaphore(VkDevice device, const VkSemaphoreCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore) __attribute__ ((weak));
      void gen8_DestroySemaphore(VkDevice device, VkSemaphore semaphore, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen8_CreateEvent(VkDevice device, const VkEventCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkEvent* pEvent) __attribute__ ((weak));
      void gen8_DestroyEvent(VkDevice device, VkEvent event, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen8_GetEventStatus(VkDevice device, VkEvent event) __attribute__ ((weak));
      VkResult gen8_SetEvent(VkDevice device, VkEvent event) __attribute__ ((weak));
      VkResult gen8_ResetEvent(VkDevice device, VkEvent event) __attribute__ ((weak));
      VkResult gen8_CreateQueryPool(VkDevice device, const VkQueryPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkQueryPool* pQueryPool) __attribute__ ((weak));
      void gen8_DestroyQueryPool(VkDevice device, VkQueryPool queryPool, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen8_GetQueryPoolResults(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, VkDeviceSize stride, VkQueryResultFlags flags) __attribute__ ((weak));
      void gen8_ResetQueryPoolEXT(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) __attribute__ ((weak));
      VkResult gen8_CreateBuffer(VkDevice device, const VkBufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer) __attribute__ ((weak));
      void gen8_DestroyBuffer(VkDevice device, VkBuffer buffer, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen8_CreateBufferView(VkDevice device, const VkBufferViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferView* pView) __attribute__ ((weak));
      void gen8_DestroyBufferView(VkDevice device, VkBufferView bufferView, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen8_CreateImage(VkDevice device, const VkImageCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImage* pImage) __attribute__ ((weak));
      void gen8_DestroyImage(VkDevice device, VkImage image, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      void gen8_GetImageSubresourceLayout(VkDevice device, VkImage image, const VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout) __attribute__ ((weak));
      VkResult gen8_CreateImageView(VkDevice device, const VkImageViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImageView* pView) __attribute__ ((weak));
      void gen8_DestroyImageView(VkDevice device, VkImageView imageView, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen8_CreateShaderModule(VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule) __attribute__ ((weak));
      void gen8_DestroyShaderModule(VkDevice device, VkShaderModule shaderModule, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen8_CreatePipelineCache(VkDevice device, const VkPipelineCacheCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineCache* pPipelineCache) __attribute__ ((weak));
      void gen8_DestroyPipelineCache(VkDevice device, VkPipelineCache pipelineCache, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen8_GetPipelineCacheData(VkDevice device, VkPipelineCache pipelineCache, size_t* pDataSize, void* pData) __attribute__ ((weak));
      VkResult gen8_MergePipelineCaches(VkDevice device, VkPipelineCache dstCache, uint32_t srcCacheCount, const VkPipelineCache* pSrcCaches) __attribute__ ((weak));
      VkResult gen8_CreateGraphicsPipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkGraphicsPipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) __attribute__ ((weak));
      VkResult gen8_CreateComputePipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkComputePipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) __attribute__ ((weak));
      void gen8_DestroyPipeline(VkDevice device, VkPipeline pipeline, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen8_CreatePipelineLayout(VkDevice device, const VkPipelineLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout) __attribute__ ((weak));
      void gen8_DestroyPipelineLayout(VkDevice device, VkPipelineLayout pipelineLayout, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen8_CreateSampler(VkDevice device, const VkSamplerCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSampler* pSampler) __attribute__ ((weak));
      void gen8_DestroySampler(VkDevice device, VkSampler sampler, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen8_CreateDescriptorSetLayout(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorSetLayout* pSetLayout) __attribute__ ((weak));
      void gen8_DestroyDescriptorSetLayout(VkDevice device, VkDescriptorSetLayout descriptorSetLayout, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen8_CreateDescriptorPool(VkDevice device, const VkDescriptorPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorPool* pDescriptorPool) __attribute__ ((weak));
      void gen8_DestroyDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen8_ResetDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags flags) __attribute__ ((weak));
      VkResult gen8_AllocateDescriptorSets(VkDevice device, const VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets) __attribute__ ((weak));
      VkResult gen8_FreeDescriptorSets(VkDevice device, VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets) __attribute__ ((weak));
      void gen8_UpdateDescriptorSets(VkDevice device, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, const VkCopyDescriptorSet* pDescriptorCopies) __attribute__ ((weak));
      VkResult gen8_CreateFramebuffer(VkDevice device, const VkFramebufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFramebuffer* pFramebuffer) __attribute__ ((weak));
      void gen8_DestroyFramebuffer(VkDevice device, VkFramebuffer framebuffer, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen8_CreateRenderPass(VkDevice device, const VkRenderPassCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) __attribute__ ((weak));
      void gen8_DestroyRenderPass(VkDevice device, VkRenderPass renderPass, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      void gen8_GetRenderAreaGranularity(VkDevice device, VkRenderPass renderPass, VkExtent2D* pGranularity) __attribute__ ((weak));
      VkResult gen8_CreateCommandPool(VkDevice device, const VkCommandPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool) __attribute__ ((weak));
      void gen8_DestroyCommandPool(VkDevice device, VkCommandPool commandPool, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen8_ResetCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolResetFlags flags) __attribute__ ((weak));
      VkResult gen8_AllocateCommandBuffers(VkDevice device, const VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers) __attribute__ ((weak));
      void gen8_FreeCommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) __attribute__ ((weak));
      VkResult gen8_BeginCommandBuffer(VkCommandBuffer commandBuffer, const VkCommandBufferBeginInfo* pBeginInfo) __attribute__ ((weak));
      VkResult gen8_EndCommandBuffer(VkCommandBuffer commandBuffer) __attribute__ ((weak));
      VkResult gen8_ResetCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags) __attribute__ ((weak));
      void gen8_CmdBindPipeline(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline) __attribute__ ((weak));
      void gen8_CmdSetViewport(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pViewports) __attribute__ ((weak));
      void gen8_CmdSetScissor(VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pScissors) __attribute__ ((weak));
      void gen8_CmdSetLineWidth(VkCommandBuffer commandBuffer, float lineWidth) __attribute__ ((weak));
      void gen8_CmdSetDepthBias(VkCommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor) __attribute__ ((weak));
      void gen8_CmdSetBlendConstants(VkCommandBuffer commandBuffer, const float blendConstants[4]) __attribute__ ((weak));
      void gen8_CmdSetDepthBounds(VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds) __attribute__ ((weak));
      void gen8_CmdSetStencilCompareMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask) __attribute__ ((weak));
      void gen8_CmdSetStencilWriteMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask) __attribute__ ((weak));
      void gen8_CmdSetStencilReference(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference) __attribute__ ((weak));
      void gen8_CmdBindDescriptorSets(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets) __attribute__ ((weak));
      void gen8_CmdBindIndexBuffer(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType) __attribute__ ((weak));
      void gen8_CmdBindVertexBuffers(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets) __attribute__ ((weak));
      void gen8_CmdDraw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) __attribute__ ((weak));
      void gen8_CmdDrawIndexed(VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) __attribute__ ((weak));
      void gen8_CmdDrawIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) __attribute__ ((weak));
      void gen8_CmdDrawIndexedIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) __attribute__ ((weak));
      void gen8_CmdDispatch(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) __attribute__ ((weak));
      void gen8_CmdDispatchIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset) __attribute__ ((weak));
      void gen8_CmdCopyBuffer(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions) __attribute__ ((weak));
      void gen8_CmdCopyImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions) __attribute__ ((weak));
      void gen8_CmdBlitImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter) __attribute__ ((weak));
      void gen8_CmdCopyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions) __attribute__ ((weak));
      void gen8_CmdCopyImageToBuffer(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions) __attribute__ ((weak));
      void gen8_CmdUpdateBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData) __attribute__ ((weak));
      void gen8_CmdFillBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data) __attribute__ ((weak));
      void gen8_CmdClearColorImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) __attribute__ ((weak));
      void gen8_CmdClearDepthStencilImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) __attribute__ ((weak));
      void gen8_CmdClearAttachments(VkCommandBuffer commandBuffer, uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects) __attribute__ ((weak));
      void gen8_CmdResolveImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageResolve* pRegions) __attribute__ ((weak));
      void gen8_CmdSetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) __attribute__ ((weak));
      void gen8_CmdResetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) __attribute__ ((weak));
      void gen8_CmdWaitEvents(VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) __attribute__ ((weak));
      void gen8_CmdPipelineBarrier(VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) __attribute__ ((weak));
      void gen8_CmdBeginQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags) __attribute__ ((weak));
      void gen8_CmdEndQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query) __attribute__ ((weak));
      void gen8_CmdBeginConditionalRenderingEXT(VkCommandBuffer commandBuffer, const VkConditionalRenderingBeginInfoEXT* pConditionalRenderingBegin) __attribute__ ((weak));
      void gen8_CmdEndConditionalRenderingEXT(VkCommandBuffer commandBuffer) __attribute__ ((weak));
      void gen8_CmdResetQueryPool(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) __attribute__ ((weak));
      void gen8_CmdWriteTimestamp(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query) __attribute__ ((weak));
      void gen8_CmdCopyQueryPoolResults(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags) __attribute__ ((weak));
      void gen8_CmdPushConstants(VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues) __attribute__ ((weak));
      void gen8_CmdBeginRenderPass(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents) __attribute__ ((weak));
      void gen8_CmdNextSubpass(VkCommandBuffer commandBuffer, VkSubpassContents contents) __attribute__ ((weak));
      void gen8_CmdEndRenderPass(VkCommandBuffer commandBuffer) __attribute__ ((weak));
      void gen8_CmdExecuteCommands(VkCommandBuffer commandBuffer, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) __attribute__ ((weak));
      VkResult gen8_CreateSwapchainKHR(VkDevice device, const VkSwapchainCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain) __attribute__ ((weak));
      void gen8_DestroySwapchainKHR(VkDevice device, VkSwapchainKHR swapchain, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen8_GetSwapchainImagesKHR(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages) __attribute__ ((weak));
      VkResult gen8_AcquireNextImageKHR(VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex) __attribute__ ((weak));
      VkResult gen8_QueuePresentKHR(VkQueue queue, const VkPresentInfoKHR* pPresentInfo) __attribute__ ((weak));
      void gen8_CmdPushDescriptorSetKHR(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites) __attribute__ ((weak));
      void gen8_TrimCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags) __attribute__ ((weak));
            VkResult gen8_GetMemoryFdKHR(VkDevice device, const VkMemoryGetFdInfoKHR* pGetFdInfo, int* pFd) __attribute__ ((weak));
      VkResult gen8_GetMemoryFdPropertiesKHR(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, int fd, VkMemoryFdPropertiesKHR* pMemoryFdProperties) __attribute__ ((weak));
      VkResult gen8_GetSemaphoreFdKHR(VkDevice device, const VkSemaphoreGetFdInfoKHR* pGetFdInfo, int* pFd) __attribute__ ((weak));
      VkResult gen8_ImportSemaphoreFdKHR(VkDevice device, const VkImportSemaphoreFdInfoKHR* pImportSemaphoreFdInfo) __attribute__ ((weak));
      VkResult gen8_GetFenceFdKHR(VkDevice device, const VkFenceGetFdInfoKHR* pGetFdInfo, int* pFd) __attribute__ ((weak));
      VkResult gen8_ImportFenceFdKHR(VkDevice device, const VkImportFenceFdInfoKHR* pImportFenceFdInfo) __attribute__ ((weak));
      VkResult gen8_DisplayPowerControlEXT(VkDevice device, VkDisplayKHR display, const VkDisplayPowerInfoEXT* pDisplayPowerInfo) __attribute__ ((weak));
      VkResult gen8_RegisterDeviceEventEXT(VkDevice device, const VkDeviceEventInfoEXT* pDeviceEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) __attribute__ ((weak));
      VkResult gen8_RegisterDisplayEventEXT(VkDevice device, VkDisplayKHR display, const VkDisplayEventInfoEXT* pDisplayEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) __attribute__ ((weak));
      VkResult gen8_GetSwapchainCounterEXT(VkDevice device, VkSwapchainKHR swapchain, VkSurfaceCounterFlagBitsEXT counter, uint64_t* pCounterValue) __attribute__ ((weak));
      void gen8_GetDeviceGroupPeerMemoryFeatures(VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures) __attribute__ ((weak));
            VkResult gen8_BindBufferMemory2(VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos) __attribute__ ((weak));
            VkResult gen8_BindImageMemory2(VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos) __attribute__ ((weak));
            void gen8_CmdSetDeviceMask(VkCommandBuffer commandBuffer, uint32_t deviceMask) __attribute__ ((weak));
            VkResult gen8_GetDeviceGroupPresentCapabilitiesKHR(VkDevice device, VkDeviceGroupPresentCapabilitiesKHR* pDeviceGroupPresentCapabilities) __attribute__ ((weak));
      VkResult gen8_GetDeviceGroupSurfacePresentModesKHR(VkDevice device, VkSurfaceKHR surface, VkDeviceGroupPresentModeFlagsKHR* pModes) __attribute__ ((weak));
      VkResult gen8_AcquireNextImage2KHR(VkDevice device, const VkAcquireNextImageInfoKHR* pAcquireInfo, uint32_t* pImageIndex) __attribute__ ((weak));
      void gen8_CmdDispatchBase(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) __attribute__ ((weak));
            VkResult gen8_CreateDescriptorUpdateTemplate(VkDevice device, const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate) __attribute__ ((weak));
            void gen8_DestroyDescriptorUpdateTemplate(VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
            void gen8_UpdateDescriptorSetWithTemplate(VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData) __attribute__ ((weak));
            void gen8_CmdPushDescriptorSetWithTemplateKHR(VkCommandBuffer commandBuffer, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, const void* pData) __attribute__ ((weak));
      void gen8_GetBufferMemoryRequirements2(VkDevice device, const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) __attribute__ ((weak));
            void gen8_GetImageMemoryRequirements2(VkDevice device, const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) __attribute__ ((weak));
            void gen8_GetImageSparseMemoryRequirements2(VkDevice device, const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) __attribute__ ((weak));
            VkResult gen8_CreateSamplerYcbcrConversion(VkDevice device, const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion) __attribute__ ((weak));
            void gen8_DestroySamplerYcbcrConversion(VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
            void gen8_GetDeviceQueue2(VkDevice device, const VkDeviceQueueInfo2* pQueueInfo, VkQueue* pQueue) __attribute__ ((weak));
      void gen8_GetDescriptorSetLayoutSupport(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport) __attribute__ ((weak));
      #ifdef VK_USE_PLATFORM_ANDROID_KHR
      VkResult gen8_GetSwapchainGrallocUsageANDROID(VkDevice device, VkFormat format, VkImageUsageFlags imageUsage, int* grallocUsage) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
      VkResult gen8_GetSwapchainGrallocUsage2ANDROID(VkDevice device, VkFormat format, VkImageUsageFlags imageUsage, VkSwapchainImageUsageFlagsANDROID swapchainImageUsage, uint64_t* grallocConsumerUsage, uint64_t* grallocProducerUsage) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
      VkResult gen8_AcquireImageANDROID(VkDevice device, VkImage image, int nativeFenceFd, VkSemaphore semaphore, VkFence fence) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
      VkResult gen8_QueueSignalReleaseImageANDROID(VkQueue queue, uint32_t waitSemaphoreCount, const VkSemaphore* pWaitSemaphores, VkImage image, int* pNativeFenceFd) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_ANDROID_KHR
      VkResult gen8_GetCalibratedTimestampsEXT(VkDevice device, uint32_t timestampCount, const VkCalibratedTimestampInfoEXT* pTimestampInfos, uint64_t* pTimestamps, uint64_t* pMaxDeviation) __attribute__ ((weak));
      VkResult gen8_GetMemoryHostPointerPropertiesEXT(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, const void* pHostPointer, VkMemoryHostPointerPropertiesEXT* pMemoryHostPointerProperties) __attribute__ ((weak));
      VkResult gen8_CreateRenderPass2KHR(VkDevice device, const VkRenderPassCreateInfo2KHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) __attribute__ ((weak));
      void gen8_CmdBeginRenderPass2KHR(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo*      pRenderPassBegin, const VkSubpassBeginInfoKHR*      pSubpassBeginInfo) __attribute__ ((weak));
      void gen8_CmdNextSubpass2KHR(VkCommandBuffer commandBuffer, const VkSubpassBeginInfoKHR*      pSubpassBeginInfo, const VkSubpassEndInfoKHR*        pSubpassEndInfo) __attribute__ ((weak));
      void gen8_CmdEndRenderPass2KHR(VkCommandBuffer commandBuffer, const VkSubpassEndInfoKHR*        pSubpassEndInfo) __attribute__ ((weak));
#ifdef VK_USE_PLATFORM_ANDROID_KHR
      VkResult gen8_GetAndroidHardwareBufferPropertiesANDROID(VkDevice device, const struct AHardwareBuffer* buffer, VkAndroidHardwareBufferPropertiesANDROID* pProperties) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
      VkResult gen8_GetMemoryAndroidHardwareBufferANDROID(VkDevice device, const VkMemoryGetAndroidHardwareBufferInfoANDROID* pInfo, struct AHardwareBuffer** pBuffer) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_ANDROID_KHR
      void gen8_CmdDrawIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) __attribute__ ((weak));
      void gen8_CmdDrawIndexedIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) __attribute__ ((weak));
      void gen8_CmdBindTransformFeedbackBuffersEXT(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes) __attribute__ ((weak));
      void gen8_CmdBeginTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets) __attribute__ ((weak));
      void gen8_CmdEndTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets) __attribute__ ((weak));
      void gen8_CmdBeginQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags, uint32_t index) __attribute__ ((weak));
      void gen8_CmdEndQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, uint32_t index) __attribute__ ((weak));
      void gen8_CmdDrawIndirectByteCountEXT(VkCommandBuffer commandBuffer, uint32_t instanceCount, uint32_t firstInstance, VkBuffer counterBuffer, VkDeviceSize counterBufferOffset, uint32_t counterOffset, uint32_t vertexStride) __attribute__ ((weak));
      VkDeviceAddress gen8_GetBufferDeviceAddressEXT(VkDevice device, const VkBufferDeviceAddressInfoEXT* pInfo) __attribute__ ((weak));
      VkResult gen8_GetPipelineExecutablePropertiesKHR(VkDevice                        device, const VkPipelineInfoKHR*        pPipelineInfo, uint32_t* pExecutableCount, VkPipelineExecutablePropertiesKHR* pProperties) __attribute__ ((weak));
      VkResult gen8_GetPipelineExecutableStatisticsKHR(VkDevice                        device, const VkPipelineExecutableInfoKHR*  pExecutableInfo, uint32_t* pStatisticCount, VkPipelineExecutableStatisticKHR* pStatistics) __attribute__ ((weak));
      VkResult gen8_GetPipelineExecutableInternalRepresentationsKHR(VkDevice                        device, const VkPipelineExecutableInfoKHR*  pExecutableInfo, uint32_t* pInternalRepresentationCount, VkPipelineExecutableInternalRepresentationKHR* pInternalRepresentations) __attribute__ ((weak));
      void gen8_CmdSetLineStippleEXT(VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern) __attribute__ ((weak));
      VkResult gen8_CreateDmaBufImageINTEL(VkDevice device, const VkDmaBufImageCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMem, VkImage* pImage) __attribute__ ((weak));

  const struct anv_device_dispatch_table gen8_device_dispatch_table = {
    .vkGetDeviceProcAddr = gen8_GetDeviceProcAddr,
    .vkDestroyDevice = gen8_DestroyDevice,
    .vkGetDeviceQueue = gen8_GetDeviceQueue,
    .vkQueueSubmit = gen8_QueueSubmit,
    .vkQueueWaitIdle = gen8_QueueWaitIdle,
    .vkDeviceWaitIdle = gen8_DeviceWaitIdle,
    .vkAllocateMemory = gen8_AllocateMemory,
    .vkFreeMemory = gen8_FreeMemory,
    .vkMapMemory = gen8_MapMemory,
    .vkUnmapMemory = gen8_UnmapMemory,
    .vkFlushMappedMemoryRanges = gen8_FlushMappedMemoryRanges,
    .vkInvalidateMappedMemoryRanges = gen8_InvalidateMappedMemoryRanges,
    .vkGetDeviceMemoryCommitment = gen8_GetDeviceMemoryCommitment,
    .vkGetBufferMemoryRequirements = gen8_GetBufferMemoryRequirements,
    .vkBindBufferMemory = gen8_BindBufferMemory,
    .vkGetImageMemoryRequirements = gen8_GetImageMemoryRequirements,
    .vkBindImageMemory = gen8_BindImageMemory,
    .vkGetImageSparseMemoryRequirements = gen8_GetImageSparseMemoryRequirements,
    .vkQueueBindSparse = gen8_QueueBindSparse,
    .vkCreateFence = gen8_CreateFence,
    .vkDestroyFence = gen8_DestroyFence,
    .vkResetFences = gen8_ResetFences,
    .vkGetFenceStatus = gen8_GetFenceStatus,
    .vkWaitForFences = gen8_WaitForFences,
    .vkCreateSemaphore = gen8_CreateSemaphore,
    .vkDestroySemaphore = gen8_DestroySemaphore,
    .vkCreateEvent = gen8_CreateEvent,
    .vkDestroyEvent = gen8_DestroyEvent,
    .vkGetEventStatus = gen8_GetEventStatus,
    .vkSetEvent = gen8_SetEvent,
    .vkResetEvent = gen8_ResetEvent,
    .vkCreateQueryPool = gen8_CreateQueryPool,
    .vkDestroyQueryPool = gen8_DestroyQueryPool,
    .vkGetQueryPoolResults = gen8_GetQueryPoolResults,
    .vkResetQueryPoolEXT = gen8_ResetQueryPoolEXT,
    .vkCreateBuffer = gen8_CreateBuffer,
    .vkDestroyBuffer = gen8_DestroyBuffer,
    .vkCreateBufferView = gen8_CreateBufferView,
    .vkDestroyBufferView = gen8_DestroyBufferView,
    .vkCreateImage = gen8_CreateImage,
    .vkDestroyImage = gen8_DestroyImage,
    .vkGetImageSubresourceLayout = gen8_GetImageSubresourceLayout,
    .vkCreateImageView = gen8_CreateImageView,
    .vkDestroyImageView = gen8_DestroyImageView,
    .vkCreateShaderModule = gen8_CreateShaderModule,
    .vkDestroyShaderModule = gen8_DestroyShaderModule,
    .vkCreatePipelineCache = gen8_CreatePipelineCache,
    .vkDestroyPipelineCache = gen8_DestroyPipelineCache,
    .vkGetPipelineCacheData = gen8_GetPipelineCacheData,
    .vkMergePipelineCaches = gen8_MergePipelineCaches,
    .vkCreateGraphicsPipelines = gen8_CreateGraphicsPipelines,
    .vkCreateComputePipelines = gen8_CreateComputePipelines,
    .vkDestroyPipeline = gen8_DestroyPipeline,
    .vkCreatePipelineLayout = gen8_CreatePipelineLayout,
    .vkDestroyPipelineLayout = gen8_DestroyPipelineLayout,
    .vkCreateSampler = gen8_CreateSampler,
    .vkDestroySampler = gen8_DestroySampler,
    .vkCreateDescriptorSetLayout = gen8_CreateDescriptorSetLayout,
    .vkDestroyDescriptorSetLayout = gen8_DestroyDescriptorSetLayout,
    .vkCreateDescriptorPool = gen8_CreateDescriptorPool,
    .vkDestroyDescriptorPool = gen8_DestroyDescriptorPool,
    .vkResetDescriptorPool = gen8_ResetDescriptorPool,
    .vkAllocateDescriptorSets = gen8_AllocateDescriptorSets,
    .vkFreeDescriptorSets = gen8_FreeDescriptorSets,
    .vkUpdateDescriptorSets = gen8_UpdateDescriptorSets,
    .vkCreateFramebuffer = gen8_CreateFramebuffer,
    .vkDestroyFramebuffer = gen8_DestroyFramebuffer,
    .vkCreateRenderPass = gen8_CreateRenderPass,
    .vkDestroyRenderPass = gen8_DestroyRenderPass,
    .vkGetRenderAreaGranularity = gen8_GetRenderAreaGranularity,
    .vkCreateCommandPool = gen8_CreateCommandPool,
    .vkDestroyCommandPool = gen8_DestroyCommandPool,
    .vkResetCommandPool = gen8_ResetCommandPool,
    .vkAllocateCommandBuffers = gen8_AllocateCommandBuffers,
    .vkFreeCommandBuffers = gen8_FreeCommandBuffers,
    .vkBeginCommandBuffer = gen8_BeginCommandBuffer,
    .vkEndCommandBuffer = gen8_EndCommandBuffer,
    .vkResetCommandBuffer = gen8_ResetCommandBuffer,
    .vkCmdBindPipeline = gen8_CmdBindPipeline,
    .vkCmdSetViewport = gen8_CmdSetViewport,
    .vkCmdSetScissor = gen8_CmdSetScissor,
    .vkCmdSetLineWidth = gen8_CmdSetLineWidth,
    .vkCmdSetDepthBias = gen8_CmdSetDepthBias,
    .vkCmdSetBlendConstants = gen8_CmdSetBlendConstants,
    .vkCmdSetDepthBounds = gen8_CmdSetDepthBounds,
    .vkCmdSetStencilCompareMask = gen8_CmdSetStencilCompareMask,
    .vkCmdSetStencilWriteMask = gen8_CmdSetStencilWriteMask,
    .vkCmdSetStencilReference = gen8_CmdSetStencilReference,
    .vkCmdBindDescriptorSets = gen8_CmdBindDescriptorSets,
    .vkCmdBindIndexBuffer = gen8_CmdBindIndexBuffer,
    .vkCmdBindVertexBuffers = gen8_CmdBindVertexBuffers,
    .vkCmdDraw = gen8_CmdDraw,
    .vkCmdDrawIndexed = gen8_CmdDrawIndexed,
    .vkCmdDrawIndirect = gen8_CmdDrawIndirect,
    .vkCmdDrawIndexedIndirect = gen8_CmdDrawIndexedIndirect,
    .vkCmdDispatch = gen8_CmdDispatch,
    .vkCmdDispatchIndirect = gen8_CmdDispatchIndirect,
    .vkCmdCopyBuffer = gen8_CmdCopyBuffer,
    .vkCmdCopyImage = gen8_CmdCopyImage,
    .vkCmdBlitImage = gen8_CmdBlitImage,
    .vkCmdCopyBufferToImage = gen8_CmdCopyBufferToImage,
    .vkCmdCopyImageToBuffer = gen8_CmdCopyImageToBuffer,
    .vkCmdUpdateBuffer = gen8_CmdUpdateBuffer,
    .vkCmdFillBuffer = gen8_CmdFillBuffer,
    .vkCmdClearColorImage = gen8_CmdClearColorImage,
    .vkCmdClearDepthStencilImage = gen8_CmdClearDepthStencilImage,
    .vkCmdClearAttachments = gen8_CmdClearAttachments,
    .vkCmdResolveImage = gen8_CmdResolveImage,
    .vkCmdSetEvent = gen8_CmdSetEvent,
    .vkCmdResetEvent = gen8_CmdResetEvent,
    .vkCmdWaitEvents = gen8_CmdWaitEvents,
    .vkCmdPipelineBarrier = gen8_CmdPipelineBarrier,
    .vkCmdBeginQuery = gen8_CmdBeginQuery,
    .vkCmdEndQuery = gen8_CmdEndQuery,
    .vkCmdBeginConditionalRenderingEXT = gen8_CmdBeginConditionalRenderingEXT,
    .vkCmdEndConditionalRenderingEXT = gen8_CmdEndConditionalRenderingEXT,
    .vkCmdResetQueryPool = gen8_CmdResetQueryPool,
    .vkCmdWriteTimestamp = gen8_CmdWriteTimestamp,
    .vkCmdCopyQueryPoolResults = gen8_CmdCopyQueryPoolResults,
    .vkCmdPushConstants = gen8_CmdPushConstants,
    .vkCmdBeginRenderPass = gen8_CmdBeginRenderPass,
    .vkCmdNextSubpass = gen8_CmdNextSubpass,
    .vkCmdEndRenderPass = gen8_CmdEndRenderPass,
    .vkCmdExecuteCommands = gen8_CmdExecuteCommands,
    .vkCreateSwapchainKHR = gen8_CreateSwapchainKHR,
    .vkDestroySwapchainKHR = gen8_DestroySwapchainKHR,
    .vkGetSwapchainImagesKHR = gen8_GetSwapchainImagesKHR,
    .vkAcquireNextImageKHR = gen8_AcquireNextImageKHR,
    .vkQueuePresentKHR = gen8_QueuePresentKHR,
    .vkCmdPushDescriptorSetKHR = gen8_CmdPushDescriptorSetKHR,
    .vkTrimCommandPool = gen8_TrimCommandPool,
    .vkTrimCommandPoolKHR = gen8_TrimCommandPool,
    .vkGetMemoryFdKHR = gen8_GetMemoryFdKHR,
    .vkGetMemoryFdPropertiesKHR = gen8_GetMemoryFdPropertiesKHR,
    .vkGetSemaphoreFdKHR = gen8_GetSemaphoreFdKHR,
    .vkImportSemaphoreFdKHR = gen8_ImportSemaphoreFdKHR,
    .vkGetFenceFdKHR = gen8_GetFenceFdKHR,
    .vkImportFenceFdKHR = gen8_ImportFenceFdKHR,
    .vkDisplayPowerControlEXT = gen8_DisplayPowerControlEXT,
    .vkRegisterDeviceEventEXT = gen8_RegisterDeviceEventEXT,
    .vkRegisterDisplayEventEXT = gen8_RegisterDisplayEventEXT,
    .vkGetSwapchainCounterEXT = gen8_GetSwapchainCounterEXT,
    .vkGetDeviceGroupPeerMemoryFeatures = gen8_GetDeviceGroupPeerMemoryFeatures,
    .vkGetDeviceGroupPeerMemoryFeaturesKHR = gen8_GetDeviceGroupPeerMemoryFeatures,
    .vkBindBufferMemory2 = gen8_BindBufferMemory2,
    .vkBindBufferMemory2KHR = gen8_BindBufferMemory2,
    .vkBindImageMemory2 = gen8_BindImageMemory2,
    .vkBindImageMemory2KHR = gen8_BindImageMemory2,
    .vkCmdSetDeviceMask = gen8_CmdSetDeviceMask,
    .vkCmdSetDeviceMaskKHR = gen8_CmdSetDeviceMask,
    .vkGetDeviceGroupPresentCapabilitiesKHR = gen8_GetDeviceGroupPresentCapabilitiesKHR,
    .vkGetDeviceGroupSurfacePresentModesKHR = gen8_GetDeviceGroupSurfacePresentModesKHR,
    .vkAcquireNextImage2KHR = gen8_AcquireNextImage2KHR,
    .vkCmdDispatchBase = gen8_CmdDispatchBase,
    .vkCmdDispatchBaseKHR = gen8_CmdDispatchBase,
    .vkCreateDescriptorUpdateTemplate = gen8_CreateDescriptorUpdateTemplate,
    .vkCreateDescriptorUpdateTemplateKHR = gen8_CreateDescriptorUpdateTemplate,
    .vkDestroyDescriptorUpdateTemplate = gen8_DestroyDescriptorUpdateTemplate,
    .vkDestroyDescriptorUpdateTemplateKHR = gen8_DestroyDescriptorUpdateTemplate,
    .vkUpdateDescriptorSetWithTemplate = gen8_UpdateDescriptorSetWithTemplate,
    .vkUpdateDescriptorSetWithTemplateKHR = gen8_UpdateDescriptorSetWithTemplate,
    .vkCmdPushDescriptorSetWithTemplateKHR = gen8_CmdPushDescriptorSetWithTemplateKHR,
    .vkGetBufferMemoryRequirements2 = gen8_GetBufferMemoryRequirements2,
    .vkGetBufferMemoryRequirements2KHR = gen8_GetBufferMemoryRequirements2,
    .vkGetImageMemoryRequirements2 = gen8_GetImageMemoryRequirements2,
    .vkGetImageMemoryRequirements2KHR = gen8_GetImageMemoryRequirements2,
    .vkGetImageSparseMemoryRequirements2 = gen8_GetImageSparseMemoryRequirements2,
    .vkGetImageSparseMemoryRequirements2KHR = gen8_GetImageSparseMemoryRequirements2,
    .vkCreateSamplerYcbcrConversion = gen8_CreateSamplerYcbcrConversion,
    .vkCreateSamplerYcbcrConversionKHR = gen8_CreateSamplerYcbcrConversion,
    .vkDestroySamplerYcbcrConversion = gen8_DestroySamplerYcbcrConversion,
    .vkDestroySamplerYcbcrConversionKHR = gen8_DestroySamplerYcbcrConversion,
    .vkGetDeviceQueue2 = gen8_GetDeviceQueue2,
    .vkGetDescriptorSetLayoutSupport = gen8_GetDescriptorSetLayoutSupport,
    .vkGetDescriptorSetLayoutSupportKHR = gen8_GetDescriptorSetLayoutSupport,
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .vkGetSwapchainGrallocUsageANDROID = gen8_GetSwapchainGrallocUsageANDROID,
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .vkGetSwapchainGrallocUsage2ANDROID = gen8_GetSwapchainGrallocUsage2ANDROID,
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .vkAcquireImageANDROID = gen8_AcquireImageANDROID,
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .vkQueueSignalReleaseImageANDROID = gen8_QueueSignalReleaseImageANDROID,
#endif // VK_USE_PLATFORM_ANDROID_KHR
    .vkGetCalibratedTimestampsEXT = gen8_GetCalibratedTimestampsEXT,
    .vkGetMemoryHostPointerPropertiesEXT = gen8_GetMemoryHostPointerPropertiesEXT,
    .vkCreateRenderPass2KHR = gen8_CreateRenderPass2KHR,
    .vkCmdBeginRenderPass2KHR = gen8_CmdBeginRenderPass2KHR,
    .vkCmdNextSubpass2KHR = gen8_CmdNextSubpass2KHR,
    .vkCmdEndRenderPass2KHR = gen8_CmdEndRenderPass2KHR,
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .vkGetAndroidHardwareBufferPropertiesANDROID = gen8_GetAndroidHardwareBufferPropertiesANDROID,
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .vkGetMemoryAndroidHardwareBufferANDROID = gen8_GetMemoryAndroidHardwareBufferANDROID,
#endif // VK_USE_PLATFORM_ANDROID_KHR
    .vkCmdDrawIndirectCountKHR = gen8_CmdDrawIndirectCountKHR,
    .vkCmdDrawIndexedIndirectCountKHR = gen8_CmdDrawIndexedIndirectCountKHR,
    .vkCmdBindTransformFeedbackBuffersEXT = gen8_CmdBindTransformFeedbackBuffersEXT,
    .vkCmdBeginTransformFeedbackEXT = gen8_CmdBeginTransformFeedbackEXT,
    .vkCmdEndTransformFeedbackEXT = gen8_CmdEndTransformFeedbackEXT,
    .vkCmdBeginQueryIndexedEXT = gen8_CmdBeginQueryIndexedEXT,
    .vkCmdEndQueryIndexedEXT = gen8_CmdEndQueryIndexedEXT,
    .vkCmdDrawIndirectByteCountEXT = gen8_CmdDrawIndirectByteCountEXT,
    .vkGetBufferDeviceAddressEXT = gen8_GetBufferDeviceAddressEXT,
    .vkGetPipelineExecutablePropertiesKHR = gen8_GetPipelineExecutablePropertiesKHR,
    .vkGetPipelineExecutableStatisticsKHR = gen8_GetPipelineExecutableStatisticsKHR,
    .vkGetPipelineExecutableInternalRepresentationsKHR = gen8_GetPipelineExecutableInternalRepresentationsKHR,
    .vkCmdSetLineStippleEXT = gen8_CmdSetLineStippleEXT,
    .vkCreateDmaBufImageINTEL = gen8_CreateDmaBufImageINTEL,
  };
      PFN_vkVoidFunction gen9_GetDeviceProcAddr(VkDevice device, const char* pName) __attribute__ ((weak));
      void gen9_DestroyDevice(VkDevice device, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      void gen9_GetDeviceQueue(VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue) __attribute__ ((weak));
      VkResult gen9_QueueSubmit(VkQueue queue, uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence) __attribute__ ((weak));
      VkResult gen9_QueueWaitIdle(VkQueue queue) __attribute__ ((weak));
      VkResult gen9_DeviceWaitIdle(VkDevice device) __attribute__ ((weak));
      VkResult gen9_AllocateMemory(VkDevice device, const VkMemoryAllocateInfo* pAllocateInfo, const VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory) __attribute__ ((weak));
      void gen9_FreeMemory(VkDevice device, VkDeviceMemory memory, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen9_MapMemory(VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData) __attribute__ ((weak));
      void gen9_UnmapMemory(VkDevice device, VkDeviceMemory memory) __attribute__ ((weak));
      VkResult gen9_FlushMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges) __attribute__ ((weak));
      VkResult gen9_InvalidateMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges) __attribute__ ((weak));
      void gen9_GetDeviceMemoryCommitment(VkDevice device, VkDeviceMemory memory, VkDeviceSize* pCommittedMemoryInBytes) __attribute__ ((weak));
      void gen9_GetBufferMemoryRequirements(VkDevice device, VkBuffer buffer, VkMemoryRequirements* pMemoryRequirements) __attribute__ ((weak));
      VkResult gen9_BindBufferMemory(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset) __attribute__ ((weak));
      void gen9_GetImageMemoryRequirements(VkDevice device, VkImage image, VkMemoryRequirements* pMemoryRequirements) __attribute__ ((weak));
      VkResult gen9_BindImageMemory(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset) __attribute__ ((weak));
      void gen9_GetImageSparseMemoryRequirements(VkDevice device, VkImage image, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements* pSparseMemoryRequirements) __attribute__ ((weak));
      VkResult gen9_QueueBindSparse(VkQueue queue, uint32_t bindInfoCount, const VkBindSparseInfo* pBindInfo, VkFence fence) __attribute__ ((weak));
      VkResult gen9_CreateFence(VkDevice device, const VkFenceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) __attribute__ ((weak));
      void gen9_DestroyFence(VkDevice device, VkFence fence, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen9_ResetFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences) __attribute__ ((weak));
      VkResult gen9_GetFenceStatus(VkDevice device, VkFence fence) __attribute__ ((weak));
      VkResult gen9_WaitForFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences, VkBool32 waitAll, uint64_t timeout) __attribute__ ((weak));
      VkResult gen9_CreateSemaphore(VkDevice device, const VkSemaphoreCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore) __attribute__ ((weak));
      void gen9_DestroySemaphore(VkDevice device, VkSemaphore semaphore, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen9_CreateEvent(VkDevice device, const VkEventCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkEvent* pEvent) __attribute__ ((weak));
      void gen9_DestroyEvent(VkDevice device, VkEvent event, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen9_GetEventStatus(VkDevice device, VkEvent event) __attribute__ ((weak));
      VkResult gen9_SetEvent(VkDevice device, VkEvent event) __attribute__ ((weak));
      VkResult gen9_ResetEvent(VkDevice device, VkEvent event) __attribute__ ((weak));
      VkResult gen9_CreateQueryPool(VkDevice device, const VkQueryPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkQueryPool* pQueryPool) __attribute__ ((weak));
      void gen9_DestroyQueryPool(VkDevice device, VkQueryPool queryPool, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen9_GetQueryPoolResults(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, VkDeviceSize stride, VkQueryResultFlags flags) __attribute__ ((weak));
      void gen9_ResetQueryPoolEXT(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) __attribute__ ((weak));
      VkResult gen9_CreateBuffer(VkDevice device, const VkBufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer) __attribute__ ((weak));
      void gen9_DestroyBuffer(VkDevice device, VkBuffer buffer, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen9_CreateBufferView(VkDevice device, const VkBufferViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferView* pView) __attribute__ ((weak));
      void gen9_DestroyBufferView(VkDevice device, VkBufferView bufferView, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen9_CreateImage(VkDevice device, const VkImageCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImage* pImage) __attribute__ ((weak));
      void gen9_DestroyImage(VkDevice device, VkImage image, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      void gen9_GetImageSubresourceLayout(VkDevice device, VkImage image, const VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout) __attribute__ ((weak));
      VkResult gen9_CreateImageView(VkDevice device, const VkImageViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImageView* pView) __attribute__ ((weak));
      void gen9_DestroyImageView(VkDevice device, VkImageView imageView, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen9_CreateShaderModule(VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule) __attribute__ ((weak));
      void gen9_DestroyShaderModule(VkDevice device, VkShaderModule shaderModule, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen9_CreatePipelineCache(VkDevice device, const VkPipelineCacheCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineCache* pPipelineCache) __attribute__ ((weak));
      void gen9_DestroyPipelineCache(VkDevice device, VkPipelineCache pipelineCache, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen9_GetPipelineCacheData(VkDevice device, VkPipelineCache pipelineCache, size_t* pDataSize, void* pData) __attribute__ ((weak));
      VkResult gen9_MergePipelineCaches(VkDevice device, VkPipelineCache dstCache, uint32_t srcCacheCount, const VkPipelineCache* pSrcCaches) __attribute__ ((weak));
      VkResult gen9_CreateGraphicsPipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkGraphicsPipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) __attribute__ ((weak));
      VkResult gen9_CreateComputePipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkComputePipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) __attribute__ ((weak));
      void gen9_DestroyPipeline(VkDevice device, VkPipeline pipeline, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen9_CreatePipelineLayout(VkDevice device, const VkPipelineLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout) __attribute__ ((weak));
      void gen9_DestroyPipelineLayout(VkDevice device, VkPipelineLayout pipelineLayout, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen9_CreateSampler(VkDevice device, const VkSamplerCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSampler* pSampler) __attribute__ ((weak));
      void gen9_DestroySampler(VkDevice device, VkSampler sampler, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen9_CreateDescriptorSetLayout(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorSetLayout* pSetLayout) __attribute__ ((weak));
      void gen9_DestroyDescriptorSetLayout(VkDevice device, VkDescriptorSetLayout descriptorSetLayout, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen9_CreateDescriptorPool(VkDevice device, const VkDescriptorPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorPool* pDescriptorPool) __attribute__ ((weak));
      void gen9_DestroyDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen9_ResetDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags flags) __attribute__ ((weak));
      VkResult gen9_AllocateDescriptorSets(VkDevice device, const VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets) __attribute__ ((weak));
      VkResult gen9_FreeDescriptorSets(VkDevice device, VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets) __attribute__ ((weak));
      void gen9_UpdateDescriptorSets(VkDevice device, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, const VkCopyDescriptorSet* pDescriptorCopies) __attribute__ ((weak));
      VkResult gen9_CreateFramebuffer(VkDevice device, const VkFramebufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFramebuffer* pFramebuffer) __attribute__ ((weak));
      void gen9_DestroyFramebuffer(VkDevice device, VkFramebuffer framebuffer, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen9_CreateRenderPass(VkDevice device, const VkRenderPassCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) __attribute__ ((weak));
      void gen9_DestroyRenderPass(VkDevice device, VkRenderPass renderPass, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      void gen9_GetRenderAreaGranularity(VkDevice device, VkRenderPass renderPass, VkExtent2D* pGranularity) __attribute__ ((weak));
      VkResult gen9_CreateCommandPool(VkDevice device, const VkCommandPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool) __attribute__ ((weak));
      void gen9_DestroyCommandPool(VkDevice device, VkCommandPool commandPool, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen9_ResetCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolResetFlags flags) __attribute__ ((weak));
      VkResult gen9_AllocateCommandBuffers(VkDevice device, const VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers) __attribute__ ((weak));
      void gen9_FreeCommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) __attribute__ ((weak));
      VkResult gen9_BeginCommandBuffer(VkCommandBuffer commandBuffer, const VkCommandBufferBeginInfo* pBeginInfo) __attribute__ ((weak));
      VkResult gen9_EndCommandBuffer(VkCommandBuffer commandBuffer) __attribute__ ((weak));
      VkResult gen9_ResetCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags) __attribute__ ((weak));
      void gen9_CmdBindPipeline(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline) __attribute__ ((weak));
      void gen9_CmdSetViewport(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pViewports) __attribute__ ((weak));
      void gen9_CmdSetScissor(VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pScissors) __attribute__ ((weak));
      void gen9_CmdSetLineWidth(VkCommandBuffer commandBuffer, float lineWidth) __attribute__ ((weak));
      void gen9_CmdSetDepthBias(VkCommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor) __attribute__ ((weak));
      void gen9_CmdSetBlendConstants(VkCommandBuffer commandBuffer, const float blendConstants[4]) __attribute__ ((weak));
      void gen9_CmdSetDepthBounds(VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds) __attribute__ ((weak));
      void gen9_CmdSetStencilCompareMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask) __attribute__ ((weak));
      void gen9_CmdSetStencilWriteMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask) __attribute__ ((weak));
      void gen9_CmdSetStencilReference(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference) __attribute__ ((weak));
      void gen9_CmdBindDescriptorSets(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets) __attribute__ ((weak));
      void gen9_CmdBindIndexBuffer(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType) __attribute__ ((weak));
      void gen9_CmdBindVertexBuffers(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets) __attribute__ ((weak));
      void gen9_CmdDraw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) __attribute__ ((weak));
      void gen9_CmdDrawIndexed(VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) __attribute__ ((weak));
      void gen9_CmdDrawIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) __attribute__ ((weak));
      void gen9_CmdDrawIndexedIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) __attribute__ ((weak));
      void gen9_CmdDispatch(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) __attribute__ ((weak));
      void gen9_CmdDispatchIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset) __attribute__ ((weak));
      void gen9_CmdCopyBuffer(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions) __attribute__ ((weak));
      void gen9_CmdCopyImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions) __attribute__ ((weak));
      void gen9_CmdBlitImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter) __attribute__ ((weak));
      void gen9_CmdCopyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions) __attribute__ ((weak));
      void gen9_CmdCopyImageToBuffer(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions) __attribute__ ((weak));
      void gen9_CmdUpdateBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData) __attribute__ ((weak));
      void gen9_CmdFillBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data) __attribute__ ((weak));
      void gen9_CmdClearColorImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) __attribute__ ((weak));
      void gen9_CmdClearDepthStencilImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) __attribute__ ((weak));
      void gen9_CmdClearAttachments(VkCommandBuffer commandBuffer, uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects) __attribute__ ((weak));
      void gen9_CmdResolveImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageResolve* pRegions) __attribute__ ((weak));
      void gen9_CmdSetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) __attribute__ ((weak));
      void gen9_CmdResetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) __attribute__ ((weak));
      void gen9_CmdWaitEvents(VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) __attribute__ ((weak));
      void gen9_CmdPipelineBarrier(VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) __attribute__ ((weak));
      void gen9_CmdBeginQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags) __attribute__ ((weak));
      void gen9_CmdEndQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query) __attribute__ ((weak));
      void gen9_CmdBeginConditionalRenderingEXT(VkCommandBuffer commandBuffer, const VkConditionalRenderingBeginInfoEXT* pConditionalRenderingBegin) __attribute__ ((weak));
      void gen9_CmdEndConditionalRenderingEXT(VkCommandBuffer commandBuffer) __attribute__ ((weak));
      void gen9_CmdResetQueryPool(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) __attribute__ ((weak));
      void gen9_CmdWriteTimestamp(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query) __attribute__ ((weak));
      void gen9_CmdCopyQueryPoolResults(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags) __attribute__ ((weak));
      void gen9_CmdPushConstants(VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues) __attribute__ ((weak));
      void gen9_CmdBeginRenderPass(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents) __attribute__ ((weak));
      void gen9_CmdNextSubpass(VkCommandBuffer commandBuffer, VkSubpassContents contents) __attribute__ ((weak));
      void gen9_CmdEndRenderPass(VkCommandBuffer commandBuffer) __attribute__ ((weak));
      void gen9_CmdExecuteCommands(VkCommandBuffer commandBuffer, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) __attribute__ ((weak));
      VkResult gen9_CreateSwapchainKHR(VkDevice device, const VkSwapchainCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain) __attribute__ ((weak));
      void gen9_DestroySwapchainKHR(VkDevice device, VkSwapchainKHR swapchain, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen9_GetSwapchainImagesKHR(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages) __attribute__ ((weak));
      VkResult gen9_AcquireNextImageKHR(VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex) __attribute__ ((weak));
      VkResult gen9_QueuePresentKHR(VkQueue queue, const VkPresentInfoKHR* pPresentInfo) __attribute__ ((weak));
      void gen9_CmdPushDescriptorSetKHR(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites) __attribute__ ((weak));
      void gen9_TrimCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags) __attribute__ ((weak));
            VkResult gen9_GetMemoryFdKHR(VkDevice device, const VkMemoryGetFdInfoKHR* pGetFdInfo, int* pFd) __attribute__ ((weak));
      VkResult gen9_GetMemoryFdPropertiesKHR(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, int fd, VkMemoryFdPropertiesKHR* pMemoryFdProperties) __attribute__ ((weak));
      VkResult gen9_GetSemaphoreFdKHR(VkDevice device, const VkSemaphoreGetFdInfoKHR* pGetFdInfo, int* pFd) __attribute__ ((weak));
      VkResult gen9_ImportSemaphoreFdKHR(VkDevice device, const VkImportSemaphoreFdInfoKHR* pImportSemaphoreFdInfo) __attribute__ ((weak));
      VkResult gen9_GetFenceFdKHR(VkDevice device, const VkFenceGetFdInfoKHR* pGetFdInfo, int* pFd) __attribute__ ((weak));
      VkResult gen9_ImportFenceFdKHR(VkDevice device, const VkImportFenceFdInfoKHR* pImportFenceFdInfo) __attribute__ ((weak));
      VkResult gen9_DisplayPowerControlEXT(VkDevice device, VkDisplayKHR display, const VkDisplayPowerInfoEXT* pDisplayPowerInfo) __attribute__ ((weak));
      VkResult gen9_RegisterDeviceEventEXT(VkDevice device, const VkDeviceEventInfoEXT* pDeviceEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) __attribute__ ((weak));
      VkResult gen9_RegisterDisplayEventEXT(VkDevice device, VkDisplayKHR display, const VkDisplayEventInfoEXT* pDisplayEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) __attribute__ ((weak));
      VkResult gen9_GetSwapchainCounterEXT(VkDevice device, VkSwapchainKHR swapchain, VkSurfaceCounterFlagBitsEXT counter, uint64_t* pCounterValue) __attribute__ ((weak));
      void gen9_GetDeviceGroupPeerMemoryFeatures(VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures) __attribute__ ((weak));
            VkResult gen9_BindBufferMemory2(VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos) __attribute__ ((weak));
            VkResult gen9_BindImageMemory2(VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos) __attribute__ ((weak));
            void gen9_CmdSetDeviceMask(VkCommandBuffer commandBuffer, uint32_t deviceMask) __attribute__ ((weak));
            VkResult gen9_GetDeviceGroupPresentCapabilitiesKHR(VkDevice device, VkDeviceGroupPresentCapabilitiesKHR* pDeviceGroupPresentCapabilities) __attribute__ ((weak));
      VkResult gen9_GetDeviceGroupSurfacePresentModesKHR(VkDevice device, VkSurfaceKHR surface, VkDeviceGroupPresentModeFlagsKHR* pModes) __attribute__ ((weak));
      VkResult gen9_AcquireNextImage2KHR(VkDevice device, const VkAcquireNextImageInfoKHR* pAcquireInfo, uint32_t* pImageIndex) __attribute__ ((weak));
      void gen9_CmdDispatchBase(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) __attribute__ ((weak));
            VkResult gen9_CreateDescriptorUpdateTemplate(VkDevice device, const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate) __attribute__ ((weak));
            void gen9_DestroyDescriptorUpdateTemplate(VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
            void gen9_UpdateDescriptorSetWithTemplate(VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData) __attribute__ ((weak));
            void gen9_CmdPushDescriptorSetWithTemplateKHR(VkCommandBuffer commandBuffer, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, const void* pData) __attribute__ ((weak));
      void gen9_GetBufferMemoryRequirements2(VkDevice device, const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) __attribute__ ((weak));
            void gen9_GetImageMemoryRequirements2(VkDevice device, const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) __attribute__ ((weak));
            void gen9_GetImageSparseMemoryRequirements2(VkDevice device, const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) __attribute__ ((weak));
            VkResult gen9_CreateSamplerYcbcrConversion(VkDevice device, const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion) __attribute__ ((weak));
            void gen9_DestroySamplerYcbcrConversion(VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
            void gen9_GetDeviceQueue2(VkDevice device, const VkDeviceQueueInfo2* pQueueInfo, VkQueue* pQueue) __attribute__ ((weak));
      void gen9_GetDescriptorSetLayoutSupport(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport) __attribute__ ((weak));
      #ifdef VK_USE_PLATFORM_ANDROID_KHR
      VkResult gen9_GetSwapchainGrallocUsageANDROID(VkDevice device, VkFormat format, VkImageUsageFlags imageUsage, int* grallocUsage) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
      VkResult gen9_GetSwapchainGrallocUsage2ANDROID(VkDevice device, VkFormat format, VkImageUsageFlags imageUsage, VkSwapchainImageUsageFlagsANDROID swapchainImageUsage, uint64_t* grallocConsumerUsage, uint64_t* grallocProducerUsage) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
      VkResult gen9_AcquireImageANDROID(VkDevice device, VkImage image, int nativeFenceFd, VkSemaphore semaphore, VkFence fence) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
      VkResult gen9_QueueSignalReleaseImageANDROID(VkQueue queue, uint32_t waitSemaphoreCount, const VkSemaphore* pWaitSemaphores, VkImage image, int* pNativeFenceFd) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_ANDROID_KHR
      VkResult gen9_GetCalibratedTimestampsEXT(VkDevice device, uint32_t timestampCount, const VkCalibratedTimestampInfoEXT* pTimestampInfos, uint64_t* pTimestamps, uint64_t* pMaxDeviation) __attribute__ ((weak));
      VkResult gen9_GetMemoryHostPointerPropertiesEXT(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, const void* pHostPointer, VkMemoryHostPointerPropertiesEXT* pMemoryHostPointerProperties) __attribute__ ((weak));
      VkResult gen9_CreateRenderPass2KHR(VkDevice device, const VkRenderPassCreateInfo2KHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) __attribute__ ((weak));
      void gen9_CmdBeginRenderPass2KHR(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo*      pRenderPassBegin, const VkSubpassBeginInfoKHR*      pSubpassBeginInfo) __attribute__ ((weak));
      void gen9_CmdNextSubpass2KHR(VkCommandBuffer commandBuffer, const VkSubpassBeginInfoKHR*      pSubpassBeginInfo, const VkSubpassEndInfoKHR*        pSubpassEndInfo) __attribute__ ((weak));
      void gen9_CmdEndRenderPass2KHR(VkCommandBuffer commandBuffer, const VkSubpassEndInfoKHR*        pSubpassEndInfo) __attribute__ ((weak));
#ifdef VK_USE_PLATFORM_ANDROID_KHR
      VkResult gen9_GetAndroidHardwareBufferPropertiesANDROID(VkDevice device, const struct AHardwareBuffer* buffer, VkAndroidHardwareBufferPropertiesANDROID* pProperties) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
      VkResult gen9_GetMemoryAndroidHardwareBufferANDROID(VkDevice device, const VkMemoryGetAndroidHardwareBufferInfoANDROID* pInfo, struct AHardwareBuffer** pBuffer) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_ANDROID_KHR
      void gen9_CmdDrawIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) __attribute__ ((weak));
      void gen9_CmdDrawIndexedIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) __attribute__ ((weak));
      void gen9_CmdBindTransformFeedbackBuffersEXT(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes) __attribute__ ((weak));
      void gen9_CmdBeginTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets) __attribute__ ((weak));
      void gen9_CmdEndTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets) __attribute__ ((weak));
      void gen9_CmdBeginQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags, uint32_t index) __attribute__ ((weak));
      void gen9_CmdEndQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, uint32_t index) __attribute__ ((weak));
      void gen9_CmdDrawIndirectByteCountEXT(VkCommandBuffer commandBuffer, uint32_t instanceCount, uint32_t firstInstance, VkBuffer counterBuffer, VkDeviceSize counterBufferOffset, uint32_t counterOffset, uint32_t vertexStride) __attribute__ ((weak));
      VkDeviceAddress gen9_GetBufferDeviceAddressEXT(VkDevice device, const VkBufferDeviceAddressInfoEXT* pInfo) __attribute__ ((weak));
      VkResult gen9_GetPipelineExecutablePropertiesKHR(VkDevice                        device, const VkPipelineInfoKHR*        pPipelineInfo, uint32_t* pExecutableCount, VkPipelineExecutablePropertiesKHR* pProperties) __attribute__ ((weak));
      VkResult gen9_GetPipelineExecutableStatisticsKHR(VkDevice                        device, const VkPipelineExecutableInfoKHR*  pExecutableInfo, uint32_t* pStatisticCount, VkPipelineExecutableStatisticKHR* pStatistics) __attribute__ ((weak));
      VkResult gen9_GetPipelineExecutableInternalRepresentationsKHR(VkDevice                        device, const VkPipelineExecutableInfoKHR*  pExecutableInfo, uint32_t* pInternalRepresentationCount, VkPipelineExecutableInternalRepresentationKHR* pInternalRepresentations) __attribute__ ((weak));
      void gen9_CmdSetLineStippleEXT(VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern) __attribute__ ((weak));
      VkResult gen9_CreateDmaBufImageINTEL(VkDevice device, const VkDmaBufImageCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMem, VkImage* pImage) __attribute__ ((weak));

  const struct anv_device_dispatch_table gen9_device_dispatch_table = {
    .vkGetDeviceProcAddr = gen9_GetDeviceProcAddr,
    .vkDestroyDevice = gen9_DestroyDevice,
    .vkGetDeviceQueue = gen9_GetDeviceQueue,
    .vkQueueSubmit = gen9_QueueSubmit,
    .vkQueueWaitIdle = gen9_QueueWaitIdle,
    .vkDeviceWaitIdle = gen9_DeviceWaitIdle,
    .vkAllocateMemory = gen9_AllocateMemory,
    .vkFreeMemory = gen9_FreeMemory,
    .vkMapMemory = gen9_MapMemory,
    .vkUnmapMemory = gen9_UnmapMemory,
    .vkFlushMappedMemoryRanges = gen9_FlushMappedMemoryRanges,
    .vkInvalidateMappedMemoryRanges = gen9_InvalidateMappedMemoryRanges,
    .vkGetDeviceMemoryCommitment = gen9_GetDeviceMemoryCommitment,
    .vkGetBufferMemoryRequirements = gen9_GetBufferMemoryRequirements,
    .vkBindBufferMemory = gen9_BindBufferMemory,
    .vkGetImageMemoryRequirements = gen9_GetImageMemoryRequirements,
    .vkBindImageMemory = gen9_BindImageMemory,
    .vkGetImageSparseMemoryRequirements = gen9_GetImageSparseMemoryRequirements,
    .vkQueueBindSparse = gen9_QueueBindSparse,
    .vkCreateFence = gen9_CreateFence,
    .vkDestroyFence = gen9_DestroyFence,
    .vkResetFences = gen9_ResetFences,
    .vkGetFenceStatus = gen9_GetFenceStatus,
    .vkWaitForFences = gen9_WaitForFences,
    .vkCreateSemaphore = gen9_CreateSemaphore,
    .vkDestroySemaphore = gen9_DestroySemaphore,
    .vkCreateEvent = gen9_CreateEvent,
    .vkDestroyEvent = gen9_DestroyEvent,
    .vkGetEventStatus = gen9_GetEventStatus,
    .vkSetEvent = gen9_SetEvent,
    .vkResetEvent = gen9_ResetEvent,
    .vkCreateQueryPool = gen9_CreateQueryPool,
    .vkDestroyQueryPool = gen9_DestroyQueryPool,
    .vkGetQueryPoolResults = gen9_GetQueryPoolResults,
    .vkResetQueryPoolEXT = gen9_ResetQueryPoolEXT,
    .vkCreateBuffer = gen9_CreateBuffer,
    .vkDestroyBuffer = gen9_DestroyBuffer,
    .vkCreateBufferView = gen9_CreateBufferView,
    .vkDestroyBufferView = gen9_DestroyBufferView,
    .vkCreateImage = gen9_CreateImage,
    .vkDestroyImage = gen9_DestroyImage,
    .vkGetImageSubresourceLayout = gen9_GetImageSubresourceLayout,
    .vkCreateImageView = gen9_CreateImageView,
    .vkDestroyImageView = gen9_DestroyImageView,
    .vkCreateShaderModule = gen9_CreateShaderModule,
    .vkDestroyShaderModule = gen9_DestroyShaderModule,
    .vkCreatePipelineCache = gen9_CreatePipelineCache,
    .vkDestroyPipelineCache = gen9_DestroyPipelineCache,
    .vkGetPipelineCacheData = gen9_GetPipelineCacheData,
    .vkMergePipelineCaches = gen9_MergePipelineCaches,
    .vkCreateGraphicsPipelines = gen9_CreateGraphicsPipelines,
    .vkCreateComputePipelines = gen9_CreateComputePipelines,
    .vkDestroyPipeline = gen9_DestroyPipeline,
    .vkCreatePipelineLayout = gen9_CreatePipelineLayout,
    .vkDestroyPipelineLayout = gen9_DestroyPipelineLayout,
    .vkCreateSampler = gen9_CreateSampler,
    .vkDestroySampler = gen9_DestroySampler,
    .vkCreateDescriptorSetLayout = gen9_CreateDescriptorSetLayout,
    .vkDestroyDescriptorSetLayout = gen9_DestroyDescriptorSetLayout,
    .vkCreateDescriptorPool = gen9_CreateDescriptorPool,
    .vkDestroyDescriptorPool = gen9_DestroyDescriptorPool,
    .vkResetDescriptorPool = gen9_ResetDescriptorPool,
    .vkAllocateDescriptorSets = gen9_AllocateDescriptorSets,
    .vkFreeDescriptorSets = gen9_FreeDescriptorSets,
    .vkUpdateDescriptorSets = gen9_UpdateDescriptorSets,
    .vkCreateFramebuffer = gen9_CreateFramebuffer,
    .vkDestroyFramebuffer = gen9_DestroyFramebuffer,
    .vkCreateRenderPass = gen9_CreateRenderPass,
    .vkDestroyRenderPass = gen9_DestroyRenderPass,
    .vkGetRenderAreaGranularity = gen9_GetRenderAreaGranularity,
    .vkCreateCommandPool = gen9_CreateCommandPool,
    .vkDestroyCommandPool = gen9_DestroyCommandPool,
    .vkResetCommandPool = gen9_ResetCommandPool,
    .vkAllocateCommandBuffers = gen9_AllocateCommandBuffers,
    .vkFreeCommandBuffers = gen9_FreeCommandBuffers,
    .vkBeginCommandBuffer = gen9_BeginCommandBuffer,
    .vkEndCommandBuffer = gen9_EndCommandBuffer,
    .vkResetCommandBuffer = gen9_ResetCommandBuffer,
    .vkCmdBindPipeline = gen9_CmdBindPipeline,
    .vkCmdSetViewport = gen9_CmdSetViewport,
    .vkCmdSetScissor = gen9_CmdSetScissor,
    .vkCmdSetLineWidth = gen9_CmdSetLineWidth,
    .vkCmdSetDepthBias = gen9_CmdSetDepthBias,
    .vkCmdSetBlendConstants = gen9_CmdSetBlendConstants,
    .vkCmdSetDepthBounds = gen9_CmdSetDepthBounds,
    .vkCmdSetStencilCompareMask = gen9_CmdSetStencilCompareMask,
    .vkCmdSetStencilWriteMask = gen9_CmdSetStencilWriteMask,
    .vkCmdSetStencilReference = gen9_CmdSetStencilReference,
    .vkCmdBindDescriptorSets = gen9_CmdBindDescriptorSets,
    .vkCmdBindIndexBuffer = gen9_CmdBindIndexBuffer,
    .vkCmdBindVertexBuffers = gen9_CmdBindVertexBuffers,
    .vkCmdDraw = gen9_CmdDraw,
    .vkCmdDrawIndexed = gen9_CmdDrawIndexed,
    .vkCmdDrawIndirect = gen9_CmdDrawIndirect,
    .vkCmdDrawIndexedIndirect = gen9_CmdDrawIndexedIndirect,
    .vkCmdDispatch = gen9_CmdDispatch,
    .vkCmdDispatchIndirect = gen9_CmdDispatchIndirect,
    .vkCmdCopyBuffer = gen9_CmdCopyBuffer,
    .vkCmdCopyImage = gen9_CmdCopyImage,
    .vkCmdBlitImage = gen9_CmdBlitImage,
    .vkCmdCopyBufferToImage = gen9_CmdCopyBufferToImage,
    .vkCmdCopyImageToBuffer = gen9_CmdCopyImageToBuffer,
    .vkCmdUpdateBuffer = gen9_CmdUpdateBuffer,
    .vkCmdFillBuffer = gen9_CmdFillBuffer,
    .vkCmdClearColorImage = gen9_CmdClearColorImage,
    .vkCmdClearDepthStencilImage = gen9_CmdClearDepthStencilImage,
    .vkCmdClearAttachments = gen9_CmdClearAttachments,
    .vkCmdResolveImage = gen9_CmdResolveImage,
    .vkCmdSetEvent = gen9_CmdSetEvent,
    .vkCmdResetEvent = gen9_CmdResetEvent,
    .vkCmdWaitEvents = gen9_CmdWaitEvents,
    .vkCmdPipelineBarrier = gen9_CmdPipelineBarrier,
    .vkCmdBeginQuery = gen9_CmdBeginQuery,
    .vkCmdEndQuery = gen9_CmdEndQuery,
    .vkCmdBeginConditionalRenderingEXT = gen9_CmdBeginConditionalRenderingEXT,
    .vkCmdEndConditionalRenderingEXT = gen9_CmdEndConditionalRenderingEXT,
    .vkCmdResetQueryPool = gen9_CmdResetQueryPool,
    .vkCmdWriteTimestamp = gen9_CmdWriteTimestamp,
    .vkCmdCopyQueryPoolResults = gen9_CmdCopyQueryPoolResults,
    .vkCmdPushConstants = gen9_CmdPushConstants,
    .vkCmdBeginRenderPass = gen9_CmdBeginRenderPass,
    .vkCmdNextSubpass = gen9_CmdNextSubpass,
    .vkCmdEndRenderPass = gen9_CmdEndRenderPass,
    .vkCmdExecuteCommands = gen9_CmdExecuteCommands,
    .vkCreateSwapchainKHR = gen9_CreateSwapchainKHR,
    .vkDestroySwapchainKHR = gen9_DestroySwapchainKHR,
    .vkGetSwapchainImagesKHR = gen9_GetSwapchainImagesKHR,
    .vkAcquireNextImageKHR = gen9_AcquireNextImageKHR,
    .vkQueuePresentKHR = gen9_QueuePresentKHR,
    .vkCmdPushDescriptorSetKHR = gen9_CmdPushDescriptorSetKHR,
    .vkTrimCommandPool = gen9_TrimCommandPool,
    .vkTrimCommandPoolKHR = gen9_TrimCommandPool,
    .vkGetMemoryFdKHR = gen9_GetMemoryFdKHR,
    .vkGetMemoryFdPropertiesKHR = gen9_GetMemoryFdPropertiesKHR,
    .vkGetSemaphoreFdKHR = gen9_GetSemaphoreFdKHR,
    .vkImportSemaphoreFdKHR = gen9_ImportSemaphoreFdKHR,
    .vkGetFenceFdKHR = gen9_GetFenceFdKHR,
    .vkImportFenceFdKHR = gen9_ImportFenceFdKHR,
    .vkDisplayPowerControlEXT = gen9_DisplayPowerControlEXT,
    .vkRegisterDeviceEventEXT = gen9_RegisterDeviceEventEXT,
    .vkRegisterDisplayEventEXT = gen9_RegisterDisplayEventEXT,
    .vkGetSwapchainCounterEXT = gen9_GetSwapchainCounterEXT,
    .vkGetDeviceGroupPeerMemoryFeatures = gen9_GetDeviceGroupPeerMemoryFeatures,
    .vkGetDeviceGroupPeerMemoryFeaturesKHR = gen9_GetDeviceGroupPeerMemoryFeatures,
    .vkBindBufferMemory2 = gen9_BindBufferMemory2,
    .vkBindBufferMemory2KHR = gen9_BindBufferMemory2,
    .vkBindImageMemory2 = gen9_BindImageMemory2,
    .vkBindImageMemory2KHR = gen9_BindImageMemory2,
    .vkCmdSetDeviceMask = gen9_CmdSetDeviceMask,
    .vkCmdSetDeviceMaskKHR = gen9_CmdSetDeviceMask,
    .vkGetDeviceGroupPresentCapabilitiesKHR = gen9_GetDeviceGroupPresentCapabilitiesKHR,
    .vkGetDeviceGroupSurfacePresentModesKHR = gen9_GetDeviceGroupSurfacePresentModesKHR,
    .vkAcquireNextImage2KHR = gen9_AcquireNextImage2KHR,
    .vkCmdDispatchBase = gen9_CmdDispatchBase,
    .vkCmdDispatchBaseKHR = gen9_CmdDispatchBase,
    .vkCreateDescriptorUpdateTemplate = gen9_CreateDescriptorUpdateTemplate,
    .vkCreateDescriptorUpdateTemplateKHR = gen9_CreateDescriptorUpdateTemplate,
    .vkDestroyDescriptorUpdateTemplate = gen9_DestroyDescriptorUpdateTemplate,
    .vkDestroyDescriptorUpdateTemplateKHR = gen9_DestroyDescriptorUpdateTemplate,
    .vkUpdateDescriptorSetWithTemplate = gen9_UpdateDescriptorSetWithTemplate,
    .vkUpdateDescriptorSetWithTemplateKHR = gen9_UpdateDescriptorSetWithTemplate,
    .vkCmdPushDescriptorSetWithTemplateKHR = gen9_CmdPushDescriptorSetWithTemplateKHR,
    .vkGetBufferMemoryRequirements2 = gen9_GetBufferMemoryRequirements2,
    .vkGetBufferMemoryRequirements2KHR = gen9_GetBufferMemoryRequirements2,
    .vkGetImageMemoryRequirements2 = gen9_GetImageMemoryRequirements2,
    .vkGetImageMemoryRequirements2KHR = gen9_GetImageMemoryRequirements2,
    .vkGetImageSparseMemoryRequirements2 = gen9_GetImageSparseMemoryRequirements2,
    .vkGetImageSparseMemoryRequirements2KHR = gen9_GetImageSparseMemoryRequirements2,
    .vkCreateSamplerYcbcrConversion = gen9_CreateSamplerYcbcrConversion,
    .vkCreateSamplerYcbcrConversionKHR = gen9_CreateSamplerYcbcrConversion,
    .vkDestroySamplerYcbcrConversion = gen9_DestroySamplerYcbcrConversion,
    .vkDestroySamplerYcbcrConversionKHR = gen9_DestroySamplerYcbcrConversion,
    .vkGetDeviceQueue2 = gen9_GetDeviceQueue2,
    .vkGetDescriptorSetLayoutSupport = gen9_GetDescriptorSetLayoutSupport,
    .vkGetDescriptorSetLayoutSupportKHR = gen9_GetDescriptorSetLayoutSupport,
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .vkGetSwapchainGrallocUsageANDROID = gen9_GetSwapchainGrallocUsageANDROID,
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .vkGetSwapchainGrallocUsage2ANDROID = gen9_GetSwapchainGrallocUsage2ANDROID,
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .vkAcquireImageANDROID = gen9_AcquireImageANDROID,
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .vkQueueSignalReleaseImageANDROID = gen9_QueueSignalReleaseImageANDROID,
#endif // VK_USE_PLATFORM_ANDROID_KHR
    .vkGetCalibratedTimestampsEXT = gen9_GetCalibratedTimestampsEXT,
    .vkGetMemoryHostPointerPropertiesEXT = gen9_GetMemoryHostPointerPropertiesEXT,
    .vkCreateRenderPass2KHR = gen9_CreateRenderPass2KHR,
    .vkCmdBeginRenderPass2KHR = gen9_CmdBeginRenderPass2KHR,
    .vkCmdNextSubpass2KHR = gen9_CmdNextSubpass2KHR,
    .vkCmdEndRenderPass2KHR = gen9_CmdEndRenderPass2KHR,
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .vkGetAndroidHardwareBufferPropertiesANDROID = gen9_GetAndroidHardwareBufferPropertiesANDROID,
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .vkGetMemoryAndroidHardwareBufferANDROID = gen9_GetMemoryAndroidHardwareBufferANDROID,
#endif // VK_USE_PLATFORM_ANDROID_KHR
    .vkCmdDrawIndirectCountKHR = gen9_CmdDrawIndirectCountKHR,
    .vkCmdDrawIndexedIndirectCountKHR = gen9_CmdDrawIndexedIndirectCountKHR,
    .vkCmdBindTransformFeedbackBuffersEXT = gen9_CmdBindTransformFeedbackBuffersEXT,
    .vkCmdBeginTransformFeedbackEXT = gen9_CmdBeginTransformFeedbackEXT,
    .vkCmdEndTransformFeedbackEXT = gen9_CmdEndTransformFeedbackEXT,
    .vkCmdBeginQueryIndexedEXT = gen9_CmdBeginQueryIndexedEXT,
    .vkCmdEndQueryIndexedEXT = gen9_CmdEndQueryIndexedEXT,
    .vkCmdDrawIndirectByteCountEXT = gen9_CmdDrawIndirectByteCountEXT,
    .vkGetBufferDeviceAddressEXT = gen9_GetBufferDeviceAddressEXT,
    .vkGetPipelineExecutablePropertiesKHR = gen9_GetPipelineExecutablePropertiesKHR,
    .vkGetPipelineExecutableStatisticsKHR = gen9_GetPipelineExecutableStatisticsKHR,
    .vkGetPipelineExecutableInternalRepresentationsKHR = gen9_GetPipelineExecutableInternalRepresentationsKHR,
    .vkCmdSetLineStippleEXT = gen9_CmdSetLineStippleEXT,
    .vkCreateDmaBufImageINTEL = gen9_CreateDmaBufImageINTEL,
  };
      PFN_vkVoidFunction gen10_GetDeviceProcAddr(VkDevice device, const char* pName) __attribute__ ((weak));
      void gen10_DestroyDevice(VkDevice device, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      void gen10_GetDeviceQueue(VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue) __attribute__ ((weak));
      VkResult gen10_QueueSubmit(VkQueue queue, uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence) __attribute__ ((weak));
      VkResult gen10_QueueWaitIdle(VkQueue queue) __attribute__ ((weak));
      VkResult gen10_DeviceWaitIdle(VkDevice device) __attribute__ ((weak));
      VkResult gen10_AllocateMemory(VkDevice device, const VkMemoryAllocateInfo* pAllocateInfo, const VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory) __attribute__ ((weak));
      void gen10_FreeMemory(VkDevice device, VkDeviceMemory memory, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen10_MapMemory(VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData) __attribute__ ((weak));
      void gen10_UnmapMemory(VkDevice device, VkDeviceMemory memory) __attribute__ ((weak));
      VkResult gen10_FlushMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges) __attribute__ ((weak));
      VkResult gen10_InvalidateMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges) __attribute__ ((weak));
      void gen10_GetDeviceMemoryCommitment(VkDevice device, VkDeviceMemory memory, VkDeviceSize* pCommittedMemoryInBytes) __attribute__ ((weak));
      void gen10_GetBufferMemoryRequirements(VkDevice device, VkBuffer buffer, VkMemoryRequirements* pMemoryRequirements) __attribute__ ((weak));
      VkResult gen10_BindBufferMemory(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset) __attribute__ ((weak));
      void gen10_GetImageMemoryRequirements(VkDevice device, VkImage image, VkMemoryRequirements* pMemoryRequirements) __attribute__ ((weak));
      VkResult gen10_BindImageMemory(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset) __attribute__ ((weak));
      void gen10_GetImageSparseMemoryRequirements(VkDevice device, VkImage image, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements* pSparseMemoryRequirements) __attribute__ ((weak));
      VkResult gen10_QueueBindSparse(VkQueue queue, uint32_t bindInfoCount, const VkBindSparseInfo* pBindInfo, VkFence fence) __attribute__ ((weak));
      VkResult gen10_CreateFence(VkDevice device, const VkFenceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) __attribute__ ((weak));
      void gen10_DestroyFence(VkDevice device, VkFence fence, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen10_ResetFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences) __attribute__ ((weak));
      VkResult gen10_GetFenceStatus(VkDevice device, VkFence fence) __attribute__ ((weak));
      VkResult gen10_WaitForFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences, VkBool32 waitAll, uint64_t timeout) __attribute__ ((weak));
      VkResult gen10_CreateSemaphore(VkDevice device, const VkSemaphoreCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore) __attribute__ ((weak));
      void gen10_DestroySemaphore(VkDevice device, VkSemaphore semaphore, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen10_CreateEvent(VkDevice device, const VkEventCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkEvent* pEvent) __attribute__ ((weak));
      void gen10_DestroyEvent(VkDevice device, VkEvent event, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen10_GetEventStatus(VkDevice device, VkEvent event) __attribute__ ((weak));
      VkResult gen10_SetEvent(VkDevice device, VkEvent event) __attribute__ ((weak));
      VkResult gen10_ResetEvent(VkDevice device, VkEvent event) __attribute__ ((weak));
      VkResult gen10_CreateQueryPool(VkDevice device, const VkQueryPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkQueryPool* pQueryPool) __attribute__ ((weak));
      void gen10_DestroyQueryPool(VkDevice device, VkQueryPool queryPool, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen10_GetQueryPoolResults(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, VkDeviceSize stride, VkQueryResultFlags flags) __attribute__ ((weak));
      void gen10_ResetQueryPoolEXT(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) __attribute__ ((weak));
      VkResult gen10_CreateBuffer(VkDevice device, const VkBufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer) __attribute__ ((weak));
      void gen10_DestroyBuffer(VkDevice device, VkBuffer buffer, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen10_CreateBufferView(VkDevice device, const VkBufferViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferView* pView) __attribute__ ((weak));
      void gen10_DestroyBufferView(VkDevice device, VkBufferView bufferView, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen10_CreateImage(VkDevice device, const VkImageCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImage* pImage) __attribute__ ((weak));
      void gen10_DestroyImage(VkDevice device, VkImage image, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      void gen10_GetImageSubresourceLayout(VkDevice device, VkImage image, const VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout) __attribute__ ((weak));
      VkResult gen10_CreateImageView(VkDevice device, const VkImageViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImageView* pView) __attribute__ ((weak));
      void gen10_DestroyImageView(VkDevice device, VkImageView imageView, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen10_CreateShaderModule(VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule) __attribute__ ((weak));
      void gen10_DestroyShaderModule(VkDevice device, VkShaderModule shaderModule, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen10_CreatePipelineCache(VkDevice device, const VkPipelineCacheCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineCache* pPipelineCache) __attribute__ ((weak));
      void gen10_DestroyPipelineCache(VkDevice device, VkPipelineCache pipelineCache, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen10_GetPipelineCacheData(VkDevice device, VkPipelineCache pipelineCache, size_t* pDataSize, void* pData) __attribute__ ((weak));
      VkResult gen10_MergePipelineCaches(VkDevice device, VkPipelineCache dstCache, uint32_t srcCacheCount, const VkPipelineCache* pSrcCaches) __attribute__ ((weak));
      VkResult gen10_CreateGraphicsPipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkGraphicsPipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) __attribute__ ((weak));
      VkResult gen10_CreateComputePipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkComputePipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) __attribute__ ((weak));
      void gen10_DestroyPipeline(VkDevice device, VkPipeline pipeline, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen10_CreatePipelineLayout(VkDevice device, const VkPipelineLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout) __attribute__ ((weak));
      void gen10_DestroyPipelineLayout(VkDevice device, VkPipelineLayout pipelineLayout, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen10_CreateSampler(VkDevice device, const VkSamplerCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSampler* pSampler) __attribute__ ((weak));
      void gen10_DestroySampler(VkDevice device, VkSampler sampler, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen10_CreateDescriptorSetLayout(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorSetLayout* pSetLayout) __attribute__ ((weak));
      void gen10_DestroyDescriptorSetLayout(VkDevice device, VkDescriptorSetLayout descriptorSetLayout, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen10_CreateDescriptorPool(VkDevice device, const VkDescriptorPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorPool* pDescriptorPool) __attribute__ ((weak));
      void gen10_DestroyDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen10_ResetDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags flags) __attribute__ ((weak));
      VkResult gen10_AllocateDescriptorSets(VkDevice device, const VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets) __attribute__ ((weak));
      VkResult gen10_FreeDescriptorSets(VkDevice device, VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets) __attribute__ ((weak));
      void gen10_UpdateDescriptorSets(VkDevice device, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, const VkCopyDescriptorSet* pDescriptorCopies) __attribute__ ((weak));
      VkResult gen10_CreateFramebuffer(VkDevice device, const VkFramebufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFramebuffer* pFramebuffer) __attribute__ ((weak));
      void gen10_DestroyFramebuffer(VkDevice device, VkFramebuffer framebuffer, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen10_CreateRenderPass(VkDevice device, const VkRenderPassCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) __attribute__ ((weak));
      void gen10_DestroyRenderPass(VkDevice device, VkRenderPass renderPass, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      void gen10_GetRenderAreaGranularity(VkDevice device, VkRenderPass renderPass, VkExtent2D* pGranularity) __attribute__ ((weak));
      VkResult gen10_CreateCommandPool(VkDevice device, const VkCommandPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool) __attribute__ ((weak));
      void gen10_DestroyCommandPool(VkDevice device, VkCommandPool commandPool, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen10_ResetCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolResetFlags flags) __attribute__ ((weak));
      VkResult gen10_AllocateCommandBuffers(VkDevice device, const VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers) __attribute__ ((weak));
      void gen10_FreeCommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) __attribute__ ((weak));
      VkResult gen10_BeginCommandBuffer(VkCommandBuffer commandBuffer, const VkCommandBufferBeginInfo* pBeginInfo) __attribute__ ((weak));
      VkResult gen10_EndCommandBuffer(VkCommandBuffer commandBuffer) __attribute__ ((weak));
      VkResult gen10_ResetCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags) __attribute__ ((weak));
      void gen10_CmdBindPipeline(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline) __attribute__ ((weak));
      void gen10_CmdSetViewport(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pViewports) __attribute__ ((weak));
      void gen10_CmdSetScissor(VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pScissors) __attribute__ ((weak));
      void gen10_CmdSetLineWidth(VkCommandBuffer commandBuffer, float lineWidth) __attribute__ ((weak));
      void gen10_CmdSetDepthBias(VkCommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor) __attribute__ ((weak));
      void gen10_CmdSetBlendConstants(VkCommandBuffer commandBuffer, const float blendConstants[4]) __attribute__ ((weak));
      void gen10_CmdSetDepthBounds(VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds) __attribute__ ((weak));
      void gen10_CmdSetStencilCompareMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask) __attribute__ ((weak));
      void gen10_CmdSetStencilWriteMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask) __attribute__ ((weak));
      void gen10_CmdSetStencilReference(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference) __attribute__ ((weak));
      void gen10_CmdBindDescriptorSets(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets) __attribute__ ((weak));
      void gen10_CmdBindIndexBuffer(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType) __attribute__ ((weak));
      void gen10_CmdBindVertexBuffers(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets) __attribute__ ((weak));
      void gen10_CmdDraw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) __attribute__ ((weak));
      void gen10_CmdDrawIndexed(VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) __attribute__ ((weak));
      void gen10_CmdDrawIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) __attribute__ ((weak));
      void gen10_CmdDrawIndexedIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) __attribute__ ((weak));
      void gen10_CmdDispatch(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) __attribute__ ((weak));
      void gen10_CmdDispatchIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset) __attribute__ ((weak));
      void gen10_CmdCopyBuffer(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions) __attribute__ ((weak));
      void gen10_CmdCopyImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions) __attribute__ ((weak));
      void gen10_CmdBlitImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter) __attribute__ ((weak));
      void gen10_CmdCopyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions) __attribute__ ((weak));
      void gen10_CmdCopyImageToBuffer(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions) __attribute__ ((weak));
      void gen10_CmdUpdateBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData) __attribute__ ((weak));
      void gen10_CmdFillBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data) __attribute__ ((weak));
      void gen10_CmdClearColorImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) __attribute__ ((weak));
      void gen10_CmdClearDepthStencilImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) __attribute__ ((weak));
      void gen10_CmdClearAttachments(VkCommandBuffer commandBuffer, uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects) __attribute__ ((weak));
      void gen10_CmdResolveImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageResolve* pRegions) __attribute__ ((weak));
      void gen10_CmdSetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) __attribute__ ((weak));
      void gen10_CmdResetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) __attribute__ ((weak));
      void gen10_CmdWaitEvents(VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) __attribute__ ((weak));
      void gen10_CmdPipelineBarrier(VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) __attribute__ ((weak));
      void gen10_CmdBeginQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags) __attribute__ ((weak));
      void gen10_CmdEndQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query) __attribute__ ((weak));
      void gen10_CmdBeginConditionalRenderingEXT(VkCommandBuffer commandBuffer, const VkConditionalRenderingBeginInfoEXT* pConditionalRenderingBegin) __attribute__ ((weak));
      void gen10_CmdEndConditionalRenderingEXT(VkCommandBuffer commandBuffer) __attribute__ ((weak));
      void gen10_CmdResetQueryPool(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) __attribute__ ((weak));
      void gen10_CmdWriteTimestamp(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query) __attribute__ ((weak));
      void gen10_CmdCopyQueryPoolResults(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags) __attribute__ ((weak));
      void gen10_CmdPushConstants(VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues) __attribute__ ((weak));
      void gen10_CmdBeginRenderPass(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents) __attribute__ ((weak));
      void gen10_CmdNextSubpass(VkCommandBuffer commandBuffer, VkSubpassContents contents) __attribute__ ((weak));
      void gen10_CmdEndRenderPass(VkCommandBuffer commandBuffer) __attribute__ ((weak));
      void gen10_CmdExecuteCommands(VkCommandBuffer commandBuffer, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) __attribute__ ((weak));
      VkResult gen10_CreateSwapchainKHR(VkDevice device, const VkSwapchainCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain) __attribute__ ((weak));
      void gen10_DestroySwapchainKHR(VkDevice device, VkSwapchainKHR swapchain, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen10_GetSwapchainImagesKHR(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages) __attribute__ ((weak));
      VkResult gen10_AcquireNextImageKHR(VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex) __attribute__ ((weak));
      VkResult gen10_QueuePresentKHR(VkQueue queue, const VkPresentInfoKHR* pPresentInfo) __attribute__ ((weak));
      void gen10_CmdPushDescriptorSetKHR(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites) __attribute__ ((weak));
      void gen10_TrimCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags) __attribute__ ((weak));
            VkResult gen10_GetMemoryFdKHR(VkDevice device, const VkMemoryGetFdInfoKHR* pGetFdInfo, int* pFd) __attribute__ ((weak));
      VkResult gen10_GetMemoryFdPropertiesKHR(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, int fd, VkMemoryFdPropertiesKHR* pMemoryFdProperties) __attribute__ ((weak));
      VkResult gen10_GetSemaphoreFdKHR(VkDevice device, const VkSemaphoreGetFdInfoKHR* pGetFdInfo, int* pFd) __attribute__ ((weak));
      VkResult gen10_ImportSemaphoreFdKHR(VkDevice device, const VkImportSemaphoreFdInfoKHR* pImportSemaphoreFdInfo) __attribute__ ((weak));
      VkResult gen10_GetFenceFdKHR(VkDevice device, const VkFenceGetFdInfoKHR* pGetFdInfo, int* pFd) __attribute__ ((weak));
      VkResult gen10_ImportFenceFdKHR(VkDevice device, const VkImportFenceFdInfoKHR* pImportFenceFdInfo) __attribute__ ((weak));
      VkResult gen10_DisplayPowerControlEXT(VkDevice device, VkDisplayKHR display, const VkDisplayPowerInfoEXT* pDisplayPowerInfo) __attribute__ ((weak));
      VkResult gen10_RegisterDeviceEventEXT(VkDevice device, const VkDeviceEventInfoEXT* pDeviceEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) __attribute__ ((weak));
      VkResult gen10_RegisterDisplayEventEXT(VkDevice device, VkDisplayKHR display, const VkDisplayEventInfoEXT* pDisplayEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) __attribute__ ((weak));
      VkResult gen10_GetSwapchainCounterEXT(VkDevice device, VkSwapchainKHR swapchain, VkSurfaceCounterFlagBitsEXT counter, uint64_t* pCounterValue) __attribute__ ((weak));
      void gen10_GetDeviceGroupPeerMemoryFeatures(VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures) __attribute__ ((weak));
            VkResult gen10_BindBufferMemory2(VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos) __attribute__ ((weak));
            VkResult gen10_BindImageMemory2(VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos) __attribute__ ((weak));
            void gen10_CmdSetDeviceMask(VkCommandBuffer commandBuffer, uint32_t deviceMask) __attribute__ ((weak));
            VkResult gen10_GetDeviceGroupPresentCapabilitiesKHR(VkDevice device, VkDeviceGroupPresentCapabilitiesKHR* pDeviceGroupPresentCapabilities) __attribute__ ((weak));
      VkResult gen10_GetDeviceGroupSurfacePresentModesKHR(VkDevice device, VkSurfaceKHR surface, VkDeviceGroupPresentModeFlagsKHR* pModes) __attribute__ ((weak));
      VkResult gen10_AcquireNextImage2KHR(VkDevice device, const VkAcquireNextImageInfoKHR* pAcquireInfo, uint32_t* pImageIndex) __attribute__ ((weak));
      void gen10_CmdDispatchBase(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) __attribute__ ((weak));
            VkResult gen10_CreateDescriptorUpdateTemplate(VkDevice device, const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate) __attribute__ ((weak));
            void gen10_DestroyDescriptorUpdateTemplate(VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
            void gen10_UpdateDescriptorSetWithTemplate(VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData) __attribute__ ((weak));
            void gen10_CmdPushDescriptorSetWithTemplateKHR(VkCommandBuffer commandBuffer, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, const void* pData) __attribute__ ((weak));
      void gen10_GetBufferMemoryRequirements2(VkDevice device, const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) __attribute__ ((weak));
            void gen10_GetImageMemoryRequirements2(VkDevice device, const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) __attribute__ ((weak));
            void gen10_GetImageSparseMemoryRequirements2(VkDevice device, const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) __attribute__ ((weak));
            VkResult gen10_CreateSamplerYcbcrConversion(VkDevice device, const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion) __attribute__ ((weak));
            void gen10_DestroySamplerYcbcrConversion(VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
            void gen10_GetDeviceQueue2(VkDevice device, const VkDeviceQueueInfo2* pQueueInfo, VkQueue* pQueue) __attribute__ ((weak));
      void gen10_GetDescriptorSetLayoutSupport(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport) __attribute__ ((weak));
      #ifdef VK_USE_PLATFORM_ANDROID_KHR
      VkResult gen10_GetSwapchainGrallocUsageANDROID(VkDevice device, VkFormat format, VkImageUsageFlags imageUsage, int* grallocUsage) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
      VkResult gen10_GetSwapchainGrallocUsage2ANDROID(VkDevice device, VkFormat format, VkImageUsageFlags imageUsage, VkSwapchainImageUsageFlagsANDROID swapchainImageUsage, uint64_t* grallocConsumerUsage, uint64_t* grallocProducerUsage) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
      VkResult gen10_AcquireImageANDROID(VkDevice device, VkImage image, int nativeFenceFd, VkSemaphore semaphore, VkFence fence) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
      VkResult gen10_QueueSignalReleaseImageANDROID(VkQueue queue, uint32_t waitSemaphoreCount, const VkSemaphore* pWaitSemaphores, VkImage image, int* pNativeFenceFd) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_ANDROID_KHR
      VkResult gen10_GetCalibratedTimestampsEXT(VkDevice device, uint32_t timestampCount, const VkCalibratedTimestampInfoEXT* pTimestampInfos, uint64_t* pTimestamps, uint64_t* pMaxDeviation) __attribute__ ((weak));
      VkResult gen10_GetMemoryHostPointerPropertiesEXT(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, const void* pHostPointer, VkMemoryHostPointerPropertiesEXT* pMemoryHostPointerProperties) __attribute__ ((weak));
      VkResult gen10_CreateRenderPass2KHR(VkDevice device, const VkRenderPassCreateInfo2KHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) __attribute__ ((weak));
      void gen10_CmdBeginRenderPass2KHR(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo*      pRenderPassBegin, const VkSubpassBeginInfoKHR*      pSubpassBeginInfo) __attribute__ ((weak));
      void gen10_CmdNextSubpass2KHR(VkCommandBuffer commandBuffer, const VkSubpassBeginInfoKHR*      pSubpassBeginInfo, const VkSubpassEndInfoKHR*        pSubpassEndInfo) __attribute__ ((weak));
      void gen10_CmdEndRenderPass2KHR(VkCommandBuffer commandBuffer, const VkSubpassEndInfoKHR*        pSubpassEndInfo) __attribute__ ((weak));
#ifdef VK_USE_PLATFORM_ANDROID_KHR
      VkResult gen10_GetAndroidHardwareBufferPropertiesANDROID(VkDevice device, const struct AHardwareBuffer* buffer, VkAndroidHardwareBufferPropertiesANDROID* pProperties) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
      VkResult gen10_GetMemoryAndroidHardwareBufferANDROID(VkDevice device, const VkMemoryGetAndroidHardwareBufferInfoANDROID* pInfo, struct AHardwareBuffer** pBuffer) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_ANDROID_KHR
      void gen10_CmdDrawIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) __attribute__ ((weak));
      void gen10_CmdDrawIndexedIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) __attribute__ ((weak));
      void gen10_CmdBindTransformFeedbackBuffersEXT(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes) __attribute__ ((weak));
      void gen10_CmdBeginTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets) __attribute__ ((weak));
      void gen10_CmdEndTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets) __attribute__ ((weak));
      void gen10_CmdBeginQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags, uint32_t index) __attribute__ ((weak));
      void gen10_CmdEndQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, uint32_t index) __attribute__ ((weak));
      void gen10_CmdDrawIndirectByteCountEXT(VkCommandBuffer commandBuffer, uint32_t instanceCount, uint32_t firstInstance, VkBuffer counterBuffer, VkDeviceSize counterBufferOffset, uint32_t counterOffset, uint32_t vertexStride) __attribute__ ((weak));
      VkDeviceAddress gen10_GetBufferDeviceAddressEXT(VkDevice device, const VkBufferDeviceAddressInfoEXT* pInfo) __attribute__ ((weak));
      VkResult gen10_GetPipelineExecutablePropertiesKHR(VkDevice                        device, const VkPipelineInfoKHR*        pPipelineInfo, uint32_t* pExecutableCount, VkPipelineExecutablePropertiesKHR* pProperties) __attribute__ ((weak));
      VkResult gen10_GetPipelineExecutableStatisticsKHR(VkDevice                        device, const VkPipelineExecutableInfoKHR*  pExecutableInfo, uint32_t* pStatisticCount, VkPipelineExecutableStatisticKHR* pStatistics) __attribute__ ((weak));
      VkResult gen10_GetPipelineExecutableInternalRepresentationsKHR(VkDevice                        device, const VkPipelineExecutableInfoKHR*  pExecutableInfo, uint32_t* pInternalRepresentationCount, VkPipelineExecutableInternalRepresentationKHR* pInternalRepresentations) __attribute__ ((weak));
      void gen10_CmdSetLineStippleEXT(VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern) __attribute__ ((weak));
      VkResult gen10_CreateDmaBufImageINTEL(VkDevice device, const VkDmaBufImageCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMem, VkImage* pImage) __attribute__ ((weak));

  const struct anv_device_dispatch_table gen10_device_dispatch_table = {
    .vkGetDeviceProcAddr = gen10_GetDeviceProcAddr,
    .vkDestroyDevice = gen10_DestroyDevice,
    .vkGetDeviceQueue = gen10_GetDeviceQueue,
    .vkQueueSubmit = gen10_QueueSubmit,
    .vkQueueWaitIdle = gen10_QueueWaitIdle,
    .vkDeviceWaitIdle = gen10_DeviceWaitIdle,
    .vkAllocateMemory = gen10_AllocateMemory,
    .vkFreeMemory = gen10_FreeMemory,
    .vkMapMemory = gen10_MapMemory,
    .vkUnmapMemory = gen10_UnmapMemory,
    .vkFlushMappedMemoryRanges = gen10_FlushMappedMemoryRanges,
    .vkInvalidateMappedMemoryRanges = gen10_InvalidateMappedMemoryRanges,
    .vkGetDeviceMemoryCommitment = gen10_GetDeviceMemoryCommitment,
    .vkGetBufferMemoryRequirements = gen10_GetBufferMemoryRequirements,
    .vkBindBufferMemory = gen10_BindBufferMemory,
    .vkGetImageMemoryRequirements = gen10_GetImageMemoryRequirements,
    .vkBindImageMemory = gen10_BindImageMemory,
    .vkGetImageSparseMemoryRequirements = gen10_GetImageSparseMemoryRequirements,
    .vkQueueBindSparse = gen10_QueueBindSparse,
    .vkCreateFence = gen10_CreateFence,
    .vkDestroyFence = gen10_DestroyFence,
    .vkResetFences = gen10_ResetFences,
    .vkGetFenceStatus = gen10_GetFenceStatus,
    .vkWaitForFences = gen10_WaitForFences,
    .vkCreateSemaphore = gen10_CreateSemaphore,
    .vkDestroySemaphore = gen10_DestroySemaphore,
    .vkCreateEvent = gen10_CreateEvent,
    .vkDestroyEvent = gen10_DestroyEvent,
    .vkGetEventStatus = gen10_GetEventStatus,
    .vkSetEvent = gen10_SetEvent,
    .vkResetEvent = gen10_ResetEvent,
    .vkCreateQueryPool = gen10_CreateQueryPool,
    .vkDestroyQueryPool = gen10_DestroyQueryPool,
    .vkGetQueryPoolResults = gen10_GetQueryPoolResults,
    .vkResetQueryPoolEXT = gen10_ResetQueryPoolEXT,
    .vkCreateBuffer = gen10_CreateBuffer,
    .vkDestroyBuffer = gen10_DestroyBuffer,
    .vkCreateBufferView = gen10_CreateBufferView,
    .vkDestroyBufferView = gen10_DestroyBufferView,
    .vkCreateImage = gen10_CreateImage,
    .vkDestroyImage = gen10_DestroyImage,
    .vkGetImageSubresourceLayout = gen10_GetImageSubresourceLayout,
    .vkCreateImageView = gen10_CreateImageView,
    .vkDestroyImageView = gen10_DestroyImageView,
    .vkCreateShaderModule = gen10_CreateShaderModule,
    .vkDestroyShaderModule = gen10_DestroyShaderModule,
    .vkCreatePipelineCache = gen10_CreatePipelineCache,
    .vkDestroyPipelineCache = gen10_DestroyPipelineCache,
    .vkGetPipelineCacheData = gen10_GetPipelineCacheData,
    .vkMergePipelineCaches = gen10_MergePipelineCaches,
    .vkCreateGraphicsPipelines = gen10_CreateGraphicsPipelines,
    .vkCreateComputePipelines = gen10_CreateComputePipelines,
    .vkDestroyPipeline = gen10_DestroyPipeline,
    .vkCreatePipelineLayout = gen10_CreatePipelineLayout,
    .vkDestroyPipelineLayout = gen10_DestroyPipelineLayout,
    .vkCreateSampler = gen10_CreateSampler,
    .vkDestroySampler = gen10_DestroySampler,
    .vkCreateDescriptorSetLayout = gen10_CreateDescriptorSetLayout,
    .vkDestroyDescriptorSetLayout = gen10_DestroyDescriptorSetLayout,
    .vkCreateDescriptorPool = gen10_CreateDescriptorPool,
    .vkDestroyDescriptorPool = gen10_DestroyDescriptorPool,
    .vkResetDescriptorPool = gen10_ResetDescriptorPool,
    .vkAllocateDescriptorSets = gen10_AllocateDescriptorSets,
    .vkFreeDescriptorSets = gen10_FreeDescriptorSets,
    .vkUpdateDescriptorSets = gen10_UpdateDescriptorSets,
    .vkCreateFramebuffer = gen10_CreateFramebuffer,
    .vkDestroyFramebuffer = gen10_DestroyFramebuffer,
    .vkCreateRenderPass = gen10_CreateRenderPass,
    .vkDestroyRenderPass = gen10_DestroyRenderPass,
    .vkGetRenderAreaGranularity = gen10_GetRenderAreaGranularity,
    .vkCreateCommandPool = gen10_CreateCommandPool,
    .vkDestroyCommandPool = gen10_DestroyCommandPool,
    .vkResetCommandPool = gen10_ResetCommandPool,
    .vkAllocateCommandBuffers = gen10_AllocateCommandBuffers,
    .vkFreeCommandBuffers = gen10_FreeCommandBuffers,
    .vkBeginCommandBuffer = gen10_BeginCommandBuffer,
    .vkEndCommandBuffer = gen10_EndCommandBuffer,
    .vkResetCommandBuffer = gen10_ResetCommandBuffer,
    .vkCmdBindPipeline = gen10_CmdBindPipeline,
    .vkCmdSetViewport = gen10_CmdSetViewport,
    .vkCmdSetScissor = gen10_CmdSetScissor,
    .vkCmdSetLineWidth = gen10_CmdSetLineWidth,
    .vkCmdSetDepthBias = gen10_CmdSetDepthBias,
    .vkCmdSetBlendConstants = gen10_CmdSetBlendConstants,
    .vkCmdSetDepthBounds = gen10_CmdSetDepthBounds,
    .vkCmdSetStencilCompareMask = gen10_CmdSetStencilCompareMask,
    .vkCmdSetStencilWriteMask = gen10_CmdSetStencilWriteMask,
    .vkCmdSetStencilReference = gen10_CmdSetStencilReference,
    .vkCmdBindDescriptorSets = gen10_CmdBindDescriptorSets,
    .vkCmdBindIndexBuffer = gen10_CmdBindIndexBuffer,
    .vkCmdBindVertexBuffers = gen10_CmdBindVertexBuffers,
    .vkCmdDraw = gen10_CmdDraw,
    .vkCmdDrawIndexed = gen10_CmdDrawIndexed,
    .vkCmdDrawIndirect = gen10_CmdDrawIndirect,
    .vkCmdDrawIndexedIndirect = gen10_CmdDrawIndexedIndirect,
    .vkCmdDispatch = gen10_CmdDispatch,
    .vkCmdDispatchIndirect = gen10_CmdDispatchIndirect,
    .vkCmdCopyBuffer = gen10_CmdCopyBuffer,
    .vkCmdCopyImage = gen10_CmdCopyImage,
    .vkCmdBlitImage = gen10_CmdBlitImage,
    .vkCmdCopyBufferToImage = gen10_CmdCopyBufferToImage,
    .vkCmdCopyImageToBuffer = gen10_CmdCopyImageToBuffer,
    .vkCmdUpdateBuffer = gen10_CmdUpdateBuffer,
    .vkCmdFillBuffer = gen10_CmdFillBuffer,
    .vkCmdClearColorImage = gen10_CmdClearColorImage,
    .vkCmdClearDepthStencilImage = gen10_CmdClearDepthStencilImage,
    .vkCmdClearAttachments = gen10_CmdClearAttachments,
    .vkCmdResolveImage = gen10_CmdResolveImage,
    .vkCmdSetEvent = gen10_CmdSetEvent,
    .vkCmdResetEvent = gen10_CmdResetEvent,
    .vkCmdWaitEvents = gen10_CmdWaitEvents,
    .vkCmdPipelineBarrier = gen10_CmdPipelineBarrier,
    .vkCmdBeginQuery = gen10_CmdBeginQuery,
    .vkCmdEndQuery = gen10_CmdEndQuery,
    .vkCmdBeginConditionalRenderingEXT = gen10_CmdBeginConditionalRenderingEXT,
    .vkCmdEndConditionalRenderingEXT = gen10_CmdEndConditionalRenderingEXT,
    .vkCmdResetQueryPool = gen10_CmdResetQueryPool,
    .vkCmdWriteTimestamp = gen10_CmdWriteTimestamp,
    .vkCmdCopyQueryPoolResults = gen10_CmdCopyQueryPoolResults,
    .vkCmdPushConstants = gen10_CmdPushConstants,
    .vkCmdBeginRenderPass = gen10_CmdBeginRenderPass,
    .vkCmdNextSubpass = gen10_CmdNextSubpass,
    .vkCmdEndRenderPass = gen10_CmdEndRenderPass,
    .vkCmdExecuteCommands = gen10_CmdExecuteCommands,
    .vkCreateSwapchainKHR = gen10_CreateSwapchainKHR,
    .vkDestroySwapchainKHR = gen10_DestroySwapchainKHR,
    .vkGetSwapchainImagesKHR = gen10_GetSwapchainImagesKHR,
    .vkAcquireNextImageKHR = gen10_AcquireNextImageKHR,
    .vkQueuePresentKHR = gen10_QueuePresentKHR,
    .vkCmdPushDescriptorSetKHR = gen10_CmdPushDescriptorSetKHR,
    .vkTrimCommandPool = gen10_TrimCommandPool,
    .vkTrimCommandPoolKHR = gen10_TrimCommandPool,
    .vkGetMemoryFdKHR = gen10_GetMemoryFdKHR,
    .vkGetMemoryFdPropertiesKHR = gen10_GetMemoryFdPropertiesKHR,
    .vkGetSemaphoreFdKHR = gen10_GetSemaphoreFdKHR,
    .vkImportSemaphoreFdKHR = gen10_ImportSemaphoreFdKHR,
    .vkGetFenceFdKHR = gen10_GetFenceFdKHR,
    .vkImportFenceFdKHR = gen10_ImportFenceFdKHR,
    .vkDisplayPowerControlEXT = gen10_DisplayPowerControlEXT,
    .vkRegisterDeviceEventEXT = gen10_RegisterDeviceEventEXT,
    .vkRegisterDisplayEventEXT = gen10_RegisterDisplayEventEXT,
    .vkGetSwapchainCounterEXT = gen10_GetSwapchainCounterEXT,
    .vkGetDeviceGroupPeerMemoryFeatures = gen10_GetDeviceGroupPeerMemoryFeatures,
    .vkGetDeviceGroupPeerMemoryFeaturesKHR = gen10_GetDeviceGroupPeerMemoryFeatures,
    .vkBindBufferMemory2 = gen10_BindBufferMemory2,
    .vkBindBufferMemory2KHR = gen10_BindBufferMemory2,
    .vkBindImageMemory2 = gen10_BindImageMemory2,
    .vkBindImageMemory2KHR = gen10_BindImageMemory2,
    .vkCmdSetDeviceMask = gen10_CmdSetDeviceMask,
    .vkCmdSetDeviceMaskKHR = gen10_CmdSetDeviceMask,
    .vkGetDeviceGroupPresentCapabilitiesKHR = gen10_GetDeviceGroupPresentCapabilitiesKHR,
    .vkGetDeviceGroupSurfacePresentModesKHR = gen10_GetDeviceGroupSurfacePresentModesKHR,
    .vkAcquireNextImage2KHR = gen10_AcquireNextImage2KHR,
    .vkCmdDispatchBase = gen10_CmdDispatchBase,
    .vkCmdDispatchBaseKHR = gen10_CmdDispatchBase,
    .vkCreateDescriptorUpdateTemplate = gen10_CreateDescriptorUpdateTemplate,
    .vkCreateDescriptorUpdateTemplateKHR = gen10_CreateDescriptorUpdateTemplate,
    .vkDestroyDescriptorUpdateTemplate = gen10_DestroyDescriptorUpdateTemplate,
    .vkDestroyDescriptorUpdateTemplateKHR = gen10_DestroyDescriptorUpdateTemplate,
    .vkUpdateDescriptorSetWithTemplate = gen10_UpdateDescriptorSetWithTemplate,
    .vkUpdateDescriptorSetWithTemplateKHR = gen10_UpdateDescriptorSetWithTemplate,
    .vkCmdPushDescriptorSetWithTemplateKHR = gen10_CmdPushDescriptorSetWithTemplateKHR,
    .vkGetBufferMemoryRequirements2 = gen10_GetBufferMemoryRequirements2,
    .vkGetBufferMemoryRequirements2KHR = gen10_GetBufferMemoryRequirements2,
    .vkGetImageMemoryRequirements2 = gen10_GetImageMemoryRequirements2,
    .vkGetImageMemoryRequirements2KHR = gen10_GetImageMemoryRequirements2,
    .vkGetImageSparseMemoryRequirements2 = gen10_GetImageSparseMemoryRequirements2,
    .vkGetImageSparseMemoryRequirements2KHR = gen10_GetImageSparseMemoryRequirements2,
    .vkCreateSamplerYcbcrConversion = gen10_CreateSamplerYcbcrConversion,
    .vkCreateSamplerYcbcrConversionKHR = gen10_CreateSamplerYcbcrConversion,
    .vkDestroySamplerYcbcrConversion = gen10_DestroySamplerYcbcrConversion,
    .vkDestroySamplerYcbcrConversionKHR = gen10_DestroySamplerYcbcrConversion,
    .vkGetDeviceQueue2 = gen10_GetDeviceQueue2,
    .vkGetDescriptorSetLayoutSupport = gen10_GetDescriptorSetLayoutSupport,
    .vkGetDescriptorSetLayoutSupportKHR = gen10_GetDescriptorSetLayoutSupport,
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .vkGetSwapchainGrallocUsageANDROID = gen10_GetSwapchainGrallocUsageANDROID,
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .vkGetSwapchainGrallocUsage2ANDROID = gen10_GetSwapchainGrallocUsage2ANDROID,
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .vkAcquireImageANDROID = gen10_AcquireImageANDROID,
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .vkQueueSignalReleaseImageANDROID = gen10_QueueSignalReleaseImageANDROID,
#endif // VK_USE_PLATFORM_ANDROID_KHR
    .vkGetCalibratedTimestampsEXT = gen10_GetCalibratedTimestampsEXT,
    .vkGetMemoryHostPointerPropertiesEXT = gen10_GetMemoryHostPointerPropertiesEXT,
    .vkCreateRenderPass2KHR = gen10_CreateRenderPass2KHR,
    .vkCmdBeginRenderPass2KHR = gen10_CmdBeginRenderPass2KHR,
    .vkCmdNextSubpass2KHR = gen10_CmdNextSubpass2KHR,
    .vkCmdEndRenderPass2KHR = gen10_CmdEndRenderPass2KHR,
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .vkGetAndroidHardwareBufferPropertiesANDROID = gen10_GetAndroidHardwareBufferPropertiesANDROID,
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .vkGetMemoryAndroidHardwareBufferANDROID = gen10_GetMemoryAndroidHardwareBufferANDROID,
#endif // VK_USE_PLATFORM_ANDROID_KHR
    .vkCmdDrawIndirectCountKHR = gen10_CmdDrawIndirectCountKHR,
    .vkCmdDrawIndexedIndirectCountKHR = gen10_CmdDrawIndexedIndirectCountKHR,
    .vkCmdBindTransformFeedbackBuffersEXT = gen10_CmdBindTransformFeedbackBuffersEXT,
    .vkCmdBeginTransformFeedbackEXT = gen10_CmdBeginTransformFeedbackEXT,
    .vkCmdEndTransformFeedbackEXT = gen10_CmdEndTransformFeedbackEXT,
    .vkCmdBeginQueryIndexedEXT = gen10_CmdBeginQueryIndexedEXT,
    .vkCmdEndQueryIndexedEXT = gen10_CmdEndQueryIndexedEXT,
    .vkCmdDrawIndirectByteCountEXT = gen10_CmdDrawIndirectByteCountEXT,
    .vkGetBufferDeviceAddressEXT = gen10_GetBufferDeviceAddressEXT,
    .vkGetPipelineExecutablePropertiesKHR = gen10_GetPipelineExecutablePropertiesKHR,
    .vkGetPipelineExecutableStatisticsKHR = gen10_GetPipelineExecutableStatisticsKHR,
    .vkGetPipelineExecutableInternalRepresentationsKHR = gen10_GetPipelineExecutableInternalRepresentationsKHR,
    .vkCmdSetLineStippleEXT = gen10_CmdSetLineStippleEXT,
    .vkCreateDmaBufImageINTEL = gen10_CreateDmaBufImageINTEL,
  };
      PFN_vkVoidFunction gen11_GetDeviceProcAddr(VkDevice device, const char* pName) __attribute__ ((weak));
      void gen11_DestroyDevice(VkDevice device, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      void gen11_GetDeviceQueue(VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue) __attribute__ ((weak));
      VkResult gen11_QueueSubmit(VkQueue queue, uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence) __attribute__ ((weak));
      VkResult gen11_QueueWaitIdle(VkQueue queue) __attribute__ ((weak));
      VkResult gen11_DeviceWaitIdle(VkDevice device) __attribute__ ((weak));
      VkResult gen11_AllocateMemory(VkDevice device, const VkMemoryAllocateInfo* pAllocateInfo, const VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory) __attribute__ ((weak));
      void gen11_FreeMemory(VkDevice device, VkDeviceMemory memory, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen11_MapMemory(VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData) __attribute__ ((weak));
      void gen11_UnmapMemory(VkDevice device, VkDeviceMemory memory) __attribute__ ((weak));
      VkResult gen11_FlushMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges) __attribute__ ((weak));
      VkResult gen11_InvalidateMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges) __attribute__ ((weak));
      void gen11_GetDeviceMemoryCommitment(VkDevice device, VkDeviceMemory memory, VkDeviceSize* pCommittedMemoryInBytes) __attribute__ ((weak));
      void gen11_GetBufferMemoryRequirements(VkDevice device, VkBuffer buffer, VkMemoryRequirements* pMemoryRequirements) __attribute__ ((weak));
      VkResult gen11_BindBufferMemory(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset) __attribute__ ((weak));
      void gen11_GetImageMemoryRequirements(VkDevice device, VkImage image, VkMemoryRequirements* pMemoryRequirements) __attribute__ ((weak));
      VkResult gen11_BindImageMemory(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset) __attribute__ ((weak));
      void gen11_GetImageSparseMemoryRequirements(VkDevice device, VkImage image, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements* pSparseMemoryRequirements) __attribute__ ((weak));
      VkResult gen11_QueueBindSparse(VkQueue queue, uint32_t bindInfoCount, const VkBindSparseInfo* pBindInfo, VkFence fence) __attribute__ ((weak));
      VkResult gen11_CreateFence(VkDevice device, const VkFenceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) __attribute__ ((weak));
      void gen11_DestroyFence(VkDevice device, VkFence fence, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen11_ResetFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences) __attribute__ ((weak));
      VkResult gen11_GetFenceStatus(VkDevice device, VkFence fence) __attribute__ ((weak));
      VkResult gen11_WaitForFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences, VkBool32 waitAll, uint64_t timeout) __attribute__ ((weak));
      VkResult gen11_CreateSemaphore(VkDevice device, const VkSemaphoreCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore) __attribute__ ((weak));
      void gen11_DestroySemaphore(VkDevice device, VkSemaphore semaphore, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen11_CreateEvent(VkDevice device, const VkEventCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkEvent* pEvent) __attribute__ ((weak));
      void gen11_DestroyEvent(VkDevice device, VkEvent event, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen11_GetEventStatus(VkDevice device, VkEvent event) __attribute__ ((weak));
      VkResult gen11_SetEvent(VkDevice device, VkEvent event) __attribute__ ((weak));
      VkResult gen11_ResetEvent(VkDevice device, VkEvent event) __attribute__ ((weak));
      VkResult gen11_CreateQueryPool(VkDevice device, const VkQueryPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkQueryPool* pQueryPool) __attribute__ ((weak));
      void gen11_DestroyQueryPool(VkDevice device, VkQueryPool queryPool, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen11_GetQueryPoolResults(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, VkDeviceSize stride, VkQueryResultFlags flags) __attribute__ ((weak));
      void gen11_ResetQueryPoolEXT(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) __attribute__ ((weak));
      VkResult gen11_CreateBuffer(VkDevice device, const VkBufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer) __attribute__ ((weak));
      void gen11_DestroyBuffer(VkDevice device, VkBuffer buffer, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen11_CreateBufferView(VkDevice device, const VkBufferViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferView* pView) __attribute__ ((weak));
      void gen11_DestroyBufferView(VkDevice device, VkBufferView bufferView, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen11_CreateImage(VkDevice device, const VkImageCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImage* pImage) __attribute__ ((weak));
      void gen11_DestroyImage(VkDevice device, VkImage image, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      void gen11_GetImageSubresourceLayout(VkDevice device, VkImage image, const VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout) __attribute__ ((weak));
      VkResult gen11_CreateImageView(VkDevice device, const VkImageViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImageView* pView) __attribute__ ((weak));
      void gen11_DestroyImageView(VkDevice device, VkImageView imageView, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen11_CreateShaderModule(VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule) __attribute__ ((weak));
      void gen11_DestroyShaderModule(VkDevice device, VkShaderModule shaderModule, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen11_CreatePipelineCache(VkDevice device, const VkPipelineCacheCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineCache* pPipelineCache) __attribute__ ((weak));
      void gen11_DestroyPipelineCache(VkDevice device, VkPipelineCache pipelineCache, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen11_GetPipelineCacheData(VkDevice device, VkPipelineCache pipelineCache, size_t* pDataSize, void* pData) __attribute__ ((weak));
      VkResult gen11_MergePipelineCaches(VkDevice device, VkPipelineCache dstCache, uint32_t srcCacheCount, const VkPipelineCache* pSrcCaches) __attribute__ ((weak));
      VkResult gen11_CreateGraphicsPipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkGraphicsPipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) __attribute__ ((weak));
      VkResult gen11_CreateComputePipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkComputePipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) __attribute__ ((weak));
      void gen11_DestroyPipeline(VkDevice device, VkPipeline pipeline, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen11_CreatePipelineLayout(VkDevice device, const VkPipelineLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout) __attribute__ ((weak));
      void gen11_DestroyPipelineLayout(VkDevice device, VkPipelineLayout pipelineLayout, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen11_CreateSampler(VkDevice device, const VkSamplerCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSampler* pSampler) __attribute__ ((weak));
      void gen11_DestroySampler(VkDevice device, VkSampler sampler, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen11_CreateDescriptorSetLayout(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorSetLayout* pSetLayout) __attribute__ ((weak));
      void gen11_DestroyDescriptorSetLayout(VkDevice device, VkDescriptorSetLayout descriptorSetLayout, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen11_CreateDescriptorPool(VkDevice device, const VkDescriptorPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorPool* pDescriptorPool) __attribute__ ((weak));
      void gen11_DestroyDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen11_ResetDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags flags) __attribute__ ((weak));
      VkResult gen11_AllocateDescriptorSets(VkDevice device, const VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets) __attribute__ ((weak));
      VkResult gen11_FreeDescriptorSets(VkDevice device, VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets) __attribute__ ((weak));
      void gen11_UpdateDescriptorSets(VkDevice device, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, const VkCopyDescriptorSet* pDescriptorCopies) __attribute__ ((weak));
      VkResult gen11_CreateFramebuffer(VkDevice device, const VkFramebufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFramebuffer* pFramebuffer) __attribute__ ((weak));
      void gen11_DestroyFramebuffer(VkDevice device, VkFramebuffer framebuffer, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen11_CreateRenderPass(VkDevice device, const VkRenderPassCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) __attribute__ ((weak));
      void gen11_DestroyRenderPass(VkDevice device, VkRenderPass renderPass, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      void gen11_GetRenderAreaGranularity(VkDevice device, VkRenderPass renderPass, VkExtent2D* pGranularity) __attribute__ ((weak));
      VkResult gen11_CreateCommandPool(VkDevice device, const VkCommandPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool) __attribute__ ((weak));
      void gen11_DestroyCommandPool(VkDevice device, VkCommandPool commandPool, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen11_ResetCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolResetFlags flags) __attribute__ ((weak));
      VkResult gen11_AllocateCommandBuffers(VkDevice device, const VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers) __attribute__ ((weak));
      void gen11_FreeCommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) __attribute__ ((weak));
      VkResult gen11_BeginCommandBuffer(VkCommandBuffer commandBuffer, const VkCommandBufferBeginInfo* pBeginInfo) __attribute__ ((weak));
      VkResult gen11_EndCommandBuffer(VkCommandBuffer commandBuffer) __attribute__ ((weak));
      VkResult gen11_ResetCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags) __attribute__ ((weak));
      void gen11_CmdBindPipeline(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline) __attribute__ ((weak));
      void gen11_CmdSetViewport(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pViewports) __attribute__ ((weak));
      void gen11_CmdSetScissor(VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pScissors) __attribute__ ((weak));
      void gen11_CmdSetLineWidth(VkCommandBuffer commandBuffer, float lineWidth) __attribute__ ((weak));
      void gen11_CmdSetDepthBias(VkCommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor) __attribute__ ((weak));
      void gen11_CmdSetBlendConstants(VkCommandBuffer commandBuffer, const float blendConstants[4]) __attribute__ ((weak));
      void gen11_CmdSetDepthBounds(VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds) __attribute__ ((weak));
      void gen11_CmdSetStencilCompareMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask) __attribute__ ((weak));
      void gen11_CmdSetStencilWriteMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask) __attribute__ ((weak));
      void gen11_CmdSetStencilReference(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference) __attribute__ ((weak));
      void gen11_CmdBindDescriptorSets(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets) __attribute__ ((weak));
      void gen11_CmdBindIndexBuffer(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType) __attribute__ ((weak));
      void gen11_CmdBindVertexBuffers(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets) __attribute__ ((weak));
      void gen11_CmdDraw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) __attribute__ ((weak));
      void gen11_CmdDrawIndexed(VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) __attribute__ ((weak));
      void gen11_CmdDrawIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) __attribute__ ((weak));
      void gen11_CmdDrawIndexedIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) __attribute__ ((weak));
      void gen11_CmdDispatch(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) __attribute__ ((weak));
      void gen11_CmdDispatchIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset) __attribute__ ((weak));
      void gen11_CmdCopyBuffer(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions) __attribute__ ((weak));
      void gen11_CmdCopyImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions) __attribute__ ((weak));
      void gen11_CmdBlitImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter) __attribute__ ((weak));
      void gen11_CmdCopyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions) __attribute__ ((weak));
      void gen11_CmdCopyImageToBuffer(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions) __attribute__ ((weak));
      void gen11_CmdUpdateBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData) __attribute__ ((weak));
      void gen11_CmdFillBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data) __attribute__ ((weak));
      void gen11_CmdClearColorImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) __attribute__ ((weak));
      void gen11_CmdClearDepthStencilImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) __attribute__ ((weak));
      void gen11_CmdClearAttachments(VkCommandBuffer commandBuffer, uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects) __attribute__ ((weak));
      void gen11_CmdResolveImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageResolve* pRegions) __attribute__ ((weak));
      void gen11_CmdSetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) __attribute__ ((weak));
      void gen11_CmdResetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) __attribute__ ((weak));
      void gen11_CmdWaitEvents(VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) __attribute__ ((weak));
      void gen11_CmdPipelineBarrier(VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) __attribute__ ((weak));
      void gen11_CmdBeginQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags) __attribute__ ((weak));
      void gen11_CmdEndQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query) __attribute__ ((weak));
      void gen11_CmdBeginConditionalRenderingEXT(VkCommandBuffer commandBuffer, const VkConditionalRenderingBeginInfoEXT* pConditionalRenderingBegin) __attribute__ ((weak));
      void gen11_CmdEndConditionalRenderingEXT(VkCommandBuffer commandBuffer) __attribute__ ((weak));
      void gen11_CmdResetQueryPool(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) __attribute__ ((weak));
      void gen11_CmdWriteTimestamp(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query) __attribute__ ((weak));
      void gen11_CmdCopyQueryPoolResults(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags) __attribute__ ((weak));
      void gen11_CmdPushConstants(VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues) __attribute__ ((weak));
      void gen11_CmdBeginRenderPass(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents) __attribute__ ((weak));
      void gen11_CmdNextSubpass(VkCommandBuffer commandBuffer, VkSubpassContents contents) __attribute__ ((weak));
      void gen11_CmdEndRenderPass(VkCommandBuffer commandBuffer) __attribute__ ((weak));
      void gen11_CmdExecuteCommands(VkCommandBuffer commandBuffer, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) __attribute__ ((weak));
      VkResult gen11_CreateSwapchainKHR(VkDevice device, const VkSwapchainCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain) __attribute__ ((weak));
      void gen11_DestroySwapchainKHR(VkDevice device, VkSwapchainKHR swapchain, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
      VkResult gen11_GetSwapchainImagesKHR(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages) __attribute__ ((weak));
      VkResult gen11_AcquireNextImageKHR(VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex) __attribute__ ((weak));
      VkResult gen11_QueuePresentKHR(VkQueue queue, const VkPresentInfoKHR* pPresentInfo) __attribute__ ((weak));
      void gen11_CmdPushDescriptorSetKHR(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites) __attribute__ ((weak));
      void gen11_TrimCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags) __attribute__ ((weak));
            VkResult gen11_GetMemoryFdKHR(VkDevice device, const VkMemoryGetFdInfoKHR* pGetFdInfo, int* pFd) __attribute__ ((weak));
      VkResult gen11_GetMemoryFdPropertiesKHR(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, int fd, VkMemoryFdPropertiesKHR* pMemoryFdProperties) __attribute__ ((weak));
      VkResult gen11_GetSemaphoreFdKHR(VkDevice device, const VkSemaphoreGetFdInfoKHR* pGetFdInfo, int* pFd) __attribute__ ((weak));
      VkResult gen11_ImportSemaphoreFdKHR(VkDevice device, const VkImportSemaphoreFdInfoKHR* pImportSemaphoreFdInfo) __attribute__ ((weak));
      VkResult gen11_GetFenceFdKHR(VkDevice device, const VkFenceGetFdInfoKHR* pGetFdInfo, int* pFd) __attribute__ ((weak));
      VkResult gen11_ImportFenceFdKHR(VkDevice device, const VkImportFenceFdInfoKHR* pImportFenceFdInfo) __attribute__ ((weak));
      VkResult gen11_DisplayPowerControlEXT(VkDevice device, VkDisplayKHR display, const VkDisplayPowerInfoEXT* pDisplayPowerInfo) __attribute__ ((weak));
      VkResult gen11_RegisterDeviceEventEXT(VkDevice device, const VkDeviceEventInfoEXT* pDeviceEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) __attribute__ ((weak));
      VkResult gen11_RegisterDisplayEventEXT(VkDevice device, VkDisplayKHR display, const VkDisplayEventInfoEXT* pDisplayEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) __attribute__ ((weak));
      VkResult gen11_GetSwapchainCounterEXT(VkDevice device, VkSwapchainKHR swapchain, VkSurfaceCounterFlagBitsEXT counter, uint64_t* pCounterValue) __attribute__ ((weak));
      void gen11_GetDeviceGroupPeerMemoryFeatures(VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures) __attribute__ ((weak));
            VkResult gen11_BindBufferMemory2(VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos) __attribute__ ((weak));
            VkResult gen11_BindImageMemory2(VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos) __attribute__ ((weak));
            void gen11_CmdSetDeviceMask(VkCommandBuffer commandBuffer, uint32_t deviceMask) __attribute__ ((weak));
            VkResult gen11_GetDeviceGroupPresentCapabilitiesKHR(VkDevice device, VkDeviceGroupPresentCapabilitiesKHR* pDeviceGroupPresentCapabilities) __attribute__ ((weak));
      VkResult gen11_GetDeviceGroupSurfacePresentModesKHR(VkDevice device, VkSurfaceKHR surface, VkDeviceGroupPresentModeFlagsKHR* pModes) __attribute__ ((weak));
      VkResult gen11_AcquireNextImage2KHR(VkDevice device, const VkAcquireNextImageInfoKHR* pAcquireInfo, uint32_t* pImageIndex) __attribute__ ((weak));
      void gen11_CmdDispatchBase(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) __attribute__ ((weak));
            VkResult gen11_CreateDescriptorUpdateTemplate(VkDevice device, const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate) __attribute__ ((weak));
            void gen11_DestroyDescriptorUpdateTemplate(VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
            void gen11_UpdateDescriptorSetWithTemplate(VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData) __attribute__ ((weak));
            void gen11_CmdPushDescriptorSetWithTemplateKHR(VkCommandBuffer commandBuffer, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, const void* pData) __attribute__ ((weak));
      void gen11_GetBufferMemoryRequirements2(VkDevice device, const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) __attribute__ ((weak));
            void gen11_GetImageMemoryRequirements2(VkDevice device, const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) __attribute__ ((weak));
            void gen11_GetImageSparseMemoryRequirements2(VkDevice device, const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) __attribute__ ((weak));
            VkResult gen11_CreateSamplerYcbcrConversion(VkDevice device, const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion) __attribute__ ((weak));
            void gen11_DestroySamplerYcbcrConversion(VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator) __attribute__ ((weak));
            void gen11_GetDeviceQueue2(VkDevice device, const VkDeviceQueueInfo2* pQueueInfo, VkQueue* pQueue) __attribute__ ((weak));
      void gen11_GetDescriptorSetLayoutSupport(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport) __attribute__ ((weak));
      #ifdef VK_USE_PLATFORM_ANDROID_KHR
      VkResult gen11_GetSwapchainGrallocUsageANDROID(VkDevice device, VkFormat format, VkImageUsageFlags imageUsage, int* grallocUsage) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
      VkResult gen11_GetSwapchainGrallocUsage2ANDROID(VkDevice device, VkFormat format, VkImageUsageFlags imageUsage, VkSwapchainImageUsageFlagsANDROID swapchainImageUsage, uint64_t* grallocConsumerUsage, uint64_t* grallocProducerUsage) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
      VkResult gen11_AcquireImageANDROID(VkDevice device, VkImage image, int nativeFenceFd, VkSemaphore semaphore, VkFence fence) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
      VkResult gen11_QueueSignalReleaseImageANDROID(VkQueue queue, uint32_t waitSemaphoreCount, const VkSemaphore* pWaitSemaphores, VkImage image, int* pNativeFenceFd) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_ANDROID_KHR
      VkResult gen11_GetCalibratedTimestampsEXT(VkDevice device, uint32_t timestampCount, const VkCalibratedTimestampInfoEXT* pTimestampInfos, uint64_t* pTimestamps, uint64_t* pMaxDeviation) __attribute__ ((weak));
      VkResult gen11_GetMemoryHostPointerPropertiesEXT(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, const void* pHostPointer, VkMemoryHostPointerPropertiesEXT* pMemoryHostPointerProperties) __attribute__ ((weak));
      VkResult gen11_CreateRenderPass2KHR(VkDevice device, const VkRenderPassCreateInfo2KHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) __attribute__ ((weak));
      void gen11_CmdBeginRenderPass2KHR(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo*      pRenderPassBegin, const VkSubpassBeginInfoKHR*      pSubpassBeginInfo) __attribute__ ((weak));
      void gen11_CmdNextSubpass2KHR(VkCommandBuffer commandBuffer, const VkSubpassBeginInfoKHR*      pSubpassBeginInfo, const VkSubpassEndInfoKHR*        pSubpassEndInfo) __attribute__ ((weak));
      void gen11_CmdEndRenderPass2KHR(VkCommandBuffer commandBuffer, const VkSubpassEndInfoKHR*        pSubpassEndInfo) __attribute__ ((weak));
#ifdef VK_USE_PLATFORM_ANDROID_KHR
      VkResult gen11_GetAndroidHardwareBufferPropertiesANDROID(VkDevice device, const struct AHardwareBuffer* buffer, VkAndroidHardwareBufferPropertiesANDROID* pProperties) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
      VkResult gen11_GetMemoryAndroidHardwareBufferANDROID(VkDevice device, const VkMemoryGetAndroidHardwareBufferInfoANDROID* pInfo, struct AHardwareBuffer** pBuffer) __attribute__ ((weak));
#endif // VK_USE_PLATFORM_ANDROID_KHR
      void gen11_CmdDrawIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) __attribute__ ((weak));
      void gen11_CmdDrawIndexedIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) __attribute__ ((weak));
      void gen11_CmdBindTransformFeedbackBuffersEXT(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes) __attribute__ ((weak));
      void gen11_CmdBeginTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets) __attribute__ ((weak));
      void gen11_CmdEndTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets) __attribute__ ((weak));
      void gen11_CmdBeginQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags, uint32_t index) __attribute__ ((weak));
      void gen11_CmdEndQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, uint32_t index) __attribute__ ((weak));
      void gen11_CmdDrawIndirectByteCountEXT(VkCommandBuffer commandBuffer, uint32_t instanceCount, uint32_t firstInstance, VkBuffer counterBuffer, VkDeviceSize counterBufferOffset, uint32_t counterOffset, uint32_t vertexStride) __attribute__ ((weak));
      VkDeviceAddress gen11_GetBufferDeviceAddressEXT(VkDevice device, const VkBufferDeviceAddressInfoEXT* pInfo) __attribute__ ((weak));
      VkResult gen11_GetPipelineExecutablePropertiesKHR(VkDevice                        device, const VkPipelineInfoKHR*        pPipelineInfo, uint32_t* pExecutableCount, VkPipelineExecutablePropertiesKHR* pProperties) __attribute__ ((weak));
      VkResult gen11_GetPipelineExecutableStatisticsKHR(VkDevice                        device, const VkPipelineExecutableInfoKHR*  pExecutableInfo, uint32_t* pStatisticCount, VkPipelineExecutableStatisticKHR* pStatistics) __attribute__ ((weak));
      VkResult gen11_GetPipelineExecutableInternalRepresentationsKHR(VkDevice                        device, const VkPipelineExecutableInfoKHR*  pExecutableInfo, uint32_t* pInternalRepresentationCount, VkPipelineExecutableInternalRepresentationKHR* pInternalRepresentations) __attribute__ ((weak));
      void gen11_CmdSetLineStippleEXT(VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern) __attribute__ ((weak));
      VkResult gen11_CreateDmaBufImageINTEL(VkDevice device, const VkDmaBufImageCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMem, VkImage* pImage) __attribute__ ((weak));

  const struct anv_device_dispatch_table gen11_device_dispatch_table = {
    .vkGetDeviceProcAddr = gen11_GetDeviceProcAddr,
    .vkDestroyDevice = gen11_DestroyDevice,
    .vkGetDeviceQueue = gen11_GetDeviceQueue,
    .vkQueueSubmit = gen11_QueueSubmit,
    .vkQueueWaitIdle = gen11_QueueWaitIdle,
    .vkDeviceWaitIdle = gen11_DeviceWaitIdle,
    .vkAllocateMemory = gen11_AllocateMemory,
    .vkFreeMemory = gen11_FreeMemory,
    .vkMapMemory = gen11_MapMemory,
    .vkUnmapMemory = gen11_UnmapMemory,
    .vkFlushMappedMemoryRanges = gen11_FlushMappedMemoryRanges,
    .vkInvalidateMappedMemoryRanges = gen11_InvalidateMappedMemoryRanges,
    .vkGetDeviceMemoryCommitment = gen11_GetDeviceMemoryCommitment,
    .vkGetBufferMemoryRequirements = gen11_GetBufferMemoryRequirements,
    .vkBindBufferMemory = gen11_BindBufferMemory,
    .vkGetImageMemoryRequirements = gen11_GetImageMemoryRequirements,
    .vkBindImageMemory = gen11_BindImageMemory,
    .vkGetImageSparseMemoryRequirements = gen11_GetImageSparseMemoryRequirements,
    .vkQueueBindSparse = gen11_QueueBindSparse,
    .vkCreateFence = gen11_CreateFence,
    .vkDestroyFence = gen11_DestroyFence,
    .vkResetFences = gen11_ResetFences,
    .vkGetFenceStatus = gen11_GetFenceStatus,
    .vkWaitForFences = gen11_WaitForFences,
    .vkCreateSemaphore = gen11_CreateSemaphore,
    .vkDestroySemaphore = gen11_DestroySemaphore,
    .vkCreateEvent = gen11_CreateEvent,
    .vkDestroyEvent = gen11_DestroyEvent,
    .vkGetEventStatus = gen11_GetEventStatus,
    .vkSetEvent = gen11_SetEvent,
    .vkResetEvent = gen11_ResetEvent,
    .vkCreateQueryPool = gen11_CreateQueryPool,
    .vkDestroyQueryPool = gen11_DestroyQueryPool,
    .vkGetQueryPoolResults = gen11_GetQueryPoolResults,
    .vkResetQueryPoolEXT = gen11_ResetQueryPoolEXT,
    .vkCreateBuffer = gen11_CreateBuffer,
    .vkDestroyBuffer = gen11_DestroyBuffer,
    .vkCreateBufferView = gen11_CreateBufferView,
    .vkDestroyBufferView = gen11_DestroyBufferView,
    .vkCreateImage = gen11_CreateImage,
    .vkDestroyImage = gen11_DestroyImage,
    .vkGetImageSubresourceLayout = gen11_GetImageSubresourceLayout,
    .vkCreateImageView = gen11_CreateImageView,
    .vkDestroyImageView = gen11_DestroyImageView,
    .vkCreateShaderModule = gen11_CreateShaderModule,
    .vkDestroyShaderModule = gen11_DestroyShaderModule,
    .vkCreatePipelineCache = gen11_CreatePipelineCache,
    .vkDestroyPipelineCache = gen11_DestroyPipelineCache,
    .vkGetPipelineCacheData = gen11_GetPipelineCacheData,
    .vkMergePipelineCaches = gen11_MergePipelineCaches,
    .vkCreateGraphicsPipelines = gen11_CreateGraphicsPipelines,
    .vkCreateComputePipelines = gen11_CreateComputePipelines,
    .vkDestroyPipeline = gen11_DestroyPipeline,
    .vkCreatePipelineLayout = gen11_CreatePipelineLayout,
    .vkDestroyPipelineLayout = gen11_DestroyPipelineLayout,
    .vkCreateSampler = gen11_CreateSampler,
    .vkDestroySampler = gen11_DestroySampler,
    .vkCreateDescriptorSetLayout = gen11_CreateDescriptorSetLayout,
    .vkDestroyDescriptorSetLayout = gen11_DestroyDescriptorSetLayout,
    .vkCreateDescriptorPool = gen11_CreateDescriptorPool,
    .vkDestroyDescriptorPool = gen11_DestroyDescriptorPool,
    .vkResetDescriptorPool = gen11_ResetDescriptorPool,
    .vkAllocateDescriptorSets = gen11_AllocateDescriptorSets,
    .vkFreeDescriptorSets = gen11_FreeDescriptorSets,
    .vkUpdateDescriptorSets = gen11_UpdateDescriptorSets,
    .vkCreateFramebuffer = gen11_CreateFramebuffer,
    .vkDestroyFramebuffer = gen11_DestroyFramebuffer,
    .vkCreateRenderPass = gen11_CreateRenderPass,
    .vkDestroyRenderPass = gen11_DestroyRenderPass,
    .vkGetRenderAreaGranularity = gen11_GetRenderAreaGranularity,
    .vkCreateCommandPool = gen11_CreateCommandPool,
    .vkDestroyCommandPool = gen11_DestroyCommandPool,
    .vkResetCommandPool = gen11_ResetCommandPool,
    .vkAllocateCommandBuffers = gen11_AllocateCommandBuffers,
    .vkFreeCommandBuffers = gen11_FreeCommandBuffers,
    .vkBeginCommandBuffer = gen11_BeginCommandBuffer,
    .vkEndCommandBuffer = gen11_EndCommandBuffer,
    .vkResetCommandBuffer = gen11_ResetCommandBuffer,
    .vkCmdBindPipeline = gen11_CmdBindPipeline,
    .vkCmdSetViewport = gen11_CmdSetViewport,
    .vkCmdSetScissor = gen11_CmdSetScissor,
    .vkCmdSetLineWidth = gen11_CmdSetLineWidth,
    .vkCmdSetDepthBias = gen11_CmdSetDepthBias,
    .vkCmdSetBlendConstants = gen11_CmdSetBlendConstants,
    .vkCmdSetDepthBounds = gen11_CmdSetDepthBounds,
    .vkCmdSetStencilCompareMask = gen11_CmdSetStencilCompareMask,
    .vkCmdSetStencilWriteMask = gen11_CmdSetStencilWriteMask,
    .vkCmdSetStencilReference = gen11_CmdSetStencilReference,
    .vkCmdBindDescriptorSets = gen11_CmdBindDescriptorSets,
    .vkCmdBindIndexBuffer = gen11_CmdBindIndexBuffer,
    .vkCmdBindVertexBuffers = gen11_CmdBindVertexBuffers,
    .vkCmdDraw = gen11_CmdDraw,
    .vkCmdDrawIndexed = gen11_CmdDrawIndexed,
    .vkCmdDrawIndirect = gen11_CmdDrawIndirect,
    .vkCmdDrawIndexedIndirect = gen11_CmdDrawIndexedIndirect,
    .vkCmdDispatch = gen11_CmdDispatch,
    .vkCmdDispatchIndirect = gen11_CmdDispatchIndirect,
    .vkCmdCopyBuffer = gen11_CmdCopyBuffer,
    .vkCmdCopyImage = gen11_CmdCopyImage,
    .vkCmdBlitImage = gen11_CmdBlitImage,
    .vkCmdCopyBufferToImage = gen11_CmdCopyBufferToImage,
    .vkCmdCopyImageToBuffer = gen11_CmdCopyImageToBuffer,
    .vkCmdUpdateBuffer = gen11_CmdUpdateBuffer,
    .vkCmdFillBuffer = gen11_CmdFillBuffer,
    .vkCmdClearColorImage = gen11_CmdClearColorImage,
    .vkCmdClearDepthStencilImage = gen11_CmdClearDepthStencilImage,
    .vkCmdClearAttachments = gen11_CmdClearAttachments,
    .vkCmdResolveImage = gen11_CmdResolveImage,
    .vkCmdSetEvent = gen11_CmdSetEvent,
    .vkCmdResetEvent = gen11_CmdResetEvent,
    .vkCmdWaitEvents = gen11_CmdWaitEvents,
    .vkCmdPipelineBarrier = gen11_CmdPipelineBarrier,
    .vkCmdBeginQuery = gen11_CmdBeginQuery,
    .vkCmdEndQuery = gen11_CmdEndQuery,
    .vkCmdBeginConditionalRenderingEXT = gen11_CmdBeginConditionalRenderingEXT,
    .vkCmdEndConditionalRenderingEXT = gen11_CmdEndConditionalRenderingEXT,
    .vkCmdResetQueryPool = gen11_CmdResetQueryPool,
    .vkCmdWriteTimestamp = gen11_CmdWriteTimestamp,
    .vkCmdCopyQueryPoolResults = gen11_CmdCopyQueryPoolResults,
    .vkCmdPushConstants = gen11_CmdPushConstants,
    .vkCmdBeginRenderPass = gen11_CmdBeginRenderPass,
    .vkCmdNextSubpass = gen11_CmdNextSubpass,
    .vkCmdEndRenderPass = gen11_CmdEndRenderPass,
    .vkCmdExecuteCommands = gen11_CmdExecuteCommands,
    .vkCreateSwapchainKHR = gen11_CreateSwapchainKHR,
    .vkDestroySwapchainKHR = gen11_DestroySwapchainKHR,
    .vkGetSwapchainImagesKHR = gen11_GetSwapchainImagesKHR,
    .vkAcquireNextImageKHR = gen11_AcquireNextImageKHR,
    .vkQueuePresentKHR = gen11_QueuePresentKHR,
    .vkCmdPushDescriptorSetKHR = gen11_CmdPushDescriptorSetKHR,
    .vkTrimCommandPool = gen11_TrimCommandPool,
    .vkTrimCommandPoolKHR = gen11_TrimCommandPool,
    .vkGetMemoryFdKHR = gen11_GetMemoryFdKHR,
    .vkGetMemoryFdPropertiesKHR = gen11_GetMemoryFdPropertiesKHR,
    .vkGetSemaphoreFdKHR = gen11_GetSemaphoreFdKHR,
    .vkImportSemaphoreFdKHR = gen11_ImportSemaphoreFdKHR,
    .vkGetFenceFdKHR = gen11_GetFenceFdKHR,
    .vkImportFenceFdKHR = gen11_ImportFenceFdKHR,
    .vkDisplayPowerControlEXT = gen11_DisplayPowerControlEXT,
    .vkRegisterDeviceEventEXT = gen11_RegisterDeviceEventEXT,
    .vkRegisterDisplayEventEXT = gen11_RegisterDisplayEventEXT,
    .vkGetSwapchainCounterEXT = gen11_GetSwapchainCounterEXT,
    .vkGetDeviceGroupPeerMemoryFeatures = gen11_GetDeviceGroupPeerMemoryFeatures,
    .vkGetDeviceGroupPeerMemoryFeaturesKHR = gen11_GetDeviceGroupPeerMemoryFeatures,
    .vkBindBufferMemory2 = gen11_BindBufferMemory2,
    .vkBindBufferMemory2KHR = gen11_BindBufferMemory2,
    .vkBindImageMemory2 = gen11_BindImageMemory2,
    .vkBindImageMemory2KHR = gen11_BindImageMemory2,
    .vkCmdSetDeviceMask = gen11_CmdSetDeviceMask,
    .vkCmdSetDeviceMaskKHR = gen11_CmdSetDeviceMask,
    .vkGetDeviceGroupPresentCapabilitiesKHR = gen11_GetDeviceGroupPresentCapabilitiesKHR,
    .vkGetDeviceGroupSurfacePresentModesKHR = gen11_GetDeviceGroupSurfacePresentModesKHR,
    .vkAcquireNextImage2KHR = gen11_AcquireNextImage2KHR,
    .vkCmdDispatchBase = gen11_CmdDispatchBase,
    .vkCmdDispatchBaseKHR = gen11_CmdDispatchBase,
    .vkCreateDescriptorUpdateTemplate = gen11_CreateDescriptorUpdateTemplate,
    .vkCreateDescriptorUpdateTemplateKHR = gen11_CreateDescriptorUpdateTemplate,
    .vkDestroyDescriptorUpdateTemplate = gen11_DestroyDescriptorUpdateTemplate,
    .vkDestroyDescriptorUpdateTemplateKHR = gen11_DestroyDescriptorUpdateTemplate,
    .vkUpdateDescriptorSetWithTemplate = gen11_UpdateDescriptorSetWithTemplate,
    .vkUpdateDescriptorSetWithTemplateKHR = gen11_UpdateDescriptorSetWithTemplate,
    .vkCmdPushDescriptorSetWithTemplateKHR = gen11_CmdPushDescriptorSetWithTemplateKHR,
    .vkGetBufferMemoryRequirements2 = gen11_GetBufferMemoryRequirements2,
    .vkGetBufferMemoryRequirements2KHR = gen11_GetBufferMemoryRequirements2,
    .vkGetImageMemoryRequirements2 = gen11_GetImageMemoryRequirements2,
    .vkGetImageMemoryRequirements2KHR = gen11_GetImageMemoryRequirements2,
    .vkGetImageSparseMemoryRequirements2 = gen11_GetImageSparseMemoryRequirements2,
    .vkGetImageSparseMemoryRequirements2KHR = gen11_GetImageSparseMemoryRequirements2,
    .vkCreateSamplerYcbcrConversion = gen11_CreateSamplerYcbcrConversion,
    .vkCreateSamplerYcbcrConversionKHR = gen11_CreateSamplerYcbcrConversion,
    .vkDestroySamplerYcbcrConversion = gen11_DestroySamplerYcbcrConversion,
    .vkDestroySamplerYcbcrConversionKHR = gen11_DestroySamplerYcbcrConversion,
    .vkGetDeviceQueue2 = gen11_GetDeviceQueue2,
    .vkGetDescriptorSetLayoutSupport = gen11_GetDescriptorSetLayoutSupport,
    .vkGetDescriptorSetLayoutSupportKHR = gen11_GetDescriptorSetLayoutSupport,
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .vkGetSwapchainGrallocUsageANDROID = gen11_GetSwapchainGrallocUsageANDROID,
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .vkGetSwapchainGrallocUsage2ANDROID = gen11_GetSwapchainGrallocUsage2ANDROID,
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .vkAcquireImageANDROID = gen11_AcquireImageANDROID,
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .vkQueueSignalReleaseImageANDROID = gen11_QueueSignalReleaseImageANDROID,
#endif // VK_USE_PLATFORM_ANDROID_KHR
    .vkGetCalibratedTimestampsEXT = gen11_GetCalibratedTimestampsEXT,
    .vkGetMemoryHostPointerPropertiesEXT = gen11_GetMemoryHostPointerPropertiesEXT,
    .vkCreateRenderPass2KHR = gen11_CreateRenderPass2KHR,
    .vkCmdBeginRenderPass2KHR = gen11_CmdBeginRenderPass2KHR,
    .vkCmdNextSubpass2KHR = gen11_CmdNextSubpass2KHR,
    .vkCmdEndRenderPass2KHR = gen11_CmdEndRenderPass2KHR,
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .vkGetAndroidHardwareBufferPropertiesANDROID = gen11_GetAndroidHardwareBufferPropertiesANDROID,
#endif // VK_USE_PLATFORM_ANDROID_KHR
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    .vkGetMemoryAndroidHardwareBufferANDROID = gen11_GetMemoryAndroidHardwareBufferANDROID,
#endif // VK_USE_PLATFORM_ANDROID_KHR
    .vkCmdDrawIndirectCountKHR = gen11_CmdDrawIndirectCountKHR,
    .vkCmdDrawIndexedIndirectCountKHR = gen11_CmdDrawIndexedIndirectCountKHR,
    .vkCmdBindTransformFeedbackBuffersEXT = gen11_CmdBindTransformFeedbackBuffersEXT,
    .vkCmdBeginTransformFeedbackEXT = gen11_CmdBeginTransformFeedbackEXT,
    .vkCmdEndTransformFeedbackEXT = gen11_CmdEndTransformFeedbackEXT,
    .vkCmdBeginQueryIndexedEXT = gen11_CmdBeginQueryIndexedEXT,
    .vkCmdEndQueryIndexedEXT = gen11_CmdEndQueryIndexedEXT,
    .vkCmdDrawIndirectByteCountEXT = gen11_CmdDrawIndirectByteCountEXT,
    .vkGetBufferDeviceAddressEXT = gen11_GetBufferDeviceAddressEXT,
    .vkGetPipelineExecutablePropertiesKHR = gen11_GetPipelineExecutablePropertiesKHR,
    .vkGetPipelineExecutableStatisticsKHR = gen11_GetPipelineExecutableStatisticsKHR,
    .vkGetPipelineExecutableInternalRepresentationsKHR = gen11_GetPipelineExecutableInternalRepresentationsKHR,
    .vkCmdSetLineStippleEXT = gen11_CmdSetLineStippleEXT,
    .vkCreateDmaBufImageINTEL = gen11_CreateDmaBufImageINTEL,
  };


/** Return true if the core version or extension in which the given entrypoint
 * is defined is enabled.
 *
 * If device is NULL, all device extensions are considered enabled.
 */
bool
anv_instance_entrypoint_is_enabled(int index, uint32_t core_version,
                                   const struct anv_instance_extension_table *instance)
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
      /* vkGetPhysicalDeviceProperties */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 5:
      /* vkGetPhysicalDeviceQueueFamilyProperties */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 6:
      /* vkGetPhysicalDeviceMemoryProperties */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 7:
      /* vkGetPhysicalDeviceFeatures */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 8:
      /* vkGetPhysicalDeviceFormatProperties */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 9:
      /* vkGetPhysicalDeviceImageFormatProperties */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 10:
      /* vkCreateDevice */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 11:
      /* vkEnumerateInstanceVersion */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 12:
      /* vkEnumerateInstanceLayerProperties */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 13:
      /* vkEnumerateInstanceExtensionProperties */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 14:
      /* vkEnumerateDeviceLayerProperties */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 15:
      /* vkEnumerateDeviceExtensionProperties */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 16:
      /* vkGetPhysicalDeviceSparseImageFormatProperties */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 17:
      /* vkGetPhysicalDeviceDisplayPropertiesKHR */
      if (instance->KHR_display) return true;
      return false;
   case 18:
      /* vkGetPhysicalDeviceDisplayPlanePropertiesKHR */
      if (instance->KHR_display) return true;
      return false;
   case 19:
      /* vkGetDisplayPlaneSupportedDisplaysKHR */
      if (instance->KHR_display) return true;
      return false;
   case 20:
      /* vkGetDisplayModePropertiesKHR */
      if (instance->KHR_display) return true;
      return false;
   case 21:
      /* vkCreateDisplayModeKHR */
      if (instance->KHR_display) return true;
      return false;
   case 22:
      /* vkGetDisplayPlaneCapabilitiesKHR */
      if (instance->KHR_display) return true;
      return false;
   case 23:
      /* vkCreateDisplayPlaneSurfaceKHR */
      if (instance->KHR_display) return true;
      return false;
   case 24:
      /* vkDestroySurfaceKHR */
      if (instance->KHR_surface) return true;
      return false;
   case 25:
      /* vkGetPhysicalDeviceSurfaceSupportKHR */
      if (instance->KHR_surface) return true;
      return false;
   case 26:
      /* vkGetPhysicalDeviceSurfaceCapabilitiesKHR */
      if (instance->KHR_surface) return true;
      return false;
   case 27:
      /* vkGetPhysicalDeviceSurfaceFormatsKHR */
      if (instance->KHR_surface) return true;
      return false;
   case 28:
      /* vkGetPhysicalDeviceSurfacePresentModesKHR */
      if (instance->KHR_surface) return true;
      return false;
   case 29:
      /* vkCreateWaylandSurfaceKHR */
      if (instance->KHR_wayland_surface) return true;
      return false;
   case 30:
      /* vkGetPhysicalDeviceWaylandPresentationSupportKHR */
      if (instance->KHR_wayland_surface) return true;
      return false;
   case 31:
      /* vkCreateXlibSurfaceKHR */
      if (instance->KHR_xlib_surface) return true;
      return false;
   case 32:
      /* vkGetPhysicalDeviceXlibPresentationSupportKHR */
      if (instance->KHR_xlib_surface) return true;
      return false;
   case 33:
      /* vkCreateXcbSurfaceKHR */
      if (instance->KHR_xcb_surface) return true;
      return false;
   case 34:
      /* vkGetPhysicalDeviceXcbPresentationSupportKHR */
      if (instance->KHR_xcb_surface) return true;
      return false;
   case 35:
      /* vkCreateDebugReportCallbackEXT */
      if (instance->EXT_debug_report) return true;
      return false;
   case 36:
      /* vkDestroyDebugReportCallbackEXT */
      if (instance->EXT_debug_report) return true;
      return false;
   case 37:
      /* vkDebugReportMessageEXT */
      if (instance->EXT_debug_report) return true;
      return false;
   case 38:
      /* vkGetPhysicalDeviceFeatures2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 39:
      /* vkGetPhysicalDeviceFeatures2KHR */
      if (instance->KHR_get_physical_device_properties2) return true;
      return false;
   case 40:
      /* vkGetPhysicalDeviceProperties2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 41:
      /* vkGetPhysicalDeviceProperties2KHR */
      if (instance->KHR_get_physical_device_properties2) return true;
      return false;
   case 42:
      /* vkGetPhysicalDeviceFormatProperties2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 43:
      /* vkGetPhysicalDeviceFormatProperties2KHR */
      if (instance->KHR_get_physical_device_properties2) return true;
      return false;
   case 44:
      /* vkGetPhysicalDeviceImageFormatProperties2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 45:
      /* vkGetPhysicalDeviceImageFormatProperties2KHR */
      if (instance->KHR_get_physical_device_properties2) return true;
      return false;
   case 46:
      /* vkGetPhysicalDeviceQueueFamilyProperties2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 47:
      /* vkGetPhysicalDeviceQueueFamilyProperties2KHR */
      if (instance->KHR_get_physical_device_properties2) return true;
      return false;
   case 48:
      /* vkGetPhysicalDeviceMemoryProperties2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 49:
      /* vkGetPhysicalDeviceMemoryProperties2KHR */
      if (instance->KHR_get_physical_device_properties2) return true;
      return false;
   case 50:
      /* vkGetPhysicalDeviceSparseImageFormatProperties2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 51:
      /* vkGetPhysicalDeviceSparseImageFormatProperties2KHR */
      if (instance->KHR_get_physical_device_properties2) return true;
      return false;
   case 52:
      /* vkGetPhysicalDeviceExternalBufferProperties */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 53:
      /* vkGetPhysicalDeviceExternalBufferPropertiesKHR */
      if (instance->KHR_external_memory_capabilities) return true;
      return false;
   case 54:
      /* vkGetPhysicalDeviceExternalSemaphoreProperties */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 55:
      /* vkGetPhysicalDeviceExternalSemaphorePropertiesKHR */
      if (instance->KHR_external_semaphore_capabilities) return true;
      return false;
   case 56:
      /* vkGetPhysicalDeviceExternalFenceProperties */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 57:
      /* vkGetPhysicalDeviceExternalFencePropertiesKHR */
      if (instance->KHR_external_fence_capabilities) return true;
      return false;
   case 58:
      /* vkReleaseDisplayEXT */
      if (instance->EXT_direct_mode_display) return true;
      return false;
   case 59:
      /* vkAcquireXlibDisplayEXT */
      if (instance->EXT_acquire_xlib_display) return true;
      return false;
   case 60:
      /* vkGetRandROutputDisplayEXT */
      if (instance->EXT_acquire_xlib_display) return true;
      return false;
   case 61:
      /* vkGetPhysicalDeviceSurfaceCapabilities2EXT */
      if (instance->EXT_display_surface_counter) return true;
      return false;
   case 62:
      /* vkEnumeratePhysicalDeviceGroups */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 63:
      /* vkEnumeratePhysicalDeviceGroupsKHR */
      if (instance->KHR_device_group_creation) return true;
      return false;
   case 64:
      /* vkGetPhysicalDevicePresentRectanglesKHR */
      /* All device extensions are considered enabled at the instance level */
      return true;
      /* All device extensions are considered enabled at the instance level */
      return true;
      return false;
   case 65:
      /* vkGetPhysicalDeviceSurfaceCapabilities2KHR */
      if (instance->KHR_get_surface_capabilities2) return true;
      return false;
   case 66:
      /* vkGetPhysicalDeviceSurfaceFormats2KHR */
      if (instance->KHR_get_surface_capabilities2) return true;
      return false;
   case 67:
      /* vkGetPhysicalDeviceDisplayProperties2KHR */
      if (instance->KHR_get_display_properties2) return true;
      return false;
   case 68:
      /* vkGetPhysicalDeviceDisplayPlaneProperties2KHR */
      if (instance->KHR_get_display_properties2) return true;
      return false;
   case 69:
      /* vkGetDisplayModeProperties2KHR */
      if (instance->KHR_get_display_properties2) return true;
      return false;
   case 70:
      /* vkGetDisplayPlaneCapabilities2KHR */
      if (instance->KHR_get_display_properties2) return true;
      return false;
   case 71:
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
anv_device_entrypoint_is_enabled(int index, uint32_t core_version,
                                 const struct anv_instance_extension_table *instance,
                                 const struct anv_device_extension_table *device)
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
      /* vkResetQueryPoolEXT */
      if (!device || device->EXT_host_query_reset) return true;
      return false;
   case 35:
      /* vkCreateBuffer */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 36:
      /* vkDestroyBuffer */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 37:
      /* vkCreateBufferView */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 38:
      /* vkDestroyBufferView */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 39:
      /* vkCreateImage */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 40:
      /* vkDestroyImage */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 41:
      /* vkGetImageSubresourceLayout */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 42:
      /* vkCreateImageView */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 43:
      /* vkDestroyImageView */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 44:
      /* vkCreateShaderModule */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 45:
      /* vkDestroyShaderModule */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 46:
      /* vkCreatePipelineCache */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 47:
      /* vkDestroyPipelineCache */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 48:
      /* vkGetPipelineCacheData */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 49:
      /* vkMergePipelineCaches */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 50:
      /* vkCreateGraphicsPipelines */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 51:
      /* vkCreateComputePipelines */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 52:
      /* vkDestroyPipeline */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 53:
      /* vkCreatePipelineLayout */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 54:
      /* vkDestroyPipelineLayout */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 55:
      /* vkCreateSampler */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 56:
      /* vkDestroySampler */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 57:
      /* vkCreateDescriptorSetLayout */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 58:
      /* vkDestroyDescriptorSetLayout */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 59:
      /* vkCreateDescriptorPool */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 60:
      /* vkDestroyDescriptorPool */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 61:
      /* vkResetDescriptorPool */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 62:
      /* vkAllocateDescriptorSets */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 63:
      /* vkFreeDescriptorSets */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 64:
      /* vkUpdateDescriptorSets */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 65:
      /* vkCreateFramebuffer */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 66:
      /* vkDestroyFramebuffer */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 67:
      /* vkCreateRenderPass */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 68:
      /* vkDestroyRenderPass */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 69:
      /* vkGetRenderAreaGranularity */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 70:
      /* vkCreateCommandPool */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 71:
      /* vkDestroyCommandPool */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 72:
      /* vkResetCommandPool */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 73:
      /* vkAllocateCommandBuffers */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 74:
      /* vkFreeCommandBuffers */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 75:
      /* vkBeginCommandBuffer */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 76:
      /* vkEndCommandBuffer */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 77:
      /* vkResetCommandBuffer */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 78:
      /* vkCmdBindPipeline */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 79:
      /* vkCmdSetViewport */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 80:
      /* vkCmdSetScissor */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 81:
      /* vkCmdSetLineWidth */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 82:
      /* vkCmdSetDepthBias */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 83:
      /* vkCmdSetBlendConstants */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 84:
      /* vkCmdSetDepthBounds */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 85:
      /* vkCmdSetStencilCompareMask */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 86:
      /* vkCmdSetStencilWriteMask */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 87:
      /* vkCmdSetStencilReference */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 88:
      /* vkCmdBindDescriptorSets */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 89:
      /* vkCmdBindIndexBuffer */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 90:
      /* vkCmdBindVertexBuffers */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 91:
      /* vkCmdDraw */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 92:
      /* vkCmdDrawIndexed */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 93:
      /* vkCmdDrawIndirect */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 94:
      /* vkCmdDrawIndexedIndirect */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 95:
      /* vkCmdDispatch */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 96:
      /* vkCmdDispatchIndirect */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 97:
      /* vkCmdCopyBuffer */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 98:
      /* vkCmdCopyImage */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 99:
      /* vkCmdBlitImage */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 100:
      /* vkCmdCopyBufferToImage */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 101:
      /* vkCmdCopyImageToBuffer */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 102:
      /* vkCmdUpdateBuffer */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 103:
      /* vkCmdFillBuffer */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 104:
      /* vkCmdClearColorImage */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 105:
      /* vkCmdClearDepthStencilImage */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 106:
      /* vkCmdClearAttachments */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 107:
      /* vkCmdResolveImage */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 108:
      /* vkCmdSetEvent */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 109:
      /* vkCmdResetEvent */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 110:
      /* vkCmdWaitEvents */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 111:
      /* vkCmdPipelineBarrier */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 112:
      /* vkCmdBeginQuery */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 113:
      /* vkCmdEndQuery */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 114:
      /* vkCmdBeginConditionalRenderingEXT */
      if (!device || device->EXT_conditional_rendering) return true;
      return false;
   case 115:
      /* vkCmdEndConditionalRenderingEXT */
      if (!device || device->EXT_conditional_rendering) return true;
      return false;
   case 116:
      /* vkCmdResetQueryPool */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 117:
      /* vkCmdWriteTimestamp */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 118:
      /* vkCmdCopyQueryPoolResults */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 119:
      /* vkCmdPushConstants */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 120:
      /* vkCmdBeginRenderPass */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 121:
      /* vkCmdNextSubpass */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 122:
      /* vkCmdEndRenderPass */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 123:
      /* vkCmdExecuteCommands */
      return VK_MAKE_VERSION(1, 0, 0) <= core_version;
   case 124:
      /* vkCreateSwapchainKHR */
      if (!device || device->KHR_swapchain) return true;
      return false;
   case 125:
      /* vkDestroySwapchainKHR */
      if (!device || device->KHR_swapchain) return true;
      return false;
   case 126:
      /* vkGetSwapchainImagesKHR */
      if (!device || device->KHR_swapchain) return true;
      return false;
   case 127:
      /* vkAcquireNextImageKHR */
      if (!device || device->KHR_swapchain) return true;
      return false;
   case 128:
      /* vkQueuePresentKHR */
      if (!device || device->KHR_swapchain) return true;
      return false;
   case 129:
      /* vkCmdPushDescriptorSetKHR */
      if (!device || device->KHR_push_descriptor) return true;
      return false;
   case 130:
      /* vkTrimCommandPool */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 131:
      /* vkTrimCommandPoolKHR */
      if (!device || device->KHR_maintenance1) return true;
      return false;
   case 132:
      /* vkGetMemoryFdKHR */
      if (!device || device->KHR_external_memory_fd) return true;
      return false;
   case 133:
      /* vkGetMemoryFdPropertiesKHR */
      if (!device || device->KHR_external_memory_fd) return true;
      return false;
   case 134:
      /* vkGetSemaphoreFdKHR */
      if (!device || device->KHR_external_semaphore_fd) return true;
      return false;
   case 135:
      /* vkImportSemaphoreFdKHR */
      if (!device || device->KHR_external_semaphore_fd) return true;
      return false;
   case 136:
      /* vkGetFenceFdKHR */
      if (!device || device->KHR_external_fence_fd) return true;
      return false;
   case 137:
      /* vkImportFenceFdKHR */
      if (!device || device->KHR_external_fence_fd) return true;
      return false;
   case 138:
      /* vkDisplayPowerControlEXT */
      if (!device || device->EXT_display_control) return true;
      return false;
   case 139:
      /* vkRegisterDeviceEventEXT */
      if (!device || device->EXT_display_control) return true;
      return false;
   case 140:
      /* vkRegisterDisplayEventEXT */
      if (!device || device->EXT_display_control) return true;
      return false;
   case 141:
      /* vkGetSwapchainCounterEXT */
      if (!device || device->EXT_display_control) return true;
      return false;
   case 142:
      /* vkGetDeviceGroupPeerMemoryFeatures */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 143:
      /* vkGetDeviceGroupPeerMemoryFeaturesKHR */
      if (!device || device->KHR_device_group) return true;
      return false;
   case 144:
      /* vkBindBufferMemory2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 145:
      /* vkBindBufferMemory2KHR */
      if (!device || device->KHR_bind_memory2) return true;
      return false;
   case 146:
      /* vkBindImageMemory2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 147:
      /* vkBindImageMemory2KHR */
      if (!device || device->KHR_bind_memory2) return true;
      return false;
   case 148:
      /* vkCmdSetDeviceMask */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 149:
      /* vkCmdSetDeviceMaskKHR */
      if (!device || device->KHR_device_group) return true;
      return false;
   case 150:
      /* vkGetDeviceGroupPresentCapabilitiesKHR */
      if (!device || device->KHR_swapchain) return true;
      if (!device || device->KHR_device_group) return true;
      return false;
   case 151:
      /* vkGetDeviceGroupSurfacePresentModesKHR */
      if (!device || device->KHR_swapchain) return true;
      if (!device || device->KHR_device_group) return true;
      return false;
   case 152:
      /* vkAcquireNextImage2KHR */
      if (!device || device->KHR_swapchain) return true;
      if (!device || device->KHR_device_group) return true;
      return false;
   case 153:
      /* vkCmdDispatchBase */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 154:
      /* vkCmdDispatchBaseKHR */
      if (!device || device->KHR_device_group) return true;
      return false;
   case 155:
      /* vkCreateDescriptorUpdateTemplate */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 156:
      /* vkCreateDescriptorUpdateTemplateKHR */
      if (!device || device->KHR_descriptor_update_template) return true;
      return false;
   case 157:
      /* vkDestroyDescriptorUpdateTemplate */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 158:
      /* vkDestroyDescriptorUpdateTemplateKHR */
      if (!device || device->KHR_descriptor_update_template) return true;
      return false;
   case 159:
      /* vkUpdateDescriptorSetWithTemplate */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 160:
      /* vkUpdateDescriptorSetWithTemplateKHR */
      if (!device || device->KHR_descriptor_update_template) return true;
      return false;
   case 161:
      /* vkCmdPushDescriptorSetWithTemplateKHR */
      if (!device || device->KHR_push_descriptor) return true;
      if (!device || device->KHR_descriptor_update_template) return true;
      return false;
   case 162:
      /* vkGetBufferMemoryRequirements2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 163:
      /* vkGetBufferMemoryRequirements2KHR */
      if (!device || device->KHR_get_memory_requirements2) return true;
      return false;
   case 164:
      /* vkGetImageMemoryRequirements2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 165:
      /* vkGetImageMemoryRequirements2KHR */
      if (!device || device->KHR_get_memory_requirements2) return true;
      return false;
   case 166:
      /* vkGetImageSparseMemoryRequirements2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 167:
      /* vkGetImageSparseMemoryRequirements2KHR */
      if (!device || device->KHR_get_memory_requirements2) return true;
      return false;
   case 168:
      /* vkCreateSamplerYcbcrConversion */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 169:
      /* vkCreateSamplerYcbcrConversionKHR */
      if (!device || device->KHR_sampler_ycbcr_conversion) return true;
      return false;
   case 170:
      /* vkDestroySamplerYcbcrConversion */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 171:
      /* vkDestroySamplerYcbcrConversionKHR */
      if (!device || device->KHR_sampler_ycbcr_conversion) return true;
      return false;
   case 172:
      /* vkGetDeviceQueue2 */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 173:
      /* vkGetDescriptorSetLayoutSupport */
      return VK_MAKE_VERSION(1, 1, 0) <= core_version;
   case 174:
      /* vkGetDescriptorSetLayoutSupportKHR */
      if (!device || device->KHR_maintenance3) return true;
      return false;
   case 175:
      /* vkGetSwapchainGrallocUsageANDROID */
      if (!device || device->ANDROID_native_buffer) return true;
      return false;
   case 176:
      /* vkGetSwapchainGrallocUsage2ANDROID */
      if (!device || device->ANDROID_native_buffer) return true;
      return false;
   case 177:
      /* vkAcquireImageANDROID */
      if (!device || device->ANDROID_native_buffer) return true;
      return false;
   case 178:
      /* vkQueueSignalReleaseImageANDROID */
      if (!device || device->ANDROID_native_buffer) return true;
      return false;
   case 179:
      /* vkGetCalibratedTimestampsEXT */
      if (!device || device->EXT_calibrated_timestamps) return true;
      return false;
   case 180:
      /* vkGetMemoryHostPointerPropertiesEXT */
      if (!device || device->EXT_external_memory_host) return true;
      return false;
   case 181:
      /* vkCreateRenderPass2KHR */
      if (!device || device->KHR_create_renderpass2) return true;
      return false;
   case 182:
      /* vkCmdBeginRenderPass2KHR */
      if (!device || device->KHR_create_renderpass2) return true;
      return false;
   case 183:
      /* vkCmdNextSubpass2KHR */
      if (!device || device->KHR_create_renderpass2) return true;
      return false;
   case 184:
      /* vkCmdEndRenderPass2KHR */
      if (!device || device->KHR_create_renderpass2) return true;
      return false;
   case 185:
      /* vkGetAndroidHardwareBufferPropertiesANDROID */
      if (!device || device->ANDROID_external_memory_android_hardware_buffer) return true;
      return false;
   case 186:
      /* vkGetMemoryAndroidHardwareBufferANDROID */
      if (!device || device->ANDROID_external_memory_android_hardware_buffer) return true;
      return false;
   case 187:
      /* vkCmdDrawIndirectCountKHR */
      if (!device || device->KHR_draw_indirect_count) return true;
      return false;
   case 188:
      /* vkCmdDrawIndexedIndirectCountKHR */
      if (!device || device->KHR_draw_indirect_count) return true;
      return false;
   case 189:
      /* vkCmdBindTransformFeedbackBuffersEXT */
      if (!device || device->EXT_transform_feedback) return true;
      return false;
   case 190:
      /* vkCmdBeginTransformFeedbackEXT */
      if (!device || device->EXT_transform_feedback) return true;
      return false;
   case 191:
      /* vkCmdEndTransformFeedbackEXT */
      if (!device || device->EXT_transform_feedback) return true;
      return false;
   case 192:
      /* vkCmdBeginQueryIndexedEXT */
      if (!device || device->EXT_transform_feedback) return true;
      return false;
   case 193:
      /* vkCmdEndQueryIndexedEXT */
      if (!device || device->EXT_transform_feedback) return true;
      return false;
   case 194:
      /* vkCmdDrawIndirectByteCountEXT */
      if (!device || device->EXT_transform_feedback) return true;
      return false;
   case 195:
      /* vkGetBufferDeviceAddressEXT */
      if (!device || device->EXT_buffer_device_address) return true;
      return false;
   case 196:
      /* vkGetPipelineExecutablePropertiesKHR */
      if (!device || device->KHR_pipeline_executable_properties) return true;
      return false;
   case 197:
      /* vkGetPipelineExecutableStatisticsKHR */
      if (!device || device->KHR_pipeline_executable_properties) return true;
      return false;
   case 198:
      /* vkGetPipelineExecutableInternalRepresentationsKHR */
      if (!device || device->KHR_pipeline_executable_properties) return true;
      return false;
   case 199:
      /* vkCmdSetLineStippleEXT */
      if (!device || device->EXT_line_rasterization) return true;
      return false;
   case 200:
      /* vkCreateDmaBufImageINTEL */
      return true;
   default:
      return false;
   }
}

int
anv_get_instance_entrypoint_index(const char *name)
{
   return instance_string_map_lookup(name);
}

int
anv_get_device_entrypoint_index(const char *name)
{
   return device_string_map_lookup(name);
}

static void * __attribute__ ((noinline))
anv_resolve_device_entrypoint(const struct gen_device_info *devinfo, uint32_t index)
{
   const struct anv_device_dispatch_table *genX_table;
   switch (devinfo->gen) {
   case 11:
      genX_table = &gen11_device_dispatch_table;
      break;
   case 10:
      genX_table = &gen10_device_dispatch_table;
      break;
   case 9:
      genX_table = &gen9_device_dispatch_table;
      break;
   case 8:
      genX_table = &gen8_device_dispatch_table;
      break;
   case 7:
      if (devinfo->is_haswell)
         genX_table = &gen75_device_dispatch_table;
      else
         genX_table = &gen7_device_dispatch_table;
      break;
   default:
      unreachable("unsupported gen\n");
   }

   if (genX_table->entrypoints[index])
      return genX_table->entrypoints[index];
   else
      return anv_device_dispatch_table.entrypoints[index];
}

void *
anv_lookup_entrypoint(const struct gen_device_info *devinfo, const char *name)
{
   int idx = anv_get_instance_entrypoint_index(name);
   if (idx >= 0)
      return anv_instance_dispatch_table.entrypoints[idx];

   idx = anv_get_device_entrypoint_index(name);
   if (idx >= 0)
      return anv_resolve_device_entrypoint(devinfo, idx);

   return NULL;
}