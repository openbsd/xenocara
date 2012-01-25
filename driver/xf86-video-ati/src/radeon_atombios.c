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

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif
#include "xf86.h"
#include "xf86_OSproc.h"

#include "radeon.h"
#include "radeon_reg.h"
#include "radeon_atombios.h"
#include "radeon_atomwrapper.h"
#include "radeon_probe.h"
#include "radeon_macros.h"

#include "ati_pciids_gen.h"

#include "xorg-server.h"

/* only for testing now */
#include "xf86DDC.h"

typedef AtomBiosResult (*AtomBiosRequestFunc)(atomBiosHandlePtr handle,
					  AtomBiosRequestID unused, AtomBiosArgPtr data);
typedef struct rhdConnectorInfo *rhdConnectorInfoPtr;

static AtomBiosResult rhdAtomInit(atomBiosHandlePtr unused1,
				      AtomBiosRequestID unused2, AtomBiosArgPtr data);
static AtomBiosResult rhdAtomTearDown(atomBiosHandlePtr handle,
					  AtomBiosRequestID unused1, AtomBiosArgPtr unused2);
static AtomBiosResult rhdAtomVramInfoQuery(atomBiosHandlePtr handle,
					       AtomBiosRequestID func, AtomBiosArgPtr data);
static AtomBiosResult rhdAtomTmdsInfoQuery(atomBiosHandlePtr handle,
					       AtomBiosRequestID func, AtomBiosArgPtr data);
static AtomBiosResult rhdAtomAllocateFbScratch(atomBiosHandlePtr handle,
						   AtomBiosRequestID func, AtomBiosArgPtr data);
static AtomBiosResult rhdAtomLvdsGetTimings(atomBiosHandlePtr handle,
					AtomBiosRequestID unused, AtomBiosArgPtr data);
static AtomBiosResult rhdAtomCVGetTimings(atomBiosHandlePtr handle,
					  AtomBiosRequestID unused, AtomBiosArgPtr data);
static AtomBiosResult rhdAtomLvdsInfoQuery(atomBiosHandlePtr handle,
					       AtomBiosRequestID func,  AtomBiosArgPtr data);
static AtomBiosResult rhdAtomGPIOI2CInfoQuery(atomBiosHandlePtr handle,
						  AtomBiosRequestID func, AtomBiosArgPtr data);
static AtomBiosResult rhdAtomFirmwareInfoQuery(atomBiosHandlePtr handle,
						   AtomBiosRequestID func, AtomBiosArgPtr data);
/*static AtomBiosResult rhdAtomConnectorInfo(atomBiosHandlePtr handle,
  AtomBiosRequestID unused, AtomBiosArgPtr data);*/
# ifdef ATOM_BIOS_PARSER
static AtomBiosResult rhdAtomExec(atomBiosHandlePtr handle,
				   AtomBiosRequestID unused, AtomBiosArgPtr data);
# endif
static AtomBiosResult
rhdAtomCompassionateDataQuery(atomBiosHandlePtr handle,
			      AtomBiosRequestID func, AtomBiosArgPtr data);


static void
RADEONGetATOMLVDSInfo(ScrnInfoPtr pScrn, radeon_lvds_ptr lvds);


enum msgDataFormat {
    MSG_FORMAT_NONE,
    MSG_FORMAT_HEX,
    MSG_FORMAT_DEC
};

struct atomBIOSRequests {
    AtomBiosRequestID id;
    AtomBiosRequestFunc request;
    char *message;
    enum msgDataFormat message_format;
} AtomBiosRequestList [] = {
    {ATOMBIOS_INIT,			rhdAtomInit,
     "AtomBIOS Init",				MSG_FORMAT_NONE},
    {ATOMBIOS_TEARDOWN,			rhdAtomTearDown,
     "AtomBIOS Teardown",			MSG_FORMAT_NONE},
# ifdef ATOM_BIOS_PARSER
    {ATOMBIOS_EXEC,			rhdAtomExec,
     "AtomBIOS Exec",				MSG_FORMAT_NONE},
#endif
    {ATOMBIOS_ALLOCATE_FB_SCRATCH,	rhdAtomAllocateFbScratch,
     "AtomBIOS Set FB Space",			MSG_FORMAT_NONE},
    /*{ATOMBIOS_GET_CONNECTORS,		rhdAtomConnectorInfo,
      "AtomBIOS Get Connectors",			MSG_FORMAT_NONE},*/
    {ATOMBIOS_GET_PANEL_MODE,		rhdAtomLvdsGetTimings,
     "AtomBIOS Get Panel Mode",			MSG_FORMAT_NONE},
    {ATOMBIOS_GET_PANEL_EDID,		rhdAtomLvdsGetTimings,
     "AtomBIOS Get Panel EDID",			MSG_FORMAT_NONE},
    {GET_DEFAULT_ENGINE_CLOCK,		rhdAtomFirmwareInfoQuery,
     "Default Engine Clock",			MSG_FORMAT_DEC},
    {GET_DEFAULT_MEMORY_CLOCK,		rhdAtomFirmwareInfoQuery,
     "Default Memory Clock",			MSG_FORMAT_DEC},
    {GET_MAX_PIXEL_CLOCK_PLL_OUTPUT,	rhdAtomFirmwareInfoQuery,
     "Maximum Pixel ClockPLL Frequency Output", MSG_FORMAT_DEC},
    {GET_MIN_PIXEL_CLOCK_PLL_OUTPUT,	rhdAtomFirmwareInfoQuery,
     "Minimum Pixel ClockPLL Frequency Output", MSG_FORMAT_DEC},
    {GET_MAX_PIXEL_CLOCK_PLL_INPUT,	rhdAtomFirmwareInfoQuery,
     "Maximum Pixel ClockPLL Frequency Input", MSG_FORMAT_DEC},
    {GET_MIN_PIXEL_CLOCK_PLL_INPUT,	rhdAtomFirmwareInfoQuery,
     "Minimum Pixel ClockPLL Frequency Input", MSG_FORMAT_DEC},
    {GET_MAX_PIXEL_CLK,			rhdAtomFirmwareInfoQuery,
     "Maximum Pixel Clock",			MSG_FORMAT_DEC},
    {GET_REF_CLOCK,			rhdAtomFirmwareInfoQuery,
     "Reference Clock",				MSG_FORMAT_DEC},
    {GET_FW_FB_START,			rhdAtomVramInfoQuery,
      "Start of VRAM area used by Firmware",	MSG_FORMAT_HEX},
    {GET_FW_FB_SIZE,			rhdAtomVramInfoQuery,
      "Framebuffer space used by Firmware (kb)", MSG_FORMAT_DEC},
    {ATOM_TMDS_FREQUENCY,		rhdAtomTmdsInfoQuery,
     "TMDS Frequency",				MSG_FORMAT_DEC},
    {ATOM_TMDS_PLL_CHARGE_PUMP,		rhdAtomTmdsInfoQuery,
     "TMDS PLL ChargePump",			MSG_FORMAT_DEC},
    {ATOM_TMDS_PLL_DUTY_CYCLE,		rhdAtomTmdsInfoQuery,
     "TMDS PLL DutyCycle",			MSG_FORMAT_DEC},
    {ATOM_TMDS_PLL_VCO_GAIN,		rhdAtomTmdsInfoQuery,
     "TMDS PLL VCO Gain",			MSG_FORMAT_DEC},
    {ATOM_TMDS_PLL_VOLTAGE_SWING,	rhdAtomTmdsInfoQuery,
     "TMDS PLL VoltageSwing",			MSG_FORMAT_DEC},
    {ATOM_LVDS_SUPPORTED_REFRESH_RATE,	rhdAtomLvdsInfoQuery,
     "LVDS Supported Refresh Rate",		MSG_FORMAT_DEC},
    {ATOM_LVDS_OFF_DELAY,		rhdAtomLvdsInfoQuery,
     "LVDS Off Delay",				MSG_FORMAT_DEC},
    {ATOM_LVDS_SEQ_DIG_ONTO_DE,		rhdAtomLvdsInfoQuery,
     "LVDS SEQ Dig onto DE",			MSG_FORMAT_DEC},
    {ATOM_LVDS_SEQ_DE_TO_BL,		rhdAtomLvdsInfoQuery,
     "LVDS SEQ DE to BL",			MSG_FORMAT_DEC},
    {ATOM_LVDS_DITHER,			rhdAtomLvdsInfoQuery,
     "LVDS Ditherc",				MSG_FORMAT_HEX},
    {ATOM_LVDS_DUALLINK,		rhdAtomLvdsInfoQuery,
     "LVDS Duallink",				MSG_FORMAT_HEX},
    {ATOM_LVDS_GREYLVL,			rhdAtomLvdsInfoQuery,
     "LVDS Grey Level",				MSG_FORMAT_HEX},
    {ATOM_LVDS_FPDI,			rhdAtomLvdsInfoQuery,
     "LVDS FPDI",				MSG_FORMAT_HEX},
    {ATOM_LVDS_24BIT,			rhdAtomLvdsInfoQuery,
     "LVDS 24Bit",				MSG_FORMAT_HEX},
    {ATOM_GPIO_I2C_CLK_MASK,		rhdAtomGPIOI2CInfoQuery,
     "GPIO_I2C_Clk_Mask",			MSG_FORMAT_HEX},
    {ATOM_DAC1_BG_ADJ,		rhdAtomCompassionateDataQuery,
     "DAC1 BG Adjustment",			MSG_FORMAT_HEX},
    {ATOM_DAC1_DAC_ADJ,		rhdAtomCompassionateDataQuery,
     "DAC1 DAC Adjustment",			MSG_FORMAT_HEX},
    {ATOM_DAC1_FORCE,		rhdAtomCompassionateDataQuery,
     "DAC1 Force Data",				MSG_FORMAT_HEX},
    {ATOM_DAC2_CRTC2_BG_ADJ,	rhdAtomCompassionateDataQuery,
     "DAC2_CRTC2 BG Adjustment",		MSG_FORMAT_HEX},
    {ATOM_DAC2_CRTC2_DAC_ADJ,	rhdAtomCompassionateDataQuery,
     "DAC2_CRTC2 DAC Adjustment",		MSG_FORMAT_HEX},
    {ATOM_DAC2_CRTC2_FORCE,	rhdAtomCompassionateDataQuery,
     "DAC2_CRTC2 Force",			MSG_FORMAT_HEX},
    {ATOM_DAC2_CRTC2_MUX_REG_IND,rhdAtomCompassionateDataQuery,
     "DAC2_CRTC2 Mux Register Index",		MSG_FORMAT_HEX},
    {ATOM_DAC2_CRTC2_MUX_REG_INFO,rhdAtomCompassionateDataQuery,
     "DAC2_CRTC2 Mux Register Info",		MSG_FORMAT_HEX},
    {ATOMBIOS_GET_CV_MODES,		rhdAtomCVGetTimings,
     "AtomBIOS Get CV Mode",			MSG_FORMAT_NONE},
    {FUNC_END,					NULL,
     NULL,					MSG_FORMAT_NONE}
};

enum {
    legacyBIOSLocation = 0xC0000,
    legacyBIOSMax = 0x10000
};

#define DEBUGP(x) {x;}
#define LOG_DEBUG 7

#  ifdef ATOM_BIOS_PARSER

#   define LOG_CAIL LOG_DEBUG + 1

#if 0

static void
RHDDebug(int scrnIndex, const char *format, ...)
{
    va_list ap;

    va_start(ap, format);
    xf86VDrvMsgVerb(scrnIndex, X_INFO, LOG_DEBUG, format, ap);
    va_end(ap);
}

static void
RHDDebugCont(const char *format, ...)
{
    va_list ap;

    va_start(ap, format);
    xf86VDrvMsgVerb(-1, X_NONE, LOG_DEBUG, format, ap);
    va_end(ap);
}

#endif

static void
CailDebug(int scrnIndex, const char *format, ...)
{
    va_list ap;

    va_start(ap, format);
    xf86VDrvMsgVerb(scrnIndex, X_INFO, LOG_CAIL, format, ap);
    va_end(ap);
}
#   define CAILFUNC(ptr) \
  CailDebug(((atomBiosHandlePtr)(ptr))->scrnIndex, "CAIL: %s\n", __func__)

#  endif

static int
rhdAtomAnalyzeCommonHdr(ATOM_COMMON_TABLE_HEADER *hdr)
{
    if (le16_to_cpu(hdr->usStructureSize) == 0xaa55)
        return FALSE;

    return TRUE;
}

static int
rhdAtomAnalyzeRomHdr(unsigned char *rombase,
		     ATOM_ROM_HEADER *hdr,
		     unsigned int *data_offset, 
		     unsigned int *command_offset)
{
    if (!rhdAtomAnalyzeCommonHdr(&hdr->sHeader)) {
        return FALSE;
    }
    xf86DrvMsg(-1,X_NONE,"\tSubsystemVendorID: 0x%4.4x SubsystemID: 0x%4.4x\n",
               le16_to_cpu(hdr->usSubsystemVendorID),le16_to_cpu(hdr->usSubsystemID));
    xf86DrvMsg(-1,X_NONE,"\tIOBaseAddress: 0x%4.4x\n",le16_to_cpu(hdr->usIoBaseAddress));
    xf86DrvMsgVerb(-1,X_NONE,3,"\tFilename: %s\n",rombase + le16_to_cpu(hdr->usConfigFilenameOffset));
    xf86DrvMsgVerb(-1,X_NONE,3,"\tBIOS Bootup Message: %s\n",
		   rombase + le16_to_cpu(hdr->usBIOS_BootupMessageOffset));

    *data_offset = le16_to_cpu(hdr->usMasterDataTableOffset);
    *command_offset = le16_to_cpu(hdr->usMasterCommandTableOffset);

    return TRUE;
}

static int
rhdAtomAnalyzeRomDataTable(unsigned char *base, uint16_t offset,
                    void *ptr,unsigned short *size)
{
    ATOM_COMMON_TABLE_HEADER *table = (ATOM_COMMON_TABLE_HEADER *)
      (base + le16_to_cpu(offset));

   if (!*size || !rhdAtomAnalyzeCommonHdr(table)) {
       if (*size) *size -= 2;
       *(void **)ptr = NULL;
       return FALSE;
   }
   *size -= 2;
   *(void **)ptr = (void *)(table);
   return TRUE;
}

Bool
rhdAtomGetTableRevisionAndSize(ATOM_COMMON_TABLE_HEADER *hdr,
			       uint8_t *contentRev,
			       uint8_t *formatRev,
			       unsigned short *size)
{
    if (!hdr)
        return FALSE;

    if (contentRev) *contentRev = hdr->ucTableContentRevision;
    if (formatRev) *formatRev = hdr->ucTableFormatRevision;
    if (size) *size = (short)le16_to_cpu(hdr->usStructureSize)
                   - sizeof(ATOM_COMMON_TABLE_HEADER);
    return TRUE;
}

static Bool
rhdAtomAnalyzeMasterDataTable(unsigned char *base,
			      ATOM_MASTER_DATA_TABLE *table,
			      atomDataTablesPtr data)
{
    ATOM_MASTER_LIST_OF_DATA_TABLES *data_table =
        &table->ListOfDataTables;
    unsigned short size;

    if (!rhdAtomAnalyzeCommonHdr(&table->sHeader))
        return FALSE;
    if (!rhdAtomGetTableRevisionAndSize(&table->sHeader,NULL,NULL,
					&size))
        return FALSE;
# define SET_DATA_TABLE(x) {\
   rhdAtomAnalyzeRomDataTable(base,data_table->x,(void *)(&(data->x)),&size); \
    }

# define SET_DATA_TABLE_VERS(x) {\
   rhdAtomAnalyzeRomDataTable(base,data_table->x,&(data->x.base),&size); \
    }

    SET_DATA_TABLE(UtilityPipeLine);
    SET_DATA_TABLE(MultimediaCapabilityInfo);
    SET_DATA_TABLE(MultimediaConfigInfo);
    SET_DATA_TABLE(StandardVESA_Timing);
    SET_DATA_TABLE_VERS(FirmwareInfo);
    SET_DATA_TABLE(DAC_Info);
    SET_DATA_TABLE_VERS(LVDS_Info);
    SET_DATA_TABLE(TMDS_Info);
    SET_DATA_TABLE_VERS(AnalogTV_Info);
    SET_DATA_TABLE_VERS(SupportedDevicesInfo);
    SET_DATA_TABLE(GPIO_I2C_Info);
    SET_DATA_TABLE(VRAM_UsageByFirmware);
    SET_DATA_TABLE(GPIO_Pin_LUT);
    SET_DATA_TABLE(VESA_ToInternalModeLUT);
    SET_DATA_TABLE_VERS(ComponentVideoInfo);
    SET_DATA_TABLE(PowerPlayInfo);
    SET_DATA_TABLE(CompassionateData);
    SET_DATA_TABLE(SaveRestoreInfo);
    SET_DATA_TABLE(PPLL_SS_Info);
    SET_DATA_TABLE(OemInfo);
    SET_DATA_TABLE(XTMDS_Info);
    SET_DATA_TABLE(MclkSS_Info);
    SET_DATA_TABLE(Object_Header);
    SET_DATA_TABLE(IndirectIOAccess);
    SET_DATA_TABLE(MC_InitParameter);
    SET_DATA_TABLE(ASIC_VDDC_Info);
    SET_DATA_TABLE(ASIC_InternalSS_Info);
    SET_DATA_TABLE(TV_VideoMode);
    SET_DATA_TABLE_VERS(VRAM_Info);
    SET_DATA_TABLE(MemoryTrainingInfo);
    SET_DATA_TABLE_VERS(IntegratedSystemInfo);
    SET_DATA_TABLE(ASIC_ProfilingInfo);
    SET_DATA_TABLE(VoltageObjectInfo);
    SET_DATA_TABLE(PowerSourceInfo);
