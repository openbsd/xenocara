/*
 * Integrated TV out support based on the GATOS code by
 * Federico Ulivi <fulivi@lycos.com>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string.h>
#include <stdio.h>

/* X and server generic header files */
#include "xf86.h"
#include "xf86_OSproc.h"
#include "vgaHW.h"
#include "xf86Modes.h"

/* Driver data structures */
#include "radeon.h"
#include "radeon_reg.h"
#include "radeon_macros.h"
#include "radeon_probe.h"
#include "radeon_version.h"
#include "radeon_tv.h"
#include "radeon_atombios.h"

/**********************************************************************
 *
 * ModeConstants
 *
 * Storage of constants related to a single video mode
 *
 **********************************************************************/

typedef struct
{
    uint16_t horResolution;
    uint16_t verResolution;
    TVStd  standard;
    uint16_t horTotal;
    uint16_t verTotal;
    uint16_t horStart;
    uint16_t horSyncStart;
    uint16_t verSyncStart;
    unsigned defRestart;
    uint16_t crtcPLL_N;
    uint8_t  crtcPLL_M;
    uint8_t  crtcPLL_postDiv;
    unsigned pixToTV;
} TVModeConstants;

static const uint16_t hor_timing_NTSC[] =
{
    0x0007,
    0x003f,
    0x0263,
    0x0a24,
    0x2a6b,
    0x0a36,
    0x126d, /* H_TABLE_POS1 */
    0x1bfe,
    0x1a8f, /* H_TABLE_POS2 */
    0x1ec7,
    0x3863,
    0x1bfe,
    0x1bfe,
    0x1a2a,
    0x1e95,
    0x0e31,
    0x201b,
    0
};

static const uint16_t vert_timing_NTSC[] =
{
    0x2001,
    0x200d,
    0x1006,
    0x0c06,
    0x1006,
    0x1818,
    0x21e3,
    0x1006,
    0x0c06,
    0x1006,
    0x1817,
    0x21d4,
    0x0002,
    0
};

static const uint16_t hor_timing_PAL[] =
{
    0x0007,
    0x0058,
    0x027c,
    0x0a31,
    0x2a77,
    0x0a95,
    0x124f, /* H_TABLE_POS1 */
    0x1bfe,
    0x1b22, /* H_TABLE_POS2 */
    0x1ef9,
    0x387c,
    0x1bfe,
    0x1bfe,
    0x1b31,
    0x1eb5,
    0x0e43,
    0x201b,
    0
};

static const uint16_t vert_timing_PAL[] =
{
    0x2001,
    0x200c,
    0x1005,
    0x0c05,
    0x1005,
    0x1401,
    0x1821,
    0x2240,
    0x1005,
    0x0c05,
    0x1005,
    0x1401,
    0x1822,
    0x2230,
    0x0002,
    0
};

/**********************************************************************
 *
 * availableModes
 *
 * Table of all allowed modes for tv output
 *
 **********************************************************************/
static const TVModeConstants availableTVModes[] =
{
    {   /* NTSC timing for 27 Mhz ref clk */
	800,                /* horResolution */
	600,                /* verResolution */
	TV_STD_NTSC,        /* standard */
	990,                /* horTotal */
	740,                /* verTotal */
	813,                /* horStart */
	824,                /* horSyncStart */
	632,                /* verSyncStart */
	625592,             /* defRestart */
	592,                /* crtcPLL_N */
	91,                 /* crtcPLL_M */
	4,                  /* crtcPLL_postDiv */
	1022,               /* pixToTV */
    },
    {   /* PAL timing for 27 Mhz ref clk */
	800,               /* horResolution */
	600,               /* verResolution */
	TV_STD_PAL,        /* standard */
	1144,              /* horTotal */
	706,               /* verTotal */
	812,               /* horStart */
	824,               /* horSyncStart */
	669,               /* verSyncStart */
	696700,            /* defRestart */
	1382,              /* crtcPLL_N */
	231,               /* crtcPLL_M */
	4,                 /* crtcPLL_postDiv */
	759,               /* pixToTV */
    },
    {   /* NTSC timing for 14 Mhz ref clk */
	800,                /* horResolution */
	600,                /* verResolution */
	TV_STD_NTSC,        /* standard */
	1018,               /* horTotal */
	727,                /* verTotal */
	813,                /* horStart */
	840,                /* horSyncStart */
	633,                /* verSyncStart */
	630627,             /* defRestart */
	347,                /* crtcPLL_N */
	14,                 /* crtcPLL_M */
	8,                  /* crtcPLL_postDiv */
	1022,               /* pixToTV */
    },
    {   /* PAL timing for 14 Mhz ref clk */
	800,                /* horResolution */
	600,                /* verResolution */
	TV_STD_PAL,         /* standard */
	1131,               /* horTotal */
	742,                /* verTotal */
	813,                /* horStart */
	840,                /* horSyncStart */
	633,                /* verSyncStart */
	708369,             /* defRestart */
	211,                /* crtcPLL_N */
	9,                  /* crtcPLL_M */
	8,                  /* crtcPLL_postDiv */
	759,                /* pixToTV */
    },
};

#define N_AVAILABLE_MODES (sizeof(availableModes) / sizeof(availableModes[ 0 ]))

static long YCOEF_value[5] = { 2, 2, 0, 4, 0 };
static long YCOEF_EN_value[5] = { 1, 1, 0, 1, 0 };
static long SLOPE_value[5] = { 1, 2, 2, 4, 8 };
static long SLOPE_limit[5] = { 6, 5, 4, 3, 2 };


static void
RADEONWaitPLLLock(ScrnInfoPtr pScrn, unsigned nTests,
		  unsigned nWaitLoops, unsigned cntThreshold)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    uint32_t savePLLTest;
    unsigned i;
    unsigned j;

    OUTREG(RADEON_TEST_DEBUG_MUX, (INREG(RADEON_TEST_DEBUG_MUX) & 0xffff60ff) | 0x100);

    savePLLTest = INPLL(pScrn, RADEON_PLL_TEST_CNTL);

    OUTPLL(pScrn, RADEON_PLL_TEST_CNTL, savePLLTest & ~RADEON_PLL_MASK_READ_B);

    /* XXX: these should probably be OUTPLL to avoid various PLL errata */

    OUTREG8(RADEON_CLOCK_CNTL_INDEX, RADEON_PLL_TEST_CNTL);

    for (i = 0; i < nTests; i++) {
	OUTREG8(RADEON_CLOCK_CNTL_DATA + 3, 0);
      
	for (j = 0; j < nWaitLoops; j++)
	    if (INREG8(RADEON_CLOCK_CNTL_DATA + 3) >= cntThreshold)
		break;
    }

    OUTPLL(pScrn, RADEON_PLL_TEST_CNTL, savePLLTest);

    OUTREG(RADEON_TEST_DEBUG_MUX, INREG(RADEON_TEST_DEBUG_MUX) & 0xffffe0ff);
}

