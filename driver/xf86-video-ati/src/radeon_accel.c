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
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/*
 * Authors:
 *   Kevin E. Martin <martin@xfree86.org>
 *   Rickard E. Faith <faith@valinux.com>
 *   Alan Hourihane <alanh@fairlite.demon.co.uk>
 *
 * Credits:
 *
 *   Thanks to Ani Joshi <ajoshi@shell.unixbox.com> for providing source
 *   code to his Radeon driver.  Portions of this file are based on the
 *   initialization code for that driver.
 *
 * References:
 *
 * !!!! FIXME !!!!
 *   RAGE 128 VR/ RAGE 128 GL Register Reference Manual (Technical
 *   Reference Manual P/N RRG-G04100-C Rev. 0.04), ATI Technologies: April
 *   1999.
 *
 *   RAGE 128 Software Development Manual (Technical Reference Manual P/N
 *   SDK-G04000 Rev. 0.01), ATI Technologies: June 1999.
 *
 * Notes on unimplemented XAA optimizations:
 *
 *   SetClipping:   This has been removed as XAA expects 16bit registers
 *                  for full clipping.
 *   TwoPointLine:  The Radeon supports this. Not Bresenham.
 *   DashedLine with non-power-of-two pattern length: Apparently, there is
 *                  no way to set the length of the pattern -- it is always
 *                  assumed to be 8 or 32 (or 1024?).
 *   ScreenToScreenColorExpandFill: See p. 4-17 of the Technical Reference
 *                  Manual where it states that monochrome expansion of frame
 *                  buffer data is not supported.
 *   CPUToScreenColorExpandFill, direct: The implementation here uses a hybrid
 *                  direct/indirect method.  If we had more data registers,
 *                  then we could do better.  If XAA supported a trigger write
 *                  address, the code would be simpler.
 *   Color8x8PatternFill: Apparently, an 8x8 color brush cannot take an 8x8
 *                  pattern from frame buffer memory.
 *   ImageWrites:   Same as CPUToScreenColorExpandFill
 *
 */

#include <errno.h>
#include <string.h>
#include <assert.h>
				/* Driver data structures */
#include "radeon.h"
#include "radeon_reg.h"
#include "r600_reg.h"
#include "radeon_macros.h"
#include "radeon_probe.h"
#include "radeon_version.h"
#ifdef XF86DRI
#define _XF86DRI_SERVER_
#include "radeon_drm.h"
#endif

#include "ati_pciids_gen.h"

				/* Line support */
#include "miline.h"

				/* X and server generic header files */
#include "xf86.h"

static void R600EngineReset(ScrnInfoPtr pScrn);

#ifdef USE_XAA
static struct {
    int rop;
    int pattern;
} RADEON_ROP[] = {
    { RADEON_ROP3_ZERO, RADEON_ROP3_ZERO }, /* GXclear        */
    { RADEON_ROP3_DSa,  RADEON_ROP3_DPa  }, /* Gxand          */
    { RADEON_ROP3_SDna, RADEON_ROP3_PDna }, /* GXandReverse   */
    { RADEON_ROP3_S,    RADEON_ROP3_P    }, /* GXcopy         */
    { RADEON_ROP3_DSna, RADEON_ROP3_DPna }, /* GXandInverted  */
    { RADEON_ROP3_D,    RADEON_ROP3_D    }, /* GXnoop         */
    { RADEON_ROP3_DSx,  RADEON_ROP3_DPx  }, /* GXxor          */
    { RADEON_ROP3_DSo,  RADEON_ROP3_DPo  }, /* GXor           */
    { RADEON_ROP3_DSon, RADEON_ROP3_DPon }, /* GXnor          */
    { RADEON_ROP3_DSxn, RADEON_ROP3_PDxn }, /* GXequiv        */
    { RADEON_ROP3_Dn,   RADEON_ROP3_Dn   }, /* GXinvert       */
    { RADEON_ROP3_SDno, RADEON_ROP3_PDno }, /* GXorReverse    */
    { RADEON_ROP3_Sn,   RADEON_ROP3_Pn   }, /* GXcopyInverted */
    { RADEON_ROP3_DSno, RADEON_ROP3_DPno }, /* GXorInverted   */
    { RADEON_ROP3_DSan, RADEON_ROP3_DPan }, /* GXnand         */
    { RADEON_ROP3_ONE,  RADEON_ROP3_ONE  }  /* GXset          */
};
#endif

/* The FIFO has 64 slots.  This routines waits until at least `entries'
 * of these slots are empty.
 */
void RADEONWaitForFifoFunction(ScrnInfoPtr pScrn, int entries)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    int            i;

    for (;;) {
	for (i = 0; i < RADEON_TIMEOUT; i++) {
	    info->accel_state->fifo_slots =
		INREG(RADEON_RBBM_STATUS) & RADEON_RBBM_FIFOCNT_MASK;
	    if (info->accel_state->fifo_slots >= entries) return;
	}
	xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		       "FIFO timed out: %u entries, stat=0x%08x\n",
		       (unsigned int)INREG(RADEON_RBBM_STATUS) & RADEON_RBBM_FIFOCNT_MASK,
		       (unsigned int)INREG(RADEON_RBBM_STATUS));
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "FIFO timed out, resetting engine...\n");
	RADEONEngineReset(pScrn);
	RADEONEngineRestore(pScrn);
#ifdef XF86DRI
	if (info->directRenderingEnabled) {
	    RADEONCP_RESET(pScrn, info);
	    RADEONCP_START(pScrn, info);
	}
#endif
    }
}

void R600WaitForFifoFunction(ScrnInfoPtr pScrn, int entries)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    int            i;

    for (;;) {
	for (i = 0; i < RADEON_TIMEOUT; i++) {
	    if (info->ChipFamily >= CHIP_FAMILY_RV770)
		info->accel_state->fifo_slots =
		    INREG(R600_GRBM_STATUS) & R700_CMDFIFO_AVAIL_MASK;
	    else
		info->accel_state->fifo_slots =
		    INREG(R600_GRBM_STATUS) & R600_CMDFIFO_AVAIL_MASK;
	    if (info->accel_state->fifo_slots >= entries) return;
	}
	xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		       "FIFO timed out: stat=0x%08x\n",
		       (unsigned int)INREG(R600_GRBM_STATUS));
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "FIFO timed out, resetting engine...\n");
	R600EngineReset(pScrn);
#ifdef XF86DRI
	if (info->directRenderingEnabled) {
	    RADEONCP_RESET(pScrn, info);
	    RADEONCP_START(pScrn, info);
	}
#endif
    }
}