# undef SET_DATA_TABLE

    return TRUE;
}

static Bool
rhdAtomGetDataTable(int scrnIndex,
		    unsigned char *base,
		    atomDataTables *atomDataPtr,
		    unsigned int *cmd_offset,
		    unsigned int BIOSImageSize)
{
    unsigned int data_offset;
    unsigned int atom_romhdr_off =  le16_to_cpu(*(unsigned short*)
        (base + OFFSET_TO_POINTER_TO_ATOM_ROM_HEADER));
    ATOM_ROM_HEADER *atom_rom_hdr =
        (ATOM_ROM_HEADER *)(base + atom_romhdr_off);

    //RHDFUNCI(scrnIndex);

    if (atom_romhdr_off + sizeof(ATOM_ROM_HEADER) > BIOSImageSize) {
	xf86DrvMsg(scrnIndex,X_ERROR,
		   "%s: AtomROM header extends beyond BIOS image\n",__func__);
	return FALSE;
    }

    if (memcmp("ATOM",&atom_rom_hdr->uaFirmWareSignature,4)) {
        xf86DrvMsg(scrnIndex,X_ERROR,"%s: No AtomBios signature found\n",
		   __func__);
        return FALSE;
    }
    xf86DrvMsg(scrnIndex, X_INFO, "ATOM BIOS Rom: \n");
    if (!rhdAtomAnalyzeRomHdr(base, atom_rom_hdr, &data_offset, cmd_offset)) {
        xf86DrvMsg(scrnIndex, X_ERROR, "RomHeader invalid\n");
        return FALSE;
    }

    if (data_offset + sizeof (ATOM_MASTER_DATA_TABLE) > BIOSImageSize) {
	xf86DrvMsg(scrnIndex,X_ERROR,"%s: Atom data table outside of BIOS\n",
		   __func__);
    }

    if (*cmd_offset + sizeof (ATOM_MASTER_COMMAND_TABLE) > BIOSImageSize) {
	xf86DrvMsg(scrnIndex,X_ERROR,"%s: Atom command table outside of BIOS\n",
		   __func__);
    }

    if (!rhdAtomAnalyzeMasterDataTable(base, (ATOM_MASTER_DATA_TABLE *)
				       (base + data_offset),
				       atomDataPtr)) {
        xf86DrvMsg(scrnIndex, X_ERROR, "%s: ROM Master Table invalid\n",
		   __func__);
        return FALSE;
    }
    return TRUE;
}

static Bool
rhdAtomGetFbBaseAndSize(atomBiosHandlePtr handle, unsigned int *base,
			unsigned int *size)
{
    AtomBiosArgRec data;
    if (RHDAtomBiosFunc(handle->scrnIndex, handle, GET_FW_FB_SIZE, &data)
	== ATOM_SUCCESS) {
	if (data.val == 0) {
	    xf86DrvMsg(handle->scrnIndex, X_WARNING, "%s: AtomBIOS specified VRAM "
		       "scratch space size invalid\n", __func__);
	    return FALSE;
	}
	if (size)
	    *size = (int)data.val;
    } else
	return FALSE;
    if (RHDAtomBiosFunc(handle->scrnIndex, handle, GET_FW_FB_START, &data)
	== ATOM_SUCCESS) {
	if (data.val == 0)
	    return FALSE;
	if (base)
	    *base = (int)data.val;
    }
    return TRUE;
}

/*
 * Uses videoRam form ScrnInfoRec.
 */
static AtomBiosResult
rhdAtomAllocateFbScratch(atomBiosHandlePtr handle,
			 AtomBiosRequestID func, AtomBiosArgPtr data)
{
    unsigned int fb_base = 0;
    unsigned int fb_size = 0;
    unsigned int start = data->fb.start;
    unsigned int size = data->fb.size;
    handle->scratchBase = NULL;
    handle->fbBase = 0;

    if (rhdAtomGetFbBaseAndSize(handle, &fb_base, &fb_size)) {
	xf86DrvMsg(handle->scrnIndex, X_INFO, "AtomBIOS requests %ikB"
		   " of VRAM scratch space\n",fb_size);
	fb_size *= 1024; /* convert to bytes */
	xf86DrvMsg(handle->scrnIndex, X_INFO, "AtomBIOS VRAM scratch base: 0x%x\n",
		   fb_base);
    } else {
	    fb_size = 20 * 1024;
	    xf86DrvMsg(handle->scrnIndex, X_INFO, " default to: %i\n",fb_size);
    }
    if (fb_base && fb_size && size) {
	/* 4k align */
	fb_size = (fb_size & ~(uint32_t)0xfff) + ((fb_size & 0xfff) ? 1 : 0);
	if ((fb_base + fb_size) > (start + size)) {
	    xf86DrvMsg(handle->scrnIndex, X_WARNING,
		       "%s: FW FB scratch area %i (size: %i)"
		       " extends beyond available framebuffer size %i\n",
		       __func__, fb_base, fb_size, size);
	} else if ((fb_base + fb_size) < (start + size)) {
	    xf86DrvMsg(handle->scrnIndex, X_WARNING,
		       "%s: FW FB scratch area not located "
		       "at the end of VRAM. Scratch End: "
		       "0x%x VRAM End: 0x%x\n", __func__,
		       (unsigned int)(fb_base + fb_size),
		       size);
	} else if (fb_base < start) {
	    xf86DrvMsg(handle->scrnIndex, X_WARNING,
		       "%s: FW FB scratch area extends below "
		       "the base of the free VRAM: 0x%x Base: 0x%x\n",
		       __func__, (unsigned int)(fb_base), start);
	} else {
	    size -= fb_size;
	    handle->fbBase = fb_base;
	    return ATOM_SUCCESS;
	}
    }

    if (!handle->fbBase) {
	xf86DrvMsg(handle->scrnIndex, X_INFO,
		   "Cannot get VRAM scratch space. "
		   "Allocating in main memory instead\n");
	handle->scratchBase = calloc(fb_size,1);
	return ATOM_SUCCESS;
    }
    return ATOM_FAILED;
}

# ifdef ATOM_BIOS_PARSER
Bool
rhdAtomASICInit(atomBiosHandlePtr handle)
{
    ASIC_INIT_PS_ALLOCATION asicInit;
    AtomBiosArgRec data;

    RHDAtomBiosFunc(handle->scrnIndex, handle,
		    GET_DEFAULT_ENGINE_CLOCK,
		    &data);
    asicInit.sASICInitClocks.ulDefaultEngineClock = cpu_to_le32(data.val / 10);/*in 10 Khz*/
    RHDAtomBiosFunc(handle->scrnIndex, handle,
		    GET_DEFAULT_MEMORY_CLOCK,
		    &data);
    asicInit.sASICInitClocks.ulDefaultMemoryClock = cpu_to_le32(data.val / 10);/*in 10 Khz*/
    data.exec.dataSpace = NULL;
    data.exec.index = 0x0;
    data.exec.pspace = &asicInit;
    xf86DrvMsg(handle->scrnIndex, X_INFO, "Calling ASIC Init\n");
    if (RHDAtomBiosFunc(handle->scrnIndex, handle,
			ATOMBIOS_EXEC, &data) == ATOM_SUCCESS) {
	xf86DrvMsg(handle->scrnIndex, X_INFO, "ASIC_INIT Successful\n");
	return TRUE;
    }
    xf86DrvMsg(handle->scrnIndex, X_INFO, "ASIC_INIT Failed\n");
    return FALSE;
}

int
atombios_clk_gating_setup(ScrnInfoPtr pScrn, Bool enable)
{
    RADEONInfoPtr info       = RADEONPTR(pScrn);
    DYNAMIC_CLOCK_GATING_PS_ALLOCATION dynclk_data;
    AtomBiosArgRec data;
    unsigned char *space;

    dynclk_data.ucEnable = enable;

    data.exec.index = GetIndexIntoMasterTable(COMMAND, DynamicClockGating);
    data.exec.dataSpace = (void *)&space;
    data.exec.pspace = &dynclk_data;

    if (RHDAtomBiosFunc(info->atomBIOS->scrnIndex, info->atomBIOS, ATOMBIOS_EXEC, &data) == ATOM_SUCCESS) {
	ErrorF("Dynamic clock gating %s success\n", enable? "enable" : "disable");
	return ATOM_SUCCESS;
    }

    ErrorF("Dynamic clock gating %s failure\n", enable? "enable" : "disable");
    return ATOM_NOT_IMPLEMENTED;

}

int
atombios_static_pwrmgt_setup(ScrnInfoPtr pScrn, Bool enable)
{
    RADEONInfoPtr info       = RADEONPTR(pScrn);
    ENABLE_ASIC_STATIC_PWR_MGT_PS_ALLOCATION pwrmgt_data;
    AtomBiosArgRec data;
    unsigned char *space;

    /* disabling static power management causes hangs on some r4xx chips */
    if (((info->ChipFamily == CHIP_FAMILY_R420) ||
	 (info->ChipFamily == CHIP_FAMILY_RV410)) &&
	!enable)
	return ATOM_NOT_IMPLEMENTED;

    pwrmgt_data.ucEnable = enable;

    data.exec.index = GetIndexIntoMasterTable(COMMAND, EnableASIC_StaticPwrMgt);
    data.exec.dataSpace = (void *)&space;
    data.exec.pspace = &pwrmgt_data;

    if (RHDAtomBiosFunc(info->atomBIOS->scrnIndex, info->atomBIOS, ATOMBIOS_EXEC, &data) == ATOM_SUCCESS) {
	ErrorF("Static power management %s success\n", enable? "enable" : "disable");
	return ATOM_SUCCESS;
    }

    ErrorF("Static power management %s failure\n", enable? "enable" : "disable");
    return ATOM_NOT_IMPLEMENTED;

}

int
atombios_set_engine_clock(ScrnInfoPtr pScrn, uint32_t engclock)
{
    RADEONInfoPtr info       = RADEONPTR(pScrn);
    SET_ENGINE_CLOCK_PS_ALLOCATION eng_clock_ps;
    AtomBiosArgRec data;
    unsigned char *space;

    RADEONWaitForIdleMMIO(pScrn);

    eng_clock_ps.ulTargetEngineClock = engclock; /* 10 khz */

    /*ErrorF("Attempting to set engine clock to: %d\n", engclock);*/
    data.exec.index = GetIndexIntoMasterTable(COMMAND, SetEngineClock);
    data.exec.dataSpace = (void *)&space;
    data.exec.pspace = &eng_clock_ps;

    if (RHDAtomBiosFunc(info->atomBIOS->scrnIndex, info->atomBIOS, ATOMBIOS_EXEC, &data) == ATOM_SUCCESS) {
	/* ErrorF("Set engine clock success\n"); */
	return ATOM_SUCCESS;
    }
    /* ErrorF("Set engine clock failed\n"); */
    return ATOM_NOT_IMPLEMENTED;
}

int
atombios_set_memory_clock(ScrnInfoPtr pScrn, uint32_t memclock)
{
    RADEONInfoPtr info       = RADEONPTR(pScrn);
    SET_MEMORY_CLOCK_PS_ALLOCATION mem_clock_ps;
    AtomBiosArgRec data;
    unsigned char *space;

    if (info->IsIGP)
	return ATOM_SUCCESS;

    RADEONWaitForIdleMMIO(pScrn);

    mem_clock_ps.ulTargetMemoryClock = memclock; /* 10 khz */

    /* ErrorF("Attempting to set mem clock to: %d\n", memclock); */
    data.exec.index = GetIndexIntoMasterTable(COMMAND, SetMemoryClock);
    data.exec.dataSpace = (void *)&space;
    data.exec.pspace = &mem_clock_ps;

    if (RHDAtomBiosFunc(info->atomBIOS->scrnIndex, info->atomBIOS, ATOMBIOS_EXEC, &data) == ATOM_SUCCESS) {
	/* ErrorF("Set memory clock success\n"); */
	return ATOM_SUCCESS;
    }
    /* ErrorF("Set memory clock failed\n"); */
    return ATOM_NOT_IMPLEMENTED;
}

# endif

static AtomBiosResult
rhdAtomInit(atomBiosHandlePtr unused1, AtomBiosRequestID unused2,
		    AtomBiosArgPtr data)
{
    int scrnIndex = data->val;
    RADEONInfoPtr  info   = RADEONPTR(xf86Screens[scrnIndex]);
    atomDataTablesPtr atomDataPtr;
    unsigned int cmd_offset;
    atomBiosHandlePtr handle = NULL;
    unsigned int BIOSImageSize = 0;
    data->atomhandle = NULL;

#ifdef XSERVER_LIBPCIACCESS
    BIOSImageSize = info->PciInfo->rom_size > RADEON_VBIOS_SIZE ? info->PciInfo->rom_size : RADEON_VBIOS_SIZE;
#else
    BIOSImageSize = RADEON_VBIOS_SIZE;
#endif

    if (!(atomDataPtr = calloc(1, sizeof(atomDataTables)))) {
	xf86DrvMsg(scrnIndex,X_ERROR,"Cannot allocate memory for "
		   "ATOM BIOS data tabes\n");
	goto error;
    }
    if (!rhdAtomGetDataTable(scrnIndex, info->VBIOS, atomDataPtr, &cmd_offset, BIOSImageSize))
	goto error1;
    if (!(handle = calloc(1, sizeof(atomBiosHandleRec)))) {
	xf86DrvMsg(scrnIndex,X_ERROR,"Cannot allocate memory\n");
	goto error1;
    }
    handle->BIOSBase = info->VBIOS;
    handle->atomDataPtr = atomDataPtr;
    handle->cmd_offset = cmd_offset;
    handle->scrnIndex = scrnIndex;
#if XSERVER_LIBPCIACCESS
    handle->device = info->PciInfo;
#else
    handle->PciTag = info->PciTag;
#endif
    handle->BIOSImageSize = BIOSImageSize;

    data->atomhandle = handle;
    return ATOM_SUCCESS;

 error1:
    free(atomDataPtr);
 error:
    return ATOM_FAILED;
}

static AtomBiosResult
rhdAtomTearDown(atomBiosHandlePtr handle,
		AtomBiosRequestID unused1, AtomBiosArgPtr unused2)
{
    //RHDFUNC(handle);

    free(handle->BIOSBase);
    free(handle->atomDataPtr);
    if (handle->scratchBase) free(handle->scratchBase);
    free(handle);
    return ATOM_SUCCESS;
}

static AtomBiosResult
rhdAtomVramInfoQuery(atomBiosHandlePtr handle, AtomBiosRequestID func,
		     AtomBiosArgPtr data)
{
    atomDataTablesPtr atomDataPtr;
    uint32_t *val = &data->val;
    //RHDFUNC(handle);

    atomDataPtr = handle->atomDataPtr;

    switch (func) {
	case GET_FW_FB_START:
	    if (atomDataPtr->VRAM_UsageByFirmware)
		*val = le32_to_cpu(atomDataPtr->VRAM_UsageByFirmware
				   ->asFirmwareVramReserveInfo[0].ulStartAddrUsedByFirmware);
	    else
		return ATOM_NOT_IMPLEMENTED;
	    break;
	case GET_FW_FB_SIZE:
	    if (atomDataPtr->VRAM_UsageByFirmware)
		*val =  le16_to_cpu(atomDataPtr->VRAM_UsageByFirmware
				    ->asFirmwareVramReserveInfo[0].usFirmwareUseInKb);
	    else
		return ATOM_NOT_IMPLEMENTED;
	    break;
	default:
	    return ATOM_NOT_IMPLEMENTED;
    }
    return ATOM_SUCCESS;
}

static AtomBiosResult
rhdAtomTmdsInfoQuery(atomBiosHandlePtr handle,
		     AtomBiosRequestID func, AtomBiosArgPtr data)
{
    atomDataTablesPtr atomDataPtr;
    uint32_t *val = &data->val;
    int idx = *val;

    atomDataPtr = handle->atomDataPtr;
    if (!rhdAtomGetTableRevisionAndSize(
	    (ATOM_COMMON_TABLE_HEADER *)(atomDataPtr->TMDS_Info),
	    NULL,NULL,NULL)) {
	return ATOM_FAILED;
    }

    //RHDFUNC(handle);

    switch (func) {
	case ATOM_TMDS_FREQUENCY:
	    *val = le16_to_cpu(atomDataPtr->TMDS_Info->asMiscInfo[idx].usFrequency);
	    break;
	case ATOM_TMDS_PLL_CHARGE_PUMP:
	    *val = atomDataPtr->TMDS_Info->asMiscInfo[idx].ucPLL_ChargePump;
	    break;
	case ATOM_TMDS_PLL_DUTY_CYCLE:
	    *val = atomDataPtr->TMDS_Info->asMiscInfo[idx].ucPLL_DutyCycle;
	    break;
	case ATOM_TMDS_PLL_VCO_GAIN:
	    *val = atomDataPtr->TMDS_Info->asMiscInfo[idx].ucPLL_VCO_Gain;
	    break;
	case ATOM_TMDS_PLL_VOLTAGE_SWING:
	    *val = atomDataPtr->TMDS_Info->asMiscInfo[idx].ucPLL_VoltageSwing;
	    break;
	default:
	    return ATOM_NOT_IMPLEMENTED;
    }
    return ATOM_SUCCESS;
}

