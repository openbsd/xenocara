/*
 * Copyright © 2006 Intel Corporation
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
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Authors:
 *    Eric Anholt <eric@anholt.net>
 *
 */

#ifndef _I830_BIOS_H_
#define _I830_BIOS_H_

#include <stdint.h>
#include <xf86str.h>

struct vbt_header {
    char signature[20];			/**< Always starts with 'VBT$' */
    uint16_t version;			/**< decimal */
    uint16_t header_size;		/**< in bytes */
    uint16_t vbt_size;			/**< in bytes */
    uint8_t vbt_checksum;
    uint8_t reserved0;
    uint32_t bdb_offset;		/**< from beginning of VBT */
    uint32_t aim_offset[4];		/**< from beginning of VBT */
} __attribute__((packed));

struct bdb_header {
    char signature[16];			/**< Always 'BIOS_DATA_BLOCK' */
    uint16_t version;			/**< decimal */
    uint16_t header_size;		/**< in bytes */
    uint16_t bdb_size;			/**< in bytes */
} __attribute__((packed));

/*
 * There are several types of BIOS data blocks (BDBs), each block has
 * an ID and size in the first 3 bytes (ID in first, size in next 2).
 * Known types are listed below.
 */
#define BDB_GENERAL_FEATURES	  1
#define BDB_GENERAL_DEFINITIONS	  2
#define BDB_OLD_TOGGLE_LIST	  3
#define BDB_MODE_SUPPORT_LIST	  4
#define BDB_GENERIC_MODE_TABLE	  5
#define BDB_EXT_MMIO_REGS	  6
#define BDB_SWF_IO		  7
#define BDB_SWF_MMIO		  8
#define BDB_DOT_CLOCK_TABLE	  9
#define BDB_MODE_REMOVAL_TABLE	 10
#define BDB_CHILD_DEVICE_TABLE	 11
#define BDB_DRIVER_FEATURES	 12
#define BDB_DRIVER_PERSISTENCE	 13
#define BDB_EXT_TABLE_PTRS	 14
#define BDB_DOT_CLOCK_OVERRIDE	 15
#define BDB_DISPLAY_SELECT	 16
/* 17 rsvd */
#define BDB_DRIVER_ROTATION	 18
#define BDB_DISPLAY_REMOVE	 19
#define BDB_OEM_CUSTOM		 20
#define BDB_EFP_LIST		 21 /* workarounds for VGA hsync/vsync */
#define BDB_SDVO_LVDS_OPTIONS	 22
#define BDB_SDVO_PANEL_DTDS	 23
#define BDB_SDVO_LVDS_PNP_IDS	 24
#define BDB_SDVO_LVDS_POWER_SEQ	 25
#define BDB_TV_OPTIONS		 26
#define BDB_LVDS_OPTIONS	 40
#define BDB_LVDS_LFP_DATA_PTRS	 41
#define BDB_LVDS_LFP_DATA	 42
#define BDB_LVDS_BACKLIGHT	 43
#define BDB_LVDS_POWER		 44
#define BDB_SKIP		254 /* VBIOS private block, ignore */

struct bdb_general_features {
        /* bits 1 */
	unsigned char panel_fitting:2;
	unsigned char flexaim:1;
	unsigned char msg_enable:1;
	unsigned char clear_screen:3;
	unsigned char color_flip:1;

        /* bits 2 */
	unsigned char download_ext_vbt:1;
	unsigned char enable_ssc:1;
	unsigned char ssc_freq:1;
	unsigned char enable_lfp_on_override:1;
	unsigned char disable_ssc_ddt:1;
	unsigned char rsvd8:3; /* finish byte */

        /* bits 3 */
	unsigned char disable_smooth_vision:1;
	unsigned char single_dvi:1;
	unsigned char rsvd9:6; /* finish byte */

        /* bits 4 */
	unsigned char legacy_monitor_detect;

        /* bits 5 */
	unsigned char int_crt_support:1;
	unsigned char int_tv_support:1;
	unsigned char rsvd11:6; /* finish byte */
} __attribute__((packed));

struct bdb_general_definitions {
	/* DDC GPIO */
	unsigned char crt_ddc_gmbus_pin;

	/* DPMS bits */
	unsigned char dpms_acpi:1;
	unsigned char skip_boot_crt_detect:1;
	unsigned char dpms_aim:1;
	unsigned char rsvd1:5; /* finish byte */

