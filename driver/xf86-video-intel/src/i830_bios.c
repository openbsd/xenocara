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
#ifdef HAVE_CONFIG_H
#include "config.h"
#undef VERSION			/* XXX edid.h has a VERSION too */
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define _PARSE_EDID_
#include "xf86.h"
#include "i830.h"
#include "i830_bios.h"
#include "edid.h"

#define INTEL_BIOS_8(_addr)	(bios[_addr])
#define INTEL_BIOS_16(_addr)	(bios[_addr] |			\
				 (bios[_addr + 1] << 8))
#define INTEL_BIOS_32(_addr)	(bios[_addr] |			\
				 (bios[_addr + 1] << 8) |	\
				 (bios[_addr + 2] << 16) |	\
				 (bios[_addr + 3] << 24))

#define		SLAVE_ADDR1	0x70
#define		SLAVE_ADDR2	0x72
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
		current_size = *((uint16_t *) (base + index));
		index += 2;
		if (current_id == section_id)
			return base + index;
		index += current_size;
	}

	return NULL;
}

static void
fill_detail_timing_data(DisplayModePtr fixed_mode, unsigned char *timing_ptr)
{
	fixed_mode->HDisplay = _H_ACTIVE(timing_ptr);
	fixed_mode->VDisplay = _V_ACTIVE(timing_ptr);
	fixed_mode->HSyncStart = fixed_mode->HDisplay + _H_SYNC_OFF(timing_ptr);
	fixed_mode->HSyncEnd = fixed_mode->HSyncStart +
	    _H_SYNC_WIDTH(timing_ptr);
	fixed_mode->HTotal = fixed_mode->HDisplay + _H_BLANK(timing_ptr);
	fixed_mode->VSyncStart = fixed_mode->VDisplay + _V_SYNC_OFF(timing_ptr);
	fixed_mode->VSyncEnd = fixed_mode->VSyncStart +
	    _V_SYNC_WIDTH(timing_ptr);
	fixed_mode->VTotal = fixed_mode->VDisplay + _V_BLANK(timing_ptr);
	fixed_mode->Clock = _PIXEL_CLOCK(timing_ptr) / 1000;
	fixed_mode->type = M_T_PREFERRED;

	/* Some VBTs have bogus h/vtotal values */
	if (fixed_mode->HSyncEnd > fixed_mode->HTotal)
		fixed_mode->HTotal = fixed_mode->HSyncEnd + 1;
	if (fixed_mode->VSyncEnd > fixed_mode->VTotal)
		fixed_mode->VTotal = fixed_mode->VSyncEnd + 1;

	xf86SetModeDefaultName(fixed_mode);

}

/**
 * Returns the BIOS's fixed panel mode.
 *
 * Note that many BIOSes will have the appropriate tables for a panel even when
 * a panel is not attached.  Additionally, many BIOSes adjust table sizes or
 * offsets, such that this parsing fails.  Thus, almost any other method for
 * detecting the panel mode is preferable.
 */
static void parse_integrated_panel_data(intel_screen_private *intel, struct bdb_header *bdb)
{
	struct bdb_lvds_options *lvds_options;
	struct bdb_lvds_lfp_data_ptrs *lvds_lfp_data_ptrs;
	struct bdb_lvds_lfp_data *lvds_data;
	struct bdb_lvds_lfp_data_entry *entry;
	DisplayModePtr fixed_mode;
	unsigned char *timing_ptr;
	int lfp_data_size;
	int dvo_offset;

	/* Defaults if we can't find VBT info */
	intel->lvds_dither = 0;

	lvds_options = find_section(bdb, BDB_LVDS_OPTIONS);
	if (!lvds_options)
		return;

	intel->lvds_dither = lvds_options->pixel_dither;
	if (lvds_options->panel_type == 0xff)
		return;

	lvds_data = find_section(bdb, BDB_LVDS_LFP_DATA);
	if (!lvds_data) {
		return;
	}

	lvds_lfp_data_ptrs = find_section(bdb, BDB_LVDS_LFP_DATA_PTRS);
	if (!lvds_lfp_data_ptrs)
		return;

	lfp_data_size = lvds_lfp_data_ptrs->ptr[1].dvo_timing_offset -
	    lvds_lfp_data_ptrs->ptr[0].dvo_timing_offset;
	dvo_offset = lvds_lfp_data_ptrs->ptr[0].dvo_timing_offset -
	    lvds_lfp_data_ptrs->ptr[0].fp_timing_offset;
	entry = (struct bdb_lvds_lfp_data_entry *)((uint8_t *) lvds_data->data +
						   (lfp_data_size *
						    lvds_options->panel_type));
	timing_ptr = (unsigned char *)entry + dvo_offset;
	if (intel->skip_panel_detect)
		return;

	fixed_mode = xnfalloc(sizeof(DisplayModeRec));
	memset(fixed_mode, 0, sizeof(*fixed_mode));

	/* Since lvds_bdb_2_fp_edid_dtd is just an EDID detailed timing
	 * block, pull the contents out using EDID macros.
	 */
	fill_detail_timing_data(fixed_mode, timing_ptr);
	intel->lvds_fixed_mode = fixed_mode;
}