/* Write to TV FIFO RAM */
static void
RADEONWriteTVFIFO(ScrnInfoPtr pScrn, uint16_t addr,
		  uint32_t value)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    uint32_t tmp;
    int i = 0;

    OUTREG(RADEON_TV_HOST_WRITE_DATA, value);

    OUTREG(RADEON_TV_HOST_RD_WT_CNTL, addr);
    OUTREG(RADEON_TV_HOST_RD_WT_CNTL, addr | RADEON_HOST_FIFO_WT);

    do {
	tmp = INREG(RADEON_TV_HOST_RD_WT_CNTL);
	if ((tmp & RADEON_HOST_FIFO_WT_ACK) == 0)
	    break;
	i++;
    }
    while (i < 10000);
    /*while ((tmp & RADEON_HOST_FIFO_WT_ACK) == 0);*/

    OUTREG(RADEON_TV_HOST_RD_WT_CNTL, 0);
}

/* Read from TV FIFO RAM */
static uint32_t
RADEONReadTVFIFO(ScrnInfoPtr pScrn, uint16_t addr)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    uint32_t tmp;
    int i = 0;
  
    OUTREG(RADEON_TV_HOST_RD_WT_CNTL, addr);
    OUTREG(RADEON_TV_HOST_RD_WT_CNTL, addr | RADEON_HOST_FIFO_RD);

    do {
	tmp = INREG(RADEON_TV_HOST_RD_WT_CNTL);
	if ((tmp & RADEON_HOST_FIFO_RD_ACK) == 0)
	    break;
	i++;
    }
    while (i < 10000);
    /*while ((tmp & RADEON_HOST_FIFO_RD_ACK) == 0);*/

    OUTREG(RADEON_TV_HOST_RD_WT_CNTL, 0);

    return INREG(RADEON_TV_HOST_READ_DATA);
}

/* Get FIFO addresses of horizontal & vertical code timing tables from
 * settings of uv_adr register. 
 */
static uint16_t
RADEONGetHTimingTablesAddr(uint32_t tv_uv_adr)
{
    uint16_t hTable;

    switch ((tv_uv_adr & RADEON_HCODE_TABLE_SEL_MASK) >> RADEON_HCODE_TABLE_SEL_SHIFT) {
    case 0:
	hTable = RADEON_TV_MAX_FIFO_ADDR_INTERNAL;
	break;
    case 1:
	hTable = ((tv_uv_adr & RADEON_TABLE1_BOT_ADR_MASK) >> RADEON_TABLE1_BOT_ADR_SHIFT) * 2;
	break;
    case 2:
	hTable = ((tv_uv_adr & RADEON_TABLE3_TOP_ADR_MASK) >> RADEON_TABLE3_TOP_ADR_SHIFT) * 2;
	break;
    default:
	/* Of course, this should never happen */
	hTable = 0;
	break;
    }
    return hTable;
}

static uint16_t
RADEONGetVTimingTablesAddr(uint32_t tv_uv_adr)
{
    uint16_t vTable;

    switch ((tv_uv_adr & RADEON_VCODE_TABLE_SEL_MASK) >> RADEON_VCODE_TABLE_SEL_SHIFT) {
    case 0:
	vTable = ((tv_uv_adr & RADEON_MAX_UV_ADR_MASK) >> RADEON_MAX_UV_ADR_SHIFT) * 2 + 1;
	break;
    case 1:
	vTable = ((tv_uv_adr & RADEON_TABLE1_BOT_ADR_MASK) >> RADEON_TABLE1_BOT_ADR_SHIFT) * 2 + 1;
	break;
    case 2:
	vTable = ((tv_uv_adr & RADEON_TABLE3_TOP_ADR_MASK) >> RADEON_TABLE3_TOP_ADR_SHIFT) * 2 + 1;
	break;
    default:
	/* Of course, this should never happen */
	vTable = 0;
	break;
    }
    return vTable;
}

/* Restore horizontal/vertical timing code tables */
static void
RADEONRestoreTVTimingTables(ScrnInfoPtr pScrn, RADEONSavePtr restore)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    uint16_t hTable;
    uint16_t vTable;
    uint32_t tmp;
    unsigned i;

    OUTREG(RADEON_TV_UV_ADR, restore->tv_uv_adr);
    hTable = RADEONGetHTimingTablesAddr(restore->tv_uv_adr);
    vTable = RADEONGetVTimingTablesAddr(restore->tv_uv_adr);

    for (i = 0; i < MAX_H_CODE_TIMING_LEN; i += 2, hTable--) {
	tmp = ((uint32_t)restore->h_code_timing[ i ] << 14) | ((uint32_t)restore->h_code_timing[ i + 1 ]);
	RADEONWriteTVFIFO(pScrn, hTable, tmp);
	if (restore->h_code_timing[ i ] == 0 || restore->h_code_timing[ i + 1 ] == 0)
	    break;
    }

    for (i = 0; i < MAX_V_CODE_TIMING_LEN; i += 2, vTable++) {
	tmp = ((uint32_t)restore->v_code_timing[ i + 1 ] << 14) | ((uint32_t)restore->v_code_timing[ i ]);
	RADEONWriteTVFIFO(pScrn, vTable, tmp);
	if (restore->v_code_timing[ i ] == 0 || restore->v_code_timing[ i + 1 ] == 0)
	    break;
    }
}

/* restore TV PLLs */
static void
RADEONRestoreTVPLLRegisters(ScrnInfoPtr pScrn, RADEONSavePtr restore)
{

    OUTPLLP(pScrn, RADEON_TV_PLL_CNTL1, 0, ~RADEON_TVCLK_SRC_SEL_TVPLL);
    OUTPLL(pScrn, RADEON_TV_PLL_CNTL, restore->tv_pll_cntl);
    OUTPLLP(pScrn, RADEON_TV_PLL_CNTL1, RADEON_TVPLL_RESET, ~RADEON_TVPLL_RESET);

    RADEONWaitPLLLock(pScrn, 200, 800, 135);
  
    OUTPLLP(pScrn, RADEON_TV_PLL_CNTL1, 0, ~RADEON_TVPLL_RESET);

    RADEONWaitPLLLock(pScrn, 300, 160, 27);
    RADEONWaitPLLLock(pScrn, 200, 800, 135);
  
    OUTPLLP(pScrn, RADEON_TV_PLL_CNTL1, 0, ~0xf);
    OUTPLLP(pScrn, RADEON_TV_PLL_CNTL1, RADEON_TVCLK_SRC_SEL_TVPLL, ~RADEON_TVCLK_SRC_SEL_TVPLL);
  
    OUTPLLP(pScrn, RADEON_TV_PLL_CNTL1, (1 << RADEON_TVPDC_SHIFT), ~RADEON_TVPDC_MASK);
    OUTPLLP(pScrn, RADEON_TV_PLL_CNTL1, 0, ~RADEON_TVPLL_SLEEP);
}

