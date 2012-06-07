/*
 * Copyright 2000 ATI Technologies Inc., Markham, Ontario, and
 *                VA Linux Systems Inc., Fremont, California.
 *
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation on the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NON-INFRINGEMENT.  IN NO EVENT SHALL ATI, VA LINUX SYSTEMS AND/OR
 * THEIR SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

/*
 * Authors:
 *   Kevin E. Martin <martin@xfree86.org>
 *
 * Modified by Marc Aurele La France <tsi@xfree86.org> for ATI driver merge.
 */

#ifndef _RADEON_PROBE_H_
#define _RADEON_PROBE_H_ 1

#include <stdint.h>
#include "xf86str.h"
#include "xf86DDC.h"
#include "randrstr.h"

#include "xf86Crtc.h"

#ifdef USE_EXA
#include "exa.h"
#endif
#ifdef USE_XAA
#include "xaa.h"
#endif

extern DriverRec RADEON;

#define RADEON_MAX_CRTC 6
#define RADEON_MAX_BIOS_CONNECTOR 16

typedef enum {
    CHIP_FAMILY_UNKNOW,
    CHIP_FAMILY_LEGACY,
    CHIP_FAMILY_RADEON,
    CHIP_FAMILY_RV100,
    CHIP_FAMILY_RS100,    /* U1 (IGP320M) or A3 (IGP320)*/
    CHIP_FAMILY_RV200,
    CHIP_FAMILY_RS200,    /* U2 (IGP330M/340M/350M) or A4 (IGP330/340/345/350), RS250 (IGP 7000) */
    CHIP_FAMILY_R200,
    CHIP_FAMILY_RV250,
    CHIP_FAMILY_RS300,    /* RS300/RS350 */
    CHIP_FAMILY_RV280,
    CHIP_FAMILY_R300,
    CHIP_FAMILY_R350,
    CHIP_FAMILY_RV350,
    CHIP_FAMILY_RV380,    /* RV370/RV380/M22/M24 */
    CHIP_FAMILY_R420,     /* R420/R423/M18 */
    CHIP_FAMILY_RV410,    /* RV410, M26 */
    CHIP_FAMILY_RS400,    /* xpress 200, 200m (RS400) Intel */
    CHIP_FAMILY_RS480,    /* xpress 200, 200m (RS410/480/482/485) AMD */
    CHIP_FAMILY_RV515,    /* rv515 */
    CHIP_FAMILY_R520,    /* r520 */
    CHIP_FAMILY_RV530,    /* rv530 */
    CHIP_FAMILY_R580,    /* r580 */
    CHIP_FAMILY_RV560,   /* rv560 */
    CHIP_FAMILY_RV570,   /* rv570 */
    CHIP_FAMILY_RS600,
    CHIP_FAMILY_RS690,
    CHIP_FAMILY_RS740,
    CHIP_FAMILY_R600,    /* r600 */
    CHIP_FAMILY_RV610,
    CHIP_FAMILY_RV630,
    CHIP_FAMILY_RV670,
    CHIP_FAMILY_RV620,
    CHIP_FAMILY_RV635,
    CHIP_FAMILY_RS780,
    CHIP_FAMILY_RS880,
    CHIP_FAMILY_RV770,   /* r700 */
    CHIP_FAMILY_RV730,
    CHIP_FAMILY_RV710,
    CHIP_FAMILY_RV740,
    CHIP_FAMILY_CEDAR,   /* evergreen */
    CHIP_FAMILY_REDWOOD,
    CHIP_FAMILY_JUNIPER,
    CHIP_FAMILY_CYPRESS,
    CHIP_FAMILY_HEMLOCK,
    CHIP_FAMILY_PALM,
    CHIP_FAMILY_SUMO,
    CHIP_FAMILY_SUMO2,
    CHIP_FAMILY_BARTS,
    CHIP_FAMILY_TURKS,
    CHIP_FAMILY_CAICOS,
    CHIP_FAMILY_CAYMAN,
    CHIP_FAMILY_ARUBA,
    CHIP_FAMILY_LAST
} RADEONChipFamily;

typedef struct {
    uint32_t pci_device_id;
    RADEONChipFamily chip_family;
    int mobility;
    int igp;
    int nocrtc2;
    int nointtvout;
    int singledac;
} RADEONCardInfo;

