/*
 * Copyright 2008  Luc Verhaegen <libv@exsuse.de>
 * Copyright 2008  Matthias Hopf <mhopf@novell.com>
 * Copyright 2008  Egbert Eich   <eich@novell.com>
 * Copyright 2008  Advanced Micro Devices, Inc.
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
 * The below code is a rework from code in xf86-video-ati/src/radeon_accel.c
 * The original license is included below, it has the messed up disclaimer and
 * an all rights reserved statement.
 */

/*
 * Copyright 2000 ATI Technologies Inc., Markham, Ontario, and
 *                VA Linux Systems Inc., Fremont, California.
 *
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation on the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NON-INFRINGEMENT.  IN NO EVENT SHALL ATI, VA LINUX SYSTEMS AND/OR
 * THEIR SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Authors:
 *   Kevin E. Martin <martin@xfree86.org>
 *   Rickard E. Faith <faith@valinux.com>
 *   Alan Hourihane <alanh@fairlite.demon.co.uk>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86.h"

#include "rhd.h"
#include "rhd_cs.h"
#include "r5xx_accel.h"
#include "r5xx_regs.h"
#include "r5xx_3dregs.h"

/*
 * Used by both XAA and EXA code.
 */
struct R5xxRop R5xxRops[] = {
    { R5XX_ROP3_ZERO, R5XX_ROP3_ZERO }, /* GXclear        */
    { R5XX_ROP3_DSa,  R5XX_ROP3_DPa  }, /* Gxand          */
    { R5XX_ROP3_SDna, R5XX_ROP3_PDna }, /* GXandReverse   */
    { R5XX_ROP3_S,    R5XX_ROP3_P    }, /* GXcopy         */
    { R5XX_ROP3_DSna, R5XX_ROP3_DPna }, /* GXandInverted  */
    { R5XX_ROP3_D,    R5XX_ROP3_D    }, /* GXnoop         */
    { R5XX_ROP3_DSx,  R5XX_ROP3_DPx  }, /* GXxor          */
    { R5XX_ROP3_DSo,  R5XX_ROP3_DPo  }, /* GXor           */
    { R5XX_ROP3_DSon, R5XX_ROP3_DPon }, /* GXnor          */
    { R5XX_ROP3_DSxn, R5XX_ROP3_PDxn }, /* GXequiv        */
    { R5XX_ROP3_Dn,   R5XX_ROP3_Dn   }, /* GXinvert       */
    { R5XX_ROP3_SDno, R5XX_ROP3_PDno }, /* GXorReverse    */
    { R5XX_ROP3_Sn,   R5XX_ROP3_Pn   }, /* GXcopyInverted */
    { R5XX_ROP3_DSno, R5XX_ROP3_DPno }, /* GXorInverted   */
    { R5XX_ROP3_DSan, R5XX_ROP3_DPan }, /* GXnand         */
    { R5XX_ROP3_ONE,  R5XX_ROP3_ONE  }  /* GXset          */
};

/*
 * The FIFO has 64 slots.  This routine waits until at least `required'
 * of these slots are empty.
 */
static Bool
R5xxFIFOWait(RHDPtr rhdPtr, CARD32 required)
{
    int i;

    for (i = 0; i < R5XX_LOOP_COUNT; i++)
	if (required <=
	    (RHDRegRead(rhdPtr, R5XX_RBBM_STATUS) & R5XX_RBBM_FIFOCNT_MASK))
	    return TRUE;

    xf86DrvMsg(rhdPtr->scrnIndex, X_ERROR, "%s: Timeout 0x%08X.\n", __func__,
	       (unsigned int) RHDRegRead(rhdPtr, R5XX_RBBM_STATUS));
    return FALSE;
}

/*
 * Flush all dirty data in the Pixel Cache to memory.
 */
static Bool
R5xx2DFlush(int scrnIndex)
{
    ScrnInfoPtr pScrn = xf86Screens[scrnIndex];
    int i;

    RHDRegMask(pScrn, R5XX_DSTCACHE_CTLSTAT,
		/* Radeon code:
		   R5XX_RB2D_DC_FLUSH_ALL, R5XX_RB2D_DC_FLUSH_ALL */
		R5XX_DSTCACHE_FLUSH_ALL, R5XX_DSTCACHE_FLUSH_ALL);

    for (i = 0; i < R5XX_LOOP_COUNT; i++)
	/* Radeon code:
	   & R5XX_RB2D_DC_BUSY */
	if (!(RHDRegRead(pScrn, R5XX_DSTCACHE_CTLSTAT) & R5XX_DSTCACHE_BUSY))
	    return TRUE;

    xf86DrvMsg(scrnIndex, X_ERROR, "%s: Timeout 0x%08x.\n", __func__,
	       (unsigned int) RHDRegRead(pScrn, R5XX_DSTCACHE_CTLSTAT));
    return FALSE;
}