/* Restore TV horizontal/vertical settings */
static void
RADEONRestoreTVHVRegisters(ScrnInfoPtr pScrn, RADEONSavePtr restore)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;

    OUTREG(RADEON_TV_RGB_CNTL, restore->tv_rgb_cntl);

    OUTREG(RADEON_TV_HTOTAL, restore->tv_htotal);
    OUTREG(RADEON_TV_HDISP, restore->tv_hdisp);
    OUTREG(RADEON_TV_HSTART, restore->tv_hstart);

    OUTREG(RADEON_TV_VTOTAL, restore->tv_vtotal);
    OUTREG(RADEON_TV_VDISP, restore->tv_vdisp);

    OUTREG(RADEON_TV_FTOTAL, restore->tv_ftotal);

    OUTREG(RADEON_TV_VSCALER_CNTL1, restore->tv_vscaler_cntl1);
    OUTREG(RADEON_TV_VSCALER_CNTL2, restore->tv_vscaler_cntl2);

    OUTREG(RADEON_TV_Y_FALL_CNTL, restore->tv_y_fall_cntl);
    OUTREG(RADEON_TV_Y_RISE_CNTL, restore->tv_y_rise_cntl);
    OUTREG(RADEON_TV_Y_SAW_TOOTH_CNTL, restore->tv_y_saw_tooth_cntl);
}

/* restore TV RESTART registers */
static void
RADEONRestoreTVRestarts(ScrnInfoPtr pScrn, RADEONSavePtr restore)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;

    OUTREG(RADEON_TV_FRESTART, restore->tv_frestart);
    OUTREG(RADEON_TV_HRESTART, restore->tv_hrestart);
    OUTREG(RADEON_TV_VRESTART, restore->tv_vrestart);
}

/* restore tv standard & output muxes */
static void
RADEONRestoreTVOutputStd(ScrnInfoPtr pScrn, RADEONSavePtr restore)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;

    OUTREG(RADEON_TV_SYNC_CNTL, restore->tv_sync_cntl);
  
    OUTREG(RADEON_TV_TIMING_CNTL, restore->tv_timing_cntl);

    OUTREG(RADEON_TV_MODULATOR_CNTL1, restore->tv_modulator_cntl1);
    OUTREG(RADEON_TV_MODULATOR_CNTL2, restore->tv_modulator_cntl2);
 
    OUTREG(RADEON_TV_PRE_DAC_MUX_CNTL, restore->tv_pre_dac_mux_cntl);

    OUTREG(RADEON_TV_CRC_CNTL, restore->tv_crc_cntl);
}

/* Restore TV out regs */
void
RADEONRestoreTVRegisters(ScrnInfoPtr pScrn, RADEONSavePtr restore)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;

    ErrorF("Entering Restore TV\n");

    OUTREG(RADEON_TV_MASTER_CNTL, (restore->tv_master_cntl
				   | RADEON_TV_ASYNC_RST
				   | RADEON_CRT_ASYNC_RST
				   | RADEON_TV_FIFO_ASYNC_RST));

    /* Temporarily turn the TV DAC off */
    OUTREG(RADEON_TV_DAC_CNTL, ((restore->tv_dac_cntl & ~RADEON_TV_DAC_NBLANK)
				| RADEON_TV_DAC_BGSLEEP
				| RADEON_TV_DAC_RDACPD
				| RADEON_TV_DAC_GDACPD
				| RADEON_TV_DAC_BDACPD));

    ErrorF("Restore TV PLL\n");
    RADEONRestoreTVPLLRegisters(pScrn, restore);

    ErrorF("Restore TVHV\n");
    RADEONRestoreTVHVRegisters(pScrn, restore);

    OUTREG(RADEON_TV_MASTER_CNTL, (restore->tv_master_cntl
				   | RADEON_TV_ASYNC_RST
				   | RADEON_CRT_ASYNC_RST));

    ErrorF("Restore TV Restarts\n");
    RADEONRestoreTVRestarts(pScrn, restore);
  
    ErrorF("Restore Timing Tables\n");
    RADEONRestoreTVTimingTables(pScrn, restore);
  

    OUTREG(RADEON_TV_MASTER_CNTL, (restore->tv_master_cntl
				   | RADEON_TV_ASYNC_RST));

    ErrorF("Restore TV standard\n");
    RADEONRestoreTVOutputStd(pScrn, restore);

    OUTREG(RADEON_TV_MASTER_CNTL, restore->tv_master_cntl);

    OUTREG(RADEON_TV_GAIN_LIMIT_SETTINGS, restore->tv_gain_limit_settings);
    OUTREG(RADEON_TV_LINEAR_GAIN_SETTINGS, restore->tv_linear_gain_settings);

    OUTREG(RADEON_TV_DAC_CNTL, restore->tv_dac_cntl);

    ErrorF("Leaving Restore TV\n");
}

/* Save horizontal/vertical timing code tables */
static void
RADEONSaveTVTimingTables(ScrnInfoPtr pScrn, RADEONSavePtr save)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    uint16_t hTable;
    uint16_t vTable;
    uint32_t tmp;
    unsigned i;

    save->tv_uv_adr = INREG(RADEON_TV_UV_ADR);
    hTable = RADEONGetHTimingTablesAddr(save->tv_uv_adr);
    vTable = RADEONGetVTimingTablesAddr(save->tv_uv_adr);

    /*
     * Reset FIFO arbiter in order to be able to access FIFO RAM
     */

    OUTREG(RADEON_TV_MASTER_CNTL, (RADEON_TV_ASYNC_RST
				   | RADEON_CRT_ASYNC_RST
				   | RADEON_RESTART_PHASE_FIX
				   | RADEON_CRT_FIFO_CE_EN
				   | RADEON_TV_FIFO_CE_EN
				   | RADEON_TV_ON));

    /*OUTREG(RADEON_TV_MASTER_CNTL, save->tv_master_cntl | RADEON_TV_ON);*/

    ErrorF("saveTimingTables: reading timing tables\n");

    for (i = 0; i < MAX_H_CODE_TIMING_LEN; i += 2) {
	tmp = RADEONReadTVFIFO(pScrn, hTable--);
	save->h_code_timing[ i     ] = (uint16_t)((tmp >> 14) & 0x3fff);
	save->h_code_timing[ i + 1 ] = (uint16_t)(tmp & 0x3fff);

	if (save->h_code_timing[ i ] == 0 || save->h_code_timing[ i + 1 ] == 0)
	    break;
    }

    for (i = 0; i < MAX_V_CODE_TIMING_LEN; i += 2) {
	tmp = RADEONReadTVFIFO(pScrn, vTable++);
	save->v_code_timing[ i     ] = (uint16_t)(tmp & 0x3fff);
	save->v_code_timing[ i + 1 ] = (uint16_t)((tmp >> 14) & 0x3fff);

	if (save->v_code_timing[ i ] == 0 || save->v_code_timing[ i + 1 ] == 0)
	    break;
    }
}

