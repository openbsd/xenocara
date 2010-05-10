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

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "../i830_bios.h"

#include <X11/Xfuncproto.h>
#include <X11/Xmd.h>
#define _PARSE_EDID_
#include "edid.h"

/* Make a fake pI830 so we can easily pull i830_bios.c code in here. */
struct _fake_i830 {
	uint8_t *VBIOS;
};
struct _fake_i830 I830;
struct _fake_i830 *pI830 = &I830;

#define INTEL_BIOS_8(_addr)	(pI830->VBIOS[_addr])
#define INTEL_BIOS_16(_addr)	(pI830->VBIOS[_addr] | \
				 (pI830->VBIOS[_addr + 1] << 8))
#define INTEL_BIOS_32(_addr)	(pI830->VBIOS[_addr] | \
				 (pI830->VBIOS[_addr + 1] << 8) | \
				 (pI830->VBIOS[_addr + 2] << 16) | \
				 (pI830->VBIOS[_addr + 3] << 24))

#define YESNO(val) ((val) ? "yes" : "no")

struct bdb_block {
	uint8_t id;
	uint16_t size;
	void *data;
};

struct bdb_header *bdb;
static int tv_present;
static int lvds_present;
static int panel_type;

static struct bdb_block *find_section(int section_id)
{
	struct bdb_block *block;
	unsigned char *base = (unsigned char *)bdb;
	int index = 0;
	uint16_t total, current_size;
	unsigned char current_id;

	/* skip to first section */
	index += bdb->header_size;
	total = bdb->bdb_size;

	block = malloc(sizeof(*block));
	if (!block) {
		fprintf(stderr, "out of memory\n");
		exit(-1);
	}

	/* walk the sections looking for section_id */
	while (index < total) {
		current_id = *(base + index);
		index++;
		current_size = *((uint16_t *) (base + index));
		index += 2;
		if (current_id == section_id) {
			block->id = current_id;
			block->size = current_size;
			block->data = base + index;
			return block;
		}
		index += current_size;
	}

	free(block);
	return NULL;
}

static void dump_general_features(void)
{
	struct bdb_general_features *features;
	struct bdb_block *block;

	block = find_section(BDB_GENERAL_FEATURES);

	if (!block)
		return;

	features = block->data;

	printf("General features block:\n");

	printf("\tPanel fitting: ");
	switch (features->panel_fitting) {
	case 0:
		printf("disabled\n");
		break;
	case 1:
		printf("text only\n");
		break;
	case 2:
		printf("graphics only\n");
		break;
	case 3:
		printf("text & graphics\n");
		break;
	}
	printf("\tFlexaim: %s\n", YESNO(features->flexaim));
	printf("\tMessage: %s\n", YESNO(features->msg_enable));
	printf("\tClear screen: %d\n", features->clear_screen);
	printf("\tDVO color flip required: %s\n", YESNO(features->color_flip));
	printf("\tExternal VBT: %s\n", YESNO(features->download_ext_vbt));
	printf("\tEnable SSC: %s\n", YESNO(features->enable_ssc));
	if (features->enable_ssc)
		printf("\tSSC frequency: %s\n", features->ssc_freq ?
		       "100 MHz (66 MHz on 855)" : "96 MHz (48 MHz on 855)");
	printf("\tLFP on override: %s\n",
	       YESNO(features->enable_lfp_on_override));
	printf("\tDisable SSC on clone: %s\n",
	       YESNO(features->disable_ssc_ddt));
	printf("\tDisable smooth vision: %s\n",
	       YESNO(features->disable_smooth_vision));
	printf("\tSingle DVI for CRT/DVI: %s\n", YESNO(features->single_dvi));
	printf("\tLegacy monitor detect: %s\n",
	       YESNO(features->legacy_monitor_detect));
	printf("\tIntegrated CRT: %s\n", YESNO(features->int_crt_support));
	printf("\tIntegrated TV: %s\n", YESNO(features->int_tv_support));

	tv_present = 1;		/* should be based on whether TV DAC exists */
	lvds_present = 1;	/* should be based on IS_MOBILE() */

	free(block);
}