static DisplayModePtr
rhdAtomDTDTimings(atomBiosHandlePtr handle, ATOM_DTD_FORMAT *dtd)
{
    DisplayModePtr mode;
#define NAME_LEN 16
    char name[NAME_LEN];

    //RHDFUNC(handle);

    if (!dtd->usHActive || !dtd->usVActive)
	return NULL;

    if (!(mode = (DisplayModePtr)calloc(1,sizeof(DisplayModeRec))))
	return NULL;

    mode->CrtcHDisplay = mode->HDisplay = le16_to_cpu(dtd->usHActive);
    mode->CrtcVDisplay = mode->VDisplay = le16_to_cpu(dtd->usVActive);
    mode->CrtcHBlankStart = le16_to_cpu(dtd->usHActive) + dtd->ucHBorder;
    mode->CrtcHBlankEnd = mode->CrtcHBlankStart + le16_to_cpu(dtd->usHBlanking_Time);
    mode->CrtcHTotal = mode->HTotal = mode->CrtcHBlankEnd + dtd->ucHBorder;
    mode->CrtcVBlankStart = le16_to_cpu(dtd->usVActive) + dtd->ucVBorder;
    mode->CrtcVBlankEnd = mode->CrtcVBlankStart + le16_to_cpu(dtd->usVBlanking_Time);
    mode->CrtcVTotal = mode->VTotal = mode->CrtcVBlankEnd + dtd->ucVBorder;
    mode->CrtcHSyncStart = mode->HSyncStart = le16_to_cpu(dtd->usHActive) + le16_to_cpu(dtd->usHSyncOffset);
    mode->CrtcHSyncEnd = mode->HSyncEnd = mode->HSyncStart + le16_to_cpu(dtd->usHSyncWidth);
    mode->CrtcVSyncStart = mode->VSyncStart = le16_to_cpu(dtd->usVActive) + le16_to_cpu(dtd->usVSyncOffset);
    mode->CrtcVSyncEnd = mode->VSyncEnd = mode->VSyncStart + le16_to_cpu(dtd->usVSyncWidth);

    mode->SynthClock = mode->Clock = le16_to_cpu(dtd->usPixClk) * 10;

    mode->HSync = ((float) mode->Clock) / ((float)mode->HTotal);
    mode->VRefresh = (1000.0 * ((float) mode->Clock))
	/ ((float)(((float)mode->HTotal) * ((float)mode->VTotal)));

    if (dtd->susModeMiscInfo.sbfAccess.CompositeSync)
	mode->Flags |= V_CSYNC;
    if (dtd->susModeMiscInfo.sbfAccess.Interlace)
	mode->Flags |= V_INTERLACE;
    if (dtd->susModeMiscInfo.sbfAccess.DoubleClock)
	mode->Flags |= V_DBLSCAN;
    if (dtd->susModeMiscInfo.sbfAccess.VSyncPolarity)
	mode->Flags |= V_NVSYNC;
    if (dtd->susModeMiscInfo.sbfAccess.HSyncPolarity)
	mode->Flags |= V_NHSYNC;

    snprintf(name, NAME_LEN, "%dx%d",
	     mode->HDisplay, mode->VDisplay);
    mode->name = xstrdup(name);

    ErrorF("DTD Modeline: %s  "
	   "%2.d  %i (%i) %i %i (%i) %i  %i (%i) %i %i (%i) %i flags: 0x%x\n",
	   mode->name, mode->Clock,
	   mode->HDisplay, mode->CrtcHBlankStart, mode->HSyncStart, mode->CrtcHSyncEnd,
	   mode->CrtcHBlankEnd, mode->HTotal,
	   mode->VDisplay, mode->CrtcVBlankStart, mode->VSyncStart, mode->VSyncEnd,
	   mode->CrtcVBlankEnd, mode->VTotal, mode->Flags);

    return mode;
}

static unsigned char*
rhdAtomLvdsDDC(atomBiosHandlePtr handle, uint32_t offset, unsigned char *record)
{
    unsigned char *EDIDBlock;

    //RHDFUNC(handle);

    while (*record != ATOM_RECORD_END_TYPE) {

	switch (*record) {
	    case LCD_MODE_PATCH_RECORD_MODE_TYPE:
		offset += sizeof(ATOM_PATCH_RECORD_MODE);
		if (offset > handle->BIOSImageSize) break;
		record += sizeof(ATOM_PATCH_RECORD_MODE);
		break;

	    case LCD_RTS_RECORD_TYPE:
		offset += sizeof(ATOM_LCD_RTS_RECORD);
		if (offset > handle->BIOSImageSize) break;
		record += sizeof(ATOM_LCD_RTS_RECORD);
		break;

	    case LCD_CAP_RECORD_TYPE:
		offset += sizeof(ATOM_LCD_MODE_CONTROL_CAP);
		if (offset > handle->BIOSImageSize) break;
		record += sizeof(ATOM_LCD_MODE_CONTROL_CAP);
		break;

	    case LCD_FAKE_EDID_PATCH_RECORD_TYPE:
		offset += sizeof(ATOM_FAKE_EDID_PATCH_RECORD);
		/* check if the structure still fully lives in the BIOS image */
		if (offset > handle->BIOSImageSize) break;
		offset += ((ATOM_FAKE_EDID_PATCH_RECORD*)record)->ucFakeEDIDLength
		    - sizeof(UCHAR);
		if (offset > handle->BIOSImageSize) break;
		/* dup string as we free it later */
		if (!(EDIDBlock = (unsigned char *)malloc(
			  ((ATOM_FAKE_EDID_PATCH_RECORD*)record)->ucFakeEDIDLength)))
		    return NULL;
		memcpy(EDIDBlock,&((ATOM_FAKE_EDID_PATCH_RECORD*)record)->ucFakeEDIDString,
		       ((ATOM_FAKE_EDID_PATCH_RECORD*)record)->ucFakeEDIDLength);

		/* for testing */
		{
		    xf86MonPtr mon = xf86InterpretEDID(handle->scrnIndex,EDIDBlock);
		    xf86PrintEDID(mon);
		    free(mon);
		}
		return EDIDBlock;

	    case LCD_PANEL_RESOLUTION_RECORD_TYPE:
		offset += sizeof(ATOM_PANEL_RESOLUTION_PATCH_RECORD);
		if (offset > handle->BIOSImageSize) break;
		record += sizeof(ATOM_PANEL_RESOLUTION_PATCH_RECORD);
		break;

	    default:
		xf86DrvMsg(handle->scrnIndex, X_ERROR,
			   "%s: unknown record type: %x\n",__func__,*record);
		return NULL;
	}
    }

    return NULL;
}

static AtomBiosResult
rhdAtomCVGetTimings(atomBiosHandlePtr handle, AtomBiosRequestID func,
		    AtomBiosArgPtr data)
{
    atomDataTablesPtr atomDataPtr;
    uint8_t crev, frev;
    DisplayModePtr  last       = NULL;
    DisplayModePtr  new        = NULL;
    DisplayModePtr  first      = NULL;
    int i;
    uint16_t size;

    data->modes = NULL;

    atomDataPtr = handle->atomDataPtr;

    if (!rhdAtomGetTableRevisionAndSize(
	    (ATOM_COMMON_TABLE_HEADER *)(atomDataPtr->ComponentVideoInfo.base),
	    &crev,&frev,&size)) {
	return ATOM_FAILED;
    }

    switch (frev) {
	case 1:
	    switch (func) {
		case ATOMBIOS_GET_CV_MODES:
		    for (i = 0; i < MAX_SUPPORTED_CV_STANDARDS; i++) {
			new = rhdAtomDTDTimings(handle,
						&atomDataPtr->ComponentVideoInfo
						.ComponentVideoInfo->aModeTimings[i]);

			if (!new)
			    continue;

			new->type      |= M_T_DRIVER;
			new->next       = NULL;
			new->prev       = last;

			if (last) last->next = new;
			last = new;
			if (!first) first = new;
		    }
		    if (last) {
			last->next   = NULL; //first;
			first->prev  = NULL; //last;
			data->modes = first;
		    }
		    if (data->modes)
			return ATOM_SUCCESS;
		default:
		    return ATOM_FAILED;
	    }
	case 2:
	    switch (func) {
		case ATOMBIOS_GET_CV_MODES:
		    for (i = 0; i < MAX_SUPPORTED_CV_STANDARDS; i++) {
		        /* my rv730 table has only room for one mode */
		        if ((void *)&atomDataPtr->ComponentVideoInfo.ComponentVideoInfo_v21->aModeTimings[i] -
			    atomDataPtr->ComponentVideoInfo.base > size)
			    break;

			new = rhdAtomDTDTimings(handle,
						&atomDataPtr->ComponentVideoInfo
						.ComponentVideoInfo_v21->aModeTimings[i]);

			if (!new)
			    continue;

			new->type      |= M_T_DRIVER;
			new->next       = NULL;
			new->prev       = last;

			if (last) last->next = new;
			last = new;
			if (!first) first = new;

		    }
		    if (last) {
			last->next   = NULL; //first;
			first->prev  = NULL; //last;
			data->modes = first;
		    }
		    if (data->modes)
			return ATOM_SUCCESS;
		    return ATOM_FAILED;

		default:
		    return ATOM_FAILED;
	    }
	default:
	    return ATOM_NOT_IMPLEMENTED;
    }
/*NOTREACHED*/
}

static AtomBiosResult
rhdAtomLvdsGetTimings(atomBiosHandlePtr handle, AtomBiosRequestID func,
		    AtomBiosArgPtr data)
{
    atomDataTablesPtr atomDataPtr;
    uint8_t crev, frev;
    unsigned long offset;

    //RHDFUNC(handle);

    atomDataPtr = handle->atomDataPtr;

    if (!rhdAtomGetTableRevisionAndSize(
	    (ATOM_COMMON_TABLE_HEADER *)(atomDataPtr->LVDS_Info.base),
	    &frev,&crev,NULL)) {
	return ATOM_FAILED;
    }

    switch (crev) {

	case 1:
	    switch (func) {
		case ATOMBIOS_GET_PANEL_MODE:
		    data->modes = rhdAtomDTDTimings(handle,
						   &atomDataPtr->LVDS_Info
						   .LVDS_Info->sLCDTiming);
		    if (data->modes)
			return ATOM_SUCCESS;
		default:
		    return ATOM_FAILED;
	    }
	case 2:
	    switch (func) {
		case ATOMBIOS_GET_PANEL_MODE:
		    data->modes = rhdAtomDTDTimings(handle,
						   &atomDataPtr->LVDS_Info
						   .LVDS_Info_v12->sLCDTiming);
		    if (data->modes)
			return ATOM_SUCCESS;
		    return ATOM_FAILED;

		case ATOMBIOS_GET_PANEL_EDID:
		    offset = (unsigned long)&atomDataPtr->LVDS_Info.base
			- (unsigned long)handle->BIOSBase
			+ le16_to_cpu(atomDataPtr->LVDS_Info
			.LVDS_Info_v12->usExtInfoTableOffset);

		    data->EDIDBlock
			= rhdAtomLvdsDDC(handle, offset,
					 (unsigned char *)
					 &atomDataPtr->LVDS_Info.base
					 + le16_to_cpu(atomDataPtr->LVDS_Info
					 .LVDS_Info_v12->usExtInfoTableOffset));
		    if (data->EDIDBlock)
			return ATOM_SUCCESS;
		default:
		    return ATOM_FAILED;
	    }
	default:
	    return ATOM_NOT_IMPLEMENTED;
    }
/*NOTREACHED*/
}

static AtomBiosResult
rhdAtomLvdsInfoQuery(atomBiosHandlePtr handle,
		     AtomBiosRequestID func,  AtomBiosArgPtr data)
{
    atomDataTablesPtr atomDataPtr;
    uint8_t crev, frev;
    uint32_t *val = &data->val;

    //RHDFUNC(handle);

    atomDataPtr = handle->atomDataPtr;

    if (!rhdAtomGetTableRevisionAndSize(
	    (ATOM_COMMON_TABLE_HEADER *)(atomDataPtr->LVDS_Info.base),
	    &frev,&crev,NULL)) {
	return ATOM_FAILED;
    }

    switch (crev) {
	case 1:
	    switch (func) {
		case ATOM_LVDS_SUPPORTED_REFRESH_RATE:
		    *val = le16_to_cpu(atomDataPtr->LVDS_Info
				       .LVDS_Info->usSupportedRefreshRate);
		    break;
		case ATOM_LVDS_OFF_DELAY:
		    *val = le16_to_cpu(atomDataPtr->LVDS_Info
				       .LVDS_Info->usOffDelayInMs);
		    break;
		case ATOM_LVDS_SEQ_DIG_ONTO_DE:
		    *val = atomDataPtr->LVDS_Info
			.LVDS_Info->ucPowerSequenceDigOntoDEin10Ms * 10;
		    break;
		case ATOM_LVDS_SEQ_DE_TO_BL:
		    *val = atomDataPtr->LVDS_Info
			.LVDS_Info->ucPowerSequenceDEtoBLOnin10Ms * 10;
		    break;
		case     ATOM_LVDS_DITHER:
		    *val = atomDataPtr->LVDS_Info
			.LVDS_Info->ucLVDS_Misc & 0x40;
		    break;
		case     ATOM_LVDS_DUALLINK:
		    *val = atomDataPtr->LVDS_Info
			.LVDS_Info->ucLVDS_Misc & 0x01;
		    break;
		case     ATOM_LVDS_24BIT:
		    *val = atomDataPtr->LVDS_Info
			.LVDS_Info->ucLVDS_Misc & 0x02;
		    break;
		case     ATOM_LVDS_GREYLVL:
		    *val = atomDataPtr->LVDS_Info
			.LVDS_Info->ucLVDS_Misc & 0x0C;
		    break;
		case     ATOM_LVDS_FPDI:
		    *val = atomDataPtr->LVDS_Info
			.LVDS_Info->ucLVDS_Misc * 0x10;
		    break;
		default:
		    return ATOM_NOT_IMPLEMENTED;
	    }
	    break;
	case 2:
	    switch (func) {
		case ATOM_LVDS_SUPPORTED_REFRESH_RATE:
		    *val = le16_to_cpu(atomDataPtr->LVDS_Info
				       .LVDS_Info_v12->usSupportedRefreshRate);
		    break;
		case ATOM_LVDS_OFF_DELAY:
		    *val = le16_to_cpu(atomDataPtr->LVDS_Info
				       .LVDS_Info_v12->usOffDelayInMs);
		    break;
		case ATOM_LVDS_SEQ_DIG_ONTO_DE:
		    *val = atomDataPtr->LVDS_Info
			.LVDS_Info_v12->ucPowerSequenceDigOntoDEin10Ms * 10;
		    break;
		case ATOM_LVDS_SEQ_DE_TO_BL:
		    *val = atomDataPtr->LVDS_Info
			.LVDS_Info_v12->ucPowerSequenceDEtoBLOnin10Ms * 10;
		    break;
		case     ATOM_LVDS_DITHER:
		    *val = atomDataPtr->LVDS_Info
			.LVDS_Info_v12->ucLVDS_Misc & 0x40;
		    break;
		case     ATOM_LVDS_DUALLINK:
		    *val = atomDataPtr->LVDS_Info
			.LVDS_Info_v12->ucLVDS_Misc & 0x01;
		    break;
		case     ATOM_LVDS_24BIT:
		    *val = atomDataPtr->LVDS_Info
			.LVDS_Info_v12->ucLVDS_Misc & 0x02;
		    break;
		case     ATOM_LVDS_GREYLVL:
		    *val = atomDataPtr->LVDS_Info
			.LVDS_Info_v12->ucLVDS_Misc & 0x0C;
		    break;
		case     ATOM_LVDS_FPDI:
		    *val = atomDataPtr->LVDS_Info
			.LVDS_Info_v12->ucLVDS_Misc * 0x10;
		    break;
		default:
		    return ATOM_NOT_IMPLEMENTED;
	    }
	    break;
	default:
	    return ATOM_NOT_IMPLEMENTED;
    }

    return ATOM_SUCCESS;
}

