#ifndef GEN8_RENDER_H
#define GEN8_RENDER_H

#define INTEL_MASK(high, low) (((1 << ((high) - (low) + 1)) - 1) << (low))

#define GEN8_3D(pipeline,op,sub) \
	((3 << 29) | ((pipeline) << 27) | ((op) << 24) | ((sub) << 16))

#define GEN8_STATE_BASE_ADDRESS			GEN8_3D(0, 1, 1)
# define BASE_ADDRESS_MODIFY			(1 << 0)

#define GEN8_STATE_SIP				GEN8_3D(0, 1, 2)

#define GEN8_3DSTATE_VF_STATISTICS		GEN8_3D(1, 0, 0xb)
#define GEN8_PIPELINE_SELECT			GEN8_3D(1, 1, 4)
# define PIPELINE_SELECT_3D		0
# define PIPELINE_SELECT_MEDIA		1

#define GEN8_MEDIA_STATE_POINTERS		GEN8_3D(2, 0, 0)
#define GEN8_MEDIA_OBJECT			GEN8_3D(2, 1, 0)

#define GEN8_3DSTATE_CLEAR_PARAMS               GEN8_3D(3, 0, 0x04)
#define GEN8_3DSTATE_DEPTH_BUFFER               GEN8_3D(3, 0, 0x05)
# define DEPTH_BUFFER_TYPE_SHIFT	29
# define DEPTH_BUFFER_FORMAT_SHIFT	18
/* DW1 */
# define DEPTH_CLEAR_VALID		(1 << 15)
#define GEN8_3DSTATE_STENCIL_BUFFER		GEN8_3D(3, 0, 0x06)
#define GEN8_3DSTATE_HIER_DEPTH_BUFFER		GEN8_3D(3, 0, 0x07)
#define GEN8_3DSTATE_VERTEX_BUFFERS		GEN8_3D(3, 0, 0x08)
# define VB_INDEX_SHIFT			26
# define VB_MODIFY_ENABLE		(1 << 14)
#define GEN8_3DSTATE_VERTEX_ELEMENTS		GEN8_3D(3, 0, 0x09)
# define VE_INDEX_SHIFT		26
# define VE_VALID					(1 << 25)
# define VE_FORMAT_SHIFT				16
# define VE_OFFSET_SHIFT				0
# define VE_COMPONENT_0_SHIFT			28
# define VE_COMPONENT_1_SHIFT			24
# define VE_COMPONENT_2_SHIFT			20
# define VE_COMPONENT_3_SHIFT			16
#define GEN8_3DSTATE_INDEX_BUFFER		GEN8_3D(3, 0, 0x0a)
#define GEN8_3DSTATE_VF				GEN8_3D(3, 0, 0x0c)

#define GEN8_3DSTATE_MULTISAMPLE		GEN8_3D(3, 0, 0x0d)
/* DW1 */
# define MULTISAMPLE_PIXEL_LOCATION_CENTER		(0 << 4)
# define MULTISAMPLE_PIXEL_LOCATION_UPPER_LEFT	(1 << 4)
# define MULTISAMPLE_NUMSAMPLES_1			(0 << 1)
# define MULTISAMPLE_NUMSAMPLES_4			(2 << 1)
# define MULTISAMPLE_NUMSAMPLES_8			(3 << 1)

#define GEN8_3DSTATE_CC_STATE_POINTERS		GEN8_3D(3, 0, 0x0e)
#define GEN8_3DSTATE_SCISSOR_STATE_POINTERS	GEN8_3D(3, 0, 0x0f)

#define GEN8_3DSTATE_VS				GEN8_3D(3, 0, 0x10)
#define GEN8_3DSTATE_GS				GEN8_3D(3, 0, 0x11)
#define GEN8_3DSTATE_CLIP			GEN8_3D(3, 0, 0x12)
#define GEN8_3DSTATE_SF				GEN8_3D(3, 0, 0x13)
# define SF_TRI_PROVOKE_SHIFT		29
# define SF_LINE_PROVOKE_SHIFT		27
# define SF_FAN_PROVOKE_SHIFT		25

#define GEN8_3DSTATE_WM				GEN8_3D(3, 0, 0x14)
/* DW1 */
# define WM_STATISTICS_ENABLE                              (1 << 31)
# define WM_DEPTH_CLEAR                                    (1 << 30)
# define WM_DISPATCH_ENABLE                                (1 << 29)
# define WM_DEPTH_RESOLVE                                  (1 << 28)
# define WM_HIERARCHICAL_DEPTH_RESOLVE                     (1 << 27)
# define WM_KILL_ENABLE                                    (1 << 25)
# define WM_PSCDEPTH_OFF                                   (0 << 23)
# define WM_PSCDEPTH_ON                                    (1 << 23)
# define WM_PSCDEPTH_ON_GE                                 (2 << 23)
# define WM_PSCDEPTH_ON_LE                                 (3 << 23)
# define WM_USES_SOURCE_DEPTH                              (1 << 20)
# define WM_USES_SOURCE_W                                  (1 << 19)
# define WM_POSITION_ZW_PIXEL                              (0 << 17)
# define WM_POSITION_ZW_CENTROID                           (2 << 17)
# define WM_POSITION_ZW_SAMPLE                             (3 << 17)
# define WM_NONPERSPECTIVE_SAMPLE_BARYCENTRIC              (1 << 16)
# define WM_NONPERSPECTIVE_CENTROID_BARYCENTRIC            (1 << 15)
# define WM_NONPERSPECTIVE_PIXEL_BARYCENTRIC               (1 << 14)
# define WM_PERSPECTIVE_SAMPLE_BARYCENTRIC                 (1 << 13)
# define WM_PERSPECTIVE_CENTROID_BARYCENTRIC               (1 << 12)
# define WM_PERSPECTIVE_PIXEL_BARYCENTRIC                  (1 << 11)
# define WM_USES_INPUT_COVERAGE_MASK                       (1 << 10)
# define WM_LINE_END_CAP_AA_WIDTH_0_5                      (0 << 8)
# define WM_LINE_END_CAP_AA_WIDTH_1_0                      (1 << 8)
# define WM_LINE_END_CAP_AA_WIDTH_2_0                      (2 << 8)
# define WM_LINE_END_CAP_AA_WIDTH_4_0                      (3 << 8)
# define WM_LINE_AA_WIDTH_0_5                              (0 << 6)
# define WM_LINE_AA_WIDTH_1_0                              (1 << 6)
# define WM_LINE_AA_WIDTH_2_0                              (2 << 6)
# define WM_LINE_AA_WIDTH_4_0                              (3 << 6)
# define WM_POLYGON_STIPPLE_ENABLE                         (1 << 4)
# define WM_LINE_STIPPLE_ENABLE                            (1 << 3)
# define WM_POINT_RASTRULE_UPPER_RIGHT                     (1 << 2)
# define WM_MSRAST_OFF_PIXEL                               (0 << 0)
# define WM_MSRAST_OFF_PATTERN                             (1 << 0)
# define WM_MSRAST_ON_PIXEL                                (2 << 0)
# define WM_MSRAST_ON_PATTERN                              (3 << 0)
/* DW2 */
# define WM_MSDISPMODE_PERPIXEL                            (1 << 31)

#define GEN8_3DSTATE_CONSTANT_VS		GEN8_3D(3, 0, 0x15)
#define GEN8_3DSTATE_CONSTANT_GS		GEN8_3D(3, 0, 0x16)
#define GEN8_3DSTATE_CONSTANT_PS		GEN8_3D(3, 0, 0x17)

#define GEN8_3DSTATE_SAMPLE_MASK		GEN8_3D(3, 0, 0x18)

#define GEN8_3DSTATE_CONSTANT_HS                GEN8_3D(3, 0, 0x19)
#define GEN8_3DSTATE_CONSTANT_DS                GEN8_3D(3, 0, 0x1a)

#define GEN8_3DSTATE_HS                         GEN8_3D(3, 0, 0x1b)
#define GEN8_3DSTATE_TE                         GEN8_3D(3, 0, 0x1c)
#define GEN8_3DSTATE_DS                         GEN8_3D(3, 0, 0x1d)
#define GEN8_3DSTATE_STREAMOUT                  GEN8_3D(3, 0, 0x1e)

#define GEN8_3DSTATE_SBE                        GEN8_3D(3, 0, 0x1f)
/* DW1 */
# define SBE_FORCE_VERTEX_URB_READ_LENGTH  (1<<29)
# define SBE_FORCE_VERTEX_URB_READ_OFFSET  (1<<28)
# define SBE_NUM_OUTPUTS_SHIFT             22
# define SBE_SWIZZLE_ENABLE                (1 << 21)
# define SBE_POINT_SPRITE_LOWERLEFT        (1 << 20)
# define SBE_URB_ENTRY_READ_LENGTH_SHIFT   11
# define SBE_URB_ENTRY_READ_OFFSET_SHIFT   4