	/* boot device bits */
	unsigned char boot_display[2];
	unsigned char child_dev_size;

	/* device info */
	unsigned char tv_or_lvds_info[33];
	unsigned char dev1[33];
	unsigned char dev2[33];
	unsigned char dev3[33];
	unsigned char dev4[33];
	/* may be another device block here on some platforms */
} __attribute__((packed));

struct bdb_lvds_options {
    uint8_t panel_type;
    uint8_t rsvd1;
    /* LVDS capabilities, stored in a dword */
    uint8_t pfit_mode:2;
    uint8_t pfit_text_mode_enhanced:1;
    uint8_t pfit_gfx_mode_enhanced:1;
    uint8_t pfit_ratio_auto:1;
    uint8_t pixel_dither:1;
    uint8_t lvds_edid:1;
    uint8_t rsvd2:1;
    uint8_t rsvd4;
} __attribute__((packed));

struct lvds_fp_timing {
    uint16_t x_res;
    uint16_t y_res;
    uint32_t lvds_reg;
    uint32_t lvds_reg_val;
    uint32_t pp_on_reg;
    uint32_t pp_on_reg_val;
    uint32_t pp_off_reg;
    uint32_t pp_off_reg_val;
    uint32_t pp_cycle_reg;
    uint32_t pp_cycle_reg_val;
    uint32_t pfit_reg;
    uint32_t pfit_reg_val;
    uint16_t terminator;
} __attribute__((packed));

struct lvds_dvo_timing {
    uint16_t dclk;		/**< In 10khz */
    uint8_t hactive;
    uint8_t hblank;
    uint8_t high_h;		/**< 7:4 = hactive 11:8, 3:0 = hblank 11:8 */
    uint8_t vactive;
    uint8_t vblank;
    uint8_t high_v;		/**< 7:4 = vactive 11:8, 3:0 = vblank 11:8 */
    uint8_t hsync_off;
    uint8_t hsync_pulse_width;
    uint8_t vsync_off;
    uint8_t high_hsync_off;	/**< 7:6 = hsync off 9:8 */
    uint8_t h_image;
    uint8_t v_image;
    uint8_t max_hv;
    uint8_t h_border;
    uint8_t v_border;
    uint8_t flags;
} __attribute__((packed));

struct lvds_pnp_id {
    uint16_t mfg_name;
    uint16_t product_code;
    uint32_t serial;
    uint8_t mfg_week;
    uint8_t mfg_year;
} __attribute__((packed));;

/* LFP pointer table contains entries to the struct below */
struct bdb_lvds_lfp_data_ptr {
    uint16_t fp_timing_offset; /* offsets are from start of bdb */
    uint8_t fp_table_size;
    uint16_t dvo_timing_offset;
    uint8_t dvo_table_size;
    uint16_t panel_pnp_id_offset;
    uint8_t pnp_table_size;
} __attribute__((packed));

struct bdb_lvds_lfp_data_ptrs {
    uint8_t lvds_entries;
    struct bdb_lvds_lfp_data_ptr ptr[16];
} __attribute__((packed));

struct bdb_lvds_lfp_data_entry {
    struct lvds_fp_timing fp_timing;
    struct lvds_dvo_timing dvo_timing;
    struct lvds_pnp_id pnp_id;
} __attribute__((packed));

struct bdb_lvds_lfp_data {
    struct bdb_lvds_lfp_data_entry data[16];
} __attribute__((packed));

struct aimdb_header {
    char    signature[16];
    char    oem_device[20];
    uint16_t  aimdb_version;
    uint16_t  aimdb_header_size;
    uint16_t  aimdb_size;
} __attribute__((packed));

struct aimdb_block {
    uint8_t   aimdb_id;
    uint16_t  aimdb_size;
} __attribute__((packed));

struct vch_panel_data {
    uint16_t	fp_timing_offset;
    uint8_t	fp_timing_size;
    uint16_t	dvo_timing_offset;
    uint8_t	dvo_timing_size;
    uint16_t	text_fitting_offset;
    uint8_t	text_fitting_size;
    uint16_t	graphics_fitting_offset;
    uint8_t	graphics_fitting_size;
} __attribute__((packed));

struct vch_bdb_22 {
    struct aimdb_block	    aimdb_block;
    struct vch_panel_data   panels[16];
} __attribute__((packed));

int i830_bios_init(ScrnInfoPtr pScrn);

#endif /* _I830_BIOS_H_ */