static AtomBiosResult
rhdAtomCompassionateDataQuery(atomBiosHandlePtr handle,
			AtomBiosRequestID func, AtomBiosArgPtr data)
{
    atomDataTablesPtr atomDataPtr;
    uint8_t crev, frev;
    uint32_t *val = &data->val;

    //RHDFUNC(handle);

    atomDataPtr = handle->atomDataPtr;

    if (!rhdAtomGetTableRevisionAndSize(
	    (ATOM_COMMON_TABLE_HEADER *)(atomDataPtr->CompassionateData),
	    &frev,&crev,NULL)) {
	return ATOM_FAILED;
    }

    switch (func) {
	case ATOM_DAC1_BG_ADJ:
	    *val = atomDataPtr->CompassionateData->
		ucDAC1_BG_Adjustment;
	    break;
	case ATOM_DAC1_DAC_ADJ:
	    *val = atomDataPtr->CompassionateData->
		ucDAC1_DAC_Adjustment;
	    break;
	case ATOM_DAC1_FORCE:
	    *val = atomDataPtr->CompassionateData->
		usDAC1_FORCE_Data;
	    break;
	case ATOM_DAC2_CRTC2_BG_ADJ:
	    *val = atomDataPtr->CompassionateData->
		ucDAC2_CRT2_BG_Adjustment;
	    break;
	case ATOM_DAC2_CRTC2_DAC_ADJ:
	    *val = atomDataPtr->CompassionateData->
		ucDAC2_CRT2_DAC_Adjustment;
	    break;
	case ATOM_DAC2_CRTC2_FORCE:
	    *val = atomDataPtr->CompassionateData->
		usDAC2_CRT2_FORCE_Data;
	    break;
	case ATOM_DAC2_CRTC2_MUX_REG_IND:
	    *val = atomDataPtr->CompassionateData->
		usDAC2_CRT2_MUX_RegisterIndex;
	    break;
	case ATOM_DAC2_CRTC2_MUX_REG_INFO:
	    *val = atomDataPtr->CompassionateData->
		ucDAC2_CRT2_MUX_RegisterInfo;
	    break;
	default:
	    return ATOM_NOT_IMPLEMENTED;
    }
    return ATOM_SUCCESS;
}

static AtomBiosResult
rhdAtomGPIOI2CInfoQuery(atomBiosHandlePtr handle,
			AtomBiosRequestID func, AtomBiosArgPtr data)
{
    atomDataTablesPtr atomDataPtr;
    uint8_t crev, frev;
    uint32_t *val = &data->val;
    unsigned short size;

    //RHDFUNC(handle);

    atomDataPtr = handle->atomDataPtr;

    if (!rhdAtomGetTableRevisionAndSize(
	    (ATOM_COMMON_TABLE_HEADER *)(atomDataPtr->GPIO_I2C_Info),
	    &frev,&crev,&size)) {
	return ATOM_FAILED;
    }

    switch (func) {
	case ATOM_GPIO_I2C_CLK_MASK:
	    if ((sizeof(ATOM_COMMON_TABLE_HEADER)
		 + (*val * sizeof(ATOM_GPIO_I2C_ASSIGMENT))) > size) {
		xf86DrvMsg(handle->scrnIndex, X_ERROR, "%s: GPIO_I2C Device "
			   "num %lu exeeds table size %u\n",__func__,
			   (unsigned long)val,
			   size);
		return ATOM_FAILED;
	    }

	    *val = le16_to_cpu(atomDataPtr->GPIO_I2C_Info->asGPIO_Info[*val]
			       .usClkMaskRegisterIndex);
	    break;

	default:
	    return ATOM_NOT_IMPLEMENTED;
    }
    return ATOM_SUCCESS;
}

static AtomBiosResult
rhdAtomFirmwareInfoQuery(atomBiosHandlePtr handle,
			 AtomBiosRequestID func, AtomBiosArgPtr data)
{
    atomDataTablesPtr atomDataPtr;
    uint8_t crev, frev;
    uint32_t *val = &data->val;

    //RHDFUNC(handle);

    atomDataPtr = handle->atomDataPtr;

    if (!rhdAtomGetTableRevisionAndSize(
	    (ATOM_COMMON_TABLE_HEADER *)(atomDataPtr->FirmwareInfo.base),
	    &crev,&frev,NULL)) {
	return ATOM_FAILED;
    }

    switch (crev) {
	case 1:
	    switch (func) {
		case GET_DEFAULT_ENGINE_CLOCK:
		    *val = le32_to_cpu(atomDataPtr->FirmwareInfo
				       .FirmwareInfo->ulDefaultEngineClock) * 10;
		    break;
		case GET_DEFAULT_MEMORY_CLOCK:
		    *val = le32_to_cpu(atomDataPtr->FirmwareInfo
				       .FirmwareInfo->ulDefaultMemoryClock) * 10;
		    break;
		case GET_MAX_PIXEL_CLOCK_PLL_OUTPUT:
		    *val = le32_to_cpu(atomDataPtr->FirmwareInfo
				       .FirmwareInfo->ulMaxPixelClockPLL_Output) * 10;
		    break;
		case GET_MIN_PIXEL_CLOCK_PLL_OUTPUT:
		    *val = le16_to_cpu(atomDataPtr->FirmwareInfo
				       .FirmwareInfo->usMinPixelClockPLL_Output) * 10;
		case GET_MAX_PIXEL_CLOCK_PLL_INPUT:
		    *val = le16_to_cpu(atomDataPtr->FirmwareInfo
				       .FirmwareInfo->usMaxPixelClockPLL_Input) * 10;
		    break;
		case GET_MIN_PIXEL_CLOCK_PLL_INPUT:
		    *val = le16_to_cpu(atomDataPtr->FirmwareInfo
				       .FirmwareInfo->usMinPixelClockPLL_Input) * 10;
		    break;
		case GET_MAX_PIXEL_CLK:
		    *val = le16_to_cpu(atomDataPtr->FirmwareInfo
				       .FirmwareInfo->usMaxPixelClock) * 10;
		    break;
		case GET_REF_CLOCK:
		    *val = le16_to_cpu(atomDataPtr->FirmwareInfo
				       .FirmwareInfo->usReferenceClock) * 10;
		    break;
		default:
		    return ATOM_NOT_IMPLEMENTED;
	    }
	case 2:
	    switch (func) {
		case GET_DEFAULT_ENGINE_CLOCK:
		    *val = le32_to_cpu(atomDataPtr->FirmwareInfo
				       .FirmwareInfo_V_1_2->ulDefaultEngineClock) * 10;
		    break;
		case GET_DEFAULT_MEMORY_CLOCK:
		    *val = le32_to_cpu(atomDataPtr->FirmwareInfo
				       .FirmwareInfo_V_1_2->ulDefaultMemoryClock) * 10;
		    break;
		case GET_MAX_PIXEL_CLOCK_PLL_OUTPUT:
		    *val = le32_to_cpu(atomDataPtr->FirmwareInfo
				       .FirmwareInfo_V_1_2->ulMaxPixelClockPLL_Output) * 10;
		    break;
		case GET_MIN_PIXEL_CLOCK_PLL_OUTPUT:
		    *val = le16_to_cpu(atomDataPtr->FirmwareInfo
				       .FirmwareInfo_V_1_2->usMinPixelClockPLL_Output) * 10;
		    break;
		case GET_MAX_PIXEL_CLOCK_PLL_INPUT:
		    *val = le16_to_cpu(atomDataPtr->FirmwareInfo
				       .FirmwareInfo_V_1_2->usMaxPixelClockPLL_Input) * 10;
		    break;
		case GET_MIN_PIXEL_CLOCK_PLL_INPUT:
		    *val = le16_to_cpu(atomDataPtr->FirmwareInfo
				       .FirmwareInfo_V_1_2->usMinPixelClockPLL_Input) * 10;
		    break;
		case GET_MAX_PIXEL_CLK:
		    *val = le16_to_cpu(atomDataPtr->FirmwareInfo
				       .FirmwareInfo_V_1_2->usMaxPixelClock) * 10;
		    break;
		case GET_REF_CLOCK:
		    *val = le16_to_cpu(atomDataPtr->FirmwareInfo
				       .FirmwareInfo_V_1_2->usReferenceClock) * 10;
		    break;
		default:
		    return ATOM_NOT_IMPLEMENTED;
	    }
	    break;
	case 3:
	    switch (func) {
		case GET_DEFAULT_ENGINE_CLOCK:
		    *val = le32_to_cpu(atomDataPtr->FirmwareInfo
				       .FirmwareInfo_V_1_3->ulDefaultEngineClock) * 10;
		    break;
		case GET_DEFAULT_MEMORY_CLOCK:
		    *val = le32_to_cpu(atomDataPtr->FirmwareInfo
				       .FirmwareInfo_V_1_3->ulDefaultMemoryClock) * 10;
		    break;
		case GET_MAX_PIXEL_CLOCK_PLL_OUTPUT:
		    *val = le32_to_cpu(atomDataPtr->FirmwareInfo
				       .FirmwareInfo_V_1_3->ulMaxPixelClockPLL_Output) * 10;
		    break;
		case GET_MIN_PIXEL_CLOCK_PLL_OUTPUT:
		    *val = le16_to_cpu(atomDataPtr->FirmwareInfo
				       .FirmwareInfo_V_1_3->usMinPixelClockPLL_Output) * 10;
		    break;
		case GET_MAX_PIXEL_CLOCK_PLL_INPUT:
		    *val = le16_to_cpu(atomDataPtr->FirmwareInfo
				       .FirmwareInfo_V_1_3->usMaxPixelClockPLL_Input) * 10;
		    break;
		case GET_MIN_PIXEL_CLOCK_PLL_INPUT:
		    *val = le16_to_cpu(atomDataPtr->FirmwareInfo
				       .FirmwareInfo_V_1_3->usMinPixelClockPLL_Input) * 10;
		    break;
		case GET_MAX_PIXEL_CLK:
		    *val = le16_to_cpu(atomDataPtr->FirmwareInfo
				       .FirmwareInfo_V_1_3->usMaxPixelClock) * 10;
		    break;
		case GET_REF_CLOCK:
		    *val = le16_to_cpu(atomDataPtr->FirmwareInfo
				       .FirmwareInfo_V_1_3->usReferenceClock) * 10;
		    break;
		default:
		    return ATOM_NOT_IMPLEMENTED;
	    }
	    break;
	case 4:
	    switch (func) {
		case GET_DEFAULT_ENGINE_CLOCK:
		    *val = le32_to_cpu(atomDataPtr->FirmwareInfo
				       .FirmwareInfo_V_1_4->ulDefaultEngineClock) * 10;
		    break;
		case GET_DEFAULT_MEMORY_CLOCK:
		    *val = le32_to_cpu(atomDataPtr->FirmwareInfo
				       .FirmwareInfo_V_1_4->ulDefaultMemoryClock) * 10;
		    break;
		case GET_MAX_PIXEL_CLOCK_PLL_INPUT:
		    *val = le16_to_cpu(atomDataPtr->FirmwareInfo
				       .FirmwareInfo_V_1_4->usMaxPixelClockPLL_Input) * 10;
		    break;
		case GET_MIN_PIXEL_CLOCK_PLL_INPUT:
		    *val = le16_to_cpu(atomDataPtr->FirmwareInfo
				       .FirmwareInfo_V_1_4->usMinPixelClockPLL_Input) * 10;
		    break;
		case GET_MAX_PIXEL_CLOCK_PLL_OUTPUT:
		    *val = le32_to_cpu(atomDataPtr->FirmwareInfo
				       .FirmwareInfo_V_1_4->ulMaxPixelClockPLL_Output) * 10;
		    break;
		case GET_MIN_PIXEL_CLOCK_PLL_OUTPUT:
		    *val = le16_to_cpu(atomDataPtr->FirmwareInfo
				       .FirmwareInfo_V_1_4->usMinPixelClockPLL_Output) * 10;
		    break;
		case GET_MAX_PIXEL_CLK:
		    *val = le16_to_cpu(atomDataPtr->FirmwareInfo
				       .FirmwareInfo_V_1_4->usMaxPixelClock) * 10;
		    break;
		case GET_REF_CLOCK:
		    *val = le16_to_cpu(atomDataPtr->FirmwareInfo
				       .FirmwareInfo_V_1_4->usReferenceClock) * 10;
		    break;
		default:
		    return ATOM_NOT_IMPLEMENTED;
	    }
	    break;
	default:
	    return ATOM_NOT_IMPLEMENTED;
    }
    return ATOM_SUCCESS;
}

const int object_connector_convert[] =
    { CONNECTOR_NONE,
      CONNECTOR_DVI_I,
      CONNECTOR_DVI_I,
      CONNECTOR_DVI_D,
      CONNECTOR_DVI_D,
      CONNECTOR_VGA,
      CONNECTOR_CTV,
      CONNECTOR_STV,
      CONNECTOR_NONE,
      CONNECTOR_NONE,
      CONNECTOR_DIN,
      CONNECTOR_SCART,
      CONNECTOR_HDMI_TYPE_A,
      CONNECTOR_HDMI_TYPE_B,
      CONNECTOR_LVDS,
      CONNECTOR_DIN,
      CONNECTOR_NONE,
      CONNECTOR_NONE,
      CONNECTOR_NONE,
      CONNECTOR_DISPLAY_PORT,
      CONNECTOR_EDP,
      CONNECTOR_NONE,
    };

xf86MonPtr radeon_atom_get_edid(xf86OutputPtr output)
{
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    RADEONInfoPtr info       = RADEONPTR(output->scrn);
    READ_EDID_FROM_HW_I2C_DATA_PS_ALLOCATION edid_data;
    AtomBiosArgRec data;
    unsigned char *space;
    int i2c_clock = 50;
    int engine_clk = (int)info->sclk * 100;
    int prescale;
    unsigned char *edid;
    xf86MonPtr mon = NULL;

    if (!radeon_output->ddc_i2c.hw_capable)
	return mon;

    if (info->atomBIOS->fbBase)
	edid = (unsigned char *)info->FB + info->atomBIOS->fbBase;
    else if (info->atomBIOS->scratchBase)
	edid = (unsigned char *)info->atomBIOS->scratchBase;
    else
	return mon;

    memset(edid, 0, ATOM_EDID_RAW_DATASIZE);

    if (info->ChipFamily == CHIP_FAMILY_R520)
	prescale = (127 << 8) + (engine_clk * 10) / (4 * 127 * i2c_clock);
    else if (info->ChipFamily < CHIP_FAMILY_R600)
	prescale = (((engine_clk * 10)/(4 * 128 * 100) + 1) << 8) + 128;
    else
	prescale = (info->pll.reference_freq * 10) / i2c_clock;

    edid_data.usPrescale = prescale;
    edid_data.usVRAMAddress = 0;
    edid_data.ucSlaveAddr = 0xa0;
    edid_data.ucLineNumber = radeon_output->ddc_i2c.hw_line;

    data.exec.index = GetIndexIntoMasterTable(COMMAND, ReadEDIDFromHWAssistedI2C);
    data.exec.dataSpace = (void *)&space;
    data.exec.pspace = &edid_data;

    if (RHDAtomBiosFunc(info->atomBIOS->scrnIndex, info->atomBIOS, ATOMBIOS_EXEC, &data) == ATOM_SUCCESS)
	ErrorF("Atom Get EDID success\n");
    else
	ErrorF("Atom Get EDID failed\n");

    if (edid[1] == 0xff)
	mon = xf86InterpretEDID(output->scrn->scrnIndex, edid);

    return mon;

}

static RADEONI2CBusRec
RADEONLookupGPIOLineForDDC(ScrnInfoPtr pScrn, uint8_t id)
{
    RADEONInfoPtr info = RADEONPTR (pScrn);
    atomDataTablesPtr atomDataPtr;
    ATOM_GPIO_I2C_ASSIGMENT *gpio;
    RADEONI2CBusRec i2c;
    uint8_t crev, frev;
    unsigned short size;
    int i, num_indices;

    memset(&i2c, 0, sizeof(RADEONI2CBusRec));
    i2c.valid = FALSE;

    atomDataPtr = info->atomBIOS->atomDataPtr;

    if (!rhdAtomGetTableRevisionAndSize(
	    &(atomDataPtr->GPIO_I2C_Info->sHeader),
	    &crev,&frev,&size)) {
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "No GPIO Info Table found!\n");
	return i2c;
    }

    num_indices = size / sizeof(ATOM_GPIO_I2C_ASSIGMENT);

    for (i = 0; i < num_indices; i++) {
	    gpio = &atomDataPtr->GPIO_I2C_Info->asGPIO_Info[i];

	    if (IS_DCE4_VARIANT) {
	        if ((i == 7) &&
		    (le16_to_cpu(gpio->usClkMaskRegisterIndex) == 0x1936) &&
		    (gpio->sucI2cId.ucAccess == 0)) {
		    gpio->sucI2cId.ucAccess = 0x97;
		    gpio->ucDataMaskShift = 8;
		    gpio->ucDataEnShift = 8;
		    gpio->ucDataY_Shift = 8;
		    gpio->ucDataA_Shift = 8;
		}
	    }

	    if (gpio->sucI2cId.ucAccess == id) {
		    i2c.mask_clk_reg = le16_to_cpu(gpio->usClkMaskRegisterIndex) * 4;
		    i2c.mask_data_reg = le16_to_cpu(gpio->usDataMaskRegisterIndex) * 4;
		    i2c.put_clk_reg = le16_to_cpu(gpio->usClkEnRegisterIndex) * 4;
		    i2c.put_data_reg = le16_to_cpu(gpio->usDataEnRegisterIndex) * 4;
		    i2c.get_clk_reg = le16_to_cpu(gpio->usClkY_RegisterIndex) * 4;
		    i2c.get_data_reg = le16_to_cpu(gpio->usDataY_RegisterIndex) * 4;
		    i2c.a_clk_reg = le16_to_cpu(gpio->usClkA_RegisterIndex) * 4;
		    i2c.a_data_reg = le16_to_cpu(gpio->usDataA_RegisterIndex) * 4;
		    i2c.mask_clk_mask = (1 << gpio->ucClkMaskShift);
		    i2c.mask_data_mask = (1 << gpio->ucDataMaskShift);
		    i2c.put_clk_mask = (1 << gpio->ucClkEnShift);
		    i2c.put_data_mask = (1 << gpio->ucDataEnShift);
		    i2c.get_clk_mask = (1 << gpio->ucClkY_Shift);
		    i2c.get_data_mask = (1 <<  gpio->ucDataY_Shift);
		    i2c.a_clk_mask = (1 << gpio->ucClkA_Shift);
		    i2c.a_data_mask = (1 <<  gpio->ucDataA_Shift);
		    i2c.hw_line = gpio->sucI2cId.ucAccess;
		    i2c.hw_capable = gpio->sucI2cId.sbfAccess.bfHW_Capable;
		    i2c.valid = TRUE;
		    break;
	    }
    }