#define GEN8_3DSTATE_PS                                 GEN8_3D(3, 0, 0x20)
/* DW1: kernel pointer */
/* DW2 */
# define PS_SPF_MODE                               (1 << 31)
# define PS_VECTOR_MASK_ENABLE                     (1 << 30)
# define PS_SAMPLER_COUNT_SHIFT                    27
# define PS_BINDING_TABLE_ENTRY_COUNT_SHIFT        18
# define PS_FLOATING_POINT_MODE_IEEE_754           (0 << 16)
# define PS_FLOATING_POINT_MODE_ALT                (1 << 16)
/* DW3: scratch space */
/* DW4 */
# define PS_MAX_THREADS_SHIFT                      23
# define PS_MAX_THREADS                            (62 << PS_MAX_THREADS_SHIFT)
# define PS_SAMPLE_MASK_SHIFT                      12
# define PS_PUSH_CONSTANT_ENABLE                   (1 << 11)
# define PS_RENDER_TARGET_CLEAR			   (1 << 8)
# define PS_RENDER_TARGET_RESOLVE		   (1 << 6)
# define PS_POSOFFSET_NONE                         (0 << 3)
# define PS_POSOFFSET_CENTROID                     (2 << 3)
# define PS_POSOFFSET_SAMPLE                       (3 << 3)
# define PS_32_DISPATCH_ENABLE                     (1 << 2)
# define PS_16_DISPATCH_ENABLE                     (1 << 1)
# define PS_8_DISPATCH_ENABLE                      (1 << 0)
/* DW5 */
# define PS_DISPATCH_START_GRF_SHIFT_0             16
# define PS_DISPATCH_START_GRF_SHIFT_1             8
# define PS_DISPATCH_START_GRF_SHIFT_2             0
/* DW6: kernel 1 pointer */
/* DW7: kernel 2 pointer */

#define GEN8_3DSTATE_VIEWPORT_STATE_POINTERS_SF_CLIP      GEN8_3D(3, 0, 0x21)
#define GEN8_3DSTATE_VIEWPORT_STATE_POINTERS_CC         GEN8_3D(3, 0, 0x23)

#define GEN8_3DSTATE_BLEND_STATE_POINTERS               GEN8_3D(3, 0, 0x24)

#define GEN8_3DSTATE_BINDING_TABLE_POINTERS_VS          GEN8_3D(3, 0, 0x26)
#define GEN8_3DSTATE_BINDING_TABLE_POINTERS_HS          GEN8_3D(3, 0, 0x27)
#define GEN8_3DSTATE_BINDING_TABLE_POINTERS_DS          GEN8_3D(3, 0, 0x28)
#define GEN8_3DSTATE_BINDING_TABLE_POINTERS_GS          GEN8_3D(3, 0, 0x29)
#define GEN8_3DSTATE_BINDING_TABLE_POINTERS_PS          GEN8_3D(3, 0, 0x2a)

#define GEN8_3DSTATE_SAMPLER_STATE_POINTERS_VS          GEN8_3D(3, 0, 0x2b)
#define GEN8_3DSTATE_SAMPLER_STATE_POINTERS_HS          GEN8_3D(3, 0, 0x2c)
#define GEN8_3DSTATE_SAMPLER_STATE_POINTERS_DS          GEN8_3D(3, 0, 0x2d)
#define GEN8_3DSTATE_SAMPLER_STATE_POINTERS_GS          GEN8_3D(3, 0, 0x2e)
#define GEN8_3DSTATE_SAMPLER_STATE_POINTERS_PS          GEN8_3D(3, 0, 0x2f)

#define GEN8_3DSTATE_URB_VS                             GEN8_3D(3, 0, 0x30)
#define GEN8_3DSTATE_URB_HS                             GEN8_3D(3, 0, 0x31)
#define GEN8_3DSTATE_URB_DS                             GEN8_3D(3, 0, 0x32)
#define GEN8_3DSTATE_URB_GS                             GEN8_3D(3, 0, 0x33)
/* DW1 */
# define URB_ENTRY_NUMBER_SHIFT            0
# define URB_ENTRY_SIZE_SHIFT              16
# define URB_STARTING_ADDRESS_SHIFT        25

#define GEN8_3DSTATE_GATHER_CONSTANT_VS             GEN8_3D(3, 0, 0x34)
#define GEN8_3DSTATE_GATHER_CONSTANT_GS             GEN8_3D(3, 0, 0x35)
#define GEN8_3DSTATE_GATHER_CONSTANT_HS             GEN8_3D(3, 0, 0x36)
#define GEN8_3DSTATE_GATHER_CONSTANT_DS             GEN8_3D(3, 0, 0x37)
#define GEN8_3DSTATE_GATHER_CONSTANT_PS             GEN8_3D(3, 0, 0x38)

#define GEN8_3DSTATE_DX9_CONSTANTF_VS             GEN8_3D(3, 0, 0x39)
#define GEN8_3DSTATE_DX9_CONSTANTF_PS             GEN8_3D(3, 0, 0x3a)
#define GEN8_3DSTATE_DX9_CONSTANTI_VS             GEN8_3D(3, 0, 0x3b)
#define GEN8_3DSTATE_DX9_CONSTANTI_PS             GEN8_3D(3, 0, 0x3c)
#define GEN8_3DSTATE_DX9_CONSTANTB_VS             GEN8_3D(3, 0, 0x3d)
#define GEN8_3DSTATE_DX9_CONSTANTB_PS             GEN8_3D(3, 0, 0x3e)
#define GEN8_3DSTATE_DX9_LOCAL_VALID_VS           GEN8_3D(3, 0, 0x3f)
#define GEN8_3DSTATE_DX9_LOCAL_VALID_PS           GEN8_3D(3, 0, 0x40)
#define GEN8_3DSTATE_DX9_GENERATE_ACTIVE_VS       GEN8_3D(3, 0, 0x41)
#define GEN8_3DSTATE_DX9_GENERATE_ACTIVE_PS       GEN8_3D(3, 0, 0x42)

#define GEN8_3DSTATE_BINDING_TABLE_EDIT_VS       GEN8_3D(3, 0, 0x43)
#define GEN8_3DSTATE_BINDING_TABLE_EDIT_GS       GEN8_3D(3, 0, 0x44)
#define GEN8_3DSTATE_BINDING_TABLE_EDIT_HS       GEN8_3D(3, 0, 0x45)
#define GEN8_3DSTATE_BINDING_TABLE_EDIT_DS       GEN8_3D(3, 0, 0x46)
#define GEN8_3DSTATE_BINDING_TABLE_EDIT_PS       GEN8_3D(3, 0, 0x47)

#define GEN8_3DSTATE_VF_INSTANCING		GEN8_3D(3, 0, 0x49)
#define GEN8_3DSTATE_VF_SGVS			GEN8_3D(3, 0, 0x4a)
# define SGVS_ENABLE_INSTANCE_ID			(1 << 31)
# define SGVS_INSTANCE_ID_COMPONENT_SHIFT		29
# define SGVS_INSTANCE_ID_ELEMENT_OFFSET_SHIFT	16
# define SGVS_ENABLE_VERTEX_ID			(1 << 15)
# define SGVS_VERTEX_ID_COMPONENT_SHIFT            13
# define SGVS_VERTEX_ID_ELEMENT_OFFSET_SHIFT	0
#define GEN8_3DSTATE_VF_TOPOLOGY		GEN8_3D(3, 0, 0x4b)
# define POINTLIST         0x01
# define LINELIST          0x02
# define LINESTRIP         0x03
# define TRILIST           0x04
# define TRISTRIP          0x05
# define TRIFAN            0x06
# define QUADLIST          0x07
# define QUADSTRIP         0x08
# define LINELIST_ADJ      0x09
# define LINESTRIP_ADJ     0x0A
# define TRILIST_ADJ       0x0B
# define TRISTRIP_ADJ      0x0C
# define TRISTRIP_REVERSE  0x0D
# define POLYGON           0x0E
# define RECTLIST          0x0F
# define LINELOOP          0x10
# define POINTLIST_BF      0x11
# define LINESTRIP_CONT    0x12
# define LINESTRIP_BF      0x13
# define LINESTRIP_CONT_BF 0x14
# define TRIFAN_NOSTIPPLE  0x15

#define GEN8_3DSTATE_WM_CHROMAKEY		GEN8_3D(3, 0, 0x4c)

