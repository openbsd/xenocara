/*****************************************************************************
 * VIA Unichrome XvMC extension client lib.
 *
 * Copyright (c) 2004 The Unichrome Project. All rights reserved.
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
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHOR(S) OR COPYRIGHT HOLDER(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

/*
 * Authors: Thomas Hellström 2004 - 2005.
 */

#ifndef VIA_LOWLEVEL_H
#define VIA_LOWLEVEL_H

/*
 * The below define is cache size sensitive. Increasing the AGP buffer size
 * will enable the library to do deeper pipelining, but will degrade the
 * performance in the drm dma command verifier. 
 */

#define LL_AGP_CMDBUF_SIZE (4096*2)
#define LL_PCI_CMDBUF_SIZE (4096)

#define LL_MODE_DECODER_SLICE 0x01
#define LL_MODE_DECODER_IDLE 0x02
#define LL_MODE_VIDEO   0x04
#define LL_MODE_2D      0x08
#define LL_MODE_3D      0x10

/*
 * Errors
 */

#define LL_DECODER_TIMEDOUT 0x00000001
#define LL_IDCT_FIFO_ERROR  0x00000002
#define LL_SLICE_FIFO_ERROR 0x00000004
#define LL_SLICE_FAULT      0x00000008
#define LL_DMA_TIMEDOUT     0x00000010
#define LL_VIDEO_TIMEDOUT   0x00000020
#define LL_ACCEL_TIMEDOUT   0x00000040
#define LL_PCI_COMMAND_ERR  0x00000080
#define LL_AGP_COMMAND_ERR  0x00000100

#define VIA_SLICEBUSYMASK        0x00000200
#define VIA_BUSYMASK             0x00000207
#define VIA_SLICEIDLEVAL         0x00000200
#define VIA_IDLEVAL              0x00000204

#include "via_drm.h"
#include "viaXvMCPriv.h"

#define setRegion(xx,yy,ww,hh,region) \
    do {			  \
	(region).x = (xx);	  \
	(region).y = (yy);	  \
	(region).w = (ww);	  \
	(region).h = (hh);	  \
    } while(0)

#define regionEqual(r1, r2)				\
    ((r1).x == (r2).x &&				\
     (r1).y == (r2).y &&				\
     (r1).w == (r2).w &&				\
     (r1).h == (r2).h)

extern void
    *initXvMCLowLevel(int fd, drm_context_t * ctx,
    drmLockPtr hwLock, drmAddress mmioAddress,
    drmAddress fbAddress, unsigned fbStride, unsigned fbDepth,
    unsigned width, unsigned height, int useAgp, unsigned chipId);

extern void setLowLevelLocking(void *xlp, int perFormLocking);
extern void closeXvMCLowLevel(void *xlp);
extern void flushPCIXvMCLowLevel(void *xlp);
extern CARD32 viaDMATimeStampLowLevel(void *xlp);
extern void setAGPSyncLowLevel(void *xlp, int val, CARD32 timeStamp);

/*
 * These two functions also return and clear the current error status.
 */

extern unsigned flushXvMCLowLevel(void *xlp);
extern unsigned syncXvMCLowLevel(void *xlp, unsigned int mode,
    unsigned int doSleep, CARD32 timeStamp);

extern void hwlUnlock(void *xlp, int videoLock);
extern void hwlLock(void *xlp, int videoLock);

extern void viaVideoSetSWFLipLocked(void *xlp, unsigned yOffs, unsigned uOffs,
    unsigned vOffs, unsigned yStride, unsigned uvStride);

extern void viaMpegReset(void *xlp);
extern void viaMpegWriteSlice(void *xlp, CARD8 * slice,
    int nBytes, CARD32 sCode);
extern void viaMpegSetSurfaceStride(void *xlp, ViaXvMCContext * ctx);
extern void viaMpegSetFB(void *xlp, unsigned i, unsigned yOffs,
    unsigned uOffs, unsigned vOffs);
extern void viaMpegBeginPicture(void *xlp, ViaXvMCContext * ctx,
    unsigned width, unsigned height, const XvMCMpegControl * control);

/*
 * Low-level Video functions in viaLowLevel.c
 */

extern void viaBlit(void *xlp, unsigned bpp, unsigned srcBase,
    unsigned srcPitch, unsigned dstBase, unsigned dstPitch,
    unsigned w, unsigned h, int xdir, int ydir,
    unsigned blitMode, unsigned color);

extern void viaVideoSWFlipLocked(void *xlp, unsigned flags,
    int progressiveSequence);

extern void viaVideoSubPictureLocked(void *xlp,
    ViaXvMCSubPicture * pViaSubPic);
extern void viaVideoSubPictureOffLocked(void *xlp);

#define PCI_CHIP_VT3204         0x3108 /* K8M800 */
#define PCI_CHIP_VT3259         0x3118 /* PM800/PM880/CN400 */
#define PCI_CHIP_CLE3122        0x3122 /* CLE266 */
#define PCI_CHIP_VT3205         0x7205 /* KM400 */
#define PCI_CHIP_VT3327         0x3343 /* P4M890 */
#define PCI_CHIP_VT3364         0x3371 /* P4M900 */

#endif