#if 0
    ErrorF("id: %d\n", id);
    ErrorF("hw capable: %d\n", gpio->sucI2cId.sbfAccess.bfHW_Capable);
    ErrorF("hw engine id: %d\n", gpio->sucI2cId.sbfAccess.bfHW_EngineID);
    ErrorF("line mux %d\n", gpio->sucI2cId.sbfAccess.bfI2C_LineMux);
    ErrorF("mask_clk_reg: 0x%x\n", le16_to_cpu(gpio->usClkMaskRegisterIndex) * 4);
    ErrorF("mask_data_reg: 0x%x\n", le16_to_cpu(gpio->usDataMaskRegisterIndex) * 4);
    ErrorF("put_clk_reg: 0x%x\n", le16_to_cpu(gpio->usClkEnRegisterIndex) * 4);
    ErrorF("put_data_reg: 0x%x\n", le16_to_cpu(gpio->usDataEnRegisterIndex) * 4);
    ErrorF("get_clk_reg: 0x%x\n", le16_to_cpu(gpio->usClkY_RegisterIndex) * 4);
    ErrorF("get_data_reg: 0x%x\n", le16_to_cpu(gpio->usDataY_RegisterIndex) * 4);
    ErrorF("a_clk_reg: 0x%x\n", le16_to_cpu(gpio->usClkA_RegisterIndex) * 4);
    ErrorF("a_data_reg: 0x%x\n", le16_to_cpu(gpio->usDataA_RegisterIndex) * 4);
    ErrorF("mask_clk_mask: %d\n", gpio->ucClkMaskShift);
    ErrorF("mask_data_mask: %d\n", gpio->ucDataMaskShift);
    ErrorF("put_clk_mask: %d\n", gpio->ucClkEnShift);
    ErrorF("put_data_mask: %d\n", gpio->ucDataEnShift);
    ErrorF("get_clk_mask: %d\n", gpio->ucClkY_Shift);
    ErrorF("get_data_mask: %d\n", gpio->ucDataY_Shift);
    ErrorF("a_clk_mask: %d\n", gpio->ucClkA_Shift);
    ErrorF("a_data_mask: %d\n", gpio->ucDataA_Shift);
#endif

    return i2c;
}

static RADEONI2CBusRec
rhdAtomParseI2CRecord(ScrnInfoPtr pScrn, atomBiosHandlePtr handle,
		      ATOM_I2C_RECORD *Record, int i)
{
    RADEONInfoPtr info = RADEONPTR (pScrn);
    uint8_t *temp = &Record->sucI2cId;

    info->BiosConnector[i].i2c_line_mux = *temp;
    info->BiosConnector[i].ucI2cId = *temp;
    return RADEONLookupGPIOLineForDDC(pScrn, *temp);
}

static uint8_t
radeon_lookup_hpd_id(ScrnInfoPtr pScrn, ATOM_HPD_INT_RECORD *record)
{
    RADEONInfoPtr info = RADEONPTR (pScrn);
    unsigned short size;
    uint8_t hpd = 0;
    int i, num_indices;
    struct _ATOM_GPIO_PIN_LUT *gpio_info;
    ATOM_GPIO_PIN_ASSIGNMENT *pin;
    atomDataTablesPtr atomDataPtr;
    uint8_t crev, frev;
    uint32_t reg;

    atomDataPtr = info->atomBIOS->atomDataPtr;

    if (!rhdAtomGetTableRevisionAndSize(
	    &(atomDataPtr->GPIO_Pin_LUT->sHeader),
	    &crev,&frev,&size)) {
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "No GPIO Pin Table found!\n");
	return hpd;
    }

    num_indices = size / sizeof(ATOM_GPIO_PIN_ASSIGNMENT);

    if (IS_DCE4_VARIANT)
	reg = EVERGREEN_DC_GPIO_HPD_A;
    else
	reg = AVIVO_DC_GPIO_HPD_A;

    gpio_info = atomDataPtr->GPIO_Pin_LUT;
    for (i = 0; i < num_indices; i++) {
	pin = &gpio_info->asGPIO_Pin[i];
	if (record->ucHPDIntGPIOID == pin->ucGPIO_ID) {
	    if ((le16_to_cpu(pin->usGpioPin_AIndex) * 4) == reg) {
		switch (pin->ucGpioPinBitShift) {
		case 0:
		default:
		    hpd = 0;
		    break;
		case 8:
		    hpd = 1;
		    break;
		case 16:
		    hpd = 2;
		    break;
		case 24:
		    hpd = 3;
		    break;
		case 26:
		    hpd = 4;
		    break;
		case 28:
		    hpd = 5;
		    break;
		}
		break;
	    }
	}
    }

    return hpd;
}

static void RADEONApplyATOMQuirks(ScrnInfoPtr pScrn, int index)
{
    RADEONInfoPtr info = RADEONPTR (pScrn);

    /* Asus M2A-VM HDMI board lists the DVI port as HDMI */
    if ((info->Chipset == PCI_CHIP_RS690_791E) &&
	(PCI_SUB_VENDOR_ID(info->PciInfo) == 0x1043) &&
	(PCI_SUB_DEVICE_ID(info->PciInfo) == 0x826d)) {
	if ((info->BiosConnector[index].ConnectorType == CONNECTOR_HDMI_TYPE_A) &&
	    (info->BiosConnector[index].devices & ATOM_DEVICE_DFP3_SUPPORT)) {
	    info->BiosConnector[index].ConnectorType = CONNECTOR_DVI_D;
	}
    }

    /* RS600 board lists the DVI port as HDMI */
    if ((info->Chipset == PCI_CHIP_RS600_7941) &&
	(PCI_SUB_VENDOR_ID(info->PciInfo) == 0x1849) &&
	(PCI_SUB_DEVICE_ID(info->PciInfo) == 0x7941)) {
	if ((info->BiosConnector[index].ConnectorType == CONNECTOR_HDMI_TYPE_A) &&
	    (info->BiosConnector[index].devices & ATOM_DEVICE_DFP3_SUPPORT)) {
	    info->BiosConnector[index].ConnectorType = CONNECTOR_DVI_D;
	}
    }

    /* a-bit f-i90hd - ciaranm on #radeonhd - this board has no DVI */
    if ((info->Chipset == PCI_CHIP_RS600_7941) &&
	(PCI_SUB_VENDOR_ID(info->PciInfo) == 0x147b) &&
	(PCI_SUB_DEVICE_ID(info->PciInfo) == 0x2412)) {
	if (info->BiosConnector[index].ConnectorType == CONNECTOR_DVI_I)
	    info->BiosConnector[index].valid = FALSE;
    }

    /* Falcon NW laptop lists vga ddc line for LVDS */
    if ((info->Chipset == PCI_CHIP_RV410_5653) &&
	(PCI_SUB_VENDOR_ID(info->PciInfo) == 0x1462) &&
	(PCI_SUB_DEVICE_ID(info->PciInfo) == 0x0291)) {
	if (info->BiosConnector[index].ConnectorType == CONNECTOR_LVDS) {
	    info->BiosConnector[index].ddc_i2c.valid = FALSE;
	}
    }

    /* Funky macbooks */
    if ((info->Chipset == PCI_CHIP_RV530_71C5) &&
	(PCI_SUB_VENDOR_ID(info->PciInfo) == 0x106b) &&
	(PCI_SUB_DEVICE_ID(info->PciInfo) == 0x0080)) {
	if ((index == ATOM_DEVICE_CRT1_INDEX) ||
	    (index == ATOM_DEVICE_CRT2_INDEX) ||
	    (index == ATOM_DEVICE_DFP2_INDEX))
	    info->BiosConnector[index].valid = FALSE;

	if (index == ATOM_DEVICE_DFP1_INDEX) {
	    info->BiosConnector[index].devices |= ATOM_DEVICE_CRT2_SUPPORT;
	}
    }

    /* ASUS HD 3600 XT board lists the DVI port as HDMI */
    if ((info->Chipset == PCI_CHIP_RV635_9598) &&
	(PCI_SUB_VENDOR_ID(info->PciInfo) == 0x1043) &&
	(PCI_SUB_DEVICE_ID(info->PciInfo) == 0x01da)) {
	if (info->BiosConnector[index].ConnectorType == CONNECTOR_HDMI_TYPE_A)
	    info->BiosConnector[index].ConnectorType = CONNECTOR_DVI_I;
    }

    /* ASUS HD 3450 board lists the DVI port as HDMI */
    if ((info->Chipset == PCI_CHIP_RV620_95C5) &&
	(PCI_SUB_VENDOR_ID(info->PciInfo) == 0x1043) &&
	(PCI_SUB_DEVICE_ID(info->PciInfo) == 0x01e2)) {
	if (info->BiosConnector[index].ConnectorType == CONNECTOR_HDMI_TYPE_A)
	    info->BiosConnector[index].ConnectorType = CONNECTOR_DVI_I;
    }

    /* some BIOSes seem to report DAC on HDMI - usually this is a board with
     * HDMI + VGA reporting as HDMI
     */
    if (info->BiosConnector[index].ConnectorType == CONNECTOR_HDMI_TYPE_A) {
	if (info->BiosConnector[index].devices & (ATOM_DEVICE_CRT_SUPPORT)) {
	    info->BiosConnector[index].devices &= ~(ATOM_DEVICE_DFP_SUPPORT);
	    info->BiosConnector[index].ConnectorType = CONNECTOR_VGA;
	    info->BiosConnector[index].connector_object = 0;
	}
    }

}

uint32_t
radeon_get_device_index(uint32_t device_support)
{
    uint32_t device_index = 0;

    if (device_support == 0)
	return 0;

    while ((device_support & 1) == 0) {
	device_support >>= 1;
	device_index++;
    }
    return device_index;
}

radeon_encoder_ptr
radeon_get_encoder(xf86OutputPtr output)
{
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    RADEONInfoPtr info = RADEONPTR(output->scrn);

    if (radeon_output->active_device)
	return info->encoders[radeon_get_device_index(radeon_output->active_device)];
    else
	return NULL;
}

Bool
radeon_add_encoder(ScrnInfoPtr pScrn, uint32_t encoder_id, uint32_t device_support)
{
    RADEONInfoPtr info = RADEONPTR (pScrn);
    uint32_t device_index = radeon_get_device_index(device_support);
    int i;

    if (device_support == 0) {
	ErrorF("device support == 0\n");
	return FALSE;
    }

    if (info->encoders[device_index] != NULL)
	return TRUE;
    else {
	/* look for the encoder */
	for (i = 0; i < RADEON_MAX_BIOS_CONNECTOR; i++) {
	    if ((info->encoders[i] != NULL) && (info->encoders[i]->encoder_id == encoder_id)) {
		info->encoders[device_index] = info->encoders[i];
		info->encoders[device_index]->ref_count++;
		switch (encoder_id) {
		case ENCODER_OBJECT_ID_INTERNAL_UNIPHY:
		case ENCODER_OBJECT_ID_INTERNAL_KLDSCP_LVTMA:
		case ENCODER_OBJECT_ID_INTERNAL_UNIPHY1:
		case ENCODER_OBJECT_ID_INTERNAL_UNIPHY2:
		case ENCODER_OBJECT_ID_INTERNAL_LVTM1:
		    if (device_support & ATOM_DEVICE_LCD1_SUPPORT) {
			if (info->encoders[device_index]->dev_priv == NULL) {
			    info->encoders[device_index]->dev_priv =
				(radeon_lvds_ptr)calloc(1,sizeof(radeon_lvds_rec));
			    if (info->encoders[device_index]->dev_priv == NULL) {
				ErrorF("calloc failed\n");
				return FALSE;
			    } else
				RADEONGetATOMLVDSInfo(pScrn, (radeon_lvds_ptr)info->encoders[device_index]->dev_priv);
			}
		    }
		    break;
		}
		return TRUE;
	    }
	}

	info->encoders[device_index] = (radeon_encoder_ptr)calloc(1,sizeof(radeon_encoder_rec));
	if (info->encoders[device_index] != NULL) {
	    info->encoders[device_index]->ref_count++;
	    info->encoders[device_index]->encoder_id = encoder_id;
	    info->encoders[device_index]->devices = 0;
	    info->encoders[device_index]->dev_priv = NULL;
	    // add dev_priv stuff
	    switch (encoder_id) {
	    case ENCODER_OBJECT_ID_INTERNAL_LVDS:
		    info->encoders[device_index]->dev_priv = (radeon_lvds_ptr)calloc(1,sizeof(radeon_lvds_rec));
		    if (info->encoders[device_index]->dev_priv == NULL) {
			ErrorF("calloc failed\n");
			return FALSE;
		    } else {
			if (info->IsAtomBios)
			    RADEONGetATOMLVDSInfo(pScrn, (radeon_lvds_ptr)info->encoders[device_index]->dev_priv);
			else
			    RADEONGetLVDSInfo(pScrn, (radeon_lvds_ptr)info->encoders[device_index]->dev_priv);
		    }
		break;
	    case ENCODER_OBJECT_ID_INTERNAL_DAC2:
		if (!IS_AVIVO_VARIANT) {
		    info->encoders[device_index]->dev_priv = (radeon_tvdac_ptr)calloc(1,sizeof(radeon_tvdac_rec));
		    if (info->encoders[device_index]->dev_priv == NULL) {
			ErrorF("calloc failed\n");
			return FALSE;
		    } else
			RADEONGetTVDacAdjInfo(pScrn, (radeon_tvdac_ptr)info->encoders[device_index]->dev_priv);
		}
		break;
	    case ENCODER_OBJECT_ID_INTERNAL_TMDS1:
		if (!IS_AVIVO_VARIANT) {
		    info->encoders[device_index]->dev_priv = (radeon_tmds_ptr)calloc(1,sizeof(radeon_tmds_rec));
		    if (info->encoders[device_index]->dev_priv == NULL) {
			ErrorF("calloc failed\n");
			return FALSE;
		    } else
			RADEONGetTMDSInfo(pScrn, (radeon_tmds_ptr)info->encoders[device_index]->dev_priv);
		}
		break;
	    case ENCODER_OBJECT_ID_INTERNAL_DVO1:
		if (!IS_AVIVO_VARIANT) {
		    info->encoders[device_index]->dev_priv = (radeon_dvo_ptr)calloc(1,sizeof(radeon_dvo_rec));
		    if (info->encoders[device_index]->dev_priv == NULL) {
			ErrorF("calloc failed\n");
			return FALSE;
		    } else
			RADEONGetExtTMDSInfo(pScrn, (radeon_dvo_ptr)info->encoders[device_index]->dev_priv);
		}
		break;
	    case ENCODER_OBJECT_ID_INTERNAL_UNIPHY:
	    case ENCODER_OBJECT_ID_INTERNAL_KLDSCP_LVTMA:
	    case ENCODER_OBJECT_ID_INTERNAL_UNIPHY1:
	    case ENCODER_OBJECT_ID_INTERNAL_UNIPHY2:
	    case ENCODER_OBJECT_ID_INTERNAL_LVTM1:
		if (device_support & ATOM_DEVICE_LCD1_SUPPORT) {
		    info->encoders[device_index]->dev_priv = (radeon_lvds_ptr)calloc(1,sizeof(radeon_lvds_rec));
		    if (info->encoders[device_index]->dev_priv == NULL) {
			ErrorF("calloc failed\n");
			return FALSE;
		    } else
			RADEONGetATOMLVDSInfo(pScrn, (radeon_lvds_ptr)info->encoders[device_index]->dev_priv);
		}
		break;
	    }
	    return TRUE;
	} else {
	    ErrorF("calloc failed\n");
	    return FALSE;
	}
    }

}