static void parse_sdvo_panel_data(intel_screen_private *intel, struct bdb_header *bdb)
{
	DisplayModePtr fixed_mode;
	struct bdb_sdvo_lvds_options *sdvo_lvds_options;
	unsigned char *timing_ptr;

	intel->sdvo_lvds_fixed_mode = NULL;

	sdvo_lvds_options = find_section(bdb, BDB_SDVO_LVDS_OPTIONS);
	if (sdvo_lvds_options == NULL)
		return;

	timing_ptr = find_section(bdb, BDB_SDVO_PANEL_DTDS);
	if (timing_ptr == NULL)
		return;

	fixed_mode = xnfalloc(sizeof(DisplayModeRec));
	if (fixed_mode == NULL)
		return;

	memset(fixed_mode, 0, sizeof(*fixed_mode));
	fill_detail_timing_data(fixed_mode, timing_ptr +
				(sdvo_lvds_options->panel_type *
				 DET_TIMING_INFO_LEN));
	intel->sdvo_lvds_fixed_mode = fixed_mode;

}

static void parse_panel_data(intel_screen_private *intel, struct bdb_header *bdb)
{
	parse_integrated_panel_data(intel, bdb);
	parse_sdvo_panel_data(intel, bdb);
}

static void parse_general_features(intel_screen_private *intel, struct bdb_header *bdb)
{
	struct bdb_general_features *general;

	/* Set sensible defaults in case we can't find the general block */
	intel->tv_present = 1;

	general = find_section(bdb, BDB_GENERAL_FEATURES);
	if (!general)
		return;

	intel->tv_present = general->int_tv_support;
	intel->lvds_use_ssc = general->enable_ssc;
	if (intel->lvds_use_ssc) {
		if (IS_I85X(intel))
			intel->lvds_ssc_freq = general->ssc_freq ? 66 : 48;
		else if (IS_IGDNG(intel))
			intel->lvds_ssc_freq = general->ssc_freq ? 100 : 120;
		else
			intel->lvds_ssc_freq = general->ssc_freq ? 100 : 96;
	}
}

static void parse_driver_feature(intel_screen_private *intel, struct bdb_header *bdb)
{
	struct bdb_driver_feature *feature;

	/* For mobile chip, set default as true */
	if (IS_MOBILE(intel) && !IS_I830(intel))
		intel->integrated_lvds = TRUE;

	/* skip pre-9xx chips which is broken to parse this block. */
	if (!IS_I9XX(intel))
		return;

	/* XXX Disable this parsing, as it looks doesn't work for all
	   VBIOS. Reenable it if we could find out the reliable VBT parsing
	   for LVDS config later. */
	if (1)
		return;

	feature = find_section(bdb, BDB_DRIVER_FEATURES);
	if (!feature)
		return;

	if (feature->lvds_config != BDB_DRIVER_INT_LVDS)
		intel->integrated_lvds = FALSE;
}

