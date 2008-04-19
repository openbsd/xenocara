/*
 * Copyright 2008  Luc Verhaegen <lverhaegen@novell.com>
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
#include "r5xx_accel.h"
#include "r5xx_2dregs.h"

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
R5xxFIFOWaitLocal(int scrnIndex, CARD32 required)
{
    int i;

    for (i = 0; i < R5XX_LOOP_COUNT; i++)
	if (required <=
	    (_RHDRegRead(scrnIndex, R5XX_RBBM_STATUS) & R5XX_RBBM_FIFOCNT_MASK))
	    return TRUE;

    xf86DrvMsg(scrnIndex, X_ERROR, "%s: Timeout 0x%08X.\n", __func__,
	       (unsigned int) _RHDRegRead(scrnIndex, R5XX_RBBM_STATUS));
    return FALSE;
}

/*
 * Flush all dirty data in the Pixel Cache to memory.
 */
static Bool
R5xx2DFlush(int scrnIndex)
{
    int i;

    _RHDRegMask(scrnIndex, R5XX_RB3D_DSTCACHE_CTLSTAT,
		R5XX_RB3D_DC_FLUSH_ALL, R5XX_RB3D_DC_FLUSH_ALL);

    for (i = 0; i < R5XX_LOOP_COUNT; i++)
	if (!(_RHDRegRead(scrnIndex, R5XX_RB3D_DSTCACHE_CTLSTAT) & R5XX_RB3D_DC_BUSY))
	    return TRUE;

    xf86DrvMsg(scrnIndex, X_ERROR, "%s: Timeout 0x%08x.\n", __func__,
	       (unsigned int)_RHDRegRead(scrnIndex, R5XX_RB3D_DSTCACHE_CTLSTAT));
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
    int i;

    /* wait for fifo to clear */
    for (i = 0; i < R5XX_LOOP_COUNT; i++)
	if (0x40 == (_RHDRegRead(scrnIndex, R5XX_RBBM_STATUS) & R5XX_RBBM_FIFOCNT_MASK))
	    break;

    if (i == R5XX_LOOP_COUNT) {
	xf86DrvMsg(scrnIndex, X_ERROR, "%s: FIFO Timeout 0x%08X.\n", __func__,
		   (unsigned int) _RHDRegRead(scrnIndex, R5XX_RBBM_STATUS));
	return FALSE;
    }

    /* wait for engine to go idle */
    for (i = 0; i < R5XX_LOOP_COUNT; i++) {
	if (!(_RHDRegRead(scrnIndex, R5XX_RBBM_STATUS) & R5XX_RBBM_ACTIVE)) {
	    R5xx2DFlush(scrnIndex);
	    return TRUE;
	}
    }

    xf86DrvMsg(scrnIndex, X_ERROR, "%s: Idle Timeout 0x%08X.\n", __func__,
	       (unsigned int) _RHDRegRead(scrnIndex, R5XX_RBBM_STATUS));
    return FALSE;
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

    tmp = RHDRegRead(rhdPtr, R5XX_RBBM_SOFT_RESET);
    tmp |= R5XX_SOFT_RESET_CP | R5XX_SOFT_RESET_HI | R5XX_SOFT_RESET_E2;
    RHDRegWrite(rhdPtr, R5XX_RBBM_SOFT_RESET, tmp);

    RHDRegRead(rhdPtr, R5XX_RBBM_SOFT_RESET);
    RHDRegWrite(rhdPtr, R5XX_RBBM_SOFT_RESET, 0);

    tmp = RHDRegRead(rhdPtr, R5XX_RB3D_DSTCACHE_MODE);
    RHDRegWrite(rhdPtr, R5XX_RB3D_DSTCACHE_MODE, tmp | (1 << 17)); /* FIXME */

    RHDRegWrite(rhdPtr, R5XX_HOST_PATH_CNTL, save | R5XX_HDP_SOFT_RESET);
    RHDRegRead(rhdPtr, R5XX_HOST_PATH_CNTL);
    RHDRegWrite(rhdPtr, R5XX_HOST_PATH_CNTL, save);
}

/*
 *
 */
void
R5xx2DSetup(ScrnInfoPtr pScrn)
{
    RHDPtr rhdPtr = RHDPTR(pScrn);
    struct R5xx2DInfo *TwoDInfo = rhdPtr->TwoDInfo;

    RHDFUNC(rhdPtr);

    /* Setup engine location. This shouldn't be necessary since we
     * set them appropriately before any accel ops, but let's avoid
     * random bogus DMA in case we inadvertently trigger the engine
     * in the wrong place (happened). */
    R5xxFIFOWaitLocal(rhdPtr->scrnIndex, 2);
    RHDRegWrite(rhdPtr, R5XX_DST_PITCH_OFFSET, TwoDInfo->dst_pitch_offset);
    RHDRegWrite(rhdPtr, R5XX_SRC_PITCH_OFFSET, TwoDInfo->dst_pitch_offset);

    R5xxFIFOWaitLocal(rhdPtr->scrnIndex, 1);
#if X_BYTE_ORDER == X_BIG_ENDIAN
    RHDRegMask(rhdPtr, R5XX_DP_DATATYPE,
	       R5XX_HOST_BIG_ENDIAN_EN, R5XX_HOST_BIG_ENDIAN_EN);
#else
    RHDRegMask(rhdPtr, R5XX_DP_DATATYPE, 0, R5XX_HOST_BIG_ENDIAN_EN);
#endif

    RHDRegWrite(rhdPtr, R5XX_SURFACE_CNTL, TwoDInfo->surface_cntl);

    R5xxFIFOWaitLocal(rhdPtr->scrnIndex, 1);
    RHDRegWrite(rhdPtr, R5XX_DEFAULT_SC_BOTTOM_RIGHT,
		R5XX_DEFAULT_SC_RIGHT_MAX | R5XX_DEFAULT_SC_BOTTOM_MAX);
    R5xxFIFOWaitLocal(rhdPtr->scrnIndex, 1);
    RHDRegWrite(rhdPtr, R5XX_DP_GUI_MASTER_CNTL, TwoDInfo->control |
		R5XX_GMC_BRUSH_SOLID_COLOR | R5XX_GMC_SRC_DATATYPE_COLOR);

    R5xxFIFOWaitLocal(rhdPtr->scrnIndex, 5);
    RHDRegWrite(rhdPtr, R5XX_DP_BRUSH_FRGD_CLR, 0xFFFFFFFF);
    RHDRegWrite(rhdPtr, R5XX_DP_BRUSH_BKGD_CLR, 0x00000000);
    RHDRegWrite(rhdPtr, R5XX_DP_SRC_FRGD_CLR, 0xFFFFFFFF);
    RHDRegWrite(rhdPtr, R5XX_DP_SRC_BKGD_CLR, 0x00000000);
    RHDRegWrite(rhdPtr, R5XX_DP_WRITE_MASK, 0xFFFFFFFF);

    R5xx2DIdleLocal(rhdPtr->scrnIndex);
}

