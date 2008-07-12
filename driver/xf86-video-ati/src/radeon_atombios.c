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
    SET_DATA_TABLE(AnalogTV_Info);
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
	handle->scratchBase = xcalloc(fb_size,1);
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
    asicInit.sASICInitClocks.ulDefaultEngineClock = data.val / 10;/*in 10 Khz*/
    RHDAtomBiosFunc(handle->scrnIndex, handle,
		    GET_DEFAULT_MEMORY_CLOCK,
		    &data);
    asicInit.sASICInitClocks.ulDefaultMemoryClock = data.val / 10;/*in 10 Khz*/
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
atombios_dyn_clk_setup(ScrnInfoPtr pScrn, int enable)
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
atombios_static_pwrmgt_setup(ScrnInfoPtr pScrn, int enable)
{
    RADEONInfoPtr info       = RADEONPTR(pScrn);
    ENABLE_ASIC_STATIC_PWR_MGT_PS_ALLOCATION pwrmgt_data;
    AtomBiosArgRec data;
    unsigned char *space;

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

    if (!(atomDataPtr = xcalloc(1, sizeof(atomDataTables)))) {
	xf86DrvMsg(scrnIndex,X_ERROR,"Cannot allocate memory for "
		   "ATOM BIOS data tabes\n");
	goto error;
    }
    if (!rhdAtomGetDataTable(scrnIndex, info->VBIOS, atomDataPtr, &cmd_offset, BIOSImageSize))
	goto error1;
    if (!(handle = xcalloc(1, sizeof(atomBiosHandleRec)))) {
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

# if ATOM_BIOS_PARSER
    /* Try to find out if BIOS has been posted (either by system or int10 */
    if (!rhdAtomGetFbBaseAndSize(handle, NULL, NULL)) {
	/* run AsicInit */
	if (!rhdAtomASICInit(handle))
	    xf86DrvMsg(scrnIndex, X_WARNING,
		       "%s: AsicInit failed. Won't be able to obtain in VRAM "
		       "FB scratch space\n",__func__);
    }
# endif

    data->atomhandle = handle;
    return ATOM_SUCCESS;

 error1:
    xfree(atomDataPtr);
 error:
    return ATOM_FAILED;
}

static AtomBiosResult
rhdAtomTearDown(atomBiosHandlePtr handle,
		AtomBiosRequestID unused1, AtomBiosArgPtr unused2)
{
    //RHDFUNC(handle);

    xfree(handle->BIOSBase);
    xfree(handle->atomDataPtr);
    if (handle->scratchBase) xfree(handle->scratchBase);
    xfree(handle);
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
	    *val = le32_to_cpu(atomDataPtr->VRAM_UsageByFirmware
			       ->asFirmwareVramReserveInfo[0].ulStartAddrUsedByFirmware);
	    break;
	case GET_FW_FB_SIZE:
	    *val =  le16_to_cpu(atomDataPtr->VRAM_UsageByFirmware
				->asFirmwareVramReserveInfo[0].usFirmwareUseInKb);
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

    if (!(mode = (DisplayModePtr)xcalloc(1,sizeof(DisplayModeRec))))
	return NULL;

    mode->CrtcHDisplay = mode->HDisplay = le16_to_cpu(dtd->usHActive);
    mode->CrtcVDisplay = mode->VDisplay = le16_to_cpu(dtd->usVActive);
    mode->CrtcHBlankStart = dtd->usHActive + dtd->ucHBorder;
    mode->CrtcHBlankEnd = mode->CrtcHBlankStart + le16_to_cpu(dtd->usHBlanking_Time);
    mode->CrtcHTotal = mode->HTotal = mode->CrtcHBlankEnd + dtd->ucHBorder;
    mode->CrtcVBlankStart = dtd->usVActive + dtd->ucVBorder;
    mode->CrtcVBlankEnd = mode->CrtcVBlankStart + le16_to_cpu(dtd->usVBlanking_Time);
    mode->CrtcVTotal = mode->VTotal = mode->CrtcVBlankEnd + dtd->ucVBorder;
    mode->CrtcHSyncStart = mode->HSyncStart = dtd->usHActive + le16_to_cpu(dtd->usHSyncOffset);
    mode->CrtcHSyncEnd = mode->HSyncEnd = mode->HSyncStart + le16_to_cpu(dtd->usHSyncWidth);
    mode->CrtcVSyncStart = mode->VSyncStart = dtd->usVActive + le16_to_cpu(dtd->usVSyncOffset);
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
		if (!(EDIDBlock = (unsigned char *)xalloc(
			  ((ATOM_FAKE_EDID_PATCH_RECORD*)record)->ucFakeEDIDLength)))
		    return NULL;
		memcpy(EDIDBlock,&((ATOM_FAKE_EDID_PATCH_RECORD*)record)->ucFakeEDIDString,
		       ((ATOM_FAKE_EDID_PATCH_RECORD*)record)->ucFakeEDIDLength);

		/* for testing */
		{
		    xf86MonPtr mon = xf86InterpretEDID(handle->scrnIndex,EDIDBlock);
		    xf86PrintEDID(mon);
		    xfree(mon);
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

    data->modes = NULL;

    atomDataPtr = handle->atomDataPtr;

    if (!rhdAtomGetTableRevisionAndSize(
	    (ATOM_COMMON_TABLE_HEADER *)(atomDataPtr->ComponentVideoInfo.base),
	    &frev,&crev,NULL)) {
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
      CONNECTOR_DIN,
      CONNECTOR_SCART,
      CONNECTOR_HDMI_TYPE_A,
      CONNECTOR_HDMI_TYPE_B,
      CONNECTOR_HDMI_TYPE_B,
      CONNECTOR_LVDS,
      CONNECTOR_DIN,
      CONNECTOR_NONE,
      CONNECTOR_NONE,
      CONNECTOR_NONE,
      CONNECTOR_DISPLAY_PORT,
    };

static void
rhdAtomParseI2CRecord(atomBiosHandlePtr handle,
			ATOM_I2C_RECORD *Record, int *ddc_line)
{
    ErrorF(" %s:  I2C Record: %s[%x] EngineID: %x I2CAddr: %x\n",
	     __func__,
	     Record->sucI2cId.bfHW_Capable ? "HW_Line" : "GPIO_ID",
	     Record->sucI2cId.bfI2C_LineMux,
	     Record->sucI2cId.bfHW_EngineID,
	     Record->ucI2CAddr);

    if (!*(unsigned char *)&(Record->sucI2cId))
	*ddc_line = 0;
    else {
	if (Record->ucI2CAddr != 0)
	    return;

	if (Record->sucI2cId.bfHW_Capable) {
	    switch(Record->sucI2cId.bfI2C_LineMux) {
	    case 0: *ddc_line = 0x7e40; break;
	    case 1: *ddc_line = 0x7e50; break;
	    case 2: *ddc_line = 0x7e30; break;
	    default: break;
	    }
	    return;
	} else {
	    /* add GPIO pin parsing */
	}
    }
}

static RADEONI2CBusRec
RADEONLookupGPIOLineForDDC(ScrnInfoPtr pScrn, uint8_t id)
{
    RADEONInfoPtr info = RADEONPTR (pScrn);
    atomDataTablesPtr atomDataPtr;
    ATOM_GPIO_I2C_ASSIGMENT gpio;
    RADEONI2CBusRec i2c;
    uint8_t crev, frev;

    memset(&i2c, 0, sizeof(RADEONI2CBusRec));
    i2c.valid = FALSE;

    atomDataPtr = info->atomBIOS->atomDataPtr;

    if (!rhdAtomGetTableRevisionAndSize(
	    &(atomDataPtr->GPIO_I2C_Info->sHeader),
	    &crev,&frev,NULL)) {
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "No GPIO Info Table found!\n");
	return i2c;
    }

    gpio = atomDataPtr->GPIO_I2C_Info->asGPIO_Info[id];
    i2c.mask_clk_reg = le16_to_cpu(gpio.usClkMaskRegisterIndex) * 4;
    i2c.mask_data_reg = le16_to_cpu(gpio.usDataMaskRegisterIndex) * 4;
    i2c.put_clk_reg = le16_to_cpu(gpio.usClkEnRegisterIndex) * 4;
    i2c.put_data_reg = le16_to_cpu(gpio.usDataEnRegisterIndex) * 4;
    i2c.get_clk_reg = le16_to_cpu(gpio.usClkY_RegisterIndex) * 4;
    i2c.get_data_reg = le16_to_cpu(gpio.usDataY_RegisterIndex) * 4;
    i2c.mask_clk_mask = (1 << gpio.ucClkMaskShift);
    i2c.mask_data_mask = (1 << gpio.ucDataMaskShift);
    i2c.put_clk_mask = (1 << gpio.ucClkEnShift);
    i2c.put_data_mask = (1 << gpio.ucDataEnShift);
    i2c.get_clk_mask = (1 << gpio.ucClkY_Shift);
    i2c.get_data_mask = (1 <<  gpio.ucDataY_Shift);
    i2c.valid = TRUE;

#if 0
    ErrorF("mask_clk_reg: 0x%x\n", gpio.usClkMaskRegisterIndex * 4);
    ErrorF("mask_data_reg: 0x%x\n", gpio.usDataMaskRegisterIndex * 4);
    ErrorF("put_clk_reg: 0x%x\n", gpio.usClkEnRegisterIndex * 4);
    ErrorF("put_data_reg: 0x%x\n", gpio.usDataEnRegisterIndex * 4);
    ErrorF("get_clk_reg: 0x%x\n", gpio.usClkY_RegisterIndex * 4);
    ErrorF("get_data_reg: 0x%x\n", gpio.usDataY_RegisterIndex * 4);
    ErrorF("other_clk_reg: 0x%x\n", gpio.usClkA_RegisterIndex * 4);
    ErrorF("other_data_reg: 0x%x\n", gpio.usDataA_RegisterIndex * 4);
    ErrorF("mask_clk_mask: %d\n", gpio.ucClkMaskShift);
    ErrorF("mask_data_mask: %d\n", gpio.ucDataMaskShift);
    ErrorF("put_clk_mask: %d\n", gpio.ucClkEnShift);
    ErrorF("put_data_mask: %d\n", gpio.ucDataEnShift);
    ErrorF("get_clk_mask: %d\n", gpio.ucClkY_Shift);
    ErrorF("get_data_mask: %d\n", gpio.ucDataY_Shift);
    ErrorF("other_clk_mask: %d\n", gpio.ucClkA_Shift);
    ErrorF("other_data_mask: %d\n", gpio.ucDataA_Shift);
#endif

    return i2c;
}

Bool
RADEONGetATOMConnectorInfoFromBIOSObject (ScrnInfoPtr pScrn)
{
    RADEONInfoPtr info = RADEONPTR (pScrn);
    uint8_t crev, frev;
    unsigned short size;
    atomDataTablesPtr atomDataPtr;
    ATOM_CONNECTOR_OBJECT_TABLE *con_obj;
    ATOM_INTEGRATED_SYSTEM_INFO_V2 *igp_obj = NULL;
    int i, j, ddc_line = 0;

    atomDataPtr = info->atomBIOS->atomDataPtr;
    if (!rhdAtomGetTableRevisionAndSize((ATOM_COMMON_TABLE_HEADER *)(atomDataPtr->Object_Header), &crev, &frev, &size))
	return FALSE;

    if (crev < 2)
	return FALSE;

    con_obj = (ATOM_CONNECTOR_OBJECT_TABLE *)
	((char *)&atomDataPtr->Object_Header->sHeader +
	 le16_to_cpu(atomDataPtr->Object_Header->usConnectorObjectTableOffset));

    for (i = 0; i < con_obj->ucNumberOfObjects; i++) {
	ATOM_SRC_DST_TABLE_FOR_ONE_OBJECT *SrcDstTable;
	ATOM_COMMON_RECORD_HEADER *Record;
	uint8_t obj_id, num, obj_type;
	int record_base;
	uint16_t con_obj_id = le16_to_cpu(con_obj->asObjects[i].usObjectID);

	obj_id = (con_obj_id & OBJECT_ID_MASK) >> OBJECT_ID_SHIFT;
	num = (con_obj_id & ENUM_ID_MASK) >> ENUM_ID_SHIFT;
	obj_type = (con_obj_id & OBJECT_TYPE_MASK) >> OBJECT_TYPE_SHIFT;
	if (obj_type != GRAPH_OBJECT_TYPE_CONNECTOR)
	    continue;

	SrcDstTable = (ATOM_SRC_DST_TABLE_FOR_ONE_OBJECT *)
	    ((char *)&atomDataPtr->Object_Header->sHeader
	     + le16_to_cpu(con_obj->asObjects[i].usSrcDstTableOffset));

	ErrorF("object id %04x %02x\n", obj_id, SrcDstTable->ucNumberOfSrc);

	if ((info->ChipFamily == CHIP_FAMILY_RS780) &&
	    (obj_id == CONNECTOR_OBJECT_ID_PCIE_CONNECTOR)) {
	    uint32_t slot_config, ct;

	    igp_obj = info->atomBIOS->atomDataPtr->IntegratedSystemInfo.IntegratedSystemInfo_v2;

	    if (!igp_obj)
		info->BiosConnector[i].ConnectorType = object_connector_convert[obj_id];
	    else {
		if (num == 1)
		    slot_config = igp_obj->ulDDISlot1Config;
		else
		    slot_config = igp_obj->ulDDISlot2Config;

		ct = (slot_config  >> 16) & 0xff;
		info->BiosConnector[i].ConnectorType = object_connector_convert[ct];
		info->BiosConnector[i].igp_lane_info = slot_config & 0xffff;
	    }
	} else
	    info->BiosConnector[i].ConnectorType = object_connector_convert[obj_id];

	if (info->BiosConnector[i].ConnectorType == CONNECTOR_NONE)
	    info->BiosConnector[i].valid = FALSE;
	else
	    info->BiosConnector[i].valid = TRUE;
	info->BiosConnector[i].devices = 0;

	for (j = 0; j < SrcDstTable->ucNumberOfSrc; j++) {
	    uint8_t sobj_id;

	    sobj_id = (SrcDstTable->usSrcObjectID[j] & OBJECT_ID_MASK) >> OBJECT_ID_SHIFT;
	    ErrorF("src object id %04x %d\n", SrcDstTable->usSrcObjectID[j], sobj_id);

	    switch(sobj_id) {
	    case ENCODER_OBJECT_ID_INTERNAL_LVDS:
		info->BiosConnector[i].devices |= (1 << ATOM_DEVICE_LCD1_INDEX);
		break;
	    case ENCODER_OBJECT_ID_INTERNAL_TMDS1:
	    case ENCODER_OBJECT_ID_INTERNAL_KLDSCP_TMDS1:
		info->BiosConnector[i].devices |= (1 << ATOM_DEVICE_DFP1_INDEX);
		info->BiosConnector[i].TMDSType = TMDS_INT;
		break;
	    case ENCODER_OBJECT_ID_INTERNAL_UNIPHY:
		if (num == 1)
		    info->BiosConnector[i].devices |= (1 << ATOM_DEVICE_DFP1_INDEX);
		else
		    info->BiosConnector[i].devices |= (1 << ATOM_DEVICE_DFP2_INDEX);
		info->BiosConnector[i].TMDSType = TMDS_UNIPHY;
		break;
	    case ENCODER_OBJECT_ID_INTERNAL_TMDS2:
	    case ENCODER_OBJECT_ID_INTERNAL_KLDSCP_DVO1:
		info->BiosConnector[i].devices |= (1 << ATOM_DEVICE_DFP2_INDEX);
		info->BiosConnector[i].TMDSType = TMDS_EXT;
		break;
	    case ENCODER_OBJECT_ID_INTERNAL_LVTM1:
	    case ENCODER_OBJECT_ID_INTERNAL_KLDSCP_LVTMA:
		info->BiosConnector[i].devices |= (1 << ATOM_DEVICE_DFP3_INDEX);
		info->BiosConnector[i].TMDSType = TMDS_LVTMA;
		break;
	    case ENCODER_OBJECT_ID_INTERNAL_DAC1:
	    case ENCODER_OBJECT_ID_INTERNAL_KLDSCP_DAC1:
		if (info->BiosConnector[i].ConnectorType == CONNECTOR_DIN ||
		    info->BiosConnector[i].ConnectorType == CONNECTOR_STV ||
		    info->BiosConnector[i].ConnectorType == CONNECTOR_CTV)
		    //info->BiosConnector[i].devices |= (1 << ATOM_DEVICE_TV1_INDEX);
		    info->BiosConnector[i].valid = FALSE;
		else
		    info->BiosConnector[i].devices |= (1 << ATOM_DEVICE_CRT1_INDEX);
		info->BiosConnector[i].DACType = DAC_PRIMARY;
		break;
	    case ENCODER_OBJECT_ID_INTERNAL_DAC2:
	    case ENCODER_OBJECT_ID_INTERNAL_KLDSCP_DAC2:
		if (info->BiosConnector[i].ConnectorType == CONNECTOR_DIN ||
		    info->BiosConnector[i].ConnectorType == CONNECTOR_STV ||
		    info->BiosConnector[i].ConnectorType == CONNECTOR_CTV)
		    //info->BiosConnector[i].devices |= (1 << ATOM_DEVICE_TV1_INDEX);
		    info->BiosConnector[i].valid = FALSE;
		else
		    info->BiosConnector[i].devices |= (1 << ATOM_DEVICE_CRT2_INDEX);
		info->BiosConnector[i].DACType = DAC_TVDAC;
		break;
	    }
	}

	Record = (ATOM_COMMON_RECORD_HEADER *)
	    ((char *)&atomDataPtr->Object_Header->sHeader
	     + le16_to_cpu(con_obj->asObjects[i].usRecordOffset));

	record_base = le16_to_cpu(con_obj->asObjects[i].usRecordOffset);

	while (Record->ucRecordType > 0
	       && Record->ucRecordType <= ATOM_MAX_OBJECT_RECORD_NUMBER ) {

	    ErrorF("record type %d\n", Record->ucRecordType);
	    switch (Record->ucRecordType) {
		case ATOM_I2C_RECORD_TYPE:
		    rhdAtomParseI2CRecord(info->atomBIOS,
					  (ATOM_I2C_RECORD *)Record,
					  &ddc_line);
		    info->BiosConnector[i].ddc_i2c = atom_setup_i2c_bus(ddc_line);
		    break;
		case ATOM_HPD_INT_RECORD_TYPE:
		    break;
		case ATOM_CONNECTOR_DEVICE_TAG_RECORD_TYPE:
		    break;
	    }

	    Record = (ATOM_COMMON_RECORD_HEADER*)
		((char *)Record + Record->ucRecordSize);
	}
    }
    return TRUE;
}

Bool
RADEONGetATOMTVInfo(xf86OutputPtr output)
{
    ScrnInfoPtr pScrn = output->scrn;
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    ATOM_ANALOG_TV_INFO *tv_info;

    tv_info = info->atomBIOS->atomDataPtr->AnalogTV_Info;

    if (!tv_info)
	return FALSE;

    switch(tv_info->ucTV_BootUpDefaultStandard) {
    case NTSC_SUPPORT:
	radeon_output->default_tvStd = TV_STD_NTSC;
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Default TV standard: NTSC\n");
	break;
    case NTSCJ_SUPPORT:
	radeon_output->default_tvStd = TV_STD_NTSC_J;
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Default TV standard: NTSC-J\n");
	break;
    case PAL_SUPPORT:
	radeon_output->default_tvStd = TV_STD_PAL;
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Default TV standard: PAL\n");
	break;
    case PALM_SUPPORT:
	radeon_output->default_tvStd = TV_STD_PAL_M;
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Default TV standard: PAL-M\n");
	break;
    case PAL60_SUPPORT:
	radeon_output->default_tvStd = TV_STD_PAL_60;
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Default TV standard: PAL-60\n");
	break;
    }

    radeon_output->tvStd = radeon_output->default_tvStd;

    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "TV standards supported by chip: ");
    radeon_output->SupportedTVStds = radeon_output->default_tvStd;
    if (tv_info->ucTV_SupportedStandard & NTSC_SUPPORT) {
	ErrorF("NTSC ");
	radeon_output->SupportedTVStds |= TV_STD_NTSC;
    }
    if (tv_info->ucTV_SupportedStandard & NTSCJ_SUPPORT) {
	ErrorF("NTSC-J ");
	radeon_output->SupportedTVStds |= TV_STD_NTSC_J;
    }
    if (tv_info->ucTV_SupportedStandard & PAL_SUPPORT) {
	ErrorF("PAL ");
	radeon_output->SupportedTVStds |= TV_STD_PAL;
    }
    if (tv_info->ucTV_SupportedStandard & PALM_SUPPORT) {
	ErrorF("PAL-M ");
	radeon_output->SupportedTVStds |= TV_STD_PAL_M;
    }
    if (tv_info->ucTV_SupportedStandard & PAL60_SUPPORT) {
	ErrorF("PAL-60 ");
	radeon_output->SupportedTVStds |= TV_STD_PAL_60;
    }
    ErrorF("\n");

    if (tv_info->ucExt_TV_ASIC_ID) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Unknown external TV ASIC\n");
	return FALSE;
    }

    return TRUE;
}

