/*
 * Copyright 2008  Luc Verhaegen <lverhaegen@novell.com>
 * Copyright 2008  Matthias Hopf <mhopf@novell.com>
 * Copyright 2008  Egbert Eich   <eich@novell.com>
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

/*
 * Command Submission backend.
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86.h"

#include "rhd.h"
#include "rhd_cs.h"
#include "r5xx_regs.h"

#if 1
#define BUILD_CS_MMIO 1
#endif

#define CS_LOOP_COUNT 10000000

#ifdef BUILD_CS_MMIO
/*
 *
 * MMIO infrastructure.
 *
 */

/* First... provide some macro's for accessing the registers, which will
   somewhat speed up things */
#define CSMMIORegRead(Reg) (*(volatile CARD32 *) (MMIOBase + (Reg)))
#define CSMMIORegWrite(Reg, Value) (*(volatile CARD32 *) (MMIOBase + (Reg))) = (Value)

static void
CSMMIORBBMStuff(struct RhdCS *CS)
{
    CARD8 *MMIOBase = RHDPTRI(CS)->MMIOBase;
    CARD32 BufferEntries = ((CS->Wptr + CS->Size - CS->Flushed) & CS->Mask) / 2;
    CARD32 RBBMEntries = CSMMIORegRead(R5XX_RBBM_STATUS) & R5XX_RBBM_FIFOCNT_MASK;
    int i, Entries;

    if (RBBMEntries < BufferEntries)
	Entries = RBBMEntries;
    else
	Entries = BufferEntries;

    for (i = 0; i < Entries; i++) {
	CSMMIORegWrite((CS->Buffer[CS->Flushed] & 0x3FFF) << 2,
		       CS->Buffer[(CS->Flushed + 1) & CS->Mask]);
	CS->Flushed = (CS->Flushed + 2) & CS->Mask;
#ifdef RHD_CS_DEBUG
	CS->Grabbed -= 2;
#endif
    }
}

/*
 *
 */
static void
CSMMIOFlush(struct RhdCS *CS)
{
    int i;

#ifdef RHD_CS_DEBUG
    if (CS->Flushed & 1)
	xf86DrvMsg(CS->scrnIndex, X_ERROR,
		   "%s: CS->Flushed is not aligned (%d - (%s))!\n",
		   __func__, (unsigned int) CS->Flushed, CS->Func);
    if (CS->Wptr & 1)
	xf86DrvMsg(CS->scrnIndex, X_ERROR,
		   "%s: CS->Wptr is not aligned (%d - (%s))!\n",
		   __func__, (unsigned int) CS->Wptr, CS->Func);
#endif

    /* go from CS->Flushed to CP->Wptr and write it out */
    for (i = 0; i < CS_LOOP_COUNT; i++) {
	if (CS->Flushed != CS->Wptr)
	    CSMMIORBBMStuff(CS);
	else
	    return;
    }

    xf86DrvMsg(CS->scrnIndex, X_ERROR,
	       "%s: Failed to empty the RBBM.\n", __func__);
}

/*
 *
 */
static void
CSMMIOGrab(struct RhdCS *CS, CARD32 Count)
{
    int i;

#ifdef RHD_CS_DEBUG
    if (CS->Flushed & 1)
	xf86DrvMsg(CS->scrnIndex, X_ERROR,
		   "%s: CS->Flushed is not aligned (%d - (%s))!\n",
		   __func__, (unsigned int) CS->Flushed, CS->Func);
    if (CS->Wptr & 1)
	xf86DrvMsg(CS->scrnIndex, X_ERROR,
		   "%s: CS->Wptr is not aligned (%d - (%s))!\n",
		   __func__, (unsigned int) CS->Wptr, CS->Func);
#endif

    for (i = 0; i < CS_LOOP_COUNT; i++) {
	if (CS->Wptr == CS->Flushed) /* 16kB should be big enough */
	    return;
	if (((CS->Wptr + CS->Size - CS->Flushed) & CS->Mask) >= Count)
	    return;
	CSMMIORBBMStuff(CS);
    }

    xf86DrvMsg(CS->scrnIndex, X_ERROR,
	       "%s: Failed to get %d slots in the RBBM.\n",
	       __func__, (unsigned int) Count);
}

/*
 *
 */
static void
CSMMIODestroy(struct RhdCS *CS)
{
    if (CS->Buffer)
	xfree(CS->Buffer);

    CS->Destroy = NULL;
}

/*
 *
 */
