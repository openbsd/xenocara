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

#include <xf86str.h>

struct vbt_header {
    char signature[20];			/**< Always starts with 'VBT$' */
    CARD16 version;			/**< decimal */
    CARD16 header_size;			/**< in bytes */
    CARD16 vbt_size;			/**< in bytes */
    CARD8 vbt_checksum;
    CARD8 reserved0;
    CARD32 bdb_offset;			/**< from beginning of VBT */
    CARD32 aim_offset[4];		/**< from beginning of VBT */
} __attribute__((packed));

struct bdb_header {
    char signature[16];			/**< Always 'BIOS_DATA_BLOCK' */
    CARD16 version;			/**< decimal */
    CARD16 header_size;			/**< in bytes */
    CARD16 bdb_size;			/**< in bytes */
} __attribute__((packed));

#define LVDS_CAP_EDID			(1 << 6)
#define LVDS_CAP_DITHER			(1 << 5)
#define LVDS_CAP_PFIT_AUTO_RATIO	(1 << 4)
#define LVDS_CAP_PFIT_GRAPHICS_MODE	(1 << 3)
#define LVDS_CAP_PFIT_TEXT_MODE		(1 << 2)
#define LVDS_CAP_PFIT_GRAPHICS		(1 << 1)
#define LVDS_CAP_PFIT_TEXT		(1 << 0)
struct lvds_bdb_1 {
    CARD8 id;				/**< 40 */
    CARD16 size;
    CARD8 panel_type;
    CARD8 reserved0;
    CARD16 caps;
} __attribute__((packed));

struct lvds_bdb_2_fp_params {
    CARD16 x_res;
    CARD16 y_res;
    CARD32 lvds_reg;
    CARD32 lvds_reg_val;
    CARD32 pp_on_reg;
    CARD32 pp_on_reg_val;
    CARD32 pp_off_reg;
    CARD32 pp_off_reg_val;
    CARD32 pp_cycle_reg;
    CARD32 pp_cycle_reg_val;
    CARD32 pfit_reg;
    CARD32 pfit_reg_val;
    CARD16 terminator;
} __attribute__((packed));

struct lvds_bdb_2_fp_edid_dtd {
    CARD16 dclk;		/**< In 10khz */
    CARD8 hactive;
    CARD8 hblank;
    CARD8 high_h;		/**< 7:4 = hactive 11:8, 3:0 = hblank 11:8 */
    CARD8 vactive;
    CARD8 vblank;
    CARD8 high_v;		/**< 7:4 = vactive 11:8, 3:0 = vblank 11:8 */
    CARD8 hsync_off;
    CARD8 hsync_pulse_width;
    CARD8 vsync_off;
    CARD8 high_hsync_off;	/**< 7:6 = hsync off 9:8 */
    CARD8 h_image;
    CARD8 v_image;
    CARD8 max_hv;
    CARD8 h_border;
    CARD8 v_border;
    CARD8 flags;
#define FP_EDID_FLAG_VSYNC_POSITIVE	(1 << 2)
#define FP_EDID_FLAG_HSYNC_POSITIVE	(1 << 1)
} __attribute__((packed));

struct lvds_bdb_2_entry {
    CARD16 fp_params_offset;		/**< From beginning of BDB */
    CARD8 fp_params_size;
    CARD16 fp_edid_dtd_offset;
    CARD8 fp_edid_dtd_size;
    CARD16 fp_edid_pid_offset;
    CARD8 fp_edid_pid_size;
} __attribute__((packed));

struct lvds_bdb_2 {
    CARD8 id;			/**< 41 */
    CARD16 size;
    CARD8 table_size;	/* not sure on this one */
    struct lvds_bdb_2_entry panels[16];
} __attribute__((packed));

struct aimdb_header {
    char    signature[16];
    char    oem_device[20];
    CARD16  aimdb_version;
    CARD16  aimdb_header_size;
    CARD16  aimdb_size;
} __attribute__((packed));

struct aimdb_block {
    CARD8   aimdb_id;
    CARD16  aimdb_size;
} __attribute__((packed));

struct vch_panel_data {
    CARD16	fp_timing_offset;
    CARD8	fp_timing_size;
    CARD16	dvo_timing_offset;
    CARD8	dvo_timing_size;
    CARD16	text_fitting_offset;
    CARD8	text_fitting_size;
    CARD16	graphics_fitting_offset;
    CARD8	graphics_fitting_size;
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
