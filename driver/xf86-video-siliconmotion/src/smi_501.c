/*
Copyright (C) 1994-1999 The XFree86 Project, Inc.  All Rights Reserved.
Copyright (C) 2000 Silicon Motion, Inc.  All Rights Reserved.
Copyright (C) 2008 Mandriva Linux.  All Rights Reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FIT-
NESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
XFREE86 PROJECT BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the names of The XFree86 Project and
Silicon Motion shall not be used in advertising or otherwise to promote the
sale, use or other dealings in this Software without prior written
authorization from The XFree86 Project or Silicon Motion.
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "smi.h"
#include "smi_crtc.h"
#include "smi_501.h"
#include "regsmi.h"

#ifdef HAVE_XEXTPROTO_71
#include <X11/extensions/dpmsconst.h>
#else
#define DPMS_SERVER
#include <X11/extensions/dpms.h>
#endif


/* Want to see register dumps for now */
#undef VERBLEV
#define VERBLEV		1


/*
 * Prototypes
 */

static char *format_integer_base2(int32_t word);
static void SMI501_SetClock(SMIPtr pSmi, int32_t port,
			    int32_t pll, int32_t value);


/*
 * Implemementation
 */

void
SMI501_Save(ScrnInfoPtr pScrn)
{
    SMIPtr	pSmi = SMIPTR(pScrn);
    MSOCRegPtr	save = pSmi->save;

    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, VERBLEV,
		   "Register dump (Before Save)\n");
    SMI501_PrintRegs(pScrn);

    /* Used mainly for DPMS info */
    save->system_ctl.value = READ_SCR(pSmi, SYSTEM_CTL);

    /* Used basically to enable dac */
    save->misc_ctl.value = READ_SCR(pSmi, MISC_CTL);

    /* Read it first to know if current power mode */
    save->power_ctl.value = READ_SCR(pSmi, POWER_CTL);

    switch (save->power_ctl.f.mode) {
	case 0:
	    save->current_gate  = POWER0_GATE;
	    save->current_clock = POWER0_CLOCK;
	    break;
	case 1:
	    save->current_gate  = POWER1_GATE;
	    save->current_clock = POWER1_CLOCK;
	    break;
	default:
	    /* FIXME
	     * Should be in sleep mode
	     * TODO
	     * select mode0 by default
	     */
	    save->current_gate = POWER0_GATE;
	    save->current_clock = POWER0_CLOCK;
	    break;
    }

    save->gate.value  = READ_SCR(pSmi, save->current_gate);
    save->clock.value = READ_SCR(pSmi, save->current_clock);

    /* FIXME Never changed */
    save->timing_ctl.value = READ_SCR(pSmi, TIMING_CTL);

    save->pll_ctl.value = READ_SCR(pSmi, PLL_CTL);
    save->device_id.value = READ_SCR(pSmi, DEVICE_ID);
    save->sleep_gate.value = READ_SCR(pSmi, SLEEP_GATE);

    save->panel_display_ctl.value = READ_SCR(pSmi, PANEL_DISPLAY_CTL);
    save->panel_fb_address.value = READ_SCR(pSmi, PANEL_FB_ADDRESS);
    save->panel_fb_width.value = READ_SCR(pSmi, PANEL_FB_WIDTH);
    save->panel_wwidth.value = READ_SCR(pSmi, PANEL_WWIDTH);
    save->panel_wheight.value = READ_SCR(pSmi, PANEL_WHEIGHT);
    save->panel_plane_tl.value = READ_SCR(pSmi, PANEL_PLANE_TL);
    save->panel_plane_br.value = READ_SCR(pSmi, PANEL_PLANE_BR);
    save->panel_htotal.value = READ_SCR(pSmi, PANEL_HTOTAL);
    save->panel_hsync.value = READ_SCR(pSmi, PANEL_HSYNC);
    save->panel_vtotal.value = READ_SCR(pSmi, PANEL_VTOTAL);
    save->panel_vsync.value = READ_SCR(pSmi, PANEL_VSYNC);

    save->crt_display_ctl.value = READ_SCR(pSmi, CRT_DISPLAY_CTL);
    save->crt_fb_address.value = READ_SCR(pSmi, CRT_FB_ADDRESS);
    save->crt_fb_width.value = READ_SCR(pSmi, CRT_FB_WIDTH);
    save->crt_htotal.value = READ_SCR(pSmi, CRT_HTOTAL);
    save->crt_hsync.value = READ_SCR(pSmi, CRT_HSYNC);
    save->crt_vtotal.value = READ_SCR(pSmi, CRT_VTOTAL);
    save->crt_vsync.value = READ_SCR(pSmi, CRT_VSYNC);

    save->alpha_display_ctl.value = READ_SCR(pSmi, ALPHA_DISPLAY_CTL);
    save->alpha_fb_address.value = READ_SCR(pSmi, ALPHA_FB_ADDRESS);
    save->alpha_fb_width.value = READ_SCR(pSmi, ALPHA_FB_WIDTH);
    save->alpha_plane_tl.value = READ_SCR(pSmi, ALPHA_PLANE_TL);
    save->alpha_plane_br.value = READ_SCR(pSmi, ALPHA_PLANE_BR);
    save->alpha_chroma_key.value = READ_SCR(pSmi, ALPHA_CHROMA_KEY);

    /* Also save accel state to properly restore kernel framebuffer */
    save->accel_src = READ_SCR(pSmi, ACCEL_SRC);
    save->accel_dst = READ_SCR(pSmi, ACCEL_DST);
    save->accel_dim = READ_SCR(pSmi, ACCEL_DIM);
    save->accel_ctl = READ_SCR(pSmi, ACCEL_CTL);
    save->accel_pitch = READ_SCR(pSmi, ACCEL_PITCH);
    save->accel_fmt = READ_SCR(pSmi, ACCEL_FMT);
    save->accel_clip_tl = READ_SCR(pSmi, ACCEL_CLIP_TL);
    save->accel_clip_br = READ_SCR(pSmi, ACCEL_CLIP_BR);
    save->accel_pat_lo = READ_SCR(pSmi, ACCEL_PAT_LO);
    save->accel_pat_hi = READ_SCR(pSmi, ACCEL_PAT_HI);
    save->accel_wwidth = READ_SCR(pSmi, ACCEL_WWIDTH);
    save->accel_src_base = READ_SCR(pSmi, ACCEL_SRC_BASE);
    save->accel_dst_base = READ_SCR(pSmi, ACCEL_DST_BASE);
}