/* Flush all dirty data in the Pixel Cache to memory */
void RADEONEngineFlush(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    int            i;

    if (info->ChipFamily <= CHIP_FAMILY_RV280) {
	OUTREGP(RADEON_RB3D_DSTCACHE_CTLSTAT,
		RADEON_RB3D_DC_FLUSH_ALL,
		~RADEON_RB3D_DC_FLUSH_ALL);
	for (i = 0; i < RADEON_TIMEOUT; i++) {
	    if (!(INREG(RADEON_RB3D_DSTCACHE_CTLSTAT) & RADEON_RB3D_DC_BUSY))
		break;
	}
	if (i == RADEON_TIMEOUT) {
	    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
			   "DC flush timeout: %x\n",
			   (unsigned int)INREG(RADEON_RB3D_DSTCACHE_CTLSTAT));
	}
    } else {
	OUTREGP(R300_DSTCACHE_CTLSTAT,
		R300_RB2D_DC_FLUSH_ALL,
		~R300_RB2D_DC_FLUSH_ALL);
	for (i = 0; i < RADEON_TIMEOUT; i++) {
	    if (!(INREG(R300_DSTCACHE_CTLSTAT) & R300_RB2D_DC_BUSY))
		break;
	}
	if (i == RADEON_TIMEOUT) {
	    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
			   "DC flush timeout: %x\n",
			   (unsigned int)INREG(R300_DSTCACHE_CTLSTAT));
	}
    }
}

/* Reset graphics card to known state */
void RADEONEngineReset(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    uint32_t       clock_cntl_index;
    uint32_t       mclk_cntl;
    uint32_t       rbbm_soft_reset;
    uint32_t       host_path_cntl;

    /* The following RBBM_SOFT_RESET sequence can help un-wedge
     * an R300 after the command processor got stuck.
     */
    rbbm_soft_reset = INREG(RADEON_RBBM_SOFT_RESET);
    OUTREG(RADEON_RBBM_SOFT_RESET, (rbbm_soft_reset |
                                   RADEON_SOFT_RESET_CP |
                                   RADEON_SOFT_RESET_HI |
                                   RADEON_SOFT_RESET_SE |
                                   RADEON_SOFT_RESET_RE |
                                   RADEON_SOFT_RESET_PP |
                                   RADEON_SOFT_RESET_E2 |
                                   RADEON_SOFT_RESET_RB));
    INREG(RADEON_RBBM_SOFT_RESET);
    OUTREG(RADEON_RBBM_SOFT_RESET, (rbbm_soft_reset & (uint32_t)
                                   ~(RADEON_SOFT_RESET_CP |
                                     RADEON_SOFT_RESET_HI |
                                     RADEON_SOFT_RESET_SE |
                                     RADEON_SOFT_RESET_RE |
                                     RADEON_SOFT_RESET_PP |
                                     RADEON_SOFT_RESET_E2 |
                                     RADEON_SOFT_RESET_RB)));
    INREG(RADEON_RBBM_SOFT_RESET);
    OUTREG(RADEON_RBBM_SOFT_RESET, rbbm_soft_reset);
    INREG(RADEON_RBBM_SOFT_RESET);

    RADEONEngineFlush(pScrn);

    clock_cntl_index = INREG(RADEON_CLOCK_CNTL_INDEX);
    RADEONPllErrataAfterIndex(info);

#if 0 /* taken care of by new PM code */
    /* Some ASICs have bugs with dynamic-on feature, which are
     * ASIC-version dependent, so we force all blocks on for now
     */
    if (info->HasCRTC2) {
	uint32_t tmp;

	tmp = INPLL(pScrn, RADEON_SCLK_CNTL);
	OUTPLL(RADEON_SCLK_CNTL, ((tmp & ~RADEON_DYN_STOP_LAT_MASK) |
				  RADEON_CP_MAX_DYN_STOP_LAT |
				  RADEON_SCLK_FORCEON_MASK));

	if (info->ChipFamily == CHIP_FAMILY_RV200) {
	    tmp = INPLL(pScrn, RADEON_SCLK_MORE_CNTL);
	    OUTPLL(RADEON_SCLK_MORE_CNTL, tmp | RADEON_SCLK_MORE_FORCEON);
	}
    }
#endif /* new PM code */

    mclk_cntl = INPLL(pScrn, RADEON_MCLK_CNTL);

#if 0 /* handled by new PM code */
    OUTPLL(RADEON_MCLK_CNTL, (mclk_cntl |
			      RADEON_FORCEON_MCLKA |
			      RADEON_FORCEON_MCLKB |
			      RADEON_FORCEON_YCLKA |
			      RADEON_FORCEON_YCLKB |
			      RADEON_FORCEON_MC |
			      RADEON_FORCEON_AIC));
#endif /* new PM code */

    /* Soft resetting HDP thru RBBM_SOFT_RESET register can cause some
     * unexpected behaviour on some machines.  Here we use
     * RADEON_HOST_PATH_CNTL to reset it.
     */
    host_path_cntl = INREG(RADEON_HOST_PATH_CNTL);
    rbbm_soft_reset = INREG(RADEON_RBBM_SOFT_RESET);

    if (IS_R300_VARIANT || IS_AVIVO_VARIANT) {
	uint32_t tmp;

	OUTREG(RADEON_RBBM_SOFT_RESET, (rbbm_soft_reset |
					RADEON_SOFT_RESET_CP |
					RADEON_SOFT_RESET_HI |
					RADEON_SOFT_RESET_E2));
	INREG(RADEON_RBBM_SOFT_RESET);
	OUTREG(RADEON_RBBM_SOFT_RESET, 0);
	tmp = INREG(RADEON_RB3D_DSTCACHE_MODE);
	OUTREG(RADEON_RB3D_DSTCACHE_MODE, tmp | (1 << 17)); /* FIXME */
    } else {
	OUTREG(RADEON_RBBM_SOFT_RESET, (rbbm_soft_reset |
					RADEON_SOFT_RESET_CP |
					RADEON_SOFT_RESET_SE |
					RADEON_SOFT_RESET_RE |
					RADEON_SOFT_RESET_PP |
					RADEON_SOFT_RESET_E2 |
					RADEON_SOFT_RESET_RB));
	INREG(RADEON_RBBM_SOFT_RESET);
	OUTREG(RADEON_RBBM_SOFT_RESET, (rbbm_soft_reset & (uint32_t)
					~(RADEON_SOFT_RESET_CP |
					  RADEON_SOFT_RESET_SE |
					  RADEON_SOFT_RESET_RE |
					  RADEON_SOFT_RESET_PP |
					  RADEON_SOFT_RESET_E2 |
					  RADEON_SOFT_RESET_RB)));
	INREG(RADEON_RBBM_SOFT_RESET);
    }

    if (!IS_R300_VARIANT && !IS_AVIVO_VARIANT)
	OUTREG(RADEON_RBBM_SOFT_RESET, rbbm_soft_reset);

    OUTREG(RADEON_CLOCK_CNTL_INDEX, clock_cntl_index);
    RADEONPllErrataAfterIndex(info);
    OUTPLL(pScrn, RADEON_MCLK_CNTL, mclk_cntl);
}