Bool
RADEONGetATOMConnectorInfoFromBIOSObject (ScrnInfoPtr pScrn)
{
    RADEONInfoPtr info = RADEONPTR (pScrn);
    uint8_t crev, frev;
    unsigned short size;
    atomDataTablesPtr atomDataPtr;
    ATOM_CONNECTOR_OBJECT_TABLE *con_obj;
    ATOM_DISPLAY_OBJECT_PATH_TABLE *path_obj;
    ATOM_INTEGRATED_SYSTEM_INFO_V2 *igp_obj = NULL;
    int i, j, path_size, device_support;
    Bool enable_tv = FALSE;

    if (xf86ReturnOptValBool(info->Options, OPTION_ATOM_TVOUT, FALSE))
	enable_tv = TRUE;

    atomDataPtr = info->atomBIOS->atomDataPtr;
    if (!rhdAtomGetTableRevisionAndSize((ATOM_COMMON_TABLE_HEADER *)(atomDataPtr->Object_Header), &crev, &frev, &size))
	return FALSE;

    if (crev < 2)
	return FALSE;

    path_obj = (ATOM_DISPLAY_OBJECT_PATH_TABLE *)
	((char *)&atomDataPtr->Object_Header->sHeader +
	 le16_to_cpu(atomDataPtr->Object_Header->usDisplayPathTableOffset));
    con_obj = (ATOM_CONNECTOR_OBJECT_TABLE *)
	((char *)&atomDataPtr->Object_Header->sHeader +
	 le16_to_cpu(atomDataPtr->Object_Header->usConnectorObjectTableOffset));
    device_support = le16_to_cpu(atomDataPtr->Object_Header->usDeviceSupport);

    path_size = 0;
    for (i = 0; i < path_obj->ucNumOfDispPath; i++) {
	uint8_t *addr = (uint8_t *)path_obj->asDispPath;
	ATOM_DISPLAY_OBJECT_PATH *path;
	addr += path_size;
	path = (ATOM_DISPLAY_OBJECT_PATH *)addr;
	path_size += le16_to_cpu(path->usSize);

	if (device_support & le16_to_cpu(path->usDeviceTag)) {
	    uint8_t con_obj_id, con_obj_num, con_obj_type;

	    con_obj_id = (le16_to_cpu(path->usConnObjectId) & OBJECT_ID_MASK) >> OBJECT_ID_SHIFT;
	    con_obj_num = (le16_to_cpu(path->usConnObjectId) & ENUM_ID_MASK) >> ENUM_ID_SHIFT;
	    con_obj_type = (le16_to_cpu(path->usConnObjectId) & OBJECT_TYPE_MASK) >> OBJECT_TYPE_SHIFT;

	    if ((le16_to_cpu(path->usDeviceTag) == ATOM_DEVICE_TV1_SUPPORT) ||
		(le16_to_cpu(path->usDeviceTag) == ATOM_DEVICE_TV2_SUPPORT)) {
		if (!enable_tv) {
		    info->BiosConnector[i].valid = FALSE;
		    continue;
		}
	    }

	    /* don't support CV yet */
	    if (le16_to_cpu(path->usDeviceTag) == ATOM_DEVICE_CV_SUPPORT) {
		info->BiosConnector[i].valid = FALSE;
		continue;
	    }

	    if (info->IsIGP &&
		(con_obj_id == CONNECTOR_OBJECT_ID_PCIE_CONNECTOR)) {
		uint32_t slot_config, ct;

		igp_obj = info->atomBIOS->atomDataPtr->IntegratedSystemInfo.IntegratedSystemInfo_v2;

		if (!igp_obj)
		    info->BiosConnector[i].ConnectorType = object_connector_convert[con_obj_id];
		else {
		    if (con_obj_num == 1)
			slot_config = igp_obj->ulDDISlot1Config;
		    else
			slot_config = igp_obj->ulDDISlot2Config;

		    ct = (slot_config  >> 16) & 0xff;
		    info->BiosConnector[i].ConnectorType = object_connector_convert[ct];
		    info->BiosConnector[i].connector_object_id = ct;
		    info->BiosConnector[i].igp_lane_info = slot_config & 0xffff;
		}
	    } else {
		info->BiosConnector[i].ConnectorType = object_connector_convert[con_obj_id];
		info->BiosConnector[i].connector_object_id = con_obj_id;
	    }

	    if (info->BiosConnector[i].ConnectorType == CONNECTOR_NONE) {
		info->BiosConnector[i].valid = FALSE;
		continue;
	    } else
		info->BiosConnector[i].valid = TRUE;
	    info->BiosConnector[i].devices = le16_to_cpu(path->usDeviceTag);
	    info->BiosConnector[i].connector_object = le16_to_cpu(path->usConnObjectId);

	    for (j = 0; j < ((le16_to_cpu(path->usSize) - 8) / 2); j++) {
		uint8_t enc_obj_id, enc_obj_num, enc_obj_type;

		enc_obj_id = (le16_to_cpu(path->usGraphicObjIds[j]) & OBJECT_ID_MASK) >> OBJECT_ID_SHIFT;
		enc_obj_num = (le16_to_cpu(path->usGraphicObjIds[j]) & ENUM_ID_MASK) >> ENUM_ID_SHIFT;
		enc_obj_type = (le16_to_cpu(path->usGraphicObjIds[j]) & OBJECT_TYPE_MASK) >> OBJECT_TYPE_SHIFT;

		if (enc_obj_type == GRAPH_OBJECT_TYPE_ENCODER) {
		    if (enc_obj_num == 2)
			info->BiosConnector[i].linkb = TRUE;
		    else
			info->BiosConnector[i].linkb = FALSE;

		    if (!radeon_add_encoder(pScrn, enc_obj_id, le16_to_cpu(path->usDeviceTag)))
			return FALSE;
		}
	    }

	    /* look up gpio for ddc */
	    if ((le16_to_cpu(path->usDeviceTag) & (ATOM_DEVICE_TV_SUPPORT | ATOM_DEVICE_CV_SUPPORT)) == 0) {
		for (j = 0; j < con_obj->ucNumberOfObjects; j++) {
		    if (le16_to_cpu(path->usConnObjectId) == le16_to_cpu(con_obj->asObjects[j].usObjectID)) {
			ATOM_COMMON_RECORD_HEADER *Record = (ATOM_COMMON_RECORD_HEADER *)
			    ((char *)&atomDataPtr->Object_Header->sHeader
			     + le16_to_cpu(con_obj->asObjects[j].usRecordOffset));

			while (Record->ucRecordType > 0
			       && Record->ucRecordType <= ATOM_MAX_OBJECT_RECORD_NUMBER ) {

			    /*ErrorF("record type %d\n", Record->ucRecordType);*/
			    switch (Record->ucRecordType) {
			    case ATOM_I2C_RECORD_TYPE:
				info->BiosConnector[i].ddc_i2c =
				    rhdAtomParseI2CRecord(pScrn, info->atomBIOS,
							  (ATOM_I2C_RECORD *)Record, j);
				break;
			    case ATOM_HPD_INT_RECORD_TYPE:
				info->BiosConnector[i].hpd_id =
				    radeon_lookup_hpd_id(pScrn,
							 (ATOM_HPD_INT_RECORD *)Record);
				break;
			    case ATOM_CONNECTOR_DEVICE_TAG_RECORD_TYPE:
				break;
			    }

			    Record = (ATOM_COMMON_RECORD_HEADER*)
				((char *)Record + Record->ucRecordSize);
			}
			break;
		    }
		}
	    }
	}
	RADEONApplyATOMQuirks(pScrn, i);
    }

    for (i = 0; i < ATOM_MAX_SUPPORTED_DEVICE; i++) {
	if (info->BiosConnector[i].valid) {
	    /* shared connectors */
	    for (j = 0; j < ATOM_MAX_SUPPORTED_DEVICE; j++) {
		if (info->BiosConnector[j].valid && (i != j) ) {
		    if (info->BiosConnector[i].connector_object == info->BiosConnector[j].connector_object) {
			info->BiosConnector[i].devices |= info->BiosConnector[j].devices;
			info->BiosConnector[j].valid = FALSE;
		    }
		}
	    }
	    /* shared ddc */
	    for (j = 0; j < ATOM_MAX_SUPPORTED_DEVICE; j++) {
		if (info->BiosConnector[j].valid && (i != j) ) {
		    if (info->BiosConnector[i].i2c_line_mux == info->BiosConnector[j].i2c_line_mux) {
			info->BiosConnector[i].shared_ddc = TRUE;
			info->BiosConnector[j].shared_ddc = TRUE;
		    }
		}
	    }
	}
    }

    return TRUE;
}

static void
RADEONGetATOMLVDSInfo(ScrnInfoPtr pScrn, radeon_lvds_ptr lvds)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    radeon_native_mode_ptr native_mode = &lvds->native_mode;
    atomDataTablesPtr atomDataPtr;
    uint8_t crev, frev;
    uint16_t misc;

    atomDataPtr = info->atomBIOS->atomDataPtr;

    if (!rhdAtomGetTableRevisionAndSize(
	    (ATOM_COMMON_TABLE_HEADER *)(atomDataPtr->LVDS_Info.base),
	    &frev,&crev,NULL)) {
	return;
    }

    switch (crev) {
    case 1:
	native_mode->PanelXRes = le16_to_cpu(atomDataPtr->LVDS_Info.LVDS_Info->sLCDTiming.usHActive);
	native_mode->PanelYRes = le16_to_cpu(atomDataPtr->LVDS_Info.LVDS_Info->sLCDTiming.usVActive);
	native_mode->DotClock   = le16_to_cpu(atomDataPtr->LVDS_Info.LVDS_Info->sLCDTiming.usPixClk) * 10;
	native_mode->HBlank     = le16_to_cpu(atomDataPtr->LVDS_Info.LVDS_Info->sLCDTiming.usHBlanking_Time);
	native_mode->HOverPlus  = le16_to_cpu(atomDataPtr->LVDS_Info.LVDS_Info->sLCDTiming.usHSyncOffset);
	native_mode->HSyncWidth = le16_to_cpu(atomDataPtr->LVDS_Info.LVDS_Info->sLCDTiming.usHSyncWidth);
	native_mode->VBlank     = le16_to_cpu(atomDataPtr->LVDS_Info.LVDS_Info->sLCDTiming.usVBlanking_Time);
	native_mode->VOverPlus  = le16_to_cpu(atomDataPtr->LVDS_Info.LVDS_Info->sLCDTiming.usVSyncOffset);
	native_mode->VSyncWidth = le16_to_cpu(atomDataPtr->LVDS_Info.LVDS_Info->sLCDTiming.usVSyncWidth);
	misc = le16_to_cpu(atomDataPtr->LVDS_Info.LVDS_Info->sLCDTiming.susModeMiscInfo.usAccess);
	if (misc & ATOM_VSYNC_POLARITY)
	    native_mode->Flags |= V_NVSYNC;
	if (misc & ATOM_HSYNC_POLARITY)
	    native_mode->Flags |= V_NHSYNC;
	if (misc & ATOM_COMPOSITESYNC)
	    native_mode->Flags |= V_CSYNC;
	if (misc & ATOM_INTERLACE)
	    native_mode->Flags |= V_INTERLACE;
	if (misc & ATOM_DOUBLE_CLOCK_MODE)
	    native_mode->Flags |= V_DBLSCAN;
	lvds->PanelPwrDly = le16_to_cpu(atomDataPtr->LVDS_Info.LVDS_Info->usOffDelayInMs);
	lvds->lvds_misc   =  atomDataPtr->LVDS_Info.LVDS_Info->ucLVDS_Misc;
	lvds->lvds_ss_id  =  atomDataPtr->LVDS_Info.LVDS_Info->ucSS_Id;
	break;
    case 2:
	native_mode->PanelXRes = le16_to_cpu(atomDataPtr->LVDS_Info.LVDS_Info_v12->sLCDTiming.usHActive);
	native_mode->PanelYRes = le16_to_cpu(atomDataPtr->LVDS_Info.LVDS_Info_v12->sLCDTiming.usVActive);
	native_mode->DotClock   = le16_to_cpu(atomDataPtr->LVDS_Info.LVDS_Info_v12->sLCDTiming.usPixClk) * 10;
	native_mode->HBlank     = le16_to_cpu(atomDataPtr->LVDS_Info.LVDS_Info_v12->sLCDTiming.usHBlanking_Time);
	native_mode->HOverPlus  = le16_to_cpu(atomDataPtr->LVDS_Info.LVDS_Info_v12->sLCDTiming.usHSyncOffset);
	native_mode->HSyncWidth = le16_to_cpu(atomDataPtr->LVDS_Info.LVDS_Info_v12->sLCDTiming.usHSyncWidth);
	native_mode->VBlank     = le16_to_cpu(atomDataPtr->LVDS_Info.LVDS_Info_v12->sLCDTiming.usVBlanking_Time);
	native_mode->VOverPlus  = le16_to_cpu(atomDataPtr->LVDS_Info.LVDS_Info_v12->sLCDTiming.usVSyncOffset);
	native_mode->VSyncWidth = le16_to_cpu(atomDataPtr->LVDS_Info.LVDS_Info_v12->sLCDTiming.usVSyncWidth);
	misc = le16_to_cpu(atomDataPtr->LVDS_Info.LVDS_Info_v12->sLCDTiming.susModeMiscInfo.usAccess);
	if (misc & ATOM_VSYNC_POLARITY)
	    native_mode->Flags |= V_NVSYNC;
	if (misc & ATOM_HSYNC_POLARITY)
	    native_mode->Flags |= V_NHSYNC;
	if (misc & ATOM_COMPOSITESYNC)
	    native_mode->Flags |= V_CSYNC;
	if (misc & ATOM_INTERLACE)
	    native_mode->Flags |= V_INTERLACE;
	if (misc & ATOM_DOUBLE_CLOCK_MODE)
	    native_mode->Flags |= V_DBLSCAN;
	lvds->PanelPwrDly = le16_to_cpu(atomDataPtr->LVDS_Info.LVDS_Info_v12->usOffDelayInMs);
	lvds->lvds_misc   =  atomDataPtr->LVDS_Info.LVDS_Info_v12->ucLVDS_Misc;
	lvds->lvds_ss_id  =  atomDataPtr->LVDS_Info.LVDS_Info_v12->ucSS_Id;
	break;
    }
    native_mode->Flags = 0;

    if (lvds->PanelPwrDly > 2000 || lvds->PanelPwrDly < 0)
	lvds->PanelPwrDly = 2000;

    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
	       "LVDS Info:\n"
	       "XRes: %d, YRes: %d, DotClock: %d\n"
	       "HBlank: %d, HOverPlus: %d, HSyncWidth: %d\n"
	       "VBlank: %d, VOverPlus: %d, VSyncWidth: %d\n",
	       native_mode->PanelXRes, native_mode->PanelYRes, native_mode->DotClock,
	       native_mode->HBlank, native_mode->HOverPlus, native_mode->HSyncWidth,
	       native_mode->VBlank, native_mode->VOverPlus, native_mode->VSyncWidth);
}

void
RADEONATOMGetIGPInfo(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    atomDataTablesPtr atomDataPtr;
    unsigned short size;
    uint8_t crev, frev;

    atomDataPtr = info->atomBIOS->atomDataPtr;

    if (!rhdAtomGetTableRevisionAndSize((ATOM_COMMON_TABLE_HEADER *)(atomDataPtr->IntegratedSystemInfo.base), &frev, &crev, &size))
	return;

    switch (crev) {
    case 1:
	info->igp_sideport_mclk = atomDataPtr->IntegratedSystemInfo.IntegratedSystemInfo->ulBootUpMemoryClock / 100.0;
	info->igp_system_mclk = le16_to_cpu(atomDataPtr->IntegratedSystemInfo.IntegratedSystemInfo->usK8MemoryClock);
	info->igp_ht_link_clk = le16_to_cpu(atomDataPtr->IntegratedSystemInfo.IntegratedSystemInfo->usFSBClock);
	info->igp_ht_link_width = atomDataPtr->IntegratedSystemInfo.IntegratedSystemInfo->ucHTLinkWidth;
	break;
    case 2:
	info->igp_sideport_mclk = atomDataPtr->IntegratedSystemInfo.IntegratedSystemInfo_v2->ulBootUpSidePortClock / 100.0;
	info->igp_system_mclk = atomDataPtr->IntegratedSystemInfo.IntegratedSystemInfo_v2->ulBootUpUMAClock / 100.0;
	info->igp_ht_link_clk = atomDataPtr->IntegratedSystemInfo.IntegratedSystemInfo_v2->ulHTLinkFreq / 100.0;
	info->igp_ht_link_width = le16_to_cpu(atomDataPtr->IntegratedSystemInfo.IntegratedSystemInfo_v2->usMinHTLinkWidth);
	break;
    }
}

Bool
RADEONGetATOMTVInfo(xf86OutputPtr output)
{
    ScrnInfoPtr pScrn = output->scrn;
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    radeon_tvout_ptr tvout = &radeon_output->tvout;
    ATOM_ANALOG_TV_INFO *tv_info;

    tv_info = info->atomBIOS->atomDataPtr->AnalogTV_Info.AnalogTV_Info;

    if (!tv_info)
	return FALSE;

    switch(tv_info->ucTV_BootUpDefaultStandard) {
    case NTSCJ_SUPPORT:
	tvout->default_tvStd = TV_STD_NTSC_J;
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Default TV standard: NTSC-J\n");
	break;
    case PAL_SUPPORT:
	tvout->default_tvStd = TV_STD_PAL;
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Default TV standard: PAL\n");
	break;
    case PALM_SUPPORT:
	tvout->default_tvStd = TV_STD_PAL_M;
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Default TV standard: PAL-M\n");
	break;
    case PAL60_SUPPORT:
	tvout->default_tvStd = TV_STD_PAL_60;
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Default TV standard: PAL-60\n");
	break;
    default:
    case NTSC_SUPPORT:
	tvout->default_tvStd = TV_STD_NTSC;
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Default TV standard: NTSC\n");
	break;
    }

    tvout->tvStd = tvout->default_tvStd;

    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "TV standards supported by chip: ");
    tvout->SupportedTVStds = tvout->default_tvStd;
    if (tv_info->ucTV_SupportedStandard & NTSC_SUPPORT) {
	ErrorF("NTSC ");
	tvout->SupportedTVStds |= TV_STD_NTSC;
    }
    if (tv_info->ucTV_SupportedStandard & NTSCJ_SUPPORT) {
	ErrorF("NTSC-J ");
	tvout->SupportedTVStds |= TV_STD_NTSC_J;
    }
    if (tv_info->ucTV_SupportedStandard & PAL_SUPPORT) {
	ErrorF("PAL ");
	tvout->SupportedTVStds |= TV_STD_PAL;
    }
    if (tv_info->ucTV_SupportedStandard & PALM_SUPPORT) {
	ErrorF("PAL-M ");
	tvout->SupportedTVStds |= TV_STD_PAL_M;
    }
    if (tv_info->ucTV_SupportedStandard & PAL60_SUPPORT) {
	ErrorF("PAL-60 ");
	tvout->SupportedTVStds |= TV_STD_PAL_60;
    }
    ErrorF("\n");

    if (tv_info->ucExt_TV_ASIC_ID) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Unknown external TV ASIC\n");
	return FALSE;
    }

    return TRUE;
}

