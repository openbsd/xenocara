/*
 * Copyright 2007  Luc Verhaegen <lverhaegen@novell.com>
 * Copyright 2007  Matthias Hopf <mhopf@novell.com>
 * Copyright 2007  Egbert Eich   <eich@novell.com>
 * Copyright 2007  Advanced Micro Devices, Inc.
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

#ifndef _RHD_MODES_H
#define _RHD_MODES_H

/*
 * In case this isn't in xf86str.h yet.
 */
#ifndef M_T_PREFERRED
#define M_T_PREFERRED 0x08
#endif
#ifndef M_T_DRIVER
#define M_T_DRIVER 0x40
#endif

DisplayModePtr RHDCVTMode(int HDisplay, int VDisplay, float VRefresh,
			  Bool Reduced, Bool Interlaced);
void RHDPrintModeline(DisplayModePtr mode);
DisplayModePtr RHDModesAdd(DisplayModePtr Modes, DisplayModePtr Additions);

DisplayModePtr RHDModesPoolCreate(ScrnInfoPtr pScrn, Bool Silent);
void RHDModesAttach(ScrnInfoPtr pScrn, DisplayModePtr Modes);
DisplayModePtr RHDModeCopy(DisplayModePtr Mode);

Bool RHDGetVirtualFromConfig(ScrnInfoPtr pScrn);
void RHDGetVirtualFromModesAndFilter(ScrnInfoPtr pScrn, DisplayModePtr Modes, Bool Silent);

int RHDRRModeFixup(ScrnInfoPtr pScrn, DisplayModePtr Mode, struct rhdCrtc *Crtc,
		   struct rhdConnector *Connector, struct rhdOutput *Output,
		   struct rhdMonitor *Monitor);

#endif /* _RHD_MODES_H */