/* Reset graphics card to known state */
static void R600EngineReset(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    uint32_t cp_ptr, cp_me_cntl, cp_rb_cntl;

    cp_ptr = INREG(R600_CP_RB_WPTR);

    cp_me_cntl = INREG(R600_CP_ME_CNTL);
    OUTREG(R600_CP_ME_CNTL, 0x10000000);

    OUTREG(R600_GRBM_SOFT_RESET, 0x7fff);
    INREG(R600_GRBM_SOFT_RESET);
    usleep (50);
    OUTREG(R600_GRBM_SOFT_RESET, 0);
    INREG(R600_GRBM_SOFT_RESET);

    OUTREG(R600_CP_RB_WPTR_DELAY, 0);
    cp_rb_cntl = INREG(R600_CP_RB_CNTL);
    OUTREG(R600_CP_RB_CNTL, 0x80000000);

    OUTREG(R600_CP_RB_RPTR_WR, cp_ptr);
    OUTREG(R600_CP_RB_WPTR, cp_ptr);
    OUTREG(R600_CP_RB_CNTL, cp_rb_cntl);
    OUTREG(R600_CP_ME_CNTL, cp_me_cntl);

}

/* Restore the acceleration hardware to its previous state */
void RADEONEngineRestore(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;

    if (info->cs)
      return;

    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "EngineRestore (%d/%d)\n",
		   info->CurrentLayout.pixel_code,
		   info->CurrentLayout.bitsPerPixel);

    /* Setup engine location. This shouldn't be necessary since we
     * set them appropriately before any accel ops, but let's avoid
     * random bogus DMA in case we inadvertently trigger the engine
     * in the wrong place (happened).
     */
    RADEONWaitForFifo(pScrn, 2);
    OUTREG(RADEON_DST_PITCH_OFFSET, info->accel_state->dst_pitch_offset);
    OUTREG(RADEON_SRC_PITCH_OFFSET, info->accel_state->dst_pitch_offset);

    RADEONWaitForFifo(pScrn, 1);
#if X_BYTE_ORDER == X_BIG_ENDIAN
    OUTREGP(RADEON_DP_DATATYPE,
	    RADEON_HOST_BIG_ENDIAN_EN,
	    ~RADEON_HOST_BIG_ENDIAN_EN);
#else
    OUTREGP(RADEON_DP_DATATYPE, 0, ~RADEON_HOST_BIG_ENDIAN_EN);
#endif

    /* Restore SURFACE_CNTL */
    OUTREG(RADEON_SURFACE_CNTL, info->ModeReg->surface_cntl);

    RADEONWaitForFifo(pScrn, 1);
    OUTREG(RADEON_DEFAULT_SC_BOTTOM_RIGHT, (RADEON_DEFAULT_SC_RIGHT_MAX
					    | RADEON_DEFAULT_SC_BOTTOM_MAX));
    RADEONWaitForFifo(pScrn, 1);
    OUTREG(RADEON_DP_GUI_MASTER_CNTL, (info->accel_state->dp_gui_master_cntl
				       | RADEON_GMC_BRUSH_SOLID_COLOR
				       | RADEON_GMC_SRC_DATATYPE_COLOR));

    RADEONWaitForFifo(pScrn, 5);
    OUTREG(RADEON_DP_BRUSH_FRGD_CLR, 0xffffffff);
    OUTREG(RADEON_DP_BRUSH_BKGD_CLR, 0x00000000);
    OUTREG(RADEON_DP_SRC_FRGD_CLR,   0xffffffff);
    OUTREG(RADEON_DP_SRC_BKGD_CLR,   0x00000000);
    OUTREG(RADEON_DP_WRITE_MASK,     0xffffffff);

    RADEONWaitForIdleMMIO(pScrn);

    info->accel_state->XInited3D = FALSE;
}

static int RADEONDRMGetNumPipes(ScrnInfoPtr pScrn, int *num_pipes)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    if (info->dri->pKernelDRMVersion->version_major < 2) {
        drm_radeon_getparam_t np;

        memset(&np, 0, sizeof(np));
        np.param = RADEON_PARAM_NUM_GB_PIPES;
        np.value = num_pipes;
        return drmCommandWriteRead(info->dri->drmFD, DRM_RADEON_GETPARAM, &np, sizeof(np));
    } else {
        struct drm_radeon_info np2;
        np2.value = (unsigned long)num_pipes;
        np2.request = RADEON_INFO_NUM_GB_PIPES;
        return drmCommandWriteRead(info->dri->drmFD, DRM_RADEON_INFO, &np2, sizeof(np2));
    }
}

/* Initialize the acceleration hardware */
void RADEONEngineInit(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    int datatype = 0;
    info->accel_state->num_gb_pipes = 0;

    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "EngineInit (%d/%d)\n",
		   info->CurrentLayout.pixel_code,
		   info->CurrentLayout.bitsPerPixel);

#ifdef XF86DRI
    if (info->directRenderingEnabled && (IS_R300_3D || IS_R500_3D)) {
	int num_pipes;

	if(RADEONDRMGetNumPipes(pScrn, &num_pipes) < 0) {
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		       "Failed to determine num pipes from DRM, falling back to "
		       "manual look-up!\n");
	    info->accel_state->num_gb_pipes = 0;
	} else {
	    info->accel_state->num_gb_pipes = num_pipes;
	}
    }
