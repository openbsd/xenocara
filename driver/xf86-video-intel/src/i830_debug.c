/*
 * Copyright © 2006 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Authors:
 *    Eric Anholt <eric@anholt.net>
 *
 */

#include <inttypes.h>

#ifdef REG_DUMPER
#include "reg_dumper/reg_dumper.h"

#else

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86.h"
#include "i830.h"
#include "i830_debug.h"
#include <strings.h>

#endif

#include "i810_reg.h"

#define DEBUGSTRING(func) static char *func(I830Ptr pI830, int reg, \
					    uint32_t val)

DEBUGSTRING(i830_debug_xyminus1)
{
    return XNFprintf("%d, %d", (val & 0xffff) + 1,
		     ((val & 0xffff0000) >> 16) + 1);
}

DEBUGSTRING(i830_debug_yxminus1)
{
    return XNFprintf("%d, %d", ((val & 0xffff0000) >> 16) + 1,
		     (val & 0xffff) + 1);
}

DEBUGSTRING(i830_debug_xy)
{
    return XNFprintf("%d, %d", (val & 0xffff),
		     ((val & 0xffff0000) >> 16));
}

DEBUGSTRING(i830_debug_dspstride)
{
    return XNFprintf("%d bytes", val);
}

DEBUGSTRING(i830_debug_dspcntr)
{
    char *enabled = val & DISPLAY_PLANE_ENABLE ? "enabled" : "disabled";
    char plane = val & DISPPLANE_SEL_PIPE_B ? 'B' : 'A';
    return XNFprintf("%s, pipe %c", enabled, plane);
}

DEBUGSTRING(i830_debug_pipeconf)
{
    char *enabled = val & PIPEACONF_ENABLE ? "enabled" : "disabled";
    char *bit30;
    if (IS_I965G(pI830))
	bit30 = val & I965_PIPECONF_ACTIVE ? "active" : "inactive";
    else
	bit30 = val & PIPEACONF_DOUBLE_WIDE ? "double-wide" : "single-wide";
    return XNFprintf("%s, %s", enabled, bit30);
}

DEBUGSTRING(i830_debug_pipestat)
{
    char *_FIFO_UNDERRUN = val & FIFO_UNDERRUN ? " FIFO_UNDERRUN" : "";
    char *_CRC_ERROR_ENABLE = val & CRC_ERROR_ENABLE ? " CRC_ERROR_ENABLE" : "";
    char *_CRC_DONE_ENABLE = val & CRC_DONE_ENABLE ? " CRC_DONE_ENABLE" : "";
    char *_GMBUS_EVENT_ENABLE = val & GMBUS_EVENT_ENABLE ? " GMBUS_EVENT_ENABLE" : "";
    char *_VSYNC_INT_ENABLE = val & VSYNC_INT_ENABLE ? " VSYNC_INT_ENABLE" : "";
    char *_DLINE_COMPARE_ENABLE = val & DLINE_COMPARE_ENABLE ? " DLINE_COMPARE_ENABLE" : "";
    char *_DPST_EVENT_ENABLE = val & DPST_EVENT_ENABLE ? " DPST_EVENT_ENABLE" : "";
    char *_LBLC_EVENT_ENABLE = val & LBLC_EVENT_ENABLE ? " LBLC_EVENT_ENABLE" : "";
    char *_OFIELD_INT_ENABLE = val & OFIELD_INT_ENABLE ? " OFIELD_INT_ENABLE" : "";
    char *_EFIELD_INT_ENABLE = val & EFIELD_INT_ENABLE ? " EFIELD_INT_ENABLE" : "";
    char *_SVBLANK_INT_ENABLE = val & SVBLANK_INT_ENABLE ? " SVBLANK_INT_ENABLE" : "";
    char *_VBLANK_INT_ENABLE = val & VBLANK_INT_ENABLE ? " VBLANK_INT_ENABLE" : "";
    char *_OREG_UPDATE_ENABLE = val & OREG_UPDATE_ENABLE ? " OREG_UPDATE_ENABLE" : "";
    char *_CRC_ERROR_INT_STATUS = val & CRC_ERROR_INT_STATUS ? " CRC_ERROR_INT_STATUS" : "";
    char *_CRC_DONE_INT_STATUS = val & CRC_DONE_INT_STATUS ? " CRC_DONE_INT_STATUS" : "";
    char *_GMBUS_INT_STATUS = val & GMBUS_INT_STATUS ? " GMBUS_INT_STATUS" : "";
    char *_VSYNC_INT_STATUS = val & VSYNC_INT_STATUS ? " VSYNC_INT_STATUS" : "";
    char *_DLINE_COMPARE_STATUS = val & DLINE_COMPARE_STATUS ? " DLINE_COMPARE_STATUS" : "";
    char *_DPST_EVENT_STATUS = val & DPST_EVENT_STATUS ? " DPST_EVENT_STATUS" : "";
    char *_LBLC_EVENT_STATUS = val & LBLC_EVENT_STATUS ? " LBLC_EVENT_STATUS" : "";
    char *_OFIELD_INT_STATUS = val & OFIELD_INT_STATUS ? " OFIELD_INT_STATUS" : "";
    char *_EFIELD_INT_STATUS = val & EFIELD_INT_STATUS ? " EFIELD_INT_STATUS" : "";
    char *_SVBLANK_INT_STATUS = val & SVBLANK_INT_STATUS ? " SVBLANK_INT_STATUS" : "";
    char *_VBLANK_INT_STATUS = val & VBLANK_INT_STATUS ? " VBLANK_INT_STATUS" : "";
    char *_OREG_UPDATE_STATUS = val & OREG_UPDATE_STATUS ? " OREG_UPDATE_STATUS" : "";
    return XNFprintf("status:%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
		     _FIFO_UNDERRUN,
		     _CRC_ERROR_ENABLE,
		     _CRC_DONE_ENABLE,
		     _GMBUS_EVENT_ENABLE,
		     _VSYNC_INT_ENABLE,
		     _DLINE_COMPARE_ENABLE,
		     _DPST_EVENT_ENABLE,
		     _LBLC_EVENT_ENABLE,
		     _OFIELD_INT_ENABLE,
		     _EFIELD_INT_ENABLE,
		     _SVBLANK_INT_ENABLE,
		     _VBLANK_INT_ENABLE,
		     _OREG_UPDATE_ENABLE,
		     _CRC_ERROR_INT_STATUS,
		     _CRC_DONE_INT_STATUS,
		     _GMBUS_INT_STATUS,
		     _VSYNC_INT_STATUS,
		     _DLINE_COMPARE_STATUS,
		     _DPST_EVENT_STATUS,
		     _LBLC_EVENT_STATUS,
		     _OFIELD_INT_STATUS,
		     _EFIELD_INT_STATUS,
		     _SVBLANK_INT_STATUS,
		     _VBLANK_INT_STATUS,
		     _OREG_UPDATE_STATUS);
}