typedef enum
{
    MT_UNKNOWN = -1,
    MT_NONE    = 0,
    MT_CRT     = 1,
    MT_LCD     = 2,
    MT_DFP     = 3,
    MT_CTV     = 4,
    MT_STV     = 5,
    MT_CV      = 6,
    MT_HDMI    = 7, // this should really just be MT_DFP
    MT_DP      = 8
} RADEONMonitorType;

typedef enum
{
    CONNECTOR_NONE,
    CONNECTOR_VGA,
    CONNECTOR_DVI_I,
    CONNECTOR_DVI_D,
    CONNECTOR_DVI_A,
    CONNECTOR_STV,
    CONNECTOR_CTV,
    CONNECTOR_LVDS,
    CONNECTOR_DIGITAL,
    CONNECTOR_SCART,
    CONNECTOR_HDMI_TYPE_A,
    CONNECTOR_HDMI_TYPE_B,
    CONNECTOR_0XC,
    CONNECTOR_0XD,
    CONNECTOR_DIN,
    CONNECTOR_DISPLAY_PORT,
    CONNECTOR_EDP,
    CONNECTOR_UNSUPPORTED
} RADEONConnectorType;

typedef enum
{
    DVI_AUTO,
    DVI_DIGITAL,
    DVI_ANALOG
} RADEONDviType;

typedef enum
{
    RMX_OFF,
    RMX_FULL,
    RMX_CENTER,
    RMX_ASPECT
} RADEONRMXType;

typedef struct {
    uint32_t freq;
    uint32_t value;
}RADEONTMDSPll;

/* standards */
typedef enum
{
    TV_STD_NTSC      = 1,
    TV_STD_PAL       = 2,
    TV_STD_PAL_M     = 4,
    TV_STD_PAL_60    = 8,
    TV_STD_NTSC_J    = 16,
    TV_STD_SCART_PAL = 32,
    TV_STD_SECAM     = 64,
    TV_STD_PAL_CN    = 128,
} TVStd;

typedef struct
{
    Bool   valid;
    uint32_t mask_clk_reg;
    uint32_t mask_data_reg;
    uint32_t a_clk_reg;
    uint32_t a_data_reg;
    uint32_t put_clk_reg;
    uint32_t put_data_reg;
    uint32_t get_clk_reg;
    uint32_t get_data_reg;
    uint32_t mask_clk_mask;
    uint32_t mask_data_mask;
    uint32_t put_clk_mask;
    uint32_t put_data_mask;
    uint32_t get_clk_mask;
    uint32_t get_data_mask;
    uint32_t a_clk_mask;
    uint32_t a_data_mask;
    int hw_line;
    Bool hw_capable;
} RADEONI2CBusRec, *RADEONI2CBusPtr;

enum radeon_pll_algo {
    RADEON_PLL_OLD,
    RADEON_PLL_NEW
};

typedef struct _RADEONCrtcPrivateRec {
    void *crtc_rotate_mem;
    void *cursor_mem;
    int crtc_id;
    int binding;
    uint32_t cursor_offset;
    /* Lookup table values to be set when the CRTC is enabled */
    uint16_t lut_r[256], lut_g[256], lut_b[256];

    uint32_t crtc_offset;
    int can_tile;
    Bool enabled;
    Bool initialized;
    Bool scaler_enabled;
    float vsc;
    float hsc;
    int pll_id;
    enum radeon_pll_algo     pll_algo;
} RADEONCrtcPrivateRec, *RADEONCrtcPrivatePtr;

typedef struct _radeon_encoder {
    uint16_t encoder_id;
    int devices;
    void *dev_priv;
} radeon_encoder_rec, *radeon_encoder_ptr;

typedef struct _radeon_tvout {
    /* TV out */
    TVStd             default_tvStd;
    TVStd             tvStd;
    int               hPos;
    int               vPos;
    int               hSize;
    float             TVRefClk;
    int               SupportedTVStds;
    Bool              tv_on;
} radeon_tvout_rec, *radeon_tvout_ptr;

typedef struct _radeon_native_mode {
    /* panel stuff */
    int               PanelXRes;
    int               PanelYRes;
    int               HOverPlus;
    int               HSyncWidth;
    int               HBlank;
    int               VOverPlus;
    int               VSyncWidth;
    int               VBlank;
    int               Flags;
    int               DotClock;
} radeon_native_mode_rec, *radeon_native_mode_ptr;

typedef struct _radeon_tvdac {
    // tv dac
    uint32_t          ps2_tvdac_adj;
    uint32_t          pal_tvdac_adj;
    uint32_t          ntsc_tvdac_adj;
} radeon_tvdac_rec, *radeon_tvdac_ptr;

