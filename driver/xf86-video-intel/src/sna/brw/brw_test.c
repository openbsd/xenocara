/*
 * Copyright (c) 2011 Intel Corporation
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
 *    Chris Wilson <chris@chris-wilson.co.uk>
 *
 */

#include "brw_test.h"
#include <string.h>

void brw_test_compare(const char *function, int gen,
		      const struct brw_instruction *new, int num_new,
		      const struct brw_instruction *old, int num_old)
{
	int n;

	if (num_new != num_old ||
	    memcmp(new, old, num_new * sizeof(struct brw_instruction))) {
		printf ("%s: new\n", function);
		for (n = 0; n < num_new; n++)
			brw_disasm(stdout, &new[n], gen);

		printf ("%s: old\n", function);
		for (n = 0; n < num_old; n++)
			brw_disasm(stdout, &old[n], gen);
		printf ("\n");
	}
}


/* Check that we can recreate all the existing programs using the assembler */
int main(int argc, char **argv)
{
	brw_test_gen4();
	brw_test_gen5();
	brw_test_gen6();
	brw_test_gen7();

	return 0;
}