/* read TV regs */
void
RADEONSaveTVRegisters(ScrnInfoPtr pScrn, RADEONSavePtr save)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;

    ErrorF("Entering TV Save\n");

    save->tv_crc_cntl = INREG(RADEON_TV_CRC_CNTL);
    save->tv_frestart = INREG(RADEON_TV_FRESTART);
    save->tv_hrestart = INREG(RADEON_TV_HRESTART);
    save->tv_vrestart = INREG(RADEON_TV_VRESTART);
    save->tv_gain_limit_settings = INREG(RADEON_TV_GAIN_LIMIT_SETTINGS);
    save->tv_hdisp = INREG(RADEON_TV_HDISP);
    save->tv_hstart = INREG(RADEON_TV_HSTART);
    save->tv_htotal = INREG(RADEON_TV_HTOTAL);
    save->tv_linear_gain_settings = INREG(RADEON_TV_LINEAR_GAIN_SETTINGS);
    save->tv_master_cntl = INREG(RADEON_TV_MASTER_CNTL);
    save->tv_rgb_cntl = INREG(RADEON_TV_RGB_CNTL);
    save->tv_modulator_cntl1 = INREG(RADEON_TV_MODULATOR_CNTL1);
    save->tv_modulator_cntl2 = INREG(RADEON_TV_MODULATOR_CNTL2);
    save->tv_pre_dac_mux_cntl = INREG(RADEON_TV_PRE_DAC_MUX_CNTL);
    save->tv_sync_cntl = INREG(RADEON_TV_SYNC_CNTL);
    save->tv_timing_cntl = INREG(RADEON_TV_TIMING_CNTL);
    save->tv_dac_cntl = INREG(RADEON_TV_DAC_CNTL);
    save->tv_upsamp_and_gain_cntl = INREG(RADEON_TV_UPSAMP_AND_GAIN_CNTL);
    save->tv_vdisp = INREG(RADEON_TV_VDISP);
    save->tv_ftotal = INREG(RADEON_TV_FTOTAL);
    save->tv_vscaler_cntl1 = INREG(RADEON_TV_VSCALER_CNTL1);
    save->tv_vscaler_cntl2 = INREG(RADEON_TV_VSCALER_CNTL2);
    save->tv_vtotal = INREG(RADEON_TV_VTOTAL);
    save->tv_y_fall_cntl = INREG(RADEON_TV_Y_FALL_CNTL);
    save->tv_y_rise_cntl = INREG(RADEON_TV_Y_RISE_CNTL);
    save->tv_y_saw_tooth_cntl = INREG(RADEON_TV_Y_SAW_TOOTH_CNTL);

    save->tv_pll_cntl = INPLL(pScrn, RADEON_TV_PLL_CNTL);
    save->tv_pll_cntl1 = INPLL(pScrn, RADEON_TV_PLL_CNTL1);

    ErrorF("Save TV timing tables\n");

    RADEONSaveTVTimingTables(pScrn, save);

    ErrorF("TV Save done\n");
}


/* Compute F,V,H restarts from default restart position and hPos & vPos
 * Return TRUE when code timing table was changed
 */
static Bool RADEONInitTVRestarts(xf86OutputPtr output, RADEONSavePtr save,
				 DisplayModePtr mode)
{
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    radeon_tvout_ptr tvout = &radeon_output->tvout;
    RADEONInfoPtr  info       = RADEONPTR(output->scrn);
    RADEONPLLPtr pll = &info->pll;
    int restart;
    unsigned hTotal;
    unsigned vTotal;
    unsigned fTotal;
    int vOffset;
    int hOffset;
    uint16_t p1;
    uint16_t p2;
    Bool hChanged;
    uint16_t hInc;
    const TVModeConstants *constPtr;

    /* FIXME: need to revisit this when we add more modes */
    if (tvout->tvStd == TV_STD_NTSC ||
	tvout->tvStd == TV_STD_NTSC_J ||
	tvout->tvStd == TV_STD_PAL_M) {
	if (pll->reference_freq == 2700)
	    constPtr = &availableTVModes[0];
	else
	    constPtr = &availableTVModes[2];
    } else {
	if (pll->reference_freq == 2700)
	    constPtr = &availableTVModes[1];
	else
	    constPtr = &availableTVModes[3];
    }

    hTotal = constPtr->horTotal;
    vTotal = constPtr->verTotal;

    if (tvout->tvStd == TV_STD_NTSC ||
	tvout->tvStd == TV_STD_NTSC_J ||
        tvout->tvStd == TV_STD_PAL_M ||
        tvout->tvStd == TV_STD_PAL_60)
	fTotal = NTSC_TV_VFTOTAL + 1;
    else
	fTotal = PAL_TV_VFTOTAL + 1;

    /* Adjust positions 1&2 in hor. code timing table */
    hOffset = tvout->hPos * H_POS_UNIT;

    if (tvout->tvStd == TV_STD_NTSC ||
	tvout->tvStd == TV_STD_NTSC_J ||
	tvout->tvStd == TV_STD_PAL_M) {
	/* improve image centering */
	hOffset -= 50;
	p1 = hor_timing_NTSC[ H_TABLE_POS1 ];
	p2 = hor_timing_NTSC[ H_TABLE_POS2 ];
    } else {
	p1 = hor_timing_PAL[ H_TABLE_POS1 ];
	p2 = hor_timing_PAL[ H_TABLE_POS2 ];
    }


    p1 = (uint16_t)((int)p1 + hOffset);
    p2 = (uint16_t)((int)p2 - hOffset);

    hChanged = (p1 != save->h_code_timing[ H_TABLE_POS1 ] || 
		p2 != save->h_code_timing[ H_TABLE_POS2 ]);

    save->h_code_timing[ H_TABLE_POS1 ] = p1;
    save->h_code_timing[ H_TABLE_POS2 ] = p2;

    /* Convert hOffset from n. of TV clock periods to n. of CRTC clock periods (CRTC pixels) */
    hOffset = (hOffset * (int)(constPtr->pixToTV)) / 1000;

    /* Adjust restart */
    restart = constPtr->defRestart;
 
    /*
     * Convert vPos TV lines to n. of CRTC pixels
     * Be verrrrry careful when mixing signed & unsigned values in C..
     */
    if (tvout->tvStd == TV_STD_NTSC ||
	tvout->tvStd == TV_STD_NTSC_J ||
	tvout->tvStd == TV_STD_PAL_M ||
	tvout->tvStd == TV_STD_PAL_60)
	vOffset = ((int)(vTotal * hTotal) * 2 * tvout->vPos) / (int)(NTSC_TV_LINES_PER_FRAME);
    else
	vOffset = ((int)(vTotal * hTotal) * 2 * tvout->vPos) / (int)(PAL_TV_LINES_PER_FRAME);

    restart -= vOffset + hOffset;

    ErrorF("computeRestarts: def = %u, h = %d, v = %d, p1=%04x, p2=%04x, restart = %d\n",
	   constPtr->defRestart , tvout->hPos , tvout->vPos , p1 , p2 , restart);

    save->tv_hrestart = restart % hTotal;
    restart /= hTotal;
    save->tv_vrestart = restart % vTotal;
    restart /= vTotal;
    save->tv_frestart = restart % fTotal;

    ErrorF("computeRestarts: F/H/V=%u,%u,%u\n",
	   (unsigned)save->tv_frestart, (unsigned)save->tv_vrestart,
	   (unsigned)save->tv_hrestart);

    /* Compute H_INC from hSize */
    if (tvout->tvStd == TV_STD_NTSC ||
	tvout->tvStd == TV_STD_NTSC_J ||
	tvout->tvStd == TV_STD_PAL_M)
	hInc = (uint16_t)((int)(constPtr->horResolution * 4096 * NTSC_TV_CLOCK_T) /
			(tvout->hSize * (int)(NTSC_TV_H_SIZE_UNIT) + (int)(NTSC_TV_ZERO_H_SIZE)));
    else
	hInc = (uint16_t)((int)(constPtr->horResolution * 4096 * PAL_TV_CLOCK_T) /
			(tvout->hSize * (int)(PAL_TV_H_SIZE_UNIT) + (int)(PAL_TV_ZERO_H_SIZE)));

    save->tv_timing_cntl = (save->tv_timing_cntl & ~RADEON_H_INC_MASK) |
	((uint32_t)hInc << RADEON_H_INC_SHIFT);

    ErrorF("computeRestarts: hSize=%d,hInc=%u\n" , tvout->hSize , hInc);

    return hChanged;
}