void
SMI501_DisplayPowerManagementSet(ScrnInfoPtr pScrn,
				 int PowerManagementMode, int flags)
{
    SMIPtr		pSmi = SMIPTR(pScrn);

    if (pSmi->CurrentDPMS != PowerManagementMode) {
	/* Set the DPMS mode to every output and CRTC */
	xf86DPMSSet(pScrn, PowerManagementMode, flags);

	pSmi->CurrentDPMS = PowerManagementMode;
    }
}

Bool
SMI501_HWInit(ScrnInfoPtr pScrn)
{
    MSOCRegPtr	save;
    MSOCRegPtr	mode;
    SMIPtr	pSmi = SMIPTR(pScrn);
    int32_t	x_select, x_divider, x_shift;

    save = pSmi->save;
    mode = pSmi->mode;

    /* Start with a fresh copy of registers before any mode change */
    memcpy(mode, save, sizeof(MSOCRegRec));

    if (pSmi->UseFBDev)
	return (TRUE);

    /* Enable DAC -- 0: enable - 1: disable */
    mode->misc_ctl.f.dac = 0;

    /* Enable 2D engine */
    mode->gate.f.engine = 1;
    /* Color space conversion */
    mode->gate.f.csc = 1;
    /* ZV port */
    mode->gate.f.zv = 1;
    /* Gpio, Pwm, and I2c */
    mode->gate.f.gpio = 1;

    /* FIXME fixed at power mode 0 as in the smi sources */
    mode->power_ctl.f.status = 0;
    mode->power_ctl.f.mode = 0;

    if (pSmi->MCLK) {
	xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, VERBLEV,
		       "MCLK request %d\n", pSmi->MCLK);
	(void)SMI501_FindMemClock(pSmi->MCLK, &x_select, &x_divider, &x_shift);
	mode->clock.f.m_select = x_select;
	mode->clock.f.m_divider = x_divider;
	mode->clock.f.m_shift = x_shift;
    }
    /* Else use what was configured by the kernel. */

    if (pSmi->MXCLK) {
	xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, VERBLEV,
		       "MXCLK request %d\n", pSmi->MXCLK);
	(void)SMI501_FindMemClock(pSmi->MXCLK, &x_select, &x_divider, &x_shift);
	mode->clock.f.m1_select = x_select;
	mode->clock.f.m1_divider = x_divider;
	mode->clock.f.m1_shift = x_shift;
    }
    /* Else use what was configured by the kernel. */

    if (!pSmi->Dualhead) {
	/* crt clones panel */
	mode->crt_display_ctl.f.enable = 0;
	/* 0: select panel - 1: select crt */
	mode->crt_display_ctl.f.select = 0;
	mode->crt_display_ctl.f.timing = 0;
    }

    SMI501_WriteMode_common(pScrn, mode);

    return (TRUE);
}