static void dump_backlight_info(void)
{
	struct bdb_block *block;
	struct bdb_lvds_backlight *backlight;
	struct blc_struct *blc;

	block = find_section(BDB_LVDS_BACKLIGHT);

	if (!block)
		return;

	backlight = block->data;

	printf("Backlight info block (len %d):\n", block->size);

	if (sizeof(struct blc_struct) != backlight->blcstruct_size) {
		printf
		    ("\tBacklight struct sizes don't match (expected %d, got %d), skipping\n",
		     sizeof(struct blc_struct), backlight->blcstruct_size);
		return;
	}

	blc = &backlight->panels[panel_type];

	printf("\tInverter type: %d\n", blc->inverter_type);
	printf("\t     polarity: %d\n", blc->inverter_polarity);
	printf("\t    GPIO pins: %d\n", blc->gpio_pins);
	printf("\t  GMBUS speed: %d\n", blc->gmbus_speed);
	printf("\t     PWM freq: %d\n", blc->pwm_freq);
	printf("\tMinimum brightness: %d\n", blc->min_brightness);
	printf("\tI2C slave addr: 0x%02x\n", blc->i2c_slave_addr);
	printf("\tI2C command: 0x%02x\n", blc->i2c_cmd);
}

static void dump_general_definitions(void)
{
	struct bdb_block *block;
	struct bdb_general_definitions *defs;
	struct child_device_config *child;
	int i;
	char child_id[11];
	int child_device_num;

	block = find_section(BDB_GENERAL_DEFINITIONS);

	if (!block)
		return;

	defs = block->data;

	printf("General definitions block:\n");

	printf("\tCRT DDC GMBUS addr: 0x%02x\n", defs->crt_ddc_gmbus_pin);
	printf("\tUse ACPI DPMS CRT power states: %s\n",
	       YESNO(defs->dpms_acpi));
	printf("\tSkip CRT detect at boot: %s\n",
	       YESNO(defs->skip_boot_crt_detect));
	printf("\tUse DPMS on AIM devices: %s\n", YESNO(defs->dpms_aim));
	printf("\tBoot display type: 0x%02x%02x\n", defs->boot_display[1],
	       defs->boot_display[0]);
	printf("\tTV data block present: %s\n", YESNO(tv_present));
	child_device_num = (block->size - sizeof(*defs)) / sizeof(*child);
	for (i = 0; i < child_device_num; i++) {
		child = &defs->devices[i];
		if (!child->device_type) {
			printf("\tChild device %d not present\n", i);
			continue;
		}
		strncpy(child_id, (char *)child->device_id, 10);
		child_id[10] = 0;
		printf("\tChild %d device info:\n", i);
		printf("\t\tSignature: %s\n", child_id);
		printf("\t\tAIM offset: %d\n", child->addin_offset);
		printf("\t\tDVO port: 0x%02x\n", child->dvo_port);
	}

	free(block);
}

#if 0
static void dump_child_devices(void)
{
	struct bdb_block *block;
	struct bdb_child_devices *child_devs;
	struct child_device_config *child;
	int i;

	block = find_section(BDB_CHILD_DEVICE_TABLE);
	if (!block) {
		printf("No child device table found\n");
		return;
	}

	child_devs = block->data;

	printf("Child devices block:\n");
	for (i = 0; i < DEVICE_CHILD_SIZE; i++) {
		child = &child_devs->children[i];
		/* Skip nonexistent children */
		if (!child->device_type)
			continue;
		printf("\tChild device %d\n", i);
		printf("\t\tType: 0x%04x\n", child->device_type);
		printf("\t\tDVO port: 0x%02x\n", child->dvo_port);
		printf("\t\tI2C pin: 0x%02x\n", child->i2c_pin);
		printf("\t\tSlave addr: 0x%02x\n", child->slave_addr);
		printf("\t\tDDC pin: 0x%02x\n", child->ddc_pin);
		printf("\t\tDVO config: 0x%02x\n", child->dvo_cfg);
		printf("\t\tDVO wiring: 0x%02x\n", child->dvo_wiring);
	}

	free(block);
}
#endif