/* intit TV-out regs */
void RADEONInitTVRegisters(xf86OutputPtr output, RADEONSavePtr save,
                                  DisplayModePtr mode, BOOL IsPrimary)
{
    ScrnInfoPtr pScrn = output->scrn;
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    radeon_tvout_ptr tvout = &radeon_output->tvout;
    RADEONInfoPtr  info = RADEONPTR(pScrn);
    RADEONPLLPtr pll = &info->pll;
    unsigned m, n, p;
    unsigned i;
    unsigned long vert_space, flicker_removal;
    uint32_t tmp;
    const TVModeConstants *constPtr;
    const uint16_t *hor_timing;
    const uint16_t *vert_timing;
    radeon_encoder_ptr radeon_encoder = radeon_get_encoder(output);
    radeon_tvdac_ptr tvdac = NULL;

    if (radeon_encoder == NULL)
	return;

    tvdac = (radeon_tvdac_ptr)radeon_encoder->dev_priv;

    if (tvdac == NULL)
	return;

    /* FIXME: need to revisit this when we add more modes */
    if (tvout->tvStd == TV_STD_NTSC ||
	tvout->tvStd == TV_STD_NTSC_J ||
	tvout->tvStd == TV_STD_PAL_M) {
	if (pll->reference_freq == 2700)
	    constPtr = &availableTVModes[0];
	else
	    constPtr = &availableTVModes[2];
    } else {
	if (pll->reference_freq == 2700)
	    constPtr = &availableTVModes[1];
	else
	    constPtr = &availableTVModes[3];
    }

    save->tv_crc_cntl = 0;

    save->tv_gain_limit_settings = (0x17f << RADEON_UV_GAIN_LIMIT_SHIFT) | 
	                           (0x5ff << RADEON_Y_GAIN_LIMIT_SHIFT);

    save->tv_hdisp = constPtr->horResolution - 1;
    save->tv_hstart = constPtr->horStart;
    save->tv_htotal = constPtr->horTotal - 1;

    save->tv_linear_gain_settings = (0x100 << RADEON_UV_GAIN_SHIFT) |
	                            (0x100 << RADEON_Y_GAIN_SHIFT);

    save->tv_master_cntl = (RADEON_VIN_ASYNC_RST
			    | RADEON_CRT_FIFO_CE_EN
			    | RADEON_TV_FIFO_CE_EN
			    | RADEON_TV_ON);

    if (!IS_R300_VARIANT)
	save->tv_master_cntl |= RADEON_TVCLK_ALWAYS_ONb;

    if (tvout->tvStd == TV_STD_NTSC ||
	tvout->tvStd == TV_STD_NTSC_J)
	save->tv_master_cntl |= RADEON_RESTART_PHASE_FIX;

    save->tv_modulator_cntl1 = RADEON_SLEW_RATE_LIMIT
	                       | RADEON_SYNC_TIP_LEVEL
	                       | RADEON_YFLT_EN
	                       | RADEON_UVFLT_EN
	                       | (6 << RADEON_CY_FILT_BLEND_SHIFT);

    if (tvout->tvStd == TV_STD_NTSC ||
	tvout->tvStd == TV_STD_NTSC_J) {
	save->tv_modulator_cntl1 |= (0x46 << RADEON_SET_UP_LEVEL_SHIFT)
	                            | (0x3b << RADEON_BLANK_LEVEL_SHIFT);
	save->tv_modulator_cntl2 = (-111 & RADEON_TV_U_BURST_LEVEL_MASK) |
	    ((0 & RADEON_TV_V_BURST_LEVEL_MASK) << RADEON_TV_V_BURST_LEVEL_SHIFT);
    } else if (tvout->tvStd == TV_STD_SCART_PAL) {
	save->tv_modulator_cntl1 |= RADEON_ALT_PHASE_EN;
	save->tv_modulator_cntl2 = (0 & RADEON_TV_U_BURST_LEVEL_MASK) |
	    ((0 & RADEON_TV_V_BURST_LEVEL_MASK) << RADEON_TV_V_BURST_LEVEL_SHIFT);
    } else {
	save->tv_modulator_cntl1 |= RADEON_ALT_PHASE_EN
	                            | (0x3b << RADEON_SET_UP_LEVEL_SHIFT)
	                            | (0x3b << RADEON_BLANK_LEVEL_SHIFT);
	save->tv_modulator_cntl2 = (-78 & RADEON_TV_U_BURST_LEVEL_MASK) |
	    ((62 & RADEON_TV_V_BURST_LEVEL_MASK) << RADEON_TV_V_BURST_LEVEL_SHIFT);
    }

    save->pll_test_cntl = 0;

    save->tv_pre_dac_mux_cntl = (RADEON_Y_RED_EN
				 | RADEON_C_GRN_EN
				 | RADEON_CMP_BLU_EN
				 | RADEON_DAC_DITHER_EN);

    save->tv_rgb_cntl = (RADEON_RGB_DITHER_EN
			 | RADEON_TVOUT_SCALE_EN
			 | (0x0b << RADEON_UVRAM_READ_MARGIN_SHIFT)
			 | (0x07 << RADEON_FIFORAM_FFMACRO_READ_MARGIN_SHIFT)
			 | RADEON_RGB_ATTEN_SEL(0x3)
			 | RADEON_RGB_ATTEN_VAL(0xc));

    if (IsPrimary) {
	if (radeon_output->Flags & RADEON_USE_RMX)
	    save->tv_rgb_cntl |= RADEON_RGB_SRC_SEL_RMX;
	else
	    save->tv_rgb_cntl |= RADEON_RGB_SRC_SEL_CRTC1;
    } else {
	save->tv_rgb_cntl |= RADEON_RGB_SRC_SEL_CRTC2;
    }

    save->tv_sync_cntl = RADEON_SYNC_PUB | RADEON_TV_SYNC_IO_DRIVE;

    save->tv_sync_size = constPtr->horResolution + 8;

    if (tvout->tvStd == TV_STD_NTSC ||
	tvout->tvStd == TV_STD_NTSC_J ||
	tvout->tvStd == TV_STD_PAL_M ||
	tvout->tvStd == TV_STD_PAL_60)
	vert_space = constPtr->verTotal * 2 * 10000 / NTSC_TV_LINES_PER_FRAME;
    else
	vert_space = constPtr->verTotal * 2 * 10000 / PAL_TV_LINES_PER_FRAME;

    save->tv_vscaler_cntl1 = RADEON_Y_W_EN;
    save->tv_vscaler_cntl1 =
	(save->tv_vscaler_cntl1 & 0xe3ff0000) | (vert_space * (1 << FRAC_BITS) / 10000);

    if (pll->reference_freq == 2700)
	save->tv_vscaler_cntl1 |= RADEON_RESTART_FIELD;

    if (constPtr->horResolution == 1024)
	save->tv_vscaler_cntl1 |= (4 << RADEON_Y_DEL_W_SIG_SHIFT);
    else
	save->tv_vscaler_cntl1 |= (2 << RADEON_Y_DEL_W_SIG_SHIFT);

    if (tvout->tvStd == TV_STD_NTSC ||
        tvout->tvStd == TV_STD_NTSC_J ||
        tvout->tvStd == TV_STD_PAL_M ||
        tvout->tvStd == TV_STD_PAL_60)
	flicker_removal =
	    (float) constPtr->verTotal * 2.0 / NTSC_TV_LINES_PER_FRAME + 0.5;
    else
	flicker_removal =
	    (float) constPtr->verTotal * 2.0 / PAL_TV_LINES_PER_FRAME + 0.5;

    if (flicker_removal < 3)
	flicker_removal = 3;
    for (i = 0; i < 6; ++i) {
	if (flicker_removal == SLOPE_limit[i])
	    break;
    }
    save->tv_y_saw_tooth_cntl =
	(vert_space * SLOPE_value[i] * (1 << (FRAC_BITS - 1)) + 5001) / 10000 / 8
	| ((SLOPE_value[i] * (1 << (FRAC_BITS - 1)) / 8) << 16);
    save->tv_y_fall_cntl =
	(YCOEF_EN_value[i] << 17) | ((YCOEF_value[i] * (1 << 8) / 8) << 24) |
	RADEON_Y_FALL_PING_PONG | (272 * SLOPE_value[i] / 8) * (1 << (FRAC_BITS - 1)) /
	1024;
    save->tv_y_rise_cntl =
	RADEON_Y_RISE_PING_PONG
	| (flicker_removal * 1024 - 272) * SLOPE_value[i] / 8 * (1 << (FRAC_BITS - 1)) / 1024;

    save->tv_vscaler_cntl2 = ((save->tv_vscaler_cntl2 & 0x00fffff0)
			      | (0x10 << 24)
			      | RADEON_DITHER_MODE
			      | RADEON_Y_OUTPUT_DITHER_EN
			      | RADEON_UV_OUTPUT_DITHER_EN
			      | RADEON_UV_TO_BUF_DITHER_EN);

    tmp = (save->tv_vscaler_cntl1 >> RADEON_UV_INC_SHIFT) & RADEON_UV_INC_MASK;
    tmp = ((16384 * 256 * 10) / tmp + 5) / 10;
    tmp = (tmp << RADEON_UV_OUTPUT_POST_SCALE_SHIFT) | 0x000b0000;
    save->tv_timing_cntl = tmp;

    if (tvout->tvStd == TV_STD_NTSC ||
        tvout->tvStd == TV_STD_NTSC_J ||
        tvout->tvStd == TV_STD_PAL_M ||
        tvout->tvStd == TV_STD_PAL_60)
	save->tv_dac_cntl = tvdac->ntsc_tvdac_adj;
    else
	save->tv_dac_cntl = tvdac->pal_tvdac_adj;

    save->tv_dac_cntl |= (RADEON_TV_DAC_NBLANK | RADEON_TV_DAC_NHOLD);

    if (tvout->tvStd == TV_STD_NTSC ||
	tvout->tvStd == TV_STD_NTSC_J)
	save->tv_dac_cntl |= RADEON_TV_DAC_STD_NTSC;
    else
	save->tv_dac_cntl |= RADEON_TV_DAC_STD_PAL;

#if 0
    /* needs fixes for r4xx */
    save->tv_dac_cntl |= (RADEON_TV_DAC_RDACPD | RADEON_TV_DAC_GDACPD
	                 | RADEON_TV_DAC_BDACPD);

    if (radeon_output->MonType == MT_CTV) {
	save->tv_dac_cntl &= ~RADEON_TV_DAC_BDACPD;
    }

    if (radeon_output->MonType == MT_STV) {
	save->tv_dac_cntl &= ~(RADEON_TV_DAC_RDACPD |
			       RADEON_TV_DAC_GDACPD);
    }
#endif

    if (tvout->tvStd == TV_STD_NTSC ||
	tvout->tvStd == TV_STD_NTSC_J) {
	if (pll->reference_freq == 2700) {
	    m = NTSC_TV_PLL_M_27;
	    n = NTSC_TV_PLL_N_27;
	    p = NTSC_TV_PLL_P_27;
	} else {
	    m = NTSC_TV_PLL_M_14;
	    n = NTSC_TV_PLL_N_14;
	    p = NTSC_TV_PLL_P_14;
	}
    } else {
	if (pll->reference_freq == 2700) {
	    m = PAL_TV_PLL_M_27;
	    n = PAL_TV_PLL_N_27;
	    p = PAL_TV_PLL_P_27;
	} else {
	    m = PAL_TV_PLL_M_14;
	    n = PAL_TV_PLL_N_14;
	    p = PAL_TV_PLL_P_14;
	}
    }
    save->tv_pll_cntl = (m & RADEON_TV_M0LO_MASK) |
	(((m >> 8) & RADEON_TV_M0HI_MASK) << RADEON_TV_M0HI_SHIFT) |
	((n & RADEON_TV_N0LO_MASK) << RADEON_TV_N0LO_SHIFT) |
	(((n >> 9) & RADEON_TV_N0HI_MASK) << RADEON_TV_N0HI_SHIFT) |
	((p & RADEON_TV_P_MASK) << RADEON_TV_P_SHIFT);

    save->tv_pll_cntl1 =  (((4 & RADEON_TVPCP_MASK)<< RADEON_TVPCP_SHIFT) |
			   ((4 & RADEON_TVPVG_MASK) << RADEON_TVPVG_SHIFT) |
			   ((1 & RADEON_TVPDC_MASK)<< RADEON_TVPDC_SHIFT) |
			   RADEON_TVCLK_SRC_SEL_TVPLL |
			   RADEON_TVPLL_TEST_DIS);

    save->tv_upsamp_and_gain_cntl = RADEON_YUPSAMP_EN | RADEON_UVUPSAMP_EN;

    save->tv_uv_adr = 0xc8;

    save->tv_vdisp = constPtr->verResolution - 1;

    if (tvout->tvStd == TV_STD_NTSC ||
        tvout->tvStd == TV_STD_NTSC_J ||
        tvout->tvStd == TV_STD_PAL_M ||
        tvout->tvStd == TV_STD_PAL_60)
	save->tv_ftotal = NTSC_TV_VFTOTAL;
    else
	save->tv_ftotal = PAL_TV_VFTOTAL;

    save->tv_vtotal = constPtr->verTotal - 1;

    if (tvout->tvStd == TV_STD_NTSC ||
	tvout->tvStd == TV_STD_NTSC_J ||
	tvout->tvStd == TV_STD_PAL_M) {
	hor_timing = hor_timing_NTSC;
    } else {
	hor_timing = hor_timing_PAL;
    }

    if (tvout->tvStd == TV_STD_NTSC ||
	tvout->tvStd == TV_STD_NTSC_J ||
	tvout->tvStd == TV_STD_PAL_M ||
	tvout->tvStd == TV_STD_PAL_60) {
	vert_timing = vert_timing_NTSC;
    } else {
	vert_timing = vert_timing_PAL;
    }

    for (i = 0; i < MAX_H_CODE_TIMING_LEN; i++) {
	if ((save->h_code_timing[ i ] = hor_timing[ i ]) == 0)
	    break;
    }

    for (i = 0; i < MAX_V_CODE_TIMING_LEN; i++) {
	if ((save->v_code_timing[ i ] = vert_timing[ i ]) == 0)
	    break;
    }

    /*
     * This must be called AFTER loading timing tables as they are modified by this function
     */
    RADEONInitTVRestarts(output, save, mode);

    save->dac_cntl &= ~RADEON_DAC_TVO_EN;

    if (IS_R300_VARIANT)
        save->gpiopad_a = info->SavedReg->gpiopad_a & ~1;

    if (IsPrimary) {
	save->disp_output_cntl &= ~RADEON_DISP_TVDAC_SOURCE_MASK;
	save->disp_output_cntl |= (RADEON_DISP_TVDAC_SOURCE_CRTC
				   | RADEON_DISP_TV_SOURCE_CRTC);
    	if (info->ChipFamily >= CHIP_FAMILY_R200) {
	    save->disp_tv_out_cntl &= ~RADEON_DISP_TV_PATH_SRC_CRTC2;
    	} else {
            save->disp_hw_debug |= RADEON_CRT2_DISP1_SEL;
    	}
    } else {
	save->disp_output_cntl &= ~RADEON_DISP_DAC_SOURCE_MASK;
	save->disp_output_cntl |= RADEON_DISP_TV_SOURCE_CRTC;

    	if (info->ChipFamily >= CHIP_FAMILY_R200) {
	    save->disp_tv_out_cntl |= RADEON_DISP_TV_PATH_SRC_CRTC2;
    	} else {
            save->disp_hw_debug &= ~RADEON_CRT2_DISP1_SEL;
    	}
    }
}


