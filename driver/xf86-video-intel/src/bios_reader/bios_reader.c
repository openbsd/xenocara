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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "../i830_bios.h"

#define _PARSE_EDID_
#include "edid.h"


/* Make a fake pI830 so we can easily pull i830_bios.c code in here. */
struct _fake_i830 {
    CARD8 *VBIOS;
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

int main(int argc, char **argv)
{
    FILE *f;
    int bios_size = 65536;
    struct vbt_header *vbt;
    struct bdb_header *bdb;
    int vbt_off, bdb_off, bdb_block_off, block_size;
    int panel_type = -1, i;
    char *filename = "bios";

    if (argc == 2)
	filename = argv[1];

    f = fopen(filename, "r");
    if (!f) {
	printf("Couldn't open %s\n", filename);
	return 1;
    }

    pI830->VBIOS = calloc(1, bios_size);
    if (fread(pI830->VBIOS, 1, bios_size, f) != bios_size)
	return 1;

    vbt_off = INTEL_BIOS_16(0x1a);
    printf("VBT offset: %08x\n", vbt_off);
    vbt = (struct vbt_header *)(pI830->VBIOS + vbt_off);
    printf("VBT sig: %20s\n", vbt->signature);
    printf("VBT vers: %d.%d\n", vbt->version / 100, vbt->version % 100);

    bdb_off = vbt_off + vbt->bdb_offset;
    bdb = (struct bdb_header *)(pI830->VBIOS + bdb_off);
    printf("BDB sig: %16s\n", bdb->signature);
    printf("BDB vers: %d.%d\n", bdb->version / 100, bdb->version % 100);
    for (bdb_block_off = bdb->header_size; bdb_block_off < bdb->bdb_size;
	 bdb_block_off += block_size)
    {
	int start = bdb_off + bdb_block_off;
	int id;
	struct lvds_bdb_1 *lvds1;
	struct lvds_bdb_2 *lvds2;
	struct lvds_bdb_2_fp_params *fpparam;
	struct lvds_bdb_2_fp_edid_dtd *fptiming;
	CARD8 *timing_ptr;

	id = INTEL_BIOS_8(start);
	block_size = INTEL_BIOS_16(start + 1) + 3;
	printf("BDB block type %03d size %d\n", id, block_size);
	switch (id) {
	case 40:
	    lvds1 = (struct lvds_bdb_1 *)(pI830->VBIOS + start);
	    panel_type = lvds1->panel_type;
	    printf("Panel type: %d, caps %04x\n", panel_type, lvds1->caps);
	    break;
	case 41:
	    if (panel_type == -1) {
		printf("Found panel block with no panel type\n");
		break;
	    }

	    lvds2 = (struct lvds_bdb_2 *)(pI830->VBIOS + start);

	    printf("Entries per table: %d\n", lvds2->table_size);
	    for (i = 0; i < 16; i++) {
		char marker;
		fpparam = (struct lvds_bdb_2_fp_params *)(pI830->VBIOS +
		    bdb_off + lvds2->panels[i].fp_params_offset);
		fptiming = (struct lvds_bdb_2_fp_edid_dtd *)(pI830->VBIOS +
		    bdb_off + lvds2->panels[i].fp_edid_dtd_offset);
		timing_ptr = pI830->VBIOS + bdb_off +
		    lvds2->panels[i].fp_edid_dtd_offset;
		if (fpparam->terminator != 0xffff) {
		    /* Apparently the offsets are wrong for some BIOSes, so we
		     * try the other offsets if we find a bad terminator.
		     */
		    fpparam = (struct lvds_bdb_2_fp_params *)(pI830->VBIOS +
			bdb_off + lvds2->panels[i].fp_params_offset + 8);
		    fptiming = (struct lvds_bdb_2_fp_edid_dtd *)(pI830->VBIOS +
			bdb_off + lvds2->panels[i].fp_edid_dtd_offset + 8);
		    timing_ptr = pI830->VBIOS + bdb_off +
			lvds2->panels[i].fp_edid_dtd_offset + 8;

		    if (fpparam->terminator != 0xffff)
			continue;
		}
		if (i == panel_type)
		    marker = '*';
		else
		    marker = ' ';
		printf("%c Panel index %02i xres %d yres %d clock %d\n", marker,
		       i, fpparam->x_res, fpparam->y_res,
		       _PIXEL_CLOCK(timing_ptr));
		printf("        %d %d %d %d %d %d %d %d\n",
		       _H_ACTIVE(timing_ptr), _H_BLANK(timing_ptr),
		       _H_SYNC_OFF(timing_ptr), _H_SYNC_WIDTH(timing_ptr),
		       _V_ACTIVE(timing_ptr), _V_BLANK(timing_ptr),
		       _V_SYNC_OFF(timing_ptr), _V_SYNC_WIDTH(timing_ptr));
	    }

	    printf("Panel of size %dx%d\n", fpparam->x_res, fpparam->y_res);
	    break;
	}
    }

    return 0;
}