void
SMI501_WriteMode_common(ScrnInfoPtr pScrn, MSOCRegPtr mode)
{
    int32_t		pll;
    MSOCClockRec	clock;
    SMIPtr		pSmi = SMIPTR(pScrn);

    if (!pSmi->UseFBDev) {
	/* Update gate first */
	WRITE_SCR(pSmi, mode->current_gate, mode->gate.value);

	clock.value = READ_SCR(pSmi, mode->current_clock);

	if (pSmi->MCLK) {
	    clock.f.m_select = mode->clock.f.m_select;
	    pll = clock.value;
	    clock.f.m_divider = mode->clock.f.m_divider;
	    clock.f.m_shift = mode->clock.f.m_shift;
	    SMI501_SetClock(pSmi, mode->current_clock, pll, clock.value);
	}

	if (pSmi->MXCLK) {
	    clock.f.m1_select = mode->clock.f.m1_select;
	    pll = clock.value;
	    clock.f.m1_divider = mode->clock.f.m1_divider;
	    clock.f.m1_shift = mode->clock.f.m1_shift;
	    SMI501_SetClock(pSmi, mode->current_clock, pll, clock.value);
	}

	WRITE_SCR(pSmi, MISC_CTL, mode->misc_ctl.value);

	WRITE_SCR(pSmi, POWER_CTL, mode->power_ctl.value);
    }

    /* Match configuration */
    /* FIXME some other fields should also be set, otherwise, since
     * neither kernel nor driver change it, a reboot is required to
     * modify or reset to default */
    mode->system_ctl.f.burst = mode->system_ctl.f.burst_read =
	pSmi->PCIBurst != FALSE;
    mode->system_ctl.f.retry = pSmi->PCIRetry != FALSE;
    WRITE_SCR(pSmi, SYSTEM_CTL, mode->system_ctl.value);

    if (!pSmi->Dualhead)
	WRITE_SCR(pSmi, CRT_DISPLAY_CTL, mode->crt_display_ctl.value);
}