#endif

    if (!info->cs) {
	if ((info->ChipFamily == CHIP_FAMILY_RV410) ||
	    (info->ChipFamily == CHIP_FAMILY_R420)  ||
	    (info->ChipFamily == CHIP_FAMILY_RS600) ||
	    (info->ChipFamily == CHIP_FAMILY_RS690) ||
	    (info->ChipFamily == CHIP_FAMILY_RS740) ||
	    (info->ChipFamily == CHIP_FAMILY_RS400) ||
	    (info->ChipFamily == CHIP_FAMILY_RS480) ||
	    IS_R500_3D) {
	    if (info->accel_state->num_gb_pipes == 0) {
		uint32_t gb_pipe_sel = INREG(R400_GB_PIPE_SELECT);

		info->accel_state->num_gb_pipes = ((gb_pipe_sel >> 12) & 0x3) + 1;
		if (IS_R500_3D)
		    OUTPLL(pScrn, R500_DYN_SCLK_PWMEM_PIPE, (1 | ((gb_pipe_sel >> 8) & 0xf) << 4));
	    }
	} else {
	    if (info->accel_state->num_gb_pipes == 0) {
		if ((info->ChipFamily == CHIP_FAMILY_R300) ||
		    (info->ChipFamily == CHIP_FAMILY_R350)) {
		    /* R3xx chips */
		    info->accel_state->num_gb_pipes = 2;
		} else {
		    /* RV3xx chips */
		    info->accel_state->num_gb_pipes = 1;
		}
	    }
	}

	/* SE cards only have 1 quadpipe */
	if ((info->Chipset == PCI_CHIP_RV410_5E4C) ||
	    (info->Chipset == PCI_CHIP_RV410_5E4F) ||
	    (info->Chipset == PCI_CHIP_R300_AD) ||
	    (info->Chipset == PCI_CHIP_R350_AH))
	    info->accel_state->num_gb_pipes = 1;

	if (IS_R300_3D || IS_R500_3D)
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		       "num quad-pipes is %d\n", info->accel_state->num_gb_pipes);

	if (IS_R300_3D || IS_R500_3D) {
	    uint32_t gb_tile_config = (R300_ENABLE_TILING | R300_TILE_SIZE_16);
	    
	    switch(info->accel_state->num_gb_pipes) {
	    case 2: gb_tile_config |= R300_PIPE_COUNT_R300; break;
	    case 3: gb_tile_config |= R300_PIPE_COUNT_R420_3P; break;
	    case 4: gb_tile_config |= R300_PIPE_COUNT_R420; break;
	    default:
	    case 1: gb_tile_config |= R300_PIPE_COUNT_RV350; break;
	    }

	    OUTREG(R300_GB_TILE_CONFIG, gb_tile_config);
	    OUTREG(RADEON_WAIT_UNTIL, RADEON_WAIT_2D_IDLECLEAN | RADEON_WAIT_3D_IDLECLEAN);
	    if (info->ChipFamily >= CHIP_FAMILY_R420)
		OUTREG(R300_DST_PIPE_CONFIG, INREG(R300_DST_PIPE_CONFIG) | R300_PIPE_AUTO_CONFIG);
	    OUTREG(R300_RB2D_DSTCACHE_MODE, (INREG(R300_RB2D_DSTCACHE_MODE) |
					     R300_DC_AUTOFLUSH_ENABLE |
					     R300_DC_DC_DISABLE_IGNORE_PE));
	} else
	    OUTREG(RADEON_RB3D_CNTL, 0);
	
	RADEONEngineReset(pScrn);
    }

    switch (info->CurrentLayout.pixel_code) {
    case 8:  datatype = 2; break;
    case 15: datatype = 3; break;
    case 16: datatype = 4; break;
    case 24: datatype = 5; break;
    case 32: datatype = 6; break;
    default:
	xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		       "Unknown depth/bpp = %d/%d (code = %d)\n",
		       info->CurrentLayout.depth,
		       info->CurrentLayout.bitsPerPixel,
		       info->CurrentLayout.pixel_code);
    }

    info->accel_state->dp_gui_master_cntl =
	((datatype << RADEON_GMC_DST_DATATYPE_SHIFT)
	 | RADEON_GMC_CLR_CMP_CNTL_DIS
	 | RADEON_GMC_DST_PITCH_OFFSET_CNTL);

    RADEONEngineRestore(pScrn);
}

uint32_t radeonGetPixmapOffset(PixmapPtr pPix)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pPix->drawable.pScreen);
    RADEONInfoPtr info = RADEONPTR(pScrn);
    uint32_t offset = 0;
    if (info->cs)
	return 0;
#ifdef USE_EXA
    if (info->useEXA) {
	offset = exaGetPixmapOffset(pPix);
    } else
#endif
    {
	offset = pPix->devPrivate.ptr - info->FB;
    }
    offset += info->fbLocation + pScrn->fbOffset;
    return offset;
}

int radeon_cs_space_remaining(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);

#ifdef XF86DRM_MODE
    if (info->cs)
	return (info->cs->ndw - info->cs->cdw);
    else
#endif
        return (info->cp->indirectBuffer->total - info->cp->indirectBuffer->used) / (int)sizeof(uint32_t);
}

#define ACCEL_MMIO
#define ACCEL_PREAMBLE()        unsigned char *RADEONMMIO = info->MMIO
#define BEGIN_ACCEL(n)          RADEONWaitForFifo(pScrn, (n))
#define OUT_ACCEL_REG(reg, val) OUTREG(reg, val)
#define FINISH_ACCEL()

#include "radeon_commonfuncs.c"
#if defined(RENDER) && defined(USE_XAA)
#include "radeon_render.c"
#endif
#include "radeon_accelfuncs.c"

#undef ACCEL_MMIO
#undef ACCEL_PREAMBLE
#undef BEGIN_ACCEL
#undef OUT_ACCEL_REG
#undef FINISH_ACCEL

#ifdef XF86DRI

#define ACCEL_CP
#define ACCEL_PREAMBLE()						\
    RING_LOCALS;							\
    RADEONCP_REFRESH(pScrn, info)
#define BEGIN_ACCEL(n)          BEGIN_RING(2*(n))
#define OUT_ACCEL_REG(reg, val) OUT_RING_REG(reg, val)
#define FINISH_ACCEL()          ADVANCE_RING()


#include "radeon_commonfuncs.c"
#if defined(RENDER) && defined(USE_XAA)
#include "radeon_render.c"
#endif
#include "radeon_accelfuncs.c"

#undef ACCEL_CP
#undef ACCEL_PREAMBLE
#undef BEGIN_ACCEL
#undef OUT_ACCEL_REG
#undef FINISH_ACCEL

/* Stop the CP */
int RADEONCPStop(ScrnInfoPtr pScrn, RADEONInfoPtr info)
{
    drm_radeon_cp_stop_t  stop;
    int              ret, i;

    stop.flush = 1;
    stop.idle  = 1;

    ret = drmCommandWrite(info->dri->drmFD, DRM_RADEON_CP_STOP, &stop,
			  sizeof(drm_radeon_cp_stop_t));

    if (ret == 0) {
	return 0;
    } else if (errno != EBUSY) {
	return -errno;
    }

    stop.flush = 0;

    i = 0;
    do {
	ret = drmCommandWrite(info->dri->drmFD, DRM_RADEON_CP_STOP, &stop,
			      sizeof(drm_radeon_cp_stop_t));
    } while (ret && errno == EBUSY && i++ < RADEON_IDLE_RETRY);

    if (ret == 0) {
	return 0;
    } else if (errno != EBUSY) {
	return -errno;
    }

    stop.idle = 0;

    if (drmCommandWrite(info->dri->drmFD, DRM_RADEON_CP_STOP,
			&stop, sizeof(drm_radeon_cp_stop_t))) {
	return -errno;
    } else {
	return 0;
    }
}

#define RADEON_IB_RESERVE (16 * sizeof(uint32_t))

