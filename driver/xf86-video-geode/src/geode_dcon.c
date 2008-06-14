/* Copyright (c) 2006 Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 * Neither the name of the Advanced Micro Devices, Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
 * */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/* Includes that are used by all drivers */
#include <xf86.h>
#include <xf86_OSproc.h>
#include <xf86Resources.h>
#include <compiler.h>

#include "geode.h"
#include <unistd.h>
#include <fcntl.h>

#define DCON_SLEEP_FILE "/sys/devices/platform/dcon/sleep"

static Bool
dcon_present(void)
{
    static int _dval = -1;

    if (_dval == -1)
	_dval = (access("/sys/devices/platform/dcon", F_OK) == 0);

    return (Bool) _dval;
}

int
DCONDPMSSet(ScrnInfoPtr pScrni, int mode, int flags)
{
    static int failed = -1;
    int fd;
    char value[1];

    if (failed == -1)
	failed = !dcon_present();

    if (failed)
	return 0;

    fd = open(DCON_SLEEP_FILE, O_WRONLY);

    if (fd < 0) {
	failed = 1;
	return 0;
    }

    switch (mode) {
    case DPMSModeOn:
	value[0] = '0';
	break;
    case DPMSModeStandby:
    case DPMSModeSuspend:
    case DPMSModeOff:
	value[0] = '1';
	break;
    }

    write(fd, value, sizeof(value));
    close(fd);

    return 1;
}

Bool
dcon_init(ScrnInfoPtr pScrni)
{
    GeodeRec *pGeode = GEODEPTR(pScrni);

    if (!dcon_present()) {
	xf86DrvMsg(pScrni->scrnIndex, X_DEFAULT, "No DCON is present\n");
	return FALSE;
    }

    /* TODO: Print board revision once sysfs exports it. */
    xf86DrvMsg(pScrni->scrnIndex, X_DEFAULT, "DCON detected.\n");

    /* Panel size setup */
    pGeode->PanelX = DCON_DEFAULT_XRES;
    pGeode->PanelY = DCON_DEFAULT_YRES;

    /* FIXME:  Mode setup should go here */
    /* FIXME:  Controller setup should go here */
    return TRUE;
}