/* Set hw registers for a new h/v position & h size */
void RADEONUpdateHVPosition(xf86OutputPtr output, DisplayModePtr mode)
{
    ScrnInfoPtr pScrn = output->scrn;
    RADEONInfoPtr  info = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    Bool reloadTable;
    RADEONSavePtr restore = info->ModeReg;

    reloadTable = RADEONInitTVRestarts(output, restore, mode);

    RADEONRestoreTVRestarts(pScrn, restore);

    OUTREG(RADEON_TV_TIMING_CNTL, restore->tv_timing_cntl);

    if (reloadTable) {
	 OUTREG(RADEON_TV_MASTER_CNTL, restore->tv_master_cntl
		                       | RADEON_TV_ASYNC_RST
		                       | RADEON_CRT_ASYNC_RST
		                       | RADEON_RESTART_PHASE_FIX);

	RADEONRestoreTVTimingTables(pScrn, restore);

	OUTREG(RADEON_TV_MASTER_CNTL, restore->tv_master_cntl);
    }
}

void RADEONAdjustCrtcRegistersForTV(ScrnInfoPtr pScrn, RADEONSavePtr save,
				    DisplayModePtr mode, xf86OutputPtr output)
{
    const TVModeConstants *constPtr;
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    radeon_tvout_ptr tvout = &radeon_output->tvout;
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    RADEONPLLPtr pll = &info->pll;

    /* FIXME: need to revisit this when we add more modes */
    if (tvout->tvStd == TV_STD_NTSC ||
	tvout->tvStd == TV_STD_NTSC_J ||
	tvout->tvStd == TV_STD_PAL_M) {
	if (pll->reference_freq == 2700)
	    constPtr = &availableTVModes[0];
	else
	    constPtr = &availableTVModes[2];
    } else {
	if (pll->reference_freq == 2700)
	    constPtr = &availableTVModes[1];
	else
	    constPtr = &availableTVModes[3];
    }

    save->crtc_h_total_disp = (((constPtr->horResolution / 8) - 1) << RADEON_CRTC_H_DISP_SHIFT) |
	(((constPtr->horTotal / 8) - 1) << RADEON_CRTC_H_TOTAL_SHIFT);

    save->crtc_h_sync_strt_wid = (save->crtc_h_sync_strt_wid 
				  & ~(RADEON_CRTC_H_SYNC_STRT_PIX | RADEON_CRTC_H_SYNC_STRT_CHAR)) |
	(((constPtr->horSyncStart / 8) - 1) << RADEON_CRTC_H_SYNC_STRT_CHAR_SHIFT) |
	(constPtr->horSyncStart & 7);

    save->crtc_v_total_disp = ((constPtr->verResolution - 1) << RADEON_CRTC_V_DISP_SHIFT) |
	((constPtr->verTotal - 1) << RADEON_CRTC_V_TOTAL_SHIFT);

    save->crtc_v_sync_strt_wid = (save->crtc_v_sync_strt_wid & ~RADEON_CRTC_V_SYNC_STRT) |
	((constPtr->verSyncStart - 1) << RADEON_CRTC_V_SYNC_STRT_SHIFT);

}