/* Get an indirect buffer for the CP 2D acceleration commands  */
drmBufPtr RADEONCPGetBuffer(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info = RADEONPTR(pScrn);
    drmDMAReq      dma;
    drmBufPtr      buf = NULL;
    int            indx = 0;
    int            size = 0;
    int            i = 0;
    int            ret;

#if 0
    /* FIXME: pScrn->pScreen has not been initialized when this is first
     * called from RADEONSelectBuffer via RADEONDRICPInit.  We could use
     * the screen index from pScrn, which is initialized, and then get
     * the screen from screenInfo.screens[index], but that is a hack.
     */
    dma.context = DRIGetContext(pScrn->pScreen);
#else
    /* This is the X server's context */
    dma.context = 0x00000001;
#endif

    dma.send_count    = 0;
    dma.send_list     = NULL;
    dma.send_sizes    = NULL;
    dma.flags         = 0;
    dma.request_count = 1;
    dma.request_size  = RADEON_BUFFER_SIZE;
    dma.request_list  = &indx;
    dma.request_sizes = &size;
    dma.granted_count = 0;

    while (1) {
	do {
	    ret = drmDMA(info->dri->drmFD, &dma);
	    if (ret && ret != -EBUSY) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			   "%s: CP GetBuffer %d\n", __FUNCTION__, ret);
	    }
	} while ((ret == -EBUSY) && (i++ < RADEON_TIMEOUT));

	if (ret == 0) {
	    buf = &info->dri->buffers->list[indx];
	    buf->used = 0;
	    if (RADEON_VERBOSE) {
		xf86DrvMsg(pScrn->scrnIndex, X_INFO,
			   "   GetBuffer returning %d %p\n",
			   buf->idx, buf->address);
	    }
	    return buf;
	}

	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "GetBuffer timed out, resetting engine...\n");

	if (info->ChipFamily < CHIP_FAMILY_R600) {
	    RADEONEngineReset(pScrn);
	    RADEONEngineRestore(pScrn);
	} else
	    R600EngineReset(pScrn);

	/* Always restart the engine when doing CP 2D acceleration */
	RADEONCP_RESET(pScrn, info);
	RADEONCP_START(pScrn, info);
    }
}

/* Flush the indirect buffer to the kernel for submission to the card */
void RADEONCPFlushIndirect(ScrnInfoPtr pScrn, int discard)
{
    RADEONInfoPtr      info   = RADEONPTR(pScrn);
    drmBufPtr          buffer = info->cp->indirectBuffer;
    int                start  = info->cp->indirectStart;
    drm_radeon_indirect_t  indirect;

    assert(!info->cs);
    if (!buffer) return;
    if (start == buffer->used && !discard) return;

    if (RADEON_VERBOSE) {
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Flushing buffer %d\n",
		   buffer->idx);
    }

    if (info->ChipFamily >= CHIP_FAMILY_R600) {
	if (buffer->used & 0x3c) {
	    RING_LOCALS;

	    while (buffer->used & 0x3c) {
		BEGIN_RING(1);
		OUT_RING(CP_PACKET2()); /* fill up to multiple of 16 dwords */
		ADVANCE_RING();
	    }
	}
    }

    indirect.idx     = buffer->idx;
    indirect.start   = start;
    indirect.end     = buffer->used;
    indirect.discard = discard;

    drmCommandWriteRead(info->dri->drmFD, DRM_RADEON_INDIRECT,
			&indirect, sizeof(drm_radeon_indirect_t));

    if (discard) {
	info->cp->indirectBuffer = RADEONCPGetBuffer(pScrn);
	info->cp->indirectStart  = 0;
    } else {
	/* Start on a double word boundary */
	info->cp->indirectStart  = buffer->used = RADEON_ALIGN(buffer->used, 8);
	if (RADEON_VERBOSE) {
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "   Starting at %d\n",
		       info->cp->indirectStart);
	}
    }
}

/* Flush and release the indirect buffer */
void RADEONCPReleaseIndirect(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr      info   = RADEONPTR(pScrn);
    drmBufPtr          buffer = info->cp->indirectBuffer;
    int                start  = info->cp->indirectStart;
    drm_radeon_indirect_t  indirect;

    assert(!info->cs);
    if (info->ChipFamily >= CHIP_FAMILY_R600) {
	if (buffer && (buffer->used & 0x3c)) {
	    RING_LOCALS;

	    while (buffer->used & 0x3c) {
		BEGIN_RING(1);
		OUT_RING(CP_PACKET2()); /* fill up to multiple of 16 dwords */
		ADVANCE_RING();
	    }
	}
    }

    info->cp->indirectBuffer = NULL;
    info->cp->indirectStart  = 0;

    if (!buffer) return;

    if (RADEON_VERBOSE) {
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Releasing buffer %d\n",
		   buffer->idx);
    }

    indirect.idx     = buffer->idx;
    indirect.start   = start;
    indirect.end     = buffer->used;
    indirect.discard = 1;

    drmCommandWriteRead(info->dri->drmFD, DRM_RADEON_INDIRECT,
			&indirect, sizeof(drm_radeon_indirect_t));
}

/** \brief Calculate HostDataBlit parameters from pointer and pitch
 *
 * This is a helper for the trivial HostDataBlit users that don't need to worry
 * about tiling etc.
 */
void
RADEONHostDataParams(ScrnInfoPtr pScrn, uint8_t *dst, uint32_t pitch, int cpp,
		     uint32_t *dstPitchOff, int *x, int *y)
{
    RADEONInfoPtr info = RADEONPTR( pScrn );
    uint32_t dstOffs = dst - (uint8_t*)info->FB + info->fbLocation;

    *dstPitchOff = pitch << 16 | (dstOffs & ~RADEON_BUFFER_ALIGN) >> 10;
    *y = ( dstOffs & RADEON_BUFFER_ALIGN ) / pitch;
    *x = ( ( dstOffs & RADEON_BUFFER_ALIGN ) - ( *y * pitch ) ) / cpp;
}

/* Set up a hostdata blit to transfer data from system memory to the
 * framebuffer. Returns the address where the data can be written to and sets
 * the dstPitch and hpass variables as required.
 */
uint8_t*
RADEONHostDataBlit(
    ScrnInfoPtr pScrn,
    unsigned int cpp,
    unsigned int w,
    uint32_t dstPitchOff,
    uint32_t *bufPitch,
    int x,
    int *y,
    unsigned int *h,
    unsigned int *hpass
){
    RADEONInfoPtr info = RADEONPTR( pScrn );
    uint32_t format, dwords;
    uint8_t *ret;
    RING_LOCALS;

    if ( *h == 0 )
    {
	return NULL;
    }

    switch ( cpp )
    {
    case 4:
	format = RADEON_GMC_DST_32BPP;
	*bufPitch = 4 * w;
	break;
    case 2:
	format = RADEON_GMC_DST_16BPP;
	*bufPitch = 2 * RADEON_ALIGN(w, 2);
	break;
    case 1:
	format = RADEON_GMC_DST_8BPP_CI;
	*bufPitch = RADEON_ALIGN(w, 4);
	break;
    default:
	xf86DrvMsg( pScrn->scrnIndex, X_ERROR,
		    "%s: Unsupported cpp %d!\n", __func__, cpp );
	return NULL;
    }

#if X_BYTE_ORDER == X_BIG_ENDIAN
    /* Swap doesn't work on R300 and later, it's handled during the
     * copy to ind. buffer pass
     */
    if (info->ChipFamily < CHIP_FAMILY_R300) {
        BEGIN_RING(2);
	if (cpp == 2)
	    OUT_RING_REG(RADEON_RBBM_GUICNTL,
			 RADEON_HOST_DATA_SWAP_HDW);
	else if (cpp == 1)
	    OUT_RING_REG(RADEON_RBBM_GUICNTL,
			 RADEON_HOST_DATA_SWAP_32BIT);
	else
	    OUT_RING_REG(RADEON_RBBM_GUICNTL,
			 RADEON_HOST_DATA_SWAP_NONE);
	ADVANCE_RING();
    }
#endif

    /*RADEON_PURGE_CACHE();
      RADEON_WAIT_UNTIL_IDLE();*/

    *hpass = min( *h, ( ( RADEON_BUFFER_SIZE - 10 * 4 ) / *bufPitch ) );
    dwords = *hpass * *bufPitch / 4;

    BEGIN_RING( dwords + 10 );
    OUT_RING( CP_PACKET3( RADEON_CP_PACKET3_CNTL_HOSTDATA_BLT, dwords + 10 - 2 ) );
    OUT_RING( RADEON_GMC_DST_PITCH_OFFSET_CNTL
	    | RADEON_GMC_DST_CLIPPING
	    | RADEON_GMC_BRUSH_NONE
	    | format
	    | RADEON_GMC_SRC_DATATYPE_COLOR
	    | RADEON_ROP3_S
	    | RADEON_DP_SRC_SOURCE_HOST_DATA
	    | RADEON_GMC_CLR_CMP_CNTL_DIS
	    | RADEON_GMC_WR_MSK_DIS );
    OUT_RING( dstPitchOff );
    OUT_RING( (*y << 16) | x );
    OUT_RING( ((*y + *hpass) << 16) | (x + w) );
    OUT_RING( 0xffffffff );
    OUT_RING( 0xffffffff );
    OUT_RING( *y << 16 | x );
    OUT_RING( *hpass << 16 | (*bufPitch / cpp) );
    OUT_RING( dwords );

    ret = ( uint8_t* )&__head[__count];

    __count += dwords;
    ADVANCE_RING();

    *y += *hpass;
    *h -= *hpass;

    return ret;
}