Bool
RADEONATOMGetTVTimings(ScrnInfoPtr pScrn, int index, SET_CRTC_TIMING_PARAMETERS_PS_ALLOCATION *crtc_timing, int32_t *pixel_clock)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    ATOM_ANALOG_TV_INFO *tv_info;

    tv_info = info->atomBIOS->atomDataPtr->AnalogTV_Info;

    if (index > MAX_SUPPORTED_TV_TIMING)
	return FALSE;

    crtc_timing->usH_Total = le16_to_cpu(tv_info->aModeTimings[index].usCRTC_H_Total);
    crtc_timing->usH_Disp = le16_to_cpu(tv_info->aModeTimings[index].usCRTC_H_Disp);
    crtc_timing->usH_SyncStart = le16_to_cpu(tv_info->aModeTimings[index].usCRTC_H_SyncStart);
    crtc_timing->usH_SyncWidth = le16_to_cpu(tv_info->aModeTimings[index].usCRTC_H_SyncWidth);

    crtc_timing->usV_Total = le16_to_cpu(tv_info->aModeTimings[index].usCRTC_V_Total);
    crtc_timing->usV_Disp = le16_to_cpu(tv_info->aModeTimings[index].usCRTC_V_Disp);
    crtc_timing->usV_SyncStart = le16_to_cpu(tv_info->aModeTimings[index].usCRTC_V_SyncStart);
    crtc_timing->usV_SyncWidth = le16_to_cpu(tv_info->aModeTimings[index].usCRTC_V_SyncWidth);

    crtc_timing->susModeMiscInfo = tv_info->aModeTimings[index].susModeMiscInfo;

    crtc_timing->ucOverscanRight = le16_to_cpu(tv_info->aModeTimings[index].usCRTC_OverscanRight);
    crtc_timing->ucOverscanLeft = le16_to_cpu(tv_info->aModeTimings[index].usCRTC_OverscanLeft);
    crtc_timing->ucOverscanBottom = le16_to_cpu(tv_info->aModeTimings[index].usCRTC_OverscanBottom);
    crtc_timing->ucOverscanTop = le16_to_cpu(tv_info->aModeTimings[index].usCRTC_OverscanTop);
    *pixel_clock = le16_to_cpu(tv_info->aModeTimings[index].usPixelClock) * 10;

    return TRUE;
}

