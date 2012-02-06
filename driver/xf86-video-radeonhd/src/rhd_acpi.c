/*
 * Copyright 2009  Luc Verhaegen <libv@exsuse.de>
 * Copyright 2009  Matthias Hopf <mhopf@novell.com>
 * Copyright 2009  Egbert Eich   <eich@novell.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#if HAVE_XF86_ANSIC_H
# include "xf86_ansic.h"
#else
# include <unistd.h>
# include <sys/types.h>
# include <dirent.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <stdio.h>
# include <string.h>
# include <errno.h>
#endif

#include "xf86.h"

#include "rhd.h"
#include "rhd_connector.h"
#include "rhd_output.h"
#include "rhd_acpi.h"


#define ACPI_PATH "/sys/class/backlight"

/*
 *
 */
static Bool
rhdDoBacklight(struct rhdOutput *Output, Bool do_write, int *val)
{
    DIR *dir = opendir(ACPI_PATH);
    struct dirent *dirent;
    char buf[10];
    RHDFUNC(Output);

    if (!dir)
	return -1;

    while ((dirent = readdir(dir)) != NULL) {
	char path[PATH_MAX];
	int fd_max;

	snprintf(path,PATH_MAX,"%s/%s/max_brightness",ACPI_PATH,dirent->d_name);
	if ((fd_max = open(path,  O_RDONLY)) > 0) {
	    int max_val;

	    while ((read(fd_max,buf,9) == -1)
		   && (errno == EINTR || errno == EAGAIN)) {};
	    close (fd_max);

	    if (sscanf(buf,"%i\n",&max_val) == 1) {
		int fd;

		snprintf(path,PATH_MAX,"%s/%s/%s",ACPI_PATH,dirent->d_name,
			 do_write ? "brightness" : "actual_brightness");
		if ((fd = open(path, do_write ? O_WRONLY : O_RDONLY)) > 0) {

		    if (do_write) {

			snprintf(buf,10,"%i\n",(*val * max_val) / RHD_BACKLIGHT_PROPERTY_MAX);
			while ((write(fd,buf,strlen(buf)) <= 0)
			       && (errno == EINTR || errno == EAGAIN)) {};

			close (fd);
			closedir (dir);
			RHDDebug(Output->scrnIndex,"%s: Wrote value %i to %s\n",
				 __func__,*val,path);

			return TRUE;
		    } else {
			memset(buf,0,10);

			while ((read(fd,buf,9) == -1)
			       && (errno == EINTR || errno == EAGAIN)) {};

			if (sscanf(buf,"%i\n",val) == 1) {
			    *val = (*val * RHD_BACKLIGHT_PROPERTY_MAX) / max_val;

			    close(fd);
			    closedir(dir);
			    RHDDebug(Output->scrnIndex,"%s: Read value %i from %s\n",
				     __func__,*val,path);

			    return TRUE;
			}
		    }
		    close (fd);
		}
	    }
	}
    }
    closedir(dir);

    return FALSE;
}

/*
 * RhdACPIGetBacklightControl(): return backlight value in range 0..255;
 * -1 means no ACPI BL support.
 */
int
RhdACPIGetBacklightControl(struct rhdOutput *Output)
{
#ifdef __linux__
    int ret;
    RHDFUNC(Output);
    if (rhdDoBacklight(Output, FALSE, &ret))
	return ret;
#endif
    return -1;
}

/*
 *
 */
void
RhdACPISetBacklightControl(struct rhdOutput *Output, int val)
{
    RHDFUNC(Output);
#ifdef __linux__
    rhdDoBacklight(Output, TRUE, &val);
#endif
}