DEBUGSTRING(i830_debug_hvtotal)
{
    return XNFprintf("%d active, %d total", (val & 0xffff) + 1,
		     ((val & 0xffff0000) >> 16) + 1);
}

DEBUGSTRING(i830_debug_hvsyncblank)
{
    return XNFprintf("%d start, %d end", (val & 0xffff) + 1,
		     ((val & 0xffff0000) >> 16) + 1);
}

DEBUGSTRING(i830_debug_vgacntrl)
{
    return XNFprintf("%s", val & VGA_DISP_DISABLE ? "disabled" : "enabled");
}

DEBUGSTRING(i830_debug_fp)
{
    return XNFprintf("n = %d, m1 = %d, m2 = %d",
		     ((val & FP_N_DIV_MASK) >> FP_N_DIV_SHIFT),
		     ((val & FP_M1_DIV_MASK) >> FP_M1_DIV_SHIFT),
		     ((val & FP_M2_DIV_MASK) >> FP_M2_DIV_SHIFT));
}

DEBUGSTRING(i830_debug_vga_pd)
{
    int vga0_p1, vga0_p2, vga1_p1, vga1_p2;

    /* XXX: i9xx version */

    if (val & VGA0_PD_P1_DIV_2)
	vga0_p1 = 2;
    else
	vga0_p1 = ((val & VGA0_PD_P1_MASK) >> VGA0_PD_P1_SHIFT) + 2;
    vga0_p2 = (val & VGA0_PD_P2_DIV_4) ? 4 : 2;

    if (val & VGA1_PD_P1_DIV_2)
	vga1_p1 = 2;
    else
	vga1_p1 = ((val & VGA1_PD_P1_MASK) >> VGA1_PD_P1_SHIFT) + 2;
    vga1_p2 = (val & VGA1_PD_P2_DIV_4) ? 4 : 2;

    return XNFprintf("vga0 p1 = %d, p2 = %d, vga1 p1 = %d, p2 = %d",
		     vga0_p1, vga0_p2, vga1_p1, vga1_p2);
}

DEBUGSTRING(i830_debug_pp_status)
{
    char *status = val & PP_ON ? "on" : "off";
    char *ready = val & PP_READY ? "ready" : "not ready";
    char *seq = "unknown";

    switch (val & PP_SEQUENCE_MASK) {
    case PP_SEQUENCE_NONE:
	seq = "idle";
	break;
    case PP_SEQUENCE_ON:
	seq = "on";
	break;
    case PP_SEQUENCE_OFF:
	seq = "off";
	break;
    }

    return XNFprintf("%s, %s, sequencing %s", status, ready, seq);
}

DEBUGSTRING(i830_debug_pp_control)
{
    return XNFprintf("power target: %s",
		     val & POWER_TARGET_ON ? "on" : "off");
}

DEBUGSTRING(i830_debug_dpll)
{
    char *enabled = val & DPLL_VCO_ENABLE ? "enabled" : "disabled";
    char *dvomode = val & DPLL_DVO_HIGH_SPEED ? "dvo" : "non-dvo";
    char *vgamode = val & DPLL_VGA_MODE_DIS ? "" : ", VGA";
    char *mode = "unknown";
    char *clock = "unknown";
    char *fpextra = val & DISPLAY_RATE_SELECT_FPA1 ? ", using FPx1!" : "";
    char sdvoextra[20];
    int p1, p2 = 0;

    if (IS_I9XX(pI830)) {
	p1 = ffs((val & DPLL_FPA01_P1_POST_DIV_MASK) >>
		 DPLL_FPA01_P1_POST_DIV_SHIFT);
	switch (val & DPLL_MODE_MASK) {
	case DPLLB_MODE_DAC_SERIAL:
	    mode = "DAC/serial";
	    p2 = val & DPLL_DAC_SERIAL_P2_CLOCK_DIV_5 ? 5 : 10;
	    break;
	case DPLLB_MODE_LVDS:
	    mode = "LVDS";
	    p2 = val & DPLLB_LVDS_P2_CLOCK_DIV_7 ? 7 : 14;
	    break;
	}
    } else {
	Bool is_lvds = (INREG(LVDS) & LVDS_PORT_EN) && (reg == DPLL_B);

	if (is_lvds) {
	    mode = "LVDS";
	    p1 = ffs((val & DPLL_FPA01_P1_POST_DIV_MASK_I830_LVDS) >>
		     DPLL_FPA01_P1_POST_DIV_SHIFT);
	    if ((INREG(LVDS) & LVDS_CLKB_POWER_MASK) == LVDS_CLKB_POWER_UP)
		p2 = 7;
	    else
		p2 = 14;

	} else {
	    mode = "DAC/serial";
	    if (val & PLL_P1_DIVIDE_BY_TWO) {
		p1 = 2;
	    } else {
		/* Map the number in the field to (3, 33) */
		p1 = ((val & DPLL_FPA01_P1_POST_DIV_MASK_I830) >>
		      DPLL_FPA01_P1_POST_DIV_SHIFT) + 2;
	    }
	    if (val & PLL_P2_DIVIDE_BY_4)
		p2 = 4;
	    else
		p2 = 2;
	}
    }

    switch (val & PLL_REF_INPUT_MASK) {
    case PLL_REF_INPUT_DREFCLK:
	clock = "default";
	break;
    case PLL_REF_INPUT_TVCLKINA:
	clock = "TV A";
	break;
    case PLL_REF_INPUT_TVCLKINBC:
	clock = "TV B/C";
	break;
    case PLLB_REF_INPUT_SPREADSPECTRUMIN:
	if (reg == DPLL_B)
	    clock = "spread spectrum";
	break;
    }

    if (IS_I945G(pI830) || IS_I945GM(pI830) || IS_G33CLASS(pI830)) {
	sprintf(sdvoextra, ", SDVO mult %d",
		(int)((val & SDVO_MULTIPLIER_MASK) >>
		SDVO_MULTIPLIER_SHIFT_HIRES) + 1);
    } else {
	sdvoextra[0] = '\0';
    }

    return XNFprintf("%s, %s%s, %s clock, %s mode, p1 = %d, "
		     "p2 = %d%s%s",
		     enabled, dvomode, vgamode, clock, mode, p1, p2,
		     fpextra, sdvoextra);
}