static void
CSMMIOInit(struct RhdCS *CS)
{
    xf86DrvMsg(CS->scrnIndex, X_INFO,
	       "Using MMIO Command Submission for acceleration.\n");

    CS->Type = RHD_CS_MMIO;

    /* allocate a 64kB buffer here as well */
    CS->Size = (64 << 10) / 4;
    CS->Buffer = xnfcalloc(1, 4 * CS->Size);
    CS->Mask = CS->Size - 1; /* easy wrap around */

    CS->Grab = CSMMIOGrab;
    CS->Flush = CSMMIOFlush;
    CS->AdvanceFlush = TRUE;
    CS->Idle = NULL;  /* just the flush is enough, the rest is in the accel code */
    CS->Start = NULL; /* already in 2d accel code */
    CS->Reset = NULL; /* already in 2d accel code */
    CS->Stop = CSMMIOFlush;
    CS->Destroy = CSMMIODestroy;
}

#endif /* BUILD_CS_MMIO */

#ifdef USE_DRI

/*
 *
 * DRM CP Backend.
 *
 */

#include "xf86drm.h"
#include "radeon_drm.h"

#define R5XX_IDLE_RETRY 16 /* Fall out of idle loops after this count */

struct RhdDRMCP {
    int DrmFd;
    drmBufPtr DrmBuffer;
};

/*
 * Flush the indirect buffer to the kernel for submission to the card
 */
static void
DRMCPFlush(struct RhdCS *CS)
{
    struct RhdDRMCP *CP = CS->Private;
    struct drm_radeon_indirect indirect;

    if (!CP->DrmBuffer)
	return;

    indirect.idx = CP->DrmBuffer->idx;
    indirect.start = CS->Flushed * 4;
    indirect.end = CS->Wptr * 4;
    indirect.discard = 0;

    drmCommandWriteRead(CP->DrmFd, DRM_RADEON_INDIRECT,
			&indirect, sizeof(struct drm_radeon_indirect));

    /* make sure we are quadword aligned */
    if (CS->Wptr & 1)
	CS->Wptr++;

    CS->Flushed = CS->Wptr;
#ifdef RHD_CS_DEBUG
    CS->Grabbed = 0;
#endif
}

/*
 *
 */
static void
DRMCPBufferDiscard(struct RhdCS *CS)
{
    struct RhdDRMCP *CP = CS->Private;
    struct drm_radeon_indirect indirect;

    indirect.idx = CP->DrmBuffer->idx;
    indirect.start = CS->Flushed * 4;
    indirect.end = CS->Wptr * 4;
    indirect.discard = 1;

    drmCommandWriteRead(CP->DrmFd, DRM_RADEON_INDIRECT,
			&indirect, sizeof(struct drm_radeon_indirect));
}

/*
 *
 */
static void
DRMCPStart(struct RhdCS *CS)
{
    struct RhdDRMCP *CP = CS->Private;
    int ret;

    ret = drmCommandNone(CP->DrmFd, DRM_RADEON_CP_START);
    if (ret)
	xf86DrvMsg(CS->scrnIndex, X_ERROR,
		   "%s: DRM CP START returned %d\n", __func__, ret);

    /* Move the responsibility of checking for a buffer away from the Grab,
       to provide symmetry with the Stop. */
    if (CP->DrmBuffer) {
	xf86DrvMsg(CS->scrnIndex, X_ERROR,
		   "%s: stale buffer present!\n", __func__);
	DRMCPBufferDiscard(CS);
    }

    CP->DrmBuffer = RHDDRMCPBuffer(CS->scrnIndex);
    CS->Buffer = CP->DrmBuffer->address;
}

/*
 *
 */
static void
DRMCPStop(struct RhdCS *CS)
{
    struct RhdDRMCP *CP = CS->Private;
    struct drm_radeon_cp_stop stop;
    int ret, i;

    /* flush and discard the indirect buffer */
    if (CP->DrmBuffer)
	DRMCPBufferDiscard(CS);
    CP->DrmBuffer = NULL;
    CS->Buffer = NULL;

    /* now stop the CP itself */
    stop.flush = 0; /* pointless in drm */
    stop.idle = 1;
    for (i = 0; i < R5XX_IDLE_RETRY; i++) {
	ret = drmCommandWrite(CP->DrmFd, DRM_RADEON_CP_STOP,
			      &stop, sizeof(struct drm_radeon_cp_stop));
	if (!ret)
	    return;
	else if (ret != -16) {
	    xf86DrvMsg(CS->scrnIndex, X_ERROR,
		       "%s Stop/Idle failed: %d\n", __func__, ret);
	    return;
	}
    }

    stop.idle = 0;
    if (drmCommandWrite(CP->DrmFd, DRM_RADEON_CP_STOP,
			&stop, sizeof(struct drm_radeon_cp_stop)))
	xf86DrvMsg(CS->scrnIndex, X_ERROR,
		   "%s Stop failed: %d\n", __func__, ret);
}

/*
 *
 */