#define GEN8_3DSTATE_PS_BLEND				GEN8_3D(3, 0, 0x4d)
# define PS_BLEND_ALPHA_TO_COVERAGE_ENABLE		(1 << 31)
# define PS_BLEND_HAS_WRITEABLE_RT			(1 << 30)
# define PS_BLEND_COLOR_BLEND_ENABLE			(1 << 29)
# define PS_BLEND_SRC_ALPHA_SHIFT			24
# define PS_BLEND_DST_ALPHA_SHIFT			19
# define PS_BLEND_SRC_SHIFT				14
# define PS_BLEND_DST_SHIFT				9
# define PS_BLEND_ALPHA_TEST_ENABLE			(1 << 8)
# define PS_BLEND_INDEPENDENT_ALPHA_BLEND_ENABLE	(1 << 7)

#define GEN8_3DSTATE_WM_DEPTH_STENCIL		GEN8_3D(3, 0, 0x4e)
/* DW1 */
# define WM_DS_STENCIL_TEST_MASK_MASK		INTEL_MASK(31, 24)
# define WM_DS_STENCIL_TEST_MASK_SHIFT		24
# define WM_DS_STENCIL_WRITE_MASK_MASK		INTEL_MASK(23, 16)
# define WM_DS_STENCIL_WRITE_MASK_SHIFT		16
# define WM_DS_BF_STENCIL_TEST_MASK_MASK		INTEL_MASK(15, 8)
# define WM_DS_BF_STENCIL_TEST_MASK_SHIFT		8
# define WM_DS_BF_STENCIL_WRITE_MASK_MASK		INTEL_MASK(7, 0)
# define WM_DS_DEPTH_FUNC_SHIFT			5
# define WM_DS_DOUBLE_SIDED_STENCIL_ENABLE		(1 << 4)
# define WM_DS_STENCIL_TEST_ENABLE			(1 << 3)
# define WM_DS_STENCIL_BUFFER_WRITE_ENABLE		(1 << 2)
# define WM_DS_DEPTH_TEST_ENABLE			(1 << 1)
# define WM_DS_DEPTH_BUFFER_WRITE_ENABLE		(1 << 0)
/* DW2 */
# define WM_DS_STENCIL_TEST_MASK_MASK		INTEL_MASK(31, 24)
# define WM_DS_STENCIL_TEST_MASK_SHIFT		24
# define WM_DS_STENCIL_WRITE_MASK_MASK		INTEL_MASK(23, 16)
# define WM_DS_STENCIL_WRITE_MASK_SHIFT		16
# define WM_DS_BF_STENCIL_TEST_MASK_MASK		INTEL_MASK(15, 8)
# define WM_DS_BF_STENCIL_TEST_MASK_SHIFT		8
# define WM_DS_BF_STENCIL_WRITE_MASK_MASK		INTEL_MASK(7, 0)
# define WM_DS_BF_STENCIL_WRITE_MASK_SHIFT		0

#define GEN8_3DSTATE_PS_EXTRA		GEN8_3D(3, 0, 0x4f)
# define PSX_PIXEL_SHADER_VALID                    (1 << 31)
# define PSX_PIXEL_SHADER_NO_RT_WRITE              (1 << 30)
# define PSX_OMASK_TO_RENDER_TARGET                (1 << 29)
# define PSX_KILL_ENABLE                           (1 << 28)
# define PSX_PSCDEPTH_OFF                          (0 << 26)
# define PSX_PSCDEPTH_ON                           (1 << 26)
# define PSX_PSCDEPTH_ON_GE                        (2 << 26)
# define PSX_PSCDEPTH_ON_LE                        (3 << 26)
# define PSX_FORCE_COMPUTED_DEPTH                  (1 << 25)
# define PSX_USES_SOURCE_DEPTH                     (1 << 24)
# define PSX_USES_SOURCE_W                         (1 << 23)
# define PSX_ATTRIBUTE_ENABLE                      (1 << 8)
# define PSX_SHADER_DISABLES_ALPHA_TO_COVERAGE     (1 << 7)
# define PSX_SHADER_IS_PER_SAMPLE                  (1 << 6)
# define PSX_SHADER_HAS_UAV                        (1 << 2)
# define PSX_SHADER_USES_INPUT_COVERAGE_MASK       (1 << 1)

#define GEN8_3DSTATE_RASTER		GEN8_3D(3, 0, 0x50)
/* DW1 */
# define RASTER_FRONT_WINDING_CCW                  (1 << 21)
# define RASTER_CULL_BOTH                          (0 << 16)
# define RASTER_CULL_NONE                          (1 << 16)
# define RASTER_CULL_FRONT                         (2 << 16)
# define RASTER_CULL_BACK                          (3 << 16)
# define RASTER_SMOOTH_POINT_ENABLE                (1 << 13)
# define RASTER_LINE_AA_ENABLE                     (1 << 2)
# define RASTER_VIEWPORT_Z_CLIP_TEST_ENABLE        (1 << 0)

#define GEN8_3DSTATE_SBE_SWIZ		GEN8_3D(3, 0, 0x51)
#define GEN8_3DSTATE_WM_HZ_OP		GEN8_3D(3, 0, 0x52)


#define GEN8_3DSTATE_DRAWING_RECTANGLE		GEN8_3D(3, 1, 0x00)
#define GEN8_3DSTATE_SAMPLER_PALETTE_LOAD	GEN8_3D(3, 1, 0x02)
#define GEN8_3DSTATE_CHROMA_KEY			GEN8_3D(3, 1, 0x04)

#define GEN8_3DSTATE_POLY_STIPPLE_OFFSET	GEN8_3D(3, 1, 0x06)
#define GEN8_3DSTATE_POLY_STIPPLE_PATTERN	GEN8_3D(3, 1, 0x07)
#define GEN8_3DSTATE_LINE_STIPPLE		GEN8_3D(3, 1, 0x08)
#define GEN8_3DSTATE_AA_LINE_PARAMS		GEN8_3D(3, 1, 0x0a)
#define GEN8_3DSTATE_SAMPLER_PALETTE_LOAD1	GEN8_3D(3, 1, 0x0c)
#define GEN8_3DSTATE_MONOFILTER_SIZE		GEN8_3D(3, 1, 0x11)
#define GEN8_3DSTATE_PUSH_CONSTANT_ALLOC_VS	GEN8_3D(3, 1, 0x12)
#define GEN8_3DSTATE_PUSH_CONSTANT_ALLOC_HS	GEN8_3D(3, 1, 0x13)
#define GEN8_3DSTATE_PUSH_CONSTANT_ALLOC_DS	GEN8_3D(3, 1, 0x14)
#define GEN8_3DSTATE_PUSH_CONSTANT_ALLOC_GS	GEN8_3D(3, 1, 0x15)
#define GEN8_3DSTATE_PUSH_CONSTANT_ALLOC_PS	GEN8_3D(3, 1, 0x16)
/* DW1 */
# define PUSH_CONSTANT_BUFFER_OFFSET_SHIFT 16
# define PUSH_CONSTANT_BUFFER_SIZE_SHIFT 0

#define GEN8_3DSTATE_SO_DECL_LIST		GEN8_3D(3, 1, 0x17)
#define GEN8_3DSTATE_SO_BUFFER			GEN8_3D(3, 1, 0x18)
#define GEN8_3DSTATE_BINDING_TABLE_POOL_ALLOC	GEN8_3D(3, 1, 0x19)
#define GEN8_3DSTATE_GATHER_BUFFER_POOL_ALLOC	GEN8_3D(3, 1, 0x1a)
#define GEN8_3DSTATE_DX9_CONSTANT_BUFFER_POOL_ALLOC	GEN8_3D(3, 1, 0x1b)
#define GEN8_3DSTATE_SAMPLE_PATTERN		GEN8_3D(3, 1, 0x1c)


/* for GEN8_PIPE_CONTROL */
#define GEN8_PIPE_CONTROL		GEN8_3D(3, 2, 0)
#define PIPE_CONTROL_CS_STALL      (1 << 20)
#define PIPE_CONTROL_NOWRITE       (0 << 14)
#define PIPE_CONTROL_WRITE_QWORD   (1 << 14)
#define PIPE_CONTROL_WRITE_DEPTH   (2 << 14)
#define PIPE_CONTROL_WRITE_TIME    (3 << 14)
#define PIPE_CONTROL_DEPTH_STALL   (1 << 13)
#define PIPE_CONTROL_WC_FLUSH      (1 << 12)
#define PIPE_CONTROL_IS_FLUSH      (1 << 11)
#define PIPE_CONTROL_TC_FLUSH      (1 << 10)
#define PIPE_CONTROL_NOTIFY_ENABLE (1 << 8)
#define PIPE_CONTROL_GLOBAL_GTT    (1 << 2)
#define PIPE_CONTROL_LOCAL_PGTT    (0 << 2)
#define PIPE_CONTROL_STALL_AT_SCOREBOARD   (1 << 1)
#define PIPE_CONTROL_DEPTH_CACHE_FLUSH	(1 << 0)