static void dump_lvds_options(void)
{
	struct bdb_block *block;
	struct bdb_lvds_options *options;

	block = find_section(BDB_LVDS_OPTIONS);
	if (!block) {
		printf("No LVDS options block\n");
		return;
	}

	options = block->data;

	printf("LVDS options block:\n");

	panel_type = options->panel_type;
	printf("\tPanel type: %d\n", panel_type);
	printf("\tLVDS EDID available: %s\n", YESNO(options->lvds_edid));
	printf("\tPixel dither: %s\n", YESNO(options->pixel_dither));
	printf("\tPFIT auto ratio: %s\n", YESNO(options->pfit_ratio_auto));
	printf("\tPFIT enhanced graphics mode: %s\n",
	       YESNO(options->pfit_gfx_mode_enhanced));
	printf("\tPFIT enhanced text mode: %s\n",
	       YESNO(options->pfit_text_mode_enhanced));
	printf("\tPFIT mode: %d\n", options->pfit_mode);

	free(block);
}

static void dump_lvds_ptr_data(void)
{
	struct bdb_block *block;
	struct bdb_lvds_lfp_data *lvds_data;
	struct bdb_lvds_lfp_data_ptrs *ptrs;
	struct lvds_fp_timing *fp_timing;
	struct bdb_lvds_lfp_data_entry *entry;
	int lfp_data_size;

	block = find_section(BDB_LVDS_LFP_DATA_PTRS);
	if (!block) {
		printf("No LFP data pointers block\n");
		return;
	}
	ptrs = block->data;

	block = find_section(BDB_LVDS_LFP_DATA);
	if (!block) {
		printf("No LVDS data block\n");
		return;
	}
	lvds_data = block->data;

	lfp_data_size =
	    ptrs->ptr[1].fp_timing_offset - ptrs->ptr[0].fp_timing_offset;
	entry =
	    (struct bdb_lvds_lfp_data_entry *)((uint8_t *) lvds_data->data +
					       (lfp_data_size * panel_type));
	fp_timing = &entry->fp_timing;

	printf("LVDS timing pointer data:\n");
	printf("  Number of entries: %d\n", ptrs->lvds_entries);

	printf("\tpanel type %02i: %dx%d\n", panel_type, fp_timing->x_res,
	       fp_timing->y_res);

	free(block);
}