Bool
RADEONGetATOMClockInfo(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr info = RADEONPTR (pScrn);
    RADEONPLLPtr pll = &info->pll;
    atomDataTablesPtr atomDataPtr;
    uint8_t crev, frev;

    atomDataPtr = info->atomBIOS->atomDataPtr;
    if (!rhdAtomGetTableRevisionAndSize(
	    (ATOM_COMMON_TABLE_HEADER *)(atomDataPtr->FirmwareInfo.base),
	    &crev,&frev,NULL)) {
	return FALSE;
    }

    switch(crev) {
    case 1:
	info->sclk = le32_to_cpu(atomDataPtr->FirmwareInfo.FirmwareInfo->ulDefaultEngineClock) / 100.0;
	info->mclk = le32_to_cpu(atomDataPtr->FirmwareInfo.FirmwareInfo->ulDefaultMemoryClock) / 100.0;
	pll->xclk = le16_to_cpu(atomDataPtr->FirmwareInfo.FirmwareInfo->usMaxPixelClock);
	pll->pll_in_min = le16_to_cpu(atomDataPtr->FirmwareInfo.FirmwareInfo->usMinPixelClockPLL_Input);
	pll->pll_in_max = le16_to_cpu(atomDataPtr->FirmwareInfo.FirmwareInfo->usMaxPixelClockPLL_Input);
	pll->pll_out_min = le16_to_cpu(atomDataPtr->FirmwareInfo.FirmwareInfo->usMinPixelClockPLL_Output);
	pll->pll_out_max = le32_to_cpu(atomDataPtr->FirmwareInfo.FirmwareInfo->ulMaxPixelClockPLL_Output);
	pll->reference_freq = le16_to_cpu(atomDataPtr->FirmwareInfo.FirmwareInfo->usReferenceClock);
	break;
    case 2:
    case 3:
    case 4:
    default:
	info->sclk = le32_to_cpu(atomDataPtr->FirmwareInfo.FirmwareInfo_V_1_2->ulDefaultEngineClock) / 100.0;
	info->mclk = le32_to_cpu(atomDataPtr->FirmwareInfo.FirmwareInfo_V_1_2->ulDefaultMemoryClock) / 100.0;
	pll->xclk = le16_to_cpu(atomDataPtr->FirmwareInfo.FirmwareInfo_V_1_2->usMaxPixelClock);
	pll->pll_in_min = le16_to_cpu(atomDataPtr->FirmwareInfo.FirmwareInfo_V_1_2->usMinPixelClockPLL_Input);
	pll->pll_in_max = le16_to_cpu(atomDataPtr->FirmwareInfo.FirmwareInfo_V_1_2->usMaxPixelClockPLL_Input);
	pll->pll_out_min = le32_to_cpu(atomDataPtr->FirmwareInfo.FirmwareInfo_V_1_2->ulMinPixelClockPLL_Output);
	pll->pll_out_max = le32_to_cpu(atomDataPtr->FirmwareInfo.FirmwareInfo_V_1_2->ulMaxPixelClockPLL_Output);
	pll->reference_freq = le16_to_cpu(atomDataPtr->FirmwareInfo.FirmwareInfo_V_1_2->usReferenceClock);
	break;
    }
    pll->reference_div = 0;
    if (pll->pll_out_min == 0) {
	if (IS_AVIVO_VARIANT)
	    pll->pll_out_min = 64800;
	else
	    pll->pll_out_min = 20000;
    }

    /* limiting the range is a good thing in most cases
     * as it limits the number of matching pll combinations,
     * however, some duallink DVI monitors seem to prefer combinations that
     * would be limited by this.  This may need to be revisited
     * per chip family.
     */
    if (!xf86ReturnOptValBool(info->Options, OPTION_NEW_PLL, TRUE)) {
	if (pll->pll_out_min > 64800)
	    pll->pll_out_min = 64800;
    }

    if (IS_DCE4_VARIANT) {
	info->default_dispclk =
	    le32_to_cpu(atomDataPtr->FirmwareInfo.FirmwareInfo_V_2_1->ulDefaultDispEngineClkFreq);
	if (info->default_dispclk == 0)
	    info->default_dispclk = 60000;
	info->dp_extclk = le16_to_cpu(atomDataPtr->FirmwareInfo.FirmwareInfo_V_2_1->usUniphyDPModeExtClkFreq);
    }
    return TRUE;
}

Bool
RADEONATOMGetTVTimings(ScrnInfoPtr pScrn, int index, DisplayModePtr mode)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    ATOM_ANALOG_TV_INFO *tv_info;
    ATOM_ANALOG_TV_INFO_V1_2 *tv_info_v1_2;
    ATOM_DTD_FORMAT *dtd_timings;
    atomDataTablesPtr atomDataPtr;
    uint8_t crev, frev;
    uint16_t misc;

    atomDataPtr = info->atomBIOS->atomDataPtr;
    if (!rhdAtomGetTableRevisionAndSize(
	    (ATOM_COMMON_TABLE_HEADER *)(atomDataPtr->AnalogTV_Info.base),
	    &crev,&frev,NULL)) {
	return FALSE;
    }

    switch(crev) {
    case 1:
	tv_info = atomDataPtr->AnalogTV_Info.AnalogTV_Info;

	if (index > MAX_SUPPORTED_TV_TIMING)
	    return FALSE;

	mode->CrtcHTotal     = le16_to_cpu(tv_info->aModeTimings[index].usCRTC_H_Total);
	mode->CrtcHDisplay   = le16_to_cpu(tv_info->aModeTimings[index].usCRTC_H_Disp);
	mode->CrtcHSyncStart = le16_to_cpu(tv_info->aModeTimings[index].usCRTC_H_SyncStart);
	mode->CrtcHSyncEnd   = le16_to_cpu(tv_info->aModeTimings[index].usCRTC_H_SyncStart) +
	                       le16_to_cpu(tv_info->aModeTimings[index].usCRTC_H_SyncWidth);

	mode->CrtcVTotal     = le16_to_cpu(tv_info->aModeTimings[index].usCRTC_V_Total);
	mode->CrtcVDisplay   = le16_to_cpu(tv_info->aModeTimings[index].usCRTC_V_Disp);
	mode->CrtcVSyncStart = le16_to_cpu(tv_info->aModeTimings[index].usCRTC_V_SyncStart);
	mode->CrtcVSyncEnd   = le16_to_cpu(tv_info->aModeTimings[index].usCRTC_V_SyncStart) +
	                       le16_to_cpu(tv_info->aModeTimings[index].usCRTC_V_SyncWidth);

	mode->Flags = 0;
	misc = le16_to_cpu(tv_info->aModeTimings[index].susModeMiscInfo.usAccess);
	if (misc & ATOM_VSYNC_POLARITY)
	    mode->Flags |= V_NVSYNC;
	if (misc & ATOM_HSYNC_POLARITY)
	    mode->Flags |= V_NHSYNC;
	if (misc & ATOM_COMPOSITESYNC)
	    mode->Flags |= V_CSYNC;
	if (misc & ATOM_INTERLACE)
	    mode->Flags |= V_INTERLACE;
	if (misc & ATOM_DOUBLE_CLOCK_MODE)
	    mode->Flags |= V_DBLSCAN;

	mode->Clock = le16_to_cpu(tv_info->aModeTimings[index].usPixelClock) * 10;

	if (index == 1) {
		/* PAL timings appear to have wrong values for totals */
		mode->CrtcHTotal -= 1;
		mode->CrtcVTotal -= 1;
	}
	break;
    case 2:
	tv_info_v1_2 = atomDataPtr->AnalogTV_Info.AnalogTV_Info_v1_2;
	if (index > MAX_SUPPORTED_TV_TIMING_V1_2)
	    return FALSE;

	dtd_timings = &tv_info_v1_2->aModeTimings[index];
	mode->CrtcHTotal     = le16_to_cpu(dtd_timings->usHActive) + le16_to_cpu(dtd_timings->usHBlanking_Time);
	mode->CrtcHDisplay   = le16_to_cpu(dtd_timings->usHActive);
	mode->CrtcHSyncStart = le16_to_cpu(dtd_timings->usHActive) + le16_to_cpu(dtd_timings->usHSyncOffset);
	mode->CrtcHSyncEnd   = mode->CrtcHSyncStart + le16_to_cpu(dtd_timings->usHSyncWidth);

	mode->CrtcVTotal     = le16_to_cpu(dtd_timings->usVActive) + le16_to_cpu(dtd_timings->usVBlanking_Time);
	mode->CrtcVDisplay   = le16_to_cpu(dtd_timings->usVActive);
	mode->CrtcVSyncStart = le16_to_cpu(dtd_timings->usVActive) + le16_to_cpu(dtd_timings->usVSyncOffset);
	mode->CrtcVSyncEnd   = mode->CrtcVSyncStart + le16_to_cpu(dtd_timings->usVSyncWidth);

	mode->Flags = 0;
	misc = le16_to_cpu(dtd_timings->susModeMiscInfo.usAccess);
	if (misc & ATOM_VSYNC_POLARITY)
	    mode->Flags |= V_NVSYNC;
	if (misc & ATOM_HSYNC_POLARITY)
	    mode->Flags |= V_NHSYNC;
	if (misc & ATOM_COMPOSITESYNC)
	    mode->Flags |= V_CSYNC;
	if (misc & ATOM_INTERLACE)
	    mode->Flags |= V_INTERLACE;
	if (misc & ATOM_DOUBLE_CLOCK_MODE)
	    mode->Flags |= V_DBLSCAN;

	mode->Clock = le16_to_cpu(dtd_timings->usPixClk) * 10;

	break;
    }

    return TRUE;
}

uint32_t
radeon_get_encoder_id_from_supported_device(ScrnInfoPtr pScrn, uint32_t supported_device, int dac)
{
    RADEONInfoPtr info = RADEONPTR (pScrn);
    uint32_t ret = 0;

    switch (supported_device) {
    case ATOM_DEVICE_CRT1_SUPPORT:
    case ATOM_DEVICE_TV1_SUPPORT:
    case ATOM_DEVICE_TV2_SUPPORT:
    case ATOM_DEVICE_CRT2_SUPPORT:
    case ATOM_DEVICE_CV_SUPPORT:
	switch (dac) {
	    // primary dac
	case 1:
	    if ((info->ChipFamily == CHIP_FAMILY_RS300) ||
		(info->ChipFamily == CHIP_FAMILY_RS400) ||
		(info->ChipFamily == CHIP_FAMILY_RS480))
		ret = ENCODER_OBJECT_ID_INTERNAL_DAC2;
	    else if (IS_AVIVO_VARIANT)
		ret = ENCODER_OBJECT_ID_INTERNAL_KLDSCP_DAC1;
	    else
		ret = ENCODER_OBJECT_ID_INTERNAL_DAC1;
	    break;
	    // secondary dac
	case 2:
	    if (IS_AVIVO_VARIANT)
		ret = ENCODER_OBJECT_ID_INTERNAL_KLDSCP_DAC2;
	    else {
		/*if (info->ChipFamily == CHIP_FAMILY_R200)
		    ret = ENCODER_OBJECT_ID_INTERNAL_DVO1;
		    else*/
		    ret = ENCODER_OBJECT_ID_INTERNAL_DAC2;
	    }
	    break;
	    // external dac
	case 3:
	    if (IS_AVIVO_VARIANT)
		ret = ENCODER_OBJECT_ID_INTERNAL_KLDSCP_DVO1;
	    else
		ret = ENCODER_OBJECT_ID_INTERNAL_DVO1;
	    break;
	}
	break;
    case ATOM_DEVICE_LCD1_SUPPORT:
	if (IS_AVIVO_VARIANT)
	    ret = ENCODER_OBJECT_ID_INTERNAL_LVTM1;
	else
	    ret = ENCODER_OBJECT_ID_INTERNAL_LVDS;
	break;
    case ATOM_DEVICE_DFP1_SUPPORT:
	if ((info->ChipFamily == CHIP_FAMILY_RS300) ||
	    (info->ChipFamily == CHIP_FAMILY_RS400) ||
	    (info->ChipFamily == CHIP_FAMILY_RS480))
	    ret = ENCODER_OBJECT_ID_INTERNAL_DVO1;
	else if (IS_AVIVO_VARIANT)
	    ret = ENCODER_OBJECT_ID_INTERNAL_KLDSCP_TMDS1;
	else
	    ret = ENCODER_OBJECT_ID_INTERNAL_TMDS1;
	break;
    case ATOM_DEVICE_LCD2_SUPPORT:
    case ATOM_DEVICE_DFP2_SUPPORT:
	if ((info->ChipFamily == CHIP_FAMILY_RS600) ||
	    (info->ChipFamily == CHIP_FAMILY_RS690) ||
	    (info->ChipFamily == CHIP_FAMILY_RS740))
	    ret = ENCODER_OBJECT_ID_INTERNAL_DDI;
	else if (IS_AVIVO_VARIANT)
	    ret = ENCODER_OBJECT_ID_INTERNAL_KLDSCP_DVO1;
	else
	    ret = ENCODER_OBJECT_ID_INTERNAL_DVO1;
	break;
    case ATOM_DEVICE_DFP3_SUPPORT:
	ret = ENCODER_OBJECT_ID_INTERNAL_LVTM1;
	break;
    }

    return ret;
}