#define GEN8_3DPRIMITIVE			GEN8_3D(3, 3, 0)

/* 3DPRIMITIVE bits */
#define VERTEX_SEQUENTIAL (0 << 15)
#define VERTEX_RANDOM	  (1 << 15)

#define ANISORATIO_2     0
#define ANISORATIO_4     1
#define ANISORATIO_6     2
#define ANISORATIO_8     3
#define ANISORATIO_10    4
#define ANISORATIO_12    5
#define ANISORATIO_14    6
#define ANISORATIO_16    7

#define BLENDFACTOR_ONE                 0x1
#define BLENDFACTOR_SRC_COLOR           0x2
#define BLENDFACTOR_SRC_ALPHA           0x3
#define BLENDFACTOR_DST_ALPHA           0x4
#define BLENDFACTOR_DST_COLOR           0x5
#define BLENDFACTOR_SRC_ALPHA_SATURATE  0x6
#define BLENDFACTOR_CONST_COLOR         0x7
#define BLENDFACTOR_CONST_ALPHA         0x8
#define BLENDFACTOR_SRC1_COLOR          0x9
#define BLENDFACTOR_SRC1_ALPHA          0x0A
#define BLENDFACTOR_ZERO                0x11
#define BLENDFACTOR_INV_SRC_COLOR       0x12
#define BLENDFACTOR_INV_SRC_ALPHA       0x13
#define BLENDFACTOR_INV_DST_ALPHA       0x14
#define BLENDFACTOR_INV_DST_COLOR       0x15
#define BLENDFACTOR_INV_CONST_COLOR     0x17
#define BLENDFACTOR_INV_CONST_ALPHA     0x18
#define BLENDFACTOR_INV_SRC1_COLOR      0x19
#define BLENDFACTOR_INV_SRC1_ALPHA      0x1A

#define BLENDFUNCTION_ADD               0
#define BLENDFUNCTION_SUBTRACT          1
#define BLENDFUNCTION_REVERSE_SUBTRACT  2
#define GEN8_BLENDFUNCTION_MIN               3
#define BLENDFUNCTION_MAX               4

#define ALPHATEST_FORMAT_UNORM8         0
#define ALPHATEST_FORMAT_FLOAT32        1

#define CHROMAKEY_KILL_ON_ANY_MATCH  0
#define CHROMAKEY_REPLACE_BLACK      1

#define CLIP_API_OGL     0
#define CLIP_API_DX      1

#define CLIPMODE_NORMAL              0
#define CLIPMODE_CLIP_ALL            1
#define CLIPMODE_CLIP_NON_REJECTED   2
#define CLIPMODE_REJECT_ALL          3
#define CLIPMODE_ACCEPT_ALL          4

#define CLIP_NDCSPACE     0
#define CLIP_SCREENSPACE  1

#define COMPAREFUNCTION_ALWAYS       0
#define COMPAREFUNCTION_NEVER        1
#define COMPAREFUNCTION_LESS         2
#define COMPAREFUNCTION_EQUAL        3
#define COMPAREFUNCTION_LEQUAL       4
#define COMPAREFUNCTION_GREATER      5
#define COMPAREFUNCTION_NOTEQUAL     6
#define COMPAREFUNCTION_GEQUAL       7

#define COVERAGE_PIXELS_HALF     0
#define COVERAGE_PIXELS_1        1
#define COVERAGE_PIXELS_2        2
#define COVERAGE_PIXELS_4        3

#define DEPTHFORMAT_D32_FLOAT_S8X24_UINT     0
#define DEPTHFORMAT_D32_FLOAT                1
#define DEPTHFORMAT_D24_UNORM_S8_UINT        2
#define DEPTHFORMAT_D16_UNORM                5

#define FLOATING_POINT_IEEE_754        0
#define FLOATING_POINT_NON_IEEE_754    1

#define INDEX_BYTE     0
#define INDEX_WORD     1
#define INDEX_DWORD    2

#define LOGICOPFUNCTION_CLEAR            0
#define LOGICOPFUNCTION_NOR              1
#define LOGICOPFUNCTION_AND_INVERTED     2
#define LOGICOPFUNCTION_COPY_INVERTED    3
#define LOGICOPFUNCTION_AND_REVERSE      4
#define LOGICOPFUNCTION_INVERT           5
#define LOGICOPFUNCTION_XOR              6
#define LOGICOPFUNCTION_NAND             7
#define LOGICOPFUNCTION_AND              8
#define LOGICOPFUNCTION_EQUIV            9
#define LOGICOPFUNCTION_NOOP             10
#define LOGICOPFUNCTION_OR_INVERTED      11
#define LOGICOPFUNCTION_COPY             12
#define LOGICOPFUNCTION_OR_REVERSE       13
#define LOGICOPFUNCTION_OR               14
#define LOGICOPFUNCTION_SET              15

#define MAPFILTER_NEAREST	0x0
#define MAPFILTER_LINEAR	0x1
#define MAPFILTER_ANISOTROPIC	0x2
#define MAPFILTER_FLEXIBLE 	0x3
#define MAPFILTER_MONO 		0x6

#define MIPFILTER_NONE        0
#define MIPFILTER_NEAREST     1
#define MIPFILTER_LINEAR      3

#define POLYGON_FRONT_FACING     0
#define POLYGON_BACK_FACING      1

#define PREFILTER_ALWAYS     0x0
#define PREFILTER_NEVER      0x1
#define PREFILTER_LESS       0x2
#define PREFILTER_EQUAL      0x3
#define PREFILTER_LEQUAL     0x4
#define PREFILTER_GREATER    0x5
#define PREFILTER_NOTEQUAL   0x6
#define PREFILTER_GEQUAL     0x7

#define RASTRULE_UPPER_LEFT  0
#define RASTRULE_UPPER_RIGHT 1

#define STENCILOP_KEEP               0
#define STENCILOP_ZERO               1
#define STENCILOP_REPLACE            2
#define STENCILOP_INCRSAT            3
#define STENCILOP_DECRSAT            4
#define STENCILOP_INCR               5
#define STENCILOP_DECR               6
#define STENCILOP_INVERT             7

#define SURFACE_MIPMAPLAYOUT_BELOW   0
#define SURFACE_MIPMAPLAYOUT_RIGHT   1

