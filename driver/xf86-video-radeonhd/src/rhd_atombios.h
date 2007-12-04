/*
 * Copyright 2007  Egbert Eich   <eich@novell.com>
 * Copyright 2007  Luc Verhaegen <lverhaegen@novell.com>
 * Copyright 2007  Matthias Hopf <mhopf@novell.com>
 * Copyright 2007  Advanced Micro Devices, Inc.
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


#ifndef RHD_ATOMBIOS_H_
# define RHD_ATOMBIOS_H_

# ifdef ATOM_BIOS

typedef enum _AtomBiosRequestID {
    ATOMBIOS_INIT,
    ATOMBIOS_TEARDOWN,
# ifdef ATOM_BIOS_PARSER
    ATOMBIOS_EXEC,
#endif
    ATOMBIOS_ALLOCATE_FB_SCRATCH,
    ATOMBIOS_GET_CONNECTORS,
    ATOMBIOS_GET_PANEL_MODE,
    ATOMBIOS_GET_PANEL_EDID,
    GET_DEFAULT_ENGINE_CLOCK,
    GET_DEFAULT_MEMORY_CLOCK,
    GET_MAX_PIXEL_CLOCK_PLL_OUTPUT,
    GET_MIN_PIXEL_CLOCK_PLL_OUTPUT,
    GET_MAX_PIXEL_CLOCK_PLL_INPUT,
    GET_MIN_PIXEL_CLOCK_PLL_INPUT,
    GET_MAX_PIXEL_CLK,
    GET_REF_CLOCK,
    GET_FW_FB_START,
    GET_FW_FB_SIZE,
    ATOM_TMDS_FREQUENCY,
    ATOM_TMDS_PLL_CHARGE_PUMP,
    ATOM_TMDS_PLL_DUTY_CYCLE,
    ATOM_TMDS_PLL_VCO_GAIN,
    ATOM_TMDS_PLL_VOLTAGE_SWING,
    ATOM_LVDS_SUPPORTED_REFRESH_RATE,
    ATOM_LVDS_OFF_DELAY,
    ATOM_LVDS_SEQ_DIG_ONTO_DE,
    ATOM_LVDS_SEQ_DE_TO_BL,
    ATOM_LVDS_DITHER,
    ATOM_LVDS_DUALLINK,
    ATOM_LVDS_24BIT,
    ATOM_LVDS_GREYLVL,
    ATOM_LVDS_FPDI,
    ATOM_GPIO_QUERIES,
    ATOM_GPIO_I2C_CLK_MASK,
    ATOM_DAC1_BG_ADJ,
    ATOM_DAC1_DAC_ADJ,
    ATOM_DAC1_FORCE,
    ATOM_DAC2_CRTC2_BG_ADJ,
    ATOM_DAC2_CRTC2_DAC_ADJ,
    ATOM_DAC2_CRTC2_FORCE,
    ATOM_DAC2_CRTC2_MUX_REG_IND,
    ATOM_DAC2_CRTC2_MUX_REG_INFO,
    FUNC_END
} AtomBiosRequestID;

typedef enum _AtomBiosResult {
    ATOM_SUCCESS,
    ATOM_FAILED,
    ATOM_NOT_IMPLEMENTED
} AtomBiosResult;

typedef struct AtomExec {
    int index;
    pointer pspace;
    pointer *dataSpace;
} AtomExecRec, *AtomExecPtr;

typedef struct AtomFb {
    unsigned int start;
    unsigned int size;
} AtomFbRec, *AtomFbPtr;

typedef union AtomBiosArg
{
    CARD32 val;
    struct rhdConnectorInfo	*connectorInfo;
    unsigned char*		EDIDBlock;
    atomBiosHandlePtr		atomhandle;
    DisplayModePtr		mode;
    AtomExecRec			exec;
    AtomFbRec			fb;
} AtomBiosArgRec, *AtomBiosArgPtr;

extern AtomBiosResult
RHDAtomBiosFunc(int scrnIndex, atomBiosHandlePtr handle,
		AtomBiosRequestID id, AtomBiosArgPtr data);
# endif

#endif /*  RHD_ATOMBIOS_H_ */