static void
DRMCPReset(struct RhdCS *CS)
{
    struct RhdDRMCP *CP = CS->Private;
    int ret;

    ret = drmCommandNone(CP->DrmFd, DRM_RADEON_CP_RESET);
    if (ret)
	xf86DrvMsg(CS->scrnIndex, X_ERROR, "%s: Reset failed %d\n", __func__, ret);

    ret = drmCommandNone(CP->DrmFd, DRM_RADEON_CP_START);
    if (ret)
	xf86DrvMsg(CS->scrnIndex, X_ERROR, "%s: Start failed %d\n", __func__, ret);
}

/*
 *
 */
static Bool
DRMCPIdle(struct RhdCS *CS)
{
    struct RhdDRMCP *CP = CS->Private;
    int i, ret;

    /* The DRM CP IDLE call does quite a lot more than just wait for the CP
     * going idle. It waits on the RBBM as well. This number needs to be huge,
     * as the DRM cluelessly uses loops instead of usecs, and this is therefor
     * decreasing rapidly with CPU advancement. */
    for (i = 0; i < 2000000; i++) {
	ret = drmCommandNone(CP->DrmFd, DRM_RADEON_CP_IDLE);
	if (!ret)
	    return TRUE;
	else if (ret != -16) {
	    xf86DrvMsg(CS->scrnIndex, X_ERROR, "%s: DRM CP IDLE returned %d\n", __func__, ret);
	    return FALSE;
	} else
	    xf86DrvMsg(CS->scrnIndex, X_WARNING, "%s: DRM CP IDLE returned BUSY!\n", __func__);
    }

    xf86DrvMsg(CS->scrnIndex, X_ERROR, "%s: Failed!\n", __func__);
    return FALSE;
}

/*
 *
 */
static void
DRMCPGrab(struct RhdCS *CS, CARD32 Count)
{
    struct RhdDRMCP *CP = CS->Private;

    if (!CP->DrmBuffer || ((CS->Size - CS->Wptr) < Count)) {
	if (CP->DrmBuffer)
	    DRMCPBufferDiscard(CS);

	CP->DrmBuffer =  RHDDRMCPBuffer(CS->scrnIndex);
	CS->Buffer = CP->DrmBuffer->address;
	CS->Flushed = 0;
	CS->Wptr = 0;
#ifdef RHD_CS_DEBUG
	CS->Grabbed = 0;
#endif
    }
}

/*
 *
 */
static void
DRMCPDestroy(struct RhdCS *CS)
{
    struct RhdDRMCP *CP = CS->Private;

    if (!CP) {
	xf86DrvMsg(CS->scrnIndex, X_ERROR,
		   "%s: Out of order: already destroyed.\n", __func__);
	return;
    }

    if (CP->DrmBuffer)
	xf86DrvMsg(CS->scrnIndex, X_ERROR,
		   "%s: Shouldn't you call Stop first?\n", __func__);

    xfree(CP);
    CS->Private = NULL;
    CS->Destroy = NULL;
}

/*
 *
 */
static Bool
CSDRMCPInit(struct RhdCS *CS)
{
    struct RhdDRMCP *CP;
    int DrmFd = RHDDRMFDGet(CS->scrnIndex);

    if (DrmFd < 0)
	return FALSE;

    xf86DrvMsg(CS->scrnIndex, X_INFO,
	       "Using DRM Command Processor (indirect) for acceleration.\n");

    CP = xnfcalloc(1, sizeof(struct RhdDRMCP));
    CP->DrmFd = DrmFd;

    CS->Private = CP;

    CS->Type = RHD_CS_CPDMA;

    CS->Size = (64 << 10) / 4;
    CS->Mask = 0xFFFFFFFF;

    CS->Grab = DRMCPGrab;
    CS->Flush = DRMCPFlush;
    /* The DRILeaveServer call flushes all the time for us */
    CS->AdvanceFlush = FALSE;
    CS->Idle = DRMCPIdle;
    CS->Start = DRMCPStart;
    CS->Reset = DRMCPReset;
    CS->Stop = DRMCPStop;
    CS->Destroy = DRMCPDestroy;

    return TRUE;
}

#endif /* USE_DRI */

/*
 *
 * Actual highlevel Command Submission.
 *
 */

/*
 * The logic is kind of tricky, we can use both ringbuffers as normal buffers.
 */

#ifdef RHD_CS_DEBUG
/*
 * Since we're CS_DEBUG, we don't really need to care about performance much.
 */
