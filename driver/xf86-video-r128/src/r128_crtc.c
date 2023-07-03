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

#include <string.h>
#include <stdio.h>

#include "xf86.h"
#include "xf86Modes.h"

#ifdef HAVE_XEXTPROTO_71
#include "X11/extensions/dpmsconst.h"
#else
#define DPMS_SERVER
#include "X11/extensions/dpms.h"
#endif

#include "r128.h"
#include "r128_probe.h"
#include "r128_reg.h"


#ifndef MAX
#define MAX(a,b) ((a)>(b)?(a):(b))
#endif


/* Define CRTC registers for requested video mode. */
Bool R128InitCrtcRegisters(xf86CrtcPtr crtc, R128SavePtr save, DisplayModePtr mode)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    R128InfoPtr info  = R128PTR(pScrn);
    xf86OutputPtr output = R128FirstOutput(crtc);
    R128OutputPrivatePtr r128_output = output->driver_private;

    int    format;
    int    hsync_start;
    int    hsync_wid;
    int    hsync_fudge;
    int    vsync_wid;
    int    hsync_fudge_default[] = { 0x00, 0x12, 0x09, 0x09, 0x06, 0x05 };
    int    hsync_fudge_fp[]      = { 0x12, 0x11, 0x09, 0x09, 0x05, 0x05 };
//   int    hsync_fudge_fp_crt[]  = { 0x12, 0x10, 0x08, 0x08, 0x04, 0x04 };

    switch (info->CurrentLayout.pixel_code) {
    case 4:  format = 1; break;
    case 8:  format = 2; break;
    case 15: format = 3; break;      /*  555 */
    case 16: format = 4; break;      /*  565 */
    case 24: format = 5; break;      /*  RGB */
    case 32: format = 6; break;      /* xRGB */
    default:
    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
           "Unsupported pixel depth (%d)\n",
           info->CurrentLayout.bitsPerPixel);
    return FALSE;
    }

    if (r128_output->MonType == MT_LCD || r128_output->MonType == MT_DFP)
    hsync_fudge = hsync_fudge_fp[format-1];
    else
        hsync_fudge = hsync_fudge_default[format-1];

    save->crtc_gen_cntl = (R128_CRTC_EXT_DISP_EN
              | R128_CRTC_EN
              | (format << 8)
              | ((mode->Flags & V_DBLSCAN)
                 ? R128_CRTC_DBL_SCAN_EN
                 : 0)
              | ((mode->Flags & V_INTERLACE)
                 ? R128_CRTC_INTERLACE_EN
                 : 0)
              | ((mode->Flags & V_CSYNC)
                 ? R128_CRTC_CSYNC_EN
                 : 0));

    if (r128_output->MonType == MT_LCD || r128_output->MonType == MT_DFP)
        save->crtc_gen_cntl &= ~(R128_CRTC_DBL_SCAN_EN | R128_CRTC_INTERLACE_EN);

    save->crtc_ext_cntl |= R128_VGA_ATI_LINEAR | R128_XCRT_CNT_EN;

    save->crtc_h_total_disp = ((((mode->CrtcHTotal / 8) - 1) & 0xffff)
                  | (((mode->CrtcHDisplay / 8) - 1) << 16));

    hsync_wid = (mode->CrtcHSyncEnd - mode->CrtcHSyncStart) / 8;
    if (!hsync_wid)       hsync_wid = 1;
    if (hsync_wid > 0x3f) hsync_wid = 0x3f;

    hsync_start = mode->CrtcHSyncStart - 8 + hsync_fudge;

    save->crtc_h_sync_strt_wid = ((hsync_start & 0xfff)
                 | (hsync_wid << 16)
                 | ((mode->Flags & V_NHSYNC)
                    ? R128_CRTC_H_SYNC_POL
                    : 0));

#if 1
                /* This works for double scan mode. */
    save->crtc_v_total_disp = (((mode->CrtcVTotal - 1) & 0xffff)
                  | ((mode->CrtcVDisplay - 1) << 16));
#else
                /* This is what cce/nbmode.c example code
                   does -- is this correct? */
    save->crtc_v_total_disp = (((mode->CrtcVTotal - 1) & 0xffff)
                  | ((mode->CrtcVDisplay
                  * ((mode->Flags & V_DBLSCAN) ? 2 : 1) - 1)
                 << 16));
#endif

    vsync_wid = mode->CrtcVSyncEnd - mode->CrtcVSyncStart;
    if (!vsync_wid)       vsync_wid = 1;
    if (vsync_wid > 0x1f) vsync_wid = 0x1f;

    save->crtc_v_sync_strt_wid = (((mode->CrtcVSyncStart - 1) & 0xfff)
                 | (vsync_wid << 16)
                 | ((mode->Flags & V_NVSYNC)
                    ? R128_CRTC_V_SYNC_POL
                    : 0));
    save->crtc_pitch       = info->CurrentLayout.displayWidth / 8;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Pitch = %d bytes (virtualX = %d, "
                        "displayWidth = %d)\n",
                        save->crtc_pitch, pScrn->virtualX,
                        info->CurrentLayout.displayWidth));

#if X_BYTE_ORDER == X_BIG_ENDIAN
    /* Change the endianness of the aperture */
    switch (info->CurrentLayout.pixel_code) {
    case 15:
    case 16: save->config_cntl |= APER_0_BIG_ENDIAN_16BPP_SWAP; break;
    case 32: save->config_cntl |= APER_0_BIG_ENDIAN_32BPP_SWAP; break;
    default: break;
    }
#endif

    return TRUE;
}