typedef struct _radeon_tmds {
    // tmds
    RADEONTMDSPll     tmds_pll[4];
} radeon_tmds_rec, *radeon_tmds_ptr;

typedef struct _radeon_lvds {
    // panel mode
    radeon_native_mode_rec native_mode;
    // lvds
    int               PanelPwrDly;
    int               lvds_misc;
    int               lvds_ss_id;
} radeon_lvds_rec, *radeon_lvds_ptr;

typedef struct _radeon_dvo {
    /* dvo */
    I2CBusPtr         pI2CBus;
    I2CDevPtr         DVOChip;
    RADEONI2CBusRec   dvo_i2c;
    int               dvo_i2c_slave_addr;
    Bool              dvo_duallink;
} radeon_dvo_rec, *radeon_dvo_ptr;

typedef struct {
    RADEONConnectorType ConnectorType;
    Bool valid;
    int output_id;
    int devices;
    int hpd_mask;
    RADEONI2CBusRec ddc_i2c;
    int igp_lane_info;
    Bool shared_ddc;
    int i2c_line_mux;
    Bool load_detection;
    Bool linkb;
    uint16_t connector_object;
    uint16_t connector_object_id;
    uint8_t ucI2cId;
    uint8_t hpd_id;
} RADEONBIOSConnector;

typedef struct _RADEONOutputPrivateRec {
    uint16_t connector_id;
    uint32_t devices;
    uint32_t active_device;
    Bool enabled;

    int  load_detection;

    // DVI/HDMI
    Bool coherent_mode;
    Bool linkb;

    RADEONConnectorType ConnectorType;
    uint16_t connector_object_id;
    RADEONDviType DVIType;
    RADEONMonitorType MonType;

    // DDC info
    I2CBusPtr         pI2CBus;
    RADEONI2CBusRec   ddc_i2c;
    Bool shared_ddc;

    Bool custom_edid;
    xf86MonPtr custom_mon;
    // router info
    // HDP info

    // panel mode
    radeon_native_mode_rec native_mode;

    // RMX
    RADEONRMXType     rmx_type;
    int               Flags;

    //tvout - move to encoder
    radeon_tvout_rec tvout;

    /* dce 3.x dig block */
    int igp_lane_info;
    int dig_encoder;

    int pixel_clock;

    /* DP - aux bus*/
    I2CBusPtr dp_pI2CBus;
    uint8_t ucI2cId;
    char dp_bus_name[20];
    uint32_t dp_i2c_addr;
    Bool dp_i2c_running;
    /* DP - general config */
    uint8_t dpcd[8];
    int dp_lane_count;
    int dp_clock;
    uint8_t hpd_id;
    int pll_id;
} RADEONOutputPrivateRec, *RADEONOutputPrivatePtr;

struct avivo_pll_state {
    uint32_t ref_div_src;
    uint32_t ref_div;
    uint32_t fb_div;
    uint32_t post_div_src;
    uint32_t post_div;
    uint32_t ext_ppll_cntl;
    uint32_t pll_cntl;
    uint32_t int_ss_cntl;
};

struct avivo_crtc_state {
    uint32_t pll_source;
    uint32_t h_total;
    uint32_t h_blank_start_end;
    uint32_t h_sync_a;
    uint32_t h_sync_a_cntl;
    uint32_t h_sync_b;
    uint32_t h_sync_b_cntl;
    uint32_t v_total;
    uint32_t v_blank_start_end;
    uint32_t v_sync_a;
    uint32_t v_sync_a_cntl;
    uint32_t v_sync_b;
    uint32_t v_sync_b_cntl;
    uint32_t control;
    uint32_t blank_control;
    uint32_t interlace_control;
    uint32_t stereo_control;
    uint32_t cursor_control;
};

struct avivo_grph_state {
    uint32_t enable;
    uint32_t control;
    uint32_t swap_control;
    uint32_t prim_surf_addr;
    uint32_t sec_surf_addr;
    uint32_t pitch;
    uint32_t prim_surf_addr_hi;
    uint32_t sec_surf_addr_hi;
    uint32_t x_offset;
    uint32_t y_offset;
    uint32_t x_start;
    uint32_t y_start;
    uint32_t x_end;
    uint32_t y_end;

    uint32_t desktop_height;
    uint32_t viewport_start;
    uint32_t viewport_size;
    uint32_t mode_data_format;
};