DEBUGSTRING(i830_debug_dpll_test)
{
    char *dpllandiv = val & DPLLA_TEST_N_BYPASS ? ", DPLLA N bypassed" : "";
    char *dpllamdiv = val & DPLLA_TEST_M_BYPASS ? ", DPLLA M bypassed" : "";
    char *dpllainput = val & DPLLA_INPUT_BUFFER_ENABLE ?
	"" : ", DPLLA input buffer disabled";
    char *dpllbndiv = val & DPLLB_TEST_N_BYPASS ? ", DPLLB N bypassed" : "";
    char *dpllbmdiv = val & DPLLB_TEST_M_BYPASS ? ", DPLLB M bypassed" : "";
    char *dpllbinput = val & DPLLB_INPUT_BUFFER_ENABLE ?
	"" : ", DPLLB input buffer disabled";

    return XNFprintf("%s%s%s%s%s%s",
		     dpllandiv, dpllamdiv, dpllainput,
		     dpllbndiv, dpllbmdiv, dpllbinput);
}

DEBUGSTRING(i830_debug_adpa)
{
    char pipe = (val & ADPA_PIPE_B_SELECT) ? 'B' : 'A';
    char *enable = (val & ADPA_DAC_ENABLE) ? "enabled" : "disabled";
    char hsync = (val & ADPA_HSYNC_ACTIVE_HIGH) ? '+' : '-';
    char vsync = (val & ADPA_VSYNC_ACTIVE_HIGH) ? '+' : '-';

    return XNFprintf("%s, pipe %c, %chsync, %cvsync",
		     enable, pipe, hsync, vsync);
}

DEBUGSTRING(i830_debug_lvds)
{
    char pipe = val & LVDS_PIPEB_SELECT ? 'B' : 'A';
    char *enable = val & LVDS_PORT_EN ? "enabled" : "disabled";
    int depth;
    char *channels;

    if ((val & LVDS_A3_POWER_MASK) == LVDS_A3_POWER_UP)
	depth = 24;
    else
	depth = 18;
    if ((val & LVDS_B0B3_POWER_MASK) == LVDS_B0B3_POWER_UP)
	channels = "2 channels";
    else
	channels = "1 channel";


    return XNFprintf("%s, pipe %c, %d bit, %s",
		     enable, pipe, depth, channels);
}

DEBUGSTRING(i830_debug_dvo)
{
    char *enable = val & DVO_ENABLE ? "enabled" : "disabled";
    char pipe = val & DVO_PIPE_B_SELECT ? 'B' : 'A';
    char *stall;
    char hsync = val & DVO_HSYNC_ACTIVE_HIGH ? '+' : '-';
    char vsync = val & DVO_VSYNC_ACTIVE_HIGH ? '+' : '-';

    switch (val & DVO_PIPE_STALL_MASK) {
    case DVO_PIPE_STALL_UNUSED:
	stall = "no stall";
	break;
    case DVO_PIPE_STALL:
	stall = "stall";
	break;
    case DVO_PIPE_STALL_TV:
	stall = "TV stall";
	break;
    default:
	stall = "unknown stall";
	break;
    }

    return XNFprintf("%s, pipe %c, %s, %chsync, %cvsync",
		     enable, pipe, stall, hsync, vsync);
}

DEBUGSTRING(i830_debug_sdvo)
{
    char *enable = val & SDVO_ENABLE ? "enabled" : "disabled";
    char pipe = val & SDVO_PIPE_B_SELECT ? 'B' : 'A';
    char *stall = val & SDVO_STALL_SELECT ? "enabled" : "disabled";
    char *detected = val & SDVO_DETECTED ? "" : "not ";
    char *gang = val & SDVOC_GANG_MODE ? ", gang mode" : "";
    char sdvoextra[20];

    if (IS_I915G(pI830) || IS_I915GM(pI830)) {
	sprintf(sdvoextra, ", SDVO mult %d",
		(int)((val & SDVO_PORT_MULTIPLY_MASK) >>
		SDVO_PORT_MULTIPLY_SHIFT) + 1);
    } else {
	sdvoextra[0] = '\0';
    }

    return XNFprintf("%s, pipe %c, stall %s, %sdetected%s%s",
		     enable, pipe, stall, detected, sdvoextra, gang);
}

DEBUGSTRING(i830_debug_dspclk_gate_d)
{
    char *DPUNIT_B = val & DPUNIT_B_CLOCK_GATE_DISABLE ? " DPUNIT_B" : "";
    char *VSUNIT = val & VSUNIT_CLOCK_GATE_DISABLE ? " VSUNIT" : "";
    char *VRHUNIT = val & VRHUNIT_CLOCK_GATE_DISABLE ? " VRHUNIT" : "";
    char *VRDUNIT = val & VRDUNIT_CLOCK_GATE_DISABLE ? " VRDUNIT" : "";
    char *AUDUNIT = val & AUDUNIT_CLOCK_GATE_DISABLE ? " AUDUNIT" : "";
    char *DPUNIT_A = val & DPUNIT_A_CLOCK_GATE_DISABLE ? " DPUNIT_A" : "";
    char *DPCUNIT = val & DPCUNIT_CLOCK_GATE_DISABLE ? " DPCUNIT" : "";
    char *TVRUNIT = val & TVRUNIT_CLOCK_GATE_DISABLE ? " TVRUNIT" : "";
    char *TVCUNIT = val & TVCUNIT_CLOCK_GATE_DISABLE ? " TVCUNIT" : "";
    char *TVFUNIT = val & TVFUNIT_CLOCK_GATE_DISABLE ? " TVFUNIT" : "";
    char *TVEUNIT = val & TVEUNIT_CLOCK_GATE_DISABLE ? " TVEUNIT" : "";
    char *DVSUNIT = val & DVSUNIT_CLOCK_GATE_DISABLE ? " DVSUNIT" : "";
    char *DSSUNIT = val & DSSUNIT_CLOCK_GATE_DISABLE ? " DSSUNIT" : "";
    char *DDBUNIT = val & DDBUNIT_CLOCK_GATE_DISABLE ? " DDBUNIT" : "";
    char *DPRUNIT = val & DPRUNIT_CLOCK_GATE_DISABLE ? " DPRUNIT" : "";
    char *DPFUNIT = val & DPFUNIT_CLOCK_GATE_DISABLE ? " DPFUNIT" : "";
    char *DPBMUNIT = val & DPBMUNIT_CLOCK_GATE_DISABLE ? " DPBMUNIT" : "";
    char *DPLSUNIT = val & DPLSUNIT_CLOCK_GATE_DISABLE ? " DPLSUNIT" : "";
    char *DPLUNIT = val & DPLUNIT_CLOCK_GATE_DISABLE ? " DPLUNIT" : "";
    char *DPOUNIT = val & DPOUNIT_CLOCK_GATE_DISABLE ? " DPOUNIT" : "";
    char *DPBUNIT = val & DPBUNIT_CLOCK_GATE_DISABLE ? " DPBUNIT" : "";
    char *DCUNIT = val & DCUNIT_CLOCK_GATE_DISABLE ? " DCUNIT" : "";
    char *DPUNIT = val & DPUNIT_CLOCK_GATE_DISABLE ? " DPUNIT" : "";
    char *VRUNIT = val & VRUNIT_CLOCK_GATE_DISABLE ? " VRUNIT" : "";
    char *OVHUNIT = val & OVHUNIT_CLOCK_GATE_DISABLE ? " OVHUNIT" : "";
    char *DPIOUNIT = val & DPIOUNIT_CLOCK_GATE_DISABLE ? " DPIOUNIT" : "";
    char *OVFUNIT = val & OVFUNIT_CLOCK_GATE_DISABLE ? " OVFUNIT" : "";
    char *OVBUNIT = val & OVBUNIT_CLOCK_GATE_DISABLE ? " OVBUNIT" : "";
    char *OVRUNIT = val & OVRUNIT_CLOCK_GATE_DISABLE ? " OVRUNIT" : "";
    char *OVCUNIT = val & OVCUNIT_CLOCK_GATE_DISABLE ? " OVCUNIT" : "";
    char *OVUUNIT = val & OVUUNIT_CLOCK_GATE_DISABLE ? " OVUUNIT" : "";
    char *OVLUNIT = val & OVLUNIT_CLOCK_GATE_DISABLE ? " OVLUNIT" : "";

    return XNFprintf ("clock gates disabled:%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
		      DPUNIT_B,
		      VSUNIT,
		      VRHUNIT,
		      VRDUNIT,
		      AUDUNIT,
		      DPUNIT_A,
		      DPCUNIT,
		      TVRUNIT,
		      TVCUNIT,
		      TVFUNIT,
		      TVEUNIT,
		      DVSUNIT,
		      DSSUNIT,
		      DDBUNIT,
		      DPRUNIT,
		      DPFUNIT,
		      DPBMUNIT,
		      DPLSUNIT,
		      DPLUNIT,
		      DPOUNIT,
		      DPBUNIT,
		      DCUNIT,
		      DPUNIT,
		      VRUNIT,
		      OVHUNIT,
		      DPIOUNIT,
		      OVFUNIT,
		      OVBUNIT,
		      OVRUNIT,
		      OVCUNIT,
		      OVUUNIT,
		      OVLUNIT);
}