/* Define CRTC2 registers for requested video mode. */
Bool R128InitCrtc2Registers(xf86CrtcPtr crtc, R128SavePtr save, DisplayModePtr mode)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    R128InfoPtr info  = R128PTR(pScrn);

    int    format;
    int    hsync_start;
    int    hsync_wid;
    int    hsync_fudge;
    int    vsync_wid;
    int    hsync_fudge_default[] = { 0x00, 0x12, 0x09, 0x09, 0x06, 0x05 };

    switch (info->CurrentLayout.pixel_code) {
    case 4:  format = 1; break;
    case 8:  format = 2; break;
    case 15: format = 3; break;      /*  555 */
    case 16: format = 4; break;      /*  565 */
    case 24: format = 5; break;      /*  RGB */
    case 32: format = 6; break;      /* xRGB */
    default:
    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
           "Unsupported pixel depth (%d)\n", info->CurrentLayout.bitsPerPixel);
    return FALSE;
    }

    hsync_fudge = hsync_fudge_default[format-1];

    save->crtc2_gen_cntl = (R128_CRTC2_EN
              | (format << 8)
              | ((mode->Flags & V_DBLSCAN)
                 ? R128_CRTC2_DBL_SCAN_EN
                 : 0));
/*
    save->crtc2_gen_cntl &= ~R128_CRTC_EXT_DISP_EN;
    save->crtc2_gen_cntl |= (1 << 21);
*/
    save->crtc2_h_total_disp = ((((mode->CrtcHTotal / 8) - 1) & 0xffff)
                  | (((mode->CrtcHDisplay / 8) - 1) << 16));

    hsync_wid = (mode->CrtcHSyncEnd - mode->CrtcHSyncStart) / 8;
    if (!hsync_wid)       hsync_wid = 1;
    if (hsync_wid > 0x3f) hsync_wid = 0x3f;

    hsync_start = mode->CrtcHSyncStart - 8 + hsync_fudge;

    save->crtc2_h_sync_strt_wid = ((hsync_start & 0xfff)
                 | (hsync_wid << 16)
                 | ((mode->Flags & V_NHSYNC)
                    ? R128_CRTC2_H_SYNC_POL
                    : 0));

#if 1
                /* This works for double scan mode. */
    save->crtc2_v_total_disp = (((mode->CrtcVTotal - 1) & 0xffff)
                  | ((mode->CrtcVDisplay - 1) << 16));
#else
                /* This is what cce/nbmode.c example code
                   does -- is this correct? */
    save->crtc2_v_total_disp = (((mode->CrtcVTotal - 1) & 0xffff)
                  | ((mode->CrtcVDisplay
                  * ((mode->Flags & V_DBLSCAN) ? 2 : 1) - 1)
                 << 16));
#endif

    vsync_wid = mode->CrtcVSyncEnd - mode->CrtcVSyncStart;
    if (!vsync_wid)       vsync_wid = 1;
    if (vsync_wid > 0x1f) vsync_wid = 0x1f;

    save->crtc2_v_sync_strt_wid = (((mode->CrtcVSyncStart - 1) & 0xfff)
                 | (vsync_wid << 16)
                 | ((mode->Flags & V_NVSYNC)
                    ? R128_CRTC2_V_SYNC_POL
                    : 0));
    save->crtc2_pitch       = info->CurrentLayout.displayWidth / 8;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Pitch = %d bytes (virtualX = %d, "
                        "displayWidth = %d)\n",
                        save->crtc2_pitch, pScrn->virtualX,
                        info->CurrentLayout.displayWidth));
    return TRUE;
}

/* Write CRTC registers. */
void R128RestoreCrtcRegisters(ScrnInfoPtr pScrn, R128SavePtr restore)
{
    R128InfoPtr   info      = R128PTR(pScrn);
    unsigned char *R128MMIO = info->MMIO;

    OUTREG(R128_CRTC_GEN_CNTL,        restore->crtc_gen_cntl);

    OUTREGP(R128_CRTC_EXT_CNTL, restore->crtc_ext_cntl,
        R128_CRTC_VSYNC_DIS | R128_CRTC_HSYNC_DIS | R128_CRTC_DISPLAY_DIS);

    OUTREG(R128_CRTC_H_TOTAL_DISP,    restore->crtc_h_total_disp);
    OUTREG(R128_CRTC_H_SYNC_STRT_WID, restore->crtc_h_sync_strt_wid);
    OUTREG(R128_CRTC_V_TOTAL_DISP,    restore->crtc_v_total_disp);
    OUTREG(R128_CRTC_V_SYNC_STRT_WID, restore->crtc_v_sync_strt_wid);
    OUTREG(R128_CRTC_OFFSET,          restore->crtc_offset);
    OUTREG(R128_CRTC_OFFSET_CNTL,     restore->crtc_offset_cntl);
    OUTREG(R128_CRTC_PITCH,           restore->crtc_pitch);
}

/* Write CRTC2 registers. */
void R128RestoreCrtc2Registers(ScrnInfoPtr pScrn, R128SavePtr restore)
{
    R128InfoPtr info        = R128PTR(pScrn);
    unsigned char *R128MMIO = info->MMIO;

    OUTREGP(R128_CRTC2_GEN_CNTL, restore->crtc2_gen_cntl,
        R128_CRTC2_DISP_DIS);

    OUTREG(R128_CRTC2_H_TOTAL_DISP,    restore->crtc2_h_total_disp);
    OUTREG(R128_CRTC2_H_SYNC_STRT_WID, restore->crtc2_h_sync_strt_wid);
    OUTREG(R128_CRTC2_V_TOTAL_DISP,    restore->crtc2_v_total_disp);
    OUTREG(R128_CRTC2_V_SYNC_STRT_WID, restore->crtc2_v_sync_strt_wid);
    OUTREG(R128_CRTC2_OFFSET,          restore->crtc2_offset);
    OUTREG(R128_CRTC2_OFFSET_CNTL,     restore->crtc2_offset_cntl);
    OUTREG(R128_CRTC2_PITCH,           restore->crtc2_pitch);
}