/*
 * Not called from RHDPreInit, but this sets up things which can only
 * change through PreInit.
 */
static void
R5xx2DPreInit(ScrnInfoPtr pScrn)
{
    RHDPtr rhdPtr = RHDPTR(pScrn);
    CARD8 datatype, Bytes;
    struct R5xx2DInfo *TwoDInfo;

    RHDFUNC(rhdPtr);

    TwoDInfo = xnfcalloc(1, sizeof(struct R5xx2DInfo));
    TwoDInfo->scrnIndex = pScrn->scrnIndex;
    rhdPtr->TwoDInfo = TwoDInfo;

    switch (pScrn->depth) {
    case 8:
	Bytes = 1;
	datatype = R5XX_DATATYPE_CI8;
	break;
    case 15:
	Bytes = 2;
	datatype = R5XX_DATATYPE_ARGB1555;
	break;
    case 16:
	Bytes = 2;
	datatype = R5XX_DATATYPE_RGB565;
	break;
    case 24:
    case 32:
	Bytes = 4;
	datatype = R5XX_DATATYPE_ARGB8888;
	break;
    default:
	xf86DrvMsg(rhdPtr->scrnIndex, X_ERROR, "%s: Unhandled pixel depth: %d.\n",
		   __func__, pScrn->depth);
	Bytes = 4;
	datatype = R5XX_DATATYPE_ARGB8888;

    }
    TwoDInfo->control = (datatype << R5XX_GMC_DST_DATATYPE_SHIFT) |
	R5XX_GMC_CLR_CMP_CNTL_DIS | R5XX_GMC_DST_PITCH_OFFSET_CNTL;

#if X_BYTE_ORDER == X_BIG_ENDIAN
    switch (pScrn->depth) {
    case 8:
	TwoDInfo->surface_cntl = 0;
	break;
    case 15:
    case 16:
	TwoDInfo->surface_cntl =
	    R5XX_NONSURF_AP0_SWP_16BPP | R5XX_NONSURF_AP1_SWP_16BPP;
	break;
    case 24:
    case 32:
	TwoDInfo->surface_cntl =
	    R5XX_NONSURF_AP0_SWP_32BPP | R5XX_NONSURF_AP1_SWP_32BPP;
	break;
    }
#else
    TwoDInfo->surface_cntl = 0;
#endif

    TwoDInfo->dst_pitch_offset = (((pScrn->displayWidth * Bytes) / 64) << 22) |
	((rhdPtr->FbIntAddress + rhdPtr->FbScanoutStart) >> 10);

    /* for ScanlineScreenToScreenColorExpand */
    TwoDInfo->Buffer = xnfcalloc(1, ((pScrn->virtualX + 31) / 32 * 4) +
				 (pScrn->virtualX * Bytes));
}

/*
 *
 */
void
R5xxFIFOWait(int scrnIndex, CARD32 required)
{
    if (!R5xxFIFOWaitLocal(scrnIndex, required)) {
	R5xx2DReset(xf86Screens[scrnIndex]);
	R5xx2DSetup(xf86Screens[scrnIndex]);
    }
}

/*
 *
 */
void
R5xx2DIdle(ScrnInfoPtr pScrn)
{
    if (!R5xx2DIdleLocal(pScrn->scrnIndex)) {
	R5xx2DReset(pScrn);
	R5xx2DSetup(pScrn);
    }
}

/*
 *
 */
void
R5xx2DInit(ScrnInfoPtr pScrn)
{
    RHDPtr rhdPtr = RHDPTR(pScrn);

    R5xx2DPreInit(pScrn);

    RHDRegWrite(rhdPtr, R5XX_RB3D_CNTL, 0);

    R5xx2DReset(pScrn);
    R5xx2DSetup(pScrn);
}

/*
 *
 */
void
R5xx2DDestroy(ScrnInfoPtr pScrn)
{
    RHDPtr rhdPtr = RHDPTR(pScrn);
    struct R5xx2DInfo *TwoDInfo = rhdPtr->TwoDInfo;

    if (!TwoDInfo)
	return;

    if (TwoDInfo->Buffer)
	xfree(TwoDInfo->Buffer);
    xfree(TwoDInfo);
    rhdPtr->TwoDInfo = NULL;
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