Bool
RADEONGetATOMConnectorInfoFromBIOSConnectorTable (ScrnInfoPtr pScrn)
{
    RADEONInfoPtr info = RADEONPTR (pScrn);
    atomDataTablesPtr atomDataPtr;
    uint8_t crev, frev;
    int i, j;
    Bool enable_tv = FALSE;

    if (xf86ReturnOptValBool(info->Options, OPTION_ATOM_TVOUT, FALSE))
	enable_tv = TRUE;

    atomDataPtr = info->atomBIOS->atomDataPtr;

    if (!rhdAtomGetTableRevisionAndSize(
	    &(atomDataPtr->SupportedDevicesInfo.SupportedDevicesInfo->sHeader),
	    &crev,&frev,NULL)) {
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "No Device Info Table found!\n");
	return FALSE;
    }

    for (i = 0; i < ATOM_MAX_SUPPORTED_DEVICE; i++) {
	ATOM_CONNECTOR_INFO_I2C ci
	    = atomDataPtr->SupportedDevicesInfo.SupportedDevicesInfo->asConnInfo[i];

	if (!(le16_to_cpu(atomDataPtr->SupportedDevicesInfo
			  .SupportedDevicesInfo->usDeviceSupport) & (1 << i))) {
	    info->BiosConnector[i].valid = FALSE;
	    continue;
	}

	/* don't support CV yet */
	if (i == ATOM_DEVICE_CV_INDEX) {
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Skipping Component Video\n");
	    info->BiosConnector[i].valid = FALSE;
	    continue;
	}

	if (!enable_tv && (i == ATOM_DEVICE_TV1_INDEX)) {
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Skipping TV-Out\n");
	    info->BiosConnector[i].valid = FALSE;
	    continue;
	}

	info->BiosConnector[i].valid = TRUE;
	info->BiosConnector[i].load_detection = TRUE;
	info->BiosConnector[i].shared_ddc = FALSE;
	info->BiosConnector[i].output_id = ci.sucI2cId.ucAccess;
	info->BiosConnector[i].devices = (1 << i);
	info->BiosConnector[i].ConnectorType = ci.sucConnectorInfo.sbfAccess.bfConnectorType;

	if (info->BiosConnector[i].ConnectorType == CONNECTOR_NONE) {
	    info->BiosConnector[i].valid = FALSE;
	    continue;
	}

	/* don't assign a gpio for tv */
	if ((i == ATOM_DEVICE_TV1_INDEX) ||
	    (i == ATOM_DEVICE_TV2_INDEX) ||
	    (i == ATOM_DEVICE_CV_INDEX))
	    info->BiosConnector[i].ddc_i2c.valid = FALSE;
	else
	    info->BiosConnector[i].ddc_i2c =
		RADEONLookupGPIOLineForDDC(pScrn, ci.sucI2cId.ucAccess);

	if (!radeon_add_encoder(pScrn,
			   radeon_get_encoder_id_from_supported_device(pScrn, (1 << i),
					  ci.sucConnectorInfo.sbfAccess.bfAssociatedDAC),
				(1 << i)))
	    return FALSE;

	/* Always set the connector type to VGA for CRT1/CRT2. if they are
	 * shared with a DVI port, we'll pick up the DVI connector below when we
	 * merge the outputs
	 */
	if ((i == ATOM_DEVICE_CRT1_INDEX || i == ATOM_DEVICE_CRT2_INDEX) &&
	    (info->BiosConnector[i].ConnectorType == CONNECTOR_DVI_I ||
	     info->BiosConnector[i].ConnectorType == CONNECTOR_DVI_D ||
	     info->BiosConnector[i].ConnectorType == CONNECTOR_DVI_A)) {
	    info->BiosConnector[i].ConnectorType = CONNECTOR_VGA;
	}

	if (crev > 1) {
	    ATOM_CONNECTOR_INC_SRC_BITMAP isb
		= atomDataPtr->SupportedDevicesInfo
		.SupportedDevicesInfo_HD->asIntSrcInfo[i];

	    switch (isb.ucIntSrcBitmap) {
		case 0x4:
		    info->BiosConnector[i].hpd_mask = 0x00000001;
		    break;
		case 0xa:
		    info->BiosConnector[i].hpd_mask = 0x00000100;
		    break;
		default:
		    info->BiosConnector[i].hpd_mask = 0;
		    break;
	    }
	} else
	    info->BiosConnector[i].hpd_mask = 0;

	RADEONApplyATOMQuirks(pScrn, i);

    }

    /* CRTs/DFPs may share a port */
    for (i = 0; i < ATOM_MAX_SUPPORTED_DEVICE; i++) {
	if (info->BiosConnector[i].valid) {
	    for (j = 0; j < ATOM_MAX_SUPPORTED_DEVICE; j++) {
		if (info->BiosConnector[j].valid && (i != j) ) {
		    if (info->BiosConnector[i].output_id == info->BiosConnector[j].output_id) {
			if (((i == ATOM_DEVICE_DFP1_INDEX) ||
			     (i == ATOM_DEVICE_DFP2_INDEX) ||
			     (i == ATOM_DEVICE_DFP3_INDEX)) &&
			    ((j == ATOM_DEVICE_CRT1_INDEX) ||
			     (j == ATOM_DEVICE_CRT2_INDEX))) {
			    info->BiosConnector[i].devices |= info->BiosConnector[j].devices;
			    if (info->BiosConnector[i].ConnectorType == CONNECTOR_DVI_D)
				info->BiosConnector[i].ConnectorType = CONNECTOR_DVI_I;
			    info->BiosConnector[j].valid = FALSE;
			} else if (((j == ATOM_DEVICE_DFP1_INDEX) ||
				    (j == ATOM_DEVICE_DFP2_INDEX) ||
				    (j == ATOM_DEVICE_DFP3_INDEX)) &&
				   ((i == ATOM_DEVICE_CRT1_INDEX) ||
				    (i == ATOM_DEVICE_CRT2_INDEX))) {
			    info->BiosConnector[j].devices |= info->BiosConnector[i].devices;
			    if (info->BiosConnector[j].ConnectorType == CONNECTOR_DVI_D)
				info->BiosConnector[j].ConnectorType = CONNECTOR_DVI_I;
			    info->BiosConnector[i].valid = FALSE;
			} else {
			    info->BiosConnector[i].shared_ddc = TRUE;
			    info->BiosConnector[j].shared_ddc = TRUE;
			}
			/* other possible combos?  */
		    }
		}
	    }
	}
    }

    return TRUE;
}

# ifdef ATOM_BIOS_PARSER
static AtomBiosResult
rhdAtomExec (atomBiosHandlePtr handle,
	     AtomBiosRequestID unused, AtomBiosArgPtr data)
{
    RADEONInfoPtr info = RADEONPTR (xf86Screens[handle->scrnIndex]);
    Bool ret = FALSE;
    char *msg;
    int idx = data->exec.index;
    void *pspace = data->exec.pspace;
    pointer *dataSpace = data->exec.dataSpace;

    //RHDFUNCI(handle->scrnIndex);

    if (dataSpace) {
	if (!handle->fbBase && !handle->scratchBase)
	    return ATOM_FAILED;
	if (handle->fbBase) {
	    if (!info->FB) {
		xf86DrvMsg(handle->scrnIndex, X_ERROR, "%s: "
			   "Cannot exec AtomBIOS: framebuffer not mapped\n",
			   __func__);
		return ATOM_FAILED;
	    }
	    *dataSpace = (uint8_t*)info->FB + handle->fbBase;
	} else
	    *dataSpace = (uint8_t*)handle->scratchBase;
    }
    ret = ParseTableWrapper(pspace, idx, handle,
			    handle->BIOSBase,
			    &msg);
    if (!ret)
	xf86DrvMsg(handle->scrnIndex, X_ERROR, "%s\n",msg);
    else
	xf86DrvMsgVerb(handle->scrnIndex, X_INFO, 5, "%s\n",msg);

    return (ret) ? ATOM_SUCCESS : ATOM_FAILED;
}
# endif

AtomBiosResult
RHDAtomBiosFunc(int scrnIndex, atomBiosHandlePtr handle,
		AtomBiosRequestID id, AtomBiosArgPtr data)
{
    AtomBiosResult ret = ATOM_FAILED;
    int i;
    char *msg = NULL;
    enum msgDataFormat msg_f = MSG_FORMAT_NONE;
    AtomBiosRequestFunc req_func = NULL;

    //RHDFUNCI(scrnIndex);

    for (i = 0; AtomBiosRequestList[i].id != FUNC_END; i++) {
	if (id ==  AtomBiosRequestList[i].id) {
	    req_func = AtomBiosRequestList[i].request;
	    msg = AtomBiosRequestList[i].message;
	    msg_f = AtomBiosRequestList[i].message_format;
	    break;
	}
    }

    if (req_func == NULL) {
	xf86DrvMsg(scrnIndex, X_ERROR, "Unknown AtomBIOS request: %i\n",id);
	return ATOM_NOT_IMPLEMENTED;
    }
    /* Hack for now */
    if (id == ATOMBIOS_INIT)
	data->val = scrnIndex;

    if (id == ATOMBIOS_INIT || handle)
	ret = req_func(handle, id, data);

    if (ret == ATOM_SUCCESS) {

	switch (msg_f) {
	    case MSG_FORMAT_DEC:
		xf86DrvMsg(scrnIndex,X_INFO,"%s: %li\n", msg,
			   (unsigned long) data->val);
		break;
	    case MSG_FORMAT_HEX:
		xf86DrvMsg(scrnIndex,X_INFO,"%s: 0x%lx\n",msg ,
			   (unsigned long) data->val);
		break;
	    case MSG_FORMAT_NONE:
		xf86DrvMsgVerb(scrnIndex, 7, X_INFO,
			       "Call to %s succeeded\n", msg);
		break;
	}

    } else {

	char *result = (ret == ATOM_FAILED) ? "failed"
	    : "not implemented";
	switch (msg_f) {
	    case MSG_FORMAT_DEC:
	    case MSG_FORMAT_HEX:
		xf86DrvMsgVerb(scrnIndex, 1, X_WARNING,
			       "Call to %s %s\n", msg, result);
		break;
	    case MSG_FORMAT_NONE:
		xf86DrvMsg(scrnIndex,X_INFO,"Query for %s: %s\n", msg, result);
		    break;
	}
    }
    return ret;
}

# ifdef ATOM_BIOS_PARSER
VOID*
CailAllocateMemory(VOID *CAIL,UINT16 size)
{
    void *ret;
    CAILFUNC(CAIL);

    ret = malloc(size);
    memset(ret, 0, size);
    return ret;
}

VOID
CailReleaseMemory(VOID *CAIL, VOID *addr)
{
    CAILFUNC(CAIL);

    free(addr);
}

VOID
CailDelayMicroSeconds(VOID *CAIL, UINT32 delay)
{
    CAILFUNC(CAIL);

    usleep(delay);

    /*DEBUGP(xf86DrvMsg(((atomBiosHandlePtr)CAIL)->scrnIndex,X_INFO,"Delay %i usec\n",delay));*/
}

UINT32
CailReadATIRegister(VOID* CAIL, UINT32 idx)
{
    ScrnInfoPtr pScrn = xf86Screens[((atomBiosHandlePtr)CAIL)->scrnIndex];
    RADEONInfoPtr info = RADEONPTR(pScrn);
    RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);
    unsigned char *RADEONMMIO = pRADEONEnt->MMIO;
    UINT32 ret;
    UINT32 mm_reg = idx << 2;
    CAILFUNC(CAIL);

    if (mm_reg < info->MMIOSize)
	ret = INREG(mm_reg);
    else {
	OUTREG(RADEON_MM_INDEX, mm_reg);
	ret = INREG(RADEON_MM_DATA);
    }

    /*DEBUGP(ErrorF("%s(%x) = %x\n",__func__,idx << 2,ret));*/
    return ret;
}

VOID
CailWriteATIRegister(VOID *CAIL, UINT32 idx, UINT32 data)
{
    ScrnInfoPtr pScrn = xf86Screens[((atomBiosHandlePtr)CAIL)->scrnIndex];
    RADEONInfoPtr info = RADEONPTR(pScrn);
    RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);
    unsigned char *RADEONMMIO = pRADEONEnt->MMIO;
    UINT32 mm_reg = idx << 2;
    CAILFUNC(CAIL);

    if (mm_reg < info->MMIOSize)
	OUTREG(mm_reg, data);
    else {
	OUTREG(RADEON_MM_INDEX, mm_reg);
	OUTREG(RADEON_MM_DATA, data);
    }

    /*DEBUGP(ErrorF("%s(%x,%x)\n",__func__,idx << 2,data));*/
}

UINT32
CailReadFBData(VOID* CAIL, UINT32 idx)
{
    ScrnInfoPtr pScrn = xf86Screens[((atomBiosHandlePtr)CAIL)->scrnIndex];
    RADEONInfoPtr  info   = RADEONPTR(pScrn);
    UINT32 ret;

    CAILFUNC(CAIL);

    if (((atomBiosHandlePtr)CAIL)->fbBase) {
	uint8_t *FBBase = (uint8_t*)info->FB;
	ret =  *((uint32_t*)(FBBase + (((atomBiosHandlePtr)CAIL)->fbBase) + idx));
	/*DEBUGP(ErrorF("%s(%x) = %x\n",__func__,idx,ret));*/
    } else if (((atomBiosHandlePtr)CAIL)->scratchBase) {
	ret = *(uint32_t*)((uint8_t*)(((atomBiosHandlePtr)CAIL)->scratchBase) + idx);
	/*DEBUGP(ErrorF("%s(%x) = %x\n",__func__,idx,ret));*/
    } else {
	xf86DrvMsg(((atomBiosHandlePtr)CAIL)->scrnIndex,X_ERROR,
		   "%s: no fbbase set\n",__func__);
	return 0;
    }
    return ret;
}

VOID
CailWriteFBData(VOID *CAIL, UINT32 idx, UINT32 data)
{
    CAILFUNC(CAIL);

    /*DEBUGP(ErrorF("%s(%x,%x)\n",__func__,idx,data));*/
    if (((atomBiosHandlePtr)CAIL)->fbBase) {
	uint8_t *FBBase = (uint8_t*)
	    RADEONPTR(xf86Screens[((atomBiosHandlePtr)CAIL)->scrnIndex])->FB;
	*((uint32_t*)(FBBase + (((atomBiosHandlePtr)CAIL)->fbBase) + idx)) = data;
    } else if (((atomBiosHandlePtr)CAIL)->scratchBase) {
	*(uint32_t*)((uint8_t*)(((atomBiosHandlePtr)CAIL)->scratchBase) + idx) = data;
    } else
	xf86DrvMsg(((atomBiosHandlePtr)CAIL)->scrnIndex,X_ERROR,
		   "%s: no fbbase set\n",__func__);
}

ULONG
CailReadMC(VOID *CAIL, ULONG Address)
{
    ScrnInfoPtr pScrn = xf86Screens[((atomBiosHandlePtr)CAIL)->scrnIndex];
    ULONG ret;

    CAILFUNC(CAIL);

    ret = INMC(pScrn, Address);
    /*DEBUGP(ErrorF("%s(%x) = %x\n",__func__,Address,ret));*/
    return ret;
}

VOID
CailWriteMC(VOID *CAIL, ULONG Address, ULONG data)
{
    ScrnInfoPtr pScrn = xf86Screens[((atomBiosHandlePtr)CAIL)->scrnIndex];

    CAILFUNC(CAIL);
    /*DEBUGP(ErrorF("%s(%x,%x)\n",__func__,Address,data));*/
    OUTMC(pScrn, Address, data);
}

#ifdef XSERVER_LIBPCIACCESS

VOID
CailReadPCIConfigData(VOID*CAIL, VOID* ret, UINT32 idx,UINT16 size)
{
    pci_device_cfg_read(RADEONPTR(xf86Screens[((atomBiosHandlePtr)CAIL)->scrnIndex])->PciInfo,
				ret,idx << 2 , size >> 3, NULL);
}

VOID
CailWritePCIConfigData(VOID*CAIL,VOID*src,UINT32 idx,UINT16 size)
{
    pci_device_cfg_write(RADEONPTR(xf86Screens[((atomBiosHandlePtr)CAIL)->scrnIndex])->PciInfo,
			 src, idx << 2, size >> 3, NULL);
}

#else

VOID
CailReadPCIConfigData(VOID*CAIL, VOID* ret, UINT32 idx,UINT16 size)
{
    PCITAG tag = ((atomBiosHandlePtr)CAIL)->PciTag;

    CAILFUNC(CAIL);

    switch (size) {
	case 8:
	    *(uint8_t*)ret = pciReadByte(tag,idx << 2);
	    break;
	case 16:
	    *(uint16_t*)ret = pciReadWord(tag,idx << 2);
	    break;
	case 32:
	    *(uint32_t*)ret = pciReadLong(tag,idx << 2);
	    break;
	default:
	xf86DrvMsg(((atomBiosHandlePtr)CAIL)->scrnIndex,
		   X_ERROR,"%s: Unsupported size: %i\n",
		   __func__,(int)size);
	return;
	    break;
    }
    /*DEBUGP(ErrorF("%s(%x) = %x\n",__func__,idx,*(unsigned int*)ret));*/

}

VOID
CailWritePCIConfigData(VOID*CAIL,VOID*src,UINT32 idx,UINT16 size)
{
    PCITAG tag = ((atomBiosHandlePtr)CAIL)->PciTag;

    CAILFUNC(CAIL);
    /*DEBUGP(ErrorF("%s(%x,%x)\n",__func__,idx,(*(unsigned int*)src)));*/
    switch (size) {
	case 8:
	    pciWriteByte(tag,idx << 2,*(uint8_t*)src);
	    break;
	case 16:
	    pciWriteWord(tag,idx << 2,*(uint16_t*)src);
	    break;
	case 32:
	    pciWriteLong(tag,idx << 2,*(uint32_t*)src);
	    break;
	default:
	    xf86DrvMsg(((atomBiosHandlePtr)CAIL)->scrnIndex,X_ERROR,
		       "%s: Unsupported size: %i\n",__func__,(int)size);
	    break;
    }
}
#endif

ULONG
CailReadPLL(VOID *CAIL, ULONG Address)
{
    ScrnInfoPtr pScrn = xf86Screens[((atomBiosHandlePtr)CAIL)->scrnIndex];
    ULONG ret;

    CAILFUNC(CAIL);

    ret = RADEONINPLL(pScrn, Address);
    /*DEBUGP(ErrorF("%s(%x) = %x\n",__func__,Address,ret));*/
    return ret;
}

VOID
CailWritePLL(VOID *CAIL, ULONG Address,ULONG Data)
{
    ScrnInfoPtr pScrn = xf86Screens[((atomBiosHandlePtr)CAIL)->scrnIndex];
    CAILFUNC(CAIL);

    /*DEBUGP(ErrorF("%s(%x,%x)\n",__func__,Address,Data));*/
    RADEONOUTPLL(pScrn, Address, Data);
}

void
atombios_get_command_table_version(atomBiosHandlePtr atomBIOS, int index, int *major, int *minor)
{
    ATOM_MASTER_COMMAND_TABLE *cmd_table = (void *)(atomBIOS->BIOSBase + atomBIOS->cmd_offset);
    ATOM_MASTER_LIST_OF_COMMAND_TABLES *table_start;
    ATOM_COMMON_ROM_COMMAND_TABLE_HEADER *table_hdr;

    //unsigned short *ptr;
    unsigned short offset;

    table_start = &cmd_table->ListOfCommandTables;

    offset  = *(((unsigned short *)table_start) + index);

    offset = le16_to_cpu(offset);
    table_hdr = (ATOM_COMMON_ROM_COMMAND_TABLE_HEADER *)(atomBIOS->BIOSBase + offset);

    *major = table_hdr->CommonHeader.ucTableFormatRevision;
    *minor = table_hdr->CommonHeader.ucTableContentRevision;
}


UINT16 ATOM_BSWAP16(UINT16 x)
{
    return bswap_16(x);
}

UINT32 ATOM_BSWAP32(UINT32 x)
{
    return bswap_32(x);
}


#endif /* ATOM_BIOS */