static Bool R128InitCrtcBase(xf86CrtcPtr crtc, R128SavePtr save, int x, int y)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    R128InfoPtr info  = R128PTR(pScrn);
    int offset = y * info->CurrentLayout.displayWidth + x;
    int Base = pScrn->fbOffset;

    switch (info->CurrentLayout.pixel_code) {
    case 15:
    case 16: offset *= 2; break;
    case 24: offset *= 3; break;
    case 32: offset *= 4; break;
    }
    Base += offset;

    if (crtc->rotatedData != NULL)
        Base = pScrn->fbOffset + (char *)crtc->rotatedData - (char *)info->FB;

    Base &= ~7;                 /* 3 lower bits are always 0 */
    if (info->CurrentLayout.pixel_code == 24)
    Base += 8 * (Base % 3); /* Must be multiple of 8 and 3 */

    save->crtc_offset = Base;
    save->crtc_offset_cntl = 0;

    return TRUE;
}

static Bool R128InitCrtc2Base(xf86CrtcPtr crtc, R128SavePtr save, int x, int y)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    R128InfoPtr info  = R128PTR(pScrn);
    int offset = y * info->CurrentLayout.displayWidth + x;
    int Base = pScrn->fbOffset;

    switch (info->CurrentLayout.pixel_code) {
    case 15:
    case 16: offset *= 2; break;
    case 24: offset *= 3; break;
    case 32: offset *= 4; break;
    }
    Base += offset;

    if (crtc->rotatedData != NULL)
        Base = pScrn->fbOffset + (char *)crtc->rotatedData - (char *)info->FB;

    Base &= ~7;                 /* 3 lower bits are always 0 */
    if (info->CurrentLayout.pixel_code == 24)
    Base += 8 * (Base % 3); /* Must be multiple of 8 and 3 */

    save->crtc2_offset = Base;
    save->crtc2_offset_cntl = 0;

    return TRUE;
}

/* Define PLL registers for requested video mode. */
static void R128InitPLLRegisters(xf86CrtcPtr crtc, R128SavePtr save,
                R128PLLPtr pll, double dot_clock)
{
#if R128_DEBUG
    ScrnInfoPtr pScrn  = crtc->scrn;
#endif
    unsigned long freq = dot_clock * 100;
    struct {
    int divider;
    int bitvalue;
    } *post_div,
      post_divs[]   = {
                /* From RAGE 128 VR/RAGE 128 GL Register
                   Reference Manual (Technical Reference
                   Manual P/N RRG-G04100-C Rev. 0.04), page
                   3-17 (PLL_DIV_[3:0]).  */
    {  1, 0 },              /* VCLK_SRC                 */
    {  2, 1 },              /* VCLK_SRC/2               */
    {  4, 2 },              /* VCLK_SRC/4               */
    {  8, 3 },              /* VCLK_SRC/8               */

    {  3, 4 },              /* VCLK_SRC/3               */
                /* bitvalue = 5 is reserved */
    {  6, 6 },              /* VCLK_SRC/6               */
    { 12, 7 },              /* VCLK_SRC/12              */
    {  0, 0 }
    };

    if (freq > pll->max_pll_freq)      freq = pll->max_pll_freq;
    if (freq * 12 < pll->min_pll_freq) freq = pll->min_pll_freq / 12;

    for (post_div = &post_divs[0]; post_div->divider; ++post_div) {
    save->pll_output_freq = post_div->divider * freq;
    if (save->pll_output_freq >= pll->min_pll_freq
        && save->pll_output_freq <= pll->max_pll_freq) break;
    }

    save->dot_clock_freq = freq;
    save->feedback_div   = R128Div(pll->reference_div * save->pll_output_freq,
                   pll->reference_freq);
    save->post_div       = post_div->divider;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "dc=%d, of=%d, fd=%d, pd=%d\n",
                        save->dot_clock_freq,
                        save->pll_output_freq,
                        save->feedback_div,
                        save->post_div));

    save->ppll_ref_div   = pll->reference_div;
    save->ppll_div_3     = (save->feedback_div | (post_div->bitvalue << 16));
    save->htotal_cntl    = 0;

}

/* Define PLL2 registers for requested video mode. */
void R128InitPLL2Registers(xf86CrtcPtr crtc, R128SavePtr save,
                   R128PLLPtr pll, double dot_clock)
{
#if R128_DEBUG
    ScrnInfoPtr pScrn  = crtc->scrn;
#endif
    unsigned long freq = dot_clock * 100;
    struct {
    int divider;
    int bitvalue;
    } *post_div,
      post_divs[]   = {
                /* From RAGE 128 VR/RAGE 128 GL Register
                   Reference Manual (Technical Reference
                   Manual P/N RRG-G04100-C Rev. 0.04), page
                   3-17 (PLL_DIV_[3:0]).  */
    {  1, 0 },              /* VCLK_SRC                 */
    {  2, 1 },              /* VCLK_SRC/2               */
    {  4, 2 },              /* VCLK_SRC/4               */
    {  8, 3 },              /* VCLK_SRC/8               */

    {  3, 4 },              /* VCLK_SRC/3               */
                /* bitvalue = 5 is reserved */
    {  6, 6 },              /* VCLK_SRC/6               */
    { 12, 7 },              /* VCLK_SRC/12              */
    {  0, 0 }
    };

    if (freq > pll->max_pll_freq)      freq = pll->max_pll_freq;
    if (freq * 12 < pll->min_pll_freq) freq = pll->min_pll_freq / 12;

    for (post_div = &post_divs[0]; post_div->divider; ++post_div) {
    save->pll_output_freq_2 = post_div->divider * freq;
    if (save->pll_output_freq_2 >= pll->min_pll_freq
        && save->pll_output_freq_2 <= pll->max_pll_freq) break;
    }

    save->dot_clock_freq_2 = freq;
    save->feedback_div_2   = R128Div(pll->reference_div
                     * save->pll_output_freq_2,
                     pll->reference_freq);
    save->post_div_2       = post_div->divider;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "dc=%d, of=%d, fd=%d, pd=%d\n",
                        save->dot_clock_freq_2,
                        save->pll_output_freq_2,
                        save->feedback_div_2,
                        save->post_div_2));

    save->p2pll_ref_div   = pll->reference_div;
    save->p2pll_div_0    = (save->feedback_div_2 | (post_div->bitvalue<<16));
    save->htotal_cntl2    = 0;
}

