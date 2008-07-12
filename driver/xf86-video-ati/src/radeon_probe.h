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

#define _XF86MISC_SERVER_
#include <X11/extensions/xf86misc.h>

#include "xf86Crtc.h"

#ifdef USE_EXA
#include "exa.h"
#endif
#ifdef USE_XAA
#include "xaa.h"
#endif

extern DriverRec RADEON;

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
    CONNECTOR_UNSUPPORTED
} RADEONConnectorType;

typedef enum
{
    DAC_NONE    = 0,
    DAC_PRIMARY = 1,
    DAC_TVDAC   = 2,
    DAC_EXT     = 3
} RADEONDacType;

typedef enum
{
    TMDS_NONE    = 0,
    TMDS_INT     = 1,
    TMDS_EXT     = 2,
    TMDS_LVTMA   = 3,
    TMDS_DDIA    = 4,
    TMDS_UNIPHY  = 5
} RADEONTmdsType;

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
    RMX_CENTER
} RADEONRMXType;

typedef struct {
    uint32_t freq;
    uint32_t value;
}RADEONTMDSPll;

typedef enum
{
    OUTPUT_NONE,
    OUTPUT_VGA,
    OUTPUT_DVI_I,
    OUTPUT_DVI_D,
    OUTPUT_DVI_A,
    OUTPUT_LVDS,
    OUTPUT_STV,
    OUTPUT_CTV,
    OUTPUT_CV,
    OUTPUT_HDMI,
    OUTPUT_DP
} RADEONOutputType;

#define OUTPUT_IS_DVI ((radeon_output->type == OUTPUT_DVI_D || \
                        radeon_output->type == OUTPUT_DVI_I || \
                        radeon_output->type == OUTPUT_DVI_A))
#define OUTPUT_IS_TV ((radeon_output->type == OUTPUT_STV || \
                       radeon_output->type == OUTPUT_CTV))

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
} RADEONI2CBusRec, *RADEONI2CBusPtr;

typedef struct _RADEONCrtcPrivateRec {
#ifdef USE_XAA
    FBLinearPtr rotate_mem_xaa;
#endif
#ifdef USE_EXA
    ExaOffscreenArea *rotate_mem_exa;
#endif
    int crtc_id;
    int binding;
    uint32_t cursor_offset;
    /* Lookup table values to be set when the CRTC is enabled */
    uint8_t lut_r[256], lut_g[256], lut_b[256];

    uint32_t crtc_offset;
    int can_tile;
    Bool enabled;
} RADEONCrtcPrivateRec, *RADEONCrtcPrivatePtr;

typedef struct {
    RADEONDacType DACType;
    RADEONTmdsType TMDSType;
    RADEONConnectorType ConnectorType;
    Bool valid;
    int output_id;
    int devices;
    int hpd_mask;
    RADEONI2CBusRec ddc_i2c;
    int igp_lane_info;
} RADEONBIOSConnector;

typedef struct _RADEONOutputPrivateRec {
    int num;
    RADEONOutputType type;
    void *dev_priv;
    uint32_t ddc_line;
    RADEONDacType DACType;
    RADEONDviType DVIType;
    RADEONTmdsType TMDSType;
    RADEONConnectorType ConnectorType;
    RADEONMonitorType MonType;
    int crtc_num;
    int DDCReg;
    I2CBusPtr         pI2CBus;
    RADEONI2CBusRec   ddc_i2c;
    uint32_t          ps2_tvdac_adj;
    uint32_t          pal_tvdac_adj;
    uint32_t          ntsc_tvdac_adj;
    /* panel stuff */
    int               PanelXRes;
    int               PanelYRes;
    int               HOverPlus;
    int               HSyncWidth;
    int               HBlank;
    int               VOverPlus;
    int               VSyncWidth;
    int               VBlank;
    int               Flags;            /* Saved copy of mode flags          */
    int               PanelPwrDly;
    int               DotClock;
    RADEONTMDSPll     tmds_pll[4];
    RADEONRMXType     rmx_type;
    /* dvo */
    I2CDevPtr         DVOChip;
    RADEONI2CBusRec   dvo_i2c;
    int               dvo_i2c_slave_addr;
    Bool              dvo_duallink;
    /* TV out */
    TVStd             default_tvStd;
    TVStd             tvStd;
    int               hPos;
    int               vPos;
    int               hSize;
    float             TVRefClk;
    int               SupportedTVStds;
    Bool              tv_on;
    int               load_detection;
    /* dig block */
    int transmitter_config;
    Bool coherent_mode;
    int igp_lane_info;

    char              *name;
    int               output_id;
    int               devices;
    Bool enabled;
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
    uint32_t prim_surf_addr;
    uint32_t sec_surf_addr;
    uint32_t pitch;
    uint32_t x_offset;
    uint32_t y_offset;
    uint32_t x_start;
    uint32_t y_start;
    uint32_t x_end;
    uint32_t y_end;

    uint32_t viewport_start;
    uint32_t viewport_size;
};

struct avivo_state
{
    uint32_t hdp_fb_location;
    uint32_t mc_memory_map;
    uint32_t vga_memory_base;
    uint32_t vga_fb_start;

    uint32_t vga1_cntl;
    uint32_t vga2_cntl;

    uint32_t crtc_master_en;
    uint32_t crtc_tv_control;

    struct avivo_pll_state pll1;
    struct avivo_pll_state pll2;

    struct avivo_crtc_state crtc1;
    struct avivo_crtc_state crtc2;

    struct avivo_grph_state grph1;
    struct avivo_grph_state grph2;

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

    /* DCE3 chips */
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
    uint32_t phy[10];
    uint32_t uniphy1[8];
    uint32_t uniphy2[8];

};

/*
 * Maximum length of horizontal/vertical code timing tables for state storage
 */
#define MAX_H_CODE_TIMING_LEN 32
#define MAX_V_CODE_TIMING_LEN 32

typedef struct {
    struct avivo_state avivo;

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
    uint32_t          palette[256];
    uint32_t          palette2[256];

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

#define RADEON_MAX_CRTC 2
#define RADEON_MAX_BIOS_CONNECTOR 16

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

#endif /* _RADEON_PROBE_H_ */