#if 0
DEBUGSTRING(i810_debug_fence_new)
{
    char *enable = (val & FENCE_VALID) ? "enabled" : "disabled";
    char format = (val & I965_FENCE_Y_MAJOR) ? 'Y' : 'X';
    int pitch = ((val & 0xffc) >> 2) * 128;
    unsigned int offset = val & 0xfffff000;

    return XNFprintf("%s, %c tile walk, %d pitch, 0x%08x offset",
		     enable, format, pitch, offset);
}
#endif

#define DEFINEREG(reg) \
	{ reg, #reg, NULL, 0 }
#define DEFINEREG2(reg, func) \
	{ reg, #reg, func, 0 }

static struct i830SnapshotRec {
    int reg;
    char *name;
    char *(*debug_output)(I830Ptr pI830, int reg, uint32_t val);
    uint32_t val;
} i830_snapshot[] = {
    DEFINEREG2(VCLK_DIVISOR_VGA0, i830_debug_fp),
    DEFINEREG2(VCLK_DIVISOR_VGA1, i830_debug_fp),
    DEFINEREG2(VCLK_POST_DIV, i830_debug_vga_pd),
    DEFINEREG2(DPLL_TEST, i830_debug_dpll_test),
    DEFINEREG(CACHE_MODE_0),
    DEFINEREG(D_STATE),
    DEFINEREG2(DSPCLK_GATE_D, i830_debug_dspclk_gate_d),
    DEFINEREG(RENCLK_GATE_D1),
    DEFINEREG(RENCLK_GATE_D2),
/*  DEFINEREG(RAMCLK_GATE_D),	CRL only */
    DEFINEREG2(SDVOB, i830_debug_sdvo),
    DEFINEREG2(SDVOC, i830_debug_sdvo),
/*    DEFINEREG(UDIB_SVB_SHB_CODES), CRL only */
/*    DEFINEREG(UDIB_SHA_BLANK_CODES), CRL only */
    DEFINEREG(SDVOUDI),
    DEFINEREG(DSPARB),
    DEFINEREG(DSPFW1),
    DEFINEREG(DSPFW2),
    DEFINEREG(DSPFW3),

    DEFINEREG2(ADPA, i830_debug_adpa),
    DEFINEREG2(LVDS, i830_debug_lvds),
    DEFINEREG2(DVOA, i830_debug_dvo),
    DEFINEREG2(DVOB, i830_debug_dvo),
    DEFINEREG2(DVOC, i830_debug_dvo),
    DEFINEREG(DVOA_SRCDIM),
    DEFINEREG(DVOB_SRCDIM),
    DEFINEREG(DVOC_SRCDIM),

    DEFINEREG2(PP_CONTROL, i830_debug_pp_control),
    DEFINEREG2(PP_STATUS, i830_debug_pp_status),
    DEFINEREG(PFIT_CONTROL),
    DEFINEREG(PFIT_PGM_RATIOS),
    DEFINEREG(PORT_HOTPLUG_EN),
    DEFINEREG(PORT_HOTPLUG_STAT),

    DEFINEREG2(DSPACNTR, i830_debug_dspcntr),
    DEFINEREG2(DSPASTRIDE, i830_debug_dspstride),
    DEFINEREG2(DSPAPOS, i830_debug_xy),
    DEFINEREG2(DSPASIZE, i830_debug_xyminus1),
    DEFINEREG(DSPABASE),
    DEFINEREG(DSPASURF),
    DEFINEREG(DSPATILEOFF),
    DEFINEREG2(PIPEACONF, i830_debug_pipeconf),
    DEFINEREG2(PIPEASRC, i830_debug_yxminus1),
    DEFINEREG2(PIPEASTAT, i830_debug_pipestat),

    DEFINEREG(FBC_CFB_BASE),
    DEFINEREG(FBC_LL_BASE),
    DEFINEREG(FBC_CONTROL),
    DEFINEREG(FBC_COMMAND),
    DEFINEREG(FBC_STATUS),
    DEFINEREG(FBC_CONTROL2),
    DEFINEREG(FBC_FENCE_OFF),
    DEFINEREG(FBC_MOD_NUM),

    DEFINEREG2(FPA0, i830_debug_fp),
    DEFINEREG2(FPA1, i830_debug_fp),
    DEFINEREG2(DPLL_A, i830_debug_dpll),
    DEFINEREG(DPLL_A_MD),
    DEFINEREG2(HTOTAL_A, i830_debug_hvtotal),
    DEFINEREG2(HBLANK_A, i830_debug_hvsyncblank),
    DEFINEREG2(HSYNC_A, i830_debug_hvsyncblank),
    DEFINEREG2(VTOTAL_A, i830_debug_hvtotal),
    DEFINEREG2(VBLANK_A, i830_debug_hvsyncblank),
    DEFINEREG2(VSYNC_A, i830_debug_hvsyncblank),
    DEFINEREG(BCLRPAT_A),
    DEFINEREG(VSYNCSHIFT_A),

    DEFINEREG2(DSPBCNTR, i830_debug_dspcntr),
    DEFINEREG2(DSPBSTRIDE, i830_debug_dspstride),
    DEFINEREG2(DSPBPOS, i830_debug_xy),
    DEFINEREG2(DSPBSIZE, i830_debug_xyminus1),
    DEFINEREG(DSPBBASE),
    DEFINEREG(DSPBSURF),
    DEFINEREG(DSPBTILEOFF),
    DEFINEREG2(PIPEBCONF, i830_debug_pipeconf),
    DEFINEREG2(PIPEBSRC, i830_debug_yxminus1),
    DEFINEREG2(PIPEBSTAT, i830_debug_pipestat),

    DEFINEREG2(FPB0, i830_debug_fp),
    DEFINEREG2(FPB1, i830_debug_fp),
    DEFINEREG2(DPLL_B, i830_debug_dpll),
    DEFINEREG(DPLL_B_MD),
    DEFINEREG2(HTOTAL_B, i830_debug_hvtotal),
    DEFINEREG2(HBLANK_B, i830_debug_hvsyncblank),
    DEFINEREG2(HSYNC_B, i830_debug_hvsyncblank),
    DEFINEREG2(VTOTAL_B, i830_debug_hvtotal),
    DEFINEREG2(VBLANK_B, i830_debug_hvsyncblank),
    DEFINEREG2(VSYNC_B, i830_debug_hvsyncblank),
    DEFINEREG(BCLRPAT_B),
    DEFINEREG(VSYNCSHIFT_B),

    DEFINEREG(VCLK_DIVISOR_VGA0),
    DEFINEREG(VCLK_DIVISOR_VGA1),
    DEFINEREG(VCLK_POST_DIV),
    DEFINEREG2(VGACNTRL, i830_debug_vgacntrl),

    DEFINEREG(TV_CTL),
    DEFINEREG(TV_DAC),
    DEFINEREG(TV_CSC_Y),
    DEFINEREG(TV_CSC_Y2),
    DEFINEREG(TV_CSC_U),
    DEFINEREG(TV_CSC_U2),
    DEFINEREG(TV_CSC_V),
    DEFINEREG(TV_CSC_V2),
    DEFINEREG(TV_CLR_KNOBS),
    DEFINEREG(TV_CLR_LEVEL),
    DEFINEREG(TV_H_CTL_1),
    DEFINEREG(TV_H_CTL_2),
    DEFINEREG(TV_H_CTL_3),
    DEFINEREG(TV_V_CTL_1),
    DEFINEREG(TV_V_CTL_2),
    DEFINEREG(TV_V_CTL_3),
    DEFINEREG(TV_V_CTL_4),
    DEFINEREG(TV_V_CTL_5),
    DEFINEREG(TV_V_CTL_6),
    DEFINEREG(TV_V_CTL_7),
    DEFINEREG(TV_SC_CTL_1),
    DEFINEREG(TV_SC_CTL_2),
    DEFINEREG(TV_SC_CTL_3),
    DEFINEREG(TV_WIN_POS),
    DEFINEREG(TV_WIN_SIZE),
    DEFINEREG(TV_FILTER_CTL_1),
    DEFINEREG(TV_FILTER_CTL_2),
    DEFINEREG(TV_FILTER_CTL_3),
    DEFINEREG(TV_CC_CONTROL),
    DEFINEREG(TV_CC_DATA),
    DEFINEREG(TV_H_LUMA_0),
    DEFINEREG(TV_H_LUMA_59),
    DEFINEREG(TV_H_CHROMA_0),
    DEFINEREG(TV_H_CHROMA_59),

    DEFINEREG(MI_MODE),
    /* DEFINEREG(MI_DISPLAY_POWER_DOWN), CRL only */
    DEFINEREG(MI_ARB_STATE),
    DEFINEREG(MI_RDRET_STATE),
    DEFINEREG(ECOSKPD),
#if 0
    DEFINEREG2(FENCE_NEW + 0, i810_debug_fence_new),
    DEFINEREG2(FENCE_NEW + 8, i810_debug_fence_new),
    DEFINEREG2(FENCE_NEW + 16, i810_debug_fence_new),
    DEFINEREG2(FENCE_NEW + 24, i810_debug_fence_new),
    DEFINEREG2(FENCE_NEW + 32, i810_debug_fence_new),
    DEFINEREG2(FENCE_NEW + 40, i810_debug_fence_new),
    DEFINEREG2(FENCE_NEW + 48, i810_debug_fence_new),
    DEFINEREG2(FENCE_NEW + 56, i810_debug_fence_new),
    DEFINEREG2(FENCE_NEW + 64, i810_debug_fence_new),
    DEFINEREG2(FENCE_NEW + 72, i810_debug_fence_new),
    DEFINEREG2(FENCE_NEW + 80, i810_debug_fence_new),
    DEFINEREG2(FENCE_NEW + 88, i810_debug_fence_new),
    DEFINEREG2(FENCE_NEW + 96, i810_debug_fence_new),
    DEFINEREG2(FENCE_NEW + 104, i810_debug_fence_new),
    DEFINEREG2(FENCE_NEW + 112, i810_debug_fence_new),
    DEFINEREG2(FENCE_NEW + 120, i810_debug_fence_new),
#endif
};
#undef DEFINEREG
#define NUM_I830_SNAPSHOTREGS (sizeof(i830_snapshot) / sizeof(i830_snapshot[0]))

#ifndef REG_DUMPER
void i830TakeRegSnapshot(ScrnInfoPtr pScrn)
{
    I830Ptr pI830 = I830PTR(pScrn);
    int i;

    for (i = 0; i < NUM_I830_SNAPSHOTREGS; i++) {
	i830_snapshot[i].val = INREG(i830_snapshot[i].reg);
    }
}

void i830CompareRegsToSnapshot(ScrnInfoPtr pScrn, char *where)
{
    I830Ptr pI830 = I830PTR(pScrn);
    int i;

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "Comparing regs from server start up to %s\n", where);
    for (i = 0; i < NUM_I830_SNAPSHOTREGS; i++) {
	uint32_t val = INREG(i830_snapshot[i].reg);
	if (i830_snapshot[i].val == val)
	    continue;

	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		   "Register 0x%x (%s) changed from 0x%08x to 0x%08x\n",
		   i830_snapshot[i].reg, i830_snapshot[i].name,
		   (int)i830_snapshot[i].val, (int)val);

	if (i830_snapshot[i].debug_output != NULL) {
	    char *before, *after;

	    before = i830_snapshot[i].debug_output(pI830,
						   i830_snapshot[i].reg,
						   i830_snapshot[i].val);
	    after = i830_snapshot[i].debug_output(pI830,
						  i830_snapshot[i].reg,
						  val);
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		       "%s before: %s\n", i830_snapshot[i].name, before);
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		       "%s after: %s\n", i830_snapshot[i].name, after);

	}
    }
}
#endif /* !REG_DUMPER */