static void R128PLLWaitForReadUpdateComplete(ScrnInfoPtr pScrn)
{
    while (INPLL(pScrn, R128_PPLL_REF_DIV) & R128_PPLL_ATOMIC_UPDATE_R);
}

static void R128PLLWriteUpdate(ScrnInfoPtr pScrn)
{
    R128InfoPtr   info      = R128PTR(pScrn);
    unsigned char *R128MMIO = info->MMIO;

    while (INPLL(pScrn, R128_PPLL_REF_DIV) & R128_PPLL_ATOMIC_UPDATE_R);

    OUTPLLP(pScrn, R128_PPLL_REF_DIV, R128_PPLL_ATOMIC_UPDATE_W,
        ~R128_PPLL_ATOMIC_UPDATE_W);

}

static void R128PLL2WaitForReadUpdateComplete(ScrnInfoPtr pScrn)
{
    while (INPLL(pScrn, R128_P2PLL_REF_DIV) & R128_P2PLL_ATOMIC_UPDATE_R);
}

static void R128PLL2WriteUpdate(ScrnInfoPtr pScrn)
{
    R128InfoPtr  info       = R128PTR(pScrn);
    unsigned char *R128MMIO = info->MMIO;

    while (INPLL(pScrn, R128_P2PLL_REF_DIV) & R128_P2PLL_ATOMIC_UPDATE_R);

    OUTPLLP(pScrn, R128_P2PLL_REF_DIV,
        R128_P2PLL_ATOMIC_UPDATE_W,
        ~(R128_P2PLL_ATOMIC_UPDATE_W));
}

/* Write PLL registers. */
void R128RestorePLLRegisters(ScrnInfoPtr pScrn, R128SavePtr restore)
{
    R128InfoPtr   info      = R128PTR(pScrn);
    unsigned char *R128MMIO = info->MMIO;


    OUTPLLP(pScrn, R128_VCLK_ECP_CNTL,
        R128_VCLK_SRC_SEL_CPUCLK,
        ~(R128_VCLK_SRC_SEL_MASK));

    OUTPLLP(pScrn,
        R128_PPLL_CNTL,
        R128_PPLL_RESET
        | R128_PPLL_ATOMIC_UPDATE_EN
        | R128_PPLL_VGA_ATOMIC_UPDATE_EN,
        ~(R128_PPLL_RESET
          | R128_PPLL_ATOMIC_UPDATE_EN
          | R128_PPLL_VGA_ATOMIC_UPDATE_EN));

    OUTREGP(R128_CLOCK_CNTL_INDEX, R128_PLL_DIV_SEL, ~(R128_PLL_DIV_SEL));

/*        R128PLLWaitForReadUpdateComplete(pScrn);*/
    OUTPLLP(pScrn, R128_PPLL_REF_DIV,
        restore->ppll_ref_div, ~R128_PPLL_REF_DIV_MASK);
/*        R128PLLWriteUpdate(pScrn);

        R128PLLWaitForReadUpdateComplete(pScrn);*/
    OUTPLLP(pScrn, R128_PPLL_DIV_3,
        restore->ppll_div_3, ~R128_PPLL_FB3_DIV_MASK);
/*    R128PLLWriteUpdate(pScrn);*/
    OUTPLLP(pScrn, R128_PPLL_DIV_3,
        restore->ppll_div_3, ~R128_PPLL_POST3_DIV_MASK);

    R128PLLWriteUpdate(pScrn);
    R128PLLWaitForReadUpdateComplete(pScrn);

    OUTPLLP(pScrn, R128_PPLL_DIV_0,
        restore->ppll_div_0, ~R128_PPLL_FB0_DIV_MASK);
/*    R128PLLWriteUpdate(pScrn);*/
    OUTPLLP(pScrn, R128_PPLL_DIV_0,
        restore->ppll_div_0, ~R128_PPLL_POST0_DIV_MASK);

    R128PLLWriteUpdate(pScrn);
    R128PLLWaitForReadUpdateComplete(pScrn);

    OUTPLL(R128_HTOTAL_CNTL, restore->htotal_cntl);
/*    R128PLLWriteUpdate(pScrn);*/

    OUTPLLP(pScrn, R128_PPLL_CNTL, 0, ~(R128_PPLL_RESET
                    | R128_PPLL_SLEEP
                    | R128_PPLL_ATOMIC_UPDATE_EN
                    | R128_PPLL_VGA_ATOMIC_UPDATE_EN));

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Wrote: 0x%08x 0x%08x 0x%08x (0x%08x)\n",
                        restore->ppll_ref_div,
                        restore->ppll_div_3,
                        restore->htotal_cntl,
                        INPLL(pScrn, R128_PPLL_CNTL)));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Wrote: rd=%d, fd=%d, pd=%d\n",
                        restore->ppll_ref_div & R128_PPLL_REF_DIV_MASK,
                        restore->ppll_div_3 & R128_PPLL_FB3_DIV_MASK,
                        (restore->ppll_div_3 &
                                R128_PPLL_POST3_DIV_MASK) >> 16));

    usleep(5000); /* let the clock lock */

    OUTPLLP(pScrn, R128_VCLK_ECP_CNTL,
        R128_VCLK_SRC_SEL_PPLLCLK,
        ~(R128_VCLK_SRC_SEL_MASK));

}