void RADEONAdjustPLLRegistersForTV(ScrnInfoPtr pScrn, RADEONSavePtr save,
				   DisplayModePtr mode, xf86OutputPtr output)
{
    unsigned postDiv;
    const TVModeConstants *constPtr;
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    radeon_tvout_ptr tvout = &radeon_output->tvout;
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    RADEONPLLPtr pll = &info->pll;

    /* FIXME: need to revisit this when we add more modes */
    if (tvout->tvStd == TV_STD_NTSC ||
	tvout->tvStd == TV_STD_NTSC_J ||
	tvout->tvStd == TV_STD_PAL_M) {
	if (pll->reference_freq == 2700)
	    constPtr = &availableTVModes[0];
	else
	    constPtr = &availableTVModes[2];
    } else {
	if (pll->reference_freq == 2700)
	    constPtr = &availableTVModes[1];
	else
	    constPtr = &availableTVModes[3];
    }

    save->htotal_cntl = (constPtr->horTotal & 0x7 /*0xf*/) | RADEON_HTOT_CNTL_VGA_EN;

    save->ppll_ref_div = constPtr->crtcPLL_M;

    switch (constPtr->crtcPLL_postDiv) {
    case 1:
	postDiv = 0;
	break;
    case 2:
	postDiv = 1;
	break;
    case 3:
	postDiv = 4;
	break;
    case 4:
	postDiv = 2;
	break;
    case 6:
	postDiv = 6;
	break;
    case 8:
	postDiv = 3;
	break;
    case 12:
	postDiv = 7;
	break;
    case 16:
    default:
	postDiv = 5;
	break;
    }

    save->ppll_div_3 = (constPtr->crtcPLL_N & 0x7ff) | (postDiv << 16);

    save->pixclks_cntl &= ~(RADEON_PIX2CLK_SRC_SEL_MASK | RADEON_PIXCLK_TV_SRC_SEL);
    save->pixclks_cntl |= RADEON_PIX2CLK_SRC_SEL_P2PLLCLK;

}