void RADEONCopySwap(uint8_t *dst, uint8_t *src, unsigned int size, int swap)
{
    switch(swap) {
    case RADEON_HOST_DATA_SWAP_HDW:
        {
	    unsigned int *d = (unsigned int *)dst;
	    unsigned int *s = (unsigned int *)src;
	    unsigned int nwords = size >> 2;

	    for (; nwords > 0; --nwords, ++d, ++s)
		*d = ((*s & 0xffff) << 16) | ((*s >> 16) & 0xffff);
	    return;
        }
    case RADEON_HOST_DATA_SWAP_32BIT:
        {
	    unsigned int *d = (unsigned int *)dst;
	    unsigned int *s = (unsigned int *)src;
	    unsigned int nwords = size >> 2;

	    for (; nwords > 0; --nwords, ++d, ++s)
#ifdef __powerpc__
		asm volatile("stwbrx %0,0,%1" : : "r" (*s), "r" (d));
#else
		*d = ((*s >> 24) & 0xff) | ((*s >> 8) & 0xff00)
			| ((*s & 0xff00) << 8) | ((*s & 0xff) << 24);
#endif
	    return;
        }
    case RADEON_HOST_DATA_SWAP_16BIT:
        {
	    unsigned short *d = (unsigned short *)dst;
	    unsigned short *s = (unsigned short *)src;
	    unsigned int nwords = size >> 1;

	    for (; nwords > 0; --nwords, ++d, ++s)
#ifdef __powerpc__
		asm volatile("sthbrx %0,0,%1" : : "r" (*s), "r" (d));
#else
	        *d = (*s >> 8) | (*s << 8);
#endif
	    return;
	}
    }
    if (src != dst)
	memcpy(dst, src, size);
}

/* Copies a single pass worth of data for a hostdata blit set up by
 * RADEONHostDataBlit().
 */
void
RADEONHostDataBlitCopyPass(
    ScrnInfoPtr pScrn,
    unsigned int cpp,
    uint8_t *dst,
    uint8_t *src,
    unsigned int hpass,
    unsigned int dstPitch,
    unsigned int srcPitch
){

#if X_BYTE_ORDER == X_BIG_ENDIAN
    RADEONInfoPtr info = RADEONPTR( pScrn );
#endif

    /* RADEONHostDataBlitCopy can return NULL ! */
    if( (dst==NULL) || (src==NULL)) return;

    if ( dstPitch == srcPitch )
    {
#if X_BYTE_ORDER == X_BIG_ENDIAN
        if (info->ChipFamily >= CHIP_FAMILY_R300) {
	    switch(cpp) {
	    case 1:
		RADEONCopySwap(dst, src, hpass * dstPitch,
			       RADEON_HOST_DATA_SWAP_32BIT);
		return;
	    case 2:
	        RADEONCopySwap(dst, src, hpass * dstPitch,
			       RADEON_HOST_DATA_SWAP_HDW);
		return;
	    }
	}
#endif
	memcpy( dst, src, hpass * dstPitch );
    }
    else
    {
	unsigned int minPitch = min( dstPitch, srcPitch );
	while ( hpass-- )
	{
#if X_BYTE_ORDER == X_BIG_ENDIAN
            if (info->ChipFamily >= CHIP_FAMILY_R300) {
		switch(cpp) {
		case 1:
		    RADEONCopySwap(dst, src, minPitch,
				   RADEON_HOST_DATA_SWAP_32BIT);
		    goto next;
		case 2:
	            RADEONCopySwap(dst, src, minPitch,
				   RADEON_HOST_DATA_SWAP_HDW);
		    goto next;
		}
	    }
#endif
	    memcpy( dst, src, minPitch );
#if X_BYTE_ORDER == X_BIG_ENDIAN
	next:
#endif
	    src += srcPitch;
	    dst += dstPitch;
	}
    }
}

#endif

Bool RADEONAccelInit(ScreenPtr pScreen)
{
    ScrnInfoPtr    pScrn = xf86ScreenToScrn(pScreen);
    RADEONInfoPtr  info  = RADEONPTR(pScrn);

#ifdef USE_EXA
    if (info->useEXA) {
# ifdef XF86DRI
	if (info->directRenderingEnabled) {
#ifdef XF86DRM_MODE
	    if (info->ChipFamily >= CHIP_FAMILY_CEDAR) {
		if (!EVERGREENDrawInit(pScreen))
		    return FALSE;
	    } else
#endif
	      if (info->ChipFamily >= CHIP_FAMILY_R600) {
		if (!R600DrawInit(pScreen))
		    return FALSE;
	    } else {
		if (!RADEONDrawInitCP(pScreen))
		    return FALSE;
	    }
	} else
# endif /* XF86DRI */
	{
	    if (info->ChipFamily >= CHIP_FAMILY_R600)
		return FALSE;
	    else {
		if (!RADEONDrawInitMMIO(pScreen))
		    return FALSE;
	    }
	}
    }
#endif /* USE_EXA */
#ifdef USE_XAA
    if (!info->useEXA) {
	XAAInfoRecPtr  a;

	if (info->ChipFamily >= CHIP_FAMILY_R600)
	    return FALSE;

	if (!(a = info->accel_state->accel = XAACreateInfoRec())) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "XAACreateInfoRec Error\n");
	    return FALSE;
	}