void
SMI501_WriteMode_lcd(ScrnInfoPtr pScrn, MSOCRegPtr mode)
{
    int32_t		pll;
    MSOCClockRec	clock;
    SMIPtr		pSmi = SMIPTR(pScrn);

    if (!pSmi->UseFBDev) {
	clock.value = READ_SCR(pSmi, mode->current_clock);

	/* Alternate pll_select is only available for the SMI 502,
	 * and the bit should be only set in that case. */
	if (mode->clock.f.pll_select)
	    WRITE_SCR(pSmi, PLL_CTL, mode->pll_ctl.value);
	clock.f.p2_select = mode->clock.f.p2_select;
	pll = clock.value;
	clock.f.p2_divider = mode->clock.f.p2_divider;
	clock.f.p2_shift = mode->clock.f.p2_shift;
	clock.f.pll_select = mode->clock.f.pll_select;
	clock.f.p2_1xclck = mode->clock.f.p2_1xclck;
	SMI501_SetClock(pSmi, mode->current_clock, pll, clock.value);

	WRITE_SCR(pSmi, PANEL_FB_ADDRESS, mode->panel_fb_address.value);
	WRITE_SCR(pSmi, PANEL_FB_WIDTH, mode->panel_fb_width.value);

	WRITE_SCR(pSmi, PANEL_WWIDTH, mode->panel_wwidth.value);
	WRITE_SCR(pSmi, PANEL_WHEIGHT, mode->panel_wheight.value);

	WRITE_SCR(pSmi, PANEL_PLANE_TL, mode->panel_plane_tl.value);
	WRITE_SCR(pSmi, PANEL_PLANE_BR, mode->panel_plane_br.value);

	WRITE_SCR(pSmi, PANEL_HTOTAL, mode->panel_htotal.value);
	WRITE_SCR(pSmi, PANEL_HSYNC, mode->panel_hsync.value);
	WRITE_SCR(pSmi, PANEL_VTOTAL, mode->panel_vtotal.value);
	WRITE_SCR(pSmi, PANEL_VSYNC, mode->panel_vsync.value);
	WRITE_SCR(pSmi, PANEL_DISPLAY_CTL, mode->panel_display_ctl.value);
    }
}

void
SMI501_WriteMode_crt(ScrnInfoPtr pScrn, MSOCRegPtr mode)
{
    int32_t		pll;
    MSOCClockRec	clock;
    SMIPtr		pSmi = SMIPTR(pScrn);

    if (!pSmi->UseFBDev) {
	clock.value = READ_SCR(pSmi, mode->current_clock);

	clock.f.v2_select = mode->clock.f.v2_select;
	pll = clock.value;
	clock.f.v2_divider = mode->clock.f.v2_divider;
	clock.f.v2_shift = mode->clock.f.v2_shift;
	clock.f.v2_1xclck = mode->clock.f.v2_1xclck;
	SMI501_SetClock(pSmi, mode->current_clock, pll, clock.value);

	WRITE_SCR(pSmi, CRT_FB_ADDRESS, mode->crt_fb_address.value);
	WRITE_SCR(pSmi, CRT_FB_WIDTH, mode->crt_fb_width.value);
	WRITE_SCR(pSmi, CRT_HTOTAL, mode->crt_htotal.value);
	WRITE_SCR(pSmi, CRT_HSYNC, mode->crt_hsync.value);
	WRITE_SCR(pSmi, CRT_VTOTAL, mode->crt_vtotal.value);
	WRITE_SCR(pSmi, CRT_VSYNC, mode->crt_vsync.value);
	WRITE_SCR(pSmi, CRT_DISPLAY_CTL, mode->crt_display_ctl.value);
    }
}

void
SMI501_WriteMode_alpha(ScrnInfoPtr pScrn, MSOCRegPtr mode)
{
    SMIPtr	pSmi = SMIPTR(pScrn);

    WRITE_SCR(pSmi, ALPHA_FB_ADDRESS, mode->alpha_fb_address.value);
    WRITE_SCR(pSmi, ALPHA_FB_WIDTH, mode->alpha_fb_width.value);

    WRITE_SCR(pSmi, ALPHA_PLANE_TL, mode->alpha_plane_tl.value);
    WRITE_SCR(pSmi, ALPHA_PLANE_BR, mode->alpha_plane_br.value);

    WRITE_SCR(pSmi, ALPHA_CHROMA_KEY, mode->alpha_chroma_key.value);

    WRITE_SCR(pSmi, ALPHA_DISPLAY_CTL, mode->alpha_display_ctl.value);
}