#define SURFACEFORMAT_R32G32B32A32_FLOAT             0x000
#define SURFACEFORMAT_R32G32B32A32_SINT              0x001
#define SURFACEFORMAT_R32G32B32A32_UINT              0x002
#define SURFACEFORMAT_R32G32B32A32_UNORM             0x003
#define SURFACEFORMAT_R32G32B32A32_SNORM             0x004
#define SURFACEFORMAT_R64G64_FLOAT                   0x005
#define SURFACEFORMAT_R32G32B32X32_FLOAT             0x006
#define SURFACEFORMAT_R32G32B32A32_SSCALED           0x007
#define SURFACEFORMAT_R32G32B32A32_USCALED           0x008
#define SURFACEFORMAT_R32G32B32_FLOAT                0x040
#define SURFACEFORMAT_R32G32B32_SINT                 0x041
#define SURFACEFORMAT_R32G32B32_UINT                 0x042
#define SURFACEFORMAT_R32G32B32_UNORM                0x043
#define SURFACEFORMAT_R32G32B32_SNORM                0x044
#define SURFACEFORMAT_R32G32B32_SSCALED              0x045
#define SURFACEFORMAT_R32G32B32_USCALED              0x046
#define SURFACEFORMAT_R16G16B16A16_UNORM             0x080
#define SURFACEFORMAT_R16G16B16A16_SNORM             0x081
#define SURFACEFORMAT_R16G16B16A16_SINT              0x082
#define SURFACEFORMAT_R16G16B16A16_UINT              0x083
#define SURFACEFORMAT_R16G16B16A16_FLOAT             0x084
#define SURFACEFORMAT_R32G32_FLOAT                   0x085
#define SURFACEFORMAT_R32G32_SINT                    0x086
#define SURFACEFORMAT_R32G32_UINT                    0x087
#define SURFACEFORMAT_R32_FLOAT_X8X24_TYPELESS       0x088
#define SURFACEFORMAT_X32_TYPELESS_G8X24_UINT        0x089
#define SURFACEFORMAT_L32A32_FLOAT                   0x08A
#define SURFACEFORMAT_R32G32_UNORM                   0x08B
#define SURFACEFORMAT_R32G32_SNORM                   0x08C
#define SURFACEFORMAT_R64_FLOAT                      0x08D
#define SURFACEFORMAT_R16G16B16X16_UNORM             0x08E
#define SURFACEFORMAT_R16G16B16X16_FLOAT             0x08F
#define SURFACEFORMAT_A32X32_FLOAT                   0x090
#define SURFACEFORMAT_L32X32_FLOAT                   0x091
#define SURFACEFORMAT_I32X32_FLOAT                   0x092
#define SURFACEFORMAT_R16G16B16A16_SSCALED           0x093
#define SURFACEFORMAT_R16G16B16A16_USCALED           0x094
#define SURFACEFORMAT_R32G32_SSCALED                 0x095
#define SURFACEFORMAT_R32G32_USCALED                 0x096
#define SURFACEFORMAT_B8G8R8A8_UNORM                 0x0C0
#define SURFACEFORMAT_B8G8R8A8_UNORM_SRGB            0x0C1
#define SURFACEFORMAT_R10G10B10A2_UNORM              0x0C2
#define SURFACEFORMAT_R10G10B10A2_UNORM_SRGB         0x0C3
#define SURFACEFORMAT_R10G10B10A2_UINT               0x0C4
#define SURFACEFORMAT_R10G10B10_SNORM_A2_UNORM       0x0C5
#define SURFACEFORMAT_R8G8B8A8_UNORM                 0x0C7
#define SURFACEFORMAT_R8G8B8A8_UNORM_SRGB            0x0C8
#define SURFACEFORMAT_R8G8B8A8_SNORM                 0x0C9
#define SURFACEFORMAT_R8G8B8A8_SINT                  0x0CA
#define SURFACEFORMAT_R8G8B8A8_UINT                  0x0CB
#define SURFACEFORMAT_R16G16_UNORM                   0x0CC
#define SURFACEFORMAT_R16G16_SNORM                   0x0CD
#define SURFACEFORMAT_R16G16_SINT                    0x0CE
#define SURFACEFORMAT_R16G16_UINT                    0x0CF
#define SURFACEFORMAT_R16G16_FLOAT                   0x0D0
#define SURFACEFORMAT_B10G10R10A2_UNORM              0x0D1
#define SURFACEFORMAT_B10G10R10A2_UNORM_SRGB         0x0D2
#define SURFACEFORMAT_R11G11B10_FLOAT                0x0D3
#define SURFACEFORMAT_R32_SINT                       0x0D6
#define SURFACEFORMAT_R32_UINT                       0x0D7
#define SURFACEFORMAT_R32_FLOAT                      0x0D8
#define SURFACEFORMAT_R24_UNORM_X8_TYPELESS          0x0D9
#define SURFACEFORMAT_X24_TYPELESS_G8_UINT           0x0DA
#define SURFACEFORMAT_L16A16_UNORM                   0x0DF
#define SURFACEFORMAT_I24X8_UNORM                    0x0E0
#define SURFACEFORMAT_L24X8_UNORM                    0x0E1
#define SURFACEFORMAT_A24X8_UNORM                    0x0E2
#define SURFACEFORMAT_I32_FLOAT                      0x0E3
#define SURFACEFORMAT_L32_FLOAT                      0x0E4
#define SURFACEFORMAT_A32_FLOAT                      0x0E5
#define SURFACEFORMAT_B8G8R8X8_UNORM                 0x0E9
#define SURFACEFORMAT_B8G8R8X8_UNORM_SRGB            0x0EA
#define SURFACEFORMAT_R8G8B8X8_UNORM                 0x0EB
#define SURFACEFORMAT_R8G8B8X8_UNORM_SRGB            0x0EC
#define SURFACEFORMAT_R9G9B9E5_SHAREDEXP             0x0ED
#define SURFACEFORMAT_B10G10R10X2_UNORM              0x0EE
#define SURFACEFORMAT_L16A16_FLOAT                   0x0F0
#define SURFACEFORMAT_R32_UNORM                      0x0F1
#define SURFACEFORMAT_R32_SNORM                      0x0F2
#define SURFACEFORMAT_R10G10B10X2_USCALED            0x0F3
#define SURFACEFORMAT_R8G8B8A8_SSCALED               0x0F4
#define SURFACEFORMAT_R8G8B8A8_USCALED               0x0F5
#define SURFACEFORMAT_R16G16_SSCALED                 0x0F6
#define SURFACEFORMAT_R16G16_USCALED                 0x0F7
#define SURFACEFORMAT_R32_SSCALED                    0x0F8
#define SURFACEFORMAT_R32_USCALED                    0x0F9
#define SURFACEFORMAT_B5G6R5_UNORM                   0x100
#define SURFACEFORMAT_B5G6R5_UNORM_SRGB              0x101
#define SURFACEFORMAT_B5G5R5A1_UNORM                 0x102
#define SURFACEFORMAT_B5G5R5A1_UNORM_SRGB            0x103
#define SURFACEFORMAT_B4G4R4A4_UNORM                 0x104
#define SURFACEFORMAT_B4G4R4A4_UNORM_SRGB            0x105
#define SURFACEFORMAT_R8G8_UNORM                     0x106
#define SURFACEFORMAT_R8G8_SNORM                     0x107
#define SURFACEFORMAT_R8G8_SINT                      0x108
#define SURFACEFORMAT_R8G8_UINT                      0x109
#define SURFACEFORMAT_R16_UNORM                      0x10A
#define SURFACEFORMAT_R16_SNORM                      0x10B
#define SURFACEFORMAT_R16_SINT                       0x10C
#define SURFACEFORMAT_R16_UINT                       0x10D
#define SURFACEFORMAT_R16_FLOAT                      0x10E
#define SURFACEFORMAT_I16_UNORM                      0x111
#define SURFACEFORMAT_L16_UNORM                      0x112
#define SURFACEFORMAT_A16_UNORM                      0x113
#define SURFACEFORMAT_L8A8_UNORM                     0x114
#define SURFACEFORMAT_I16_FLOAT                      0x115
#define SURFACEFORMAT_L16_FLOAT                      0x116
#define SURFACEFORMAT_A16_FLOAT                      0x117
#define SURFACEFORMAT_R5G5_SNORM_B6_UNORM            0x119
#define SURFACEFORMAT_B5G5R5X1_UNORM                 0x11A
#define SURFACEFORMAT_B5G5R5X1_UNORM_SRGB            0x11B
#define SURFACEFORMAT_R8G8_SSCALED                   0x11C
#define SURFACEFORMAT_R8G8_USCALED                   0x11D
#define SURFACEFORMAT_R16_SSCALED                    0x11E
#define SURFACEFORMAT_R16_USCALED                    0x11F
#define SURFACEFORMAT_R8_UNORM                       0x140
#define SURFACEFORMAT_R8_SNORM                       0x141
#define SURFACEFORMAT_R8_SINT                        0x142
#define SURFACEFORMAT_R8_UINT                        0x143
#define SURFACEFORMAT_A8_UNORM                       0x144
#define SURFACEFORMAT_I8_UNORM                       0x145
#define SURFACEFORMAT_L8_UNORM                       0x146
#define SURFACEFORMAT_P4A4_UNORM                     0x147
#define SURFACEFORMAT_A4P4_UNORM                     0x148
#define SURFACEFORMAT_R8_SSCALED                     0x149
#define SURFACEFORMAT_R8_USCALED                     0x14A
#define SURFACEFORMAT_R1_UINT                        0x181
#define SURFACEFORMAT_YCRCB_NORMAL                   0x182
#define SURFACEFORMAT_YCRCB_SWAPUVY                  0x183
#define SURFACEFORMAT_BC1_UNORM                      0x186
#define SURFACEFORMAT_BC2_UNORM                      0x187
#define SURFACEFORMAT_BC3_UNORM                      0x188
#define SURFACEFORMAT_BC4_UNORM                      0x189
#define SURFACEFORMAT_BC5_UNORM                      0x18A
#define SURFACEFORMAT_BC1_UNORM_SRGB                 0x18B
#define SURFACEFORMAT_BC2_UNORM_SRGB                 0x18C
#define SURFACEFORMAT_BC3_UNORM_SRGB                 0x18D
#define SURFACEFORMAT_MONO8                          0x18E
#define SURFACEFORMAT_YCRCB_SWAPUV                   0x18F
#define SURFACEFORMAT_YCRCB_SWAPY                    0x190
#define SURFACEFORMAT_DXT1_RGB                       0x191
#define SURFACEFORMAT_FXT1                           0x192
#define SURFACEFORMAT_R8G8B8_UNORM                   0x193
#define SURFACEFORMAT_R8G8B8_SNORM                   0x194
#define SURFACEFORMAT_R8G8B8_SSCALED                 0x195
#define SURFACEFORMAT_R8G8B8_USCALED                 0x196
#define SURFACEFORMAT_R64G64B64A64_FLOAT             0x197
#define SURFACEFORMAT_R64G64B64_FLOAT                0x198
#define SURFACEFORMAT_BC4_SNORM                      0x199
#define SURFACEFORMAT_BC5_SNORM                      0x19A
#define SURFACEFORMAT_R16G16B16_UNORM                0x19C
#define SURFACEFORMAT_R16G16B16_SNORM                0x19D
#define SURFACEFORMAT_R16G16B16_SSCALED              0x19E
#define SURFACEFORMAT_R16G16B16_USCALED              0x19F