/* Write PLL2 registers. */
void R128RestorePLL2Registers(ScrnInfoPtr pScrn, R128SavePtr restore)
{
    R128InfoPtr info        = R128PTR(pScrn);
    unsigned char *R128MMIO = info->MMIO;

    OUTPLLP(pScrn, R128_V2CLK_VCLKTV_CNTL,
        R128_V2CLK_SRC_SEL_CPUCLK,
        ~R128_V2CLK_SRC_SEL_MASK);

    OUTPLLP(pScrn,
        R128_P2PLL_CNTL,
        R128_P2PLL_RESET
        | R128_P2PLL_ATOMIC_UPDATE_EN
        | R128_P2PLL_VGA_ATOMIC_UPDATE_EN,
        ~(R128_P2PLL_RESET
          | R128_P2PLL_ATOMIC_UPDATE_EN
          | R128_P2PLL_VGA_ATOMIC_UPDATE_EN));

#if 1
    OUTREGP(R128_CLOCK_CNTL_INDEX, 0, R128_PLL2_DIV_SEL_MASK);
#endif

        /*R128PLL2WaitForReadUpdateComplete(pScrn);*/

    OUTPLLP(pScrn, R128_P2PLL_REF_DIV, restore->p2pll_ref_div, ~R128_P2PLL_REF_DIV_MASK);

/*        R128PLL2WriteUpdate(pScrn);
    R128PLL2WaitForReadUpdateComplete(pScrn);*/

    OUTPLLP(pScrn, R128_P2PLL_DIV_0,
            restore->p2pll_div_0, ~R128_P2PLL_FB0_DIV_MASK);

/*    R128PLL2WriteUpdate(pScrn);
    R128PLL2WaitForReadUpdateComplete(pScrn);*/

    OUTPLLP(pScrn, R128_P2PLL_DIV_0,
            restore->p2pll_div_0, ~R128_P2PLL_POST0_DIV_MASK);

    R128PLL2WriteUpdate(pScrn);
    R128PLL2WaitForReadUpdateComplete(pScrn);

    OUTPLL(R128_HTOTAL2_CNTL, restore->htotal_cntl2);

/*        R128PLL2WriteUpdate(pScrn);*/

    OUTPLLP(pScrn, R128_P2PLL_CNTL, 0, ~(R128_P2PLL_RESET
                    | R128_P2PLL_SLEEP
                    | R128_P2PLL_ATOMIC_UPDATE_EN
                    | R128_P2PLL_VGA_ATOMIC_UPDATE_EN));

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Wrote: 0x%08x 0x%08x 0x%08x (0x%08x)\n",
                        restore->p2pll_ref_div,
                        restore->p2pll_div_0,
                        restore->htotal_cntl2,
                        INPLL(pScrn, R128_P2PLL_CNTL)));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Wrote: rd=%d, fd=%d, pd=%d\n",
                        restore->p2pll_ref_div & R128_P2PLL_REF_DIV_MASK,
                        restore->p2pll_div_0 & R128_P2PLL_FB0_DIV_MASK,
                        (restore->p2pll_div_0 &
                                R128_P2PLL_POST0_DIV_MASK) >>16));

    usleep(5000); /* Let the clock to lock */

    OUTPLLP(pScrn, R128_V2CLK_VCLKTV_CNTL,
        R128_V2CLK_SRC_SEL_P2PLLCLK,
        ~R128_V2CLK_SRC_SEL_MASK);

}

/* Define DDA registers for requested video mode. */
Bool R128InitDDARegisters(xf86CrtcPtr crtc, R128SavePtr save,
                 R128PLLPtr pll, DisplayModePtr mode)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    R128InfoPtr info  = R128PTR(pScrn);
    xf86OutputPtr output = R128FirstOutput(crtc);
    R128OutputPrivatePtr r128_output = output->driver_private;

    int         DisplayFifoWidth = 128;
    int         DisplayFifoDepth = 32;
    int         XclkFreq;
    int         VclkFreq;
    int         XclksPerTransfer;
    int         XclksPerTransferPrecise;
    int         UseablePrecision;
    int         Roff;
    int         Ron;

    XclkFreq = pll->xclk;

    VclkFreq = R128Div(pll->reference_freq * save->feedback_div,
               pll->reference_div * save->post_div);

    if (info->isDFP && !info->isPro2 && r128_output->PanelXRes > 0) {
        if (r128_output->PanelXRes != mode->CrtcHDisplay)
            VclkFreq = (VclkFreq * mode->CrtcHDisplay) / r128_output->PanelXRes;
    }

    XclksPerTransfer = R128Div(XclkFreq * DisplayFifoWidth,
                   VclkFreq * (info->CurrentLayout.pixel_bytes * 8));

    UseablePrecision = R128MinBits(XclksPerTransfer) + 1;

    XclksPerTransferPrecise = R128Div((XclkFreq * DisplayFifoWidth)
                      << (11 - UseablePrecision),
                      VclkFreq * (info->CurrentLayout.pixel_bytes * 8));

    Roff  = XclksPerTransferPrecise * (DisplayFifoDepth - 4);

    Ron   = (4 * info->ram->MB
         + 3 * MAX(info->ram->Trcd - 2, 0)
         + 2 * info->ram->Trp
         + info->ram->Twr
         + info->ram->CL
         + info->ram->Tr2w
         + XclksPerTransfer) << (11 - UseablePrecision);

    if (Ron + info->ram->Rloop >= Roff) {
    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
           "(Ron = %d) + (Rloop = %d) >= (Roff = %d)\n",
           Ron, info->ram->Rloop, Roff);
    return FALSE;
    }

    save->dda_config = (XclksPerTransferPrecise
            | (UseablePrecision << 16)
            | (info->ram->Rloop << 20));

    save->dda_on_off = (Ron << 16) | Roff;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "XclkFreq = %d; VclkFreq = %d; "
                        "per = %d, %d (usable = %d)\n",
                        XclkFreq,
                        VclkFreq,
                        XclksPerTransfer,
                        XclksPerTransferPrecise,
                        UseablePrecision));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Roff = %d, Ron = %d, Rloop = %d\n",
                        Roff, Ron, info->ram->Rloop));

    return TRUE;
}

