/*
 * Copyright © 2008 Intel Corporation
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
 *    Zhenyu Wang <zhenyu.z.wang@intel.com>
 *
 */
#include "intel_xvmc_private.h"

#define DUMPFILE "./intel_xvmc_dump"

static int xvmc_dump = 0;
static FILE *fp = NULL;

void intel_xvmc_dump_open(void)
{
	char *d = NULL;

	if (xvmc_dump)
		return;

	if ((d = getenv("INTEL_XVMC_DUMP")))
		xvmc_dump = 1;

	if (xvmc_dump) {
		fp = fopen(DUMPFILE, "a");
		if (!fp)
			xvmc_dump = 0;
	}
}

void intel_xvmc_dump_close(void)
{
	if (xvmc_dump) {
		fclose(fp);
		xvmc_dump = 0;
	}
}

void intel_xvmc_dump_render(XvMCContext * context,
			    unsigned int picture_structure,
			    XvMCSurface * target, XvMCSurface * past,
			    XvMCSurface * future, unsigned int flags,
			    unsigned int num_macroblocks,
			    unsigned int first_macroblock,
			    XvMCMacroBlockArray * macroblock_array,
			    XvMCBlockArray * blocks)
{
	int i;
	XvMCMacroBlock *mb;

	if (!xvmc_dump)
		return;

	fprintf(fp, "========== new surface rendering ==========\n");
	fprintf(fp,
		"Context (id:%d) (surface_type_id:%d) (width:%d) (height:%d)\n",
		(int)context->context_id, context->surface_type_id,
		context->width, context->height);

	if (picture_structure == XVMC_FRAME_PICTURE)
		fprintf(fp, "picture structure: frame picture\n");
	else if (picture_structure == XVMC_TOP_FIELD)
		fprintf(fp, "picture structure: top field picture (%s)\n",
			(flags == XVMC_SECOND_FIELD) ? "second" : "first");
	else if (picture_structure == XVMC_BOTTOM_FIELD)
		fprintf(fp, "picture structure: bottom field picture (%s)\n",
			(flags == XVMC_SECOND_FIELD) ? "second" : "first");

	if (!past && !future)
		fprintf(fp, "picture type: I\n");
	else if (past && !future)
		fprintf(fp, "picture type: P\n");
	else if (past && future)
		fprintf(fp, "picture type: B\n");
	else
		fprintf(fp, "picture type: Bad!\n");

	fprintf(fp, "target picture: id (%d) width (%d) height (%d)\n",
		(int)target->surface_id, target->width, target->height);
	if (past)
		fprintf(fp, "past picture: id (%d) width (%d) height (%d)\n",
			(int)past->surface_id, past->width, past->height);
	if (future)
		fprintf(fp, "future picture: id (%d) width (%d) height (%d)\n",
			(int)future->surface_id, future->width, future->height);

	fprintf(fp, "num macroblocks: %d, first macroblocks %d\n",
		num_macroblocks, first_macroblock);

	for (i = first_macroblock; i < (first_macroblock + num_macroblocks);
	     i++) {
		mb = &macroblock_array->macro_blocks[i];

		fprintf(fp, "- MB(%d): ", i);
		fprintf(fp, "x (%d) y (%d)  ", mb->x, mb->y);
		fprintf(fp, "macroblock type (");
		if (mb->macroblock_type & XVMC_MB_TYPE_MOTION_FORWARD)
			fprintf(fp, "motion_forward ");
		if (mb->macroblock_type & XVMC_MB_TYPE_MOTION_BACKWARD)
			fprintf(fp, "motion_backward ");
		if (mb->macroblock_type & XVMC_MB_TYPE_PATTERN)
			fprintf(fp, "pattern ");
		if (mb->macroblock_type & XVMC_MB_TYPE_INTRA)
			fprintf(fp, "intra ");
		fprintf(fp, ")  ");
		fprintf(fp, "mc type ");
		if (picture_structure == XVMC_FRAME_PICTURE) {
			if (mb->motion_type & XVMC_PREDICTION_FIELD)
				fprintf(fp, "(field)  ");
			else if (mb->motion_type & XVMC_PREDICTION_FRAME)
				fprintf(fp, "(frame)  ");
			else if (mb->motion_type & XVMC_PREDICTION_DUAL_PRIME)
				fprintf(fp, "(dual-prime)  ");
			else
				fprintf(fp, "(unknown %d)  ", mb->motion_type);
		} else {	/* field */
			if (mb->motion_type & XVMC_PREDICTION_FIELD)
				fprintf(fp, "(field)  ");
			else if (mb->motion_type & XVMC_PREDICTION_DUAL_PRIME)
				fprintf(fp, "(dual-prime)  ");
			else if (mb->motion_type & XVMC_PREDICTION_16x8)
				fprintf(fp, "(16x8)  ");
			else
				fprintf(fp, "(unknown %d)  ", mb->motion_type);
		}

		if (mb->dct_type == XVMC_DCT_TYPE_FRAME)
			fprintf(fp, "dct type (frame)  ");
		else if (mb->dct_type == XVMC_DCT_TYPE_FIELD)
			fprintf(fp, "dct type (field)  ");

		fprintf(fp, "coded_block_pattern (0x%x)\n",
			mb->coded_block_pattern);

		/* XXX mv dump */
	}

}