#ifdef XF86DRI
	if (info->directRenderingEnabled)
	    RADEONAccelInitCP(pScreen, a);
	else
#endif /* XF86DRI */
	    RADEONAccelInitMMIO(pScreen, a);

	RADEONEngineInit(pScrn);

	if (!XAAInit(pScreen, a)) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "XAAInit Error\n");
	    return FALSE;
	}
    }
#endif /* USE_XAA */
    return TRUE;
}

void RADEONInit3DEngine(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr info = RADEONPTR (pScrn);

#ifdef XF86DRI
    if (info->directRenderingEnabled) {
	drm_radeon_sarea_t *pSAREAPriv;

	if (!info->kms_enabled) {
	    pSAREAPriv = DRIGetSAREAPrivate(pScrn->pScreen);
	    pSAREAPriv->ctx_owner = DRIGetContext(pScrn->pScreen);
	}
	RADEONInit3DEngineCP(pScrn);
    } else
#endif
	RADEONInit3DEngineMMIO(pScrn);

    info->accel_state->XInited3D = TRUE;
}

#ifdef USE_XAA
#ifdef XF86DRI
Bool
RADEONSetupMemXAA_DRI(ScreenPtr pScreen)
{
    ScrnInfoPtr    pScrn = xf86ScreenToScrn(pScreen);
    RADEONInfoPtr  info  = RADEONPTR(pScrn);
    int            cpp = info->CurrentLayout.pixel_bytes;
    int            depthCpp = (info->dri->depthBits - 8) / 4;
    int            width_bytes = pScrn->displayWidth * cpp;
    int            bufferSize;
    int            depthSize;
    int            l;
    int            scanlines;
    int            texsizerequest;
    BoxRec         MemBox;
    FBAreaPtr      fbarea;

    info->dri->frontOffset = 0;
    info->dri->frontPitch = pScrn->displayWidth;
    info->dri->backPitch = pScrn->displayWidth;

    /* make sure we use 16 line alignment for tiling (8 might be enough).
     * Might need that for non-XF86DRI too?
     */
    if (info->allowColorTiling) {
	bufferSize = RADEON_ALIGN((RADEON_ALIGN(pScrn->virtualY, 16)) * width_bytes,
		      RADEON_GPU_PAGE_SIZE);
    } else {
        bufferSize = RADEON_ALIGN(pScrn->virtualY * width_bytes,
		      RADEON_GPU_PAGE_SIZE);
    }

    /* Due to tiling, the Z buffer pitch must be a multiple of 32 pixels,
     * which is always the case if color tiling is used due to color pitch
     * but not necessarily otherwise, and its height a multiple of 16 lines.
     */
    info->dri->depthPitch = RADEON_ALIGN(pScrn->displayWidth, 32);
    depthSize = RADEON_ALIGN((RADEON_ALIGN(pScrn->virtualY, 16)) * info->dri->depthPitch
		  * depthCpp, RADEON_GPU_PAGE_SIZE);

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "Using %d MB GART aperture\n", info->dri->gartSize);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "Using %d MB for the ring buffer\n", info->dri->ringSize);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "Using %d MB for vertex/indirect buffers\n", info->dri->bufSize);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "Using %d MB for GART textures\n", info->dri->gartTexSize);

    /* Try for front, back, depth, and three framebuffers worth of
     * pixmap cache.  Should be enough for a fullscreen background
     * image plus some leftovers.
     * If the FBTexPercent option was used, try to achieve that percentage instead,
     * but still have at least one pixmap buffer (get problems with xvideo/render
     * otherwise probably), and never reserve more than 3 offscreen buffers as it's
     * probably useless for XAA.
     */
    if (info->dri->textureSize >= 0) {
	texsizerequest = ((int)info->FbMapSize - 2 * bufferSize - depthSize
			 - 2 * width_bytes - 16384 - info->FbSecureSize)
	/* first divide, then multiply or we'll get an overflow (been there...) */
			 / 100 * info->dri->textureSize;
    }
    else {
	texsizerequest = (int)info->FbMapSize / 2;
    }
    info->dri->textureSize = info->FbMapSize - info->FbSecureSize - 5 * bufferSize - depthSize;

    /* If that gives us less than the requested memory, let's
     * be greedy and grab some more.  Sorry, I care more about 3D
     * performance than playing nicely, and you'll get around a full
     * framebuffer's worth of pixmap cache anyway.
     */
    if (info->dri->textureSize < texsizerequest) {
        info->dri->textureSize = info->FbMapSize - 4 * bufferSize - depthSize;
    }
    if (info->dri->textureSize < texsizerequest) {
        info->dri->textureSize = info->FbMapSize - 3 * bufferSize - depthSize;
    }

    /* If there's still no space for textures, try without pixmap cache, but
     * never use the reserved space, the space hw cursor and PCIGART table might
     * use.
     */
    if (info->dri->textureSize < 0) {
	info->dri->textureSize = info->FbMapSize - 2 * bufferSize - depthSize
	                    - 2 * width_bytes - 16384 - info->FbSecureSize;
    }

    /* Check to see if there is more room available after the 8192nd
     * scanline for textures
     */
    /* FIXME: what's this good for? condition is pretty much impossible to meet */
    if ((int)info->FbMapSize - 8192*width_bytes - bufferSize - depthSize
	> info->dri->textureSize) {
	info->dri->textureSize =
		info->FbMapSize - 8192*width_bytes - bufferSize - depthSize;
    }

    /* If backbuffer is disabled, don't allocate memory for it */
    if (info->dri->noBackBuffer) {
	info->dri->textureSize += bufferSize;
    }

    /* RADEON_BUFFER_ALIGN is not sufficient for backbuffer!
       At least for pageflip + color tiling, need to make sure it's 16 scanlines aligned,
       otherwise the copy-from-front-to-back will fail (width_bytes * 16 will also guarantee
       it's still 4kb aligned for tiled case). Need to round up offset (might get into cursor
       area otherwise).
       This might cause some space at the end of the video memory to be unused, since it
       can't be used (?) due to that log_tex_granularity thing???
       Could use different copyscreentoscreen function for the pageflip copies
       (which would use different src and dst offsets) to avoid this. */   
    if (info->allowColorTiling && !info->dri->noBackBuffer) {
	info->dri->textureSize = info->FbMapSize - ((info->FbMapSize - info->dri->textureSize +
			  width_bytes * 16 - 1) / (width_bytes * 16)) * (width_bytes * 16);
    }
    if (info->dri->textureSize > 0) {
	l = RADEONMinBits((info->dri->textureSize-1) / RADEON_NR_TEX_REGIONS);
	if (l < RADEON_LOG_TEX_GRANULARITY)
	    l = RADEON_LOG_TEX_GRANULARITY;
	/* Round the texture size up to the nearest whole number of
	 * texture regions.  Again, be greedy about this, don't
	 * round down.
	 */
	info->dri->log2TexGran = l;
	info->dri->textureSize = (info->dri->textureSize >> l) << l;
    } else {
	info->dri->textureSize = 0;
    }

    /* Set a minimum usable local texture heap size.  This will fit
     * two 256x256x32bpp textures.
     */
    if (info->dri->textureSize < 512 * 1024) {
	info->dri->textureOffset = 0;
	info->dri->textureSize = 0;
    }

    if (info->allowColorTiling && !info->dri->noBackBuffer) {
	info->dri->textureOffset = ((info->FbMapSize - info->dri->textureSize) /
				    (width_bytes * 16)) * (width_bytes * 16);
    }
    else {
	/* Reserve space for textures */
	info->dri->textureOffset = RADEON_ALIGN(info->FbMapSize - info->dri->textureSize,
				     RADEON_GPU_PAGE_SIZE);
    }

    /* Reserve space for the shared depth
     * buffer.
     */
    info->dri->depthOffset = RADEON_ALIGN(info->dri->textureOffset - depthSize,
			       RADEON_GPU_PAGE_SIZE);

    /* Reserve space for the shared back buffer */
    if (info->dri->noBackBuffer) {
       info->dri->backOffset = info->dri->depthOffset;
    } else {
       info->dri->backOffset = RADEON_ALIGN(info->dri->depthOffset - bufferSize,
				 RADEON_GPU_PAGE_SIZE);
    }

    info->dri->backY = info->dri->backOffset / width_bytes;
    info->dri->backX = (info->dri->backOffset - (info->dri->backY * width_bytes)) / cpp;

    scanlines = (info->FbMapSize-info->FbSecureSize) / width_bytes;
    if (scanlines > 8191)
	scanlines = 8191;

    MemBox.x1 = 0;
    MemBox.y1 = 0;
    MemBox.x2 = pScrn->displayWidth;
    MemBox.y2 = scanlines;

    if (!xf86InitFBManager(pScreen, &MemBox)) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "Memory manager initialization to "
		   "(%d,%d) (%d,%d) failed\n",
		   MemBox.x1, MemBox.y1, MemBox.x2, MemBox.y2);
	return FALSE;
    } else {
	int  width, height;

	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "Memory manager initialized to (%d,%d) (%d,%d)\n",
		   MemBox.x1, MemBox.y1, MemBox.x2, MemBox.y2);
	/* why oh why can't we just request modes which are guaranteed to be 16 lines
	   aligned... sigh */
	if ((fbarea = xf86AllocateOffscreenArea(pScreen,
						pScrn->displayWidth,
						info->allowColorTiling ? 
						(RADEON_ALIGN(pScrn->virtualY, 16))
						- pScrn->virtualY + 2 : 2,
						0, NULL, NULL,
						NULL))) {
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		       "Reserved area from (%d,%d) to (%d,%d)\n",
		       fbarea->box.x1, fbarea->box.y1,
		       fbarea->box.x2, fbarea->box.y2);
	} else {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Unable to reserve area\n");
	}

	RADEONDRIAllocatePCIGARTTable(pScreen);

	if (xf86QueryLargestOffscreenArea(pScreen, &width,
					  &height, 0, 0, 0)) {
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		       "Largest offscreen area available: %d x %d\n",
		       width, height);

	    /* Lines in offscreen area needed for depth buffer and
	     * textures
	     */
	    info->dri->depthTexLines = (scanlines
					- info->dri->depthOffset / width_bytes);
	    info->dri->backLines	    = (scanlines
					       - info->dri->backOffset / width_bytes
					       - info->dri->depthTexLines);
	    info->dri->backArea	    = NULL;
	} else {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "Unable to determine largest offscreen area "
		       "available\n");
	    return FALSE;
	}
    }

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "Will use front buffer at offset 0x%x\n",
	       info->dri->frontOffset);

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "Will use back buffer at offset 0x%x\n",
	       info->dri->backOffset);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "Will use depth buffer at offset 0x%x\n",
	       info->dri->depthOffset);
    if (info->cardType==CARD_PCIE)
    	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	           "Will use %d kb for PCI GART table at offset 0x%x\n",
		   info->dri->pciGartSize/1024, (unsigned)info->dri->pciGartOffset);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "Will use %d kb for textures at offset 0x%x\n",
	       info->dri->textureSize/1024, info->dri->textureOffset);

    info->dri->frontPitchOffset = (((info->dri->frontPitch * cpp / 64) << 22) |
				   ((info->dri->frontOffset + info->fbLocation) >> 10));

    info->dri->backPitchOffset = (((info->dri->backPitch * cpp / 64) << 22) |
				  ((info->dri->backOffset + info->fbLocation) >> 10));

    info->dri->depthPitchOffset = (((info->dri->depthPitch * depthCpp / 64) << 22) |
				   ((info->dri->depthOffset + info->fbLocation) >> 10));
    return TRUE;
}
#endif /* XF86DRI */

