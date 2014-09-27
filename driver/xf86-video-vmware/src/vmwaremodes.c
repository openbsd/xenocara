/*
 * Copyright 2007 by VMware, Inc.
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
 *
 * Except as contained in this notice, the name of the copyright holder(s)
 * and author(s) shall not be used in advertising or otherwise to promote
 * the sale, use or other dealings in this Software without prior written
 * authorization from the copyright holder(s) and author(s).
 */

/*
 * vmwaremodes.c --
 *      
 *      Provide additional modes for the driver.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "xf86.h"
#ifdef HAVE_XORG_SERVER_1_2_0
#include <xf86Modes.h>
#endif
#include "vm_basic_types.h"
#include "vmware.h"

#ifndef M_T_DRIVER
# define M_T_DRIVER  0x40	/* Supplied by the driver (EDID, etc) */
#endif

#define MODEPREFIX NULL, NULL, NULL, 0, M_T_DRIVER
#define MODESUFFIX 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,FALSE,FALSE,0,NULL,0,0.0,0.0

#define VMW_DEFLT_MODE_NAME "vmwlegacy-default-%dx%d"

/*
 *-----------------------------------------------------------------------------
 *
 * vmwareAddDefaultMode --
 *
 *    Add a default mode with the current screen dimensions.
 *
 * Results:
 *    The default mode.
 *
 * Side effects:
 *    None.
 *
 *-----------------------------------------------------------------------------
 */

void
vmwareAddDefaultMode(ScrnInfoPtr pScrn, uint32 dwidth, uint32 dheight)
{
    DisplayModePtr *monitorModes = &pScrn->monitor->Modes;
    DisplayModePtr modes = NULL;

    if (monitorModes == NULL || *monitorModes == NULL) {
	goto out_err;
    }

#ifdef HAVE_XORG_SERVER_1_2_0
    if (dwidth && dheight) {
	MonPtr monitor = pScrn->monitor;
	DisplayModePtr mode = NULL;
	DisplayModeRec dynamic =
	    { MODEPREFIX, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, MODESUFFIX };
	unsigned dispModeCount = 0;
	char **dispModeList;
	char *dynModeName;
	char name[80];
	VMWAREPtr pVMWARE = VMWAREPTR(pScrn);

	/* First, add the default mode name to the display mode
	 * requests.
	 */

	snprintf(name, sizeof(name), VMW_DEFLT_MODE_NAME, dwidth, dheight);

	dynModeName = xnfstrdup(name);
	if (!dynModeName || !pScrn->display)
	    goto out_err;

	if (pScrn->display->modes) {
	    dispModeList = pScrn->display->modes;
	    while(*dispModeList)
		dispModeList++;
	    dispModeCount = (unsigned)(((size_t)dispModeList -
	                                (size_t)pScrn->display->modes) /
	                               sizeof(*dispModeList));
	}

	dispModeList = xnfcalloc(dispModeCount + 2, sizeof(*dispModeList));
	if (!dispModeList)
	    goto out_err;

	memcpy(dispModeList, pScrn->display->modes,
	       dispModeCount * sizeof(*dispModeList));
	dispModeList[dispModeCount] = dynModeName;
	pScrn->display->modes = dispModeList;

	/* Then, add the default mode itself.
	 */

	dynamic.name = name;
	dynamic.HDisplay = dwidth;
	dynamic.HSyncStart = dynamic.HDisplay + 1;
	dynamic.HSyncEnd = dynamic.HSyncStart + 1;
	dynamic.HTotal = dynamic.HSyncEnd * 5 / 4;
	dynamic.VDisplay = dheight;
	dynamic.VSyncStart = dynamic.VDisplay + 1;
	dynamic.VSyncEnd = dynamic.VSyncStart + 1;
	dynamic.VTotal = dynamic.VSyncEnd + 1;
	if (monitor->nVrefresh > 0)
	    dynamic.VRefresh = monitor->vrefresh[0].lo;
	else
	    dynamic.VRefresh = 60;
	dynamic.Clock = dynamic.VRefresh * dynamic.VTotal *
	    dynamic.HTotal / 1000;
	mode = xf86DuplicateMode(&dynamic);
	modes = xf86ModesAdd(modes, mode);

	if (dispModeCount == 0) {

	    /*
	     * Set up a large virtual size, so that we allow also
	     * setting modes larger than the initial mode.
	     *
	     * We might also want to consider the case where
	     * dispModeCount != 0, but the requested display modes
	     * are not available. This is sufficient for now.
	     */

	    if (pScrn->display->virtualX == 0)
		pScrn->display->virtualX = pVMWARE->maxWidth;
	    if (pScrn->display->virtualY == 0)
		pScrn->display->virtualY = pVMWARE->maxHeight;
	}
    }

    *monitorModes = xf86ModesAdd(*monitorModes, modes);
#else
    (void) modes;
#endif
    return;
  out_err:
    xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Failed to add default mode.");
}