struct dce4_main_block_state {
    struct avivo_grph_state grph;
    uint32_t scl[6];
    uint32_t crtc[15];
    uint32_t fmt[10];
    uint32_t dig[20];
};

struct dce4_state
{

    uint32_t vga1_cntl;
    uint32_t vga2_cntl;
    uint32_t vga3_cntl;
    uint32_t vga4_cntl;
    uint32_t vga5_cntl;
    uint32_t vga6_cntl;
    uint32_t vga_render_control;

    struct dce4_main_block_state block[6];

    uint32_t vga_pll[3][3];
    uint32_t pll[2][15];
    uint32_t pll_route[6];

    uint32_t dac[2][26];
    uint32_t uniphy[6][10];

    uint32_t dig[20];
};

struct avivo_state
{
    uint32_t hdp_fb_location;
    uint32_t mc_memory_map;
    uint32_t vga_memory_base;
    uint32_t vga_fb_start;

    uint32_t vga1_cntl;
    uint32_t vga2_cntl;
    uint32_t vga3_cntl;
    uint32_t vga4_cntl;
    uint32_t vga5_cntl;
    uint32_t vga6_cntl;
    uint32_t vga_render_control;

    uint32_t crtc_master_en;
    uint32_t crtc_tv_control;
    uint32_t dc_lb_memory_split;

    struct avivo_pll_state pll[2];

    struct avivo_pll_state vga25_ppll;
    struct avivo_pll_state vga28_ppll;
    struct avivo_pll_state vga41_ppll;

    struct avivo_crtc_state crtc[2];

    struct avivo_grph_state grph[2];

    /* DDIA block on RS6xx chips */
    uint32_t ddia[37];

    /* scalers */
    uint32_t d1scl[40];
    uint32_t d2scl[40];
    uint32_t dxscl[6+2];

    /* dac regs */
    uint32_t daca[26];
    uint32_t dacb[26];

    /* tmdsa */
    uint32_t tmdsa[31];

    /* lvtma */
    uint32_t lvtma[39];

    /* dvoa */
    uint32_t dvoa[16];

    /* DCE3+ chips */
    uint32_t fmt1[18];
    uint32_t fmt2[18];
    uint32_t dig1[19];
    uint32_t dig2[19];
    uint32_t hdmi1[57];
    uint32_t hdmi2[57];
    uint32_t aux_cntl1[14];
    uint32_t aux_cntl2[14];
    uint32_t aux_cntl3[14];
    uint32_t aux_cntl4[14];
    uint32_t aux_cntl5[14];
    uint32_t aux_cntl6[14];
    uint32_t phy[10];
    uint32_t uniphy1[8];
    uint32_t uniphy2[8];
    uint32_t uniphy3[8];
    uint32_t uniphy4[8];
    uint32_t uniphy5[8];
    uint32_t uniphy6[8];

};

/*
 * Maximum length of horizontal/vertical code timing tables for state storage
 */
#define MAX_H_CODE_TIMING_LEN 32
#define MAX_V_CODE_TIMING_LEN 32