Bool
RADEONSetupMemXAA(ScreenPtr pScreen)
{
    ScrnInfoPtr    pScrn = xf86ScreenToScrn(pScreen);
    RADEONInfoPtr  info  = RADEONPTR(pScrn);
    BoxRec         MemBox;
    int            y2;

    int width_bytes = pScrn->displayWidth * info->CurrentLayout.pixel_bytes;

    MemBox.x1 = 0;
    MemBox.y1 = 0;
    MemBox.x2 = pScrn->displayWidth;
    y2 = info->FbMapSize / width_bytes;
    if (y2 >= 32768)
	y2 = 32767; /* because MemBox.y2 is signed short */
    MemBox.y2 = y2;
    
    /* The acceleration engine uses 14 bit
     * signed coordinates, so we can't have any
     * drawable caches beyond this region.
     */
    if (MemBox.y2 > 8191)
	MemBox.y2 = 8191;

    if (!xf86InitFBManager(pScreen, &MemBox)) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "Memory manager initialization to "
		   "(%d,%d) (%d,%d) failed\n",
		   MemBox.x1, MemBox.y1, MemBox.x2, MemBox.y2);
	return FALSE;
    } else {
	int       width, height;
	FBAreaPtr fbarea;

	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "Memory manager initialized to (%d,%d) (%d,%d)\n",
		   MemBox.x1, MemBox.y1, MemBox.x2, MemBox.y2);
	if ((fbarea = xf86AllocateOffscreenArea(pScreen,
						pScrn->displayWidth,
						info->allowColorTiling ? 
						(RADEON_ALIGN(pScrn->virtualY, 16))
						- pScrn->virtualY + 2 : 2,
						0, NULL, NULL,
						NULL))) {
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		       "Reserved area from (%d,%d) to (%d,%d)\n",
		       fbarea->box.x1, fbarea->box.y1,
		       fbarea->box.x2, fbarea->box.y2);
	} else {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Unable to reserve area\n");
	}
	if (xf86QueryLargestOffscreenArea(pScreen, &width, &height,
					      0, 0, 0)) {
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		       "Largest offscreen area available: %d x %d\n",
		       width, height);
	}
	return TRUE;
    }    
}
#endif /* USE_XAA */