static void i830DumpIndexed (ScrnInfoPtr pScrn, char *name, int id, int val, int min, int max)
{
    I830Ptr pI830 = I830PTR(pScrn);
    int	i;

    for (i = min; i <= max; i++) {
	OUTREG8 (id, i);
	xf86DrvMsg (pScrn->scrnIndex, X_INFO, "%18.18s%02x: 0x%02x\n",
		    name, i, INREG8(val));
    }
}

static void i830DumpAR(ScrnInfoPtr pScrn)
{
    I830Ptr pI830 = I830PTR(pScrn);
    int i;
    uint16_t st01;
    unsigned char orig_arx, msr;

    msr = INREG8(0x3cc);
    if (msr & 1)
	st01 = 0x3da;
    else
	st01 = 0x3ba;

    INREG8(st01); /* make sure index/write register is in index mode */
    orig_arx = INREG8(0x3c0);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "%19.19sX: 0x%02x\n",
	       "AR", orig_arx);
    
    for (i = 0; i <= 0x14; i++) {
	INREG8(st01);
	OUTREG8(0x3c0, i);
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "%18.18s%02x: 0x%02x\n",
		   "AR", i, INREG8(0x3c1));
    }
    INREG8(st01);
    OUTREG8(0x3c0, orig_arx);
    INREG8(st01); /* switch back to index mode */
}