/* Define DDA2 registers for requested video mode. */
Bool R128InitDDA2Registers(xf86CrtcPtr crtc, R128SavePtr save,
                 R128PLLPtr pll, DisplayModePtr mode)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    R128InfoPtr info  = R128PTR(pScrn);
    xf86OutputPtr output = R128FirstOutput(crtc);
    R128OutputPrivatePtr r128_output = output->driver_private;

    int         DisplayFifoWidth = 128;
    int         DisplayFifoDepth = 32;
    int         XclkFreq;
    int         VclkFreq;
    int         XclksPerTransfer;
    int         XclksPerTransferPrecise;
    int         UseablePrecision;
    int         Roff;
    int         Ron;

    XclkFreq = pll->xclk;

    VclkFreq = R128Div(pll->reference_freq * save->feedback_div_2,
               pll->reference_div * save->post_div_2);

    if (info->isDFP && !info->isPro2 && r128_output->PanelXRes > 0) {
        if (r128_output->PanelXRes != mode->CrtcHDisplay)
            VclkFreq = (VclkFreq * mode->CrtcHDisplay) / r128_output->PanelXRes;
    }

    XclksPerTransfer = R128Div(XclkFreq * DisplayFifoWidth,
                   VclkFreq * (info->CurrentLayout.pixel_bytes * 8));

    UseablePrecision = R128MinBits(XclksPerTransfer) + 1;

    XclksPerTransferPrecise = R128Div((XclkFreq * DisplayFifoWidth)
                      << (11 - UseablePrecision),
                      VclkFreq * (info->CurrentLayout.pixel_bytes * 8));

    Roff  = XclksPerTransferPrecise * (DisplayFifoDepth - 4);

    Ron   = (4 * info->ram->MB
         + 3 * MAX(info->ram->Trcd - 2, 0)
         + 2 * info->ram->Trp
         + info->ram->Twr
         + info->ram->CL
         + info->ram->Tr2w
         + XclksPerTransfer) << (11 - UseablePrecision);


    if (Ron + info->ram->Rloop >= Roff) {
    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
           "(Ron = %d) + (Rloop = %d) >= (Roff = %d)\n",
           Ron, info->ram->Rloop, Roff);
    return FALSE;
    }

    save->dda2_config = (XclksPerTransferPrecise
            | (UseablePrecision << 16)
            | (info->ram->Rloop << 20));

    /*save->dda2_on_off = (Ron << 16) | Roff;*/
    /* shift most be 18 otherwise there's corruption on crtc2 */
    save->dda2_on_off = (Ron << 18) | Roff;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "XclkFreq = %d; VclkFreq = %d; "
                        "per = %d, %d (usable = %d)\n",
                        XclkFreq,
                        VclkFreq,
                        XclksPerTransfer,
                        XclksPerTransferPrecise,
                        UseablePrecision));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Roff = %d, Ron = %d, Rloop = %d\n",
                        Roff, Ron, info->ram->Rloop));

    return TRUE;
}

static void r128_crtc_load_lut(xf86CrtcPtr crtc);

static void r128_crtc_dpms(xf86CrtcPtr crtc, int mode)
{
    int mask;
    ScrnInfoPtr pScrn = crtc->scrn;
    R128InfoPtr info = R128PTR(pScrn);
    unsigned char *R128MMIO = info->MMIO;
    R128CrtcPrivatePtr r128_crtc = crtc->driver_private;

    /* XXX: The HSYNC and VSYNC bits for CRTC2 don't exist on the r128? */
    mask = r128_crtc->crtc_id ? R128_CRTC2_DISP_DIS : (R128_CRTC_DISPLAY_DIS | R128_CRTC_HSYNC_DIS | R128_CRTC_VSYNC_DIS);

    switch (mode) {
    case DPMSModeOn:
        if (r128_crtc->crtc_id) {
            OUTREGP(R128_CRTC2_GEN_CNTL, 0, ~mask);
        } else {
            OUTREGP(R128_CRTC_EXT_CNTL, 0, ~mask);
        }
        break;
    case DPMSModeStandby:
        if (r128_crtc->crtc_id) {
            OUTREGP(R128_CRTC2_GEN_CNTL, R128_CRTC2_DISP_DIS, ~mask);
        } else {
            OUTREGP(R128_CRTC_EXT_CNTL, (R128_CRTC_DISPLAY_DIS | R128_CRTC_HSYNC_DIS), ~mask);
        }
        break;
    case DPMSModeSuspend:
        if (r128_crtc->crtc_id) {
            OUTREGP(R128_CRTC2_GEN_CNTL, R128_CRTC2_DISP_DIS, ~mask);
        } else {
            OUTREGP(R128_CRTC_EXT_CNTL, (R128_CRTC_DISPLAY_DIS | R128_CRTC_VSYNC_DIS), ~mask);
        }
        break;
    case DPMSModeOff:
        if (r128_crtc->crtc_id) {
            OUTREGP(R128_CRTC2_GEN_CNTL, mask, ~mask);
        } else {
            OUTREGP(R128_CRTC_EXT_CNTL, mask, ~mask);
        }
        break;
    }

    if (mode != DPMSModeOn) {
        if (r128_crtc->crtc_id) {
            OUTREGP(R128_CRTC2_GEN_CNTL, 0, ~R128_CRTC2_EN);
        } else {
            OUTREGP(R128_CRTC_GEN_CNTL, 0, ~R128_CRTC_EN);
        }
    } else {
        if (r128_crtc->crtc_id) {
            OUTREGP(R128_CRTC2_GEN_CNTL, R128_CRTC2_EN, ~R128_CRTC2_EN);
        } else {
            OUTREGP(R128_CRTC_GEN_CNTL, R128_CRTC_EN, ~R128_CRTC_EN);
        }
    }

    if (mode != DPMSModeOff)
        r128_crtc_load_lut(crtc);
}

