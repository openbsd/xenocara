/*
 * Copyright 2024 Intel Corporation
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


/* DO NOT EDIT - This file generated automatically by intel_device_info_gen_h.py script */

#ifndef INTEL_DEVICE_INFO_GEN_H
#define INTEL_DEVICE_INFO_GEN_H

#include <stdbool.h>
#include <stdint.h>

#include "util/macros.h"
#include "compiler/shader_enums.h"
#include "intel/dev/intel_wa.h"

#ifdef __cplusplus
extern "C" {
#endif

#define INTEL_DEVICE_MAX_NAME_SIZE (64)
#define INTEL_DEVICE_MAX_SLICES (8)
#define INTEL_DEVICE_MAX_SUBSLICES (8) /* Maximum on gfx11 */
#define INTEL_DEVICE_MAX_EUS_PER_SUBSLICE (16) /* Maximum on gfx11 */
#define INTEL_DEVICE_MAX_PIXEL_PIPES (16) /* Maximum on DG2 */

enum intel_platform {
   INTEL_PLATFORM_GFX3 = 1,
   INTEL_PLATFORM_I965,
   INTEL_PLATFORM_ILK,
   INTEL_PLATFORM_G4X,
   INTEL_PLATFORM_SNB,
   INTEL_PLATFORM_IVB,
   INTEL_PLATFORM_BYT,
   INTEL_PLATFORM_HSW,
   INTEL_PLATFORM_BDW,
   INTEL_PLATFORM_CHV,
   INTEL_PLATFORM_SKL,
   INTEL_PLATFORM_BXT,
   INTEL_PLATFORM_KBL,
   INTEL_PLATFORM_GLK,
   INTEL_PLATFORM_CFL,
   INTEL_PLATFORM_ICL,
   INTEL_PLATFORM_EHL,
   INTEL_PLATFORM_TGL,
   INTEL_PLATFORM_RKL,
   INTEL_PLATFORM_DG1,
   INTEL_PLATFORM_ADL,
   INTEL_PLATFORM_RPL,
   INTEL_PLATFORM_DG2_G10,
   INTEL_PLATFORM_DG2_START = INTEL_PLATFORM_DG2_G10,
   INTEL_PLATFORM_DG2_G11,
   INTEL_PLATFORM_DG2_G12,
   INTEL_PLATFORM_DG2_END = INTEL_PLATFORM_DG2_G12,
   INTEL_PLATFORM_ATSM_G10,
   INTEL_PLATFORM_ATSM_START = INTEL_PLATFORM_ATSM_G10,
   INTEL_PLATFORM_ATSM_G11,
   INTEL_PLATFORM_ATSM_END = INTEL_PLATFORM_ATSM_G11,
   INTEL_PLATFORM_MTL_U,
   INTEL_PLATFORM_MTL_START = INTEL_PLATFORM_MTL_U,
   INTEL_PLATFORM_MTL_H,
   INTEL_PLATFORM_MTL_END = INTEL_PLATFORM_MTL_H,
   INTEL_PLATFORM_ARL_U,
   INTEL_PLATFORM_ARL_START = INTEL_PLATFORM_ARL_U,
   INTEL_PLATFORM_ARL_H,
   INTEL_PLATFORM_ARL_END = INTEL_PLATFORM_ARL_H,
   INTEL_PLATFORM_LNL,
   INTEL_PLATFORM_BMG,
   INTEL_PLATFORM_PTL,
};

struct intel_memory_class_instance
{

   /* Kernel backend specific class value, no translation needed yet */
   int klass;
   int instance;
};

enum intel_device_info_mmap_mode {
   INTEL_DEVICE_INFO_MMAP_MODE_UC = 0,
   INTEL_DEVICE_INFO_MMAP_MODE_WC,
   INTEL_DEVICE_INFO_MMAP_MODE_WB,
   INTEL_DEVICE_INFO_MMAP_MODE_XD, /* Xe2+ only. Only supported in GPU side and used for displayable
   buffers. */
};

struct intel_device_info_pat_entry
{
   uint8_t index;

   /**
    * This tells KMD what caching mode the CPU mapping should use.
    * It has nothing to do with any PAT cache modes.
    */
   enum intel_device_info_mmap_mode mmap;
};

enum intel_cmat_scope {
   INTEL_CMAT_SCOPE_NONE = 0,
   INTEL_CMAT_SCOPE_SUBGROUP,
};

enum intel_cooperative_matrix_component_type {
   INTEL_CMAT_FLOAT16,
   INTEL_CMAT_FLOAT32,
   INTEL_CMAT_SINT32,
   INTEL_CMAT_SINT8,
   INTEL_CMAT_UINT32,
   INTEL_CMAT_UINT8,
};

enum intel_engine_class {
   INTEL_ENGINE_CLASS_RENDER,
   INTEL_ENGINE_CLASS_COPY,
   INTEL_ENGINE_CLASS_VIDEO,
   INTEL_ENGINE_CLASS_VIDEO_ENHANCE,
   INTEL_ENGINE_CLASS_COMPUTE,
   INTEL_ENGINE_CLASS_INVALID,
};

struct intel_cooperative_matrix_configuration
{

   /**
    * Matrix A is MxK.
    * Matrix B is KxN.
    * Matrix C and Matrix Result are MxN.
    *
    * Result = A * B + C;
    */
   enum intel_cmat_scope scope;
   uint8_t m;
   uint8_t n;
   uint8_t k;
   enum intel_cooperative_matrix_component_type a;
   enum intel_cooperative_matrix_component_type b;
   enum intel_cooperative_matrix_component_type c;
   enum intel_cooperative_matrix_component_type result;
};

struct intel_device_info_mem_region
{
   uint64_t size;
   uint64_t free;
};

struct intel_device_info_ram_desc
{
   struct intel_memory_class_instance mem;
   struct intel_device_info_mem_region mappable;
   struct intel_device_info_mem_region unmappable;
};

struct intel_device_info_mem_desc
{
   bool use_class_instance;
   struct intel_device_info_ram_desc sram;
   struct intel_device_info_ram_desc vram;
};

struct intel_device_info_urb_desc
{
   int size;
   int min_entries[4];
   int max_entries[4];
};

struct intel_device_info_pat_desc
{

   /* To be used when CPU access is frequent, WB + 1 or 2 way coherent */
   struct intel_device_info_pat_entry cached_coherent;

   /* scanout and external BOs */
   struct intel_device_info_pat_entry scanout;

   /* Only supported in Xe2, compressed + WC */
   struct intel_device_info_pat_entry compressed;

   /* BOs without special needs, can be WB not coherent or WC it depends on the platforms and KMD */
   struct intel_device_info_pat_entry writeback_incoherent;
   struct intel_device_info_pat_entry writecombining;
};

struct intel_device_info
{
   enum intel_kmd_type kmd_type;

   /* Driver internal numbers used to differentiate platforms. */
   int ver;
   int verx10;

   /**
    * This is the run-time hardware GFX IP version that may be more specific
    * than ver/verx10. ver/verx10 may be more useful for comparing a class
    * of devices whereas gfx_ip_ver may be more useful for precisely
    * checking for a graphics ip type. GFX_IP_VER(major, minor) should be
    * used to compare IP versions.
    */
   uint32_t gfx_ip_ver;

   /**
    * This revision is queried from KMD unlike
    * pci_revision_id from drm device. Its value is not always
    * same as the pci_revision_id.
    * For LNL+ this is the stepping of GT IP/GMD RevId.
    */
   int revision;
   int gt;

   /* PCI info */
   uint16_t pci_domain;
   uint8_t pci_bus;
   uint8_t pci_dev;
   uint8_t pci_func;
   uint16_t pci_device_id;
   uint8_t pci_revision_id;
   enum intel_platform platform;
   bool has_hiz_and_separate_stencil;
   bool must_use_separate_stencil;
   bool has_sample_with_hiz;
   bool has_bit6_swizzle;
   bool has_llc;
   bool has_pln;
   bool has_64bit_float;
   bool has_64bit_float_via_math_pipe;
   bool has_64bit_int;
   bool has_integer_dword_mul;
   bool has_compr4;
   bool has_surface_tile_offset;
   bool supports_simd16_3src;
   bool disable_ccs_repack;

   /* True if CCS needs to be initialized before use. */
   bool has_illegal_ccs_values;

   /**
    * True if CCS uses a flat virtual address translation to a memory
    * carve-out, rather than aux map translations, or additional surfaces.
    */
   bool has_flat_ccs;
   bool has_aux_map;
   bool has_caching_uapi;
   bool has_tiling_uapi;
   bool has_ray_tracing;
   bool has_ray_query;
   bool has_local_mem;
   bool has_lsc;
   bool has_mesh_shading;
   bool has_mmap_offset;
   bool has_userptr_probe;
   bool has_context_isolation;
   bool has_set_pat_uapi;
   bool has_indirect_unroll;

   /* Intel hardware quirks */
   bool has_negative_rhw_bug;

   /**
    * Whether this platform supports fragment shading rate controlled by a
    * primitive in geometry shaders and by a control buffer.
    */
   bool has_coarse_pixel_primitive_and_cb;

   /* Whether this platform has compute engine */
   bool has_compute_engine;

   /**
    * Some versions of Gen hardware don't do centroid interpolation correctly
    * on unlit pixels, causing incorrect values for derivatives near triangle
    * edges.  Enabling this flag causes the fragment shader to use
    * non-centroid interpolation for unlit pixels, at the expense of two extra
    * fragment shader instructions.
    */
   bool needs_unlit_centroid_workaround;

   /**
    * Whether the platform needs an undocumented workaround for a hardware bug
    * that affects draw calls with a pixel shader that has 0 push constant cycles
    * when TBIMR is enabled, which has been seen to lead to hangs.  To avoid the
    * issue we simply pad the push constant payload to be at least 1 register.
    */
   bool needs_null_push_constant_tbimr_workaround;

   /* We need this for ADL-N specific Wa_14014966230. */
   bool is_adl_n;

   /**
    * GPU hardware limits
    *
    * In general, you can find shader thread maximums by looking at the "Maximum
    * Number of Threads" field in the Intel PRM description of the 3DSTATE_VS,
    * 3DSTATE_GS, 3DSTATE_HS, 3DSTATE_DS, and 3DSTATE_PS commands. URB entry
    * limits come from the "Number of URB Entries" field in the
    * 3DSTATE_URB_VS command and friends.
    *
    * These fields are used to calculate the scratch space to allocate.  The
    * amount of scratch space can be larger without being harmful on modern
    * GPUs, however, prior to Haswell, programming the maximum number of threads
    * to greater than the hardware maximum would cause GPU performance to tank.
    *
    * Total number of slices present on the device whether or not they've been
    * fused off.
    *
    * XXX: CS thread counts are limited by the inability to do cross subslice
    * communication. It is the effectively the number of logical threads which
    * can be executed in a subslice. Fuse configurations may cause this number
    * to change, so we program @max_cs_threads as the lower maximum.
    */
   unsigned num_slices;

   /**
    * Maximum number of slices present on this device (can be more than
    * num_slices if some slices are fused).
    */
   unsigned max_slices;

   /* Number of subslices for each slice (used to be uniform until CNL). */
   unsigned num_subslices[INTEL_DEVICE_MAX_SLICES];

   /**
    * Maximum number of subslices per slice present on this device (can be
    * more than the maximum value in the num_subslices[] array if some
    * subslices are fused).
    *
    * This is GT_SS_PER_SLICE in SKU.
    */
   unsigned max_subslices_per_slice;

   /* Number of subslices on each pixel pipe (ICL). */
   unsigned ppipe_subslices[INTEL_DEVICE_MAX_PIXEL_PIPES];

   /* Maximum number of EUs per subslice (some EUs can be fused off). */
   unsigned max_eus_per_subslice;

   /* Number of threads per eu, varies between 4 and 8 between generations. */
   unsigned num_thread_per_eu;

   /* Size of a register from the EU GRF file in bytes. */
   uint8_t grf_size;

   /* A bit mask of the slices available. */
   uint8_t slice_masks;

   /**
    * An array of bit mask of the subslices available, use subslice_slice_stride
    * to access this array.
    */
   uint8_t subslice_masks[INTEL_DEVICE_MAX_SLICES * DIV_ROUND_UP(INTEL_DEVICE_MAX_SUBSLICES, 8)];

   /**
    * The number of enabled subslices (considering fusing). For exactly which
    * subslices are enabled, see subslice_masks[].
    */
   unsigned subslice_total;

   /**
    * An array of bit mask of EUs available, use eu_slice_stride &
    * eu_subslice_stride to access this array.
    */
   uint8_t eu_masks[INTEL_DEVICE_MAX_SLICES * INTEL_DEVICE_MAX_SUBSLICES * DIV_ROUND_UP(INTEL_DEVICE_MAX_EUS_PER_SUBSLICE, 8)];

   /* Stride to access subslice_masks[]. */
   uint16_t subslice_slice_stride;

   /* Strides to access eu_masks[]. */
   uint16_t eu_slice_stride;
   uint16_t eu_subslice_stride;
   unsigned l3_banks;

   /* Maximum Vertex Shader threads */
   unsigned max_vs_threads;

   /* Maximum Hull Shader threads */
   unsigned max_tcs_threads;

   /* Maximum Domain Shader threads */
   unsigned max_tes_threads;

   /* Maximum Geometry Shader threads */
   unsigned max_gs_threads;

   /**
    * Theoretical maximum number of Pixel Shader threads.
    *
    * PSD means Pixel Shader Dispatcher. On modern Intel GPUs, hardware will
    * automatically scale pixel shader thread count, based on a single value
    * programmed into 3DSTATE_PS.
    *
    * To calculate the maximum number of threads for Gfx8 beyond (which have
    * multiple Pixel Shader Dispatchers):
    *
    * - Look up 3DSTATE_PS and find "Maximum Number of Threads Per PSD"
    * - Usually there's only one PSD per subslice, so use the number of
    *   subslices for number of PSDs.
    * - For max_wm_threads, the total should be PSD threads * #PSDs.
    */
   unsigned max_wm_threads;
   unsigned max_threads_per_psd;

   /**
    * Maximum Compute Shader threads.
    *
    * Thread count * number of EUs per subslice
    */
   unsigned max_cs_threads;

   /**
    * Maximum number of threads per workgroup supported by the GPGPU_WALKER or
    * COMPUTE_WALKER command.
    *
    * This may be smaller than max_cs_threads as it takes into account added
    * restrictions on the GPGPU/COMPUTE_WALKER commands.  While max_cs_threads
    * expresses the total parallelism of the GPU, this expresses the maximum
    * number of threads we can dispatch in a single workgroup.
    */
   unsigned max_cs_workgroup_threads;

   /**
    * The maximum number of potential scratch ids. Due to hardware
    * implementation details, the range of scratch ids may be larger than the
    * number of subslices.
    */
   unsigned max_scratch_ids[MESA_SHADER_STAGES];
   uint32_t max_scratch_size_per_thread;
   struct intel_device_info_urb_desc urb;
   unsigned max_constant_urb_size_kb;
   unsigned mesh_max_constant_urb_size_kb;
   unsigned engine_class_prefetch[INTEL_ENGINE_CLASS_INVALID];
   unsigned engine_class_supported_count[INTEL_ENGINE_CLASS_INVALID];
   unsigned mem_alignment;
   uint64_t timestamp_frequency;
   uint64_t aperture_bytes;
   uint64_t gtt_size;
   int simulator_id;
   char name[INTEL_DEVICE_MAX_NAME_SIZE];
   bool no_hw;

   /* Device needed INTEL_FORCE_PROBE */
   bool probe_forced;
   struct intel_device_info_mem_desc mem;
   struct intel_device_info_pat_desc pat;
   struct intel_cooperative_matrix_configuration cooperative_matrix_configurations[4];
   BITSET_DECLARE(workarounds, INTEL_WA_NUM);
};

#ifdef __cplusplus
}
#endif

#endif /* INTEL_DEVICE_INFO_GEN_H */
