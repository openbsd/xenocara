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

#define LVDS_CAP_EDID			(1 << 6)
#define LVDS_CAP_DITHER			(1 << 5)
#define LVDS_CAP_PFIT_AUTO_RATIO	(1 << 4)
#define LVDS_CAP_PFIT_GRAPHICS_MODE	(1 << 3)
#define LVDS_CAP_PFIT_TEXT_MODE		(1 << 2)
#define LVDS_CAP_PFIT_GRAPHICS		(1 << 1)
#define LVDS_CAP_PFIT_TEXT		(1 << 0)
struct lvds_bdb_1 {
    uint8_t id;				/**< 40 */
    uint16_t size;
    uint8_t panel_type;
    uint8_t reserved0;
    uint16_t caps;
} __attribute__((packed));

struct lvds_bdb_2_fp_params {
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

struct lvds_bdb_2_fp_edid_dtd {
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
#define FP_EDID_FLAG_VSYNC_POSITIVE	(1 << 2)
#define FP_EDID_FLAG_HSYNC_POSITIVE	(1 << 1)
} __attribute__((packed));

struct lvds_bdb_2_entry {
    uint16_t fp_params_offset;		/**< From beginning of BDB */
    uint8_t fp_params_size;
    uint16_t fp_edid_dtd_offset;
    uint8_t fp_edid_dtd_size;
    uint16_t fp_edid_pid_offset;
    uint8_t fp_edid_pid_size;
} __attribute__((packed));

struct lvds_bdb_2 {
    uint8_t id;			/**< 41 */
    uint16_t size;
    uint8_t table_size;	/* not sure on this one */
    struct lvds_bdb_2_entry panels[16];
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

unsigned char *
i830_bios_get (ScrnInfoPtr pScrn);

DisplayModePtr i830_bios_get_panel_mode(ScrnInfoPtr pScrn, Bool *wants_dither);

unsigned char *
i830_bios_get_aim_data_block (ScrnInfoPtr pScrn, int aim, int data_block);

#endif /* _I830_BIOS_H_ */
