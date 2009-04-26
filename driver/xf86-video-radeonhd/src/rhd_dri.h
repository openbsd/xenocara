/*
 * Copyright 2007-2008  Luc Verhaegen <libv@exsuse.de>
 * Copyright 2007-2008  Matthias Hopf <mhopf@novell.com>
 * Copyright 2007-2008  Egbert Eich   <eich@novell.com>
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

#ifndef _RHD_DRI_
#define _RHD_DRI_

extern Bool RHDDRIPreInit(ScrnInfoPtr pScrn);
extern Bool RHDDRIAllocateBuffers(ScrnInfoPtr pScrn);
extern Bool RHDDRIScreenInit(ScreenPtr pScreen);

extern Bool RHDDRICloseScreen(ScreenPtr pScreen);
extern Bool RHDDRIFinishScreenInit(ScreenPtr pScreen);
extern void RHDDRIEnterVT(ScreenPtr pScreen);
extern void RHDDRILeaveVT(ScreenPtr pScreen);
extern Bool RHDDRIScreenInit(ScreenPtr pScreen);

/* Claim the 3D context */
void RHDDRIContextClaim(ScrnInfoPtr pScrn);

#endif