void i830DumpRegs (ScrnInfoPtr pScrn)
{
    I830Ptr pI830 = I830PTR(pScrn);
    int i;
    int	fp, dpll;
    int pipe;
    int	n, m1, m2, m, p1, p2;
    int ref;
    int	dot;
    int phase;
    int msr;
    int crt;

    xf86DrvMsg (pScrn->scrnIndex, X_INFO, "DumpRegsBegin\n");
    for (i = 0; i < NUM_I830_SNAPSHOTREGS; i++) {
	uint32_t val = INREG(i830_snapshot[i].reg);

	if (i830_snapshot[i].debug_output != NULL) {
	    char *debug = i830_snapshot[i].debug_output(pI830,
							i830_snapshot[i].reg,
							val);
	    xf86DrvMsg (pScrn->scrnIndex, X_INFO, "%20.20s: 0x%08x (%s)\n",
			i830_snapshot[i].name, (unsigned int)val, debug);
	    xfree(debug);
	} else {
	    xf86DrvMsg (pScrn->scrnIndex, X_INFO, "%20.20s: 0x%08x\n",
			i830_snapshot[i].name, (unsigned int)val);
	}
    }
    i830DumpIndexed (pScrn, "SR", 0x3c4, 0x3c5, 0, 7);
    msr = INREG8(0x3cc);
    xf86DrvMsg (pScrn->scrnIndex, X_INFO, "%20.20s: 0x%02x\n",
		    "MSR", (unsigned int) msr);

    i830DumpAR (pScrn);
    if (msr & 1)
	crt = 0x3d0;
    else
	crt = 0x3b0;
    i830DumpIndexed (pScrn, "CR", crt + 4, crt + 5, 0, 0x24);
    for (pipe = 0; pipe <= 1; pipe++)
    {
	fp = INREG(pipe == 0 ? FPA0 : FPB0);
	dpll = INREG(pipe == 0 ? DPLL_A : DPLL_B);
	if (IS_I9XX(pI830)) 
	{
	    uint32_t lvds = INREG(LVDS);
	    if ((lvds & LVDS_PORT_EN) &&
		(lvds & LVDS_PIPEB_SELECT) == (pipe << 30))
	    {
		if ((lvds & LVDS_CLKB_POWER_MASK) == LVDS_CLKB_POWER_UP)
		    p2 = 7;
		else
		    p2 = 14;
	    }
	    else
	    {
		switch ((dpll >> 24) & 0x3) {
		case 0:
		    p2 = 10;
		    break;
		case 1:
		    p2 = 5;
		    break;
		default:
		    p2 = 1;
		    xf86DrvMsg (pScrn->scrnIndex, X_WARNING, "p2 out of range\n");
		    break;
		}
	    }
	    switch ((dpll >> 16) & 0xff) {
	    case 1:
		p1 = 1; break;
	    case 2:
		p1 = 2; break;
	    case 4:
		p1 = 3; break;
	    case 8:
		p1 = 4; break;
	    case 16:
		p1 = 5; break;
	    case 32:
		p1 = 6; break;
	    case 64:
		p1 = 7; break;
	    case 128:
		p1 = 8; break;
	    default:
		p1 = 1;
		xf86DrvMsg (pScrn->scrnIndex, X_WARNING, "p1 out of range\n");
		break;
	    }
	    
	    switch ((dpll >> 13) & 0x3) {
	    case 0:
		ref = 96000;
		break;
	    case 3:
		ref = 100000;
		break;
	    default:
		ref = 0;
		xf86DrvMsg (pScrn->scrnIndex, X_WARNING, "ref out of range\n");
		break;
	    }
	}
	else
	{
	    uint32_t lvds = INREG(LVDS);
	    if (IS_I85X (pI830) && 
		(lvds & LVDS_PORT_EN) &&
		(lvds & LVDS_PIPEB_SELECT) == (pipe << 30))
	    {
		if ((lvds & LVDS_CLKB_POWER_MASK) == LVDS_CLKB_POWER_UP)
		    p2 = 7;
		else
		    p2 = 14;
		switch ((dpll >> 16) & 0x3f) {
		case 0x01:  p1 = 1; break;
		case 0x02:  p1 = 2; break;
		case 0x04:  p1 = 3; break;
		case 0x08:  p1 = 4; break;
		case 0x10:  p1 = 5; break;
		case 0x20:  p1 = 6; break;
		default:
		    p1 = 1;
		    xf86DrvMsg (pScrn->scrnIndex, X_WARNING, "LVDS P1 0x%x invalid encoding\n",
				(dpll >> 16) & 0x3f);
		    break;
		}
	    }
	    else
	    {
		if (dpll & (1 << 23))
		    p2 = 4;
		else
		    p2 = 2;
		if (dpll & PLL_P1_DIVIDE_BY_TWO)
		    p1 = 2;
		else
		    p1 = ((dpll >> 16) & 0x3f) + 2;
	    }

	    switch ((dpll >> 13) & 0x3) {
	    case 0:
		ref = 48000;
		break;
	    case 3:
		ref = 66000;
		break;
	    default:
		ref = 0;
		xf86DrvMsg (pScrn->scrnIndex, X_WARNING, "ref out of range\n");
		break;
	    }
	}
	if (IS_I965G(pI830)) {
	    phase = (dpll >> 9) & 0xf;
	    switch (phase) {
	    case 6:
		break;
	    default:
		xf86DrvMsg (pScrn->scrnIndex, X_INFO,
			    "SDVO phase shift %d out of range -- probobly not "
			    "an issue.\n", phase);
		break;
	    }
	}
	switch ((dpll >> 8) & 1) {
	case 0:
	    break;
	default:
	    xf86DrvMsg (pScrn->scrnIndex, X_WARNING,
			"fp select out of range\n");
	    break;
	}
	n = ((fp >> 16) & 0x3f);
	m1 = ((fp >> 8) & 0x3f);
	m2 = ((fp >> 0) & 0x3f);
	m = 5 * (m1 + 2) + (m2 + 2);
	dot = (ref * (5 * (m1 + 2) + (m2 + 2)) / (n + 2)) / (p1 * p2);
	xf86DrvMsg (pScrn->scrnIndex, X_INFO, "pipe %s dot %d n %d m1 %d m2 %d p1 %d p2 %d\n",
		    pipe == 0 ? "A" : "B", dot, n, m1, m2, p1, p2);
    }
    xf86DrvMsg (pScrn->scrnIndex, X_INFO, "DumpRegsEnd\n");
}