void r128_crtc_load_lut(xf86CrtcPtr crtc)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    R128InfoPtr info = R128PTR(pScrn);
    unsigned char *R128MMIO = info->MMIO;
    R128CrtcPrivatePtr r128_crtc = crtc->driver_private;
    int i;

    if (!crtc->enabled)
        return;

    PAL_SELECT(r128_crtc->crtc_id);

    for (i = 0; i < 256; i++) {
        OUTPAL(i, r128_crtc->lut_r[i], r128_crtc->lut_g[i], r128_crtc->lut_b[i]);
    }
}

static Bool r128_crtc_mode_fixup(xf86CrtcPtr crtc, DisplayModePtr mode, DisplayModePtr adjusted_mode)
{
    return TRUE;
}

static void r128_crtc_mode_prepare(xf86CrtcPtr crtc)
{
    r128_crtc_dpms(crtc, DPMSModeOff);
}

static void r128_crtc_mode_set(xf86CrtcPtr crtc, DisplayModePtr mode, DisplayModePtr adjusted_mode, int x, int y)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    R128CrtcPrivatePtr r128_crtc = crtc->driver_private;
    R128InfoPtr info = R128PTR(pScrn);
    double dot_clock = adjusted_mode->Clock / 1000.0;

    if (r128_crtc->cursor_offset) r128_crtc_hide_cursor(crtc);
    xf86PrintModeline(pScrn->scrnIndex, adjusted_mode);
    R128InitCommonRegisters(&info->ModeReg, info);

    switch (r128_crtc->crtc_id) {
    case 0:
        R128InitCrtcRegisters(crtc, &info->ModeReg, adjusted_mode);
	R128InitCrtcBase(crtc, &info->ModeReg, x, y);
        if (dot_clock) {
            R128InitPLLRegisters(crtc, &info->ModeReg, &info->pll, dot_clock);
            R128InitDDARegisters(crtc, &info->ModeReg, &info->pll, adjusted_mode);
        } else {
            info->ModeReg.ppll_ref_div         = info->SavedReg.ppll_ref_div;
            info->ModeReg.ppll_div_3           = info->SavedReg.ppll_div_3;
            info->ModeReg.htotal_cntl          = info->SavedReg.htotal_cntl;
            info->ModeReg.dda_config           = info->SavedReg.dda_config;
            info->ModeReg.dda_on_off           = info->SavedReg.dda_on_off;
        }
        break;
    case 1:
        R128InitCrtc2Registers(crtc, &info->ModeReg, adjusted_mode);
	R128InitCrtc2Base(crtc, &info->ModeReg, x, y);
        if (dot_clock) {
            R128InitPLL2Registers(crtc, &info->ModeReg, &info->pll, dot_clock);
            R128InitDDA2Registers(crtc, &info->ModeReg, &info->pll, adjusted_mode);
        }
        break;
    }

    R128RestoreCommonRegisters(pScrn, &info->ModeReg);

    switch (r128_crtc->crtc_id) {
    case 0:
        R128RestoreDDARegisters(pScrn, &info->ModeReg);
        R128RestoreCrtcRegisters(pScrn, &info->ModeReg);
        R128RestorePLLRegisters(pScrn, &info->ModeReg);
        break;
    case 1:
        R128RestoreDDA2Registers(pScrn, &info->ModeReg);
        R128RestoreCrtc2Registers(pScrn, &info->ModeReg);
        R128RestorePLL2Registers(pScrn, &info->ModeReg);
	break;
    }

    if (r128_crtc->cursor_offset) r128_crtc_show_cursor(crtc);
}

static void r128_crtc_mode_commit(xf86CrtcPtr crtc)
{
    r128_crtc_dpms(crtc, DPMSModeOn);
}

static void r128_crtc_gamma_set(xf86CrtcPtr crtc, uint16_t *red, uint16_t *green, uint16_t *blue, int size)
{
    R128CrtcPrivatePtr r128_crtc = crtc->driver_private;
    int i;

    for (i = 0; i < 256; i++) {
        r128_crtc->lut_r[i] = red[i] >> 8;
        r128_crtc->lut_g[i] = green[i] >> 8;
        r128_crtc->lut_b[i] = blue[i] >> 8;
    }

    r128_crtc_load_lut(crtc);
}

static Bool r128_crtc_lock(xf86CrtcPtr crtc)
{
    ScrnInfoPtr   pScrn   = crtc->scrn;
    ScreenPtr     pScreen = xf86ScrnToScreen(pScrn);
    R128InfoPtr   info    = R128PTR(pScrn);

#ifdef HAVE_XAA_H
    if (info->accel) info->accel->Sync(pScrn);
#endif
#ifdef USE_EXA
    if (info->ExaDriver) exaWaitSync(pScreen);
#endif

    return FALSE;
}

static void r128_crtc_unlock(xf86CrtcPtr crtc)
{
    ScrnInfoPtr   pScrn   = crtc->scrn;
    ScreenPtr     pScreen = xf86ScrnToScreen(pScrn);
    R128InfoPtr   info    = R128PTR(pScrn);

#ifdef HAVE_XAA_H
    if (info->accel) info->accel->Sync(pScrn);
#endif
#ifdef USE_EXA
    if (info->ExaDriver) exaWaitSync(pScreen);
#endif
}