void
SMI501_WriteMode(ScrnInfoPtr pScrn, MSOCRegPtr restore)
{
    SMIPtr	pSmi = SMIPTR(pScrn);

    SMI501_WriteMode_common(pScrn, restore);
    SMI501_WriteMode_lcd(pScrn, restore);
    SMI501_WriteMode_crt(pScrn, restore);
#if SMI_CURSOR_ALPHA_PLANE
    SMI501_WriteMode_alpha(pScrn, restore);
#endif

    /* This function should be called when switching to virtual console */
    WRITE_SCR(pSmi, ACCEL_SRC, restore->accel_src);
    WRITE_SCR(pSmi, ACCEL_DST, restore->accel_dst);
    WRITE_SCR(pSmi, ACCEL_DIM, restore->accel_dim);
    WRITE_SCR(pSmi, ACCEL_CTL, restore->accel_ctl);
    WRITE_SCR(pSmi, ACCEL_PITCH, restore->accel_pitch);
    WRITE_SCR(pSmi, ACCEL_FMT, restore->accel_fmt);
    WRITE_SCR(pSmi, ACCEL_CLIP_TL, restore->accel_clip_tl);
    WRITE_SCR(pSmi, ACCEL_CLIP_BR, restore->accel_clip_br);
    WRITE_SCR(pSmi, ACCEL_PAT_LO, restore->accel_pat_lo);
    WRITE_SCR(pSmi, ACCEL_PAT_HI, restore->accel_pat_hi);
    WRITE_SCR(pSmi, ACCEL_WWIDTH, restore->accel_wwidth);
    WRITE_SCR(pSmi, ACCEL_SRC_BASE, restore->accel_src_base);
    WRITE_SCR(pSmi, ACCEL_DST_BASE, restore->accel_dst_base);
}

void
SMI501_PowerPanel(ScrnInfoPtr pScrn, MSOCRegPtr mode, Bool on)
{
    SMIPtr	pSmi = SMIPTR(pScrn);

    if (on != FALSE) {
	mode->panel_display_ctl.f.vdd = 1;
	WRITE_SCR(pSmi, PANEL_DISPLAY_CTL, mode->panel_display_ctl.value);
	SMI501_WaitVSync(pSmi, 4);

	mode->panel_display_ctl.f.signal = 1;
	WRITE_SCR(pSmi, PANEL_DISPLAY_CTL, mode->panel_display_ctl.value);
	SMI501_WaitVSync(pSmi, 4);

	mode->panel_display_ctl.f.bias = 1;
	WRITE_SCR(pSmi, PANEL_DISPLAY_CTL, mode->panel_display_ctl.value);
	SMI501_WaitVSync(pSmi, 4);

	mode->panel_display_ctl.f.fp = 1;
	WRITE_SCR(pSmi, PANEL_DISPLAY_CTL, mode->panel_display_ctl.value);
	SMI501_WaitVSync(pSmi, 4);
    }
    else {
	mode->panel_display_ctl.f.fp = 0;
	WRITE_SCR(pSmi, PANEL_DISPLAY_CTL, mode->panel_display_ctl.value);
	SMI501_WaitVSync(pSmi, 4);

	mode->panel_display_ctl.f.bias = 0;
	WRITE_SCR(pSmi, PANEL_DISPLAY_CTL, mode->panel_display_ctl.value);
	SMI501_WaitVSync(pSmi, 4);

	mode->panel_display_ctl.f.signal = 0;
	WRITE_SCR(pSmi, PANEL_DISPLAY_CTL, mode->panel_display_ctl.value);
	SMI501_WaitVSync(pSmi, 4);

	mode->panel_display_ctl.f.vdd = 0;
	WRITE_SCR(pSmi, PANEL_DISPLAY_CTL, mode->panel_display_ctl.value);
	SMI501_WaitVSync(pSmi, 4);
    }
}

static char *
format_integer_base2(int32_t word)
{
    int		i;
    static char	buffer[33];

    for (i = 0; i < 32; i++) {
	if (word & (1 << i))
	    buffer[31 - i] = '1';
	else
	    buffer[31 - i] = '0';
    }

    return (buffer);
}