static void dump_lvds_data(void)
{
	struct bdb_block *block;
	struct bdb_lvds_lfp_data *lvds_data;
	struct bdb_lvds_lfp_data_ptrs *ptrs;
	int num_entries;
	int i;
	int hdisplay, hsyncstart, hsyncend, htotal;
	int vdisplay, vsyncstart, vsyncend, vtotal;
	float clock;
	int lfp_data_size, dvo_offset;

	block = find_section(BDB_LVDS_LFP_DATA_PTRS);
	if (!block) {
		printf("No LVDS ptr block\n");
		return;
	}
	ptrs = block->data;
	lfp_data_size =
	    ptrs->ptr[1].fp_timing_offset - ptrs->ptr[0].fp_timing_offset;
	dvo_offset =
	    ptrs->ptr[0].dvo_timing_offset - ptrs->ptr[0].fp_timing_offset;
	free(block);

	block = find_section(BDB_LVDS_LFP_DATA);
	if (!block) {
		printf("No LVDS data block\n");
		return;
	}

	lvds_data = block->data;
	num_entries = block->size / lfp_data_size;

	printf("LVDS panel data block (preferred block marked with '*'):\n");
	printf("  Number of entries: %d\n", num_entries);

	for (i = 0; i < num_entries; i++) {
		uint8_t *lfp_data_ptr =
		    (uint8_t *) lvds_data->data + lfp_data_size * i;
		uint8_t *timing_data = lfp_data_ptr + dvo_offset;
		struct bdb_lvds_lfp_data_entry *lfp_data =
		    (struct bdb_lvds_lfp_data_entry *)lfp_data_ptr;
		char marker;

		if (i == panel_type)
			marker = '*';
		else
			marker = ' ';

		hdisplay = _H_ACTIVE(timing_data);
		hsyncstart = hdisplay + _H_SYNC_OFF(timing_data);
		hsyncend = hsyncstart + _H_SYNC_WIDTH(timing_data);
		htotal = hdisplay + _H_BLANK(timing_data);

		vdisplay = _V_ACTIVE(timing_data);
		vsyncstart = vdisplay + _V_SYNC_OFF(timing_data);
		vsyncend = vsyncstart + _V_SYNC_WIDTH(timing_data);
		vtotal = vdisplay + _V_BLANK(timing_data);
		clock = _PIXEL_CLOCK(timing_data) / 1000;

		printf("%c\tpanel type %02i: %dx%d clock %d\n", marker,
		       i, lfp_data->fp_timing.x_res, lfp_data->fp_timing.y_res,
		       _PIXEL_CLOCK(timing_data));
		printf("\t\tinfo:\n");
		printf("\t\t  LVDS: 0x%08lx\n",
		       (unsigned long)lfp_data->fp_timing.lvds_reg_val);
		printf("\t\t  PP_ON_DELAYS: 0x%08lx\n",
		       (unsigned long)lfp_data->fp_timing.pp_on_reg_val);
		printf("\t\t  PP_OFF_DELAYS: 0x%08lx\n",
		       (unsigned long)lfp_data->fp_timing.pp_off_reg_val);
		printf("\t\t  PP_DIVISOR: 0x%08lx\n",
		       (unsigned long)lfp_data->fp_timing.pp_cycle_reg_val);
		printf("\t\t  PFIT: 0x%08lx\n",
		       (unsigned long)lfp_data->fp_timing.pfit_reg_val);
		printf("\t\ttimings: %d %d %d %d %d %d %d %d %.2f (%s)\n",
		       hdisplay, hsyncstart, hsyncend, htotal,
		       vdisplay, vsyncstart, vsyncend, vtotal, clock,
		       (hsyncend > htotal || vsyncend > vtotal) ?
		       "BAD!" : "good");
	}
	free(block);
}

static void dump_driver_feature(void)
{
	struct bdb_block *block;
	struct bdb_driver_feature *feature;

	block = find_section(BDB_DRIVER_FEATURES);
	if (!block) {
		printf("No Driver feature data block\n");
		return;
	}
	feature = block->data;

	printf("Driver feature Data Block:\n");
	printf("\tBoot Device Algorithm: %s\n", feature->boot_dev_algorithm ?
	       "driver default" : "os default");
	printf("\tBlock display switching when DVD active: %s\n",
	       YESNO(feature->block_display_switch));
	printf("\tAllow display switching when in Full Screen DOS: %s\n",
	       YESNO(feature->allow_display_switch));
	printf("\tHot Plug DVO: %s\n", YESNO(feature->hotplug_dvo));
	printf("\tDual View Zoom: %s\n", YESNO(feature->dual_view_zoom));
	printf("\tDriver INT 15h hook: %s\n", YESNO(feature->int15h_hook));
	printf("\tEnable Sprite in Clone Mode: %s\n",
	       YESNO(feature->sprite_in_clone));
	printf("\tUse 00000110h ID for Primary LFP: %s\n",
	       YESNO(feature->primary_lfp_id));
	printf("\tBoot Mode X: %u\n", feature->boot_mode_x);
	printf("\tBoot Mode Y: %u\n", feature->boot_mode_y);
	printf("\tBoot Mode Bpp: %u\n", feature->boot_mode_bpp);
	printf("\tBoot Mode Refresh: %u\n", feature->boot_mode_refresh);
	printf("\tEnable LFP as primary: %s\n",
	       YESNO(feature->enable_lfp_primary));
	printf("\tSelective Mode Pruning: %s\n",
	       YESNO(feature->selective_mode_pruning));
	printf("\tDual-Frequency Graphics Technology: %s\n",
	       YESNO(feature->dual_frequency));
	printf("\tDefault Render Clock Frequency: %s\n",
	       feature->render_clock_freq ? "low" : "high");
	printf("\tNT 4.0 Dual Display Clone Support: %s\n",
	       YESNO(feature->nt_clone_support));
	printf("\tDefault Power Scheme user interface: %s\n",
	       feature->power_scheme_ui ? "3rd party" : "CUI");
	printf
	    ("\tSprite Display Assignment when Overlay is Active in Clone Mode: %s\n",
	     feature->sprite_display_assign ? "primary" : "secondary");
	printf("\tDisplay Maintain Aspect Scaling via CUI: %s\n",
	       YESNO(feature->cui_aspect_scaling));
	printf("\tPreserve Aspect Ratio: %s\n",
	       YESNO(feature->preserve_aspect_ratio));
	printf("\tEnable SDVO device power down: %s\n",
	       YESNO(feature->sdvo_device_power_down));
	printf("\tCRT hotplug: %s\n", YESNO(feature->crt_hotplug));
	printf("\tLVDS config: ");
	switch (feature->lvds_config) {
	case BDB_DRIVER_NO_LVDS:
		printf("No LVDS\n");
		break;
	case BDB_DRIVER_INT_LVDS:
		printf("Integrated LVDS\n");
		break;
	case BDB_DRIVER_SDVO_LVDS:
		printf("SDVO LVDS\n");
		break;
	case BDB_DRIVER_EDP:
		printf("Embedded DisplayPort\n");
		break;
	}
	printf("\tDefine Display statically: %s\n",
	       YESNO(feature->static_display));
	printf("\tLegacy CRT max X: %d\n", feature->legacy_crt_max_x);
	printf("\tLegacy CRT max Y: %d\n", feature->legacy_crt_max_y);
	printf("\tLegacy CRT max refresh: %d\n",
	       feature->legacy_crt_max_refresh);
	free(block);
}