/*
 * Wait for the graphics engine to be completely idle: the FIFO has
 * drained, the Pixel Cache is flushed, and the engine is idle.  This is
 * a standard "sync" function that will make the hardware "quiescent".
 */
static Bool
R5xx2DIdleLocal(int scrnIndex)
{
    ScrnInfoPtr pScrn = xf86Screens[scrnIndex];
    int i;

    /* wait for fifo to clear */
    for (i = 0; i < R5XX_LOOP_COUNT; i++)
	if (0x40 == (RHDRegRead(pScrn, R5XX_RBBM_STATUS) & R5XX_RBBM_FIFOCNT_MASK))
	    break;

    if (i == R5XX_LOOP_COUNT) {
	xf86DrvMsg(scrnIndex, X_ERROR, "%s: FIFO Timeout 0x%08X.\n", __func__,
		   (unsigned int) RHDRegRead(pScrn, R5XX_RBBM_STATUS));
	return FALSE;
    }

    /* wait for engine to go idle */
    for (i = 0; i < R5XX_LOOP_COUNT; i++)
	if (!(RHDRegRead(pScrn, R5XX_RBBM_STATUS) & R5XX_RBBM_ACTIVE))
	    break;

    if (i == R5XX_LOOP_COUNT) {
	xf86DrvMsg(scrnIndex, X_ERROR, "%s: Idle Timeout 0x%08X.\n", __func__,
		   (unsigned int) RHDRegRead(pScrn, R5XX_RBBM_STATUS));
	return FALSE;
    }

    R5xx2DFlush(scrnIndex);
    return TRUE;
}

/*
 * Reset graphics card to known state
 */
static void
R5xx2DReset(ScrnInfoPtr pScrn)
{
    RHDPtr rhdPtr = RHDPTR(pScrn);
    CARD32 save, tmp;

    RHDFUNC(rhdPtr);

    /* The following RBBM_SOFT_RESET sequence can help un-wedge
     * an R300 after the command processor got stuck. */
    save = RHDRegRead(rhdPtr, R5XX_RBBM_SOFT_RESET);
    tmp = save | R5XX_SOFT_RESET_CP |
	R5XX_SOFT_RESET_HI | R5XX_SOFT_RESET_SE |
	R5XX_SOFT_RESET_RE | R5XX_SOFT_RESET_PP |
	R5XX_SOFT_RESET_E2 | R5XX_SOFT_RESET_RB;
    RHDRegWrite(rhdPtr, R5XX_RBBM_SOFT_RESET, tmp);

    RHDRegRead(rhdPtr, R5XX_RBBM_SOFT_RESET);
    tmp &= ~(R5XX_SOFT_RESET_CP | R5XX_SOFT_RESET_HI |
	     R5XX_SOFT_RESET_SE | R5XX_SOFT_RESET_RE |
	     R5XX_SOFT_RESET_PP | R5XX_SOFT_RESET_E2 |
	     R5XX_SOFT_RESET_RB);
    RHDRegWrite(rhdPtr, R5XX_RBBM_SOFT_RESET, tmp);

    RHDRegRead(rhdPtr, R5XX_RBBM_SOFT_RESET);
    RHDRegWrite(rhdPtr, R5XX_RBBM_SOFT_RESET, save);
    RHDRegRead(rhdPtr, R5XX_RBBM_SOFT_RESET);

    R5xx2DFlush(rhdPtr->scrnIndex);

    /* Soft resetting HDP thru RBBM_SOFT_RESET register can cause some
     * unexpected behaviour on some machines.  Here we use
     * R5XX_HOST_PATH_CNTL to reset it. */
    save = RHDRegRead(rhdPtr, R5XX_HOST_PATH_CNTL);
    RHDRegMask(rhdPtr, R5XX_RBBM_SOFT_RESET,
	       R5XX_SOFT_RESET_CP | R5XX_SOFT_RESET_HI | R5XX_SOFT_RESET_E2,
	       R5XX_SOFT_RESET_CP | R5XX_SOFT_RESET_HI | R5XX_SOFT_RESET_E2);

    RHDRegRead(rhdPtr, R5XX_RBBM_SOFT_RESET);
    RHDRegWrite(rhdPtr, R5XX_RBBM_SOFT_RESET, 0);

    RHDRegMask(rhdPtr, R5XX_RB2D_DSTCACHE_MODE,
	       R5XX_RB2D_DC_AUTOFLUSH_ENABLE | R5XX_RB2D_DC_DISABLE_IGNORE_PE,
	       R5XX_RB2D_DC_AUTOFLUSH_ENABLE | R5XX_RB2D_DC_DISABLE_IGNORE_PE);

    RHDRegWrite(rhdPtr, R5XX_HOST_PATH_CNTL, save | R5XX_HDP_SOFT_RESET);
    RHDRegRead(rhdPtr, R5XX_HOST_PATH_CNTL);
    RHDRegWrite(rhdPtr, R5XX_HOST_PATH_CNTL, save);
}