#ifndef REG_DUMPER

#define NUM_RING_DUMP	64

static void
i830_dump_ring(ScrnInfoPtr pScrn)
{
    I830Ptr pI830 = I830PTR(pScrn);
    unsigned int head, tail, ring, mask;
    volatile unsigned char *virt;
    
    head = (INREG (LP_RING + RING_HEAD)) & I830_HEAD_MASK;
    tail = INREG (LP_RING + RING_TAIL) & I830_TAIL_MASK;
    mask = pI830->LpRing->tail_mask;
    
    virt = pI830->LpRing->virtual_start;
    ErrorF ("Ring at virtual %p head 0x%x tail 0x%x count %d\n",
	    virt, head, tail, (((tail + mask + 1) - head) & mask) >> 2);
    for (ring = (head - 128) & mask; ring != ((head + 4) & mask);
	 ring = (ring + 4) & mask)
    {
	ErrorF ("\t%08x: %08x\n", ring, *(volatile unsigned int *) (virt + ring));
    }
    ErrorF ("Ring end\n");
}

/* Famous last words
 */
void
i830_dump_error_state(ScrnInfoPtr pScrn)
{
    I830Ptr pI830 = I830PTR(pScrn);

    ErrorF("pgetbl_ctl: 0x%08x getbl_err: 0x%08x\n",
	   INREG(PGETBL_CTL), INREG(PGE_ERR));

    ErrorF("ipeir: 0x%08x iphdr: 0x%08x\n", INREG(IPEIR), INREG(IPEHR));

    ErrorF("LP ring tail: 0x%08x head: 0x%08x len: 0x%08x start 0x%08x\n",
	   INREG(LP_RING + RING_TAIL),
	   INREG(LP_RING + RING_HEAD) & HEAD_ADDR,
	   INREG(LP_RING + RING_LEN),
	   INREG(LP_RING + RING_START));

    ErrorF("eir: 0x%04x esr: 0x%04x emr: 0x%04x\n",
	   INREG16(EIR), INREG16(ESR), INREG16(EMR));

    ErrorF("instdone: 0x%04x instpm: 0x%04x\n",
	   INREG16(INST_DONE), INREG8(INST_PM));

    ErrorF("memmode: 0x%08x instps: 0x%08x\n",
	   INREG(MEMMODE), INREG(INST_PS));

    ErrorF("hwstam: 0x%04x ier: 0x%04x imr: 0x%04x iir: 0x%04x\n",
	   INREG16(HWSTAM), INREG16(IER), INREG16(IMR), INREG16(IIR));
    i830_dump_ring (pScrn);
}

void
i965_dump_error_state(ScrnInfoPtr pScrn)
{
    I830Ptr pI830 = I830PTR(pScrn);

    ErrorF("pgetbl_ctl: 0x%08x pgetbl_err: 0x%08x\n",
	   INREG(PGETBL_CTL), INREG(PGE_ERR));

    ErrorF("ipeir: 0x%08x iphdr: 0x%08x\n",
	   INREG(IPEIR_I965), INREG(IPEHR_I965));

    ErrorF("LP ring tail: 0x%08x head: %x len: 0x%08x start 0x%08x\n",
	   INREG(LP_RING + RING_TAIL),
	   INREG(LP_RING + RING_HEAD) & HEAD_ADDR,
	   INREG(LP_RING + RING_LEN),
	   INREG(LP_RING + RING_START));

    ErrorF("Err ID (eir): 0x%08x\n"
	   "Err Status (esr): 0x%08x\n"
	   "Err Mask (emr): 0x%08x\n",
	   INREG(EIR), INREG(ESR), INREG(EMR));

    ErrorF("instdone: 0x%08x instdone_1: 0x%08x\n",
	   INREG(INST_DONE_I965), INREG(INST_DONE_1));
    ErrorF("instpm: 0x%08x\n", INREG(INST_PM));

    ErrorF("memmode: 0x%08x instps: 0x%08x\n",
	   INREG(MEMMODE), INREG(INST_PS_I965));

    ErrorF("HW Status mask (hwstam): 0x%08x\nIRQ enable (ier): 0x%08x "
	   "imr: 0x%08x iir: 0x%08x\n",
	   INREG(HWSTAM), INREG(IER), INREG(IMR), INREG(IIR));

    ErrorF("acthd: 0x%08x dma_fadd_p: 0x%08x\n",
	   INREG(ACTHD), INREG(DMA_FADD_P));
    ErrorF("ecoskpd: 0x%08x excc: 0x%08x\n",
	   INREG(ECOSKPD), INREG(EXCC));

    ErrorF("cache_mode: 0x%08x/0x%08x\n", INREG(CACHE_MODE_0),
	   INREG(CACHE_MODE_1));
    ErrorF("mi_arb_state: 0x%08x\n", INREG(MI_ARB_STATE));

    ErrorF("IA_VERTICES_COUNT_QW 0x%08x/0x%08x\n",
	   INREG(IA_VERTICES_COUNT_QW),
	   INREG(IA_VERTICES_COUNT_QW+4));
    ErrorF("IA_PRIMITIVES_COUNT_QW 0x%08x/0x%08x\n",
	   INREG(IA_PRIMITIVES_COUNT_QW),
	   INREG(IA_PRIMITIVES_COUNT_QW+4));

    ErrorF("VS_INVOCATION_COUNT_QW 0x%08x/0x%08x\n",
	   INREG(VS_INVOCATION_COUNT_QW),
	   INREG(VS_INVOCATION_COUNT_QW+4));

    ErrorF("GS_INVOCATION_COUNT_QW 0x%08x/0x%08x\n",
	   INREG(GS_INVOCATION_COUNT_QW),
	   INREG(GS_INVOCATION_COUNT_QW+4));
    ErrorF("GS_PRIMITIVES_COUNT_QW 0x%08x/0x%08x\n",
	   INREG(GS_PRIMITIVES_COUNT_QW),
	   INREG(GS_PRIMITIVES_COUNT_QW+4));

    ErrorF("CL_INVOCATION_COUNT_QW 0x%08x/0x%08x\n",
	   INREG(CL_INVOCATION_COUNT_QW),
	   INREG(CL_INVOCATION_COUNT_QW+4));
    ErrorF("CL_PRIMITIVES_COUNT_QW 0x%08x/0x%08x\n",
	   INREG(CL_PRIMITIVES_COUNT_QW),
	   INREG(CL_PRIMITIVES_COUNT_QW+4));

    ErrorF("PS_INVOCATION_COUNT_QW 0x%08x/0x%08x\n",
	   INREG(PS_INVOCATION_COUNT_QW),
	   INREG(PS_INVOCATION_COUNT_QW+4));
    ErrorF("PS_DEPTH_COUNT_QW 0x%08x/0x%08x\n",
	   INREG(PS_DEPTH_COUNT_QW),
	   INREG(PS_DEPTH_COUNT_QW+4));

    ErrorF("WIZ_CTL 0x%08x\n", INREG(WIZ_CTL));
    ErrorF("TS_CTL 0x%08x  TS_DEBUG_DATA 0x%08x\n", INREG(TS_CTL),
	   INREG(TS_DEBUG_DATA));
    ErrorF("TD_CTL 0x%08x / 0x%08x\n",
	   INREG(TD_CTL), INREG(TD_CTL2));
}