int main(int argc, char **argv)
{
	int fd;
	struct vbt_header *vbt = NULL;
	int vbt_off, bdb_off, i;
	char *filename = "bios";
	struct stat finfo;
	struct bdb_block *block;
	char signature[17];

	if (argc != 2) {
		printf("usage: %s <rom file>\n", argv[0]);
		return 1;
	}

	filename = argv[1];

	fd = open(filename, O_RDONLY);
	if (fd == -1) {
		printf("Couldn't open \"%s\": %s\n", filename, strerror(errno));
		return 1;
	}

	if (stat(filename, &finfo)) {
		printf("failed to stat \"%s\": %s\n", filename,
		       strerror(errno));
		return 1;
	}

	pI830->VBIOS = mmap(NULL, finfo.st_size, PROT_READ, MAP_SHARED, fd, 0);
	if (pI830->VBIOS == MAP_FAILED) {
		printf("failed to map \"%s\": %s\n", filename, strerror(errno));
		return 1;
	}

	/* Scour memory looking for the VBT signature */
	for (i = 0; i + 4 < finfo.st_size; i++) {
		if (!memcmp(pI830->VBIOS + i, "$VBT", 4)) {
			vbt_off = i;
			vbt = (struct vbt_header *)(pI830->VBIOS + i);
			break;
		}
	}

	if (!vbt) {
		printf("VBT signature missing\n");
		return 1;
	}

	printf("VBT vers: %d.%d\n", vbt->version / 100, vbt->version % 100);

	bdb_off = vbt_off + vbt->bdb_offset;
	bdb = (struct bdb_header *)(pI830->VBIOS + bdb_off);
	strncpy(signature, (char *)bdb->signature, 16);
	signature[16] = 0;
	printf("BDB sig: %s\n", signature);
	printf("BDB vers: %d.%d\n", bdb->version / 100, bdb->version % 100);

	printf("Available sections: ");
	for (i = 0; i < 256; i++) {
		block = find_section(i);
		if (!block)
			continue;
		printf("%d ", i);
		free(block);
	}
	printf("\n");

	dump_general_features();
	dump_general_definitions();
//    dump_child_devices();
	dump_lvds_options();
	dump_lvds_data();
	dump_lvds_ptr_data();
	dump_backlight_info();

	dump_driver_feature();

	return 0;
}