/*
 *
 */
CARD8
R5xx2DDatatypeGet(ScrnInfoPtr pScrn)
{
    switch (pScrn->depth) {
    case 8:
	return R5XX_DATATYPE_CI8;
    case 15:
	return R5XX_DATATYPE_ARGB1555;
    case 16:
	return R5XX_DATATYPE_RGB565;
    case 24:
    case 32:
	return R5XX_DATATYPE_ARGB8888;
    default:
	/* should never happen, as we only support the above bpps */
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "%s: Unhandled pixel depth: %d.\n",
		   __func__, pScrn->depth);
	return R5XX_DATATYPE_ARGB8888;
    }
}

/*
 *
 */
void
R5xx2DSetup(ScrnInfoPtr pScrn)
{
    RHDPtr rhdPtr = RHDPTR(pScrn);
    CARD32 tmp;

    RHDFUNC(rhdPtr);

    /* Setup engine location. This shouldn't be necessary since we
     * set them appropriately before any accel ops, but let's avoid
     * random bogus DMA in case we inadvertently trigger the engine
     * in the wrong place (happened). */
    tmp = (((pScrn->displayWidth * (pScrn->bitsPerPixel / 8)) / 64) << 22) |
	((rhdPtr->FbIntAddress + rhdPtr->FbScanoutStart) >> 10);

    R5xxFIFOWait(rhdPtr, 2);
    RHDRegWrite(rhdPtr, R5XX_DST_PITCH_OFFSET, tmp);
    RHDRegWrite(rhdPtr, R5XX_SRC_PITCH_OFFSET, tmp);

    R5xxFIFOWait(rhdPtr, 2);
#if X_BYTE_ORDER == X_BIG_ENDIAN
    RHDRegMask(rhdPtr, R5XX_DP_DATATYPE,
	       R5XX_HOST_BIG_ENDIAN_EN, R5XX_HOST_BIG_ENDIAN_EN);

    switch (pScrn->bitsPerPixel) {
    case 8:
	RHDRegWrite(rhdPtr, R5XX_SURFACE_CNTL, 0);
	break;
    case 16: /* depths 15 and 16 */
	RHDRegWrite(rhdPtr, R5XX_SURFACE_CNTL,
		    R5XX_NONSURF_AP0_SWP_16BPP | R5XX_NONSURF_AP1_SWP_16BPP);
	break;
    case 32: /* depth 24 */
	RHDRegWrite(rhdPtr, R5XX_SURFACE_CNTL,
		    R5XX_NONSURF_AP0_SWP_32BPP | R5XX_NONSURF_AP1_SWP_32BPP);
	break;
    }
#else
    RHDRegMask(rhdPtr, R5XX_DP_DATATYPE, 0, R5XX_HOST_BIG_ENDIAN_EN);
    RHDRegWrite(rhdPtr, R5XX_SURFACE_CNTL, 0);
#endif

    R5xxFIFOWait(rhdPtr, 1);
    RHDRegWrite(rhdPtr, R5XX_DEFAULT_SC_BOTTOM_RIGHT,
		R5XX_DEFAULT_SC_RIGHT_MAX | R5XX_DEFAULT_SC_BOTTOM_MAX);

    R5xxFIFOWait(rhdPtr, 1);
    RHDRegWrite(rhdPtr, R5XX_DP_GUI_MASTER_CNTL,
		(R5xx2DDatatypeGet(pScrn) << R5XX_GMC_DST_DATATYPE_SHIFT) |
		R5XX_GMC_CLR_CMP_CNTL_DIS | R5XX_GMC_DST_PITCH_OFFSET_CNTL |
		R5XX_GMC_BRUSH_SOLID_COLOR | R5XX_GMC_SRC_DATATYPE_COLOR);

    R5xxFIFOWait(rhdPtr, 5);
    RHDRegWrite(rhdPtr, R5XX_DP_BRUSH_FRGD_CLR, 0xFFFFFFFF);
    RHDRegWrite(rhdPtr, R5XX_DP_BRUSH_BKGD_CLR, 0x00000000);
    RHDRegWrite(rhdPtr, R5XX_DP_SRC_FRGD_CLR, 0xFFFFFFFF);
    RHDRegWrite(rhdPtr, R5XX_DP_SRC_BKGD_CLR, 0x00000000);
    RHDRegWrite(rhdPtr, R5XX_DP_WRITE_MASK, 0xFFFFFFFF);
}