void RADEONAdjustCrtc2RegistersForTV(ScrnInfoPtr pScrn, RADEONSavePtr save,
				     DisplayModePtr mode, xf86OutputPtr output)
{
    const TVModeConstants *constPtr;
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    radeon_tvout_ptr tvout = &radeon_output->tvout;
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    RADEONPLLPtr pll = &info->pll;

    /* FIXME: need to revisit this when we add more modes */
    if (tvout->tvStd == TV_STD_NTSC ||
	tvout->tvStd == TV_STD_NTSC_J ||
	tvout->tvStd == TV_STD_PAL_M) {
	if (pll->reference_freq == 2700)
	    constPtr = &availableTVModes[0];
	else
	    constPtr = &availableTVModes[2];
    } else {
	if (pll->reference_freq == 2700)
	    constPtr = &availableTVModes[1];
	else
	    constPtr = &availableTVModes[3];
    }

    save->crtc2_h_total_disp = (((constPtr->horResolution / 8) - 1) << RADEON_CRTC_H_DISP_SHIFT) |
	(((constPtr->horTotal / 8) - 1) << RADEON_CRTC_H_TOTAL_SHIFT);

    save->crtc2_h_sync_strt_wid = (save->crtc2_h_sync_strt_wid 
				  & ~(RADEON_CRTC_H_SYNC_STRT_PIX | RADEON_CRTC_H_SYNC_STRT_CHAR)) |
	(((constPtr->horSyncStart / 8) - 1) << RADEON_CRTC_H_SYNC_STRT_CHAR_SHIFT) |
	(constPtr->horSyncStart & 7);

    save->crtc2_v_total_disp = ((constPtr->verResolution - 1) << RADEON_CRTC_V_DISP_SHIFT) |
	((constPtr->verTotal - 1) << RADEON_CRTC_V_TOTAL_SHIFT);

    save->crtc2_v_sync_strt_wid = (save->crtc2_v_sync_strt_wid & ~RADEON_CRTC_V_SYNC_STRT) |
	((constPtr->verSyncStart - 1) << RADEON_CRTC_V_SYNC_STRT_SHIFT);

}

void RADEONAdjustPLL2RegistersForTV(ScrnInfoPtr pScrn, RADEONSavePtr save,
				    DisplayModePtr mode, xf86OutputPtr output)
{
    unsigned postDiv;
    const TVModeConstants *constPtr;
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    radeon_tvout_ptr tvout = &radeon_output->tvout;
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    RADEONPLLPtr pll = &info->pll;

    /* FIXME: need to revisit this when we add more modes */
    if (tvout->tvStd == TV_STD_NTSC ||
	tvout->tvStd == TV_STD_NTSC_J ||
	tvout->tvStd == TV_STD_PAL_M) {
	if (pll->reference_freq == 2700)
	    constPtr = &availableTVModes[0];
	else
	    constPtr = &availableTVModes[2];
    } else {
	if (pll->reference_freq == 2700)
	    constPtr = &availableTVModes[1];
	else
	    constPtr = &availableTVModes[3];
    }

    save->htotal_cntl2 = (constPtr->horTotal & 0x7); /* 0xf */

    save->p2pll_ref_div = constPtr->crtcPLL_M;

    switch (constPtr->crtcPLL_postDiv) {
    case 1:
	postDiv = 0;
	break;
    case 2:
	postDiv = 1;
	break;
    case 3:
	postDiv = 4;
	break;
    case 4:
	postDiv = 2;
	break;
    case 6:
	postDiv = 6;
	break;
    case 8:
	postDiv = 3;
	break;
    case 12:
	postDiv = 7;
	break;
    case 16:
    default:
	postDiv = 5;
	break;
    }

    save->p2pll_div_0 = (constPtr->crtcPLL_N & 0x7ff) | (postDiv << 16);

    save->pixclks_cntl &= ~RADEON_PIX2CLK_SRC_SEL_MASK;
    save->pixclks_cntl |= (RADEON_PIX2CLK_SRC_SEL_P2PLLCLK
			   | RADEON_PIXCLK_TV_SRC_SEL);

}