typedef struct {
    struct avivo_state avivo;
    struct dce4_state dce4;

				/* Common registers */
    uint32_t          ovr_clr;
    uint32_t          ovr_wid_left_right;
    uint32_t          ovr_wid_top_bottom;
    uint32_t          ov0_scale_cntl;
    uint32_t          mpp_tb_config;
    uint32_t          mpp_gp_config;
    uint32_t          subpic_cntl;
    uint32_t          viph_control;
    uint32_t          i2c_cntl_1;
    uint32_t          gen_int_cntl;
    uint32_t          cap0_trig_cntl;
    uint32_t          cap1_trig_cntl;
    uint32_t          bus_cntl;

    uint32_t          bios_0_scratch;
    uint32_t          bios_1_scratch;
    uint32_t          bios_2_scratch;
    uint32_t          bios_3_scratch;
    uint32_t          bios_4_scratch;
    uint32_t          bios_5_scratch;
    uint32_t          bios_6_scratch;
    uint32_t          bios_7_scratch;

    uint32_t          surface_cntl;
    uint32_t          surfaces[8][3];
    uint32_t          mc_agp_location;
    uint32_t          mc_agp_location_hi;
    uint32_t          mc_fb_location;
    uint32_t          display_base_addr;
    uint32_t          display2_base_addr;
    uint32_t          ov0_base_addr;

				/* Other registers to save for VT switches */
    uint32_t          dp_datatype;
    uint32_t          rbbm_soft_reset;
    uint32_t          clock_cntl_index;
    uint32_t          amcgpio_en_reg;
    uint32_t          amcgpio_mask;

				/* CRTC registers */
    uint32_t          crtc_gen_cntl;
    uint32_t          crtc_ext_cntl;
    uint32_t          dac_cntl;
    uint32_t          crtc_h_total_disp;
    uint32_t          crtc_h_sync_strt_wid;
    uint32_t          crtc_v_total_disp;
    uint32_t          crtc_v_sync_strt_wid;
    uint32_t          crtc_offset;
    uint32_t          crtc_offset_cntl;
    uint32_t          crtc_pitch;
    uint32_t          disp_merge_cntl;
    uint32_t          grph_buffer_cntl;
    uint32_t          crtc_more_cntl;
    uint32_t          crtc_tile_x0_y0;

				/* CRTC2 registers */
    uint32_t          crtc2_gen_cntl;
    uint32_t          dac_macro_cntl;
    uint32_t          dac2_cntl;
    uint32_t          disp_output_cntl;
    uint32_t          disp_tv_out_cntl;
    uint32_t          disp_hw_debug;
    uint32_t          disp2_merge_cntl;
    uint32_t          grph2_buffer_cntl;
    uint32_t          crtc2_h_total_disp;
    uint32_t          crtc2_h_sync_strt_wid;
    uint32_t          crtc2_v_total_disp;
    uint32_t          crtc2_v_sync_strt_wid;
    uint32_t          crtc2_offset;
    uint32_t          crtc2_offset_cntl;
    uint32_t          crtc2_pitch;
    uint32_t          crtc2_tile_x0_y0;

				/* Flat panel registers */
    uint32_t          fp_crtc_h_total_disp;
    uint32_t          fp_crtc_v_total_disp;
    uint32_t          fp_gen_cntl;
    uint32_t          fp2_gen_cntl;
    uint32_t          fp_h_sync_strt_wid;
    uint32_t          fp_h2_sync_strt_wid;
    uint32_t          fp_horz_stretch;
    uint32_t          fp_horz_vert_active;
    uint32_t          fp_panel_cntl;
    uint32_t          fp_v_sync_strt_wid;
    uint32_t          fp_v2_sync_strt_wid;
    uint32_t          fp_vert_stretch;
    uint32_t          lvds_gen_cntl;
    uint32_t          lvds_pll_cntl;
    uint32_t          tmds_pll_cntl;
    uint32_t          tmds_transmitter_cntl;

				/* Computed values for PLL */
    uint32_t          dot_clock_freq;
    uint32_t          pll_output_freq;
    int               feedback_div;
    int               reference_div;
    int               post_div;

				/* PLL registers */
    unsigned          ppll_ref_div;
    unsigned          ppll_div_3;
    uint32_t          htotal_cntl;
    uint32_t          vclk_ecp_cntl;

				/* Computed values for PLL2 */
    uint32_t          dot_clock_freq_2;
    uint32_t          pll_output_freq_2;
    int               feedback_div_2;
    int               reference_div_2;
    int               post_div_2;

				/* PLL2 registers */
    uint32_t          p2pll_ref_div;
    uint32_t          p2pll_div_0;
    uint32_t          htotal_cntl2;
    uint32_t          pixclks_cntl;

				/* Pallet */
    Bool              palette_valid;
    Bool	      palette_saved[2];
    uint32_t          palette[2][256];

    uint32_t          disp2_req_cntl1;
    uint32_t          disp2_req_cntl2;
    uint32_t          dmif_mem_cntl1;
    uint32_t          disp1_req_cntl1;

    uint32_t          fp_2nd_gen_cntl;
    uint32_t          fp2_2_gen_cntl;
    uint32_t          tmds2_cntl;
    uint32_t          tmds2_transmitter_cntl;


    /* TV out registers */
    uint32_t 	      tv_master_cntl;
    uint32_t 	      tv_htotal;
    uint32_t 	      tv_hsize;
    uint32_t 	      tv_hdisp;
    uint32_t 	      tv_hstart;
    uint32_t 	      tv_vtotal;
    uint32_t 	      tv_vdisp;
    uint32_t 	      tv_timing_cntl;
    uint32_t 	      tv_vscaler_cntl1;
    uint32_t 	      tv_vscaler_cntl2;
    uint32_t 	      tv_sync_size;
    uint32_t 	      tv_vrestart;
    uint32_t 	      tv_hrestart;
    uint32_t 	      tv_frestart;
    uint32_t 	      tv_ftotal;
    uint32_t 	      tv_clock_sel_cntl;
    uint32_t 	      tv_clkout_cntl;
    uint32_t 	      tv_data_delay_a;
    uint32_t 	      tv_data_delay_b;
    uint32_t 	      tv_dac_cntl;
    uint32_t 	      tv_pll_cntl;
    uint32_t 	      tv_pll_cntl1;
    uint32_t	      tv_pll_fine_cntl;
    uint32_t 	      tv_modulator_cntl1;
    uint32_t 	      tv_modulator_cntl2;
    uint32_t 	      tv_frame_lock_cntl;
    uint32_t 	      tv_pre_dac_mux_cntl;
    uint32_t 	      tv_rgb_cntl;
    uint32_t 	      tv_y_saw_tooth_cntl;
    uint32_t 	      tv_y_rise_cntl;
    uint32_t 	      tv_y_fall_cntl;
    uint32_t 	      tv_uv_adr;
    uint32_t	      tv_upsamp_and_gain_cntl;
    uint32_t	      tv_gain_limit_settings;
    uint32_t	      tv_linear_gain_settings;
    uint32_t	      tv_crc_cntl;
    uint32_t          tv_sync_cntl;
    uint32_t	      gpiopad_a;
    uint32_t          pll_test_cntl;

    uint16_t          h_code_timing[MAX_H_CODE_TIMING_LEN];
    uint16_t          v_code_timing[MAX_V_CODE_TIMING_LEN];

} RADEONSaveRec, *RADEONSavePtr;