static
void parse_sdvo_mapping(ScrnInfoPtr scrn, struct bdb_header *bdb)
{
	unsigned int block_size;
	uint16_t *block_ptr;
	struct bdb_general_definitions *defs;
	struct child_device_config *child;
	int i, child_device_num, count;
	struct sdvo_device_mapping *p_mapping;
	intel_screen_private *intel = intel_get_screen_private(scrn);

	defs = find_section(bdb, BDB_GENERAL_DEFINITIONS);
	if (!defs) {
		xf86DrvMsg(scrn->scrnIndex, X_WARNING,
			   "can't find the general definition blocks\n");
		return;
	}
	/* Get the block size of general defintion block */
	block_ptr = (uint16_t *) ((char *)defs - 2);
	block_size = *block_ptr;
	child_device_num = (block_size - sizeof(*defs)) / sizeof(*child);
	count = 0;

	for (i = 0; i < child_device_num; i++) {
		child = &defs->devices[i];
		if (!child->device_type) {
			/* skip invalid child device type */
			continue;
		}
		if (child->slave_addr == SLAVE_ADDR1 ||
		    child->slave_addr == SLAVE_ADDR2) {
			if (child->dvo_port != DEVICE_PORT_DVOB &&
			    child->dvo_port != DEVICE_PORT_DVOC) {
				/* skip the incorrect sdvo port */
				xf86DrvMsg(scrn->scrnIndex, X_WARNING,
					   "Incorrect SDVO port\n");
				continue;
			}
			xf86DrvMsg(scrn->scrnIndex, X_INFO,
				   "the SDVO device with slave addr %x "
				   "is found on DVO %x port\n",
				   child->slave_addr, child->dvo_port);
			/* fill the primary dvo port */
			p_mapping =
			    &(intel->sdvo_mappings[child->dvo_port - 1]);
			if (!p_mapping->initialized) {
				p_mapping->dvo_port = child->dvo_port;
				p_mapping->dvo_wiring = child->dvo_wiring;
				p_mapping->initialized = 1;
				p_mapping->slave_addr = child->slave_addr;
			} else {
				xf86DrvMsg(scrn->scrnIndex, X_WARNING,
					   "One DVO port is shared by two slave "
					   "address. Maybe it can't be handled\n");
			}
			/* If there exists the slave2_addr, maybe it is a sdvo
			 * device that contain multiple inputs. And it can't
			 * handled by SDVO driver.
			 * Ignore the dvo mapping of slave2_addr
			 * of course its mapping info won't be added.
			 */
			if (child->slave2_addr) {
				xf86DrvMsg(scrn->scrnIndex, X_WARNING,
					   "Two DVO ports uses the same slave address."
					   "Maybe it can't be handled by SDVO driver\n");
			}
			count++;
		} else {
			/* if the slave address is neither 0x70 nor 0x72, skip it. */
			continue;
		}
	}
	/* If the count is zero, it indicates that no sdvo device is found */
	if (!count)
		xf86DrvMsg(scrn->scrnIndex, X_INFO,
			   "No SDVO device is found in VBT\n");

	return;
}

#define INTEL_VBIOS_SIZE (64 * 1024)	/* XXX */

/**
 * i830_bios_init - map VBIOS, find VBT
 *
 * VBT existence is a sanity check that is relied on by other i830_bios.c code.
 * Note that it would be better to use a BIOS call to get the VBT, as BIOSes may
 * feed an updated VBT back through that, compared to what we'll fetch using
 * this method of groping around in the BIOS data.
 *
 * Returns 0 on success, nonzero on failure.
 */
int i830_bios_init(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	struct vbt_header *vbt;
	struct bdb_header *bdb;
	int vbt_off, bdb_off;
	unsigned char *bios;
	int ret;
	int size;

	size = intel->PciInfo->rom_size;
	if (size == 0) {
		size = INTEL_VBIOS_SIZE;
		xf86DrvMsg(scrn->scrnIndex, X_WARNING,
			   "libpciaccess reported 0 rom size, guessing %dkB\n",
			   size / 1024);
	}
	bios = malloc(size);
	if (bios == NULL)
		return -1;

	ret = pci_device_read_rom(intel->PciInfo, bios);
	if (ret != 0) {
		xf86DrvMsg(scrn->scrnIndex, X_WARNING,
			   "libpciaccess failed to read %dkB video BIOS: %s\n",
			   size / 1024, strerror(-ret));
		free(bios);
		return -1;
	}

	vbt_off = INTEL_BIOS_16(0x1a);
	if (vbt_off >= size) {
		xf86DrvMsg(scrn->scrnIndex, X_ERROR, "Bad VBT offset: 0x%x\n",
			   vbt_off);
		free(bios);
		return -1;
	}

	vbt = (struct vbt_header *)(bios + vbt_off);

	if (memcmp(vbt->signature, "$VBT", 4) != 0) {
		xf86DrvMsg(scrn->scrnIndex, X_ERROR, "Bad VBT signature\n");
		free(bios);
		return -1;
	}

	/* Now that we've found the VBIOS, go scour the VBTs */
	bdb_off = vbt_off + vbt->bdb_offset;
	bdb = (struct bdb_header *)(bios + bdb_off);

	parse_general_features(intel, bdb);
	parse_panel_data(intel, bdb);
	parse_driver_feature(intel, bdb);
	parse_sdvo_mapping(scrn, bdb);

	free(bios);

	return 0;
}