double
SMI501_FindClock(double clock, int32_t max_divider, Bool has1xclck,
		 int32_t *x2_1xclck,
		 int32_t *x2_select, int32_t *x2_divider, int32_t *x2_shift)
{
    double	diff, best, mclk;
    int32_t	multiplier, divider, shift, xclck;

    /* The Crystal input frequency is 24Mhz, and can be multiplied
     * by 12 or 14 (actually, there are other values, see TIMING_CTL,
     * MMIO 0x068) */

    /* Find clock best matching mode */
    best = 0x7fffffff;
    for (multiplier = 12, mclk  = multiplier * 24 * 1000.0;
	 mclk <= 14 * 24 * 1000.0;
	 multiplier += 2, mclk  = multiplier * 24 * 1000.0) {
	for (divider = 1; divider <= max_divider; divider += 2) {
	    for (shift = 0; shift < 8; shift++) {
		/* Divider 1 not in specs for cards older then 502 */
		for (xclck = 1; xclck >= !has1xclck; xclck--) {
		    diff = (mclk / (divider << shift << xclck)) - clock;
		    if (fabs(diff) < best) {
			*x2_shift = shift;
			*x2_divider = divider == 1 ? 0 : divider == 3 ? 1 : 2;
			*x2_select = mclk == 12 * 24 * 1000.0 ? 0 : 1;
			*x2_1xclck = xclck == 0;

			/* Remember best diff */
			best = fabs(diff);
		    }
		}
	    }
	}
    }

    xf86ErrorFVerb(VERBLEV,
		   "\tMatching clock %5.2f, diff %5.2f (%d/%d/%d/%d)\n",
		   ((*x2_select ? 14 : 12) * 24 * 1000.0) /
		   ((*x2_divider == 0 ? 1 : *x2_divider == 1 ? 3 : 5) <<
		    *x2_shift << (*x2_1xclck ? 0 : 1)),
		   best, *x2_shift, *x2_divider, *x2_select, *x2_1xclck);

    return (best);
}

double
SMI501_FindMemClock(double clock, int32_t *x1_select,
		    int32_t *x1_divider, int32_t *x1_shift)
{
    double	diff, best, mclk;
    int32_t	multiplier, divider, shift;

    best = 0x7fffffff;
    for (multiplier = 12, mclk  = multiplier * 24 * 1000.0;
	 mclk <= 14 * 24 * 1000.0;
	 multiplier += 2, mclk = multiplier * 24 * 1000.0) {
	for (divider = 1; divider <= 3; divider += 2) {
	    for (shift = 0; shift < 8; shift++) {
		diff = (mclk / (divider << shift)) - clock;
		if (fabs(diff) < best) {
		    *x1_shift = shift;
		    *x1_divider = divider == 1 ? 0 : 1;
		    *x1_select = mclk == 12 * 24 * 1000.0 ? 0 : 1;

		    /* Remember best diff */
		    best = fabs(diff);
		}
	    }
	}
    }

    xf86ErrorFVerb(VERBLEV,
		   "\tMatching clock %5.2f, diff %5.2f (%d/%d/%d)\n",
		   ((*x1_select ? 14 : 12) * 24 * 1000.0) /
		   ((*x1_divider == 0 ? 1 : 3) << *x1_shift),
		    best, *x1_shift, *x1_divider, *x1_select);

    return (best);
}