/*
 *
 */
static void
R5xxEngineReset(ScrnInfoPtr pScrn)
{
    RHDPtr rhdPtr = RHDPTR(pScrn);

    xf86DrvMsg(rhdPtr->scrnIndex, X_ERROR, "%s!!!!!\n", __func__);

    R5xx2DReset(pScrn);
    R5xx2DSetup(pScrn);
    RHDCSReset(rhdPtr->CS);

    /* we also need to reinitialise the 3d engine now */
    if (rhdPtr->ThreeDPrivate)
	((struct R5xx3D *) rhdPtr->ThreeDPrivate)->XHas3DEngineState = FALSE;
}

/*
 *
 */
void
R5xx2DIdle(ScrnInfoPtr pScrn)
{
    if (!R5xx2DIdleLocal(pScrn->scrnIndex))
	R5xxEngineReset(pScrn);
}

/*
 *
 */
void
R5xx2DStart(ScrnInfoPtr pScrn)
{
    RHDPtr rhdPtr = RHDPTR(pScrn);

    RHDFUNC(pScrn);

    if ((rhdPtr->ChipSet != RHD_RS690) &&
	(rhdPtr->ChipSet != RHD_RS600) &&
	(rhdPtr->ChipSet != RHD_RS740)) {
	CARD8 pipe = (RHDRegRead(rhdPtr, R400_GB_PIPE_SELECT) >> 4) & 0xF0;
	RHDWritePLL(pScrn, R500_DYN_SCLK_PWMEM_PIPE, pipe | 0x01);
    }

    RHDRegMask(pScrn, R5XX_GB_TILE_CONFIG, 0, R5XX_ENABLE_TILING);
    RHDRegWrite(pScrn, R5XX_WAIT_UNTIL,
		R5XX_WAIT_2D_IDLECLEAN | R5XX_WAIT_3D_IDLECLEAN);
    RHDRegMask(pScrn, R5XX_DST_PIPE_CONFIG,
	       R5XX_PIPE_AUTO_CONFIG, R5XX_PIPE_AUTO_CONFIG);
    RHDRegMask(pScrn, R5XX_RB2D_DSTCACHE_MODE,
	       R5XX_RB2D_DC_AUTOFLUSH_ENABLE | R5XX_RB2D_DC_DISABLE_IGNORE_PE,
	       R5XX_RB2D_DC_AUTOFLUSH_ENABLE | R5XX_RB2D_DC_DISABLE_IGNORE_PE);

    R5xx2DReset(pScrn);
    R5xx2DSetup(pScrn);
    R5xx2DIdleLocal(pScrn->scrnIndex);
}

/*
 * Impose some acceleration limitations.
 */
Bool
R5xx2DFBValid(RHDPtr rhdPtr, CARD16 Width, CARD16 Height, int bpp,
	      CARD32 Offset, CARD32 Size, CARD32 Pitch)
{
    int Bytes;

    /* we do not care about offset atm. */

    switch (bpp) {
    case 8:
	Bytes = 1;
	break;
    case 15:
    case 16:
	Bytes = 2;
	break;
    case 24:
    case 32:
	Bytes = 4;
	break;
    default:
	xf86DrvMsg(rhdPtr->scrnIndex, X_ERROR, "%s: unhandled bpp (%d)\n",
		   __func__, bpp);
	Bytes = 4;
	break;
    }

    if ((Pitch * Bytes) & 0x3F) /* bits are ignored */
	return FALSE;

    if ((Pitch * Bytes) >= 0x4000)
	return FALSE;

    /* TODO check */
    if (Height >= 0x2000)
	return FALSE;

    return TRUE;
}