/**
 * Checks the hardware error state bits.
 *
 * \return TRUE if any errors were found.
 */
Bool
i830_check_error_state(ScrnInfoPtr pScrn)
{
    I830Ptr pI830 = I830PTR(pScrn);
    int errors = 0;
    unsigned long temp, head, tail;

    if (!I830IsPrimary(pScrn)) return TRUE;

    temp = INREG16(ESR);
    if (temp != 0) {
	Bool vertex_max = !IS_I965G(pI830) && (temp & ERR_VERTEX_MAX);
	Bool pgtbl = temp & ERR_PGTBL_ERROR;
	Bool underrun = !IS_I965G(pI830) &&
	    (temp & ERR_DISPLAY_OVERLAY_UNDERRUN);
	Bool instruction = !IS_I965G(pI830) && (temp & ERR_INSTRUCTION_ERROR);

	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		   "ESR is 0x%08lx%s%s%s%s\n", temp,
		   vertex_max ? ", max vertices exceeded" : "",
		   pgtbl ? ", page table error" : "",
		   underrun ? ", display/overlay underrun" : "",
		   instruction ? ", instruction error" : "");
	errors++;
    }
    /* Check first for page table errors */
    if (!IS_I9XX(pI830)) {
	temp = INREG(PGE_ERR);
	if (temp != 0) {
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		       "PGTBL_ER is 0x%08lx\n", temp);
	    errors++;
	}
    } else {
	temp = INREG(PGTBL_ER);
	if (temp != 0) {
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		       "PGTBL_ER is 0x%08lx"
		       "%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s\n", temp,
		       temp & PGTBL_ERR_HOST_GTT_PTE ? ", host gtt pte" : "",
		       temp & PGTBL_ERR_HOST_PTE_DATA ? ", host pte data" : "",
		       temp & PGTBL_ERR_DISPA_GTT_PTE ? ", display A pte" : "",
		       temp & PGTBL_ERR_DISPA_TILING ?
		       ", display A tiling" : "",
		       temp & PGTBL_ERR_DISPB_GTT_PTE ? ", display B pte" : "",
		       temp & PGTBL_ERR_DISPB_TILING ?
		       ", display B tiling" : "",
		       temp & PGTBL_ERR_DISPC_GTT_PTE ? ", display C pte" : "",
		       temp & PGTBL_ERR_DISPC_TILING ?
		       ", display C tiling" : "",
		       temp & PGTBL_ERR_OVERLAY_GTT_PTE ?
		       ", overlay GTT PTE" : "",
		       temp & PGTBL_ERR_OVERLAY_TILING ?
		       ", overlay tiling" : "",
		       temp & PGTBL_ERR_CS_GTT ? ", CS GTT" : "",
		       temp & PGTBL_ERR_CS_INSTRUCTION_GTT_PTE ?
		       ", CS instruction GTT PTE" : "",
		       temp & PGTBL_ERR_CS_VERTEXDATA_GTT_PTE ?
		       ", CS vertex data GTT PTE" : "",
		       temp & PGTBL_ERR_BIN_INSTRUCTION_GTT_PTE ?
		       ", BIN instruction GTT PTE" : "",
		       temp & PGTBL_ERR_BIN_VERTEXDATA_GTT_PTE ?
		       ", BIN vertex data GTT PTE" : "",
		       temp & PGTBL_ERR_LC_GTT_PTE ? ", LC pte" : "",
		       temp & PGTBL_ERR_LC_TILING ? ", LC tiling" : "",
		       temp & PGTBL_ERR_MT_GTT_PTE ? ", MT pte" : "",
		       temp & PGTBL_ERR_MT_TILING ? ", MT tiling" : "");
	    errors++;
	}
    }
    temp = INREG(PGETBL_CTL);
    if (!(temp & 1)) {
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		   "PGTBL_CTL (0x%08lx) indicates GTT is disabled\n", temp);
	errors++;
    }
    temp = INREG(LP_RING + RING_LEN);
    if (temp & 1) {
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		   "PRB0_CTL (0x%08lx) indicates ring buffer enabled\n", temp);
	errors++;
    }
    head = INREG(LP_RING + RING_HEAD);
    tail = INREG(LP_RING + RING_TAIL);
    if ((tail & I830_TAIL_MASK) != (head & I830_HEAD_MASK)) {
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		   "PRB0_HEAD (0x%08lx) and PRB0_TAIL (0x%08lx) indicate "
		   "ring buffer not flushed\n", head, tail);
	errors++;
    }

#if 0
    if (errors) {
	if (IS_I965G(pI830))
	    i965_dump_error_state(pScrn);
	else
	    i830_dump_error_state(pScrn);
    }
#endif

    return (errors != 0);
}
#endif /* !REG_DUMPER */
