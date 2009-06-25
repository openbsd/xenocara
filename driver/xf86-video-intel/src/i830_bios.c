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
#undef VERSION	/* XXX edid.h has a VERSION too */
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
				 (bios[_addr + 1] << 8)		\
				 (bios[_addr + 2] << 16)	\
				 (bios[_addr + 3] << 24))

static void *
find_section(struct bdb_header *bdb, int section_id)
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

/**
 * Returns the BIOS's fixed panel mode.
 *
 * Note that many BIOSes will have the appropriate tables for a panel even when
 * a panel is not attached.  Additionally, many BIOSes adjust table sizes or
 * offsets, such that this parsing fails.  Thus, almost any other method for
 * detecting the panel mode is preferable.
 */
static void
parse_panel_data(I830Ptr pI830, struct bdb_header *bdb)
{
    struct bdb_lvds_options *lvds_options;
    struct bdb_lvds_lfp_data_ptrs *lvds_lfp_data_ptrs;
    int timing_offset;
    DisplayModePtr fixed_mode;
    unsigned char *timing_ptr;

    /* Defaults if we can't find VBT info */
    pI830->lvds_dither = 0;

    lvds_options = find_section(bdb, BDB_LVDS_OPTIONS);
    if (!lvds_options)
	return;

    pI830->lvds_dither = lvds_options->pixel_dither;
    if (lvds_options->panel_type == 0xff)
	return;

    lvds_lfp_data_ptrs = find_section(bdb, BDB_LVDS_LFP_DATA_PTRS);
    if (!lvds_lfp_data_ptrs)
	return;

    timing_offset =
	lvds_lfp_data_ptrs->ptr[lvds_options->panel_type].dvo_timing_offset;
    timing_ptr = (unsigned char *)bdb + timing_offset;

    if (pI830->skip_panel_detect)
	return;

    fixed_mode = xnfalloc(sizeof(DisplayModeRec));
    memset(fixed_mode, 0, sizeof(*fixed_mode));

    /* Since lvds_bdb_2_fp_edid_dtd is just an EDID detailed timing
     * block, pull the contents out using EDID macros.
     */
    fixed_mode->HDisplay   = _H_ACTIVE(timing_ptr);
    fixed_mode->VDisplay   = _V_ACTIVE(timing_ptr);
    fixed_mode->HSyncStart = fixed_mode->HDisplay +
	_H_SYNC_OFF(timing_ptr);
    fixed_mode->HSyncEnd   = fixed_mode->HSyncStart +
	_H_SYNC_WIDTH(timing_ptr);
    fixed_mode->HTotal     = fixed_mode->HDisplay +
	_H_BLANK(timing_ptr);
    fixed_mode->VSyncStart = fixed_mode->VDisplay +
	_V_SYNC_OFF(timing_ptr);
    fixed_mode->VSyncEnd   = fixed_mode->VSyncStart +
	_V_SYNC_WIDTH(timing_ptr);
    fixed_mode->VTotal     = fixed_mode->VDisplay +
	_V_BLANK(timing_ptr);
    fixed_mode->Clock      = _PIXEL_CLOCK(timing_ptr) / 1000;
    fixed_mode->type       = M_T_PREFERRED;

    /* Some VBTs have bogus h/vtotal values */
    if (fixed_mode->HSyncEnd > fixed_mode->HTotal)
	fixed_mode->HTotal = fixed_mode->HSyncEnd + 1;
    if (fixed_mode->VSyncEnd > fixed_mode->VTotal)
	fixed_mode->VTotal = fixed_mode->VSyncEnd + 1;

    xf86SetModeDefaultName(fixed_mode);

    pI830->lvds_fixed_mode = fixed_mode;
}

static void
parse_general_features(I830Ptr pI830, struct bdb_header *bdb)
{
    struct bdb_general_features *general;

    /* Set sensible defaults in case we can't find the general block */
    pI830->tv_present = 1;

    general = find_section(bdb, BDB_GENERAL_FEATURES);
    if (!general)
	return;

    pI830->tv_present = general->int_tv_support;
    pI830->lvds_use_ssc = general->enable_ssc;
    if (pI830->lvds_use_ssc) {
	if (IS_I85X(pI830))
	    pI830->lvds_ssc_freq = general->ssc_freq ? 66 : 48;
	else
	    pI830->lvds_ssc_freq = general->ssc_freq ? 100 : 96;
    }
}

static void
parse_driver_feature(I830Ptr pI830, struct bdb_header *bdb)
{
    struct bdb_driver_feature *feature;

    /* For mobile chip, set default as true */
    if (IS_MOBILE(pI830) && !IS_I830(pI830))
	pI830->integrated_lvds = TRUE;

    /* skip pre-9xx chips which is broken to parse this block. */
    if (!IS_I9XX(pI830))
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
	pI830->integrated_lvds = FALSE;
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
int
i830_bios_init(ScrnInfoPtr pScrn)
{
    I830Ptr pI830 = I830PTR(pScrn);
    struct vbt_header *vbt;
    struct bdb_header *bdb;
    int vbt_off, bdb_off;
    unsigned char *bios;
    int ret;
    int size;

#if XSERVER_LIBPCIACCESS
    size = pI830->PciInfo->rom_size;
    if (size == 0) {
	size = INTEL_VBIOS_SIZE;
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		   "libpciaccess reported 0 rom size, guessing %dkB\n",
		   size / 1024);
    }
#else
    size = INTEL_VBIOS_SIZE;
#endif
    bios = xalloc(size);
    if (bios == NULL)
	return -1;

#if XSERVER_LIBPCIACCESS
    ret = pci_device_read_rom (pI830->PciInfo, bios);
    if (ret != 0) {
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
                  "libpciaccess failed to read %dkB video BIOS: %s\n",
                  size / 1024, strerror(-ret));
	xfree (bios);
	return -1;
    }
#else
    /* xf86ReadPciBIOS returns the length read */
    ret = xf86ReadPciBIOS(0, pI830->PciTag, 0, bios, size);
    if (ret <= 0) {
	xfree (bios);
	return -1;
    }
#endif

    vbt_off = INTEL_BIOS_16(0x1a);
    if (vbt_off >= size) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Bad VBT offset: 0x%x\n",
		   vbt_off);
	xfree(bios);
	return -1;
    }

    vbt = (struct vbt_header *)(bios + vbt_off);

    if (memcmp(vbt->signature, "$VBT", 4) != 0) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Bad VBT signature\n");
	xfree(bios);
	return -1;
    }

    /* Now that we've found the VBIOS, go scour the VBTs */
    bdb_off = vbt_off + vbt->bdb_offset;
    bdb = (struct bdb_header *)(bios + bdb_off);

    parse_general_features(pI830, bdb);
    parse_panel_data(pI830, bdb);
    parse_driver_feature(pI830, bdb);

    xfree(bios);

    return 0;
}