typedef struct
{
    Bool HasSecondary;
    Bool              HasCRTC2;         /* All cards except original Radeon  */
    /*
     * The next two are used to make sure CRTC2 is restored before CRTC_EXT,
     * otherwise it could lead to blank screens.
     */
    Bool IsSecondaryRestored;
    Bool RestorePrimary;

    Bool ReversedDAC;	  /* TVDAC used as primary dac */
    Bool ReversedTMDS;    /* DDC_DVI is used for external TMDS */
    xf86CrtcPtr pCrtc[RADEON_MAX_CRTC];
    RADEONCrtcPrivatePtr Controller[RADEON_MAX_CRTC];

    ScrnInfoPtr pSecondaryScrn;    
    ScrnInfoPtr pPrimaryScrn;

    RADEONSaveRec     ModeReg;          /* Current mode                      */
    RADEONSaveRec     SavedReg;         /* Original (text) mode              */

    void              *MMIO;            /* Map of MMIO region                */
    int               MMIO_cnt;         /* Map of FB region refcount         */
    void              *FB;              /* Map of FB region                  */
    int               FB_cnt;           /* Map of FB region refcount         */
    int fd;                             /* for sharing across zaphod heads   */
    unsigned long     fd_wakeup_registered; /* server generation for which fd has been registered for wakeup handling */
    int dri2_info_cnt;
} RADEONEntRec, *RADEONEntPtr;

/* radeon_probe.c */
extern PciChipsets          RADEONPciChipsets[];

/* radeon_driver.c */
extern Bool                 RADEONPreInit(ScrnInfoPtr, int);
extern Bool                 RADEONScreenInit(int, ScreenPtr, int, char **);
extern Bool                 RADEONSwitchMode(int, DisplayModePtr, int);
#ifdef X_XF86MiscPassMessage
extern Bool                 RADEONHandleMessage(int, const char*, const char*,
					        char**);
#endif
extern void                 RADEONAdjustFrame(int, int, int, int);
extern Bool                 RADEONEnterVT(int, int);
extern void                 RADEONLeaveVT(int, int);
extern void                 RADEONFreeScreen(int, int);
extern ModeStatus           RADEONValidMode(int, DisplayModePtr, Bool, int);

extern const OptionInfoRec *RADEONOptionsWeak(void);

#ifdef XF86DRM_MODE
extern Bool                 RADEONPreInit_KMS(ScrnInfoPtr, int);
extern Bool                 RADEONScreenInit_KMS(int, ScreenPtr, int, char **);
extern Bool                 RADEONSwitchMode_KMS(int, DisplayModePtr, int);
extern void                 RADEONAdjustFrame_KMS(int, int, int, int);
extern Bool                 RADEONEnterVT_KMS(int, int);
extern void                 RADEONLeaveVT_KMS(int, int);
extern void RADEONFreeScreen_KMS(int scrnIndex, int flags);
#endif

#endif /* _RADEON_PROBE_H_ */