#define SURFACE_1D      0
#define SURFACE_2D      1
#define SURFACE_3D      2
#define SURFACE_CUBE    3
#define SURFACE_BUFFER  4
#define SURFACE_NULL    7

#define TEXCOORDMODE_WRAP            0
#define TEXCOORDMODE_MIRROR          1
#define TEXCOORDMODE_CLAMP           2
#define TEXCOORDMODE_CUBE            3
#define TEXCOORDMODE_CLAMP_BORDER    4
#define TEXCOORDMODE_MIRROR_ONCE     5

#define THREAD_PRIORITY_NORMAL   0
#define THREAD_PRIORITY_HIGH     1

#define VERTEX_SUBPIXEL_PRECISION_8BITS  0
#define VERTEX_SUBPIXEL_PRECISION_4BITS  1

#define COMPONENT_NOSTORE      0
#define COMPONENT_STORE_SRC    1
#define COMPONENT_STORE_0      2
#define COMPONENT_STORE_1_FLT  3
#define COMPONENT_STORE_1_INT  4
#define COMPONENT_STORE_VID    5
#define COMPONENT_STORE_IID    6
#define COMPONENT_STORE_PID    7

/* Execution Unit (EU) defines
 */

#define GEN8_ALIGN_1   0
#define GEN8_ALIGN_16  1

#define GEN8_ADDRESS_DIRECT                        0
#define GEN8_ADDRESS_REGISTER_INDIRECT_REGISTER    1

#define GEN8_CHANNEL_X     0
#define GEN8_CHANNEL_Y     1
#define GEN8_CHANNEL_Z     2
#define GEN8_CHANNEL_W     3

#define GEN8_COMPRESSION_NONE          0
#define GEN8_COMPRESSION_2NDHALF       1
#define GEN8_COMPRESSION_COMPRESSED    2

#define GEN8_CONDITIONAL_NONE  0
#define GEN8_CONDITIONAL_Z     1
#define GEN8_CONDITIONAL_NZ    2
#define GEN8_CONDITIONAL_EQ    1	/* Z */
#define GEN8_CONDITIONAL_NEQ   2	/* NZ */
#define GEN8_CONDITIONAL_G     3
#define GEN8_CONDITIONAL_GE    4
#define GEN8_CONDITIONAL_L     5
#define GEN8_CONDITIONAL_LE    6
#define GEN8_CONDITIONAL_C     7
#define GEN8_CONDITIONAL_O     8

#define GEN8_DEBUG_NONE        0
#define GEN8_DEBUG_BREAKPOINT  1

#define GEN8_DEPENDENCY_NORMAL         0
#define GEN8_DEPENDENCY_NOTCLEARED     1
#define GEN8_DEPENDENCY_NOTCHECKED     2
#define GEN8_DEPENDENCY_DISABLE        3

#define GEN8_EXECUTE_1     0
#define GEN8_EXECUTE_2     1
#define GEN8_EXECUTE_4     2
#define GEN8_EXECUTE_8     3
#define GEN8_EXECUTE_16    4
#define GEN8_EXECUTE_32    5

#define GEN8_HORIZONTAL_STRIDE_0   0
#define GEN8_HORIZONTAL_STRIDE_1   1
#define GEN8_HORIZONTAL_STRIDE_2   2
#define GEN8_HORIZONTAL_STRIDE_4   3

#define GEN8_INSTRUCTION_NORMAL    0
#define GEN8_INSTRUCTION_SATURATE  1

#define GEN8_OPCODE_MOV        1
#define GEN8_OPCODE_SEL        2
#define GEN8_OPCODE_NOT        4
#define GEN8_OPCODE_AND        5
#define GEN8_OPCODE_OR         6
#define GEN8_OPCODE_XOR        7
#define GEN8_OPCODE_SHR        8
#define GEN8_OPCODE_SHL        9
#define GEN8_OPCODE_RSR        10
#define GEN8_OPCODE_RSL        11
#define GEN8_OPCODE_ASR        12
#define GEN8_OPCODE_CMP        16
#define GEN8_OPCODE_JMPI       32
#define GEN8_OPCODE_IF         34
#define GEN8_OPCODE_IFF        35
#define GEN8_OPCODE_ELSE       36
#define GEN8_OPCODE_ENDIF      37
#define GEN8_OPCODE_DO         38
#define GEN8_OPCODE_WHILE      39
#define GEN8_OPCODE_BREAK      40
#define GEN8_OPCODE_CONTINUE   41
#define GEN8_OPCODE_HALT       42
#define GEN8_OPCODE_MSAVE      44
#define GEN8_OPCODE_MRESTORE   45
#define GEN8_OPCODE_PUSH       46
#define GEN8_OPCODE_POP        47
#define GEN8_OPCODE_WAIT       48
#define GEN8_OPCODE_SEND       49
#define GEN8_OPCODE_ADD        64
#define GEN8_OPCODE_MUL        65
#define GEN8_OPCODE_AVG        66
#define GEN8_OPCODE_FRC        67
#define GEN8_OPCODE_RNDU       68
#define GEN8_OPCODE_RNDD       69
#define GEN8_OPCODE_RNDE       70
#define GEN8_OPCODE_RNDZ       71
#define GEN8_OPCODE_MAC        72
#define GEN8_OPCODE_MACH       73
#define GEN8_OPCODE_LZD        74
#define GEN8_OPCODE_SAD2       80
#define GEN8_OPCODE_SADA2      81
#define GEN8_OPCODE_DP4        84
#define GEN8_OPCODE_DPH        85
#define GEN8_OPCODE_DP3        86
#define GEN8_OPCODE_DP2        87
#define GEN8_OPCODE_DPA2       88
#define GEN8_OPCODE_LINE       89
#define GEN8_OPCODE_NOP        126

#define GEN8_PREDICATE_NONE             0
#define GEN8_PREDICATE_NORMAL           1
#define GEN8_PREDICATE_ALIGN1_ANYV             2
#define GEN8_PREDICATE_ALIGN1_ALLV             3
#define GEN8_PREDICATE_ALIGN1_ANY2H            4
#define GEN8_PREDICATE_ALIGN1_ALL2H            5
#define GEN8_PREDICATE_ALIGN1_ANY4H            6
#define GEN8_PREDICATE_ALIGN1_ALL4H            7
#define GEN8_PREDICATE_ALIGN1_ANY8H            8
#define GEN8_PREDICATE_ALIGN1_ALL8H            9
#define GEN8_PREDICATE_ALIGN1_ANY16H           10
#define GEN8_PREDICATE_ALIGN1_ALL16H           11
#define GEN8_PREDICATE_ALIGN16_REPLICATE_X     2
#define GEN8_PREDICATE_ALIGN16_REPLICATE_Y     3
#define GEN8_PREDICATE_ALIGN16_REPLICATE_Z     4
#define GEN8_PREDICATE_ALIGN16_REPLICATE_W     5
#define GEN8_PREDICATE_ALIGN16_ANY4H           6
#define GEN8_PREDICATE_ALIGN16_ALL4H           7

#define GEN8_ARCHITECTURE_REGISTER_FILE    0
#define GEN8_GENERAL_REGISTER_FILE         1
#define GEN8_MESSAGE_REGISTER_FILE         2
#define GEN8_IMMEDIATE_VALUE               3

#define GEN8_REGISTER_TYPE_UD  0
#define GEN8_REGISTER_TYPE_D   1
#define GEN8_REGISTER_TYPE_UW  2
#define GEN8_REGISTER_TYPE_W   3
#define GEN8_REGISTER_TYPE_UB  4
#define GEN8_REGISTER_TYPE_B   5
#define GEN8_REGISTER_TYPE_VF  5	/* packed float vector, immediates only? */
#define GEN8_REGISTER_TYPE_HF  6
#define GEN8_REGISTER_TYPE_V   6	/* packed int vector, immediates only, uword dest only */
#define GEN8_REGISTER_TYPE_F   7