double
SMI501_FindPLLClock(double clock, int32_t *m, int32_t *n, int32_t *xclck)
{
    int32_t	M, N, K;
    double	diff, best;
    double	frequency;

    /*   This method, available only on the 502 is intended to cover the
     * disadvantage of the other method where certain modes cannot be
     * displayed correctly due to the big difference on the requested
     * pixel clock, with the actual pixel clock that can be achieved by
     * those divisions. In this method, N can be any integer between 2
     * and 24, M can be any positive, 8 bits integer, and K is either 1
     * or 2.
     *   To calculate the programmable PLL, the following formula is
     * used:
     *
     *	Requested Pixel Clock = Input Frequency * M / N
     *
     *   Input Frequency is the crystal input frequency value (24 MHz in
     * the SMI VGX Demo Board).
     *
     *   K is a divisor, used by setting bit 15 of the PLL_CTL
     * (PLL Output Divided by 2).
     *
     *   So, it should be requested_clock = input_frequency * M / N / K
     */

    /* That said, use what actually works, that is:
     * requested_clock = input_frequency * K * M / N
     *
     * where requested_clock is modeline pixel clock,
     * input_frequency is 12, K is either 1 or 2 (and sets bit15 accordingly),
     * M is a non zero 8 bits unsigned integer, and N is a value from 2 to 24.
     */

    best = 0x7fffffff;
    frequency = 12 * 1000.0;
    for (N = 2; N <= 24; N++) {
	for (K = 1; K <= 2; K++) {
	    M = clock / frequency * K * N;
	    diff = ((int32_t)(frequency / K * M) / N) - clock;
	    /* Ensure M is larger then 0 and fits in 8 bits */
	    if (M > 0 && M < 0x100 && fabs(diff) < best) {
		*m = M;
		*n = N;
		*xclck = K == 1;

		/* Remember best diff */
		best = fabs(diff);
	    }
	}
    }

    xf86ErrorFVerb(VERBLEV,
		   "\tMatching alternate clock %5.2f, diff %5.2f (%d/%d/%d)\n",
		   frequency / (*xclck ? 1 : 2) * *m / *n, best,
		   *m, *n, *xclck);

    return (best);
}

void
SMI501_PrintRegs(ScrnInfoPtr pScrn)
{
    int		i;
    SMIPtr	pSmi = SMIPTR(pScrn);

    xf86ErrorFVerb(VERBLEV, "    SMI501 System Setup:\n");
    for (i = 0x00; i <= 0x74; i += 4)
	xf86ErrorFVerb(VERBLEV, "\t%08x: %s\n", i,
		       format_integer_base2(READ_SCR(pSmi, i)));
    xf86ErrorFVerb(VERBLEV, "    SMI501 Display Setup:\n");
    for (i = 0x80000; i < 0x80400; i += 4)
	xf86ErrorFVerb(VERBLEV, "\t%08x: %s\n", i,
		       format_integer_base2(READ_SCR(pSmi, i)));
}

void
SMI501_WaitVSync(SMIPtr pSmi, int vsync_count)
{
    MSOCCmdStatusRec	status;
    int32_t		timeout;

    while (vsync_count-- > 0) {
	/* Wait for end of vsync */
	timeout = 0;
	do {
	    /* bit 11: vsync active *if set* */
	    status.value = READ_SCR(pSmi, CMD_STATUS);
	    if (++timeout == 10000)
		break;
	} while (status.f.pvsync);

	/* Wait for start of vsync */
	timeout = 0;
	do {
	    status.value = READ_SCR(pSmi, CMD_STATUS);
	    if (++timeout == 10000)
		break;
	} while (!status.f.pvsync);
    }
}

static void
SMI501_SetClock(SMIPtr pSmi, int32_t port, int32_t pll, int32_t value)
{
    /*
     *	Rules to Program the Power Mode Clock Registers for Clock Selection
     *
     *	1. There should be only one clock source changed at a time.
     *	   To change clock source for P2XCLK, V2XCLK, MCLK, M2XCLK
     *	   simultaneously may cause the internal logic normal operation
     *	   to be disrupted. There should be a minimum of 16mS wait from
     *	   change one clock source to another.
     *	2. When adjusting the clock rate, the PLL selection bit should
     *	   be programmed first before changing the divider value for each
     *	   clock source. For example, to change the P2XCLK clock rate:
     *		. bit 29 should be set first
     *		. wait for a minimum of 16ms (about one Vsync time)
     *		. adjust bits [28:24].
     *	   The minimum 16 ms wait is necessary for logic to settle down
     *	   before the clock rate is changed.
     *	3. There should be a minimum 16 ms wait after a clock source is
     *	   changed before any operation that could result in a bus
     *	   transaction.
     */

    /* register contents selecting clock */
    WRITE_SCR(pSmi, port, pll);
    SMI501_WaitVSync(pSmi, 1);

    /* full register contents */
    WRITE_SCR(pSmi, port, value);
    SMI501_WaitVSync(pSmi, 1);
}
