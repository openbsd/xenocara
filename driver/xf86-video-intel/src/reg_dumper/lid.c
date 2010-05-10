/*
 * Copyright © 2009 Intel Corporation
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
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Authors:
 *    Zhenyu Wang <zhenyu.z.wang@intel.com>
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <pciaccess.h>
#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "reg_dumper.h"
#include "../i810_reg.h"
#include "../i830_bios.h"

enum lid_status {
	LID_UNKNOWN = -1,
	LID_OPEN,
	LID_CLOSE,
};

#define ACPI_BUTTON "/proc/acpi/button/"
#define ACPI_LID "/proc/acpi/button/lid/"

static int i830_lvds_acpi_lid_state(void)
{
	int fd;
	DIR *button_dir;
	DIR *lid_dir;
	struct dirent *lid_dent;
	char *state_name;
	char state[64];
	enum lid_status ret = LID_UNKNOWN;

	button_dir = opendir(ACPI_BUTTON);
	/* If acpi button driver is not loaded, bypass ACPI check method */
	if (button_dir == NULL)
		goto out;
	closedir(button_dir);

	lid_dir = opendir(ACPI_LID);

	/* no acpi lid object found */
	if (lid_dir == NULL)
		goto out;

	while (1) {
		lid_dent = readdir(lid_dir);
		if (lid_dent == NULL) {
			/* no LID object */
			closedir(lid_dir);
			goto out;
		}
		if (strcmp(lid_dent->d_name, ".") &&
		    strcmp(lid_dent->d_name, "..")) {
			break;
		}
	}
	state_name = malloc(strlen(ACPI_LID) + strlen(lid_dent->d_name) + 7);
	memset(state_name, 0, sizeof(state_name));
	strcat(state_name, ACPI_LID);
	strcat(state_name, lid_dent->d_name);
	strcat(state_name, "/state");

	closedir(lid_dir);

	if ((fd = open(state_name, O_RDONLY)) == -1) {
		free(state_name);
		goto out;
	}
	free(state_name);
	if (read(fd, state, 64) == -1) {
		close(fd);
		goto out;
	}
	close(fd);
	if (strstr(state, "open"))
		ret = LID_OPEN;
	else if (strstr(state, "closed"))
		ret = LID_CLOSE;
	else			/* "unsupported" */
		ret = LID_UNKNOWN;

out:
	return ret;
}

int main(int argc, char **argv)
{
	I830Rec i830;
	I830Ptr pI830 = &i830;
	int swf14, acpi_lid;

	intel_i830rec_init(pI830);

	while (1) {
		swf14 = INREG(SWF14);

		printf("Intel LVDS Lid status:\n");
		printf("\tSWF14(0x%x) : %s\n", swf14,
		       swf14 & SWF14_LID_SWITCH_EN ? "close" : "open");

		acpi_lid = i830_lvds_acpi_lid_state();
		switch (acpi_lid) {
		case LID_UNKNOWN:
			printf("\tACPI Lid state : unknown\n");
			break;
		case LID_OPEN:
			printf("\tACPI Lid state : open\n");
			break;
		case LID_CLOSE:
			printf("\tACPI Lid state : close\n");
			break;
		}
		sleep(2);
	}
	return 0;
}