static void RADEONApplyATOMQuirks(ScrnInfoPtr pScrn, int index)
{
    RADEONInfoPtr info = RADEONPTR (pScrn);

    /* Asus M2A-VM HDMI board lists the DVI port as HDMI */
    if ((info->Chipset == PCI_CHIP_RS690_791E) &&
	(PCI_SUB_VENDOR_ID(info->PciInfo) == 0x1043) &&
	(PCI_SUB_DEVICE_ID(info->PciInfo) == 0x826d)) {
	if ((info->BiosConnector[index].ConnectorType == CONNECTOR_HDMI_TYPE_A) &&
	    (info->BiosConnector[index].TMDSType == TMDS_LVTMA)) {
	    info->BiosConnector[index].ConnectorType = CONNECTOR_DVI_D;
	}
    }

}

Bool
RADEONGetATOMConnectorInfoFromBIOSConnectorTable (ScrnInfoPtr pScrn)
{
    RADEONInfoPtr info = RADEONPTR (pScrn);
    atomDataTablesPtr atomDataPtr;
    uint8_t crev, frev;
    int i, j;

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

#if 1
	if (i == ATOM_DEVICE_CV_INDEX) {
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Skipping Component Video\n");
	    info->BiosConnector[i].valid = FALSE;
	    continue;
	}
#endif
#if 1
	if (i == ATOM_DEVICE_TV1_INDEX) {
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Skipping TV-Out\n");
	    info->BiosConnector[i].valid = FALSE;
	    continue;
	}
#endif

	info->BiosConnector[i].valid = TRUE;
	info->BiosConnector[i].output_id = ci.sucI2cId.sbfAccess.bfI2C_LineMux;
	info->BiosConnector[i].devices = (1 << i);
	info->BiosConnector[i].ConnectorType = ci.sucConnectorInfo.sbfAccess.bfConnectorType;

	if (info->BiosConnector[i].ConnectorType == CONNECTOR_NONE) {
	    info->BiosConnector[i].valid = FALSE;
	    continue;
	}

	info->BiosConnector[i].DACType = ci.sucConnectorInfo.sbfAccess.bfAssociatedDAC;

	/* don't assign a gpio for tv */
	if ((i == ATOM_DEVICE_TV1_INDEX) ||
	    (i == ATOM_DEVICE_TV2_INDEX) ||
	    (i == ATOM_DEVICE_CV_INDEX))
	    info->BiosConnector[i].ddc_i2c.valid = FALSE;
	else if ((info->ChipFamily == CHIP_FAMILY_RS600) ||
		 (info->ChipFamily == CHIP_FAMILY_RS690) ||
		 (info->ChipFamily == CHIP_FAMILY_RS740)) {
	    /* IGP DFP ports use non-standard gpio entries */
	    if ((i == ATOM_DEVICE_DFP2_INDEX) || (i == ATOM_DEVICE_DFP3_INDEX))
		info->BiosConnector[i].ddc_i2c =
		    RADEONLookupGPIOLineForDDC(pScrn, ci.sucI2cId.sbfAccess.bfI2C_LineMux + 1);
	    else
		info->BiosConnector[i].ddc_i2c =
		    RADEONLookupGPIOLineForDDC(pScrn, ci.sucI2cId.sbfAccess.bfI2C_LineMux);
	} else
	    info->BiosConnector[i].ddc_i2c =
		RADEONLookupGPIOLineForDDC(pScrn, ci.sucI2cId.sbfAccess.bfI2C_LineMux);

	if (i == ATOM_DEVICE_DFP1_INDEX)
	    info->BiosConnector[i].TMDSType = TMDS_INT;
	else if (i == ATOM_DEVICE_DFP2_INDEX) {
	    if ((info->ChipFamily == CHIP_FAMILY_RS600) ||
		(info->ChipFamily == CHIP_FAMILY_RS690) ||
		(info->ChipFamily == CHIP_FAMILY_RS740))
		info->BiosConnector[i].TMDSType = TMDS_DDIA;
	    else
		info->BiosConnector[i].TMDSType = TMDS_EXT;
	} else if (i == ATOM_DEVICE_DFP3_INDEX)
	    info->BiosConnector[i].TMDSType = TMDS_LVTMA;
	else
	    info->BiosConnector[i].TMDSType = TMDS_NONE;

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
	} else {
	    info->BiosConnector[i].hpd_mask = 0;
	}

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
			    ((j == ATOM_DEVICE_CRT1_INDEX) || (j == ATOM_DEVICE_CRT2_INDEX))) {
			    info->BiosConnector[i].DACType = info->BiosConnector[j].DACType;
			    info->BiosConnector[i].devices |= info->BiosConnector[j].devices;
			    info->BiosConnector[j].valid = FALSE;
			} else if (((j == ATOM_DEVICE_DFP1_INDEX) ||
			     (j == ATOM_DEVICE_DFP2_INDEX) ||
			     (j == ATOM_DEVICE_DFP3_INDEX)) &&
			    ((i == ATOM_DEVICE_CRT1_INDEX) || (i == ATOM_DEVICE_CRT2_INDEX))) {
			    info->BiosConnector[j].DACType = info->BiosConnector[i].DACType;
			    info->BiosConnector[j].devices |= info->BiosConnector[i].devices;
			    info->BiosConnector[i].valid = FALSE;
			}
			/* other possible combos?  */
		    }
		}
	    }
	}
    }

    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Bios Connector table: \n");
    for (i = 0; i < ATOM_MAX_SUPPORTED_DEVICE; i++) {
	if (info->BiosConnector[i].valid) {
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Port%d: DDCType-0x%x, DACType-%d, TMDSType-%d, ConnectorType-%d, hpd_mask-0x%x\n",
		       i, (unsigned int)info->BiosConnector[i].ddc_i2c.mask_clk_reg, info->BiosConnector[i].DACType,
		       info->BiosConnector[i].TMDSType, info->BiosConnector[i].ConnectorType,
		       info->BiosConnector[i].hpd_mask);
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
    CAILFUNC(CAIL);

    return malloc(size);
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
    RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);
    unsigned char *RADEONMMIO = pRADEONEnt->MMIO;
    UINT32 ret;
    CAILFUNC(CAIL);

    ret  =  INREG(idx << 2);
    /*DEBUGP(ErrorF("%s(%x) = %x\n",__func__,idx << 2,ret));*/
    return ret;
}

VOID
CailWriteATIRegister(VOID *CAIL, UINT32 idx, UINT32 data)
{
    ScrnInfoPtr pScrn = xf86Screens[((atomBiosHandlePtr)CAIL)->scrnIndex];
    RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);
    unsigned char *RADEONMMIO = pRADEONEnt->MMIO;
    CAILFUNC(CAIL);

    OUTREG(idx << 2,data);
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


#endif /* ATOM_BIOS */
