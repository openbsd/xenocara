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
				 (pI830->VBIOS[_addr + 1] << 8) \
				 (pI830->VBIOS[_addr + 2] << 16) \
				 (pI830->VBIOS[_addr + 3] << 24))

#define YESNO(val) ((val) ? "yes" : "no")

static int tv_present;
static int lvds_present;
static int panel_type;

static void *find_section(struct bdb_header *bdb, int section_id)
{
	unsigned char *base = (unsigned char *)bdb;
	int index = 0;
	uint16_t total, current_size;
	unsigned char current_id;

	/* skip to first section */
	index += bdb->header_size;
	total = bdb->bdb_size;

	/* walk the sections looking for section_id */
	while (index < total) {
		current_id = *(base + index);
		index++;
		current_size = *((uint16_t *)(base + index));
		index += 2;
		if (current_id == section_id)
			return base + index;
		index += current_size;
	}

	return NULL;
}

static void dump_general_features(void *data)
{
    struct bdb_general_features *features = data;

    if (!data)
	return;

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
    printf("\tLFP on override: %s\n", YESNO(features->enable_lfp_on_override));
    printf("\tDisable SSC on clone: %s\n", YESNO(features->disable_ssc_ddt));
    printf("\tDisable smooth vision: %s\n",
	   YESNO(features->disable_smooth_vision));
    printf("\tSingle DVI for CRT/DVI: %s\n", YESNO(features->single_dvi));
    printf("\tLegacy monitor detect: %s\n",
	   YESNO(features->legacy_monitor_detect));
    printf("\tIntegrated CRT: %s\n", YESNO(features->int_crt_support));
    printf("\tIntegrated TV: %s\n", YESNO(features->int_tv_support));

    tv_present = 1; /* should be based on whether TV DAC exists */
    lvds_present = 1; /* should be based on IS_MOBILE() */
}

static void dump_general_definitions(void *data)
{
    struct bdb_general_definitions *defs = data;
    unsigned char *lvds_data = defs->tv_or_lvds_info;

    if (!data)
	return;

    printf("General definitions block:\n");

    printf("\tCRT DDC GMBUS addr: 0x%02x\n", defs->crt_ddc_gmbus_pin);
    printf("\tUse ACPI DPMS CRT power states: %s\n", YESNO(defs->dpms_acpi));
    printf("\tSkip CRT detect at boot: %s\n",
	   YESNO(defs->skip_boot_crt_detect));
    printf("\tUse DPMS on AIM devices: %s\n", YESNO(defs->dpms_aim));
    printf("\tBoot display type: 0x%02x%02x\n", defs->boot_display[1],
	   defs->boot_display[0]);
    printf("\tTV data block present: %s\n", YESNO(tv_present));
    if (tv_present)
	lvds_data += 33;
    if (lvds_present)
	printf("\tLFP DDC GMBUS addr: 0x%02x\n", lvds_data[19]);
}

static void dump_lvds_options(void *data)
{
    struct bdb_lvds_options *options = data;

    if (!data)
	return;

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
}

static void dump_lvds_data(void *data, unsigned char *base)
{
    struct bdb_lvds_lfp_data *lvds_data = data;
    int i;

    if (!data)
	return;

    printf("LVDS panel data block (preferred block marked with '*'):\n");

    for (i = 0; i < 16; i++) {
	struct bdb_lvds_lfp_data_entry *lfp_data = &lvds_data->data[i];
	uint8_t *timing_data = (uint8_t *)&lfp_data->dvo_timing;
	char marker;

	if (i == panel_type)
	    marker = '*';
	else
	    marker = ' ';

	printf("%c\tpanel type %02i: %dx%d clock %d\n", marker,
	       i, lfp_data->fp_timing.x_res, lfp_data->fp_timing.y_res,
	       _PIXEL_CLOCK(timing_data));
	printf("\t\ttimings: %d %d %d %d %d %d %d %d\n",
	       _H_ACTIVE(timing_data),
	       _H_BLANK(timing_data),
	       _H_SYNC_OFF(timing_data),
	       _H_SYNC_WIDTH(timing_data),
	       _V_ACTIVE(timing_data),
	       _V_BLANK(timing_data),
	       _V_SYNC_OFF(timing_data),
	       _V_SYNC_WIDTH(timing_data));
    }

}

int main(int argc, char **argv)
{
    int fd;
    struct vbt_header *vbt = NULL;
    struct bdb_header *bdb;
    int vbt_off, bdb_off, i;
    char *filename = "bios";
    struct stat finfo;

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
	printf("failed to stat \"%s\": %s\n", filename, strerror(errno));
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
    printf("BDB sig: %16s\n", bdb->signature);
    printf("BDB vers: %d.%d\n", bdb->version / 100, bdb->version % 100);

    dump_general_features(find_section(bdb, BDB_GENERAL_FEATURES));
    dump_general_definitions(find_section(bdb, BDB_GENERAL_DEFINITIONS));
    dump_lvds_options(find_section(bdb, BDB_LVDS_OPTIONS));
    dump_lvds_data(find_section(bdb, BDB_LVDS_LFP_DATA), bdb);

    return 0;
}