static void *r128_crtc_shadow_allocate(xf86CrtcPtr crtc, int width, int height)
{
    ScrnInfoPtr   pScrn   = crtc->scrn;
    R128InfoPtr   info    = R128PTR(pScrn);

    R128CrtcPrivatePtr r128_crtc = crtc->driver_private;
    unsigned long rotate_offset = 0;
    unsigned long rotate_pitch;
    int cpp = pScrn->bitsPerPixel / 8;
    int align = 4096;
    int size;

    rotate_pitch = pScrn->displayWidth * cpp;
    size = rotate_pitch * height;
    rotate_offset = R128AllocateMemory(pScrn, &(r128_crtc->rotate_mem), size, align, TRUE);

    /* If allocations failed or if there was no accel. */
    if (rotate_offset == 0)
        return NULL;

    return info->FB + rotate_offset;
}

static PixmapPtr r128_crtc_shadow_create(xf86CrtcPtr crtc, void *data, int width, int height)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    PixmapPtr rotate_pixmap;
    unsigned long rotate_pitch;
    int cpp = pScrn->bitsPerPixel / 8;

    if (!data) data = r128_crtc_shadow_allocate(crtc, width, height);

    rotate_pitch = pScrn->displayWidth * cpp;
    rotate_pixmap = GetScratchPixmapHeader(xf86ScrnToScreen(pScrn),
                                           width, height,
                                           pScrn->depth,
                                           pScrn->bitsPerPixel,
                                           rotate_pitch,
                                           data);

    if (rotate_pixmap == NULL) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "Couldn't allocate shadow memory for rotated CRTC\n");
        return NULL;
    }

    return rotate_pixmap;
}

static void r128_crtc_shadow_destroy(xf86CrtcPtr crtc, PixmapPtr rotate_pixmap, void *data)
{
    ScrnInfoPtr   pScrn   = crtc->scrn;
    ScreenPtr     pScreen = xf86ScrnToScreen(pScrn);
    R128InfoPtr   info    = R128PTR(pScrn);

    R128CrtcPrivatePtr r128_crtc = crtc->driver_private;

    if (rotate_pixmap) FreeScratchPixmapHeader(rotate_pixmap);

    if (data && r128_crtc->rotate_mem != NULL) {
#ifdef USE_EXA
        if (info->ExaDriver)
            exaOffscreenFree(pScreen, (ExaOffscreenArea *) r128_crtc->rotate_mem);
#endif
#ifdef HAVE_XAA_H
        if (info->accel)
            xf86FreeOffscreenLinear((FBLinearPtr) r128_crtc->rotate_mem);
#endif
        r128_crtc->rotate_mem = NULL;
    }
}

static const xf86CrtcFuncsRec r128_crtc_funcs = {
    .dpms = r128_crtc_dpms,
    .save = NULL,
    .restore = NULL,
    .mode_fixup = r128_crtc_mode_fixup,
    .prepare = r128_crtc_mode_prepare,
    .mode_set = r128_crtc_mode_set,
    .commit = r128_crtc_mode_commit,
    .gamma_set = r128_crtc_gamma_set,
    .lock = r128_crtc_lock,
    .unlock = r128_crtc_unlock,
    .shadow_create = r128_crtc_shadow_create,
    .shadow_allocate = r128_crtc_shadow_allocate,
    .shadow_destroy = r128_crtc_shadow_destroy,
    .set_cursor_colors = r128_crtc_set_cursor_colors,
    .set_cursor_position = r128_crtc_set_cursor_position,
    .show_cursor = r128_crtc_show_cursor,
    .hide_cursor = r128_crtc_hide_cursor,
    .load_cursor_image = r128_crtc_load_cursor_image,
    .destroy = NULL,
};

Bool R128AllocateControllers(ScrnInfoPtr pScrn)
{
    R128EntPtr pR128Ent = R128EntPriv(pScrn);

    if (pR128Ent->Controller[0])
        return TRUE;

    pR128Ent->pCrtc[0] = xf86CrtcCreate(pScrn, &r128_crtc_funcs);
    if (!pR128Ent->pCrtc[0])
        return FALSE;

    pR128Ent->Controller[0] = xnfcalloc(sizeof(R128CrtcPrivateRec), 1);
    if (!pR128Ent->Controller[0])
        return FALSE;

    pR128Ent->pCrtc[0]->driver_private = pR128Ent->Controller[0];
    pR128Ent->Controller[0]->crtc_id = 0;

    if (!pR128Ent->HasCRTC2)
        return TRUE;

    pR128Ent->pCrtc[1] = xf86CrtcCreate(pScrn, &r128_crtc_funcs);
    if (!pR128Ent->pCrtc[1])
        return FALSE;

    pR128Ent->Controller[1] = xnfcalloc(sizeof(R128CrtcPrivateRec), 1);
    if (!pR128Ent->Controller[1]) {
        free(pR128Ent->Controller[0]);
        return FALSE;
    }

    pR128Ent->pCrtc[1]->driver_private = pR128Ent->Controller[1];
    pR128Ent->Controller[1]->crtc_id = 1;

    return TRUE;
}

void R128Blank(ScrnInfoPtr pScrn)
{
    xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    xf86OutputPtr output;
    xf86CrtcPtr crtc;
    int o, c;

    for (c = 0; c < xf86_config->num_crtc; c++) {
        crtc = xf86_config->crtc[c];
        for (o = 0; o < xf86_config->num_output; o++) {
            output = xf86_config->output[o];
            if (output->crtc != crtc)
                continue;

            output->funcs->dpms(output, DPMSModeOff);
        }
        crtc->funcs->dpms(crtc, DPMSModeOff);
    }
}

void R128Unblank(ScrnInfoPtr pScrn)
{
    xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    xf86OutputPtr output;
    xf86CrtcPtr crtc;
    int o, c;

    for (c = 0; c < xf86_config->num_crtc; c++) {
        crtc = xf86_config->crtc[c];
        if (!crtc->enabled)
            continue;
        crtc->funcs->dpms(crtc, DPMSModeOn);
        for (o = 0; o < xf86_config->num_output; o++) {
            output = xf86_config->output[o];
            if (output->crtc != crtc)
                continue;

            output->funcs->dpms(output, DPMSModeOn);
        }
    }
}