void
RHDCSGrabDebug(struct RhdCS *CS, CARD32 Count, const char *func)
{
   if (!CS->Active)
	xf86DrvMsg(CS->scrnIndex, X_ERROR,
                   "%s: Grabbing while CS is not started!\n", func);

   if (CS->Wptr != ((CS->Flushed + CS->Grabbed) & CS->Mask))
	xf86DrvMsg(CS->scrnIndex, X_ERROR,
		   "%s: Wptr != Flushed + Grabbed (%d vs %d + %d) (%s -> %s)\n",
		   func, (unsigned int) CS->Wptr, (unsigned int) CS->Flushed,
		   (unsigned int) CS->Grabbed, CS->Func, func);

    _RHDCSGrab(CS, Count);

    CS->Grabbed += Count;
    CS->Func = func;
}
#endif /* RHD_CS_DEBUG */

/*
 *
 */
void
RHDCSFlush(struct RhdCS *CS)
{
#ifdef RHD_CS_DEBUG
    if (!CS->Active) {
	xf86DrvMsg(CS->scrnIndex, X_ERROR, "%s: CS is not active (%s)!\n",
		   __func__, CS->Func);
	return;
    }

    if (CS->Wptr != ((CS->Flushed + CS->Grabbed) & CS->Mask))
	xf86DrvMsg(CS->scrnIndex, X_ERROR,
		   "%s: Wptr != Flushed + Grabbed (%d vs %d + %d) (From %s)\n",
		   __func__, (unsigned int) CS->Wptr, (unsigned int) CS->Flushed,
		   (unsigned int) CS->Grabbed, CS->Func);
#endif

    if (CS->Flushed != CS->Wptr)
	CS->Flush(CS);
}

/*
 *
 */
Bool
RHDCSIdle(struct RhdCS *CS)
{
#ifdef RHD_CS_DEBUG
    if (!CS->Active) {
	xf86DrvMsg(CS->scrnIndex, X_ERROR, "%s: CS is not active!\n",
		   __func__);
	return TRUE;
    }
#endif

    if (CS->Idle)
	return CS->Idle(CS);

    return TRUE;
}

/*
 *
 */
void
RHDCSStart(struct RhdCS *CS)
{
    RHDFUNC(CS);

    if (CS->Active) {
	xf86DrvMsg(CS->scrnIndex, X_ERROR, "%s: Command Submission backend is "
		   "already active!\n", __func__);
	return;
    }

    if (CS->Start)
	CS->Start(CS);

    CS->Flushed = 0;
    CS->Wptr = 0;
#ifdef RHD_CS_DEBUG
    CS->Grabbed = 0;
#endif

    CS->Clean = RHD_CS_CLEAN_DONE;
    CS->Active = TRUE;
}

/*
 *
 */
void
RHDCSReset(struct RhdCS *CS)
{
    RHDFUNC(CS);

#ifdef RHD_CS_DEBUG
    if (!CS->Active)
	xf86DrvMsg(CS->scrnIndex, X_ERROR, "%s: CS is not active!\n",
		   __func__);
#endif

    if (CS->Reset)
	CS->Reset(CS);
}

/*
 *
 */
void
RHDCSStop(struct RhdCS *CS)
{
    RHDFUNC(CS);

    if (!CS->Active) {
	xf86DrvMsg(CS->scrnIndex, X_ERROR, "%s: Command Submission backend is "
		   "not active!\n", __func__);
	return;
    }

#ifdef RHD_CS_DEBUG
    if (CS->Wptr != (CS->Flushed + CS->Grabbed))
	xf86DrvMsg(CS->scrnIndex, X_ERROR,
		   "%s: Wptr != Flushed + Grabbed (%d vs %d + %d) (From %s)\n",
		   __func__, (unsigned int) CS->Wptr, (unsigned int) CS->Flushed,
		   (unsigned int) CS->Grabbed, CS->Func);
#endif

    CS->Stop(CS);

    CS->Flushed = 0;
    CS->Wptr = 0;
#ifdef RHD_CS_DEBUG
    CS->Grabbed = 0;
#endif

    CS->Active = FALSE;
}

/*
 *
 */
void
RHDCSInit(ScrnInfoPtr pScrn)
{
    RHDPtr rhdPtr = RHDPTR(pScrn);
    struct RhdCS *CS = xnfcalloc(1, sizeof(struct RhdCS));

    CS->scrnIndex = pScrn->scrnIndex;

    rhdPtr->CS = CS;

#ifdef USE_DRI
    if (CSDRMCPInit(CS))
	return;
#endif

    if (rhdPtr->ChipSet >= RHD_R600) {
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		   "%s: No CS for R600 and up yet.\n", __func__);
	xfree(CS);
	rhdPtr->CS = NULL;
	return;
    }

    /* hook in direct CP backend here */

    CSMMIOInit(CS);
}

/*
 *
 */
void
RHDCSDestroy(ScrnInfoPtr pScrn)
{
    struct RhdCS *CS = RHDPTR(pScrn)->CS;

    if (CS->Destroy)
	CS->Destroy(CS);

    xfree(CS);
    RHDPTR(pScrn)->CS = NULL;
}