#define GEN8_ARF_NULL                  0x00
#define GEN8_ARF_ADDRESS               0x10
#define GEN8_ARF_ACCUMULATOR           0x20
#define GEN8_ARF_FLAG                  0x30
#define GEN8_ARF_MASK                  0x40
#define GEN8_ARF_MASK_STACK            0x50
#define GEN8_ARF_MASK_STACK_DEPTH      0x60
#define GEN8_ARF_STATE                 0x70
#define GEN8_ARF_CONTROL               0x80
#define GEN8_ARF_NOTIFICATION_COUNT    0x90
#define GEN8_ARF_IP                    0xA0

#define GEN8_AMASK   0
#define GEN8_IMASK   1
#define GEN8_LMASK   2
#define GEN8_CMASK   3

#define GEN8_THREAD_NORMAL     0
#define GEN8_THREAD_ATOMIC     1
#define GEN8_THREAD_SWITCH     2

#define GEN8_VERTICAL_STRIDE_0                 0
#define GEN8_VERTICAL_STRIDE_1                 1
#define GEN8_VERTICAL_STRIDE_2                 2
#define GEN8_VERTICAL_STRIDE_4                 3
#define GEN8_VERTICAL_STRIDE_8                 4
#define GEN8_VERTICAL_STRIDE_16                5
#define GEN8_VERTICAL_STRIDE_32                6
#define GEN8_VERTICAL_STRIDE_64                7
#define GEN8_VERTICAL_STRIDE_128               8
#define GEN8_VERTICAL_STRIDE_256               9
#define GEN8_VERTICAL_STRIDE_ONE_DIMENSIONAL   0xF

#define GEN8_WIDTH_1       0
#define GEN8_WIDTH_2       1
#define GEN8_WIDTH_4       2
#define GEN8_WIDTH_8       3
#define GEN8_WIDTH_16      4

#define GEN8_STATELESS_BUFFER_BOUNDARY_1K      0
#define GEN8_STATELESS_BUFFER_BOUNDARY_2K      1
#define GEN8_STATELESS_BUFFER_BOUNDARY_4K      2
#define GEN8_STATELESS_BUFFER_BOUNDARY_8K      3
#define GEN8_STATELESS_BUFFER_BOUNDARY_16K     4
#define GEN8_STATELESS_BUFFER_BOUNDARY_32K     5
#define GEN8_STATELESS_BUFFER_BOUNDARY_64K     6
#define GEN8_STATELESS_BUFFER_BOUNDARY_128K    7
#define GEN8_STATELESS_BUFFER_BOUNDARY_256K    8
#define GEN8_STATELESS_BUFFER_BOUNDARY_512K    9
#define GEN8_STATELESS_BUFFER_BOUNDARY_1M      10
#define GEN8_STATELESS_BUFFER_BOUNDARY_2M      11

#define GEN8_POLYGON_FACING_FRONT      0
#define GEN8_POLYGON_FACING_BACK       1

#define GEN8_MESSAGE_TARGET_NULL               0
#define GEN8_MESSAGE_TARGET_MATH               1
#define GEN8_MESSAGE_TARGET_SAMPLER            2
#define GEN8_MESSAGE_TARGET_GATEWAY            3
#define GEN8_MESSAGE_TARGET_DATAPORT_READ      4
#define GEN8_MESSAGE_TARGET_DATAPORT_WRITE     5
#define GEN8_MESSAGE_TARGET_URB                6
#define GEN8_MESSAGE_TARGET_THREAD_SPAWNER     7

#define GEN8_SAMPLER_RETURN_FORMAT_FLOAT32     0
#define GEN8_SAMPLER_RETURN_FORMAT_UINT32      2
#define GEN8_SAMPLER_RETURN_FORMAT_SINT32      3

#define GEN8_SAMPLER_MESSAGE_SIMD8_SAMPLE              0
#define GEN8_SAMPLER_MESSAGE_SIMD16_SAMPLE             0
#define GEN8_SAMPLER_MESSAGE_SIMD16_SAMPLE_BIAS        0
#define GEN8_SAMPLER_MESSAGE_SIMD8_KILLPIX             1
#define GEN8_SAMPLER_MESSAGE_SIMD4X2_SAMPLE_LOD        1
#define GEN8_SAMPLER_MESSAGE_SIMD16_SAMPLE_LOD         1
#define GEN8_SAMPLER_MESSAGE_SIMD4X2_SAMPLE_GRADIENTS  2
#define GEN8_SAMPLER_MESSAGE_SIMD8_SAMPLE_GRADIENTS    2
#define GEN8_SAMPLER_MESSAGE_SIMD4X2_SAMPLE_COMPARE    0
#define GEN8_SAMPLER_MESSAGE_SIMD16_SAMPLE_COMPARE     2
#define GEN8_SAMPLER_MESSAGE_SIMD4X2_RESINFO           2
#define GEN8_SAMPLER_MESSAGE_SIMD8_RESINFO             2
#define GEN8_SAMPLER_MESSAGE_SIMD16_RESINFO            2
#define GEN8_SAMPLER_MESSAGE_SIMD4X2_LD                3
#define GEN8_SAMPLER_MESSAGE_SIMD8_LD                  3
#define GEN8_SAMPLER_MESSAGE_SIMD16_LD                 3

#define GEN8_DATAPORT_OWORD_BLOCK_1_OWORDLOW   0
#define GEN8_DATAPORT_OWORD_BLOCK_1_OWORDHIGH  1
#define GEN8_DATAPORT_OWORD_BLOCK_2_OWORDS     2
#define GEN8_DATAPORT_OWORD_BLOCK_4_OWORDS     3
#define GEN8_DATAPORT_OWORD_BLOCK_8_OWORDS     4

#define GEN8_DATAPORT_OWORD_DUAL_BLOCK_1OWORD     0
#define GEN8_DATAPORT_OWORD_DUAL_BLOCK_4OWORDS    2

#define GEN8_DATAPORT_DWORD_SCATTERED_BLOCK_8DWORDS   2
#define GEN8_DATAPORT_DWORD_SCATTERED_BLOCK_16DWORDS  3

#define GEN8_DATAPORT_READ_MESSAGE_OWORD_BLOCK_READ          0
#define GEN8_DATAPORT_READ_MESSAGE_OWORD_DUAL_BLOCK_READ     1
#define GEN8_DATAPORT_READ_MESSAGE_DWORD_BLOCK_READ          2
#define GEN8_DATAPORT_READ_MESSAGE_DWORD_SCATTERED_READ      3

#define GEN8_DATAPORT_READ_TARGET_DATA_CACHE      0
#define GEN8_DATAPORT_READ_TARGET_RENDER_CACHE    1
#define GEN8_DATAPORT_READ_TARGET_SAMPLER_CACHE   2

#define GEN8_DATAPORT_RENDER_TARGET_WRITE_SIMD16_SINGLE_SOURCE                0
#define GEN8_DATAPORT_RENDER_TARGET_WRITE_SIMD16_SINGLE_SOURCE_REPLICATED     1
#define GEN8_DATAPORT_RENDER_TARGET_WRITE_SIMD8_DUAL_SOURCE_SUBSPAN01         2
#define GEN8_DATAPORT_RENDER_TARGET_WRITE_SIMD8_DUAL_SOURCE_SUBSPAN23         3
#define GEN8_DATAPORT_RENDER_TARGET_WRITE_SIMD8_SINGLE_SOURCE_SUBSPAN01       4

#define GEN8_DATAPORT_WRITE_MESSAGE_OWORD_BLOCK_WRITE                0
#define GEN8_DATAPORT_WRITE_MESSAGE_OWORD_DUAL_BLOCK_WRITE           1
#define GEN8_DATAPORT_WRITE_MESSAGE_DWORD_BLOCK_WRITE                2
#define GEN8_DATAPORT_WRITE_MESSAGE_DWORD_SCATTERED_WRITE            3
#define GEN8_DATAPORT_WRITE_MESSAGE_RENDER_TARGET_WRITE              4
#define GEN8_DATAPORT_WRITE_MESSAGE_STREAMED_VERTEX_BUFFER_WRITE     5
#define GEN8_DATAPORT_WRITE_MESSAGE_FLUSH_RENDER_CACHE               7