/*
 * Handlers for rhdPtr->ThreeDInfo.
 */
void
R5xx3DInit(ScrnInfoPtr pScrn)
{
    RHDPtr rhdPtr = RHDPTR(pScrn);
    struct R5xx3D *R5xx3D;

    if (rhdPtr->ThreeDPrivate) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "%s: rhdPtr->ThreeDPrivate is already initialised.\n",
		   __func__);
	return;
    }

    R5xx3D = (struct R5xx3D *) xnfcalloc(1, sizeof(struct R5xx3D));
    R5xx3D->XHas3DEngineState = FALSE;
    rhdPtr->ThreeDPrivate = R5xx3D;
}

/*
 *
 */
void
R5xx3DDestroy(ScrnInfoPtr pScrn)
{
    RHDPtr rhdPtr = RHDPTR(pScrn);

    if (!rhdPtr->ThreeDPrivate) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "%s: rhdPtr->ThreeDPrivate is not assigned.\n", __func__);
	return;
    }

    xfree(rhdPtr->ThreeDPrivate);
    rhdPtr->ThreeDPrivate = NULL;
}

/*
 *
 */
void
R5xxDstCacheFlush(struct RhdCS *CS)
{
    RHDCSGrab(CS, 2);
    RHDCSRegWrite(CS, R5XX_RB3D_DSTCACHE_CTLSTAT, R5XX_RB3D_DC_FLUSH_ALL);
}

/*
 *
 */
void
R5xxZCacheFlush(struct RhdCS *CS)
{
    RHDCSGrab(CS, 2);
    RHDCSRegWrite(CS, R5XX_RB3D_ZCACHE_CTLSTAT, R5XX_RB3D_ZC_FLUSH_ALL);
}

/*
 * When we switch between 3d and 2d rendering all the time, we need to make
 * sure that the other engine is idle first before the new engine goes and
 * fires off.
 */
void
R5xxEngineWaitIdleFull(struct RhdCS *CS)
{
    RHDPtr rhdPtr = RHDPTRI(CS);

    RHDCSGrab(CS, 2);
    RHDCSRegWrite(CS, R5XX_WAIT_UNTIL,
		  R5XX_WAIT_HOST_IDLECLEAN | R5XX_WAIT_3D_IDLECLEAN |
		  R5XX_WAIT_2D_IDLECLEAN | R5XX_WAIT_DMA_GUI_IDLE);

    if (rhdPtr->ThreeDPrivate) {
	struct R5xx3D *State = rhdPtr->ThreeDPrivate;
	State->engineMode = R5XX_ENGINEMODE_IDLE_FULL;
    }
}

/*
 *
 */
void
R5xxEngineWaitIdle3D(struct RhdCS *CS)
{
    struct R5xx3D *State = RHDPTRI(CS)->ThreeDPrivate;

    if (!State)
	return;

    if (State->engineMode == R5XX_ENGINEMODE_IDLE_2D) {
	RHDCSGrab(CS, 2);
	RHDCSRegWrite(CS, R5XX_WAIT_UNTIL, R5XX_WAIT_3D_IDLECLEAN);
    } /* FULL/3D is always good */

    State->engineMode = R5XX_ENGINEMODE_IDLE_3D;
}

/*
 *
 */
void
R5xxEngineWaitIdle2D(struct RhdCS *CS)
{
    struct R5xx3D *State = RHDPTRI(CS)->ThreeDPrivate;

    if (!State)
	return;

    if (State->engineMode == R5XX_ENGINEMODE_IDLE_3D) {
	RHDCSGrab(CS, 2);
	RHDCSRegWrite(CS, R5XX_WAIT_UNTIL,
		      R5XX_WAIT_2D_IDLECLEAN | R5XX_WAIT_DMA_GUI_IDLE);
    } /* FULL/2D is always good */

    State->engineMode = R5XX_ENGINEMODE_IDLE_2D;
}