#define GEN8_MATH_FUNCTION_INV                              1
#define GEN8_MATH_FUNCTION_LOG                              2
#define GEN8_MATH_FUNCTION_EXP                              3
#define GEN8_MATH_FUNCTION_SQRT                             4
#define GEN8_MATH_FUNCTION_RSQ                              5
#define GEN8_MATH_FUNCTION_SIN                              6 /* was 7 */
#define GEN8_MATH_FUNCTION_COS                              7 /* was 8 */
#define GEN8_MATH_FUNCTION_SINCOS                           8 /* was 6 */
#define GEN8_MATH_FUNCTION_TAN                              9
#define GEN8_MATH_FUNCTION_POW                              10
#define GEN8_MATH_FUNCTION_INT_DIV_QUOTIENT_AND_REMAINDER   11
#define GEN8_MATH_FUNCTION_INT_DIV_QUOTIENT                 12
#define GEN8_MATH_FUNCTION_INT_DIV_REMAINDER                13

#define GEN8_MATH_INTEGER_UNSIGNED     0
#define GEN8_MATH_INTEGER_SIGNED       1

#define GEN8_MATH_PRECISION_FULL        0
#define GEN8_MATH_PRECISION_PARTIAL     1

#define GEN8_MATH_SATURATE_NONE         0
#define GEN8_MATH_SATURATE_SATURATE     1

#define GEN8_MATH_DATA_VECTOR  0
#define GEN8_MATH_DATA_SCALAR  1

#define GEN8_URB_OPCODE_WRITE  0

#define GEN8_URB_SWIZZLE_NONE          0
#define GEN8_URB_SWIZZLE_INTERLEAVE    1
#define GEN8_URB_SWIZZLE_TRANSPOSE     2

#define GEN8_SCRATCH_SPACE_SIZE_1K     0
#define GEN8_SCRATCH_SPACE_SIZE_2K     1
#define GEN8_SCRATCH_SPACE_SIZE_4K     2
#define GEN8_SCRATCH_SPACE_SIZE_8K     3
#define GEN8_SCRATCH_SPACE_SIZE_16K    4
#define GEN8_SCRATCH_SPACE_SIZE_32K    5
#define GEN8_SCRATCH_SPACE_SIZE_64K    6
#define GEN8_SCRATCH_SPACE_SIZE_128K   7
#define GEN8_SCRATCH_SPACE_SIZE_256K   8
#define GEN8_SCRATCH_SPACE_SIZE_512K   9
#define GEN8_SCRATCH_SPACE_SIZE_1M     10
#define GEN8_SCRATCH_SPACE_SIZE_2M     11

struct gen8_blend_state {
	struct {
		/* 00 */ uint32_t pad:19;
		/* 19 */ uint32_t y_dither_offset:2;
		/* 21 */ uint32_t x_dither_offset:2;
		/* 23 */ uint32_t color_dither_enable:1;
		/* 24 */ uint32_t alpha_test_function:3;
		/* 27 */ uint32_t alpha_test:1;
		/* 28 */ uint32_t alpha_to_coverage_dither:1;
		/* 29 */ uint32_t alpha_to_one:1;
		/* 30 */ uint32_t ia_blend:1;
		/* 31 */ uint32_t alpha_to_coverage:1;
	} common;

	struct {
		/* 00 */ uint32_t write_disable_blue:1;
		/* 01 */ uint32_t write_disable_green:1;
		/* 02 */ uint32_t write_disable_red:1;
		/* 03 */ uint32_t write_disable_alpha:1;
		/* 04 */ uint32_t pad0:1;
		/* 05 */ uint32_t alpha_blend_function:3;
		/* 08 */ uint32_t dest_alpha_blend_factor:5;
		/* 13 */ uint32_t source_alpha_blend_factor:5;
		/* 18 */ uint32_t color_blend_function:3;
		/* 21 */ uint32_t dest_blend_factor:5;
		/* 26 */ uint32_t source_blend_factor:5;
		/* 31 */ uint32_t color_blend:1;
		/* 32 */ uint32_t post_blend_clamp:1;
		/* 33 */ uint32_t pre_blend_clamp:1;
		/* 34 */ uint32_t color_clamp_range:2;
		/* 36 */ uint32_t pre_blend_source_only_clamp:1;
		/* 37 */ uint32_t pad1:22;
		/* 59 */ uint32_t logic_op_function:4;
		/* 63 */ uint32_t logic_op:1;
	} rt;
};

struct gen8_color_calc_state {
	struct {
		/* 00 */ uint32_t alpha_test_format:1;
		/* 01 */ uint32_t pad0:14;
		/* 15 */ uint32_t round_disable:1;
		/* 16 */ uint32_t bf_stencil_ref:8;
		/* 24 */ uint32_t stencil_ref:8;
	} cc0;

	union {
		float alpha_ref_f;
		struct {
			uint32_t ui:8;
			uint32_t pad0:24;
		} alpha_ref_fi;
	} cc1;

	float constant_r;
	float constant_g;
	float constant_b;
	float constant_a;
};

struct gen8_sampler_state {
	struct {
		/* 00 */ unsigned int aniso_algorithm:1;
		/* 01 */ unsigned int lod_bias:13;
		/* 14 */ unsigned int min_filter:3;
		/* 17 */ unsigned int mag_filter:3;
		/* 20 */ unsigned int mip_filter:2;
		/* 22 */ unsigned int base_level:5;
		/* 27 */ unsigned int lod_preclamp:2;
		/* 29 */ unsigned int default_color_mode:1;
		/* 30 */ unsigned int flexible_filter_clamp:1;
		/* 31 */ unsigned int disable:1;
	} ss0;

	struct {
		/* 00 */ unsigned int cube_control_mode:1;
		/* 01 */ unsigned int shadow_function:3;
		/* 04 */ unsigned int chroma_key_mode:1;
		/* 05 */ unsigned int chroma_key_index:2;
		/* 07 */ unsigned int chroma_key_enable:1;
		/* 08 */ unsigned int max_lod:12;
		/* 20 */ unsigned int min_lod:12;
	} ss1;

	struct {
		unsigned int pad:6;
		unsigned int default_color_pointer:26;
	} ss2;

	struct {
		/* 00 */ unsigned int r_wrap_mode:3;
		/* 03 */ unsigned int t_wrap_mode:3;
		/* 06 */ unsigned int s_wrap_mode:3;
		/* 09 */ unsigned int pad:1;
		/* 10 */ unsigned int non_normalized_coord:1;
		/* 11 */ unsigned int trilinear_quality:2;
		/* 13 */ unsigned int address_round:6;
		/* 19 */ unsigned int max_aniso:3;
		/* 22 */ unsigned int pad0:2;
		/* 24 */ unsigned int non_separable_filter:8;
	} ss3;
};

/* Surface state DW0 */
#define SURFACE_RC_READ_WRITE	(1 << 8)
#define SURFACE_TILED		(1 << 13)
#define SURFACE_TILED_Y		(1 << 12)
#define SURFACE_FORMAT_SHIFT	18
#define SURFACE_VALIGN_1	(0 << 16) /* reserved! */
#define SURFACE_VALIGN_4	(1 << 16)
#define SURFACE_VALIGN_8	(2 << 16)
#define SURFACE_VALIGN_16	(3 << 16)
#define SURFACE_HALIGN_1	(0 << 14) /* reserved! */
#define SURFACE_HALIGN_4	(1 << 14)
#define SURFACE_HALIGN_8	(2 << 14)
#define SURFACE_HALIGN_16	(3 << 14)
#define SURFACE_TYPE_SHIFT		29

/* Surface state DW2 */
#define SURFACE_HEIGHT_SHIFT        16
#define SURFACE_WIDTH_SHIFT         0

/* Surface state DW3 */
#define SURFACE_DEPTH_SHIFT         21
#define SURFACE_PITCH_SHIFT         0

#define SWIZZLE_ZERO		0
#define SWIZZLE_ONE		1
#define SWIZZLE_RED		4
#define SWIZZLE_GREEN		5
#define SWIZZLE_BLUE		6
#define SWIZZLE_ALPHA		7
#define __SURFACE_SWIZZLE(r,g,b,a) \
	((a) << 16 | (b) << 19 | (g) << 22 | (r) << 25)
#define SURFACE_SWIZZLE(r,g,b,a) \
	__SURFACE_SWIZZLE(SWIZZLE_##r, SWIZZLE_##g, SWIZZLE_##b, SWIZZLE_##a)

typedef enum {
	SAMPLER_FILTER_NEAREST = 0,
	SAMPLER_FILTER_BILINEAR,
	FILTER_COUNT
} sampler_filter_t;

typedef enum {
	SAMPLER_EXTEND_NONE = 0,
	SAMPLER_EXTEND_REPEAT,
	SAMPLER_EXTEND_PAD,
	SAMPLER_EXTEND_REFLECT,
	EXTEND_COUNT
} sampler_extend_t;

#endif
